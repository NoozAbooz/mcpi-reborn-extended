#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#include <libreborn/libreborn.h>

#include "bootstrap.h"
#include "ldconfig.h"

// Set Environmental Variable
#define PRESERVE_ENVIRONMENTAL_VARIABLE(name) \
    { \
        char *original_env_value = getenv(name); \
        if (original_env_value != NULL) { \
            setenv("ORIGINAL_" name, original_env_value, 1); \
        } \
    }
static void trim(char **value) {
    // Remove Trailing Colon
    int length = strlen(*value);
    if ((*value)[length - 1] == ':') {
        (*value)[length - 1] = '\0';
    }
    if ((*value)[0] == ':') {
        *value = &(*value)[1];
    }
}
void set_and_print_env(const char *name, char *value) {
    // Set Variable With No Trailing Colon
    static const char *unmodified_name_prefix = "MCPI_";
    if (strncmp(unmodified_name_prefix, name, strlen(unmodified_name_prefix)) != 0) {
        trim(&value);
    }

    // Print New Value
    DEBUG("Set %s = %s", name, value);

    // Set The Value
    setenv(name, value, 1);
}
#ifndef __ARM_ARCH
#define PASS_ENVIRONMENTAL_VARIABLE_TO_QEMU(name) \
    { \
        char *old_value = getenv("QEMU_SET_ENV"); \
        char *new_value = NULL; \
        /* Pass Variable */ \
        safe_asprintf(&new_value, "%s%s%s=%s", old_value == NULL ? "" : old_value, old_value == NULL ? "" : ",", name, getenv(name)); \
        setenv("QEMU_SET_ENV", new_value, 1); \
        free(new_value); \
        /* Reset Variable */ \
        RESET_ENVIRONMENTAL_VARIABLE(name); \
    }
#endif

// Get Environmental Variable
static char *get_env_safe(const char *name) {
    // Get Variable Or Blank String If Not Set
    char *ret = getenv(name);
    return ret != NULL ? ret : "";
}

// Get All Mods In Folder
static void load(char **ld_preload, char *folder) {
    int folder_name_length = strlen(folder);
    // Retry Until Successful
    while (1) {
        // Open Folder
        DIR *dp = opendir(folder);
        if (dp != NULL) {
            // Loop Through Folder
            struct dirent *entry = NULL;
            errno = 0;
            while (1) {
                errno = 0;
                entry = readdir(dp);
                if (entry != NULL) {
                    // Check If File Is Regular
                    if (entry->d_type == DT_REG) {
                        // Get Full Name
                        int name_length = strlen(entry->d_name);
                        int total_length = folder_name_length + name_length;
                        char name[total_length + 1];

                        // Concatenate Folder Name And File Name
                        for (int i = 0; i < folder_name_length; i++) {
                            name[i] = folder[i];
                        }
                        for (int i = 0; i < name_length; i++) {
                            name[folder_name_length + i] = entry->d_name[i];
                        }
                        // Add Terminator
                        name[total_length] = '\0';

                        // Check If File Is Executable
                        int result = access(name, R_OK);
                        if (result == 0) {
                            // Add To LD_PRELOAD
                            string_append(ld_preload, ":%s", name);
                        } else if (result == -1 && errno != 0) {
                            // Fail
                            INFO("Unable To Acesss: %s: %s", name, strerror(errno));
                            errno = 0;
                        }
                    }
                } else if (errno != 0) {
                    // Error Reading Contents Of Folder
                    ERR("Error Reading Directory: %s: %s", folder, strerror(errno));
                } else {
                    // Done!
                    break;
                }
            }
            // Close Folder
            closedir(dp);

            // Exit Function
            return;
        } else if (errno == ENOENT) {
            // Folder Doesn't Exists, Attempt Creation
            int ret = mkdir(folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (ret != 0) {
                // Unable To Create Folder
                ERR("Error Creating Directory: %s: %s", folder, strerror(errno));
            }
            // Continue Retrying
        } else {
            // Unable To Open Folder
            ERR("Error Opening Directory: %s: %s", folder, strerror(errno));
        }
    }
}

#define MCPI_BINARY "minecraft-pi"
#define QEMU_BINARY "qemu-arm"

// Pre-Bootstrap
void pre_bootstrap() {
    // AppImage
#ifdef MCPI_IS_APPIMAGE_BUILD
    char *owd = getenv("OWD");
    if (owd != NULL && chdir(owd) != 0) {
        ERR("AppImage: Unable To Fix Current Directory: %s", strerror(errno));
    }
#endif

    // Get Binary Directory
    char *binary_directory = get_binary_directory();

    // Configure PATH
    {
        // Add Library Directory
        char *new_path;
        safe_asprintf(&new_path, "%s/bin", binary_directory);
        // Add Existing PATH
        {
            char *value = get_env_safe("PATH");
            if (strlen(value) > 0) {
                string_append(&new_path, ":%s", value);
            }
        }
        // Set And Free
        set_and_print_env("PATH", new_path);
        free(new_path);
    }

    // Free Binary Directory
    free(binary_directory);
}

// Bootstrap
void bootstrap(int argc, char *argv[]) {
    INFO("%s", "Configuring Game...");

    // Get Binary Directory
    char *binary_directory = get_binary_directory();

    // Handle AppImage
    char *usr_prefix = NULL;
#ifdef MCPI_IS_APPIMAGE_BUILD
    usr_prefix = getenv("APPDIR");
#endif
    if (usr_prefix == NULL) {
        usr_prefix = "";
    }

    // Configure LD_LIBRARY_PATH
    {
        // Preserve
        PRESERVE_ENVIRONMENTAL_VARIABLE("LD_LIBRARY_PATH");
        char *new_ld_path = NULL;

        // Add Library Directory
        safe_asprintf(&new_ld_path, "%s/lib", binary_directory);

        // Add MCPI_LD_LIBRARY_PATH
        {
            char *value = get_env_safe("MCPI_LD_LIBRARY_PATH");
            if (strlen(value) > 0) {
                string_append(&new_ld_path, ":%s", value);
            }
        }

        // Add LD_LIBRARY_PATH (ARM32 Only)
#ifdef __arm__
        {
            char *value = get_env_safe("LD_LIBRARY_PATH");
            if (strlen(value) > 0) {
                string_append(&new_ld_path, ":%s", value);
            }
        }
#endif

        // Load ARM Libraries (Ensure Priority)
        string_append(&new_ld_path, ":%s/usr/lib/arm-linux-gnueabihf:%s/usr/arm-linux-gnueabihf/lib", usr_prefix, usr_prefix);

        // Add Full Library Search Path
        {
            char *value = get_full_library_search_path();
            if (strlen(value) > 0) {
                string_append(&new_ld_path, ":%s", value);
            }
            free(value);
        }

        // Add Fallback Library Directory
        string_append(&new_ld_path, ":%s/fallback-lib", binary_directory);

        // Set And Free
        set_and_print_env("LD_LIBRARY_PATH", new_ld_path);
        free(new_ld_path);
    }

    // Configure LD_PRELOAD
    {
        // Preserve
        PRESERVE_ENVIRONMENTAL_VARIABLE("LD_PRELOAD");
        char *new_ld_preload = NULL;

        // ~/.minecraft-pi/mods
        {
            // Get Mods Folder
            char *mods_folder = NULL;
            safe_asprintf(&mods_folder, "%s/mods/", binary_directory);
            // Load Mods From ./mods
            load(&new_ld_preload, mods_folder);
            // Free Mods Folder
            free(mods_folder);
        }

        // Built-In Mods
        {
            // Get Mods Folder
            char *mods_folder = NULL;
            safe_asprintf(&mods_folder, "%s/mods/", binary_directory);
            // Load Mods From ./mods
            load(&new_ld_preload, mods_folder);
            // Free Mods Folder
            free(mods_folder);
        }

        // Add MCPI_LD_PRELOAD
        {
            char *value = get_env_safe("MCPI_LD_PRELOAD");
            if (strlen(value) > 0) {
                string_append(&new_ld_preload, ":%s", value);
            }
        }

        // Add LD_PRELOAD (ARM32 Only)
#ifdef __arm__
        {
            char *value = get_env_safe("MCPI_LD_PRELOAD");
            if (strlen(value) > 0) {
                string_append(&new_ld_preload, ":%s", value);
            }
        }
#endif

        // Set LD_PRELOAD
        set_and_print_env("LD_PRELOAD", new_ld_preload);
        free(new_ld_preload);
    }

    // Resolve Binary Path & Set MCPI_DIRECTORY
    {
        // Resolve Full Binary Path
        char *full_path = NULL;
        safe_asprintf(&full_path, "%s/" MCPI_BINARY, binary_directory);
        char *resolved_path = realpath(full_path, NULL);
        ALLOC_CHECK(resolved_path);
        free(full_path);

        // Set MCPI_EXECUTABLE_PATH
        set_and_print_env("MCPI_EXECUTABLE_PATH", resolved_path);

        // Set MCPI_DIRECTORY
        chop_last_component(&resolved_path);
        set_and_print_env("MCPI_DIRECTORY", resolved_path);
        free(resolved_path);
    }

    // Free Binary Directory
    free(binary_directory);

    // Start Game
    INFO("%s", "Starting Game...");

    // Arguments
    int argv_start = 2; // argv = &new_args[argv_start]
    const char *new_args[argv_start /* 2 Potential Prefix Arguments (QEMU And Linker) */ + argc + 1 /* NULL-Terminator */]; //

    // Copy Existing Arguments
    for (int i = 1; i < argc; i++) {
        new_args[i + argv_start] = argv[i];
    }
    // NULL-Terminator
    new_args[argv_start + argc] = NULL;

    // Set Executable Argument
    new_args[argv_start] = getenv("MCPI_EXECUTABLE_PATH");

    // Non-ARM32 Systems Need Manually Specified Linker
#ifndef __arm__
    argv_start--;
    char *linker = NULL;
    safe_asprintf(&linker, "%s/usr/arm-linux-gnueabihf/lib/ld-linux-armhf.so.3", usr_prefix);
    new_args[argv_start] = linker;

    // Non-ARM Systems Need QEMU
#ifndef __ARM_ARCH
    argv_start--;
    new_args[argv_start] = QEMU_BINARY;

    // Prevent QEMU From Being Modded
    PASS_ENVIRONMENTAL_VARIABLE_TO_QEMU("LD_LIBRARY_PATH");
    PASS_ENVIRONMENTAL_VARIABLE_TO_QEMU("LD_PRELOAD");
#endif
#endif

    // Run
    const char **new_argv = &new_args[argv_start];
    safe_execvpe(new_argv, (const char *const *) environ);
}
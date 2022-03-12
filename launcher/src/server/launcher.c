#include "../bootstrap.h"

#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Pre-Bootstrap
    pre_bootstrap();

    // Set Home To Current Directory, So World Data Is Stored There
    char *launch_directory = getcwd(NULL, 0);
    set_and_print_env("HOME", launch_directory);
    free(launch_directory);

    // Bootstrap
    bootstrap(argc, argv);
}

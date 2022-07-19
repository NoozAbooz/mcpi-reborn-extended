#include <cstdlib>
#include <stdio.h>
#include <sys/socket.h>

#include <libreborn/libreborn.h>
#include <symbols/minecraft.h>

#include <mods/init/init.h>
#include <mods/misc/misc.h>
#include <mods/chest/chest.h>

// Send API Command
static void send_api_command(unsigned char *minecraft, char *str) {
    struct ConnectedClient client;
    client.sock = -1;
    client.str = "";
    client.time = 0;
    unsigned char *command_server = *(unsigned char **) (minecraft + Minecraft_command_server_property_offset);
    if (command_server != NULL) {
        (*CommandServer_parse)(command_server, client, str);
    }
}
static void send_api_chat_command(unsigned char *minecraft, char *str) {
    char *command = NULL;
    safe_asprintf(&command, "chat.post(%s)\n", str);
    send_api_command(minecraft, command);
    free(command);
}

// Cursed Chest Activation Handler
static int32_t CursedChest_use(__attribute__((unused)) unsigned char *tile, __attribute__((unused)) unsigned char *level, int32_t x, int32_t y, int32_t z, __attribute__((unused)) unsigned char *player) {
    
    // Black Magic To Make API Work
    unsigned char is_server_player = *(unsigned char *) (player + Player_is_server_property_offset);
    
    if (!is_server_player) {
        unsigned char *minecraft = *(unsigned char **) (player + LocalPlayer_minecraft_property_offset);

        // Print Coords To Chat
        char *str = NULL;
        safe_asprintf(&str, "Cursed Chest Location: %i %i %i", x, y, z);
        send_api_chat_command(minecraft, str);
        free(str);
    }
    
    // Log To Console
    INFO("Cursed Chest Location: %i %i %i", x, y, z);

    return 1;
}

// Cursed Chest Activation VTable
static unsigned char *get_cursed_chest_vtable() {
    static unsigned char *vtable = NULL;
    if (vtable == NULL) {
        // Init
        vtable = (unsigned char *) malloc(TILE_VTABLE_SIZE);
        ALLOC_CHECK(vtable);
        // Copy Old VTable
        memcpy((void *) vtable, (void *) Tile_vtable, TILE_VTABLE_SIZE);

        // Modify
        *(Tile_use_t *) (vtable + Tile_use_vtable_offset) = CursedChest_use;
    }
    return vtable;
}
__attribute__((destructor)) static void free_cursed_chest_vtable() {
    free(get_cursed_chest_vtable());
}

// Add Block
unsigned char *cursed_chest = NULL;
void Tile_initTiles_injection(__attribute__((unused)) unsigned char *obj) {
    // Allocate
    cursed_chest = (unsigned char *) ::operator new(TILE_SIZE);
    ALLOC_CHECK(cursed_chest);

    // Construct
    (*Tile)(cursed_chest, 19, 25, *Material_wood);

    // Set VTable
    *(unsigned char **) cursed_chest = get_cursed_chest_vtable();
    unsigned char *cursed_chest_vtable = *(unsigned char **) cursed_chest;

    // Get Functions
    Tile_setDestroyTime_t Tile_setDestroyTime = *(Tile_setDestroyTime_t *) (cursed_chest_vtable + Tile_setDestroyTime_vtable_offset);
    Tile_setExplodeable_t Tile_setExplodeable = *(Tile_setExplodeable_t *) (cursed_chest_vtable + Tile_setExplodeable_vtable_offset);
    Tile_setSoundType_t Tile_setSoundType = *(Tile_setSoundType_t *) (cursed_chest_vtable + Tile_setSoundType_vtable_offset);
    Tile_setDescriptionId_t Tile_setDescriptionId = *(Tile_setDescriptionId_t *) (cursed_chest_vtable + Tile_setDescriptionId_vtable_offset);

    // Init
    (*Tile_init)(cursed_chest);
    (*Tile_setDestroyTime)(cursed_chest, 2.0f);
    (*Tile_setExplodeable)(cursed_chest, 10.0f);
    (*Tile_setSoundType)(cursed_chest, SOUND_WOOD);
    *(int32_t *) (cursed_chest + Tile_category_property_offset) = 1;
    (*Tile_setDescriptionId)(cursed_chest, "Cursed Chest");

    // Call Original Method
    (*Tile_initTiles)();
}

// Init
void init_cursed_chest() {
    // Add Cursed Chest
    misc_run_on_tiles_setup(Tile_initTiles_injection);
}
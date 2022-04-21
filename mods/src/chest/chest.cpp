#include <cstdlib>
#include <stdio.h>
#include <sys/socket.h>

#include <libreborn/libreborn.h>
#include <symbols/minecraft.h>

#include "../init/init.h"
#include "chest.h"

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

// Send API Chat Command
static void send_api_chat_command(unsigned char *minecraft, char *str) {
    char *command = NULL;
    safe_asprintf(&command, "chat.post(%s)\n", str);
    send_api_command(minecraft, command);
    free(command);
}

// Cursed Chest Features
static int32_t MyTile_use(__attribute__((unused)) unsigned char *tile, __attribute__((unused)) unsigned char *level, int32_t x, int32_t y, int32_t z, __attribute__((unused)) unsigned char *player) {
    
    // Black Magic To Make API Work
    // unsigned char is_server_player = *(unsigned char *) (player + Player_is_server_property_offset);
    
    if (!is_server_player) {
        unsigned char *minecraft = *(unsigned char **) (player + LocalPlayer_minecraft_property_offset);

        // Print Coords To Chat
        char *str = NULL;
        safe_asprintf(&str, "Cursed Chest Location: %i %i %i", x, y, z);
        send_api_chat_command(minecraft, str);
        free(str);
    }
    
    // Log To Console
    INFO("Cursed Chest location: %i %i %i", x, y, z);

    return 1;
}

// My Tile VTable
static unsigned char *get_my_tile_vtable() {
    static unsigned char *vtable = NULL;
    if (vtable == NULL) {
        // Init
        vtable = (unsigned char *) malloc(TILE_VTABLE_SIZE);
        ALLOC_CHECK(vtable);
        // Copy Old VTable
        memcpy((void *) vtable, (void *) Tile_vtable, TILE_VTABLE_SIZE);

        // Modify
        *(Tile_use_t *) (vtable + Tile_use_vtable_offset) = MyTile_use;
    }
    return vtable;
}
__attribute__((destructor)) static void free_my_tile_vtable() {
    free(get_my_tile_vtable());
}

// Add My Tile
unsigned char *my_tile = NULL;
static void Tile_initTiles_injection() {
    // Allocate
    my_tile = (unsigned char *) ::operator new(TILE_SIZE);
    ALLOC_CHECK(my_tile);

    // Construct
    (*Tile)(my_tile, 19, 27, *Material_stone);

    // Set VTable
    *(unsigned char **) my_tile = get_my_tile_vtable();
    unsigned char *my_tile_vtable = *(unsigned char **) my_tile;

    // Get Functions
    Tile_setDestroyTime_t Tile_setDestroyTime = *(Tile_setDestroyTime_t *) (my_tile_vtable + Tile_setDestroyTime_vtable_offset);
    Tile_setExplodeable_t Tile_setExplodeable = *(Tile_setExplodeable_t *) (my_tile_vtable + Tile_setExplodeable_vtable_offset);
    Tile_setSoundType_t Tile_setSoundType = *(Tile_setSoundType_t *) (my_tile_vtable + Tile_setSoundType_vtable_offset);
    Tile_setDescriptionId_t Tile_setDescriptionId = *(Tile_setDescriptionId_t *) (my_tile_vtable + Tile_setDescriptionId_vtable_offset);

    // Init
    (*Tile_init)(my_tile);
    (*Tile_setDestroyTime)(my_tile, 2.0f);
    (*Tile_setExplodeable)(my_tile, 10.0f);
    (*Tile_setSoundType)(my_tile, SOUND_STONE);
    *(int32_t *) (my_tile + Tile_category_property_offset) = 1;
    (*Tile_setDescriptionId)(my_tile, "my_tile");

    // Call Original Method
    (*Tile_initTiles)();
}

// Init
void init_my_tile() {
    // Add My Tile
    overwrite_calls((void *) Tile_initTiles, (void *) Tile_initTiles_injection);
}
#include <cstdlib>

#include <libreborn/libreborn.h>
#include <symbols/minecraft.h>

#include "../init/init.h"
#include "chest.h"

// My Tile Use
static int32_t MyTile_use(__attribute__((unused)) unsigned char *tile, __attribute__((unused)) unsigned char *level, int32_t x, int32_t y, int32_t z, __attribute__((unused)) unsigned char *player) {
    INFO("%i %i %i", x, y, z);
    return 1;
}

// My Tile VTable
static unsigned char *get_chest_vtable() {
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
__attribute__((destructor)) static void free_chest_vtable() {
    free(get_chest_vtable());
}

// Add My Tile
unsigned char *chest = NULL;
static void Tile_initTiles_injection() {
    // Allocate
    chest = (unsigned char *) ::operator new(TILE_SIZE);
    ALLOC_CHECK(chest);

    // Construct
    (*Tile)(chest, 19, 25, *Material_stone);

    // Set VTable
    *(unsigned char **) chest = get_chest_vtable();
    unsigned char *chest_vtable = *(unsigned char **) chest;

    // Get Functions
    Tile_setDestroyTime_t Tile_setDestroyTime = *(Tile_setDestroyTime_t *) (chest_vtable + Tile_setDestroyTime_vtable_offset);
    Tile_setExplodeable_t Tile_setExplodeable = *(Tile_setExplodeable_t *) (chest_vtable + Tile_setExplodeable_vtable_offset);
    Tile_setSoundType_t Tile_setSoundType = *(Tile_setSoundType_t *) (chest_vtable + Tile_setSoundType_vtable_offset);
    Tile_setDescriptionId_t Tile_setDescriptionId = *(Tile_setDescriptionId_t *) (chest_vtable + Tile_setDescriptionId_vtable_offset);

    // Init
    (*Tile_init)(chest);
    (*Tile_setDestroyTime)(chest, 2.0f);
    (*Tile_setExplodeable)(chest, 10.0f);
    (*Tile_setSoundType)(chest, SOUND_STONE);
    *(int32_t *) (chest + Tile_category_property_offset) = 1;
    (*Tile_setDescriptionId)(chest, "cursedChest");

    // Call Original Method
    (*Tile_initTiles)();
}

// Init
void init_chest() {
    // Add My Tile
    overwrite_calls((void *) Tile_initTiles, (void *) Tile_initTiles_injection);
}

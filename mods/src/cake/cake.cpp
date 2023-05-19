#include <libreborn/libreborn.h>
#include <symbols/minecraft.h>

#include <mods/misc/misc.h>
#include <mods/cake/cake.h>

constexpr float cake_len = 0.0625F;

uchar *level = NULL;
static void mcpi_callback(uchar *minecraft){
    // Get the level
    if (minecraft == NULL) return;
    level = *(uchar **) (minecraft + Minecraft_level_property_offset);
}

static std::string cake_getDescriptionId(__attribute__((unused)) uchar *tile, __attribute__((unused)) int id) {
    return "tile.cake";
}

// Textures
static int cake_getTexture2(__attribute__((unused)) uchar *tile, int face) {
    if (face == 1) {
        // Top texture
        return GET_TEXTURE_POS_IN_ATLAS(9, 7);
    } else if (face == 0) {
        // Bottom texture
        return GET_TEXTURE_POS_IN_ATLAS(12, 7);
    }
    // Side texture
    return GET_TEXTURE_POS_IN_ATLAS(10, 7);
}

static int cake_getTexture3(uchar *tile, __attribute__((unused)) uchar *_level, int x, int y, int z, int face) {
    // Eaten face
    if (face == 3) {
        int data = (*Level_getData)(level, x, y, z);
        if (data != 0 && data < 6) {
            // Sliced texture
            return GET_TEXTURE_POS_IN_ATLAS(11, 7);
        } else {
            // Normal
            return cake_getTexture2(tile, face);
        }
    }
    // Normal
    return cake_getTexture2(tile, face);
}

static bool cake_isSolidRender(__attribute__((unused)) uchar *tile) {
    // Stop it from turning other blocks invisable
    return 0;
}

static int cake_getRenderLayer(__attribute__((unused)) uchar *tile) {
    // Stop weird transparency issues
    return 1;
}

static void cake_updateDefaultShape(uchar *tile) {
    // Set the default shape
    Tile_setShape(
        tile,
        cake_len,       0.0, cake_len,
        1.0 - cake_len, 0.5, 1.0 - cake_len
    );
}

static AABB *cake_getAABB(uchar *tile, __attribute__((unused)) uchar *_level, int x, int y, int z) {
    // Get the hitbox
    AABB *aabb = (AABB *)(tile + 0x40);

    // Get the size of the slices
    int data = (*Level_getData)(level, x, y, z);
    if (data >= 6) data = 0;
    float slice_size = (1.0 / 7.0) * (float) data;

    // Corner 1
    aabb->x1 = (float) x + cake_len;
    aabb->y1 = (float) y;
    aabb->z1 = (float) z + cake_len;

    // Corner 2
    aabb->x2 = (float) x + (1.0 - cake_len);
    aabb->y2 = (float) y + 0.5;
    aabb->z2 = (float) z + (1.0 - cake_len) - slice_size;

    return aabb;
}

static void cake_updateShape(uchar *tile, __attribute__((unused)) uchar *_level, int x, int y, int z) {
    // Set cake
    int data = (*Level_getData)(level, x, y, z);
    if (data >= 6) data = 0;
    // Get slice amount
    float slice_size = (1.0 / 7.0) * (float) data;
    Tile_setShape(
        tile,
        cake_len,      0.0, cake_len,
        1.0 - cake_len, 0.5, (1.0 - cake_len) - slice_size
    );
}

static int cake_use(__attribute__((unused)) uchar *tile, __attribute__((unused)) uchar *_level, int x, int y, int z, uchar *player) {
    // Eat
    uchar *food_data = (player + 0xc00);
    SimpleFoodData_eat(food_data, 3);
    // Set the new tile
    int data = (*Level_getData)(level, x, y, z);
    if (data >= 5) {
        // Delete tile
        (*Level_setTileAndData)(level, x, y, z, 0, 0);
    } else {
        // Change data
        (*Level_setTileAndData)(level, x, y, z, 92, data + 1);
    }
    return 1;
}

// Makes the cakes
static void make_cake() {
    // Allocate
    uchar *cake = (uchar *) ::operator new(TILE_VTABLE_SIZE);
    ALLOC_CHECK(cake);

    // Construct
    int texture = GET_TEXTURE_POS_IN_ATLAS(10, 7);
    Tile(cake, 92, texture, *Material_dirt);
    *(int *) (cake + Tile_texture_property_offset) = texture;

    // Set shape
    Tile_setShape(
        cake,
        cake_len,      0.0, cake_len,
        1.0 - cake_len, 0.5, 1.0 - cake_len
    );

    // Set VTable
    uchar *vtable = (uchar *) malloc(TILE_VTABLE_SIZE);
    ALLOC_CHECK(vtable);
    memcpy((void *) vtable, (void *) Tile_vtable, TILE_VTABLE_SIZE);
    *(uchar **) cake = vtable;
    vtable = *(uchar **) cake;

    // Get Functions
    Tile_setDestroyTime_t Tile_setDestroyTime = *(Tile_setDestroyTime_t *) (vtable + Tile_setDestroyTime_vtable_offset);
    Tile_setExplodeable_t Tile_setExplodeable = *(Tile_setExplodeable_t *) (vtable + Tile_setExplodeable_vtable_offset);
    Tile_setDescriptionId_t Tile_setDescriptionId = *(Tile_setDescriptionId_t *) (vtable + Tile_setDescriptionId_vtable_offset);

    // Init
    (*Tile_init)(cake);
    (*Tile_setDestroyTime)(cake, 1.0f);
    (*Tile_setExplodeable)(cake, 20.0f);
    *(int32_t *) (cake + Tile_category_property_offset) = 4;
    (*Tile_setDescriptionId)(cake, "Cake");

    // Modify functions
    *(Tile_getDescriptionId_t*) (vtable + Tile_getDescriptionId_vtable_offset) = cake_getDescriptionId;
    *(Tile_getTexture3_t *) (vtable + Tile_getTexture3_vtable_offset) = cake_getTexture3;
    *(Tile_getTexture2_t *) (vtable + Tile_getTexture2_vtable_offset) = cake_getTexture2;
    *(Tile_isSolidRender_t *) (vtable + Tile_isSolidRender_vtable_offset) = cake_isSolidRender;
    *(Tile_getRenderLayer_t *) (vtable + Tile_getRenderLayer_vtable_offset) = cake_getRenderLayer;
    *(Tile_updateShape_t *) (vtable + Tile_updateShape_vtable_offset) = cake_updateShape;
    *(Tile_updateDefaultShape_t *) (vtable + Tile_updateDefaultShape_vtable_offset) = cake_updateDefaultShape;
    *(Tile_getAABB_t *) (vtable + Tile_getAABB_vtable_offset) = cake_getAABB;
    *(Tile_use_t *) (vtable + Tile_use_vtable_offset) = cake_use;

    // Logging
    DEBUG("Created cake");
}

static void Tile_initTiles_injection(__attribute__((unused)) uchar *null) {
    // Makes the cake
    make_cake();
}

// Crafting recipe
static void Recipes_injection(uchar *recipes) {
    // Sugar
    Recipes_Type sugar = {
        .item = 0,
        .tile = 0,
        .instance = {
            .count = 1,
            .id = 353,
            .auxiliary = 0
        },
        .letter = 's'
    };
    // Wheat
    Recipes_Type wheat = {
        .item = 0,
        .tile = 0,
        .instance = {
            .count = 1,
            .id = 296,
            .auxiliary = 0
        },
        .letter = 'w'
    };
    // Eggs
    Recipes_Type eggs = {
        .item = 0,
        .tile = 0,
        .instance = {
            .count = 1,
            .id = 344,
            .auxiliary = 0
        },
        .letter = 'e'
    };
    // Cake
    ItemInstance cake = {
        .count = 1,
        .id = 92,
        .auxiliary = 0
    };
    // Add (use sugar instead of milk, as it doesn't exist)
    (*Recipes_addShapedRecipe_3)(recipes, cake, "sss", "ses", "www", {sugar, wheat, eggs});
}

__attribute__((constructor)) static void init() {
    misc_run_on_tiles_setup(Tile_initTiles_injection);
    misc_run_on_recipes_setup(Recipes_injection);
    misc_run_on_tick(mcpi_callback);
}

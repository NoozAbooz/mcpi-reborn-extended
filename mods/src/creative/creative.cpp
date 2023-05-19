#include <libreborn/libreborn.h>
#include <symbols/minecraft.h>

#include <mods/init/init.h>
#include <mods/feature/feature.h>
#include <mods/misc/misc.h>
#include <mods/creative/creative.h>
#include <mods/chest/chest.h>

#ifndef MCPI_SERVER_MODE
// Add Item To Inventory
static void inventory_add_item(unsigned char *inventory, unsigned char *item, bool is_tile) {
    ItemInstance *item_instance = new ItemInstance;
    ALLOC_CHECK(item_instance);
    item_instance = (*(is_tile ? ItemInstance_constructor_tile : ItemInstance_constructor_item))(item_instance, item);
    (*FillingContainer_addItem)(inventory, item_instance);
}

// A macro to save space and make the code more readble
#define ADD_ITEM(name, item_id) \
    { \
        ItemInstance *name##_instance = new ItemInstance; \
        ALLOC_CHECK(name##_instance); \
        name##_instance->count = 255; \
        name##_instance->auxiliary = 0; \
        name##_instance->id = item_id; \
        (*FillingContainer_addItem)(filling_container, name##_instance); \
    }

// Expand Creative Inventory
static void Inventory_setupDefault_FillingContainer_addItem_call_injection(unsigned char *filling_container) {
    // Add Items
    inventory_add_item(filling_container, *Item_flintAndSteel, false);
    inventory_add_item(filling_container, *Item_snowball, false);
    inventory_add_item(filling_container, *Item_egg, false);
    inventory_add_item(filling_container, *Item_shears, false);
    // Dyes
    for (int i = 0; i < 16; i++) {
        if (i == 15) {
            // Bonemeal Is Already In The Creative Inventory
            continue;
        }
        ItemInstance *new_item_instance = new ItemInstance;
        ALLOC_CHECK(new_item_instance);
        new_item_instance = (*ItemInstance_constructor_item_extra)(new_item_instance, *Item_dye_powder, 1, i);
        (*FillingContainer_addItem)(filling_container, new_item_instance);
    }
    inventory_add_item(filling_container, *Item_camera, false);
    // Add Tiles
    inventory_add_item(filling_container, *Tile_water, true);
    inventory_add_item(filling_container, *Tile_lava, true);
    inventory_add_item(filling_container, *Tile_calmWater, true);
    inventory_add_item(filling_container, *Tile_calmLava, true);
    inventory_add_item(filling_container, *Tile_glowingObsidian, true);
    inventory_add_item(filling_container, *Tile_web, true);
    inventory_add_item(filling_container, *Tile_topSnow, true);
    inventory_add_item(filling_container, *Tile_ice, true);
    inventory_add_item(filling_container, *Tile_invisible_bedrock, true);
    inventory_add_item(filling_container, *Tile_bedrock, true);
    inventory_add_item(filling_container, *Tile_info_updateGame1, true);
    inventory_add_item(filling_container, *Tile_info_updateGame2, true);
    // Nether Reactor
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            // Default Is Already In The Creative Inventory
            continue;
        }
        ItemInstance *new_item_instance = new ItemInstance;
        ALLOC_CHECK(new_item_instance);
        new_item_instance = (*ItemInstance_constructor_tile_extra)(new_item_instance, *Tile_netherReactor, 1, i);
        (*FillingContainer_addItem)(filling_container, new_item_instance);
    }
    // Tall Grass
    for (int i = 0; i < 4; i++) {
        if (i == 2) {
            // Identical To Previous Auxiliary Value
            continue;
        }
        ItemInstance *new_item_instance = new ItemInstance;
        ALLOC_CHECK(new_item_instance);
        new_item_instance = (*ItemInstance_constructor_tile_extra)(new_item_instance, *Tile_tallgrass, 1, i);
        (*FillingContainer_addItem)(filling_container, new_item_instance);
    }
    // Smooth Stone Slab
    {
        ItemInstance *new_item_instance = new ItemInstance;
        ALLOC_CHECK(new_item_instance);
        new_item_instance = (*ItemInstance_constructor_tile_extra)(new_item_instance, *Tile_stoneSlab, 1, 6);
        (*FillingContainer_addItem)(filling_container, new_item_instance);
    }

    // Add everything else
    ADD_ITEM(bed, 26);
    ADD_ITEM(fire, 51);
    ADD_ITEM(sign, 63);
    ADD_ITEM(door, 64);
    ADD_ITEM(ironDoor, 71);
    ADD_ITEM(trapdoor, 96);
    ADD_ITEM(mushroomStew, 282);
    ADD_ITEM(steak, 364);
    ADD_ITEM(cookedChicken, 366);
    ADD_ITEM(porkCooked, 320);
    ADD_ITEM(apple, 260);

    // Add cake to creative inventory
    {
        ItemInstance *cakeBlock_instance = new ItemInstance;
        ALLOC_CHECK(cakeBlock_instance);
        cakeBlock_instance->count = 255;
        cakeBlock_instance->auxiliary = 0;
        cakeBlock_instance->id = 92;
        (*FillingContainer_addItem)(filling_container, cakeBlock_instance);
    }
    
    ADD_ITEM(crops, 59);
    ADD_ITEM(farmland, 60);
    ADD_ITEM(activeFurnace, 62);
    ADD_ITEM(iron_door, 330);
    ADD_ITEM(activeRedstoneOre, 74);
    ADD_ITEM(pumkinStem, 105);
    ADD_ITEM(newGrass, 253);
    ADD_ITEM(doubleStoneSlab, 43);
    ADD_ITEM(arrow, 262);
    ADD_ITEM(coal, 263);
    ADD_ITEM(diamond, 264);
    ADD_ITEM(ironIngot, 265);
    ADD_ITEM(goldIngot, 266);
    ADD_ITEM(woodSword, 268);
    ADD_ITEM(woodShovel, 269);
    ADD_ITEM(woodPickaxe, 270);
    ADD_ITEM(woodAxe, 271);
    ADD_ITEM(stoneSword, 272);
    ADD_ITEM(stoneShovel, 273);
    ADD_ITEM(stonePickaxe, 274);
    ADD_ITEM(stoneAxe, 275);
    ADD_ITEM(shovelIron, 256);
    ADD_ITEM(ironPick, 257);
    ADD_ITEM(ironAxe, 258);
    ADD_ITEM(diamondSword, 276);
    ADD_ITEM(diamondShovel, 277);
    ADD_ITEM(diamondPickaxe, 278);
    ADD_ITEM(diamondAxe, 279);
    ADD_ITEM(magicWand, 280);
    ADD_ITEM(bowl, 281);
    ADD_ITEM(goldSword, 283);
    ADD_ITEM(goldShovel, 284);
    ADD_ITEM(goldPickaxe, 285);
    ADD_ITEM(goldAxe, 286);
    ADD_ITEM(string, 287);
    ADD_ITEM(feather, 288);
    ADD_ITEM(gunpowder, 289);
    ADD_ITEM(woodHoe, 290);
    ADD_ITEM(stoneHoe, 291);
    ADD_ITEM(flint1, 292);
    ADD_ITEM(diamondHoe, 293);
    ADD_ITEM(goldHoe, 294);
    ADD_ITEM(seeds, 295);
    ADD_ITEM(wheat, 296);
    ADD_ITEM(bread, 297);
    ADD_ITEM(diamondHelm, 310);
    ADD_ITEM(diamondChest, 311);
    ADD_ITEM(diamondLeg, 312);
    ADD_ITEM(diamondBoot, 313);
    ADD_ITEM(leatherCap, 298);
    ADD_ITEM(leatherShirt, 299);
    ADD_ITEM(leatherPants, 300);
    ADD_ITEM(leatherBoots, 301);
    ADD_ITEM(chainHelm, 302);
    ADD_ITEM(chainShirt, 303);
    ADD_ITEM(chainLegs, 304);
    ADD_ITEM(chainBoots, 305);
    ADD_ITEM(goldHelm, 314);
    ADD_ITEM(goldChest, 315);
    ADD_ITEM(goldLegs, 316);
    ADD_ITEM(goldBoots, 317);
    ADD_ITEM(ironHelm, 306);
    ADD_ITEM(ironChest, 307);
    ADD_ITEM(ironLegs, 308);
    ADD_ITEM(ironBoots, 309);
    ADD_ITEM(flint2, 318);
    ADD_ITEM(porkRaw, 319);
    ADD_ITEM(leather, 334);
    ADD_ITEM(clayBrick, 336);
    ADD_ITEM(clay, 337);
    ADD_ITEM(notepad, 339);
    ADD_ITEM(book, 340);
    ADD_ITEM(slimeball, 341);
    ADD_ITEM(compass, 345);
    ADD_ITEM(clock, 347);
    ADD_ITEM(glowDust, 348);
    ADD_ITEM(bone, 352);
    ADD_ITEM(sugar, 353);
    ADD_ITEM(melon, 360);
    ADD_ITEM(beefRaw, 363);
    ADD_ITEM(chickenRaw, 365);
}
#endif

// Hook Specific TileItem Constructor
static unsigned char *Tile_initTiles_TileItem_injection(unsigned char *tile_item, int32_t id) {
    // Call Original Method
    unsigned char *ret = (*TileItem)(tile_item, id);

    // Switch VTable
    *(unsigned char **) tile_item = AuxDataTileItem_vtable;
    // Configure Item
    *(bool *) (tile_item + Item_is_stacked_by_data_property_offset) = true;
    *(int32_t *) (tile_item + Item_max_damage_property_offset) = 0;
    *(unsigned char **) (tile_item + AuxDataTileItem_icon_tile_property_offset) = Tile_tiles[id + 0x100];

    // Return
    return ret;
}

static void Item_initItems_injection(__attribute__((unused)) unsigned char *obj) {
    // Patch all tiles that don't exist
    for (int i=1;i<=256;i++){
        // Checks if the item exists
        if (*(Tile_tiles + i) == NULL){
            // The tile doesn't exist, so patch it.
            *(Tile_tiles + i) = *(Tile_tiles + 248);
        }
    }
    // Now do the same to items
    for (int i=1;i<=500;i++){
        if (*(Item_items + i) == NULL){
            *(Item_items + i) = *(Item_items + 248);
        }
    }
}

// Check Restriction Status
static int is_restricted = 1;
int creative_is_restricted() {
    return is_restricted;
}

int printf_injection(__attribute__((unused)) const char *format, ... ){
    return 0;
}

// Init
void init_creative() {
    // Add Extra Items To Creative Inventory (Only Replace Specific Function Call)
    if (feature_has("Expand Creative Mode Inventory", server_enabled)) {
#ifndef MCPI_SERVER_MODE
        misc_run_on_creative_inventory_setup(Inventory_setupDefault_FillingContainer_addItem_call_injection);
#endif

        // Use AuxDataTileItem by default instead of TileItem, so tiles in the Creative
        // Inventory can have arbitrary auxiliary values.
        {
            // Fix Size
            unsigned char size_patch[4] = {AUX_DATA_TILE_ITEM_SIZE, 0x00, 0xa0, 0xe3}; // "mov r0, #AUX_DATA_TILE_ITEM_SIZE"
            patch((void *) 0xc6f64, size_patch);
            // Hook Constructor
            overwrite_call((void *) 0xc6f74, (void *) Tile_initTiles_TileItem_injection);
        }

        // Prevent crashing from invalid items
        misc_run_on_items_setup(Item_initItems_injection);
        // Remove annoying messages
        // "Item conflict id @ %d! Id already used\n"
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0x93694), (void*) printf_injection);
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0x93ff8), (void*) printf_injection);
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0x994e8), (void*) printf_injection);
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0x9a438), (void*) printf_injection);
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0x9a67c), (void*) printf_injection);
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0x9ae40), (void*) printf_injection);
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0x9b0f0), (void*) printf_injection);
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0xce404), (void*) printf_injection);
        // "Slot %d is already occupied by %p when adding %p\n"
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0xc3374), (void*) printf_injection);
        overwrite_calls(extract_from_bl_instruction((unsigned char*) 0xc3440), (void*) printf_injection);
    }

    // Remove Creative Mode Restrictions (Opening Chests, Crafting, Etc)
    if (feature_has("Remove Creative Mode Restrictions", server_enabled)) {
        unsigned char nop_patch[4] = {0x00, 0xf0, 0x20, 0xe3}; // "nop"
        // Remove Restrictions
        patch((void *) 0x43ee8, nop_patch);
        patch((void *) 0x43f3c, nop_patch);
        patch((void *) 0x43f8c, nop_patch);
        patch((void *) 0x43fd8, nop_patch);
        patch((void *) 0x99010, nop_patch);
        // Fix UI
        patch((void *) 0x341c0, nop_patch);
        patch((void *) 0x3adb4, nop_patch);
        patch((void *) 0x3b374, nop_patch);
        // Fix Inventory
        patch((void *) 0x8d080, nop_patch);
        patch((void *) 0x8d090, nop_patch);
        patch((void *) 0x91d48, nop_patch);
        patch((void *) 0x92098, nop_patch);
        unsigned char inv_creative_check_r3_patch[4] = {0x03, 0x00, 0x53, 0xe1}; // "cmp r3, r3"
        patch((void *) 0x923c0, inv_creative_check_r3_patch);
        patch((void *) 0x92828, nop_patch);
        patch((void *) 0x92830, nop_patch);
        // Display Slot Count
        patch((void *) 0x1e3f4, nop_patch);
        unsigned char slot_count_patch[4] = {0x18, 0x00, 0x00, 0xea}; // "b 0x27110"
        patch((void *) 0x270a8, slot_count_patch);
        patch((void *) 0x33954, nop_patch);
        // Maximize Creative Inventory Stack Size
        unsigned char maximize_stack_patch[4] = {0xff, 0xc0, 0xa0, 0xe3}; // "mov r12, 0xff"
        patch((void *) 0x8e104, maximize_stack_patch);
        // Allow Nether Reactor
        patch((void *) 0xc0290, nop_patch);
        // Disable Other Restrictions
        is_restricted = 0;
    }
}

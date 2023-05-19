#pragma once

#include <map>

#include <symbols/minecraft.h>

typedef unsigned char uchar;

// Armor materials
static uchar **Material_cloth   = (uchar **) 0x17a780; // Material
static uchar **Material_chain   = (uchar **) 0x17a794; // Material
static uchar **Material_iron    = (uchar **) 0x17a7a8; // Material
static uchar **Material_gold    = (uchar **) 0x17a7bc; // Material
static uchar **Material_diamond = (uchar **) 0x17a7d0; // Material

// Block materials
static uchar **Material_dirt  = (uchar **) 0x180a94; // Material
static uchar **Material_metal = (uchar **) 0x180aa0; // Material

// Item tiers
static uchar **Tier_iron    = (uchar **) 0x17ba54; // Tier
static uchar **Tier_wood    = (uchar **) 0x17ba90; // Tier
static uchar **Tier_stone   = (uchar **) 0x17bab0; // Tier
static uchar **Tier_diamond = (uchar **) 0x17bad0; // Tier
static uchar **Tier_gold    = (uchar **) 0x17baf8; // Tier

// Armor init
typedef uchar *(*ArmorItem_t)(uchar *ArmorItem, int id, uchar **material, int texture, int type);

static ArmorItem_t ArmorItem = (ArmorItem_t) 0x9362c;
static uchar *ArmorItem_vtable = (uchar *) 0x10e300;
#define ARMOR_SIZE 0x34

// Tools init
typedef uchar *(*ToolItem_t)(uchar *item, int id, uchar **tier);
static ToolItem_t WeaponItem  = (ToolItem_t) 0x9988c; // Size 0x13c
static ToolItem_t HatchetItem = (ToolItem_t) 0x9a60c;
static ToolItem_t HoeItem     = (ToolItem_t) 0x9add8; // Size 0x34
static ToolItem_t PickaxeItem = (ToolItem_t) 0x9b080; // Size 0x13c
static ToolItem_t ShovelItem  = (ToolItem_t) 0x9b388; // Size 0x13c
static uchar *WeaponItem_vtable  = (uchar *) 0x10ef30;
static uchar *HatchetItem_vtable = (uchar *) 0x10f728;
static uchar *HoeItem_vtable     = (uchar *) 0x10f7e8;
static uchar *PickaxeItem_vtable = (uchar *) 0x10f8a0;
static uchar *ShovelItem_vtable  = (uchar *) 0x10f960;
#define TOOL_ITEM_SIZE 0x13c
#define PICKAXE_ITEM_SIZE 0x34

// Item VTables
static uchar *FoodItem_vtable = (uchar *) 0x10e7b0;
static uchar *BowItem_vtable  = (uchar *) 0x10e850;

// Tile VTables
static uchar *ChestTile_vtable     = (uchar *) 0x110ea8;
static uchar *TorchTile_vtable     = (uchar *) 0x113478;
static uchar *WorkbenchTile_vtable = (uchar *) 0x113588;
static uchar *GrassTile_vtable     = (uchar *) 0x1115a8;
static uchar *TntTile_vtable       = (uchar *) 0x113148;
static uchar *SnowTile_vtable      = (uchar *) 0x113cf8;

// Tile initializers
typedef uchar *(*ChestTile_t)(uchar *tile, int id);
static ChestTile_t ChestTile = (ChestTile_t) 0xbce40;

typedef uchar *(*EntityTile_t)(uchar *entityTile, int id, const void *material);
static EntityTile_t EntityTile = (EntityTile_t) 0xbe544;

// Tile functions
typedef void (*Tile_setShape_t)(uchar *tile, float x_crop, float y_crop, float z_crop, float x_size, float y_size, float z_size);
static Tile_setShape_t Tile_setShape = (Tile_setShape_t) 0xc2ce0;

// Tile VTable (WIP)
typedef bool (*Tile_isCubeShaped_t)(uchar *tile);
static uint32_t Tile_isCubeShaped_vtable_offset = 0x8;

typedef int32_t (*Tile_getRenderShape_t)(uchar *tile);
static uint32_t Tile_getRenderShape_vtable_offset = 0xc;

typedef void (*Tile_updateShape_t)(uchar *tile, uchar *level, int x, int y, int z);
static uint32_t Tile_updateShape_vtable_offset = 0x14;

typedef void (*Tile_updateDefaultShape_t)(uchar *tile);
static uint32_t Tile_updateDefaultShape_vtable_offset = 0x18;

typedef int (*Tile_getTexture1_t)(uchar *tile);
static uint32_t Tile_getTexture1_vtable_offset = 0x28;

typedef int (*Tile_getTexture2_t)(uchar *tile, int face);
static uint32_t Tile_getTexture2_vtable_offset = 0x2c;

typedef int (*Tile_getTexture3_t)(uchar *tile, uchar *level_source, int x, int y, int z, int face);
static uint32_t Tile_getTexture3_vtable_offset = 0x30;

typedef AABB *(*Tile_getAABB_t)(uchar *tile, uchar *level, int x, int y, int z);
static uint32_t Tile_getAABB_vtable_offset = 0x34;

typedef bool (*Tile_isSolidRender_t)(uchar *tile);
static uint32_t Tile_isSolidRender_vtable_offset = 0x40;

typedef void (*Tile_tick_t)(uchar *tile, uchar *level, int x, int y, int z);
static uint32_t Tile_tick_vtable_offset = 0x58;

typedef void (*Tile_neighborChanged_t)(uchar *tile, uchar *level, int x, int y, int z, int neighborId);
static uint32_t Tile_neighborChanged_vtable_offset = 0x64;

typedef void (*Tile_onPlace_t)(uchar *tile, uchar *level, int x, int y, int z);
static uint32_t Tile_onPlace_vtable_offset = 0x68;

typedef void (*Tile_onRemove_t)(uchar *tile, uchar *level, int x, int y, int z);
static uint32_t Tile_onRemove_vtable_offset = 0x6c;

typedef int (*Tile_getRenderLayer_t)(uchar *tile);
static uint32_t Tile_getRenderLayer_vtable_offset = 0x94;

typedef uchar *(*Tile_getColor_t)(uchar *tile);
static uint32_t Tile_getColor_vtable_offset = 0xb8;

typedef std::string (*Tile_getDescriptionId_t)(uchar *tile, int id);
static uint32_t Tile_getDescriptionId_vtable_offset = 0xdc;

typedef uchar *(*ChestTile_newTileEntity_t)(uchar *chestTile);
static uint32_t ChestTile_getDescriptionId_vtable_offset = 0x104;

static uchar *Tile_shouldTick = (uchar *) 0x181a14;

static uint32_t Tile_texture_property_offset = 0x4; // int

// Entity
typedef void (*Entity_moveTo_t)(uchar *entity, float x, float y, float z, float pitch, float yaw);
static Entity_moveTo_t Entity_moveTo = (Entity_moveTo_t) 0x7a834;

static uint32_t Entity_id_property_offset = 0x1c; // int32_t
static uint32_t AgableMob_age_property_offset = 0xbfc; // int32_t
static uint32_t TntEntity_fuse_property_offset = 0xd0; // int32_t
static uint32_t ItemEntity_instance_property_offset = 0xd0; // ItemInstance
static uint32_t TileEntity_tile_property_offset = 0xd0; // int
static uint32_t TileEntity_lifetime_property_offset = 0xd8; // int
static uint32_t ArrowEntity_isCritical_property_offset = 0xd8; // bool

// Entity tile
typedef void (*EntityTile_onPlace_t)(uchar *entityTile, uchar *level, int x, int y, int z);
static EntityTile_onPlace_t EntityTile_onPlace = (EntityTile_onPlace_t) 0xbe508;

// Tile entity
typedef uchar *(*TileEntityFactory_createTileEntity_t)(int id);
static TileEntityFactory_createTileEntity_t TileEntityFactory_createTileEntity = (TileEntityFactory_createTileEntity_t) 0xd226c;

typedef unsigned char *(*TileEntity_t)(uchar *tileEntity, int id);
static TileEntity_t TileEntity = (TileEntity_t) 0xd2308;

// Filling container
typedef uchar *(*FillingContainer_t)(uchar *fillingContainer, int param_2, int param_3, int param_4, bool param_5);
static FillingContainer_t FillingContainer = (FillingContainer_t) 0x92f08;

// Item VTable (WIP)
typedef int (*Item_getUseDuration_t)(uchar *item, ItemInstance *instance);
static uint32_t Item_getUseDuration_vtable_offset = 0x24;
typedef void (*Item_useTimeDepleted_t)(uchar *item, ItemInstance *instance, ItemInstance *leftover, uchar *param_4, uchar *param_5);
static uint32_t Item_useTimeDepleted_vtable_offset = 0x28;

typedef int (*Item_getDestroySpeed_t)(uchar *item, ItemInstance *instance, uchar *tile);
static uint32_t Item_getDestroySpeed_vtable_offset = 0x2c;

typedef void (*Item_use_t)(uchar *item, ItemInstance *instance, uchar *level, uchar *player);
static uint32_t Item_use_vtable_offset = 0x30;

typedef int (*Item_mineBlock_t)(uchar *item, ItemInstance *instance, int param_2, int param_3, int param_4, int param_5);
static uint32_t Item_mineBlock_vtable_offset = 0x48;

typedef int (*Item_getUseAnimation_t)(uchar *item);
static uint32_t Item_getUseAnimation_vtable_offset = 0x94;

typedef void (*SimpleFoodData_eat_t)(uchar *simpleFoodData, int amount);
static SimpleFoodData_eat_t SimpleFoodData_eat = (SimpleFoodData_eat_t) 0x91470;

// Mob
typedef uchar *(*MobFactory_CreateMob_t)(int mob, uchar *level);
static MobFactory_CreateMob_t MobFactory_CreateMob = (MobFactory_CreateMob_t) 0x18184;
typedef uchar *(*EntityFactory_CreateEntity_t)(int id, uchar *level);
static EntityFactory_CreateEntity_t EntityFactory_CreateEntity = (EntityFactory_CreateEntity_t) 0x7d794;

typedef uchar *(*Mob_t)(uchar *mob, uchar *level);
static Mob_t Chicken = (Mob_t) 0x855a0;
static Mob_t Zombie = (Mob_t) 0x89cc8;
static Mob_t Spider = (Mob_t) 0x89618;

// Level
Level_getTile_t Level_getData = (Level_getTile_t) 0xa3324;

typedef uchar *(*Level_addEntity_t)(uchar *level, uchar *entity);
static Level_addEntity_t Level_addEntity = (Level_addEntity_t) 0xa7cbc;

typedef void (*Level_setTileEntity_t)(uchar *level, int x, int y, int z, uchar *tileEntity);
static Level_setTileEntity_t Level_setTileEntity = (Level_setTileEntity_t) 0xa7b58;

typedef int (*Level_getHeightmap_t)(uchar *level, int x, int z);static Level_getHeightmap_t Level_getHeightmap = (Level_getHeightmap_t) 0xa3c10;

// Lang map
static auto langMap = (std::map<std::string, std::string> *) 0x137d98;

// Helpers
#define GET_TEXTURE_POS_IN_ATLAS(x, y) (((y) * 16) + (x))
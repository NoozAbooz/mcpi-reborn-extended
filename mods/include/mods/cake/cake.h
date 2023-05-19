#pragma once

typedef unsigned char uchar;

// Material
static uchar **Material_dirt  = (uchar **) 0x180a94;

// Tile functions
typedef void (*Tile_setShape_t)(uchar *tile, float x_crop, float y_crop, float z_crop, float x_size, float y_size, float z_size);
static Tile_setShape_t Tile_setShape = (Tile_setShape_t) 0xc2ce0;

// Tile VTable (WIP)
typedef void (*Tile_updateShape_t)(uchar *tile, uchar *level, int x, int y, int z);
static uint32_t Tile_updateShape_vtable_offset = 0x14;

typedef void (*Tile_updateDefaultShape_t)(uchar *tile);
static uint32_t Tile_updateDefaultShape_vtable_offset = 0x18;

typedef int (*Tile_getTexture2_t)(uchar *tile, int face);
static uint32_t Tile_getTexture2_vtable_offset = 0x2c;

typedef int (*Tile_getTexture3_t)(uchar *tile, uchar *level_source, int x, int y, int z, int face);
static uint32_t Tile_getTexture3_vtable_offset = 0x30;

typedef AABB *(*Tile_getAABB_t)(uchar *tile, uchar *level, int x, int y, int z);
static uint32_t Tile_getAABB_vtable_offset = 0x34;

typedef bool (*Tile_isSolidRender_t)(uchar *tile);
static uint32_t Tile_isSolidRender_vtable_offset = 0x40;

typedef int (*Tile_getRenderLayer_t)(uchar *tile);
static uint32_t Tile_getRenderLayer_vtable_offset = 0x94;

typedef std::string (*Tile_getDescriptionId_t)(uchar *tile, int id);
static uint32_t Tile_getDescriptionId_vtable_offset = 0xdc;

static uint32_t Tile_texture_property_offset = 0x4; // int

typedef void (*SimpleFoodData_eat_t)(uchar *simpleFoodData, int amount);
static SimpleFoodData_eat_t SimpleFoodData_eat = (SimpleFoodData_eat_t) 0x91470;

// Level
Level_getTile_t Level_getData = (Level_getTile_t) 0xa3324;

// Helpers
#define GET_TEXTURE_POS_IN_ATLAS(x, y) (((y) * 16) + (x))

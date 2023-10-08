#ifndef MATERIALS_H
#define MATERIALS_H
#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <string>

using std::string;

enum Material_id {
    MATERIAL_AIR,
    MATERIAL_STONE,
    MATERIAL_SAND,
};

enum Material_type {
    MATERIAL_TYPE_NONE,
    MATERIAL_TYPE_SOLID,
    MATERIAL_TYPE_POWDER,
    MATERIAL_TYPE_FLUID,
    MATERIAL_TYPE_GAS,
};

typedef struct {
    Material_id id;
    Material_type type = MATERIAL_TYPE_SOLID;
    int8_t gravity_multiplier = 1;
    string name = "Unknown material";
    float minColorMultiplier = 1;
    float r = 1;
    float g = 1;
    float b = 1;
} Material;

typedef struct {
    Material_id material;
    float temperature = 0;
    float colorMultiplier = 0.9;
    u16 x;
    u16 y;
} Particle;


#endif

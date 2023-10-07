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

typedef struct Material
{
    Material_id id;
    int8_t gravity = -1;
    string name = "Unknown material";
    u32 color = C2D_Color32f(1.0f,0.0f,0.0f,1.0f);
};

typedef struct Particle
{
    Material_id material;
    float temperature = 0;
    int x;
    int y;
};


#endif

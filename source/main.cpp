#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include "materials.h"
#include "debug.h"

using std::string;
using std::to_string;
using std::vector;
using std::map;

const int BOTTOM_SCREEN_WIDTH = 320;
const int BOTTOM_SCREEN_HEIGHT = 240;

int brushSize = 5;
int gravity = 10;
bool borders = true;
vector<Particle> particles;
Material_id currentMaterial = MATERIAL_SAND;

map<Material_id, Material> materials;

void registerMaterial(Material_id id, Material_type type, float r = 1, float g = 1, float b = 1,float minColorMultiplier = 1, int8_t gravity_multiplier = 1){
	Material m;
	m.id = id;
	m.type = type;
	m.r = r;
	m.g = g;
	m.b = b;
	m.minColorMultiplier = minColorMultiplier;
	m.gravity_multiplier = gravity_multiplier;

	materials[id] = m;
}

void addParticle(Particle particle){
	particles.push_back(particle);
}

void addParticle(Material_id material, u16 x, u16 y, float colorMultiplier = 1){
	if(x>BOTTOM_SCREEN_WIDTH) return;
	if(y>BOTTOM_SCREEN_HEIGHT) return;

	Particle p;
	p.material = material;
	p.x = x;
	p.y = y;
	p.colorMultiplier = colorMultiplier;

	addParticle(p);
}

void removeParticle(size_t index){
	particles.erase(particles.begin()+index);
}

void removeParticle(int x, int y){
	for(size_t i=0;i<particles.size();i++){
		Particle p = particles[i];
		if(p.x == x && p.y == y){
			removeParticle(i);
			return;
		}
	}
}

Particle getParticle(int x, int y){
	for(size_t i=0;i<particles.size();i++){
		Particle p = particles[i];
		if(p.x == x && p.y == y){
			return p;
		}
	}
	return nullParticle();
}

bool particleExists(int x, int y){
	for(size_t i=0;i<particles.size();i++){
		Particle p = particles[i];
		if(p.x == x && p.y == y){
			return true;
		}
	}
	return false;
}

Material_id getMaterialId(int x, int y){
	for(size_t i=0;i<particles.size();i++){
		Particle p = particles[i];
		if(p.x == x && p.y == y){
			return p.material;
		}
	}
	return MATERIAL_AIR;
}

float randf(float min, float max) {
    return min + (rand() / (RAND_MAX / (max - min + 1) + 1));
}


Material_type getMaterialType(int x, int y){
	Material_id m = getMaterialId(x,y);
	if(m != MATERIAL_AIR && materials.count(m)){
		return materials[m].type;
	}
	return MATERIAL_TYPE_NONE;
}

u32 fall(u32 x,u32 y, int grav){
	for(size_t i=1;i<grav;i++){
		Material_type mt = getMaterialType(x,y+i);
		if(mt != MATERIAL_TYPE_NONE && mt != MATERIAL_TYPE_GAS && mt != MATERIAL_TYPE_FLUID){
			return y+i-1;
		}
	}
	if(borders && y+grav > BOTTOM_SCREEN_HEIGHT) return BOTTOM_SCREEN_HEIGHT;
	return y+grav;
}

int main(int argc, char* argv[]){
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(1024*100);
	C2D_Prepare();

	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	registerMaterial(MATERIAL_STONE,MATERIAL_TYPE_SOLID,0.5,0.5,0.5, 0.9);
	registerMaterial(MATERIAL_SAND,MATERIAL_TYPE_POWDER,1,1,0, 0.8);

	while (aptMainLoop()){
		gspWaitForVBlank();
		hidScanInput();

		
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if (kDown & KEY_START) break;

		if(kHeld & KEY_TOUCH){
			touchPosition touchPos;
			touchRead(&touchPos);

			int startX = touchPos.px - brushSize / 2;
    		int startY = touchPos.py - brushSize / 2;
    		int endX = startX + brushSize;
    		int endY = startY + brushSize;

			Material data = materials[currentMaterial];
    		for (int x = startX; x < endX; x++) {
        		for (int y = startY; y < endY; y++) {
					if(data.type == MATERIAL_TYPE_POWDER && randf(0,1) < 0.5f){ // scatter
						removeParticle(x, y);
            			addParticle(currentMaterial, x, y,randf(data.minColorMultiplier,1));
					}
        		}
    		}
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(bottom, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));
		C2D_SceneBegin(bottom);

		for(size_t i=0;i<particles.size();i++){
			Particle p = particles[i];

			if(p.x < 0 || p.x > BOTTOM_SCREEN_WIDTH || p.y < 0 || p.y > BOTTOM_SCREEN_HEIGHT){
				removeParticle(i);
				i--;
				continue;
			}

			auto materialIt = materials.find(p.material);
			if(materialIt != materials.end()){
				Material m = materialIt->second;
				float mul = p.colorMultiplier;
				C2D_DrawRectSolid(p.x,p.y, 0.0f, 1.0f,1.0f, C2D_Color32f(m.r*mul,m.g*mul,m.b*mul,1) );

				if(m.type == MATERIAL_TYPE_POWDER || m.type == MATERIAL_TYPE_FLUID){
					u32 gr = gravity*m.gravity_multiplier;
					particles[i].y = fall(p.x,p.y,gr);
				}
			}
			
		}

		C3D_FrameEnd(0);
	}

	gfxExit();
	return 0;
}

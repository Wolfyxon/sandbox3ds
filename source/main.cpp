#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include "materials.h"
#include "debug.h"

using std::string;
using std::to_string;

const int BOTTOM_SCREEN_WIDTH = 320;
const int BOTTOM_SCREEN_HEIGHT = 240;

int brushSize = 2;
int particleAmount = 0;
Particle *particles[BOTTOM_SCREEN_HEIGHT*BOTTOM_SCREEN_WIDTH];
Material_id currentMaterial = MATERIAL_STONE;

void addParticle(Particle *particle){
	particles[particleAmount] = particle;
	particleAmount++;
}

void addParticle(Material_id material, u16 x, u16 y){
	Particle p;
	p.material = material;
	p.x = x;
	p.y = y;

	addParticle(&p);
}

void removeParticle(size_t index){
	if(particleAmount > particleAmount-1) return;
	particles[index] = NULL;
	particleAmount--;
}

void removeParticle(int x, int y){
	for(size_t i=0;i<particleAmount;i++){
		Particle *p = particles[i];
		if(p->x == x && p->y == y){
			removeParticle(i);
			return;
		}
	}
}

Particle *getParticle(int x, int y){
	for(size_t i=0;i<particleAmount;i++){
		Particle *p = particles[i];
		if(p->x == x && p->y == y){
			return p;
		}
	}
	return NULL;
}

int main(int argc, char* argv[]){
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	while (aptMainLoop()){
		gspWaitForVBlank();
		hidScanInput();

		
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;

		if(kDown & KEY_TOUCH){
			touchPosition touchPos;
			touchRead(&touchPos);

			removeParticle(touchPos.px,touchPos.py);
			addParticle(currentMaterial,touchPos.px,touchPos.py);
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(bottom, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));
		C2D_SceneBegin(bottom);

		for(int i=0;i<particleAmount;i++){
			Particle *p = particles[i];
			C2D_DrawRectSolid(p->x,p->y, 0.0f, 1.0f,1.0f, C2D_Color32f(1.0f,0.0f,0.0f,1.0f) );
		}

		C3D_FrameEnd(0);
	}

	gfxExit();
	return 0;
}

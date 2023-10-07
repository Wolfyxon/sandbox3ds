#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include "materials.h"

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

void addParticle(Material_id material, int x, int y){
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
	
	while (aptMainLoop()){
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;

		if(kDown & KEY_TOUCH){
			touchPosition touchPos;
			touchRead(&touchPos);

			removeParticle(touchPos.px,touchPos.py);
			addParticle(currentMaterial,touchPos.px,touchPos.py);
		}
	}

	gfxExit();
	return 0;
}

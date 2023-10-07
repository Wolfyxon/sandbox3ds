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

int main(int argc, char* argv[]){
	gfxInitDefault();
	
	while (aptMainLoop()){
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;

		if(kDown & KEY_TOUCH){

		}
	}

	gfxExit();
	return 0;
}

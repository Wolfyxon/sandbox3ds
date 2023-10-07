#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include "materials.h"
#include "debug.h"

using std::string;
using std::to_string;
using std::vector;

const int BOTTOM_SCREEN_WIDTH = 320;
const int BOTTOM_SCREEN_HEIGHT = 240;

int brushSize = 5;
vector<Particle> particles;
Material_id currentMaterial = MATERIAL_STONE;

void addParticle(Particle particle){
	particles.push_back(particle);
}

void addParticle(Material_id material, u16 x, u16 y){
	if(x>BOTTOM_SCREEN_WIDTH) return;
	if(y>BOTTOM_SCREEN_HEIGHT) return;

	Particle p;
	p.material = material;
	p.x = x;
	p.y = y;

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
}

int main(int argc, char* argv[]){
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(1024*100);
	C2D_Prepare();

	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

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

    		for (int x = startX; x < endX; x++) {
        		for (int y = startY; y < endY; y++) {
            		removeParticle(x, y);
            		addParticle(currentMaterial, x, y);
        		}
    		}
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(bottom, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));
		C2D_SceneBegin(bottom);

		for(size_t i=0;i<particles.size();i++){
			Particle p = particles[i];
			C2D_DrawRectSolid(p.x,p.y, 0.0f, 1.0f,1.0f, C2D_Color32f(1.0f,0.0f,0.0f,1.0f) );
		}

		C3D_FrameEnd(0);
	}

	gfxExit();
	return 0;
}

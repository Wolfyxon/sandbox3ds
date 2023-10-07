#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include "materials.h"

const int BOTTOM_SCREEN_WIDTH = 320;
const int BOTTOM_SCREEN_HEIGHT = 240;

int brushSize = 2;
Material_id currentMaterial = MATERIAL_STONE;

int main(int argc, char* argv[])
{
	gfxInitDefault();
	
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

		if(kDown & KEY_TOUCH){

		}

	}

	gfxExit();
	return 0;
}

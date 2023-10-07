#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include "materials.h"

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
	}

	gfxExit();
	return 0;
}

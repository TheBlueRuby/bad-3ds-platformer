#include <3ds.h>
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

typedef struct Collider
{
	float x;
	float y;
	float width;
	float height;
	u32 color;
} Collider;

int main(int argc, char *argv[])
{
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	u32 clrClear = C2D_Color32(0xEE, 0xEE, 0xEE, 0x68);
	u32 playerColor = C2D_Color32(0xEE, 0x33, 0x55, 0xFF);

	// Game Init

	float playerX = SCREEN_WIDTH / 2.0;
	float playerY = SCREEN_HEIGHT / 2.0;

	Collider platforms[3] = 
	{
		{x : 0, y : 230, width : 400, height : 10, color : C2D_Color32(0x33, 0xEE, 0x55, 0xFF)},
		{x : 100, y : 100, width : 250, height : 10, color : C2D_Color32(0xEE, 0x99, 0x55, 0xFF)},
		{x : 50, y : 200, width : 150, height : 10, color : C2D_Color32(0xEE, 0x99, 0x55, 0xFF)},
	};

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysHeld();
		u32 newDown = hidKeysDown();
		if (kDown & KEY_START)
		{
			break; // break in order to return to hbmenu
		}
		printf("\x1b[1;1HBad 3DS Platformer");

		printf("\x1b[3;1HUse D-Pad or Circle Pad to move!");

		printf("\x1b[7;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime() * 6.0f);
		printf("\x1b[8;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime() * 6.0f);
		printf("\x1b[9;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage() * 100.0f);

		// Process player input

		if (((kDown & KEY_LEFT) || (kDown & KEY_CPAD_LEFT)) && (playerX > 25))
		{
			playerX -= 5.0;
		}
		if (((kDown & KEY_RIGHT) || (kDown & KEY_CPAD_RIGHT)) && (playerX < SCREEN_WIDTH - 25))
		{
			playerX += 5.0;
		}

		if (((newDown & KEY_UP) || (newDown & KEY_CPAD_UP) || (newDown & KEY_A) || (newDown & KEY_B)) && (playerY > 25))
		{
			playerY -= 10.0;
		}
		if (playerY < SCREEN_HEIGHT - 25)
		{
			playerY += 5.0;
		}

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		for (int i = 0; i < 2; i++)
		{
			C2D_DrawRectangle(platforms[i].x, platforms[i].y, 0, platforms[i].width, platforms[i].height, platforms[i].color, platforms[i].color, platforms[i].color, platforms[i].color);
		}

		C2D_DrawRectangle(playerX - 25, playerY - 25, 0, 50, 50, playerColor, playerColor, playerColor, playerColor);
	
		C3D_FrameEnd(0);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <GL/gl.h>
#include "stage.h"
#include "texture.h"

static void mainloop(void) {
	SDL_Event evt;
	
	const char* title = "Flare";
	SPStage* win = SPStage_new(title, 800, 600);
	bool quit = false;

//	SPTexture* tex = SPTexture_frompng("tex.png");

	SPColor color;
	SPPoint p1;
	SPPoint p2;
	SPColor_set(&color, 1.0, 1.0, 1.0, 1.0);
	SPPoint_set(&p1, 0, 0);
	SPPoint_set(&p2, 200, 300);
	
	SPPoint camera;
	SPPoint_set(&camera, 0, 0);
		
	while(1) {
		while(SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) quit = true;
		}
	
		if(quit) break;
		
		camera.x += 1.0;
		SPStage_center(win, &camera);
		SPStage_draw_line(win, &p1, &p2, &color, 2.0, 2);

		SPStage_finalize(win);
		SDL_Delay(1);
	}

//	SPTexture_free(tex);
	SPStage_free(win);
}

int main(int argc, char** argv) {
	if(argc > 1) {
		
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL video initialization error.\n");
	}

	mainloop();

	SDL_Quit();
	
	return 0;
}

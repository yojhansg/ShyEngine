#include <PhysicsManager.h>
#include <RendererManager.h>
#include <InputManager.h>
#include <SoundManager.h>

#include <iostream>

int main(int argc, char* args[]) {

    RendererManager rm;
    InputManager im;

	// a boolean to exit the loop
	bool exit = false;
	SDL_Event event;

	while (!exit) {

		Uint32 startTime = SDL_GetTicks();

		im.clearState();
		while (SDL_PollEvent(&event))
			im.update(event);

		if (im.isKeyDown(SDL_SCANCODE_ESCAPE) || event.type == SDL_QUIT) {
			exit = true;
			continue;
		}

		if (im.mouseMotionEvent()) {
			std::cout << im.getMousePos().first << " " << im.getMousePos().second << "\n";
		}

		//rm.clearRenderer();

		//rm.presentRenderer();

		Uint32 frameTime = SDL_GetTicks() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

	rm.closeSDL();

	return 0;
}
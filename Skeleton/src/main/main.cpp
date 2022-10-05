#include <PhysicsManager.h>
#include <SoundManager.h>
#include <RendererManager.h>
#include <InputManager.h>

#include <iostream>
#include <memory>

int main(int argc, char* args[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto rm = RendererManager::RendererManager::init("PhosphorusEngine Window", 1280, 720);
	auto im = InputManager::InputManager::init();

	// a boolean to exit the loop
	bool exit = false;
	SDL_Event event;

	SDL_Color c = rm->createColor(0x00000000);

	while (!exit) {

		Uint32 startTime = SDL_GetTicks();

		im->clearState();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit = true;
				continue;
			}
			im->update(event);
		}

		if (im->isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit =  true;
			continue;
		}
		
		// Test de botones
		if (im->isJoystickButtonEventDown()) {
			for (int i = 0; i < im->getJoysticksNumButtons(); i++) {
				std::cout << im->getJoystickButtonState(i) << "  ";
			}
			std::cout << std::endl;
		}

		// Test de joysticks

		if (im->isJoystickAxisMotion()) {
			if (im->isLeftTriggerMotion()) {
				float a = im->getJoystickTriggerValue(InputManager::InputManager::LEFT_TRIGGER);
				a *= 255;

				c.r = a; c.g = a; c.b = a;
			}
			else {
				c.r = 0; c.g = 0; c.b = 0;
			}
		}
		

		
		rm->clearRenderer(c);



		rm->presentRenderer();

		Uint32 frameTime = SDL_GetTicks() - startTime;

		if (frameTime < 60)
			SDL_Delay(60 - frameTime);
	}

	return 0;
}
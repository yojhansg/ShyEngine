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
	auto sm = SoundManager::SoundManager::init();

	sm->loadSound("GoT.mp3");
	sm->setVolume(1);
	sm->playSound(0);

	const SDL_Rect* r = new SDL_Rect({ rm->getWidth() / 2 - 50, rm->getHeight() / 2 - 50, 100, 100});

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

		// Test de teclado
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

		// Test de triggers
		if (im->isJoystickAxisMotion()) {
			if (im->isLeftTriggerMotion()) {

			}
		}

		// Test de audio
		if (im->isKeyDown(SDL_SCANCODE_A)) {
			sm->togglePlay();
		}
		
		
		rm->clearRenderer(c);

		SDL_SetRenderDrawColor(rm->getRenderer(), 255, 0, 0, 255);
		SDL_RenderFillRect(rm->getRenderer(), r);

		rm->presentRenderer();

		Uint32 frameTime = SDL_GetTicks() - startTime;

		if (frameTime < 60)
			SDL_Delay(60 - frameTime);
	}

	delete r; r = NULL;

	return 0;
}
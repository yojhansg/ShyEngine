#include "Engine.h"

#include <PhysicsManager.h>
#include <RendererManager.h>
#include <SoundManager.h>
#include <InputManager.h>

#include <iostream>
#include <memory>


Engine::Engine() {

	pm = nullptr; rm = nullptr; im = nullptr; sm = nullptr;
}

Engine::~Engine() {

}

void Engine::init() {

	pm = PhysicsManager::PhysicsManager::init();
	rm = RendererManager::RendererManager::init("PhosphorusEngine Window", WIN_WIDTH, WIN_HEIGHT);
	im = InputManager::InputManager::init();
	sm = SoundManager::SoundManager::init();

	pm->configureScene();
}

void Engine::update() {

	SDL_Rect* r = new SDL_Rect({ rm->getWidth() / 2 - 25, rm->getHeight() / 2 - 25, 50, 50 });
	SDL_Color c = rm->createColor(0x00000000);

	// a boolean to exit the loop
	bool exit = false;
	SDL_Event event;

	int frames = 0;
	Uint32 startTime = SDL_GetTicks();
	Uint32 timeSinceStart = 0;

	while (!exit) {

		Uint32 sT = SDL_GetTicks();

		// Input
		im->clearState();
		while (SDL_PollEvent(&event)) {
			im->update(event);

			if (im->isKeyDown(SDL_SCANCODE_ESCAPE) || event.type == SDL_QUIT) {
				exit = true;
				continue;
			}
		}

		// Update
		pm->update();

		// Render
		rm->clearRenderer(c);

		SDL_SetRenderDrawColor(rm->getRenderer(), 255, 185, 0, 255);
		SDL_RenderFillRect(rm->getRenderer(), r);

		rm->presentRenderer();


		// Tiempo
		timeSinceStart = SDL_GetTicks() - startTime;

		if (timeSinceStart > 1000) {
			std::cout << "Frames por segundo: " << frames << std::endl;

			timeSinceStart = 0;
			startTime = SDL_GetTicks();
			frames = 0;
		}
		frames++;

		Uint32 fT = SDL_GetTicks() - sT;

		if (fT < 20)
			SDL_Delay(20 - fT);
	}

	delete r; r = nullptr;
}

void Engine::close() {

}

#include "Image.h"
#include <InputManager.h>
#include "SceneManager.h"
#include <iostream>

namespace ECS {

	void Image::init() {
		std::cout << "Componente Image iniciado!\n";
	}

	void Image::update(float deltaTime) {
		std::cout << "Componente Image actualizandose!\n";
	}

	void Image::lateUpdate(float deltaTime) {
		//std::cout << "Componente Image actualizandose mas tarde!\n";
	}

	void Image::fixedUpdate(float fixedDeltaTime) {
		//std::cout << "Componente Image actualizandose en el framerate fisico!\n";
	}


	void Image::handleInput() {
		if (InputManager::InputManager::instance()->isKeyDown(SDL_SCANCODE_S)) {
			SceneManager::instance()->changeScene(nullptr, SceneManager::POP);
		}
	}

}
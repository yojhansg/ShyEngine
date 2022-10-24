#include "Image.h"
#include <iostream>

namespace ECS {

	void Image::init() {

		std::cout << "Componente Image iniciado!\n";
	}

	void Image::update() {
		std::cout << "Componente Image actualizandose!\n";
	}

	void Image::lateUpdate() {
		std::cout << "Componente Image actualizandose mas tarde!\n";
	}

	void Image::handleInput() {

	}

}
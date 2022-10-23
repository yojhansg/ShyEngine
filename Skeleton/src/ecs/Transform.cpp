#include "Transform.h"
#include <iostream>

namespace ECS {

	void Transform::init() {
		std::cout << "Componente Transform iniciado!\n";
	}

	void Transform::update() {
		std::cout << "Componente Transform actualizandose!\n";
	}

	void Transform::lateUpdate() {
		std::cout << "Componente Transform actualizandose mas tarde!\n";
	}

}
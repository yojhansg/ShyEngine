#include "Transform.h"
#include "Vector2D.h"
#include <iostream>

namespace ECS {

	Transform::Transform() {

		position = Utilities::Vector2D(0, 0);
		scale = Utilities::Vector2D(1, 1);
		rotation = 0;
	}

	Utilities::Vector2D Transform::getPosition() {
		return position;
	}

	Utilities::Vector2D Transform::getScale() {
		return scale;
	}

	float Transform::getRotation() {
		return rotation;
	}

	void Transform::setScale(const Utilities::Vector2D& scale) {
		this->scale = scale;
	}

	void Transform::translate(const Utilities::Vector2D& position) {
		this->position += position;
	}

	void Transform::rotate(float rotation) {
		this->rotation = rotation;
	}

}
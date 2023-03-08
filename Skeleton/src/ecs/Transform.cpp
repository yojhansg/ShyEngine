#include "Transform.h"
#include "Vector2D.h"

#include <iostream>

namespace ECS {

	Transform::Transform() {
		position_ = Utilities::Vector2D(0, 0);
		scale_ = Utilities::Vector2D(1, 1);
		rotation_ = 0;

		//registeredMethods.insert(std::make_pair("HolaMamasita", &Component::cacadevaca));
		registeredMethods.emplace("HolaMamasita", MiMetodo);
	}

	Transform::Transform(const Utilities::Vector2D& position, const Utilities::Vector2D& scale, float rotation) {
		position_ = position; 
		scale_ = scale;
		rotation_ = rotation;
	}

	// ------------- Getters -----------------

	Utilities::Vector2D* Transform::getPosition() {
		return &position_;
	}

	Utilities::Vector2D* Transform::getScale() {
		return &scale_;
	}

	const double* Transform::getRotation() {
		return &rotation_;
	}

	// ------------- Setters -----------------

	void Transform::setPosition(const Utilities::Vector2D& position) {
		this->position_ = position;
	}

	void Transform::setPosition(float x, float y) {
		this->position_.set(x, y);
	}

	void Transform::setScale(const Utilities::Vector2D& scale) {
		this->scale_ = scale;
	}

	void Transform::setScale(float x, float y) {
		this->scale_.set(x, y);
	}

	void Transform::setRotation(float rotation) {
		this->rotation_ = rotation;
	}

	// ------------ Modifiers ------------------

	void Transform::translate(const Utilities::Vector2D& position) {
		this->position_ += position;
	}

	void Transform::translate(float x, float y) {
		this->position_.set(this->position_.getX() + x, this->position_.getY() + y);
	}

	void Transform::rotate(float rotation) {
		this->rotation_ += rotation;
	}

	void Transform::scale(float scale) {
		this->scale_ *= scale;
	}

	Scripting::Variable MiMetodo(std::vector<Scripting::Variable> const& input)
	{
		Transform* tr = static_cast<Transform*>(input[0].value.pointer);
		tr->rotate(input[1].value.Float);

		std::cout << "Hola que tal" << std::endl;

		return Scripting::Variable::Null();
	}
}
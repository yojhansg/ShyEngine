#include "Transform.h"
#include "Vector2D.h"

#include "Entity.h"

namespace ECS {

	Transform::Transform() {
		position_ = Utilities::Vector2D(0, 0);
		scale_ = Utilities::Vector2D(1, 1);
		rotation_ = 0;

	}

	Transform::Transform(const Utilities::Vector2D& position, const Utilities::Vector2D& scale, float rotation) {
		position_ = position; 
		scale_ = scale;
		rotation_ = rotation;

	}

	// ------------- Getters -----------------
	

	Utilities::Vector2D Transform::getPosition() {
		return position_;
	}

	Utilities::Vector2D Transform::getScale() {
		return scale_;
	}

	float Transform::getRotation()
	{
		return rotation_;
	}

	Utilities::Vector2D* Transform::getPositionPointer()
	{
		return &position_;
	}

	Utilities::Vector2D* Transform::getScalePointer()
	{
		return &scale_;
	}

	const double* Transform::getRotationPointer() {
		return &rotation_;
	}

	// ------------- Setters -----------------

	void Transform::setPosition(Utilities::Vector2D position) {
		this->position_ = position;
	}

	void Transform::setPositionX(float x) {
		this->position_.set(x, position_.getY());
	}

	void Transform::setPositionY(float y) {
		this->position_.set(position_.getX(), y);
	}

	void Transform::setScale(Utilities::Vector2D scale) {
		this->scale_ = scale;
	}

	void Transform::setScaleX(float x) {
		this->scale_.set(x, scale_.getY());
	}

	void Transform::setScaleY(float x) {
		this->scale_.set(x, scale_.getY());
	}


	void Transform::setRotation(float rotation) {
		this->rotation_ = rotation;
	}

	// ------------ Modifiers ------------------

	void Transform::translate(Utilities::Vector2D direction) {
		this->position_ += direction;
	}

	void Transform::translateX(float x) {
		this->position_.set(this->position_.getX() + x, this->position_.getY());
	}

	void Transform::translateY(float y) {
		this->position_.set(this->position_.getX(), this->position_.getY() + y);
	}

	void Transform::rotate(float rotation) {
		this->rotation_ += rotation;
	}

	void Transform::scale(float scale) {
		this->scale_ *= scale;
	}

}
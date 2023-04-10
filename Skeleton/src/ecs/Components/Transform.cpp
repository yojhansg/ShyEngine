#include "Transform.h"
#include "Vector2D.h"

#include "Entity.h"

namespace ECS {

	Transform::Transform() {
		localPosition = Utilities::Vector2D(0, 0);
		localScale = Utilities::Vector2D(1, 1);
		localRotation = 0;

		parent = nullptr;
	}

	Transform::Transform(const Utilities::Vector2D& position, const Utilities::Vector2D& scale, float rotation) {
		localPosition = position;
		localScale = scale;
		localRotation = rotation;

		parent = nullptr;
	}

	// ------------- Getters -----------------


	Utilities::Vector2D Transform::GetLocalPosition() {
		return localPosition;
	}

	Utilities::Vector2D Transform::GetLocalScale() {
		return localScale;
	}

	float Transform::GetLocalRotation()
	{
		return localRotation;
	}

	void Transform::onDestroy()
	{
		if (parent != nullptr) {

			parent->RemoveChildren(this);
		}
	}

	Utilities::Vector2D* Transform::getPositionPointer()
	{
		return &localPosition;
	}

	Utilities::Vector2D* Transform::getScalePointer()
	{
		return &localScale;
	}

	const double* Transform::getRotationPointer() {
		return &localRotation;
	}

	void Transform::SetParent(Transform* tr)
	{
		if (parent != nullptr) {
			parent->RemoveChildren(this);
		}

		parent = tr;
		if (tr != nullptr)
			tr->SetChildren(this);
	}

	void Transform::SetChildren(Transform* tr)
	{
		children.push_back(tr);
	}

	void Transform::RemoveChildren(Transform* tr)
	{
		children.remove(tr);
	}

	// ------------- Setters -----------------

	void Transform::SetLocalPosition(Utilities::Vector2D position) {
		this->localPosition = position;
	}

	void Transform::SetLocalPositionX(float x) {
		this->localPosition.set(x, localPosition.getY());
	}

	void Transform::SetLocalPositionY(float y) {
		this->localPosition.set(localPosition.getX(), y);
	}

	void Transform::SetWorldPosition(Utilities::Vector2D position)
	{
		if (parent != nullptr) {

			this->localPosition = position - parent->GetWorldPosition();
			return;
		}

		this->localPosition = position;
	}

	void Transform::SetScale(Utilities::Vector2D scale) {
		this->localScale = scale;
	}

	void Transform::SetScaleX(float x) {
		this->localScale.set(x, localScale.getY());
	}

	void Transform::SetScaleY(float x) {
		this->localScale.set(x, localScale.getY());
	}

	void Transform::SetWorldScale(Utilities::Vector2D scale)
	{
		if (parent != nullptr) {
			
			auto parentWorldScale = parent->GetWorldScale();
			this->localScale.x_ = scale.x_ / parentWorldScale.x_;
			this->localScale.y_ = scale.y_ / parentWorldScale.y_;
			return;
		}

		this->localScale = scale;
	}


	void Transform::SetLocalRotation(float rotation) {
		this->localRotation = rotation;
	}

	void Transform::SetWorldRotation(float rotation)
	{
		if (parent != nullptr) {

			this->localRotation = rotation - parent->GetWorldRotation();
			return;
		}

		this->localRotation = rotation;
	}


	Utilities::Vector2D Transform::GetWorldPosition()
	{
		if (parent != nullptr)
			return parent->GetWorldPosition() + localPosition.rotate(parent->GetWorldRotation());

		return localPosition;
	}

	Utilities::Vector2D Transform::GetWorldScale()
	{
		if (parent != nullptr)
			return localScale.mult(parent->GetWorldScale());

		return localScale;
	}

	float Transform::GetWorldRotation()
	{
		if (parent != nullptr)
			return localRotation + parent->GetLocalRotation();

		return localRotation;
	}

	// ------------ Modifiers ------------------

	void Transform::Translate(Utilities::Vector2D direction) {
		this->localPosition += direction;
	}

	void Transform::TranslateX(float x) {
		this->localPosition.set(this->localPosition.getX() + x, this->localPosition.getY());
	}

	void Transform::TranslateY(float y) {
		this->localPosition.set(this->localPosition.getX(), this->localPosition.getY() + y);
	}

	void Transform::Rotate(float rotation) {
		this->localRotation += rotation;
	}

	void Transform::Scale(float scale) {
		this->localScale *= scale;
	}

}
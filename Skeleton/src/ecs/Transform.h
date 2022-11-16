#pragma once

#include "Component.h"
#include "Vector2D.h"

namespace ECS {

	class Transform : public Component{
	public:
		
		Transform();

		// Setters & getters
		Utilities::Vector2D getPosition();
		Utilities::Vector2D getScale();
		float getRotation();

		void setScale(const Utilities::Vector2D& scale);

		// Modifiers
		void translate(const Utilities::Vector2D& position);
		void rotate(float rotation);

	private:

		Utilities::Vector2D position;
		Utilities::Vector2D scale;
		float rotation;
	};

}
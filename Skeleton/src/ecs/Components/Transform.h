#pragma once

#include "Component.h"
#include "Vector2D.h"

#include "Scripting/Variable.h"

class Game;
namespace ECS {

	class Transform : public Component {
	public:

		Transform();

		Transform(const Utilities::Vector2D& position, const Utilities::Vector2D& scale, float rotation);

		friend Game;

	publish:
		// Getters

		//TODO: temporal

		Utilities::Vector2D getPosition();
		Utilities::Vector2D getScale();

		float getRotation();

	public:
		Utilities::Vector2D* getPositionPointer();
		Utilities::Vector2D* getScalePointer();

	public:
		//TODO: que esto no devuelva un puntero o hacer otro metodo para las fisicas
		const double* getRotationPointer();

		// Setters
		//TODO: get rotation
	publish:
		void setPosition(Utilities::Vector2D position);
		void setPositionX(float x);
		void setPositionY(float y);

		void setScale(Utilities::Vector2D scale);
		void setScaleX(float x);
		void setScaleY(float y);
		void setRotation(float rotation);

		void translate(Utilities::Vector2D direction);

		void translateX(float x);
		void translateY(float y);

		void rotate(float rotation);
		void scale(float scale);

	private:

		reflect Utilities::Vector2D position_;
		reflect Utilities::Vector2D scale_;
		reflect double rotation_;
	};

}
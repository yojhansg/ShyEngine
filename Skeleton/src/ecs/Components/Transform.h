#pragma once

#include "Component.h"
#include "Vector2D.h"

#include "Scripting/Variable.h"

namespace ECS {

	class Transform : public Component {
	public:

		Transform();

		Transform(const Utilities::Vector2D& position, const Utilities::Vector2D& scale, float rotation);

	publish:
		// Getters
		Utilities::Vector2D* getPosition();
		Utilities::Vector2D* getScale();

	public:
		const double* getRotation();

		// Setters
		void setPosition(const Utilities::Vector2D& position);
	publish:
		void setPosition(float x, float y);
	public:
		void setScale(const Utilities::Vector2D& scale);
	publish:
		void setScale(float x, float y);
		void setRotation(float rotation);

		// Modifiers
	public:
		void translate(const Utilities::Vector2D& direction);
	publish:
		void translate(float x, float y);
		void rotate(float rotation);
		void scale(float scale);

	private:

		Utilities::Vector2D position_;
		Utilities::Vector2D scale_;
		double rotation_;
	};



	/*Metodos scripting*/
	Scripting::Variable MiMetodo(std::vector<Scripting::Variable>const&);
}
#pragma once

#include "Component.h"

namespace Input {
	class InputManager;
}

namespace ECS {

	class Transform;
	class PhysicBody;

	class TopDownController : public Component {

	public:

		TopDownController();

		void init() override;

		void update(float dt) override;

		void fixedUpdate(float fixedDt) override;

	private:

		Input::InputManager* im;

		Transform* transform;
		PhysicBody* body;

		Utilities::Vector2D direction;

		reflect float velocity;
		reflect bool clampDiagonalVelocity;
		reflect bool usePhysics;

	};

}

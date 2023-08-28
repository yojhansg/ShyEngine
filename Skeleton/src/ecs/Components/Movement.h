#pragma once

#include "Component.h"

namespace Input {
	class InputManager;
}

namespace ECS {

	class Transform;

	class Movement : public Component {

	public:

		Movement();

		void init() override;

		void update(float dt) override;

	private:

		Input::InputManager* im;

		Transform* transform;

		reflect float velocity;

	};

}

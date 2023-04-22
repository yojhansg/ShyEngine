#pragma once

#include "Component.h"

namespace Input {
	class InputManager;
}

namespace ECS {

	class Transform;
	class CircleBody;
	class MusicEmitter;

	class BallComponent : public Component {

	public:

		void init() override;

		void update(float deltaTime) override;

		void onCollisionEnter(Entity* b) override;

		void onCollisionExit(Entity* b) override;

	private:

		Input::InputManager* im;

		MusicEmitter* music;

		Transform* transform;

		CircleBody* body;

		bool onGround;

	};

}


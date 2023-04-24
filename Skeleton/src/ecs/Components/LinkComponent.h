#pragma once

#include "Component.h"

namespace Input {
	class InputManager;
}

namespace ECS {

	class Transform;
	class BoxBody;
	class SoundEmitter;

	class LinkComponent : public Component {

	public:

		void init() override;

		void update(float deltaTime) override;

		void onCollisionEnter(Entity* b) override;

		void onCollisionExit(Entity* b) override;

	private:

		Input::InputManager* im;

		SoundEmitter* sound;

		Transform* transform;

		BoxBody* body;

		bool onGround;

	};
}


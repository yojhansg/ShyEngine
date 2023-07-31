#pragma once

#include "Component.h"

namespace Input {
	class InputManager;
}

namespace ECS {

	class Transform;
	class BoxBody;
	class ParticleSystem;

	class LinkComponent : public Component {

	public:

		void init() override;

		void update(float deltaTime) override;

		void onCollisionEnter(Entity* b) override;

		void onCollisionExit(Entity* b) override;

	private:

		Input::InputManager* im;

		Transform* transform;

		ParticleSystem* pSystem;

		BoxBody* body;

		bool onGround;

	};
}


#pragma once

#include "Component.h"

namespace ECS {

	class Transform;
	class CircleBody;

	class TestComponent : public Component {

	public:

		void start() override;

		void update(float deltaTime) override;

		void fixedUpdate(float fixedDeltaTime) override;

		void onCollisionEnter(Entity* b) override;

		void onCollisionExit(Entity* b) override;

	private:

		Transform* transform;

		CircleBody* body;

		bool onGround;

	};
}


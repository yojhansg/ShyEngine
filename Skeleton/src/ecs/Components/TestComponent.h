#pragma once
#include "Component.h"

namespace ECS {

	class Transform;
	class Rigidbody;

	class TestComponent : public Component {

	public:

		void start() override;

		void update(float deltaTime) override;

		void fixedUpdate(float fixedDeltaTime) override;

		void onCollisionEnter(Entity* a, Entity* b) override;

		void onCollisionExit(Entity* a, Entity* b) override;

	private:

		Transform* transform;

		Rigidbody* rb;

		bool onGround;

	};
}


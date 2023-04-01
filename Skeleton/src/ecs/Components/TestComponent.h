#pragma once

#include "Component.h"

namespace ECS {

	class Transform;
	class BoxBody;

	class TestComponent : public Component {

	public:

		void start() override;

		void update(float deltaTime) override;

		void fixedUpdate(float fixedDeltaTime) override;

		void onCollisionEnter(Entity* b) override;

		void onCollisionExit(Entity* b) override;

		void onTriggerEnter(Entity* b) override;

		void onTriggerExit(Entity* b) override;

		void onCollisionStay(Entity* b) override;

		void onTriggerStay(Entity* b) override;

	private:

		Transform* transform;

		BoxBody* body;

		bool onGround;

	};
}


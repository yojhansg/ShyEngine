#pragma once
#include "Component.h"

namespace ECS {

	class Transform;

	class TestComponent : public Component {

	public:

		void start() override;

		void update(float deltaTime) override;

		void onCollisionEnter(Entity* a, Entity* b) override;

		void onCollisionExit(Entity* a, Entity* b) override;

	private:

		Transform* transform;

	};
}


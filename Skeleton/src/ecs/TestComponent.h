#pragma once
#include "Component.h"

namespace ECS {

	class Transform;

	class TestComponent : public Component {

	public:

		void start() override;

		void update(float deltaTime) override;

	private:

		Transform* transform;

	};
}


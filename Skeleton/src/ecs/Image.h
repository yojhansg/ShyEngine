#pragma once

#include "Component.h"

namespace ECS {

	class Image : public Component {
	public:
		void init() override;

		void update(float deltaTime) override;

		void lateUpdate(float deltaTime) override;

		void fixedUpdate(float fixedDeltaTime) override;

		void handleInput() override;

	private:

	};

}
#pragma once

#include "Component.h"

namespace ECS {

	class Transform : public Component {
	public:
		void init() override;

		void update() override;

		void lateUpdate() override;

		void handleInput() override;

	private:


	};

}
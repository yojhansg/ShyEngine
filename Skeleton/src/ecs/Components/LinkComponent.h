#pragma once

#include "Component.h"

namespace ECS {

	class Transform;
	class BoxBody;
	class SoundEmitter;

	class LinkComponent : public Component {

	public:

		void start() override;

		void update(float deltaTime) override;

		void onCollisionEnter(Entity* b) override;

		void onCollisionExit(Entity* b) override;

	private:

		SoundEmitter* soundEmitter;

		Transform* transform;

		BoxBody* body;

		bool onGround;

	};
}


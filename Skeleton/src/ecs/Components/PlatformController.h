#pragma once

#include "Component.h"

namespace Input {
	class InputManager;
}

namespace ECS {

	class Transform;
	class PhysicBody;
	class Entity;

	class PlatformController : public Component {

	public:

		PlatformController();

		void init() override;

		void update(float dt);

		void fixedUpdate(float fixedDt) override;

		void onCollisionEnter(Entity* ent) override;


		void onCollisionExit(Entity* ent) override;

	private:

		void getPhysicComponentFromEntity();

		Input::InputManager* im;

		Transform* transform;
		PhysicBody* body;

		float direction;

		reflect float velocity;
		reflect float maxVelocity;
		reflect float jumpForce;
		reflect std::string platformLayer;
		reflect float horizontalDamping;

		bool onGround;
		bool hasJumped;
	};

}


#pragma once

#include <Singleton.h>

#include <box2d/b2_world_callbacks.h>

namespace ECS {

	class ContactListener : public b2ContactListener, public Utilities::Singleton<ContactListener> {

		friend Singleton<ContactListener>;

	public:

		~ContactListener() {};

		void BeginContact(b2Contact* contact) override;

		void EndContact(b2Contact* contact) override;

	private:

		ContactListener();

	};

}


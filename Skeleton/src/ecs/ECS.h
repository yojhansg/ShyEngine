#pragma once

#include <Singleton.h>

namespace ECS {

	class ContactListener;

	class ECS : public Utilities::Singleton<ECS> {

		friend Singleton<ECS>;

	public:

		~ECS();

	private:

		ECS();

		ContactListener* contactListener;

	};

}
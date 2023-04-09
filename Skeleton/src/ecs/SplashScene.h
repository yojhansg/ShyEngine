#pragma once

#include "Component.h"

namespace ECS {

	class Entity;
	class Scene;
	class SplashScene
	: public Component
	{
	public:

		SplashScene();
		~SplashScene();


		void update(float dt) override;


		void SetMaxtTime(float time);


		static void CreateSplashScreen(Scene* scene);


	private:

		float timer;
		float maxTime;
	};
}


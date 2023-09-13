#pragma once

#include "Component.h"

#include <string>
#include <Vector2D.h>

namespace ECS {

	class Entity;
	class Scene;

	class SplashSceneManager {

		friend class SplashScene;

	public:

		static SplashSceneManager* Instance();
		static void Release();

		static void LoadSplashScreen();
		static void LoadConfigurationData(const std::string& initialScene);

	private:

		static SplashSceneManager* instance;

		std::string initialScene;

	};

	// Es importante que esten en lineas diferentes para que el ECS reader no lo reconozca como componente

	class SplashScene
		: public Component {

	public:

		SplashScene();
		~SplashScene();

		void update(float dt) override;

	private:

		float timer;
		float maxTime;

	};


}


#pragma once

#include "Component.h"

namespace ECS {

	// TODO Cambiar splahscreen a la del editor (Hay que meter todos los recursos necesarios en Assets, 
	// ojo la carpeta Assets del motor, no la del editor, que esta en el mismo directorio que el exe del motor)

	class Entity;
	class Scene;

	//Es importante que esten en lineas diferentes para que el ECS reader no lo reconozca como componente

	class SplashScene
		: public Component {

	public:

		SplashScene();
		~SplashScene();

		void update(float dt) override;

		void SetMaxtTime(float time);

		static void CreateSplashScreen(Scene* scene);
		static void LoadResources();

	private:

		float timer;
		float maxTime;

	};
}


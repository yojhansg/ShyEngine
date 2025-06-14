#pragma once

namespace Physics {
	class PhysicsManager;
}

namespace Renderer {
	class RendererManager;
}

namespace Input {
	class InputManager;
}

namespace ECS {
	class OverlayManager;
	class RenderManager;
	class SceneManager;
	class PrefabManager;
}

namespace Resources {
	class ResourcesManager;
}

namespace Utilities {
	class Time;
}

class Engine {

public:

	Engine();

	bool init();
	void update();
	void close();

private:

	Resources::ResourcesManager* resourcesManager;
	Renderer::RendererManager* rendererManager;
	Physics::PhysicsManager* physicsManager;
	ECS::OverlayManager* overlayManager;
	Input::InputManager* inputManager;
	ECS::RenderManager* renderManager;
	ECS::SceneManager* sceneManager;
	Utilities::Time* engineTime;

};


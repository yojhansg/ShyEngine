#pragma once

namespace PhysicsManager {
	class PhysicsManager;
}

namespace RendererManager {
	class RendererManager;
}

namespace InputManager {
	class InputManager;
}

namespace SoundManager {
	class SoundManager;
}

namespace ECS {
	class SceneManager;
}

namespace ResourcesManager {
	class ResourcesManager;
}

namespace Utilities {
	class EngineTime;
}

class Engine {

public:

	Engine();

	void init();
	void update();
	void close();

private:

	ECS::SceneManager* sceneManager;
	PhysicsManager::PhysicsManager* physicsManager;
	RendererManager::RendererManager* rendererManager;
	InputManager::InputManager* inputManager;
	SoundManager::SoundManager* soundManager;
	ResourcesManager::ResourcesManager* resourcesManager;

	Utilities::EngineTime* engineTime;
};


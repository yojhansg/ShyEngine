#pragma once

namespace Physics {
	class PhysicsManager;
}

namespace RendererManager {
	class RendererManager;
}

namespace Input {
	class InputManager;
}

namespace SoundManager {
	class SoundManager;
}

namespace ECS {
	class ContactListener;
	class SceneManager;
}

namespace ResourcesManager {
	class ResourcesManager;
}

namespace Utilities {
	class EngineTime;
}


namespace Scripting {
	class ScriptManager;
	class ScriptFunctionality;
}

class Engine {

public:

	Engine();

	void init();
	void update();
	void close();

private:

	ECS::ContactListener* contactListener;
	ECS::SceneManager* sceneManager;
	Physics::PhysicsManager* physicsManager;
	RendererManager::RendererManager* rendererManager;
	Input::InputManager* inputManager;
	SoundManager::SoundManager* soundManager;
	ResourcesManager::ResourcesManager* resourcesManager;
	Scripting::ScriptManager* scriptManager;
	Scripting::ScriptFunctionality* scriptFunctionality;
	Utilities::EngineTime* engineTime;
};


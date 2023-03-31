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

namespace SoundManager {
	class SoundManager;
}

namespace ECS {
	class ContactListener;
	class SceneManager;
}

namespace Resources {
	class ResourcesManager;
}

namespace Utilities {
	class EngineTime;
}


namespace Scripting {
	class ScriptManager;
	class ScriptFunctionality;
}


class ComponentFactory;

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
	Renderer::RendererManager* rendererManager;
	Input::InputManager* inputManager;
	SoundManager::SoundManager* soundManager;
	Resources::ResourcesManager* resourcesManager;
	Scripting::ScriptManager* scriptManager;
	Scripting::ScriptFunctionality* scriptFunctionality;
	Utilities::EngineTime* engineTime;

	ComponentFactory* componentFactory;
};


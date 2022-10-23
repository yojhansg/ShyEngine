#pragma once

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

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

class SDL_Rect;

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
};


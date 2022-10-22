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

class Engine {

public:

	Engine();
	~Engine();

	void init();
	void update();
	void close();

private:

	PhysicsManager::PhysicsManager* pm;
	RendererManager::RendererManager* rm;
	InputManager::InputManager* im;
	SoundManager::SoundManager* sm;

};


#include "LinkComponent.h"
#include "ParticleSystem.h"
#include "InputManager.h"
#include "SoundEmitter.h"
#include "Transform.h"
#include "BoxBody.h"
#include "Entity.h"

#include <ConsoleManager.h>
#include "PrefabManager.h"

namespace ECS {

	void LinkComponent::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		body = this->getEntity()->getComponent<BoxBody>();

		onGround = false;
	}


	void LinkComponent::update(float deltaTime) {

		if (im->keyDownEvent()) {

			if (im->isKeyDown(SDL_SCANCODE_A)) {
				Console::Output::Print("Prefab Manager", "Entidad instanciada");
				PrefabManager::instance()->InstantiatePrefabWithTransform("Link", this->getScene());
			}

		}

	}

	void LinkComponent::onCollisionEnter(Entity* b) {
		onGround = true;
	}

	void LinkComponent::onCollisionExit(Entity* b) {
		onGround = false;
	}

}

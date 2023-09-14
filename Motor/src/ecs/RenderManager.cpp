#include "RenderManager.h"
#include "Entity.h"
#include "SceneManager.h"

ECS::RenderManager::RenderManager() {}

ECS::RenderManager::~RenderManager() {}

void ECS::RenderManager::Render() {
	for (Entity* elem : set) {

		if (elem->active && elem->getScene() == SceneManager::instance()->getCurrentScene())
			elem->render();
	}
}

std::multiset<ECS::Entity*>::iterator ECS::RenderManager::AddElement(Entity* elem) {
	return set.insert(elem);
}

void ECS::RenderManager::RemoveElement(std::multiset<ECS::Entity*>::iterator elem) {
	set.erase(elem);
}

bool ECS::EntityComparer::operator()(ECS::Entity* a, ECS::Entity* b) const {
	return a->GetRenderOrder() < b->GetRenderOrder();
}
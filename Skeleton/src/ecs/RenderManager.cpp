#include "RenderManager.h"
#include "Entity.h"

ECS::RenderManager::RenderManager()
{
}

ECS::RenderManager::~RenderManager()
{
}

void ECS::RenderManager::Render()
{
	for (Entity* elem : set) {

		if (elem->active)
			elem->render();
	}
}

void ECS::RenderManager::AddElement(Entity* elem)
{
	set.insert(elem);
}

void ECS::RenderManager::RemoveElement(Entity* elem)
{
	set.erase(elem);
}




bool ECS::EntityComparer::operator()(ECS::Entity* a, ECS::Entity* b) const
{
	return a->GetRenderOrder() < b->GetRenderOrder();
}
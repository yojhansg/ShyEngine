#include "OverlayManager.h"
#include "Components/OverlayElement.h"

#include "Entity.h"
ECS::OverlayManager::OverlayManager()
{
	isDirty = true;
}

ECS::OverlayManager::~OverlayManager()
{

}

void ECS::OverlayManager::Render()
{
	if (isDirty) {

		for (OverlayElement* overlay : overlays) {

			overlay->RecalculatePosition();
		}

		isDirty = false;
	}

	for (OverlayElement* overlay : overlays) {

		Entity* ent = overlay->getEntity();
		if (ent->active)
			ent->render();
	}
}

void ECS::OverlayManager::SetDirty()
{
	isDirty = true;
}

void ECS::OverlayManager::AddElement(ECS::OverlayElement* elem)
{
	overlays.push_back(elem);
}

void ECS::OverlayManager::RemoveElement(ECS::OverlayElement* elem)
{
	overlays.push_back(elem);
}

#include "OverlayManager.h"
#include "Components/OverlayElement.h"
#include "InputManager.h"
#include "Entity.h"
#include "EngineTime.h"

ECS::OverlayManager::OverlayManager()
{
	isDirty = true;

	timeToHold = 0.3f;
	timeToDoubleClick = 0.5f;
	selected = nullptr;
	lastClickTime = 0;
	holdTimer = 0;
}

ECS::OverlayManager::~OverlayManager()
{

}

void ECS::OverlayManager::Render()
{
	if (isDirty) {

		RecalculateOverlay();
	}

	for (OverlayElement* overlay : overlays) {

		Entity* ent = overlay->getEntity();
		if (ent->active)
			ent->render();
	}
}

void ECS::OverlayManager::Update(float dt)
{
	/*
		TODO:
		
		hoverEnter
		hover
		hoverExit
	*/

	Utilities::Vector2D mousePosition = Input::InputManager::instance()->getMousePos();

	bool hold = Input::InputManager::instance()->isMouseButtonDown(Input::InputManager::LEFT);
	bool clickBegin = Input::InputManager::instance()->isMouseButtonDownEvent(Input::InputManager::LEFT);
	bool click = Input::InputManager::instance()->isMouseButtonUpEvent(Input::InputManager::LEFT);

	bool rightClick = Input::InputManager::instance()->isMouseButtonUpEvent(Input::InputManager::RIGHT);


	if (selected != nullptr) {

		if (!selected->PointInsideBounds(mousePosition)) {

			selected = nullptr;
		}
	}


	if (clickBegin) {

		for (int i = overlays.size() - 1; clickBegin && i >= 0; i--) {

			OverlayElement* elem = overlays[i];

			if (elem->isActive() && elem->getEntity()->active && elem->PointInsideBounds(mousePosition)) {

				selected = elem;
				clickBegin = false;
			}
		}

		if (selected != nullptr) {
			selected->getEntity()->onClickBegin();
		}

		holdTimer = 0;
	}
	else if (click && selected != nullptr) {

		float currentTime = Utilities::EngineTime::instance()->timeSinceStart;

		if (currentTime - lastClickTime < timeToDoubleClick) {

			selected->getEntity()->onDoubleClick();
			lastClickTime = 0;
		}
		else {

			lastClickTime = currentTime;
		}
		selected->getEntity()->onClick();
	}

	if (hold && selected != nullptr) {

		holdTimer += dt;

		if (holdTimer > timeToHold) {

			selected->getEntity()->onClickHold();
		}
	}


	if (rightClick) {

		for (int i = overlays.size() - 1; clickBegin && i >= 0; i--) {

			OverlayElement* elem = overlays[i];

			if (elem->isActive() && elem->getEntity()->active && elem->PointInsideBounds(mousePosition)) {

				elem->getEntity()->onRightClick();
				break;
			}
		}
	}
}

void ECS::OverlayManager::SetDirty()
{
	isDirty = true;
}

bool ECS::OverlayManager::IsDirty()
{
	return isDirty;
}

void ECS::OverlayManager::RecalculateOverlay()
{
	for (OverlayElement* overlay : overlays) {

		overlay->RecalculatePosition();
	}

	isDirty = false;
}

void ECS::OverlayManager::AddElement(ECS::OverlayElement* elem)
{
	overlays.push_back(elem);
}

void ECS::OverlayManager::RemoveElement(ECS::OverlayElement* elem)
{
	if (selected == elem)
		selected = nullptr;

	overlays.erase(std::find(overlays.begin(), overlays.end(), elem));
}

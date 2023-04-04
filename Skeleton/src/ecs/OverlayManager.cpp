#include "OverlayManager.h"
#include "Components/OverlayElement.h"
#include "InputManager.h"
#include "Entity.h"
#include "EngineTime.h"

#include <iostream>

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

void ECS::OverlayManager::Update()
{
	/*
		TODO:

		hoverEnter
		hover
		hoverExit
	*/

	const Utilities::Vector2D mousePosition = Input::InputManager::instance()->getMousePos();

	const bool hold = Input::InputManager::instance()->isMouseButtonDown(Input::InputManager::LEFT);
	const bool clickBegin = Input::InputManager::instance()->isMouseButtonDownEvent(Input::InputManager::LEFT);
	const bool click = Input::InputManager::instance()->isMouseButtonUpEvent(Input::InputManager::LEFT);
	const bool rightClick = Input::InputManager::instance()->isMouseButtonUpEvent(Input::InputManager::RIGHT);


	OverlayElement* newelem = nullptr;

	for (int i = overlays.size() - 1; i >= 0; i--) {

		OverlayElement* elem = overlays[i];

		if (elem->isActive() && elem->getEntity()->active && elem->PointInsideBounds(mousePosition)) {

			newelem = elem;
			break;
		}
	}

	if (newelem == nullptr) {

		if (selected != nullptr) {
			selected->getEntity()->onMouseExit();
		}

		selected = nullptr;
		return;
	}

	if (selected == nullptr) {

		selected = newelem;
		selected->getEntity()->onMouseEnter();
	}

	else if (selected != newelem) {

		selected->getEntity()->onMouseExit();
		selected = newelem;
	}

	selected->getEntity()->onMouseHover();

	if (clickBegin) {

		selected->getEntity()->onClickBegin();

		holdTimer = 0;
	}
	else if (click) {

		selected->getEntity()->onClick();

		float currentTime = Utilities::Time::instance()->timeSinceStart;

		if (currentTime - lastClickTime < timeToDoubleClick) {

			selected->getEntity()->onDoubleClick();
			lastClickTime = 0;
		}
		else {

			lastClickTime = currentTime;
		}
	}

	if (hold) {

		holdTimer += Utilities::Time::instance()->deltaTime;

		if (holdTimer > timeToHold) {

			selected->getEntity()->onClickHold();
		}
	}

	if (rightClick) {

		selected->getEntity()->onRightClick();
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

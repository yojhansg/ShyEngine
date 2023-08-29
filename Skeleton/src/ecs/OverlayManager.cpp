#include "OverlayManager.h"
#include "Components/Overlay.h"
#include "Components/OverlayText.h"
#include "InputManager.h"

#include "Entity.h"
#include "EngineTime.h"

#include <iostream>

ECS::OverlayManager::OverlayManager() {

	isDirty = true;
	selected = nullptr;
	lastClickTime = 0;
	holdTimer = 0;
	timeToDoubleClick = 0.5f;
	timeToHold = 0.1f;
	frameRateEntity = nullptr;
	frameRateText = nullptr;
}

ECS::OverlayManager::OverlayManager(bool debugFrameRate, float timeToDoubleClick, float timeToHold) : timeToHold(timeToHold), timeToDoubleClick(timeToDoubleClick)
{
	isDirty = true;
	selected = nullptr;
	lastClickTime = 0;
	holdTimer = 0;


	if (debugFrameRate) {

		frameRateEntity = new Entity("Frame rate", 0);

		frameRateEntity->addComponent<ECS::Overlay>()->SetPositioned({ 0, 0 }, { 100, 10 });
		frameRateText = frameRateEntity->addComponent<ECS::OverlayText>();

		frameRateText->SetFit("Overflow");
		frameRateText->SetText("Frame rate: 60");
		frameRateText->SetFont("Default.ttf");
		frameRateEntity->init();
		frameRateEntity->start();
	}
	else {

		frameRateEntity = nullptr;
		frameRateText = nullptr;
	}
}

ECS::OverlayManager::~OverlayManager()
{
	if (frameRateEntity) {

		delete frameRateEntity;
		frameRateEntity = nullptr;
		frameRateText = nullptr;
	}
}

void ECS::OverlayManager::Render()
{
	if (isDirty) {

		RecalculateOverlay();
	}

	for (Overlay* overlay : overlays) {

		Entity* ent = overlay->getEntity();
		if (ent->active)
			ent->render();
	}

	if (frameRateEntity) {
		frameRateText->SetText("FrameRate: " + std::to_string((int)Utilities::Time::instance()->GetFrameRate()));
		frameRateEntity->render();
	}
}

void ECS::OverlayManager::Update()
{
	const Utilities::Vector2D mousePosition = Input::InputManager::instance()->getMousePos();

	const bool hold = Input::InputManager::instance()->isMouseButtonDown((int)Input::InputManager::MOUSEBUTTON::LEFT);
	const bool clickBegin = Input::InputManager::instance()->isMouseButtonDownEvent((int)Input::InputManager::MOUSEBUTTON::LEFT);
	const bool click = Input::InputManager::instance()->isMouseButtonUpEvent((int)Input::InputManager::MOUSEBUTTON::LEFT);
	const bool rightClick = Input::InputManager::instance()->isMouseButtonUpEvent((int)Input::InputManager::MOUSEBUTTON::RIGHT);


	Overlay* newelem = nullptr;

	for (int i = overlays.size() - 1; i >= 0; i--) {

		Overlay* elem = overlays[i];

		if (elem->isActive() && elem->getEntity()->active && elem->IsInteractable() && elem->PointInsideBounds(mousePosition)) {

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
		selected->getEntity()->onMouseEnter();
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

	else if (hold) {

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
	for (Overlay* overlay : overlays) {

		overlay->RecalculatePosition();
	}

	isDirty = false;
}

void ECS::OverlayManager::AddElement(ECS::Overlay* elem)
{
	overlays.push_back(elem);
}

void ECS::OverlayManager::RemoveElement(ECS::Overlay* elem)
{
	if (selected == elem)
		selected = nullptr;

	auto it = std::find(overlays.begin(), overlays.end(), elem);
	if (it != overlays.end())
		overlays.erase(it);
}

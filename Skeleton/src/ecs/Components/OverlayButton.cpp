#include "OverlayButton.h"
#include "Overlay.h"
#include "Entity.h"

#include <ConsoleManager.h>


ECS::OverlayButton::OverlayButton() {

	overlay = nullptr;
	defaultColor = Utilities::Color::White();
	hoverColor = Utilities::Color::Grey();
	downColor = Utilities::Color::DarkGrey();

	scale = .9f;
}

void ECS::OverlayButton::init() {
	overlay = entity->getComponent<ECS::Overlay>();

	if (overlay == nullptr) {
		printError("Entity does not contain overlay component. Removing component", "OverlayImage");
		this->remove();
		return;
	}
}

void ECS::OverlayButton::start() {
	overlay->SetColor(defaultColor);
}

void ECS::OverlayButton::onMouseEnter() {
	overlay->SetColor(hoverColor);
	//overlay->SetSize(overlay->GetSize() * scale);
	overlay->SetRenderScale(scale);
}

void ECS::OverlayButton::onMouseExit() {
	overlay->SetColor(defaultColor);
	//overlay->SetSize(overlay->GetSize() / scale);
	//TODO: guardar el valor que tenia antes
	overlay->ResetRenderScale();

}

void ECS::OverlayButton::onClickBegin() {
	overlay->SetColor(downColor);
}

void ECS::OverlayButton::onClickHold() {
	overlay->SetColor(downColor);
}


void ECS::OverlayButton::onClick() {
	overlay->SetColor(hoverColor);
}

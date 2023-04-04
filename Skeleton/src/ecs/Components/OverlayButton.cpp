#include "OverlayButton.h"
#include "Overlay.h"
#include "Entity.h"


ECS::OverlayButton::OverlayButton() {

	overlay = nullptr;
	defaultColor = Utilities::Color::White();
	hoverColor = Utilities::Color::Grey();
	downColor = Utilities::Color::DarkGrey();

	scale = .9f;
}

void ECS::OverlayButton::init()
{
	overlay = entity->getComponent<ECS::Overlay>();
}

void ECS::OverlayButton::start()
{
	overlay->SetColor(defaultColor);
}

void ECS::OverlayButton::onMouseEnter()
{
	overlay->SetColor(hoverColor);
	overlay->SetSize(overlay->GetSize() * scale);

}

void ECS::OverlayButton::onMouseExit()
{
	overlay->SetColor(defaultColor);
	overlay->SetSize(overlay->GetSize() / scale);
}

void ECS::OverlayButton::onClickBegin()
{
	overlay->SetColor(downColor);
}

void ECS::OverlayButton::onClickHold()
{
	overlay->SetColor(downColor);
}


void ECS::OverlayButton::onClick()
{
	overlay->SetColor(hoverColor);
}

#include "OverlayText.h"
#include "ResourcesManager.h"
#include "RendererManager.h"

#include "Entity.h"
#include "Font.h"
#include "Texture.h"
#include "Overlay.h"
#include "SDL.h"
#include "SDL_ttf.h"

#include "CheckML.h"
#include "StringTrim.h"

int ECS::OverlayText::ConvertFit(cString str)
{
	std::string lower = Utilities::tolower(str);

	if (lower == "clamp")
		return (int)Fit::Clamp;
	if (lower == "overflow")
		return (int)Fit::Overflow;
	if (lower == "wrapClamp")
		return (int)Fit::WrapClamp;
	if (lower == "wrapOverflow")
		return (int)Fit::WrapOverflow;
	if (lower == "expand")
		return (int)Fit::Expand;

	return 0;
}

int ECS::OverlayText::ConvertVerticalAlignment(cString str)
{
	std::string lower = Utilities::tolower(str);

	if (str == "top")
		return (int)VerticalAlignment::Top;
	if (str == "center")
		return (int)VerticalAlignment::Center;
	if (str == "bottom")
		return (int)VerticalAlignment::Bottom;


	return 0;
}

int ECS::OverlayText::ConvertHorizontalAlignment(cString str)
{
	std::string lower = Utilities::tolower(str);

	if (str == "left")
		return (int)HorizontalAlignment::Left;
	if (str == "center")
		return (int)HorizontalAlignment::Center;
	if (str == "right")
		return (int)HorizontalAlignment::Right;


	return 0;
}

ECS::OverlayText::OverlayText()
{
	fontSize = 32;
	font = "Assets\\Default.ttf";

	fontPtr = nullptr;
	texture = nullptr;

	fit_internal = 0;
	horizontalAlignment_internal = 0;
	verticalAlignment_internal = 0;

	fit = "";
	horizontalAlignment = "";
	verticalAlignment = "";


	lineSpacing = 50;

	text = "MUY BUENOS DIAS MI CABALLERO";
}

ECS::OverlayText::~OverlayText()
{
	freeTexture();
}

void ECS::OverlayText::init()
{
	fit_internal = ConvertFit(fit);
	horizontalAlignment_internal = ConvertHorizontalAlignment(horizontalAlignment);
	verticalAlignment_internal = ConvertVerticalAlignment(verticalAlignment);

	overlay = entity->getComponent<ECS::Overlay>();

	if (overlay == nullptr) {
		printError("Entity does not contain overlay component. Removing component", "OverlayImage");
		this->remove();
		return;
	}

	if (fontPtr == nullptr)
		createFont();
	if (texture == nullptr)
		createTexture();
}

void ECS::OverlayText::createFont()
{
	fontPtr = Resources::ResourcesManager::instance()->addFont(font, fontSize);
}

void ECS::OverlayText::freeTexture()
{
	if (texture != nullptr) {
		delete texture;
		texture = nullptr;
	}
}

void ECS::OverlayText::createTexture()
{
	freeTexture();

	if ((Fit)fit_internal == Fit::WrapClamp || (Fit)fit_internal == Fit::WrapOverflow)
		texture = fontPtr->CreateWrappedText(text, overlay->GetSize().getX());
	else
		texture = fontPtr->CreateText(text);
}

void ECS::OverlayText::render()
{
	if (texture == nullptr)
		return;

	SDL_Rect destination = { 0, 0, 0, 0 };

	overlay->GetRenderRect(destination.x, destination.y, destination.w, destination.h);

	SDL_Rect source = { 0, 0, texture->getWidth(), texture->getHeight() };

	switch ((Fit)fit_internal) {

	case Fit::WrapClamp:
	case Fit::Clamp: {

		int w = std::min(source.w, destination.w);
		int h = std::min(source.h, destination.h);

		if (horizontalAlignment_internal != (int)HorizontalAlignment::Left) {

			float mult = horizontalAlignment_internal == (int)HorizontalAlignment::Center ? 0.5f : 1;

			source.x = std::max(0, source.w - w) * mult;
			destination.x += std::max(0, destination.w - w) * mult;
		}

		if (verticalAlignment_internal != (int)VerticalAlignment::Top) {

			float mult = verticalAlignment_internal == (int)VerticalAlignment::Center ? 0.5f : 1;

			source.y = std::max(0, source.h - h) * mult;
			destination.y += std::max(0, destination.h - h) * mult;
		}

		source.w = w;
		source.h = h;

		destination.w = source.w;
		destination.h = source.h;

	}

	case Fit::WrapOverflow:
	case Fit::Overflow: {

		float multx = horizontalAlignment_internal == (int)HorizontalAlignment::Left ? 0 : horizontalAlignment_internal == (int)HorizontalAlignment::Center ? 0.5f : 1;
		float multy = verticalAlignment_internal == (int)VerticalAlignment::Top ? 0 : verticalAlignment_internal == (int)VerticalAlignment::Center ? 0.5f : 1;

		destination.x += (destination.w - source.w) * multx;
		destination.y += (destination.h - source.h) * multy;

		destination.h = source.h;

		destination.w = source.w;
		break;
	}

	default:
		break;
	}


	overlay->RenderTexture(texture, &source, &destination);
}


std::string ECS::OverlayText::GetFit()
{
	return fit;
}

void ECS::OverlayText::SetFit(cString fit)
{
	int newFit = ConvertFit(fit);


	if (fontPtr != nullptr && this->fit_internal / 2 != newFit / 2)
	{
		this->fit = fit;
		fit_internal = newFit;
		createTexture();
	}
	else {

		this->fit = fit;
		fit_internal = newFit;
	}


}

std::string ECS::OverlayText::GetVerticalAlignment()
{
	return verticalAlignment;
}

std::string ECS::OverlayText::GetHorizontalAlignment()
{
	return horizontalAlignment;
}

void ECS::OverlayText::SetVerticalAlignment(cString align)
{
	verticalAlignment_internal = ConvertVerticalAlignment(align);
	verticalAlignment = align;
}

void ECS::OverlayText::SetHorizontalAlignment(cString align)
{
	horizontalAlignment_internal = ConvertHorizontalAlignment(align);
	horizontalAlignment = align;
}

void ECS::OverlayText::SetFont(std::string font)
{
	this->font = font;
	if (this->fontPtr != nullptr) {
		createFont();
		createTexture();
	}
}

std::string ECS::OverlayText::GetFont()
{
	return font;
}

std::string ECS::OverlayText::GetText()
{
	return text;
}

void ECS::OverlayText::SetText(std::string text)
{
	this->text = text;

	if (fontPtr != nullptr) {
		createTexture();
	}
}

int ECS::OverlayText::GetPointSize()
{
	return fontSize;
}

void ECS::OverlayText::SetPointSize(int size)
{
	fontSize = size;
	if (fontPtr != nullptr) {
		createFont();
		createTexture();
	}
}

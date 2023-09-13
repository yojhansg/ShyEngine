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

std::string ECS::OverlayText::ConvertFit(int idx)
{
	Fit fitIdx = (Fit)idx;

	switch (fitIdx)
	{
	case ECS::OverlayText::Fit::Clamp:
		return "Clamp";
	case ECS::OverlayText::Fit::Overflow:
		return "Overflow";
	case ECS::OverlayText::Fit::WrapClamp:
		return "WrapClamp";
	case ECS::OverlayText::Fit::WrapOverflow:
		return "WrapOverflow";
	case ECS::OverlayText::Fit::Expand:
		return "Expand";
	}

	return "Clamp";
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


std::string ECS::OverlayText::ConvertVerticalAlignment(int idx)
{
	VerticalAlignment align = (VerticalAlignment)idx;

	switch (align)
	{
	case ECS::OverlayText::VerticalAlignment::Top:
		return "Top";
	case ECS::OverlayText::VerticalAlignment::Center:
		return "Center";
	case ECS::OverlayText::VerticalAlignment::Bottom:
		return "Bottom";
	default:
		break;
	}

	return "Top";
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

std::string ECS::OverlayText::ConvertHorizontalAlignment(int idx)
{
	HorizontalAlignment align = (HorizontalAlignment)idx;

	switch (align)
	{
	case ECS::OverlayText::HorizontalAlignment::Left:
		return "Left";
	case ECS::OverlayText::HorizontalAlignment::Center:
		return "Center";
	case ECS::OverlayText::HorizontalAlignment::Right:
		return "Right";
	default:
		break;
	}

	return "Left";
}


ECS::OverlayText::OverlayText()
{
	fontSize = 32;
	font = "Assets\\Default.ttf";

	fontPtr = nullptr;
	texture = nullptr;

	fit = 0;
	horizontalAlignment = 0;
	verticalAlignment = 0;


	lineSpacing = 50;

	text = "MUY BUENOS DIAS MI CABALLERO";
}

ECS::OverlayText::~OverlayText()
{
	freeTexture();
}

void ECS::OverlayText::init()
{
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

	if ((Fit)fit == Fit::WrapClamp || (Fit)fit == Fit::WrapOverflow)
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

	switch ((Fit)fit) {

	case Fit::WrapClamp:
	case Fit::Clamp: {

		int w = std::min(source.w, destination.w);
		int h = std::min(source.h, destination.h);

		if (horizontalAlignment != (int)HorizontalAlignment::Left) {

			float mult = horizontalAlignment == (int)HorizontalAlignment::Center ? 0.5f : 1;

			source.x = std::max(0, source.w - w) * mult;
			destination.x += std::max(0, destination.w - w) * mult;
		}

		if (verticalAlignment != (int)VerticalAlignment::Top) {

			float mult = verticalAlignment == (int)VerticalAlignment::Center ? 0.5f : 1;

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

		float multx = horizontalAlignment == (int)HorizontalAlignment::Left ? 0 : horizontalAlignment == (int)HorizontalAlignment::Center ? 0.5f : 1;
		float multy = verticalAlignment == (int)VerticalAlignment::Top ? 0 : verticalAlignment == (int)VerticalAlignment::Center ? 0.5f : 1;

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
	return ConvertFit(fit);
}

void ECS::OverlayText::SetFit(cString fit)
{
	int newFit = ConvertFit(fit);


	if (fontPtr != nullptr && this->fit / 2 != newFit / 2)
	{
		this->fit = newFit;
		createTexture();
	}
	else
		this->fit = newFit;
}

std::string ECS::OverlayText::GetVerticalAlignment()
{
	return ConvertVerticalAlignment(verticalAlignment);
}

std::string ECS::OverlayText::GetHorizontalAlignment()
{
	return ConvertHorizontalAlignment(horizontalAlignment);
}

void ECS::OverlayText::SetVerticalAlignment(cString align)
{
	verticalAlignment = ConvertVerticalAlignment(align);
}

void ECS::OverlayText::SetHorizontalAlignment(cString align)
{
	horizontalAlignment = ConvertHorizontalAlignment(align);
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

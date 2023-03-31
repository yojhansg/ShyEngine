#include "OverlayText.h"
#include "ResourcesManager.h"
#include "RendererManager.h"

#include "Entity.h"
#include "Font.h"
#include "Texture.h"
#include "OverlayElement.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include <iostream>
#include "CheckML.h"


ECS::OverlayText::OverlayText()
{
	pointSize = 32;
	path = "1up.ttf";

	font = nullptr;
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

void ECS::OverlayText::start()
{
	overlay = entity->getComponent<ECS::OverlayElement>();

	if (font == nullptr)
		createFont();
	if (texture == nullptr)
		createTexture();
}

void ECS::OverlayText::createFont()
{
	font = Resources::ResourcesManager::instance()->addFont(path, pointSize);
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

	if ((Fit)fit == Fit::WrapClamp || (Fit)fit == Fit::WrapOverflow) {
		texture = font->CreateWrappedText(text, overlay->GetSize().getX());
	}
	else

		texture = font->CreateText(text);
}

void ECS::OverlayText::render()
{

	SDL_Rect destination = { 0, 0, 0, 0 };

	overlay->CalculateDestinationRect(destination.x, destination.y, destination.w, destination.h);


	SDL_Rect source = { 0, 0, texture->getWidth(), texture->getHeight() };

	switch ((Fit)fit) {

	case Fit::WrapClamp:
	case Fit::Clamp: {

		source.w = std::min(source.w, destination.w);
		source.h = std::min(source.h, destination.h);

		destination.w = source.w;
		destination.h = source.h;

		break;
	}

	case Fit::WrapOverflow:
	case Fit::Overflow: {

		destination.w = source.w;
		destination.h = source.h;

		break;
	}

	default:
		break;
	}


	SDL_RenderCopy(Renderer::RendererManager::instance()->getRenderer(),
		texture->getSDLTexture(), &source, &destination);
}


int ECS::OverlayText::GetFit()
{
	return fit;
}

void ECS::OverlayText::SetFit(int fit)
{
	this->fit = fit;
}

int ECS::OverlayText::GetVerticalAlignment()
{
	return verticalAlignment;
}

int ECS::OverlayText::GetHorizontalAlignment()
{
	return horizontalAlignment;
}

void ECS::OverlayText::SetVerticalAlignment(int align)
{
	verticalAlignment = align;
}

void ECS::OverlayText::SetHorizontalAlignment(int align)
{
	horizontalAlignment = align;
}

void ECS::OverlayText::SetFont(std::string font)
{
	path = font;
	createFont();
	createTexture();
}

std::string ECS::OverlayText::GetFont()
{
	return path;
}

std::string ECS::OverlayText::GetText()
{
	return text;
}

void ECS::OverlayText::SetText(std::string text)
{
	this->text = text;

	if (font != nullptr) {
		createTexture();
	}
}

int ECS::OverlayText::GetPointSize()
{
	return pointSize;
}

void ECS::OverlayText::SetPointSize(int size)
{
	pointSize = size;
	createFont();
	createTexture();
}

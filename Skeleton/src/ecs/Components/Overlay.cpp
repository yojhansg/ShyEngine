#include "Overlay.h"
#include "RendererManager.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "OverlayManager.h"
#include "Texture.h"

ECS::Overlay::Overlay() {

	color = Utilities::Color::White();

	parent = nullptr;
	SetPositioned({ 0, 0 }, { 100, 100 });
	SetAnchorCenter();

	render_x = render_y = render_w = render_h = 0;

	renderScale = 1;
	interactable = true;

	OverlayManager::instance()->AddElement(this);
}

ECS::Overlay::~Overlay()
{
	OverlayManager::instance()->RemoveElement(this);
}


int ECS::Overlay::GetPlacement()
{
	return placement;
}


void ECS::Overlay::SetDirty()
{
	OverlayManager::instance()->SetDirty();
}

void ECS::Overlay::SetPositioned(Utilities::Vector2D position, Utilities::Vector2D size)
{
	placement = 0;

	this->position = position;
	this->size = size;

	SetDirty();
}

void ECS::Overlay::SetStreched(int left, int top, int right, int bottom)
{
	placement = 1;


	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;

	SetDirty();
}

void ECS::Overlay::Move(Utilities::Vector2D position)
{
	SetPositioned(this->position + position, size);
}

void ECS::Overlay::MoveTo(Utilities::Vector2D position)
{
	SetPositioned(position, size);
}

void ECS::Overlay::SetSize(Utilities::Vector2D size)
{
	this->size = size;

	if (placement == (int)Placement::Positioned)
		SetDirty();
}

Utilities::Vector2D ECS::Overlay::GetPosition()
{
	return position;
}

Utilities::Vector2D ECS::Overlay::GetSize()
{
	return size;
}

int ECS::Overlay::GetTop()
{
	return top;
}

int ECS::Overlay::GetLeft()
{
	return left;
}

int ECS::Overlay::GetRight()
{
	return right;
}

int ECS::Overlay::GetBottom()
{
	return bottom;
}

void ECS::Overlay::SetTop(int pos)
{
	this->top = pos;

	if (placement == (int)Placement::Stretched) {
		SetDirty();
	}
}

void ECS::Overlay::SetLeft(int pos)
{
	this->left = pos;

	if (placement == (int)Placement::Stretched) {
		SetDirty();
	}
}

void ECS::Overlay::SetRight(int pos)
{
	this->right = pos;

	if (placement == (int)Placement::Stretched) {
		SetDirty();
	}
}

void ECS::Overlay::SetBottom(int pos)
{
	this->bottom = pos;

	if (placement == (int)Placement::Stretched) {
		SetDirty();
	}
}

Utilities::Vector2D ECS::Overlay::GetAnchor()
{
	return anchor;
}

void ECS::Overlay::SetAnchor(Utilities::Vector2D anchor)
{
	this->anchor = anchor;

	SetDirty();
}

void ECS::Overlay::SetAnchorCenter()
{
	SetAnchor({ 0.5f, 0.5f });
}

void ECS::Overlay::SetAnchorTopLeft()
{
	SetAnchor({ 0, 0 });
}

void ECS::Overlay::SetAnchorTopRight()
{
	SetAnchor({ 1, 0 });
}

void ECS::Overlay::SetAnchorBottomLeft()
{
	SetAnchor({ 0, 1 });
}

void ECS::Overlay::SetAnchorBottomRight()
{
	SetAnchor({ 1, 1 });
}

Utilities::Color ECS::Overlay::GetColor()
{
	return color;
}

void ECS::Overlay::SetColor(Utilities::Color color)
{
	this->color = color;
}

float ECS::Overlay::GetRenderScale()
{
	return renderScale;
}

void ECS::Overlay::SetRenderScale(float newRenderScale)
{
	renderScale = newRenderScale;
}

void ECS::Overlay::ResetRenderScale()
{
	SetRenderScale(1);
}

void ECS::Overlay::SetInteractable(bool value)
{
	interactable = value;
}

bool ECS::Overlay::IsInteractable()
{
	return interactable;
}

bool ECS::Overlay::PointInsideBounds(Utilities::Vector2D const& p)
{
	if (OverlayManager::instance()->IsDirty())
		OverlayManager::instance()->RecalculateOverlay();

	return p.x_ > render_x && p.x_ < render_x + render_w &&
		p.y_ > render_y && p.y_ < render_y + render_h;
}

void ECS::Overlay::SetParent(Overlay* element)
{
	parent = element;

	SetDirty();
}

void ECS::Overlay::RecalculatePosition()
{
	CalculateRenderRect(render_x, render_y, render_w, render_h);
}

void ECS::Overlay::GetRenderRect(int& x, int& y, int& w, int& h)
{
	x = render_x;
	y = render_y;
	w = render_w;
	h = render_h;
}

void ECS::Overlay::RenderTexture(Renderer::Texture* texture, SDL_Rect* source, SDL_Rect* destination)
{
	auto SDL_texture = texture->getSDLTexture();

	SDL_SetTextureColorMod(SDL_texture, color.r, color.g, color.b);

	float scale = RenderScale();

	if (scale != 1.f) {

		int new_w = (int)std::round(destination->w * scale);
		int new_h = (int)std::round(destination->h * scale);

		destination->x += (int)std::round((destination->w - new_w) * 0.5f);
		destination->y += (int)std::round((destination->h - new_h) * 0.5f);

		destination->w = new_w;
		destination->h = new_h;
	}

	SDL_RenderCopy(Renderer::RendererManager::instance()->getRenderer(),
		SDL_texture, source, destination);

	SDL_SetTextureColorMod(SDL_texture, 255, 255, 255);
}

float ECS::Overlay::RenderScale()
{
	if (parent != nullptr) {

		return renderScale * parent->RenderScale();
	}

	return renderScale;
}


void ECS::Overlay::CalculateRenderRect(int& x, int& y, int& w, int& h)
{

	switch ((Overlay::Placement)placement)
	{
	case Overlay::Placement::Positioned:
	{
		x = (int)std::round(position.getX());
		y = (int)std::round(position.getY());
		w = (int)std::round(size.getX());
		h = (int)std::round(size.getY());

		x -= anchor.getX() * size.getX();
		y -= anchor.getY() * size.getY();

		if (parent != nullptr) {
			auto parentPos = parent->CalculateCenterPosition();
			x += parentPos.getX();
			y += parentPos.getY();
		}

		break;
	}
	case Overlay::Placement::Stretched:
	{

		int parent_left, parent_top, parent_width, parent_height;

		if (parent != nullptr) {

			parent->CalculateRenderRect(parent_left, parent_top, parent_width, parent_height);
		}
		else {

			auto rend = Renderer::RendererManager::instance();
			parent_left = 0;
			parent_top = 0;
			parent_width = rend->getWidth();
			parent_height = rend->getHeight();
		}

		x = parent_left + left;
		y = parent_top + top;
		w = parent_width - right - left;
		h = parent_height - bottom - top;

		break;
	}
	default:
		//TODO: error
		
		print("Entity with invalid placement", "Overlay");
		break;
	}


}

Utilities::Vector2D ECS::Overlay::CalculateCenterPosition()
{
	Utilities::Vector2D center;
	switch ((Overlay::Placement)placement)
	{
	case Overlay::Placement::Positioned:

		center = position;

		if (parent != nullptr)
			center += parent->CalculateCenterPosition();

		break;

	case Overlay::Placement::Stretched:

		int x, y, w, h;
		CalculateRenderRect(x, y, w, h);

		center = { x + w * anchor.getX(), y + h * anchor.getY() };

		break;

	}
	return center;
}

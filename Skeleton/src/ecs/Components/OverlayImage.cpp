#include "OverlayImage.h"
#include "Overlay.h"
#include "Entity.h"
#include "ResourcesManager.h"
#include "Texture.h"
#include "RendererManager.h"

ECS::OverlayImage::OverlayImage()
{
	//TODO: Textura por defecto del juego
	path = "ground.png";
	texture = nullptr;
	overlay = nullptr;
}

void ECS::OverlayImage::init()
{
	overlay = entity->getComponent<Overlay>();

	if (overlay == nullptr) {
		printError("Entity does not contain overlay component. Removing component", "OverlayImage");
		this->remove();
		return;
	}

	SetTexture(path);
}

void ECS::OverlayImage::render()
{
	SDL_Rect destination = { 0, 0, 0, 0 };
	
	overlay->GetRenderRect(destination.x, destination.y, destination.w, destination.h);

	auto SDL_texture = texture->getSDLTexture();

	auto color = overlay->GetColor();

	SDL_SetTextureColorMod(SDL_texture, color.r, color.g, color.b);
	SDL_RenderCopy(Renderer::RendererManager::instance()->getRenderer(),
		SDL_texture, NULL, &destination);

	SDL_SetTextureColorMod(SDL_texture, 255, 255, 255);

}


void ECS::OverlayImage::SetTexture(std::string newpath)
{
	path = newpath;

	texture = Resources::ResourcesManager::instance()->addTexture(path);
}


std::string ECS::OverlayImage::GetTexture()
{
	return path;
}

#include "OverlayImage.h"
#include "OverlayElement.h"
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

void ECS::OverlayImage::start()
{
	//TODO: Ver si deberia dar error o añadirse uno automaticamente
	overlay = entity->getComponent<OverlayElement>();
	assert(overlay != nullptr, "La entidad debe contener un componente Overlay");

	SetTexture(path);
}

void ECS::OverlayImage::render()
{
	SDL_Rect destination = { 0, 0, 0, 0 };
	
	overlay->CalculateDestinationRect(destination.x, destination.y, destination.w, destination.h);

	SDL_RenderCopy(RendererManager::RendererManager::instance()->getRenderer(),
		texture->getSDLTexture(), NULL, &destination);
}


void ECS::OverlayImage::SetTexture(std::string newpath)
{
	path = newpath;

	texture = ResourcesManager::ResourcesManager::instance()->addTexture(path);
}


std::string ECS::OverlayImage::GetTexture()
{
	return path;
}

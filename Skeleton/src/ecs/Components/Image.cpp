#include "Image.h"
#include "Entity.h"
#include "Transform.h"
#include <SDL_image.h>
#include <Texture.h>
#include <ResourcesManager.h>

namespace ECS {

	Image::Image(const std::string& fileName) {
		renderer = nullptr; texture = nullptr; transform = nullptr; rotationPoint = nullptr;
		srcX = srcY = srcWidth = srcHeight = 0;

		flipX = flipY = false;

		this->fileName = fileName;
	}

	Image::~Image() {
		delete rotationPoint;
	}

	void Image::init() {

		renderer = Renderer::RendererManager::instance()->getRenderer();

		transform = getEntity()->getComponent<Transform>();
		assert(transform != nullptr, "La entidad debe contener un componente Transform");


		ChangeTexture(fileName);

		// Flip
		flipMode();

		// Rotation point
		rotationPoint = new SDL_Point({ srcWidth / 2, srcHeight / 2 });

		entity->AddToRenderSet();
	}

	void Image::render() {

		srcRect = { srcX, srcY, srcWidth, srcHeight };

		int width = texture->getWidth();
		int height = texture->getHeight();

		auto trPos = transform->getPosition();
		auto trScale = transform->getScale();

		float scaledX = (width * trScale.getX());
		float scaledY = (height * trScale.getY());

		float distanceX = (abs(width - scaledX)) / 2;
		float distanceY = (abs(height - scaledY)) / 2;

		distanceX = (trScale.getX() > 1) ? -distanceX : distanceX;
		distanceY = (trScale.getY() > 1) ? -distanceY : distanceY;

		int x = std::round(trPos.getX() - width / 2 + distanceX);
		int y = std::round(trPos.getY() - height / 2 + distanceY);
		int w = std::round(trScale.getX() * width);
		int h = std::round(trScale.getY() * height);

		dstRect = { x, y, w, h };

		//TODO: porque rotation point es un puntero
		rotationPoint->x = w / 2;
		rotationPoint->y = h / 2;

		SDL_RenderCopyEx(renderer, texture->getSDLTexture(), &srcRect, &dstRect, *transform->getRotationPointer(), rotationPoint, mode);
	}

	int Image::getTextureWidth() {
		return texture->getWidth();
	}

	int Image::getTextureHeight() {
		return texture->getHeight();
	}

	void Image::setSrcRect(int x, int y, int w, int h) {
		srcX = x; srcY = y;
		srcWidth = w; srcHeight = h;
	}

	void Image::setFlipX(bool flip) {
		flipX = flip;
		flipMode();
	}

	void Image::setFlipY(bool flip) {
		flipY = flip;
		flipMode();
	}

	void Image::setRotaionPoint(int x, int y) {
		rotationPoint->x = x;
		rotationPoint->y = y;
	}

	Utilities::Vector2D Image::scaledSize()
	{
		auto scale = transform->getScale();
		return { texture->getWidth() * scale.getX(), texture->getHeight() * scale.getY() };
	}

	void Image::ChangeTexture(cstring texturePath)
	{
		fileName = texturePath;
		texture = Resources::ResourcesManager::instance()->addTexture(fileName);
		srcWidth = texture->getWidth(); srcHeight = texture->getHeight();
	}

	void Image::flipMode() {
		if (!flipX && !flipY) mode = SDL_FLIP_NONE;
		else {

			assert(!flipX && flipY || flipX && !flipY, "Image can not be fliped horizontally and vertically.");

			if (flipX) mode = SDL_FLIP_HORIZONTAL;
			else mode = SDL_FLIP_VERTICAL;
		}
	}

}
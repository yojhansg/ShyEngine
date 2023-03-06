#include "Image.h"
#include "Entity.h"
#include "Transform.h"
#include <SDL_image.h>
#include <Texture.h>
#include <ResourcesManager.h>

namespace ECS {

	Image::Image(const std::string& fileName) {
		renderer = nullptr; texture = nullptr; transform = nullptr; rotationPoint = nullptr;
		width = height = srcX = srcY = srcWidth = srcHeight = 0;

		flipX = flipY = false;

		this->fileName = fileName;
	}

	Image::~Image() {
		delete rotationPoint;
	}
	
	void Image::init() {

		renderer = RendererManager::RendererManager::instance()->getRenderer();

		transform = getEntity()->getComponent<Transform>();
		assert(transform != nullptr, "La entidad debe contener un componente Transform");

		auto t = ResourcesManager::ResourcesManager::instance()->addTexture(fileName);

		texture = t->getSDLTexture(); width = t->getWidth(); height = t->getHeight();

		srcWidth = width; srcHeight = height;

		// Flip
		flipMode();

		// Rotation point
		rotationPoint = new SDL_Point({ width / 2, height / 2 });
	}

	void Image::render() {

		srcRect = { srcX, srcY, srcWidth, srcHeight };

		int x = std::round(transform->getPosition()->getX());
		int y = std::round(transform->getPosition()->getY());
		int w = std::round(transform->getScale()->getX() * width);
		int h = std::round(transform->getScale()->getY() * height);

		dstRect = { x, y, w, h };

		SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, *transform->getRotation(), rotationPoint, mode);
	}

	int Image::getWidth() {
		return width;
	}

	int Image::getHeight() {
		return height;
	}

	void Image::setSrcRect(int x, int y, int w, int h) {
		srcX = x; srcY = y;
		srcWidth = w; srcHeight = h;
	}

	void Image::setWidth(float w) {
		width = w;
	}

	void Image::setHeight(float h) {
		height = h;
	}

	void Image::setSize(float w, float h) {
		width = w;
		height = h;
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

	void Image::flipMode() {
		if (!flipX && !flipY) mode = SDL_FLIP_NONE;
		else {

			assert(!flipX && flipY || flipX && !flipY, "Image can not be fliped horizontally and vertically.");

			if (flipX) mode = SDL_FLIP_HORIZONTAL;
			else mode = SDL_FLIP_VERTICAL;
		}
	}

}
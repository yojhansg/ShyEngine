#include "Image.h"
#include "Entity.h"
#include "Transform.h"

#include <ScriptFunctionality.h>
#include <ResourcesManager.h>
#include <RendererManager.h>
#include <SDL_image.h>
#include <Texture.h>

namespace ECS {

	Image::Image(const std::string& fileName) {
		renderer = nullptr; texture = nullptr; transform = nullptr; point = nullptr;
		srcX = srcY = srcWidth = srcHeight = 0; flipmode = 0;

		this->fileName = fileName;
	}

	Image::~Image() {
		delete point;
	}

	void Image::init() {

		renderer = Renderer::RendererManager::instance()->getRenderer();

		transform = getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		loadTexture(fileName);

		// Rotation point
		point = new SDL_Point({ srcWidth / 2, srcHeight / 2 });

		entity->AddToRenderSet();
	}

	void Image::render() {

		srcRect = { srcX, srcY, srcWidth, srcHeight };

		int width = srcRect.w;
		int height = srcRect.h;

		auto trPos = transform->GetWorldPosition();
		auto trScale = transform->GetWorldScale();

		int w = std::round(trScale.getX() * width);
		int h = std::round(trScale.getY() * height);

		int x = std::round(trPos.getX() - w * 0.5f);
		int y = std::round(trPos.getY() + h * 0.5f);

		Renderer::RendererManager::instance()->AdjustRectToCamera(&x, &y, &w, &h);

		dstRect = { x, y, w, h };

		point->x = rotationPoint.getX() * w;
		point->y = rotationPoint.getY() * h;

		SDL_RenderCopyEx(renderer, texture->getSDLTexture(), &srcRect, &dstRect, transform->GetWorldRotation(), point, (SDL_RendererFlip) flipmode);
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

	void Image::setRotaionPoint(int x, int y) {
		point->x = x;
		point->y = y;
	}

	Utilities::Vector2D Image::scaledSize() {
		auto scale = transform->GetLocalScale();
		return { texture->getWidth() * scale.getX(), texture->getHeight() * scale.getY() };
	}

	void Image::loadTexture(cstring texturePath) {
		fileName = texturePath;
		texture = Resources::ResourcesManager::instance()->addTexture(fileName);
		srcWidth = texture->getWidth(); srcHeight = texture->getHeight();
	}

	void Image::setFlipMode(int flipmode) {
		this->flipmode = flipmode;
	}
}
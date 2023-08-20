#include "Camera.h"
#include "imgui.h"
#include "GameObject.h"
#include "SDL.h"

namespace ShyEditor {

	Camera::Camera(ImVec2 position, float sf)
	{
		scrollFactor = sf;

		middleMouseButton = false;

		clickOffsetX = clickOffsetY = 0;
		mousePosX = mousePosY = 0;

		posX = position.x;
		posY = position.y;
	}

	void Camera::handleInput(SDL_Event* event, bool isMouseInsideWindow)
	{
		if (isMouseInsideWindow) {
			if (event->type == SDL_MOUSEBUTTONDOWN)
			{
				if (!middleMouseButton && event->button.button == SDL_BUTTON_MIDDLE)
				{
					middleMouseButton = true;

					clickOffsetX = mousePosX - posX;
					clickOffsetY = mousePosY - posY;
				}
			}

			if (event->type == SDL_MOUSEMOTION)
			{
				mousePosX = event->motion.x;
				mousePosY = event->motion.y;

				if (middleMouseButton)
				{
					posX = mousePosX - clickOffsetX;
					posY = mousePosY - clickOffsetY;
				}
			}

			if (event->type == SDL_MOUSEWHEEL && !(SDL_GetModState() & KMOD_SHIFT)) {
				if (event->wheel.y > 0) // scroll up
				{
					scrollFactor += 0.025;
				}
				else if (event->wheel.y < 0) // scroll down
				{
					scrollFactor -= 0.025;
				}
			}
		}

		if (event->type == SDL_MOUSEBUTTONUP)
		{
			if (middleMouseButton && event->button.button == SDL_BUTTON_MIDDLE)
			{
				middleMouseButton = false;
			}
		}
	}

	ImVec2 Camera::getPosition()
	{
		return ImVec2(posX, posY);
	}

	float Camera::getScrollFactor() {
		return scrollFactor;
	}

}


#include "Camera.h"
#include "imgui.h"
#include "GameObject.h"
#include "SDL.h"

Camera::Camera(ImVec2 position, float sf)
{
	scrollFactor = sf;

	rightMouseButtonDown = false;

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
			if (!rightMouseButtonDown && event->button.button == SDL_BUTTON_RIGHT)
			{
				rightMouseButtonDown = true;

				clickOffsetX = mousePosX - posX;
				clickOffsetY = mousePosY - posY;
			}
		}

		if (event->type == SDL_MOUSEMOTION)
		{
			mousePosX = event->motion.x;
			mousePosY = event->motion.y;

			if (rightMouseButtonDown)
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
		if (rightMouseButtonDown && event->button.button == SDL_BUTTON_RIGHT)
		{
			rightMouseButtonDown = false;
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


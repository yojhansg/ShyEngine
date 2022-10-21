#include "Camera.h"
#include "imgui.h"
#include "Image.h"
#include "SDL.h"

Camera::Camera(ImVec2 position, float sf)
{
	scrollFactor = sf;

	leftMouseButtonDown = false;

	posX = position.x;
	posY = position.y;
}

void Camera::handleInput(SDL_Event* event)
{
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (!leftMouseButtonDown && event->button.button == SDL_BUTTON_LEFT)
		{
			leftMouseButtonDown = true;
		
			clickOffsetX = mousePosX - posX;
			clickOffsetY = mousePosY - posY;
		}
	}

	if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (leftMouseButtonDown && event->button.button == SDL_BUTTON_LEFT)
		{
			leftMouseButtonDown = false;
		}
	}

	if(event->type  == SDL_MOUSEMOTION)
	{
		mousePosX = event->motion.x;
		mousePosY = event->motion.y;

		if (leftMouseButtonDown)
		{
			posX = mousePosX - clickOffsetX;
			posY = mousePosY - clickOffsetY;
		}
	}

	if (event->type == SDL_MOUSEWHEEL) {
		if (event->wheel.y > 0) // scroll up
		{
			scrollFactor += 0.05;
		}
		else if (event->wheel.y < 0) // scroll down
		{
			scrollFactor -= 0.05;
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


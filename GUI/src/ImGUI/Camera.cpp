#include "Camera.h"

#include "LogManager.h"
#include "GameObject.h"
#include "imgui.h"
#include "SDL.h"

#include "CheckML.h"

namespace ShyEditor {

	Camera::Camera(ImVec2 position, float scale, SDL_Renderer* renderer)
	{
		this->scale = scale;

		middleMouseButton = false;

		camera_position_x = position.x;
		camera_position_y = position.y;

		this->renderer = renderer;

		texture_h = texture_w = 0;
		targetTexture = nullptr;

		maxScale = 10;
		minScale = 0.1;
	}

	Camera::~Camera()
	{
		if (targetTexture != nullptr) {
			SDL_DestroyTexture(targetTexture);
			targetTexture = nullptr;
		}
	}


	void Camera::GenerateTexture(int w, int h)
	{
		if (targetTexture != nullptr) {
			SDL_DestroyTexture(targetTexture);
			targetTexture = nullptr;
		}

		texture_w = w;
		texture_h = h;
		targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

		if (targetTexture == NULL) {
			LogManager::LogError("Could not create the target texture for the camera.");
			return;
		}

		SDL_SetTextureBlendMode(targetTexture, SDL_BLENDMODE_BLEND);

	}



	void Camera::handleInput(SDL_Event* event, bool isMouseInsideWindow, bool isFocused)
	{

		if (isMouseInsideWindow && event->type == SDL_MOUSEBUTTONDOWN)
		{
			if (!middleMouseButton && event->button.button == SDL_BUTTON_MIDDLE)
			{
				middleMouseButton = true;
			}
		}

		if (event->type == SDL_MOUSEBUTTONUP)
		{
			if (middleMouseButton && event->button.button == SDL_BUTTON_MIDDLE)
			{
				middleMouseButton = false;
			}
		}

		if (event->type == SDL_MOUSEMOTION)
		{
			if (middleMouseButton)
			{
				camera_position_x -= event->motion.xrel;
				camera_position_y -= event->motion.yrel;

			}
		}


#define ScrollSpeed 0.025f

		if (isFocused && isMouseInsideWindow) {
			if (event->type == SDL_MOUSEWHEEL && !(SDL_GetModState() & KMOD_SHIFT)) {

				float pscale = scale;

				if (event->wheel.y > 0) // scroll up
				{
					scale += ScrollSpeed;

					if (scale > maxScale) {

						scale = maxScale;
					}

				}
				else if (event->wheel.y < 0) // scroll down
				{
					scale -= ScrollSpeed;

					if (scale < minScale) {

						scale = minScale;
					}
				}
			}
		}
	}

	void Camera::SetConstrains(float min, float max)
	{
		minScale = min;
		maxScale = max;
	}

	ImVec2 Camera::GetPosition()
	{
		return ImVec2(-camera_position_x, -camera_position_y);
	}

	float& Camera::GetScale() {
		return scale;
	}

	void Camera::SetPosition(float x, float y)
	{
		camera_position_x = x;
		camera_position_y = y;
	}

	void Camera::AddPosition(float x, float y)
	{
		camera_position_x += x;
		camera_position_y += y;
	}

	float Camera::GetMinScale()
	{
		return minScale;
	}

	float Camera::GetMaxScale()
	{
		return maxScale;
	}

	bool Camera::ShouldResize(int w, int h)
	{
		return targetTexture == nullptr || w != texture_w || h != texture_h;
	}

	void Camera::Resize(int w, int h)
	{
		GenerateTexture(w, h);
	}

	void Camera::PrepareCameraRender()
	{
		if (targetTexture != NULL) {
			SDL_SetRenderTarget(renderer, targetTexture);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_RenderClear(renderer);
		}
	}

	void Camera::StopCameraRender()
	{
		SDL_SetRenderTarget(renderer, NULL);
	}

	SDL_Texture* Camera::GetTexture()
	{
		return targetTexture;
	}

	void Camera::CenterPosition(float& x, float& y)
	{
		x += texture_w * 0.5f;
		y += texture_h * 0.5f;
	}

}


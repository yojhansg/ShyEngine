#include "Camera.h"
#include "imgui.h"
#include "Image.h"

Camera::Camera(ImVec2 size, ImVec2 position)
{
	sizeX = size.x;
	sizeY = size.y;

	posX = position.x;
	posY = position.y;
}

void Camera::render()
{
	for (auto img : images) {
		img->render();
	}
}

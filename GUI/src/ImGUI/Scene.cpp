#include "Scene.h"
#include "SDL.h"
#include "GameObject.h"
#include "Window.h"
#include "imgui.h"
#include "Camera.h"
#include "ImGUIManager.h"

PEditor::Scene::Scene(): Window("Scene", NoMove | NoResize | NoCollapse)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	imGUIManager->setScene(this);

	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(1095 * mainWindowSize.x / 1920, 755 * mainWindowSize.y / 1080);

	oriWidth = windowSize.x;
	oriHeight = windowSize.y;

	setSize(windowSize);
	setPosition(ImVec2(mainWindowSize.x / 2 - windowSize.x / 2, 20));

	oriPosX = posX;
	oriPosY = posY;

	addGameObject("test1.jpg");

	camera = new Camera(ImVec2(50, 50), 1);

	renderer = imGUIManager->getRenderer();

	targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, oriWidth -15 , oriHeight -35);

	selectedGameObject = gameObjects[0];
}

void PEditor::Scene::addGameObject(std::string path)
{
	gameObjects.push_back(new PEditor::GameObject(path));
}

std::vector<PEditor::GameObject*> PEditor::Scene::getGameObjects()
{
	return gameObjects;
}

PEditor::GameObject* PEditor::Scene::getSelectedGameObject()
{
	return selectedGameObject;
}

bool PEditor::Scene::entityOutsideCamera(ImVec2 pos, float width, float height)
{
	//TODO
	/*if (pos.x > width || pos.y > height || pos.x + width <= 0 || pos.y + height <= 0) {
		return true;
	}

	return false;*/
	return true;
}

void PEditor::Scene::handleInput(SDL_Event* event)
{
	int mouseX, mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	if ((mouseX > posX && mouseX < posX + width + 15 && mouseY > posY && mouseY < posY + height + 35) || event->type == SDL_MOUSEBUTTONUP)
		camera->handleInput(event);
}

void PEditor::Scene::render()
{
	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);


	ImGui::SetWindowSize(ImVec2(width, height));
	ImGui::SetWindowPos(ImVec2(posX, posY));

	SDL_SetRenderTarget(renderer, targetTexture);
	SDL_RenderClear(renderer);
	for (auto gameObject : gameObjects) {

		ImVec2 position = ImVec2(gameObject->getPosition().x + camera->getPosition().x, gameObject->getPosition().y + camera->getPosition().y);
		float width = gameObject->getWidth() * camera->getScrollFactor();
		float height = gameObject->getHeight() * camera->getScrollFactor();

		//if (entityOutsideCamera(position, width, height)) continue;

		SDL_Rect dst = { position.x, position.y, width, height };
		SDL_RenderCopy(renderer, gameObject->getTexture(), NULL, &dst);
	}

	SDL_SetRenderTarget(renderer, NULL);

	ImGui::Image(targetTexture, ImVec2(width - 15, height - 35));

	ImGui::End();

}


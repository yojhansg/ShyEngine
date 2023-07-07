#include "Scene.h"
#include "SDL.h"
#include "GameObject.h"
#include "Window.h"
#include "imgui.h"
#include "Camera.h"
#include "ImGUIManager.h"
#include <string>

bool PEditor::Scene::mouseInsideWindow()
{
	ImVec2 mousePos = ImGui::GetMousePos();

	float mouseX = mousePos.x;
	float mouseY = mousePos.y;

	if ((mouseX > windowPosX && mouseX < windowPosX + windowWidth && mouseY > windowPosY + 15 && mouseY < windowPosY + windowHeight))
		return true;

	return false;
}

bool PEditor::Scene::mouseInsideGameObject(GameObject* go)
{
	ImVec2 mousePos = ImGui::GetMousePos();

	float cameraOffsetX = camera->getPosition().x * camera->getScrollFactor() * windowWidth / windowOriWidth;
	float cameraOffsetY = camera->getPosition().y * camera->getScrollFactor() * windowWidth / windowOriWidth;

	float mouseX = mousePos.x - windowPosX - 8 - cameraOffsetX;
	float mouseY = mousePos.y - windowPosY - 27 - cameraOffsetY;

	ImVec2 gameObjectPosition = go->getPosition();

	float goPosX = gameObjectPosition.x;
	float goPosY = gameObjectPosition.y;

	float width = go->getWidth() * camera->getScrollFactor();
	float height = go->getHeight() * camera->getScrollFactor();

	if ((mouseX > goPosX && mouseX < goPosX + width && mouseY > goPosY && mouseY < goPosY + height)) {
		return true;
	}

	return false;
}

PEditor::Scene::Scene(): Window("Scene", NoMove | NoResize | NoCollapse | NoScrollbar | NoScrollWithMouse)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	imGUIManager->setScene(this);

	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(1095 * mainWindowSize.x / 1920, 755 * mainWindowSize.y / 1080);

	windowOriWidth = windowSize.x;
	windowOriHeight = windowSize.y;

	setSize(windowSize);
	setPosition(ImVec2(mainWindowSize.x / 2 - windowSize.x / 2, 20));

	windowOriPosX = windowPosX;
	windowOriPosY = windowPosY;

	addGameObject("test1.jpg");

	camera = new Camera(ImVec2(50, 50), 0.5);

	renderer = imGUIManager->getRenderer();

	targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowOriWidth , windowOriHeight);

	selectedGameObject = nullptr;
}

PEditor::Scene::~Scene()
{
	for (auto gameObject : gameObjects) {
		delete gameObject;
	}

	delete camera;
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

void PEditor::Scene::setSelectedGameObject(GameObject* go)
{
	selectedGameObject = go;
}


void PEditor::Scene::renderGameObjects()
{
	for (auto gameObject : gameObjects) {

		gameObject->render(renderer, camera);
	}
}

void PEditor::Scene::renderFrame()
{
	//SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	ImVec2 position = ImVec2(camera->getPosition().x * camera->getScrollFactor(),camera->getPosition().y * camera->getScrollFactor());

	ImVec2 gameSize = ImGUIManager::getInstance()->getGameSize();

	float width = gameSize.x * camera->getScrollFactor();
	float height = gameSize.y * camera->getScrollFactor();

	SDL_Rect frameRect = { position.x, position.y, width, height };
	SDL_RenderDrawRect(renderer, &frameRect);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void PEditor::Scene::handleInput(SDL_Event* event)
{
	for (auto gameObject : gameObjects) {
		if (mouseInsideGameObject(gameObject) && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			selectedGameObject = gameObject;
		}
	}

	if ((mouseInsideWindow() || (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT))) {
		camera->handleInput(event);
	}

}

void PEditor::Scene::render()
{
	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);


	ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));
	ImGui::SetWindowPos(ImVec2(windowPosX, windowPosY));

	SDL_SetRenderTarget(renderer, targetTexture);
	SDL_RenderClear(renderer);

	renderGameObjects();
	renderFrame();

	SDL_SetRenderTarget(renderer, NULL);

	ImGui::Image(targetTexture, ImVec2(windowWidth, windowOriHeight * windowWidth / windowOriWidth));

	ImGui::End();

}


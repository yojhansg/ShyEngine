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

bool PEditor::Scene::entityOutsideCamera(ImVec2 pos, float width, float height)
{
	//TODO: SET SCREENWIDTH AND HEIGHT CONSTANT
	float screenWidth = 1080;
	float screenHeight = 720;

	if (pos.x > screenWidth || pos.y > screenHeight || pos.x + width <= 0 || pos.y + height <= 0) {
		return true;
	}

	return false;
}

void PEditor::Scene::renderGameObjects()
{
	for (auto gameObject : gameObjects) {

		ImVec2 position = gameObject->getPosition();
		float width = gameObject->getWidth();
		float height = gameObject->getHeight();

		if (entityOutsideCamera(position, width, height)) continue;

		//Posicion y tamaños relativos al frame de la escena
		ImVec2 relativePosition = ImVec2((position.x + camera->getPosition().x) * camera->getScrollFactor(),
								(position.y + camera->getPosition().y) * camera->getScrollFactor());

		float relativeWidth = width * camera->getScrollFactor();
		float relativeHeight = height * camera->getScrollFactor();

		SDL_Rect dst = { relativePosition.x, relativePosition.y, relativeWidth, relativeHeight };
		SDL_RenderCopy(renderer, gameObject->getTexture(), NULL, &dst);
	
		if (gameObject == selectedGameObject) {

			//SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
			Uint8 r, g, b, a;
			SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

			SDL_Rect outlineRect = { relativePosition.x, relativePosition.y, relativeWidth, relativeHeight };
			int thickness = 3; // Desired thickness of the outline

			// Draw multiple rectangles with increasing sizes to achieve the desired thickness
			for (int i = 0; i < thickness; i++) {
				SDL_Rect currentRect = { outlineRect.x - i, outlineRect.y - i, outlineRect.w + i * 2, outlineRect.h + i * 2 };
				SDL_RenderDrawRect(renderer, &currentRect);
			}
		}
	}
}

void PEditor::Scene::renderFrame()
{
	//SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	ImVec2 position = ImVec2(camera->getPosition().x * camera->getScrollFactor(),camera->getPosition().y * camera->getScrollFactor());

	//TODO: SET SCREENWIDTH AND HEIGHT CONSTANT
	float width = 1080 * camera->getScrollFactor();
	float height = 720 * camera->getScrollFactor();

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


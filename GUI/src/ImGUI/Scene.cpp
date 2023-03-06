#include "Scene.h"
#include "SDL.h"
#include "GameObject.h"
#include "Window.h"
#include "imgui.h"
#include "Camera.h"
#include "ImGUIManager.h"

PEditor::Scene::Scene()
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	//SCENE WINDOW
	PEditor::Window* sceneWindow = new PEditor::Window("Scene", NoMove | NoResize | NoCollapse);
	ImVec2 sceneSize = ImVec2(1095 * mainWindowSize.x / 1920, 755 * mainWindowSize.y / 1080);
	sceneWindow->setSize(sceneSize);
	sceneWindow->setPosition(ImVec2(mainWindowSize.x / 2 - sceneSize.x / 2, 20));

	addImage("test1.jpg");

	imGUIManager->addWindow(sceneWindow);
	imGUIManager->setScene(this);

	camera = new Camera(ImVec2(50, 50), 1);

	window = sceneWindow;
	window->addComponent(this);

	renderer = ImGUIManager::getInstance()->getRenderer();

	// -15 and - 35 is to get the resolution without the offset (TODO: create variable so there are no arbitrary numbers)
	ImVec2 size = window->getSize();
	width = size.x - 15;
	height = size.y - 35;

	ImVec2 position = window->getPosition();
	posX = position.x;
	posY = position.y;

	targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

}

void PEditor::Scene::addImage(std::string path)
{
	gameObjects.push_back(new PEditor::GameObject(path));
}

std::vector<PEditor::GameObject*> PEditor::Scene::getGameObjects()
{
	return gameObjects;
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
	SDL_SetRenderTarget(renderer, targetTexture);
	SDL_RenderClear(renderer);
	for (auto gameObject : gameObjects) {

		ImVec2 position = ImVec2(gameObject->getPosition().x + camera->getPosition().x , gameObject->getPosition().y + camera->getPosition().y);
		float width = gameObject->getWidth() * camera->getScrollFactor();
		float height = gameObject->getHeight() * camera->getScrollFactor();

		//if (entityOutsideCamera(position, width, height)) continue;

		SDL_Rect dst = { position.x, position.y, width, height };
		SDL_RenderCopy(renderer, gameObject->getTexture(), NULL, &dst);
	}

	SDL_SetRenderTarget(renderer, NULL);

	ImGui::Image(targetTexture, ImVec2(width, height));
}


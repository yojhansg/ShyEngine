#include "Window.h"
#include "WindowComponent.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "SDL.h"
#include "SDL_image.h"

PEditor::Window::Window(std::string _windowName, WindowFlags f = None)
{
	flags = f;
	windowName = _windowName;


	width = 50;
	height = 50;

	posX = 0;
	posY = 0;
}

void PEditor::Window::addComponent(WindowComponent* component)
{
	components.push_back(component);
}

void PEditor::Window::setSize(ImVec2 size)
{
	width = size.x;
	height = size.y;
}
void PEditor::Window::setPosition(ImVec2 position)
{
	posX = position.x;
	posY = position.y;
}


ImVec2 PEditor::Window::getSize()
{
	return ImVec2(width, height);
}

ImVec2 PEditor::Window::getPosition()
{
	return ImVec2(posX, posY);
}


void PEditor::Window::update()
{
}

void PEditor::Window::render()
{	
	ImGui::Begin(windowName.c_str(),(bool*)0, (ImGuiWindowFlags_) flags);


	ImGui::SetWindowSize(ImVec2(width, height));

	ImGui::SetWindowPos(ImVec2(posX, posY));

	for(auto component : components)
	{
		component->render();
	}


	ImGui::End();
}

void PEditor::Window::handleInput(SDL_Event* event)
{
	for (auto component : components)
	{
		component->handleInput(event);
	}
}

PEditor::Window::~Window()
{
	for (auto component : components)
	{
		delete component;
	}
}



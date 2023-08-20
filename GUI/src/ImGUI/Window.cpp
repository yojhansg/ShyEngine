#include "Window.h"

#include "imgui_impl_sdlrenderer.h"
#include "imgui_impl_sdl.h"
#include "SDL_image.h"
#include "Editor.h"
#include "imgui.h"
#include "SDL.h"

namespace ShyEditor {

	Window::Window(std::string _windowName, WindowFlags f = None)
	{
		flags = f;
		windowName = _windowName;

		canBeDisplayedOnTop = false;

		windowWidth = windowOriWidth = 50;
		windowHeight = windowOriHeight = 50;

		windowPosX = 0;
		windowPosY = 0;

		focused = false;
	}

	void Window::setSize(ImVec2 size)
	{
		windowWidth = size.x;
		windowHeight = size.y;
	}
	void Window::setPosition(ImVec2 position)
	{
		windowPosX = position.x;
		windowPosY = position.y;
	}


	ImVec2 Window::getSize()
	{
		return ImVec2(windowWidth, windowHeight);
	}

	ImVec2 Window::getPosition()
	{
		return ImVec2(windowPosX, windowPosY);
	}

	void Window::update() {}

	void Window::render()
	{
		ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);


		ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

		ImGui::SetWindowPos(ImVec2(windowPosX, windowPosY));


		ImGui::End();
	}

	void Window::handleInput(SDL_Event* event) {}

	bool Window::isFocused()
	{
		return focused;
	}

	Window::~Window() {}

	bool Window::CanBeDrawnOnTop()
	{
		return canBeDisplayedOnTop;
	}

}


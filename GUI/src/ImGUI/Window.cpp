#include "Window.h"

#include "imgui_impl_sdlrenderer2.h"
#include "imgui_impl_sdl2.h"
#include "SDL_image.h"
#include "Editor.h"
#include "imgui.h"
#include "SDL.h"


#include "imgui_internal.h"

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
		visible = true;
		docked = false;

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

	void Window::Behaviour()
	{

	}

	void Window::HandleInput(SDL_Event* event) {}

	void Window::UpdateWindow()
	{
		if (visible) {

			if (!docked)
			{

				ImGui::SetNextWindowPos(ImVec2(windowPosX, windowPosY), ImGuiCond_Once);
				ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Once);
			}

			ImGui::Begin(windowName.c_str(), (bool*)0, /*(ImGuiWindowFlags_)flags*/0);

			Behaviour();

			auto pos = ImGui::GetWindowPos();
			auto size = ImGui::GetWindowSize();

			windowPosX = pos.x;
			windowPosY = pos.y;

			windowWidth = size.x;
			windowHeight = size.y;


			docked = ImGui::IsWindowDocked();
			focused = ImGui::IsWindowFocused();


			ImGui::End();

		}

	}

	bool Window::isFocused()
	{
		return focused;
	}

	Window::~Window() {}

	bool Window::CanBeDrawnOnTop()
	{
		return canBeDisplayedOnTop;
	}

	void Window::Hide()
	{
		visible = false;
	}

	void Window::Show()
	{
		visible = true;
	}

	bool Window::IsMouseHoveringWindow()
	{
		auto mouse = ImGui::GetMousePos();

		return mouse.x > windowPosX && mouse.x < windowPosX + windowWidth &&
			mouse.y > windowPosY && mouse.y < windowPosY + windowHeight;
	}


}


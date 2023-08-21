#include "Window.h"

#include "imgui_impl_sdlrenderer2.h"
#include "imgui_impl_sdl2.h"
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
		visible = true;
		docked = false;

		top = bottom = left = right = nullptr;
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

			/*if (docked)
			{
				ImGui::SetNextWindowPos(WindowCostrainsMin(), ImGuiCond_Always);
				ImGui::SetWindowSize(CalculateWindowSize(), ImGuiCond_Always);
			}
			else*/
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


			focused = ImGui::IsWindowFocused();


			ImGui::End();

		}

	}

	void Window::SetTop(Window* other)
	{
		top = other;
		other->bottom = this;
	}

	void Window::SetLeft(Window* other)
	{
		left = other;
		other->right = this;
	}

	void Window::SetRight(Window* other)
	{
		right = other;
		other->left = this;
	}

	void Window::SetBottom(Window* other)
	{
		bottom = other;
		other->top = this;
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


	ImVec2 Window::CalculateWindowSize()
	{
		ImVec2 min = WindowCostrainsMin();
		ImVec2 max = WindowCostrainsMax();

		ImVec2 size;

		size.x = max.x - min.x;
		size.y = max.y - min.y;

		return size;
	}

	ImVec2 Window::WindowCostrainsMin()
	{
		ImVec2 min = ImVec2(0, 0);

		if (left != nullptr) {

			min.x = left->windowPosX + left->windowWidth;
		}

		if (top != nullptr) {

			min.y = top->windowPosY + top->windowHeight;
		}

		return min;
	}

	ImVec2 Window::WindowCostrainsMax()
	{
		ImVec2 max = Editor::getInstance()->getMainWindowSize();

		if (right != nullptr) {

			max.x = right->windowPosX;
		}

		if (bottom != nullptr) {

			max.y = bottom->windowPosY;
		}

		return max;
	}

}


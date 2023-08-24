#include "Window.h"

#include "imgui_impl_sdlrenderer2.h"
#include "imgui_impl_sdl2.h"
#include "SDL_image.h"
#include "Editor.h"
#include "imgui.h"
#include "SDL.h"

#include "imgui_internal.h"
#include "CheckML.h"

#include "ResourcesManager.h"

namespace ShyEditor {


	Window::Window(std::string _windowName, int f = 0)
	{
		flags = f;
		windowName = _windowName;

		canBeDisplayedOnTop = false;

		windowWidth = 50;
		windowHeight = 50;

		windowPosX = 0;
		windowPosY = 0;

		focused = false;
		visible = true;
		docked = false;
		acceptAssetDrop = false;
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

	void Window::ReceiveAssetDrop(Asset& asset)
	{
		std::cout << asset.name << std::endl;
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

			ImGui::Begin(windowName.c_str(), &visible, (ImGuiWindowFlags_)flags);

			auto pos = ImGui::GetWindowPos();
			auto size = ImGui::GetWindowSize();

			windowPosX = pos.x;
			windowPosY = pos.y;

			windowWidth = size.x;
			windowHeight = size.y;

			Behaviour();

			docked = ImGui::IsWindowDocked();
			focused = ImGui::IsWindowFocused();


			if (acceptAssetDrop && ImGui::IsDragDropActive() && IsMouseHoveringWindow())
			{
				int frameHeight = ImGui::GetFrameHeight();

				int margin = 8;

				ImGui::SetCursorPos(ImVec2(margin, frameHeight + margin));
				ImGui::BeginChild("##End", ImVec2(windowWidth - margin * 2, windowHeight - frameHeight - margin * 2));

				ImGui::EndChild();


				if (ImGui::BeginDragDropTarget()) {

					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
					{

						Asset* asset = (Asset*)payload->Data;

						ReceiveAssetDrop(*asset);

					}
					ImGui::EndDragDropTarget();
				}

			}






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

	bool Window::isDocked()
	{
		return docked;
	}

	void Window::Hide()
	{
		visible = false;
	}

	void Window::Show()
	{
		visible = true;
	}

	bool Window::IsVisible()
	{
		return visible;
	}

	bool Window::IsMouseHoveringWindow()
	{
		auto mouse = ImGui::GetMousePos();

		return mouse.x > windowPosX && mouse.x < windowPosX + windowWidth &&
			mouse.y > windowPosY && mouse.y < windowPosY + windowHeight;
	}

	std::string Window::GetWindowName()
	{
		return windowName;
	}


}


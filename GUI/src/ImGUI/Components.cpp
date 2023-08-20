#include "Components.h"
#include <imgui.h>
#include "Editor.h"
#include "GameObject.h"
#include "Scene.h"
#include "ComponentInfo.h"
#include "Window.h"
#include "ComponentManager.h"
#include "FileExplorer.h"


namespace ShyEditor {

	ComponentWindow::ComponentWindow() : Window("Components", NoCollapse | NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus)
	{
		Editor* editor = Editor::getInstance();
		ImVec2 mainWindowSize = editor->getMainWindowSize();

		windowOriWidth = mainWindowSize.x * COMPONENTS_WIN_WIDTH_RATIO;
		windowOriHeight = mainWindowSize.y * COMPONENTS_WIN_HEIGHT_RATIO;

		float menuBarHeight = ImGui::GetFrameHeight();

		windowOriPosX = mainWindowSize.x - windowOriWidth;
		windowOriPosY = menuBarHeight;

		setSize(ImVec2(windowOriWidth, windowOriHeight));
		setPosition(ImVec2(windowOriPosX, windowOriPosY));
	}

	void ComponentWindow::render()
	{

		Editor* editor = Editor::getInstance();
		ImVec2 mainWindowSize = editor->getMainWindowSize();

		Scene* scene = editor->getScene();
		FileExplorer* fileExplorer = editor->getFileExplorer();

		GameObject* gameObject = scene->getSelectedGameObject();


		if (focused) 
			ImGui::SetNextWindowSizeConstraints(ImVec2(mainWindowSize.x * 0.1f, mainWindowSize.y - mainWindowSize.y * 0.5f - 24), 
						 ImVec2(mainWindowSize.x * COMPONENTS_WIN_WIDTH_RATIO, mainWindowSize.y - mainWindowSize.y * 0.1f - 24));
		else
			ImGui::SetNextWindowSizeConstraints(ImVec2(mainWindowSize.x * 0.1f, mainWindowSize.y - fileExplorer->getSize().y - 24), 
						 ImVec2(mainWindowSize.x * COMPONENTS_WIN_WIDTH_RATIO, mainWindowSize.y - fileExplorer->getSize().y - 24));

		focused = false;

		ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);

		if (ImGui::IsWindowFocused())
			focused = true;

		ImVec2 imGUIWindowSize = ImGui::GetWindowSize();
		ImVec2 imGUIWindowPos = ImGui::GetWindowPos();

		if (windowPosX < mainWindowSize.x - mainWindowSize.x * COMPONENTS_WIN_WIDTH_RATIO) {
			windowPosX = mainWindowSize.x - mainWindowSize.x * COMPONENTS_WIN_WIDTH_RATIO;
			windowWidth = mainWindowSize.x * COMPONENTS_WIN_WIDTH_RATIO;
		}
		else {
			windowPosX = imGUIWindowPos.x;
			windowWidth = mainWindowSize.x - imGUIWindowPos.x;
		}

		windowPosY = imGUIWindowPos.y;
		windowHeight = imGUIWindowSize.y;

		float menuBarHeight = ImGui::GetFrameHeight();

		ImGui::SetWindowPos(ImVec2(windowPosX, menuBarHeight));
		ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

		if (gameObject != nullptr) {

			gameObject->drawTransformInEditor();
			gameObject->drawComponentsInEditor();
			gameObject->drawScriptsInEditor();

			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

			if (ImGui::CollapsingHeader("Add component"))
			{
				for (auto& comp : ::Components::ComponentManager::GetAllComponents()) {

					if (!gameObject->getComponents()->contains(comp.first) && comp.first != "Transform")
						if (ImGui::Button(comp.first.c_str(), ImVec2(windowWidth, 40))) {
							gameObject->addComponent(comp.second);
						};
				}
			}

			ImGui::PopStyleColor(6); // reset colors

			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.45f, 0.2f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.65f, 0.2f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.75f, 0.2f, 0.2f, 1.0f)); // change header active color

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.2f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.2f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.75f, 0.2f, 0.2f, 1.0f)); // change header active color

			if (ImGui::CollapsingHeader("Add scripts"))
			{

				for (auto& script : ::Components::ComponentManager::GetAllScripts()) {

					if (!gameObject->getScripts()->contains(script.GetName()))
						if (ImGui::Button(script.GetName().c_str(), ImVec2(windowWidth, 40))) {
							gameObject->addScript(script);
						};
				}
			}

			ImGui::PopStyleColor(6); // reset colors

		}

		ImGui::End();

	}
}
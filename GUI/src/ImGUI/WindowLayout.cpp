#include "WindowLayout.h"

#include "Editor.h"
#include "imgui.h"
#include "imgui_internal.h"

ShyEditor::WindowLayout::WindowLayout()
{
	updateLayout = false;
	initialization = true;
	current = Layout::Default;
}

void ShyEditor::WindowLayout::SetLayout(Layout layout)
{
	current = layout;
	updateLayout = true;
}

std::vector<std::string> ShyEditor::WindowLayout::GetAllLayouts()
{
	return { "Default", "Wide", "Scene view", "2 | 1 | 1" };
}


bool ShyEditor::WindowLayout::Update()
{
	if (!updateLayout) return false;


	auto id = ImGui::DockSpaceOverViewport();

	auto rootNode = ImGui::DockBuilderGetNode(id);
	if (updateLayout || (initialization && !rootNode->IsEmpty())) {

		ImGui::DockBuilderRemoveNode(id);

		ImGui::DockBuilderAddNode(id);
		ImGui::DockBuilderSetNodeSize(id, Editor::getInstance()->getMainWindowSize());


		switch (current)
		{
		case ShyEditor::WindowLayout::Layout::Default:

		{
			auto abajo = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.3f, nullptr, &id);
			auto izquierda = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.2f, nullptr, &id);
			auto centro = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.6f, nullptr, &id);

			ImGui::DockBuilderDockWindow("Hierarchy", izquierda);
			ImGui::DockBuilderDockWindow("File Explorer", abajo);
			ImGui::DockBuilderDockWindow("Scene", centro);
			ImGui::DockBuilderDockWindow("Components", id);

		}
			break;
		case ShyEditor::WindowLayout::Layout::Wide:

		{
			auto arriba = ImGui::DockBuilderSplitNode(id, ImGuiDir_Up, 0.8f, nullptr, &id);

			auto izquierda = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.33f, nullptr, &id);

			auto centro = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.5f, nullptr, &id);


			ImGui::DockBuilderDockWindow("Scene", arriba);
			ImGui::DockBuilderDockWindow("Hierarchy", izquierda);
			ImGui::DockBuilderDockWindow("File Explorer", id);
			ImGui::DockBuilderDockWindow("Components", centro);
		}

			break;
		case ShyEditor::WindowLayout::Layout::SceneView:
		{

			auto izquierda = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.2f, nullptr, &id);

			auto centro = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.75f, nullptr, &id);

			auto derecha = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.5f, nullptr, &id);


			ImGui::DockBuilderDockWindow("Hierarchy", izquierda);
			ImGui::DockBuilderDockWindow("Scene", centro);

			ImGui::DockBuilderDockWindow("File Explorer", derecha);
			ImGui::DockBuilderDockWindow("Components", id);
		}

			break;
		case ShyEditor::WindowLayout::Layout::_2_1_1:

		{
			auto izquierda = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.5f, nullptr, &id);
			auto centro = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.5f, nullptr, &id);


			ImGui::DockBuilderDockWindow("Components", centro);
			ImGui::DockBuilderDockWindow("Hierarchy", id);

			auto arribaIzquierda = ImGui::DockBuilderSplitNode(izquierda, ImGuiDir_Up, 0.8f, nullptr, &id);


			ImGui::DockBuilderDockWindow("Scene", arribaIzquierda);
			ImGui::DockBuilderDockWindow("File Explorer", id);


		}
			break;
		default:
			break;
		}








		ImGui::DockBuilderFinish(id);
		updateLayout = false;
		initialization = false;

	}

	return true;
}


#include "Build.h"

#include "imgui.h"

#include <iostream>

//Para convertir de PWSTR a string
#include "atlstr.h"

#include <Shlobj.h>
#include <shellapi.h>
#include <Windows.h>
#include <shlwapi.h>

#include <filesystem>

ShyEditor::Build::Build()
{
	inProgress = false;
	progression = 0;
	popUpOpen = false;
	buildPath = "";
}


bool ShyEditor::Build::SelectFolder()
{
	bool ret = false;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr)) {
		return false;
	}
	IFileDialog* pFileDialog;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
	if (SUCCEEDED(hr)) {
		// Configura el cuadro de diálogo para seleccionar carpetas.
		DWORD dwOptions;
		pFileDialog->GetOptions(&dwOptions);
		pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

		// Muestra el cuadro de diálogo de selección de carpeta.
		hr = pFileDialog->Show(NULL);
		if (SUCCEEDED(hr)) {
			// Obtiene el resultado (la carpeta seleccionada).
			IShellItem* pItem;
			hr = pFileDialog->GetResult(&pItem);
			if (SUCCEEDED(hr)) {
				PWSTR pszFolderPath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
				if (SUCCEEDED(hr)) {

					buildPath = CW2A(pszFolderPath);
					CoTaskMemFree(pszFolderPath);

					ret = true;
				}

				pItem->Release();
			}
		}
		pFileDialog->Release();
	}

	return ret;
}



void ShyEditor::Build::Copy(std::string file, std::string path, int size)
{





}




void ShyEditor::Build::BuildThread()
{




	std::cout << "Empezando hilo" << std::endl;

	while (progression < 1) {

		std::cout << "Avanzo un poco" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		progression += .2f;
	}

	std::cout << "hilo terminado" << std::endl;

	inProgress = false;
}


void ShyEditor::Build::BuildProgression()
{
	if (!popUpOpen)
		return;


	if (popUpOpen) {

		ImGui::OpenPopup("Build");


		int width = 500;
		int height = 100;
		ImGui::SetNextWindowSize(ImVec2(500, 100));

		auto windowSize = ImGui::GetIO().DisplaySize;

		ImGui::SetNextWindowPos(ImVec2((windowSize.x - width) * 0.5f, (windowSize.y - height) * 0.5f));
	}


	if (ImGui::BeginPopupModal("Build"), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize) {

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(.2f, 0.2f, 0.2f, 1));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1, .2f, 0.2f, 1));

		ImGui::ProgressBar(progression, ImVec2(-FLT_MIN, 0));
		ImGui::PopStyleColor(2);

		if (!inProgress && popUpOpen) {

			if (ImGui::Button("Close##Build")) {

				popUpOpen = false;
				ImGui::CloseCurrentPopup();
			}
		}


		ImGui::EndPopup();
	}

}

void ShyEditor::Build::GenerateBuild()
{
	if (inProgress)
		return;

	if (!SelectFolder())
		return;

	if (thread.joinable())
		thread.join();


	std::cout << buildPath << std::endl;

	popUpOpen = true;
	inProgress = true;
	progression = 0;
	thread = std::thread(&Build::BuildThread, this);
}

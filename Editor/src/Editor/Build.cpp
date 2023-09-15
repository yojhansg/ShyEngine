#include "Build.h"
#include "ResourcesManager.h"



#include "imgui.h"
#include "Preferences.h"
#include <iostream>

//Para convertir de PWSTR a string
#include "atlstr.h"

#include <Shlobj.h>
#include <shellapi.h>
#include <Windows.h>
#include <shlwapi.h>

#include <filesystem>

#include <locale>
#include <codecvt>




ShyEditor::Build::Build()
{
	inProgress = false;
	progression = 0;
	popUpOpen = false;
	buildPath = "";
}

ShyEditor::Build::~Build()
{
	if (thread.joinable())
		thread.join();
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



void ShyEditor::Build::Copy(std::string origin, std::string destination)
{
	std::filesystem::path file(origin);

	std::filesystem::path dest(destination);

	if (std::filesystem::is_directory(origin))
		dest /= file.filename();


	try {
		std::filesystem::copy(file, dest, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
	}
	catch (std::exception& e) {


		//TODO: hacer algo con el error
		std::cout << e.what() << std::endl;
	}
}


void ShyEditor::Build::BuildThread()
{

	/*
		Recursos a copiar al generar una build:

		-Exe del motor (Hay que cambiarle el nombre y el icono)
		-Dlls del exe
		-Recursos del motor
		-Recursos del proyecto

	*/

	Preferences::GenerateBuild();


	float enginefilesPercentage = 0.25f;
	float projectFilesPercentage = 0.50f;
	float generateExePercentage = 0.10f;

	std::vector<std::string> engineFiles = {
		"EngineRelease.exe", "config.json",
		"SDL2.dll", "SDL2_image.dll" ,
		"SDL2_mixer.dll",  "SDL2_ttf.dll" ,
		"Assets"
	};

	
	for (auto& value : engineFiles) {

		Copy(ResourcesManager::EDITORENGINEFOLDER + "/" + value, buildPath);

		progression += enginefilesPercentage / engineFiles.size();
	}

	std::string exe = buildPath + "/" + Preferences::GetData().name + ".exe";

	Copy(ResourcesManager::GetProjectPath() + "/Assets", buildPath);

	progression += projectFilesPercentage;

	std::string relaseBuildPath = buildPath + "/EngineRelease.exe";

	std::string command = std::format("ResourceHacker.exe -open {} -save {} -action addoverwrite -res {} -mask ICONGROUP, 101, 0", 
		relaseBuildPath,
		exe,
		std::string(buildPath + "/Assets/" + Preferences::GetData().icon)
		);

	system((command).c_str());


	progression += generateExePercentage;


	if (!std::filesystem::exists(exe))
		std::filesystem::rename(relaseBuildPath, exe);
	else
		std::filesystem::remove(relaseBuildPath);


	progression = 1;

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


	popUpOpen = true;
	inProgress = true;
	progression = 0;
	thread = std::thread(&Build::BuildThread, this);
}

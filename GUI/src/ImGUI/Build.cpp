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

void ShyEditor::Build::ChangeExeIcon(const std::string& exe)
{

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::wstring w_exe = converter.from_bytes(exe);
	const wchar_t* exeFilePath = L"C:\\Users\\sryoj\\Desktop\\Build\\FlappyBird.exe";
	const wchar_t* exeFilePath2 = L"C:\\Users\\sryoj\\Desktop\\Build\\FlappyBird2.exe";


	const wchar_t* iconFilePath = L"C:\\Users\\sryoj\\Desktop\\Build\\Assets\\icon.ico";

#define ICON_ID 101

	// Cargamos la biblioteca de recursos del archivo ejecutable
	

	// Encuentra el recurso ICON en la biblioteca de recursos



	HICON hIcon = (HICON)LoadImage(NULL, L"C:\\Users\\sryoj\\Desktop\\Build\\Assets\\icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	if (hIcon == NULL) {
		// Manejar errores, si es necesario
		std::cout << "no se lee el fichero" << std::endl;
		return;
	}

	// Abre el archivo ejecutable para actualizar los recursos
	HANDLE hUpdate = BeginUpdateResource(exeFilePath2, FALSE);
	if (hUpdate == NULL) {
		std::cout << "Error al abrir el archivo ejecutable" << std::endl;
		return;
	}

	auto lockResource = LockResource(hIcon);

	if (lockResource == NULL) {

		std::cout << "No se puede hacer lock" << std::endl;
		return;
	}

	auto resourceSize = SizeofResource(NULL, (HRSRC)hIcon);



	// Actualiza el recurso ICON en el archivo ejecutable
	if (!UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(ICON_ID), 0, lockResource, resourceSize)) {
		std::cout << "Error al actualizar el recurso ICON" << std::endl;
		EndUpdateResource(hUpdate, TRUE);
		return;
	}

	// Cierra el archivo ejecutable
	if (!EndUpdateResource(hUpdate, FALSE)) {
		std::cout << "Error al guardar el archivo ejecutable" << std::endl;
		return;
	}

	DestroyIcon(hIcon);

	std::cout << "Icono actualizado con éxito" << std::endl;



	return;

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



	struct CopyValues {
		std::string path;
		float percentage;
	};

	std::vector<CopyValues> engineFiles = { {"EngineRelease.exe", 0.33f}, {"config.json", 0.33f},  {"SDL2.dll", 0.33f}, {"SDL2_image.dll", 0.33f}, {"SDL2_mixer.dll", 0.33f}, {"SDL2_ttf.dll", 0.33f}, {"Assets", 0.33f} };


	for (auto& value : engineFiles) {

		Copy(ResourcesManager::EDITORENGINEFOLDER + "/" + value.path, buildPath);
	}


	Copy(ResourcesManager::GetProjectPath() + "/Assets", buildPath);


	std::string exe = buildPath + "/" + Preferences::GetData().name + ".exe";


	std::filesystem::rename(buildPath + "/EngineRelease.exe", exe);


	//ChangeExeIcon("");
	



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

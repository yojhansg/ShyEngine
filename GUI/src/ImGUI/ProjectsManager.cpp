#include "ProjectsManager.h"

#include "imgui_impl_sdlrenderer2.h"
#include "imgui_impl_sdl2.h"
#include "LogManager.h"
#include "Editor.h"
#include "Window.h"
#include "imgui.h"
#include "SDL.h"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <shlwapi.h>
#include <Windows.h>
#include <Shlobj.h>
#include <iostream>
#include <fstream>
#include <ctime>

#include "CheckML.h"

#define ProjectExtension ".shyproject"

using namespace nlohmann;

namespace ShyEditor {

    ProjectsManager* ProjectsManager::instance = nullptr;

    char ProjectsManager::project_name[128] = "";
    char ProjectsManager::create_project_path[256] = "";
    char ProjectsManager::open_project_path[256] = "";

    std::wstring ProjectsManager::projectfileExtension = L"" ProjectExtension;
    std::wstring ProjectsManager::projectsfileFolder = L"\\ShyEngine\\RecentProjects";
    std::wstring ProjectsManager::projectsfileName = L"\\recentprojects.json";

    const std::string ProjectsManager::ImGuiINIPath = "\\UserSettings\\imgui.ini";

    const std::vector<std::string> ProjectsManager::assetsFolders = { "\\Scripts" };

    ProjectsManager::ProjectsManager() {

        editor = Editor::GetInstance();

        windowClosed = invalidProjectName = false;
        invalidNewProjectPath = invalidOpenProjectPath = false;
        showPopUpWindowNewProject = showPopUpWindowOpenProject = false;

        creationDate = name = projectFilePath = openPath = lastSavedProjectPath = "";
        lastOpenedScenePath = "Scene.scene";

        errorMessage = L"Default error message. No one knows what went wrong :(";

        w = h = 0.0f;
    }

    ProjectsManager* ProjectsManager::GetInstance() {
        if (instance == nullptr)
            instance = new ProjectsManager();

        return instance;
    }

    void ProjectsManager::Release() {
        delete instance;
        instance = nullptr;
    }

    void ProjectsManager::StartImGuiFrame() {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void ProjectsManager::ImGuiRender(SDL_Renderer* renderer) {
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }


    ProjectsManager::Result ProjectsManager::ManageProjectSelection(SDL_Renderer* renderer) {

        Result result = Result::CLOSED;

        while (!windowClosed) {

            HandleInput();

            StartImGuiFrame();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 3.0f, ImGui::GetIO().DisplaySize.y * 4.0f / 5.0f));

            // New Project Window
            result = NewProject();

            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 3, 0));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 1.5f, ImGui::GetIO().DisplaySize.y * 4.0f / 5.0f));

            // Open Project Window
            if (!windowClosed)
                result = OpenProject();

            ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().DisplaySize.y / 5.0f));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y / 5.0f));

            // Close Editor
            if (!windowClosed)
                result = CloseEditor();

            ImGuiRender(renderer);

        }

        return result;
    }


    void ProjectsManager::HandleInput() {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                windowClosed = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                windowClosed = true;
        }

    }

    ProjectsManager::Result ProjectsManager::NewProject() {

        ImGui::Begin("Create Project", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImVec2 size = ImGui::GetWindowSize();
        w = size.x; h = size.y;

        ImGui::SetCursorPosY(h / 12.0f);

        ImGui::Text("Project Name:");
        ImGui::SetNextItemWidth(w / 1.2f);
        ImGui::InputText("##project_name", project_name, IM_ARRAYSIZE(project_name));

        // Shows an error message
        if (invalidProjectName)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project name. Can not be empty!");
        else
            name = project_name;


        ImGui::SetCursorPosY(h / 4.0f);

        ImGui::Text("Project Path:");
        ImGui::SetNextItemWidth(w / 1.2f);
        ImGui::InputText("##project_path", create_project_path, IM_ARRAYSIZE(create_project_path));

        // Select folder button logic
        if (ImGui::Button("Select Folder", ImVec2(w / 2.0f, h / 20.0f))) {

            BROWSEINFOA bi;
            ZeroMemory(&bi, sizeof(bi));
            bi.hwndOwner = GetForegroundWindow();
            bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

            LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);

            if (pidl) {
                char selectedPath[MAX_PATH];
                if (SHGetPathFromIDListA(pidl, selectedPath)) {
                    strcpy_s(create_project_path, sizeof(create_project_path), selectedPath);
                }
                CoTaskMemFree(pidl);
            }

        }

        // Shows an error message
        if (invalidNewProjectPath)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project path. Can not be empty and \nmust be an existing folder!");
        else
            projectFilePath = std::string(create_project_path) + "\\" + std::string(project_name) + "\\" + std::string(project_name) + ProjectExtension;


        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 2.0f));

        // Create project button logic
        if (ImGui::Button("Create Project", ImVec2(w / 2.0f, h / 15.0f)))
        {
            // Input validation
            bool valid = ValidateInputOnNewProject();

            if (valid) {

                if (!SaveProject())
                    showPopUpWindowNewProject = true;
                else {
                    windowClosed = true;

                    ImGui::End();

                    return Result::ENTERED;
                }
            }

        }

        if (showPopUpWindowNewProject)
            ShowErrorPopup(errorMessage);

        ImGui::End();
        return Result::CLOSED;
    }

    ProjectsManager::Result ProjectsManager::OpenProject() {

        ImGui::Begin("Open Project", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImVec2 size = ImGui::GetWindowSize();
        w = size.x; h = size.y;

        ImGui::SetCursorPosY(h / 12.0f);

        ImGui::Text("Project Path:");
        ImGui::SetNextItemWidth(w / 1.2f);
        ImGui::InputText("##project_path", open_project_path, IM_ARRAYSIZE(open_project_path));

        // Select file logic button
        if (ImGui::Button("Select File", ImVec2(w / 4.0f, h / 20.0f))) {


            OPENFILENAMEA ofn;  // Estructura de di�logo de archivo
            char fileName[MAX_PATH] = "";
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = GetForegroundWindow();
            ofn.lpstrFilter = "Todos los archivos\0*.*\0";
            ofn.lpstrFile = fileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrTitle = "Seleccionar Archivo";
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

            if (GetOpenFileNameA(&ofn))
                strncpy_s(open_project_path, sizeof(open_project_path), fileName, _TRUNCATE);

        }

        // Shows an error message
        if (invalidOpenProjectPath)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project path. Cannot be empty and must be an existing " ProjectExtension " file!");
        else
            openPath = open_project_path;


        // Open project logic button
        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 3.3f));
        if (ImGui::Button("Open Project", ImVec2(w / 2.0f, h / 15.0f))) {

            // Input validation
            bool valid = ValidateInputOnOpenProject();

            if (valid) {

                if (!ReadInfoFromProjectFile(openPath) || !StoreProjectPath(openPath))
                    showPopUpWindowOpenProject = true;
                else {

                    projectFilePath = openPath;

                    std::string folderPath = std::filesystem::path(openPath).parent_path().string();
                    editor->SetProjectInfo(new ProjectInfo(name, creationDate, folderPath));
                    editor->SetLastOpenedScene(lastOpenedScenePath);

                    windowClosed = true;
                    ImGui::End();
                    std::cout << std::filesystem::current_path() << std::endl;

                    return Result::ENTERED;
                }
            }

        }

        ImGui::SetCursorPosY(h / 2.3f);
        ImGui::Text("Recent Projects:");

        // Listbox for the recent projects
        bool recentProjectOpened = false;
        std::string recentProjectOpenedPath;
        if (ImGui::BeginListBox("##", ImVec2(w - w / 10, h / 2))) {

            for (int i = 0; i < recentProjectsInfo.size() && !recentProjectOpened; i++) {

                std::string t = "Project: " + recentProjectsInfo[i].name + "\t\tDate: " + recentProjectsInfo[i].date;
                if (ImGui::Button(t.c_str(), ImVec2(w, h / 8))) {
                    recentProjectOpened = true;
                    recentProjectOpenedPath = recentProjectsInfo[i].path;
                }
            }

            ImGui::EndListBox();
        }


        // On opened one of the recent projects
        if (recentProjectOpened) {

            if (!ReadInfoFromProjectFile(recentProjectOpenedPath))
                showPopUpWindowOpenProject = true;
            else {

                projectFilePath = recentProjectOpenedPath;

                std::string folderPath = std::filesystem::path(openPath).parent_path().string();
                editor->SetProjectInfo(new ProjectInfo(name, creationDate, folderPath));
                editor->SetLastOpenedScene(lastOpenedScenePath);

                windowClosed = true;
                ImGui::End();
                return Result::ENTERED;
            }
        }

        if (showPopUpWindowOpenProject)
            ShowErrorPopup(errorMessage);


        ImGui::End();

        return Result::CLOSED;
    }

    ProjectsManager::Result ProjectsManager::CloseEditor() {

        ImGui::Begin("CLOSE", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

        ImVec2 size = ImGui::GetWindowSize();
        w = size.x; h = size.y;

        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 8.0f, h / 2.0f));

        if (ImGui::Button("Close", ImVec2(w / 4.0f, h / 5.0f)))
            windowClosed = true;

        ImGui::End();
        return Result::CLOSED;
    }

    bool ProjectsManager::MakeFolderToStoreRecentProjects() {

        PWSTR appDataPath;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &appDataPath))) {
            std::wstring appDataDir(appDataPath);
            CoTaskMemFree(appDataPath);

            std::wstring projectsDir = appDataDir + projectsfileFolder;

            recentProjectsFile = projectsDir + projectsfileName;

            // Check if folder already exists
            std::error_code ec;
            if (!std::filesystem::exists(projectsDir, ec)) {

                // Error handling for checking if the directory exists or not.
                if (ec) {
                    LogManager::LogError("PROJECTS MANAGER: Error when checking if the directory to be created already exists in your AppData path.");
                    return false;
                }

                // The folder is created
                std::filesystem::create_directories(projectsDir, ec);

                // Error handling on folders creation
                if (ec) {
                    LogManager::LogError("PROJECTS MANAGER: Error while creating the directory to host the necessary files for the proper functioning of the editor.");
                    return false;
                }

                // The JSON is created with an empty array of paths to be dumped into the recent projects file.
                nlohmann::json j;
                j["Recent Projects Paths"] = json::array();
                j["Last Project Saved Path"] = "";

                std::ofstream jsonFile(recentProjectsFile);

                if (!jsonFile.is_open()) {
                    LogManager::LogError("PROJECTS MANAGER: The recent projects file could not be opened to dump the content.");
                    return false;
                }

                jsonFile << j.dump(2);
                jsonFile.close();

                // Both the directory and the file are currently created correctly.

            }
            // If folder already exists, reads the content of the recent projects file
            // to check for any missing ones.
            else {

                // Read recent projects file
                std::ifstream fileStreamIn(recentProjectsFile);

                if (!fileStreamIn.is_open()) {
                    LogManager::LogError("PROJECTS MANAGER: The recent projects file could not be opened to read its content.");
                    return false;
                }

                // Gets the json from the recent projects file
                json jsonData = json::parse(fileStreamIn);

                fileStreamIn.close();

                // Check if the 'recentProjectsFile' contains the expected format
                if (!jsonData.contains("Recent Projects Paths") || !jsonData.contains("Last Project Saved Path") || !jsonData["Recent Projects Paths"].is_array()) {
                    LogManager::LogError("PROJECTS MANAGER: The recent projects file does not contain the expected format.");
                    return false;
                }

                // Save the information of the last saved project path
                lastSavedProjectPath = jsonData["Last Project Saved Path"];

                // Save the information of name, path, and date for all recent projects.
                int i = 0;
                std::vector<int> deletedProjects;
                for (const auto& project : jsonData["Recent Projects Paths"]) {

                    std::string projectPath(project);

                    std::ifstream fileStreamIn;
                    fileStreamIn.open(projectPath);

                    if (fileStreamIn.is_open()) {

                        json json = json::parse(fileStreamIn);

                        // Adds the project data to the recent projects info array
                        recentProjectsInfo.push_back(ProjectInfo(json["Project Name"], json["Creation Date"], json["Project Path"]));

                        fileStreamIn.close();

                    }
                    else deletedProjects.push_back(i); // Mark the proyect as deleted

                    i++;

                }

                // Remove the path from the recent projects file for those projects that couldn't be opened (marked as deleted).
                for (i = 0; i < deletedProjects.size(); i++) {
                    jsonData["Recent Projects Paths"].erase(deletedProjects[i]);
                    for (int c = i + 1; c < deletedProjects.size(); c++)
                        deletedProjects[c]--;
                }

                // After updating de JSON, dumps its content to the 'recentProjectsFile'
                std::ofstream fileStreamOut(recentProjectsFile, std::ofstream::out | std::ofstream::trunc);

                if (!fileStreamOut.is_open()) {
                    LogManager::LogError("PROJECTS MANAGER: The recent projects file cannot be opened for writing.");
                    return false;
                }

                fileStreamOut << jsonData.dump(2);
                fileStreamOut.close();

                // The 'recentProjectsFile' is now updated correctly.

                strncpy_s(create_project_path, sizeof(create_project_path), lastSavedProjectPath.c_str(), _TRUNCATE);

            }

        }
        else {
            LogManager::LogError("PROJECTS MANAGER: Could not create the necessary directories in your AppData path! Restart the editor and try again.");
            return false;
        }

        return true;

    }

    bool ProjectsManager::SaveProject() {

        std::string folder = std::string(create_project_path);
        std::string path = projectFilePath; // (Ends with .shyproject)

        // If the create path is an empty folder with the same name as the project
        // the folder project wont be created, it uses the create path.
        if (GetLastPathComponent(create_project_path, '\\') == name) {

            if (!IsDirectoryEmpty(create_project_path)) {
                errorMessage = L"The selected directory is not empty.";
                return false;
            }
            else path = folder + "\\" + std::string(project_name) + ProjectExtension;
        }
        else {

            if (HasNestedDirectoryWithName(create_project_path, name)) {
                errorMessage = L"There is already a project with the same name in the selected directory.";
                return false;
            }

            // The folder which contains the project file is created
            folder += "\\" + std::string(project_name);

            std::error_code ec;
            std::filesystem::create_directories(folder, ec);

            if (ec) {
                errorMessage = L"Error while creating the project directory.";
                return false;
            }
        }

        projectFilePath = path;

        if (!StoreProjectPath(projectFilePath))
            return false;

        if (!CreateAssetsFolders(folder))
            return false;

        // Store the values in the .shyproject file
        nlohmann::json j;

        creationDate = getCurrentDate();

        j["Project Name"] = name;
        j["Project Path"] = projectFilePath;
        j["Creation Date"] = creationDate;
        j["Last Opened Scene"] = lastOpenedScenePath;

        // Create the project file in the specified folder
        std::ofstream outputFile(projectFilePath);

        if (!outputFile.is_open()) {
            errorMessage = L"Error while creating the project file in the selected directory.";
            return false;
        }

        outputFile << j.dump(4);
        outputFile.close();

        editor->SetProjectInfo(new ProjectInfo(name, creationDate, folder));
        editor->SetLastOpenedScene(lastOpenedScenePath);

        return true;

    }

    bool ProjectsManager::StoreProjectPath(const std::string& path) {

        // Add the project path to the recent projects
        std::ifstream fileStreamIn(recentProjectsFile);

        if (!fileStreamIn.is_open()) {
            errorMessage = L"Error while opening the recent projects file for reading.";
            return false;
        }

        // Gets the json from the recent projects file
        json json = json::parse(fileStreamIn);

        fileStreamIn.close();

        // Check if the array already contains the path
        bool containsPath = false;
        for (const auto& p : json["Recent Projects Paths"]) {
            if (p == path) {
                containsPath = true;
                break;
            }
        }

        // Adds the path of the new project to the json
        if (!containsPath) {

            json["Last Project Saved Path"] = std::string(create_project_path);
            json["Recent Projects Paths"].push_back(path);

            // Write the json in the recent projects file
            std::ofstream fileStreamOut(recentProjectsFile, std::ofstream::out | std::ofstream::trunc);

            if (!fileStreamOut.is_open()) {
                errorMessage = L"Error while opening the recent projects file for writing.";
                return false;
            }

            fileStreamOut << json.dump(2);
            fileStreamOut.close();

        }

        return true;
    }

    bool ProjectsManager::ValidateInputOnNewProject() {

        bool name, path;
        name = path = true;

        if (strlen(project_name) <= 0) {
            invalidProjectName = true;
            name = false;
        }
        else invalidProjectName = false;

        DWORD fileAttributes = GetFileAttributesA(create_project_path);
        if (strlen(create_project_path) <= 0 || fileAttributes == INVALID_FILE_ATTRIBUTES) {
            invalidNewProjectPath = true;
            path = false;
        }
        else invalidNewProjectPath = false;

        return name && path;
    }

    bool ProjectsManager::ValidateInputOnOpenProject() {

        DWORD fileAttributes = GetFileAttributesA(open_project_path);
        if (strlen(open_project_path) <= 0 || fileAttributes == INVALID_FILE_ATTRIBUTES) {

            invalidOpenProjectPath = true;
            return false;
        }
        else {

            std::wstring filePathWide(open_project_path, open_project_path + strlen(open_project_path));

            if (!HasExtension(filePathWide, projectfileExtension)) {
                invalidOpenProjectPath = true;
                return false;
            }
            else invalidOpenProjectPath = false;

        }

        return true;
    }

    bool ProjectsManager::HasExtension(const std::wstring& filePath, const std::wstring& extension) {

        return filePath.size() >= extension.size() &&
            filePath.compare(filePath.size() - extension.size(), extension.size(), extension) == 0;
    }

    std::string ProjectsManager::GetLastPathComponent(const std::string& path, char delimiter) {

        std::istringstream iss(path);
        std::string token;

        std::vector<std::string> tokens;
        while (std::getline(iss, token, delimiter)) {
            tokens.push_back(token);
        }

        if (!tokens.empty()) {
            return tokens.back();
        }

        return "";

    }

    void ProjectsManager::StoreLastOpenedScene(const std::string& scenePath) {

        std::ifstream fileStreamIn(instance->projectFilePath);

        if (!fileStreamIn.is_open()) {
            LogManager::LogError("PROJECTS MANAGER: Could not store the last opened scene path");
            return;
        }

        json json = json::parse(fileStreamIn);
        fileStreamIn.close();

        json["Last Opened Scene"] = scenePath;

        std::ofstream fileStreamOut(instance->projectFilePath);

        if (!fileStreamOut.is_open()) {
            LogManager::LogError("PROJECTS MANAGER: Could not store the last opened scene path");
            return;
        }

        fileStreamOut << json.dump(4);
        fileStreamOut.close();
    }

    std::string ProjectsManager::GetProjectFilePath() {
        return instance->projectFilePath;
    }

    std::string ProjectsManager::GetProjectFileName()
    {
        std::filesystem::path p(instance->projectFilePath);

        return p.filename().string();
    }

    bool ProjectsManager::CreateAssetsFolders(const std::string& root) {

        std::error_code ec;
        std::filesystem::create_directories(root + "\\Assets", ec);

        if (ec) {
            errorMessage = L"Error while creating the project assets folder.";
            return false;
        }

        std::filesystem::create_directories(root + "\\UserSettings", ec);
        if (ec) {
            errorMessage = L"Error while creating the project user settings folder.";
            return false;
        }

        for (int i = 0; i < assetsFolders.size(); i++) {

            std::filesystem::create_directories(root + "\\Assets" + assetsFolders[i], ec);

            std::wstring folderName(assetsFolders[i].begin(), assetsFolders[i].end());

            if (ec) {
                errorMessage = L"Error while creating the project assets folder with name: " + folderName;
                return false;
            }
        }

        // Create default scene file
        nlohmann::ordered_json j;

        j["name"] = lastOpenedScenePath;
        j["objects"] = nlohmann::json::array();
        j["overlays"] = nlohmann::json::array();

        std::ofstream outputFile(root + "\\Assets\\" + lastOpenedScenePath);
        if (outputFile.is_open()) {
            outputFile << j.dump(4);
            outputFile.close();
        }
        else LogManager::LogError("Could not create the default scene file.");

        return true;
    }

    bool ProjectsManager::IsDirectoryEmpty(const std::string& directory) {

        WIN32_FIND_DATAA findData;
        HANDLE findHandle = FindFirstFileA((directory + "\\*").c_str(), &findData);

        if (findHandle == INVALID_HANDLE_VALUE) {
            errorMessage = L"The directory doesn't exist or there was a problem opening it.";
            return false;
        }

        bool isEmpty = true;
        do {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                isEmpty = false; // At least one file or directory exists
                break;
            }
        } while (FindNextFileA(findHandle, &findData) != 0);

        FindClose(findHandle);
        return isEmpty;
    }

    bool ProjectsManager::ReadInfoFromProjectFile(const std::string& filepath) {

        std::ifstream fileStreamIn(filepath);

        if (!fileStreamIn.is_open()) {
            errorMessage = L"Error opening the project file for reading.";
            return false;
        }

        json json = json::parse(fileStreamIn);
        fileStreamIn.close();

        name = json["Project Name"];
        openPath = json["Project Path"];
        creationDate = json["Creation Date"];
        lastOpenedScenePath = json["Last Opened Scene"];

        return true;

    }

    bool ProjectsManager::HasNestedDirectoryWithName(const std::string& parentDir, const std::string& dirName) {

        std::filesystem::path parentPath(parentDir);
        for (const auto& entry : std::filesystem::directory_iterator(parentPath)) {
            if (entry.is_directory() && entry.path().filename() == dirName) {
                return true;
            }
        }
        return false;

    }

    void ProjectsManager::ShowErrorPopup(const std::wstring& errorMessage) {

        ImGui::OpenPopup("Error");

        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 1.2f, ImGui::GetIO().DisplaySize.y / 6.0f));

        if (ImGui::BeginPopupModal("Error")) {
            ImGui::Text("%ls", errorMessage.c_str());

            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2.0f - ImGui::GetContentRegionAvail().x / 16.0f);
            ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 0.8f);
            if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x / 4.0f, ImGui::GetContentRegionAvail().y / 1.5f))) {
                ImGui::CloseCurrentPopup();
                showPopUpWindowNewProject = showPopUpWindowOpenProject = false;
            }
            ImGui::EndPopup();
        }

    }

    std::string ProjectsManager::getCurrentDate() {

        std::time_t currentTime = std::time(nullptr);

        std::tm localTime = {};

        localtime_s(&localTime, &currentTime);

        char formattedDate[50];
        strftime(formattedDate, sizeof(formattedDate), "%Y-%m-%d %H:%M:%S", &localTime);

        std::string formattedDateString(formattedDate);

        return formattedDateString;

    }

}

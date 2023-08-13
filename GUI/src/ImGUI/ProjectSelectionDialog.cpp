#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "Window.h"
#include "SDL.h"
#include <iostream>
#include <fstream>
#include "ScriptCreation.h"
#include "ComponentManager.h"
#include "ProjectSelectionDialog.h"


#include <Windows.h>
#include <string>

void PEditor::ProjectSelectionDialog::HandleInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            windowClosed = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
            windowClosed = true;
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            windowClosed = true;
    }
}

PEditor::ProjectSelectionDialog::ProjectSelectionDialog()
{
    windowClosed = false;
}

PEditor::ProjectSelectionDialog:: Result PEditor::ProjectSelectionDialog::ManageProjectSelectionDialog(SDL_Renderer* renderer)
{
    while (!windowClosed) {

        HandleInput();

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("Ventana principal", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

        if (ImGui::Button("Seleccionar Archivo")) {
            OPENFILENAMEA ofn;       // Estructura de diálogo de archivo
            char fileName[MAX_PATH] = "";
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = GetForegroundWindow();
            ofn.lpstrFilter = "Todos los archivos\0*.*\0";
            ofn.lpstrFile = fileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrTitle = "Seleccionar Archivo";
            ofn.Flags = OFN_FILEMUSTEXIST;

            if (GetOpenFileNameA(&ofn)) {

            }
        }


        ImGui::End();

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    return Result::Closed;
}

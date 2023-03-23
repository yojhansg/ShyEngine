#pragma once
#include <iostream>
#include <unordered_map>

struct ImVec2;
struct SDL_Texture;
struct SDL_Renderer;
class ImGUIManager;
class Transform;
class Component;

namespace PEditor {
	class GameObject
	{

		SDL_Texture* text;
		Transform* tr;
		std::unordered_map<int ,Component*> components;

	public:

		GameObject(std::string& path);

		virtual void render();

		SDL_Texture* getTexture();

		int getWidth();
		int getHeight();

		std::unordered_map<int, Component*>* getComponents();

		void setPosition(ImVec2 newPos);
		ImVec2 getPosition();
	};
}
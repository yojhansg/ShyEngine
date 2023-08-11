#pragma once
#include <iostream>
#include <list>
#include <unordered_map>

union SDL_Event;

struct ImVec2;
struct SDL_Texture;
struct SDL_Renderer;
class ImGUIManager;
class Transform;
class Camera;

namespace Components {
	class Component;
	class Attribute;
}

namespace PEditor {
	class GameObject
	{
		static int lastId;

		std::string name;
		int id;
		SDL_Texture* text;
		SDL_Texture* gizmoText;
		std::unordered_map<std::string, ::Components::Component&> components;

		ImGUIManager* imGuiManager;

		bool visible;

		bool leftMouseButtonDown;
		bool rightMouseButtonDown;

		bool showGizmo;

		bool waitingToDelete;

		ImVec2* size;
		ImVec2* pos;

		float previousMousePosX;
		float previousMousePosY;
		float rotation;

		int renderOrder;

		void drawFloat(std::string attrName, ::Components::Attribute* attr);
		void drawVector2(std::string attrName, ::Components::Attribute* attr);
		void drawString(std::string attrName, ::Components::Attribute* attr);
		void drawBool(std::string attrName, ::Components::Attribute* attr);
		void drawColor(std::string attrName, ::Components::Attribute* attr);

	public:

		GameObject(std::string& path);
		~GameObject();

		virtual void render();

		SDL_Texture* getTexture();
		std::string getName();
		int getId();

		bool isVisible();
		void setVisible(bool visible);

		int getWidth();
		int getHeight();

		void drawTransformInEditor();

		void render(SDL_Renderer* renderer, Camera* camera);
		void handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos);

		void addComponent(::Components::Component& comp);
		std::unordered_map<std::string, ::Components::Component&>* getComponents();

		void setPosition(ImVec2 newPos);
		void setName(const std::string newName);
		ImVec2 getPosition();
		float getRotation();

		bool isWaitingToDelete();
		void toDelete();
	
		void drawComponentsInEditor();
		std::string toJson();
	};
}
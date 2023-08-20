#pragma once
#include <iostream>
#include <list>
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;
struct ImVec2;

union SDL_Event;
class Transform;
class Editor;

namespace Components {
	class Component;
	class Attribute;
	class Script;
}

namespace ShyEditor {

	class Camera;

	class GameObject {

	private:

		static int lastId;

		std::string name;
		int id;

		std::string imagePath;
		Components::Component* imageComponent;
		SDL_Texture* text;
		SDL_Texture* gizmoText;
		std::unordered_map<std::string, ::Components::Component> components;

		std::unordered_map<std::string, Components::Script> scripts;

		Editor* editor;

		bool visible;

		bool leftMouseButtonDown;
		bool rightMouseButtonDown;

		bool showGizmo;

		bool waitingToDelete;

		ImVec2* scale;
		ImVec2* pos;
		ImVec2* size;

		float previousMousePosX;
		float previousMousePosY;
		float rotation;

		int renderOrder;

		void drawFloat(std::string attrName, ::Components::Attribute* attr);
		void drawVector2(std::string attrName, ::Components::Attribute* attr);
		void drawString(std::string attrName, ::Components::Attribute* attr);
		void drawBool(std::string attrName, ::Components::Attribute* attr);
		void drawColor(std::string attrName, ::Components::Attribute* attr);
		void drawChar(std::string attrName, ::Components::Attribute* attr);
		void drawGameobject(std::string attrName, ::Components::Attribute* attr);

		void translateChildren(GameObject* go, ImVec2* previousPos);
		void scaleChildren(GameObject* go, int scaleFactor);
		void setChildrenVisible(GameObject* go, bool visible);
		void rotateChildren(GameObject* go, GameObject* goCenter, float rotationAngle);

		GameObject* parent;
		std::unordered_map<int, GameObject*> children;

	public:

		GameObject(std::string& path);
		~GameObject();

		SDL_Texture* getTexture();
		std::string getName();
		int getId();

		int getRenderOrder();

		bool isVisible();
		void setVisible(bool visible);

		float getScale_x();
		float getScale_y();

		void drawTransformInEditor();

		void render(SDL_Renderer* renderer, Camera* camera);
		bool handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos);
		void update();

		void addComponent(::Components::Component comp);
		void addScript(::Components::Script script);

		std::unordered_map<std::string, ::Components::Component>* getComponents();
		std::unordered_map<std::string, ::Components::Script>* getScripts();

		void setPosition(ImVec2 newPos);
		void setName(const std::string newName);
		ImVec2 getPosition();
		float getRotation();

		ImVec2 getAdjustedPosition();
		ImVec2 getSize();

		bool isWaitingToDelete();
		void toDelete();
	
		void setParent(GameObject* go);
		GameObject* getParent();
		void removeChild(GameObject* go);
		void addChild(GameObject* go);
		std::unordered_map<int, GameObject*> getChildren();
		bool isAscendant(GameObject* go);

		void drawComponentsInEditor();
		void drawScriptsInEditor();
		std::string toJson(bool isPrefab = false);

		static GameObject* fromJson(std::string json, bool isPrefab = false);

	};
}
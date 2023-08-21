#pragma once
#include <iostream>
#include <list>
#include <unordered_map>

struct SDL_Renderer;
union SDL_Event;
struct ImVec2;
class Editor;

namespace Components {
	class Component;
	class Attribute;
	class Script;
}

namespace ShyEditor {

	class Camera;
	class Texture;

	class GameObject {

	public:

		GameObject(std::string& path);
		~GameObject();

		// Render, update and input
		void render(SDL_Renderer* renderer, Camera* camera);
		void update();
		bool handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos);

		// Name, ID and texture getters/setters
		std::string getName();
		void setName(const std::string& newName);
		Texture* getTexture();
		int getId();

		// Visibility getters/setters
		int getRenderOrder();
		bool isVisible();
		void setVisible(bool visible);

		// Components and Scripts logic
		void addComponent(Components::Component comp);
		void addScript(Components::Script script);
		std::unordered_map<std::string, Components::Component>* getComponents();
		std::unordered_map<std::string, Components::Script>* getScripts();

		// Tranform attributes getters/setters
		void setPosition(ImVec2 newPos);
		ImVec2 getPosition();
		float getRotation();
		ImVec2 getAdjustedPosition();
		ImVec2 getSize();
		float getScale_x();
		float getScale_y();

		// Deleting gameobject logic
		bool isWaitingToDelete();
		void toDelete();

		// Gameobject children and parent logic
		void setParent(GameObject* go);
		GameObject* getParent();
		void removeChild(GameObject* go);
		void addChild(GameObject* go);
		std::unordered_map<int, GameObject*> getChildren();
		bool isAscendant(GameObject* go);

		// Gameobject transform, components and scripts drawing
		void drawTransformInEditor();
		void drawComponentsInEditor();
		void drawScriptsInEditor();

		// Serialization and deseralization logic
		std::string toJson(bool isPrefab = false);
		static GameObject* fromJson(std::string json, bool isPrefab = false);

	private:

		// Static variable to store the last assigned id (Needed for ids assingment)
		static int lastId;

		// Gameobject name
		std::string name;

		// Gameobject id
		int id;

		// Gameobject components, scripts, children and parent
		std::unordered_map<std::string, ::Components::Component> components;
		std::unordered_map<std::string, Components::Script> scripts;
		std::unordered_map<int, GameObject*> children;
		GameObject* parent;

		// A reference to the editor singleton
		Editor* editor;

		// Gameobject visibility
		bool visible;
		bool showGizmo;
		int renderOrder;

		// Transform attributes
		ImVec2* scale;
		ImVec2* pos;
		ImVec2* size;
		float rotation;

		// Gameobject texture and gizmo, path to the image and image component
		Components::Component* imageComponent;
		std::string imagePath;
		Texture* texture;
		Texture* gizmo;

		// Some other needed information (Input and deleting gameobject logic)
		bool leftMouseButtonDown;
		bool rightMouseButtonDown;
		float previousMousePosX;
		float previousMousePosY;
		bool waitingToDelete;

		// Gameobject draw methods
		void drawFloat(std::string attrName, Components::Attribute* attr);
		void drawVector2(std::string attrName, Components::Attribute* attr);
		void drawString(std::string attrName, Components::Attribute* attr);
		void drawBool(std::string attrName, Components::Attribute* attr);
		void drawColor(std::string attrName, Components::Attribute* attr);
		void drawChar(std::string attrName, Components::Attribute* attr);
		void drawGameobject(std::string attrName, Components::Attribute* attr);

		// Gameobject children settings (Transform and visibility)
		void translateChildren(GameObject* go, ImVec2* previousPos);
		void scaleChildren(GameObject* go, int scaleFactor);
		void setChildrenVisible(GameObject* go, bool visible);
		void rotateChildren(GameObject* go, GameObject* goCenter, float rotationAngle);

	};
}
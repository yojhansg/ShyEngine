#include "Entity.h"

#include "ResourcesManager.h"
#include "ComponentManager.h"
#include "nlohmann/json.hpp"
#include "ComponentInfo.h"
#include "Preferences.h"
#include "LogManager.h"
#include "Texture.h"
#include "Editor.h"
#include "Camera.h"
#include "Scene.h"
#include "imgui.h"
#include "PrefabManager.h"
#include "SDL.h"
#include "Font.h"
#include "ColorPalette.h"

#include <fstream>
#include "CheckML.h"

#define GizmoImage "gizmo.png"


namespace ShyEditor {

	int Entity::lastId = 1;
	std::vector<int> Entity::unusedIds = std::vector<int>();

	Entity::Entity(std::string& path, bool isTransform) : isTransform(isTransform) {

		editor = Editor::GetInstance();

		name = path;

		this->isTransform = isTransform;

		// Transform attributes
		transform = nullptr;
		overlay = nullptr;

		textureSize = new ImVec2(100, 100);

		if (isTransform) {

			transform = new Transform(this);
		}
		else {

			overlay = new Overlay(this);
		}


		imagePath = path;

		parent = nullptr;

		showGizmo = false;
		visible = true;
		renderOrder = 0;
		prefabId = 0;

		waitingToDelete = false;

		previousMousePosX = 0;
		previousMousePosY = 0;

		AssignId(this);

		// Gizmo texture
		gizmo = ResourcesManager::GetInstance()->AddTexture(GizmoImage, true);

		// Enitity texture
		texture = ResourcesManager::GetInstance()->AddTexture(path, false);

		if (texture->getSDLTexture() != NULL) {

			// Sets the size of the entity based on the texture width and height
			*textureSize = ImVec2(texture->getWidth(), texture->getHeight());

			// Add component image
			Components::Component imageComponent = Components::ComponentManager::GetAllComponents().find("Image")->second;
			Components::AttributeValue attributeValue;
			attributeValue.valueString = imagePath;
			imageComponent.GetAttribute("fileName").SetValue(attributeValue);
			this->AddComponent(imageComponent);

			// Set the entity name as the image name
			std::size_t extensionPos = path.find_last_of('.');
			name = (extensionPos != std::string::npos) ? path.substr(0, extensionPos) : path;

		}

	}

	//Copy constructor
	Entity::Entity(const Entity& entity, bool isPasting)
	{
		editor = entity.editor;

		name = entity.name;

		AssignId(this);

		prefabId = 0;

		if (entity.IsPrefab()) {
			prefabId = entity.id;
			PrefabManager::AddInstance(entity.id, id);
		}
		if (entity.IsPrefabInstance()) {
			prefabId = entity.prefabId;
			PrefabManager::AddInstance(entity.prefabId, id);
		}

		for (auto& pair : entity.components) {
			Components::Component component = pair.second;

			components.emplace(component.GetName(), component);
		}

		for (auto& pair : entity.scripts) {
			Components::Script script = pair.second;

			scripts.emplace(script.GetName(), script);
		}

		visible = entity.visible;
		showGizmo = entity.showGizmo;
		renderOrder = entity.renderOrder;

		isTransform = entity.isTransform;

		if (isTransform) {
			transform = new Transform(*entity.transform, this);
		}
		else {
			overlay = new Overlay(*entity.overlay, this);
		}

		textureSize = new ImVec2(entity.textureSize->x, entity.textureSize->y);

		imagePath = entity.imagePath;

		texture = entity.texture;

		gizmo = entity.gizmo;

		leftMouseButtonDown = entity.leftMouseButtonDown;
		rightMouseButtonDown = entity.rightMouseButtonDown;
		previousMousePosX = entity.previousMousePosX;
		previousMousePosY = entity.previousMousePosY;
		waitingToDelete = entity.waitingToDelete;

		parent = nullptr;

		for (auto& pair : entity.children) {
			Entity* child = new Entity(*pair.second, isPasting);

			child->SetParent(this, isPasting);

			children.emplace(child->GetId(), child);
		}

	}

	Entity::~Entity() {

		if (parent != nullptr)
			parent->RemoveChild(this);

		parent = nullptr;

		for (auto child : children)
			child.second->SetParent(nullptr);

		components.clear();

		delete textureSize;

		if (isTransform)
			delete transform;
		else
			delete overlay;
	}


	// ------------------------------------- Render, update and input -----------------------------------

	void Entity::RenderTransform(SDL_Renderer* renderer, Camera* camera) {

		ImVec2 position = GetAdjustedPosition();

		ImVec2 cameraPosition = camera->GetPosition();
		float cameraScale = camera->GetScale();

		float width = textureSize->x * GetWorldScale().x * cameraScale;
		float height = textureSize->y * GetWorldScale().y * cameraScale;


		ImVec2 worldPosition = ImVec2(position.x * cameraScale + cameraPosition.x, position.y * cameraScale + cameraPosition.y);

		camera->CenterPosition(worldPosition.x, worldPosition.y);

		SDL_Rect dst = { worldPosition.x, worldPosition.y, width, height };

		// Image render
		if (visible && texture != NULL)
			SDL_RenderCopyEx(renderer, texture->getSDLTexture(), NULL, &dst, GetWorldRotation(), NULL, SDL_FLIP_NONE);

		else

			// Render outline
			if (this == editor->GetScene()->GetSelectedEntity()) {

				// Save the previous color to restart it later
				auto& currentPalette = ColorPalette::GetCurrentPalette().scene;
				int thickness = currentPalette.objectFrameWidth;

				if (thickness > 0) {


					Uint8 r, g, b, a;
					SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);


					SDL_SetRenderDrawColor(renderer, ColorPaletteParamsInt(currentPalette.objectFrame), 255);



					for (int i = 0; i < thickness; i++) {
						SDL_Rect currentRect = { dst.x - i, dst.y - i, dst.w + i * 2, dst.h + i * 2 };
						SDL_RenderDrawRect(renderer, &currentRect);
					}

					SDL_SetRenderDrawColor(renderer, r, g, b, a);

				}
				//if (showGizmo) {
				//	dst.x += dst.w / 2;
				//	dst.y -= dst.h / 2;

				//	SDL_RenderCopy(renderer, gizmo->getSDLTexture(), NULL, &dst);
				//}
			}
	}

	void Entity::Update() {

		if (IsPrefabInstance() && PrefabManager::GetPrefabById(GetPrefabId()) == nullptr) {
			SetPrefabId(0);
		}

		if (isTransform) {

			auto image = components.find("Image");

			// If the enitiyt has no image component
			if (image == components.end()) {
				texture = nullptr;
				return;
			}

			std::string currentImagePath = (image->second).GetAttribute("fileName").value.valueString;

			// Checks if the current path exists in the filesystem
			//if (!std::filesystem::exists(currentImagePath)) return;

			// If the image path have changed, checks if the new path is valid
			if (currentImagePath != imagePath) {

				texture = ResourcesManager::GetInstance()->AddTexture(currentImagePath, false);

				if (texture->getSDLTexture() != NULL) {
					imagePath = currentImagePath;
					*textureSize = ImVec2(texture->getWidth(), texture->getHeight());
				}
				else {
					*textureSize = ImVec2(100, 100);
					imagePath = "";
				}
			}
		}

		else {


			overlay->Update();

		}

	}

	bool Entity::HandleInput(SDL_Event* event, bool isMouseInsideEntity, ImVec2 mousePos) {


		return false;
	}



	// ----------------------------- Name, ID and texture getters/setters ---------------------------

	std::string Entity::GetName() {
		return name;
	}

	void Entity::SetName(const std::string& newName) {
		name = newName;
	}

	Texture* Entity::GetTexture() {
		return texture;
	}

	ImVec2& Entity::GetTextureSize() {
		return *textureSize;
	}

	int Entity::GetId() {
		return id;
	}

	void Entity::SetId(int id)
	{
		this->id = id;
	}



	// ----------------------------- Prefab related getters/setters ---------------------------

	void Entity::SetPrefabId(int prefabId)
	{
		this->prefabId = prefabId;

	}

	int Entity::GetPrefabId()
	{
		return prefabId;
	}

	bool Entity::IsPrefab() const
	{
		return id < 0;
	}

	bool Entity::IsPrefabInstance() const
	{
		return prefabId != 0;
	}

	Entity* Entity::GetTopParentPrefab()
	{
		Entity* entity = this;
		while (entity->parent != nullptr && entity->parent->IsPrefab()) {
			entity = entity->parent;
		}

		return entity;
	}


	// ----------------------------------- Visibility getters/setters -----------------------------------

	int Entity::GetRenderOrder() {
		return renderOrder;
	}

	bool Entity::IsVisible() {
		return visible;
	}

	void Entity::SetVisible(bool visible) {
		if (this->parent == nullptr) {
			this->visible = visible;
			SetChildrenVisible(this, visible);
		}
	}

	bool Entity::IsTransform()
	{
		return isTransform;
	}


	// ----------------------------------- Components and Scripts logic --------------------------------------

	void Entity::AddComponent(Components::Component comp) {

		if (components.find(comp.GetName()) == components.end())
			components.insert({ comp.GetName(), comp });

	}

	void Entity::AddScript(Components::Script script) {

		if (scripts.contains(script.GetName())) return;

		scripts.emplace(script.GetName(), script);
	}

	std::unordered_map<std::string, Components::Component>* Entity::GetComponents() {
		return &components;
	}

	std::unordered_map<std::string, Components::Component> Entity::GetComponentsCopy()
	{
		return components;
	}

	std::unordered_map<std::string, Components::Script>* Entity::GetScripts() {
		return &scripts;
	}

	std::unordered_map<std::string, Components::Script> Entity::GetScriptsCopy()
	{
		return scripts;
	}

	void Entity::SetComponents(std::unordered_map<std::string, ::Components::Component> components)
	{
		for (auto& component : components) {
			this->components.emplace(component.first, component.second);
		}

	}

	void Entity::SetScripts(std::unordered_map<std::string, Components::Script> scripts)
	{
		for (auto& script : scripts) {
			this->scripts.emplace(script.first, script.second);
		}
	}



	// --------------------------------- Tranform attributes getters/setters -----------------------------------


	ImVec2& Entity::GetAdjustedPosition() {

		ImVec2 position = GetWorldPosition();

		float width = textureSize->x * GetWorldScale().x;
		float height = textureSize->y * GetWorldScale().y;

		// The entities have their origin at the center
		position.x -= width * 0.5f;
		position.y -= height * 0.5f;

		return position;
	}

	ImVec2& Entity::GetLocalPosition() {
		return transform->GetLocalPosition();
	}

	ImVec2& Entity::GetWorldPosition()
	{

		return transform->GetWorldPosition();
	}

	float& Entity::GetLocalRotation() {
		return transform->GetLocalRotation();
	}

	float& Entity::GetWorldRotation()
	{
		return transform->GetWorldRotation();
	}

	ImVec2& Entity::GetLocalScale() {
		return transform->GetLocalScale();
	}

	ImVec2& Entity::GetWorldScale()
	{
		return transform->GetWorldScale();
	}

	void Entity::SetLocalPosition(ImVec2& newPos) {
		transform->SetLocalPosition(newPos);
	}

	void Entity::SetWorldPosition(ImVec2& pos)
	{
		transform->SetWorldPosition(pos);
	}

	void Entity::SetLocalRotation(float r)
	{
		transform->SetLocalRotation(r);
	}

	void Entity::SetWorldRotation(float r)
	{
		transform->SetWorldRotation(r);
	}

	void Entity::SetLocalScale(ImVec2& newScale)
	{
		transform->SetLocalScale(newScale);
	}

	void Entity::SetWorldScale(ImVec2& scale)
	{
		transform->SetWorldScale(scale);
	}

	Overlay* Entity::GetOverlay()
	{
		return overlay;
	}



	// --------------------------------------- Deleting entity logic ---------------------------------------

	bool Entity::IsWaitingToDelete() {
		return waitingToDelete;
	}

	void Entity::ToDelete() {
		waitingToDelete = true;

		for (auto& attr : referencesToEntity) {
			attr->value.value.entityIdx = 0;
		}

		//If its a prefab instance we remove it from the manager
		if (IsPrefabInstance()) {
			PrefabManager::RemoveInstance(prefabId, id);
		}

		if (id > 0) {
			Entity::unusedIds.push_back(id);
		}
		else {
			PrefabManager::unusedIds.push_back(id);
		}

		for (auto& child : children) {
			child.second->ToDelete();
		}
	}



	// --------------------------------- Entity children and parent logic ------------------------------------

	void Entity::SetParent(Entity* entity, bool adjustToParent) {

		SetTransformToWorldValues();

		parent = entity;

		if (adjustToParent) {
			SetTransformRelativeToNewParent();
		}
	}

	Entity* Entity::GetParent() {
		return parent;
	}

	void Entity::RemoveChild(Entity* entity) {
		entity->SetParent(nullptr);
		children.erase(entity->GetId());
	}

	void Entity::AddChild(Entity* entity) {
		entity->SetParent(this);
		children.insert({ entity->GetId(), entity });
	}

	std::unordered_map<int, Entity*> Entity::GetChildren() {
		return children;
	}

	bool Entity::IsAscendant(Entity* entity) {

		for (auto childPair : children) {
			Entity* child = childPair.second;

			if (child == entity || child->IsAscendant(entity))
				return true;
		}

		return false;
	}




	// ---------------------------------------- Entity drawing logic ----------------------------------------------

	void Entity::DrawTransformInEditor() {

		ImVec2 currentPos = GetLocalPosition();
		currentPos.y *= -1;

		ImGui::Text("Position");
		if (ImGui::DragFloat2("##position_drag", (float*)&currentPos, 0.3f, 0.0f, 0.0f, "%.2f")) {
			ImVec2 newPos = { currentPos.x, -currentPos.y };
			SetLocalPosition(newPos);
		}

		ImVec2 currentScale = GetLocalScale();
		ImGui::Text("Scale");
		if (ImGui::DragFloat2("##scale_drag", (float*)&currentScale, 0.02f, 0.0f, FLT_MAX, "%.2f")) {
			ImVec2 newScale = { currentScale.x, currentScale.y };
			SetLocalScale(newScale);
		}

		float currentRot = GetLocalRotation();
		ImGui::Text("Rotation");
		if (ImGui::DragFloat("##rotation_drag", &currentRot, 0.1f, 0.0f, 0.0f, "%.2f")) {
			SetLocalRotation(currentRot);
		}

		ImGui::Text("Render order");
		ImGui::InputInt("##render_order", &renderOrder);
	}

	void Entity::DrawOverlayInEditor()
	{
		int& placement = overlay->GetPlacement();
		ImGui::Combo("Placement mode", &placement,

			"Positioned\0Streched"
		);


		if (placement == 0) {


			ImGui::Text("Position");
			ImGui::DragFloat2("##Overlayposition_drag", (float*)&overlay->GetPosition(), 0.3f, 0.0f, 0.0f, "%.2f");


			ImGui::Text("Size");
			ImGui::DragFloat2("##OverlaySize_drag", (float*)&overlay->GetSize(), 0.3f, 0.0f, 0.0f, "%.2f");


		}
		else {

			ImGui::Text("Left");
			ImGui::DragInt("##OverlayLeft", &overlay->GetLeft(), 0.3f, 0.0f, 0.0f, "%d");

			ImGui::Text("Top");
			ImGui::DragInt("##OverlayTop", &overlay->GetTop(), 0.3f, 0.0f, 0.0f, "%d");

			ImGui::Text("Right");
			ImGui::DragInt("##OverlayRight", &overlay->GetRight(), 0.3f, 0.0f, 0.0f, "%d");

			ImGui::Text("Bottom");
			ImGui::DragInt("##OverlayBottom", &overlay->GetBottom(), 0.3f, 0.0f, 0.0f, "%d");
		}


		ImGui::SeparatorText("Anchor");

		ImGui::Text("Anchor");
		ImGui::DragFloat2("##Anchor_drag", (float*)&overlay->GetAnchor(), 0.001f, 0.0f, 0.0f, "%.2f");


		int childWidth = 88;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.65f * 0.5f - childWidth * 0.5f);
		ImGui::BeginChild("##AnchoSelect", ImVec2(childWidth, 80));


		DrawArrowButton(overlay->GetAnchor(), ImVec2(-1, -1));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(0, 1));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(1, -1));

		DrawArrowButton(overlay->GetAnchor(), ImVec2(-1, 0));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(0, 0));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(1, 0));

		DrawArrowButton(overlay->GetAnchor(), ImVec2(-1, 1));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(0, -1));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(1, 1));


		ImGui::EndChild();
	}

	//Returns true if there has been a change in a component
	bool Entity::DrawComponentsInEditor() {

		bool changes = false;
		for (auto it = components.begin(); it != components.end();) {
			std::string componentName = (*it).second.GetName();


			if (ImGui::CollapsingHeader(Editor::Beautify(componentName).c_str())) {


				std::string compDescription;
				if (Components::ComponentManager::GetComponentInfo(compDescription, componentName)) {

					Editor::HelpMarker(compDescription);
				}


				for (auto& attribute : (*it).second.GetAttributesOrdered()) {
					std::string attributeName = attribute->GetName();
					Components::Attribute* attr = attribute;

					ImGui::Text(attributeName.c_str());

					std::string attributeDescription;
					if (Components::ComponentManager::GetComponentElementInfo(&attributeDescription, componentName, attributeName, "info")) {

						Editor::HelpMarker(attributeDescription);
					}


					switch (attr->GetType()) {

					case Components::AttributesType::FLOAT:

					{
						std::string enums = "";
						if (Components::ComponentManager::GetComponentElementInfo(&enums, componentName, attributeName, "enum")) {


							int currentSelected = attr->value.value.valueFloat;
							std::vector<std::string> strs = Components::ComponentManager::GetEnum(enums);


							if (ImGui::BeginCombo(("##" + attributeName + it->first).c_str(), strs[currentSelected].c_str())) {


								for (int i = 0; i < strs.size(); i++) {

									bool isSelected = i == currentSelected;

									if (ImGui::Selectable(strs[i].c_str(), isSelected))
									{
										attr->value.value.valueFloat = i;
										changes = true;
									}
								}


								ImGui::EndCombo();
							}

						}
						else
						{
							bool isInteger = Components::ComponentManager::GetComponentElementInfo(NULL, componentName, attributeName, "integer");

							float min = 0;
							float max = 0;

							int minInt = 0;
							int maxInt = 0;

							std::string value;

							if (Components::ComponentManager::GetComponentElementInfo(&value, componentName, attributeName, "min")) {

								min = std::stof(value);
								max = FLT_MAX;

								minInt = min;
								maxInt = INT_MAX;
							}

							if (Components::ComponentManager::GetComponentElementInfo(&value, componentName, attributeName, "max")) {

								max = std::stof(value);
								min = -FLT_MAX;


								minInt = -INT_MAX;
								maxInt = max;
							}

							if (isInteger)
							{
								int intValue = attr->value.value.valueFloat;
								changes = ImGui::DragInt(("##" + attributeName + it->first).c_str(), &intValue, 0.3f, minInt, maxInt, "%d");
								if (changes)
									attr->value.value.valueFloat = intValue;
							}
							else
								changes = ImGui::DragFloat(("##" + attributeName + it->first).c_str(), &attr->value.value.valueFloat, 0.3f, min, max, "%.2f");
						}
					}
					break;
					case Components::AttributesType::VECTOR2:
						changes = DrawVector2(attributeName + it->first, attr);
						break;
					case Components::AttributesType::STRING:

					{

						std::string extension;
						if (Components::ComponentManager::GetComponentElementInfo(&extension, componentName, attributeName, "asset")) {



							std::string completePath = attr->value.valueString;

							std::string output = (completePath == "" ? "None" : completePath);

							int textheight = ImGui::GetTextLineHeight();
							int textheightspacing = ImGui::GetTextLineHeightWithSpacing();

							int width = ImGui::GetWindowWidth() * 0.65f;
							int textWidth = ImGui::CalcTextSize(output.c_str()).x;

							ImGui::BeginChild(("##" + attributeName + it->first).c_str(), ImVec2(width, textheightspacing));


							ImGui::SetCursorPosX((width - textWidth) * 0.5f);
							ImGui::SetCursorPosY((textheightspacing - textheight) * 0.5f);
							ImGui::Text(output.c_str());


							Editor::HelpMarker("You can drag your assets from the file explorer to use them.\nSupported extensions: " + extension);

							ImGui::EndChild();



							auto payload = ImGui::GetDragDropPayload();

							if (payload != nullptr && std::strcmp(payload->DataType, "Asset") == 0) {

								Asset* asset = (Asset*)payload->Data;

								std::string assetExtension = asset->extension;

								if (extension.find(assetExtension) != std::string::npos) {

									if (ImGui::BeginDragDropTarget()) {

										if (ImGui::AcceptDragDropPayload("Asset"))
										{
											attr->value.valueString = asset->relativePath;
											changes = true;
										}

										ImGui::EndDragDropTarget();
									}

								}
							}

						}
						else {

							changes = DrawString(attributeName + it->first, attr);
						}

					}


					break;
					case Components::AttributesType::BOOL:
						changes = DrawBool(attributeName + it->first, attr);
						break;
					case Components::AttributesType::COLOR:
						changes = DrawColor(attributeName + it->first, attr);
						break;
					case Components::AttributesType::CHAR:
						changes = DrawChar(attributeName + it->first, attr);
						break;
					case Components::AttributesType::ENTITY:
						changes = DrawEntity(attributeName + it->first, attr);
						break;
					default:
						break;
					}
				}

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1.0f));

				if (ImGui::Button(("Delete component##" + componentName).c_str(), ImVec2(ImGui::GetColumnWidth(), 40))) {
					it = components.erase(it);
					changes = true;
				}
				else {
					++it;
				}

				ImGui::PopStyleColor(2);

			}
			else
			{

				std::string compDescription;

				if (Components::ComponentManager::GetComponentInfo(compDescription, componentName)) {

					Editor::HelpMarker(compDescription);
				}

				++it;
			}
		}

		return changes;
	}

	bool Entity::DrawScriptsInEditor() {
		bool changes = false;

		for (auto it = scripts.begin(); it != scripts.end();) {

			std::string scriptName = Editor::Beautify((*it).first);
			if (ImGui::CollapsingHeader(scriptName.c_str())) {

				for (auto& attribute : (*it).second.GetAllAttributes()) {
					std::string attributeName = attribute.first;
					Components::Attribute* attr = &attribute.second;

					ImGui::Text(attributeName.c_str());

					switch (attr->GetType()) {

					case Components::AttributesType::FLOAT:

					{
						if (it->second.IsEnum(attributeName))
						{

							std::string enums = it->second.GetEnum(attributeName);


							int currentSelected = attr->value.value.valueFloat;
							std::vector<std::string> strs = Components::ComponentManager::GetEnum(enums);


							if (ImGui::BeginCombo(("##" + attributeName + it->first).c_str(), strs[currentSelected].c_str())) {


								for (int i = 0; i < strs.size(); i++) {

									bool isSelected = i == currentSelected;

									if (ImGui::Selectable(strs[i].c_str(), isSelected))
									{
										attr->value.value.valueFloat = i;
										changes = true;
									}
								}


								ImGui::EndCombo();
							}

						}
						else
							changes = DrawFloat(attributeName + it->first, attr);
					}
					break;
					case Components::AttributesType::VECTOR2:
						changes = DrawVector2(attributeName + it->first, attr);
						break;
					case Components::AttributesType::STRING:
						changes = DrawString(attributeName + it->first, attr);
						break;
					case Components::AttributesType::BOOL:
						changes = DrawBool(attributeName + it->first, attr);
						break;
					case Components::AttributesType::COLOR:
						changes = DrawColor(attributeName + it->first, attr);
						break;
					case Components::AttributesType::CHAR:
						changes = DrawChar(attributeName + it->first, attr);
						break;
					case Components::AttributesType::ENTITY:
						changes = DrawEntity(attributeName + it->first, attr);
						break;
					default:
						break;
					}
				}

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255.0f, 255.0f, 255.0f, 1.0f));

				if (ImGui::Button(("Edit script##" + scriptName).c_str(), ImVec2(ImGui::GetColumnWidth(), 40))) {
					editor->OpenScript(scriptName);
					changes = true;
				}

				if (ImGui::Button(("Delete script##" + scriptName).c_str(), ImVec2(ImGui::GetColumnWidth(), 40)))
				{
					it = scripts.erase(it);
					changes = true;
				}
				else
					++it;

				ImGui::PopStyleColor(2);

			}
			else ++it;
		}
		return changes;
	}


	//Add an attribute that references this object
	void Entity::AddReferenceToEntity(Components::Attribute* attr)
	{
		referencesToEntity.push_back(attr);
	}


	// ------------------------ Serialization and deseralization logic -------------------------

	std::string Entity::ToJson() {

		nlohmann::ordered_json j;
		j["name"] = name;

		nlohmann::ordered_json childsJson = nlohmann::json::array();
		for (auto& childPair : children) {
			auto child = j.parse(childPair.second->ToJson());

			childsJson.push_back(child);
		}

		j["id"] = id;

		j["childs"] = childsJson;

		j["order"] = renderOrder;

		j["prefabId"] = prefabId;

		j["isTransform"] = isTransform;

		if (isTransform) {
			ImVec2 position = GetLocalPosition();
			ImVec2 size = GetLocalScale();
			float rotation = GetLocalRotation();

			j["localPosition"] = std::to_string(position.x) + ", " + std::to_string(-position.y);
			j["localScale"] = std::to_string(size.x) + ", " + std::to_string(size.y);
			j["localRotation"] = std::to_string(rotation);
		}
		else {

			ImVec2 position = ImVec2(overlay->GetPosition());
			ImVec2 size = ImVec2(overlay->GetSize());

			//"placement", "anchor", "top", "left", "right", "bottom", "position", "size", "color", "interactable"

			j["placement"] = std::to_string(overlay->GetPlacement());
			j["anchor"] = std::to_string(overlay->GetAnchor().x) + "," + std::to_string(overlay->GetAnchor().y);
			j["top"] = std::to_string(overlay->GetTop());
			j["left"] = std::to_string(overlay->GetLeft());
			j["right"] = std::to_string(overlay->GetRight());
			j["bottom"] = std::to_string(overlay->GetBottom());
			j["position"] = std::to_string(position.x) + ", " + std::to_string(position.y);
			j["size"] = std::to_string(size.x) + ", " + std::to_string(size.y);


			//TODO: color e interactable
		}

		nlohmann::ordered_json componentsJson = nlohmann::json::array();
		for (auto it = components.begin(); it != components.end(); it++) {


			componentsJson.push_back(it->second.ToJson());
		}

		j["components"] = componentsJson;


		nlohmann::ordered_json scriptsJson = nlohmann::json();
		for (auto it = scripts.begin(); it != scripts.end(); it++) {
			auto scriptJson = j.parse(it->second.ToJson());

			if (scriptJson.is_null())
				scriptJson = nlohmann::json::array();

			scriptsJson[it->second.GetPath() + it->second.GetName()] = scriptJson;
		}

		j["scripts"] = scriptsJson;


		return j.dump(2);
	}

	Entity* Entity::FromJson(nlohmann::ordered_json& jsonData) {

		std::string errorMsg = "The JSON entity has not the correct format.";

		if (!jsonData.contains("name")) {
			LogManager::LogError(errorMsg);
			return nullptr;
		}

		std::string entityName = jsonData["name"];


		Entity::unusedIds.push_back(jsonData["id"]);

		bool isTransform = jsonData["isTransform"];

		Entity* entity = new Entity(entityName, isTransform);
		entity->name = entityName;

		entity->prefabId = jsonData["prefabId"];

		if (entity->prefabId != 0) {
			PrefabManager::AddInstance(entity->prefabId, entity->id);
		}

		if (!jsonData.contains("id")) {
			LogManager::LogError(errorMsg);
			return nullptr;
		}

		if (!jsonData.contains("order")) {
			LogManager::LogError(errorMsg);
			return entity;
		}

		entity->renderOrder = jsonData["order"];

		if (isTransform)
		{
			if (!jsonData.contains("localPosition")) {
				LogManager::LogError(errorMsg);
				return entity;
			}

			if (!jsonData.contains("localScale")) {
				LogManager::LogError(errorMsg);
				return entity;
			}

			if (!jsonData.contains("localRotation")) {
				LogManager::LogError(errorMsg);
				return entity;
			}

			// Deserialize localPosition, localScale, and localRotation
			std::string localPositionStr = jsonData["localPosition"];
			std::string localScaleStr = jsonData["localScale"];
			std::string localRotation = jsonData["localRotation"];

			// Parse localPosition and localScale
			sscanf_s(localPositionStr.c_str(), "%f, %f", &entity->GetLocalPosition().x, &entity->GetLocalPosition().y);
			sscanf_s(localScaleStr.c_str(), "%f, %f", &entity->GetLocalScale().x, &entity->GetLocalScale().y);

			//La y se maneja internamente en el editor por comodidad de forma invertida -> hacia abajo es positivo, hacia arriba negativo
			//Por ello al serializar o deserializar invertimos su valor (el motor usa el eje invertido)

			entity->GetLocalPosition().y *= -1;

			entity->SetLocalRotation(std::stof(localRotation));
		}
		else {

			//"placement", "anchor", "top", "left", "right", "bottom", "position", "size", "color", "interactable"

			entity->overlay->GetPlacement() = jsonData.contains("placement") ? std::stoi(jsonData["placement"].get<std::string>()) : 0;
			entity->overlay->GetTop() = jsonData.contains("top") ? std::stoi(jsonData["top"].get<std::string>()) : 0;
			entity->overlay->GetBottom() = jsonData.contains("bottom") ? std::stoi(jsonData["bottom"].get<std::string>()) : 0;
			entity->overlay->GetLeft() = jsonData.contains("left") ? std::stoi(jsonData["left"].get<std::string>()) : 0;
			entity->overlay->GetRight() = jsonData.contains("right") ? std::stoi(jsonData["right"].get<std::string>()) : 0;

			if (jsonData.contains("position")) {

				std::string position = jsonData["position"];
				sscanf_s(position.c_str(), "%f, %f", &entity->overlay->GetPosition().x, &entity->overlay->GetPosition().y);
			}

			if (jsonData.contains("size")) {

				std::string size = jsonData["size"];
				sscanf_s(size.c_str(), "%f, %f", &entity->overlay->GetSize().x, &entity->overlay->GetSize().y);
			}

			//TODO: color y interactable

		}

		// Components
		if (!jsonData.contains("components")) {
			LogManager::LogError(errorMsg);
			return entity;
		}

		for (const auto& compJson : jsonData["components"]) {
			Components::Component component = Components::Component::FromJson(compJson.dump());
			entity->AddComponent(component);

		}

		// Scripts
		if (!jsonData.contains("scripts")) {
			LogManager::LogError(errorMsg);
			return entity;
		}

		for (const auto& scriptJson : jsonData["scripts"].items()) {
			Components::Script script = Components::Script::fromJson(scriptJson.key(), scriptJson.value().dump());
			entity->AddScript(script);
		}

		if (!jsonData.contains("childs")) {
			LogManager::LogError(errorMsg);
			return entity;
		}

		for (auto& childJson : jsonData["childs"]) {
			Entity* child = Entity::FromJson(childJson);

			entity->AddChild(child);
			child->SetParent(entity, false);
		}

		return entity;
	}



	// ------------------------ Id Management -------------------------

	void Entity::AssignId(Entity* entity)
	{
		// See if we can reutilize an id
		if (Entity::unusedIds.size() != 0) {
			int  id = Entity::unusedIds.back();
			Entity::unusedIds.pop_back();

			if (IsIdAlreadyUsed(id)) {
				AssignId(entity);
			}
			else {
				entity->SetId(id);
			}
		}
		else {

			//Ensure id is not being used already
			while (IsIdAlreadyUsed(Entity::lastId)) {
				Entity::lastId++;
			}

			entity->SetId(Entity::lastId);
			Entity::lastId++;
		}
	}

	bool Entity::IsIdAlreadyUsed(int id)
	{
		std::map<int, Entity*>& entities = Editor::GetInstance()->GetScene()->GetEntities();
		std::vector<Entity*>& overlays = Editor::GetInstance()->GetScene()->GetOverlays();

		if (entities.find(id) != entities.end()) {
			return true;
		}

		for (auto overlay : overlays) {
			if (overlay->GetId() == id) {
				return true;
			}
		}

		return false;
	}


	// ------------------------ Editor drawing methods -------------------------

	bool Entity::DrawFloat(std::string attrName, ::Components::Attribute* attr) {

		return ImGui::DragFloat(("##" + attrName).c_str(), &attr->value.value.valueFloat, 0.3f, 0.0f, 0.0f, "%.2f");
	}

	bool Entity::DrawVector2(std::string attrName, ::Components::Attribute* attr) {

		return ImGui::DragFloat2(("##" + attrName).c_str(), (float*)&attr->value.value.valueVector2, 0.3f, 0.0f, 0.0f, "%.2f");
	}

	bool Entity::DrawString(std::string attrName, ::Components::Attribute* attr, bool readOnly) {

		char inputBuffer[256];
		strncpy_s(inputBuffer, attr->value.valueString.c_str(), sizeof(inputBuffer));

		int flags = readOnly ? ImGuiInputTextFlags_ReadOnly : 0;

		if (ImGui::InputText(("##" + attrName).c_str(), inputBuffer, sizeof(inputBuffer), flags)) {
			attr->value.valueString = inputBuffer;
			return true;
		}

		return false;
	}

	bool Entity::DrawBool(std::string attrName, ::Components::Attribute* attr) {

		return ImGui::Checkbox(("##" + attrName).c_str(), &attr->value.value.valueBool);
	}

	bool Entity::DrawColor(std::string attrName, ::Components::Attribute* attr) {

		return ImGui::ColorEdit3(("##" + attrName).c_str(), (float*)&attr->value.value.valueColor);
	}

	bool Entity::DrawChar(std::string attrName, ::Components::Attribute* attr) {

		return ImGui::InputText(("##" + attrName).c_str(), &attr->value.value.valueChar, 2, ImGuiInputTextFlags_CharsNoBlank);
	}

	bool Entity::DrawEntity(std::string attrName, ::Components::Attribute* attr) {

		std::map<int, Entity*>& entities = editor->GetScene()->GetEntities();
		std::vector<Entity*>& overlays = editor->GetScene()->GetOverlays();
		std::unordered_map<int, Entity*>& prefabs = PrefabManager::GetPrefabs();

		Entity* entity = nullptr;

		auto entityIt = entities.find(attr->value.value.entityIdx);
		auto prefab = PrefabManager::GetPrefabById(attr->value.value.entityIdx);

		if (prefab != nullptr) {
			entity = prefab;
		}
		else if (entityIt != entities.end()) {
			entity = entityIt->second;
		}
		else {
			for (auto overlay : overlays) {
				if (overlay->GetId() == attr->value.value.entityIdx) {
					entity = overlay;
				}
			}
		}

		if (entity != nullptr && entity->IsPrefab()) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.831f, 0.168f, 0.604f, 1.0f));
		}

		if (ImGui::BeginCombo(("##" + attrName).c_str(), entity != nullptr ? entity->GetName().c_str() : "")) {

			if (entity != nullptr && entity->IsPrefab()) {
				ImGui::PopStyleColor();
			}

			//Prefabs cant reference entities
			if (IsPrefab()) {
				ImGui::EndCombo();
				return false;
			}

			for (auto entity : entities) {
				std::string selectableId = entity.second->GetName() + "##" + std::to_string(entity.second->id);
				if (ImGui::Selectable(selectableId.c_str())) {
					attr->value.value.entityIdx = entity.second->GetId();

					entity.second->AddReferenceToEntity(attr);

					ImGui::EndCombo();

					return true;
				}
			}

			for (auto overlay : overlays) {
				std::string selectableId = overlay->GetName() + "##" + std::to_string(overlay->id);
				if (ImGui::Selectable(selectableId.c_str())) {
					attr->value.value.entityIdx = overlay->GetId();

					overlay->AddReferenceToEntity(attr);

					ImGui::EndCombo();

					return true;
				}
			}

			for (auto prefab : prefabs) {

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.831f, 0.168f, 0.604f, 1.0f));

				std::string selectableId = prefab.second->GetName() + "##" + std::to_string(prefab.second->id);

				if (ImGui::Selectable(selectableId.c_str())) {
					attr->value.value.entityIdx = prefab.second->GetId();

					prefab.second->AddReferenceToEntity(attr);

					ImGui::PopStyleColor();

					ImGui::EndCombo();

					return true;
				}

				ImGui::PopStyleColor();
			}

			ImGui::EndCombo();
		}
		else if (entity != nullptr && entity->IsPrefab()) {
			ImGui::PopStyleColor();
		}


		ImGui::SameLine();

		if (ImGui::Button(("X##" + attrName).c_str())) {
			attr->value.value.entityIdx = 0;
			return true;
		}

		return false;
	}

	void Entity::DrawArrowButton(ImVec2& value, const ImVec2& dir)
	{

		if (!(dir.x == dir.y && dir.x == 0))
		{
			//Vertical arrows
			if (dir.x == 0)
			{
				if (dir.y < 0) {
					if (ImGui::ArrowButton("##Down", ImGuiDir_Down))
						value = ImVec2(0.5f, 1);
				}
				else
					if (ImGui::ArrowButton("##Up", ImGuiDir_Up))
						value = ImVec2(0.5f, 0);
				return;
			}

			//Horizontal arrows
			if (dir.y == 0) {

				if (dir.x < 0) {
					if (ImGui::ArrowButton("##Left", ImGuiDir_Left))
						value = ImVec2(0, 0.5f);
				}
				else
					if (ImGui::ArrowButton("##Right", ImGuiDir_Right))
						value = ImVec2(1, 0.5f);
				return;
			}
		}

		ImGui::PushID((void*)&dir);

		int size = ImGui::GetFrameHeight();

		if (ImGui::Button("##", ImVec2(size, size))) {

			value = ImVec2(dir.x * 0.5f + 0.5f, dir.y * 0.5f + 0.5f);
		}

		auto drawList = ImGui::GetWindowDrawList();

		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();



		//Diagonal arrows
		ImVec2 a = min;
		ImVec2 b = ImVec2(max.x, min.y);
		ImVec2 c = max;
		ImVec2 d = ImVec2(min.x, max.y);


		size *= 0.25f;

		a.x += size;
		a.y += size;

		b.x -= size;
		b.y += size;

		c.x -= size;
		c.y -= size;

		d.x += size;
		d.y -= size;

		if (dir.x == dir.y && dir.x == 0) {

			drawList->AddQuadFilled(a, b, c, d, IM_COL32(255, 255, 255, 255));
			ImGui::PopID();
			return;
		}



		ImVec2 p1, p2, p3;


		if (dir.x < 0)
		{
			if (dir.y < 0)
			{
				p1 = a;
				p2 = b;
				p3 = d;
			}
			else {
				p1 = a;
				p2 = c;
				p3 = d;
			}
		}
		else {

			if (dir.y < 0)
			{
				p1 = a;
				p2 = b;
				p3 = c;
			}
			else {
				p1 = b;
				p2 = c;
				p3 = d;
			}
		}



		drawList->AddTriangleFilled(p1, p2, p3, IM_COL32(255, 255, 255, 255));

		ImGui::PopID();
	}




	// ---------------------- Entity children settings (Transform and visibility) ----------------------


	void Entity::SetChildrenVisible(Entity* entity, bool visible) {

		for (auto childPair : entity->GetChildren()) {

			childPair.second->visible = visible;

			SetChildrenVisible(childPair.second, visible);
		}
	}

	void Entity::SetTransformRelativeToNewParent()
	{
		if (IsTransform() && parent != nullptr) {
			ImVec2 parentPos = parent->GetWorldPosition();
			ImVec2 newPos = { GetLocalPosition().x - parentPos.x, GetLocalPosition().y - parentPos.y };

			//Tiene sentido
			newPos = Transform::rotate(-parent->GetWorldRotation(), newPos);

			SetLocalPosition(newPos);

			float parentRot = parent->GetWorldRotation();
			float newRot = GetLocalRotation() - parentRot;

			SetLocalRotation(newRot);
		
			ImVec2 parentScale = parent->GetWorldScale();
			ImVec2 newScale = { GetLocalScale().x / parentScale.x, GetLocalScale().y / parentScale.y };

			SetLocalScale(newScale);
		}
	}

	void Entity::SetTransformToWorldValues()
	{
		if (IsTransform()) {
			ImVec2 worldPos = GetWorldPosition();
			SetLocalPosition(worldPos);

			ImVec2 worldScale = GetWorldScale();
			SetLocalScale(worldScale);

			float worldRotation = GetWorldRotation();
			SetLocalRotation(worldRotation);
		}
	}




	//============ COMPONENTS =================================================================================================================================================================

	Transform::Transform(Entity* obj) : obj(obj)
	{
		scale = new ImVec2(1, 1);
		position = new ImVec2(0, 0);
		rotation = 0;
	}

	Transform::Transform(const Transform& tr, Entity* obj)
	{
		this->obj = obj;

		scale = new ImVec2(tr.scale->x, tr.scale->y);
		position = new ImVec2(tr.position->x, tr.position->y);
		rotation = tr.rotation;
	}

	Transform::~Transform()
	{
		delete scale;
		delete position;

		scale = nullptr;
		position = nullptr;
	}

	ImVec2& Transform::GetLocalPosition()
	{
		return *position;
	}

	ImVec2& Transform::GetWorldPosition()
	{
		Entity* parent = obj->GetParent();

		if (parent != nullptr) {
			
			//Tiene sentido
			ImVec2 rotatedPosition = Transform::rotate(parent->GetWorldRotation(), GetLocalPosition());
			ImVec2 parentWorldPos = parent->GetWorldPosition();

			ImVec2 worldPos = { parentWorldPos.x + rotatedPosition.x, parentWorldPos.y + rotatedPosition.y };

			return worldPos;
		}


		return GetLocalPosition();
	}

	float& Transform::GetLocalRotation()
	{
		return rotation;
	}

	float& Transform::GetWorldRotation()
	{
		Entity* parent = obj->GetParent();

		if (parent != nullptr) {
			float worldRotation = GetLocalRotation() + parent->GetWorldRotation();

			return worldRotation;
		}

		return GetLocalRotation();
	}

	ImVec2& Transform::GetLocalScale()
	{
		return *scale;
	}

	ImVec2& Transform::GetWorldScale()
	{
		Entity* parent = obj->GetParent();

		if (parent != nullptr) {
			ImVec2 scale = GetLocalScale();
			ImVec2 parentScale = parent->GetWorldScale();

			ImVec2 worldScale = { scale.x * parentScale.x, scale.y * parentScale.y };

			return worldScale;
		}

		return GetLocalScale();
	}

	void Transform::SetLocalPosition(ImVec2& newPos)
	{
		position->x = newPos.x;
		position->y = newPos.y;
	}

	void Transform::SetWorldPosition(ImVec2& pos)
	{
		Entity* parent = obj->GetParent();

		if (parent != nullptr) {
			ImVec2 parentWorldPos = parent->GetWorldPosition();

			ImVec2 newPos = { pos.x - parentWorldPos.x, pos.y - parentWorldPos.y };

			newPos = Transform::rotate(-parent->GetWorldRotation(), newPos);

			SetLocalPosition(newPos);
			return;
		}

		SetLocalPosition(pos);
	}

	void Transform::SetLocalRotation(float r)
	{
		rotation = r;
	}

	void Transform::SetWorldRotation(float r)
	{
		Entity* parent = obj->GetParent();

		if (parent != nullptr) {

			SetLocalRotation(r - parent->GetWorldRotation());
			return;
		}

		SetLocalRotation(r);
	}

	void Transform::SetLocalScale(ImVec2& newScale)
	{
		scale->x = newScale.x;
		scale->y = newScale.y;
	}

	void Transform::SetWorldScale(ImVec2& scale)
	{
		Entity* parent = obj->GetParent();

		if (parent != nullptr) {

			ImVec2 parentWorldScale = parent->GetWorldScale();

			ImVec2 newScale = { scale.x / parentWorldScale.x, scale.y / parentWorldScale.y };

			SetLocalScale(newScale);
			return;
		}

		SetLocalScale(scale);
	}


	//ImVec2 doesnt have rotate method, this is just a workaround so we dont have to create a whole Vector2 class just for this
	ImVec2& Transform::rotate(float degrees, ImVec2 vec)
	{
		ImVec2 r;
		degrees = fmod(degrees, 360.0f);
		if (degrees > 180.0f) {
			degrees = degrees - 360.0f;
		}
		else if (degrees <= -180.0f) {
			degrees = 360.0f + degrees;
		}

		float angle = degrees * PI / 180.0f;
		float sine = sin(angle);
		float cosine = cos(angle);

		//rotation matrix
		float matrix[2][2] = { { cosine, -sine }, { sine, cosine } };

		float x = vec.x;
		float y = vec.y;

		r.x = matrix[0][0] * x + matrix[0][1] * y;
		r.y = matrix[1][0] * x + matrix[1][1] * y;

		return r;
	}




	//===== Overlays=====================================
	Overlay::Overlay(Entity* obj) : obj(obj)
	{
		placement = 0;

		position = new ImVec2(0, 0);
		anchor = new ImVec2(0.5f, 0.5f);
		size = new ImVec2(100, 100);
		scale = 1;

		left = top = right = bottom = 0;

		image = new OverlayImage();
		text = new OverlayText();
	}

	Overlay::Overlay(const Overlay& ov, Entity* obj)
	{
		this->obj = obj;

		placement = ov.placement;
		position = new ImVec2(ov.position->x, ov.position->y);
		size = new ImVec2(ov.size->x, ov.size->y);

		anchor = new ImVec2(ov.anchor->x, ov.anchor->y);

		left = ov.left;
		top = ov.top;
		right = ov.right;
		bottom = ov.bottom;

		scale = ov.scale;
		interactable = ov.interactable;

		image = new OverlayImage();
		text = nullptr;
	}

	Overlay::~Overlay()
	{
		delete anchor;
		delete size;
		delete position;
		delete image;
		delete text;
	}

	int& Overlay::GetPlacement()
	{
		return placement;
	}

	ImVec2& Overlay::GetAnchor()
	{
		return *anchor;
	}

	float& Overlay::GetScale()
	{
		return scale;
	}

	bool& Overlay::GetInteractable()
	{
		return interactable;
	}

	OverlayImage* Overlay::GetImage()
	{
		return image;
	}

	ImVec2& Overlay::GetPosition()
	{
		return *position;
	}

	void Overlay::SetPosition(ImVec2* pos)
	{
		delete position;

		position = pos;
	}

	ImVec2& Overlay::GetSize()
	{
		return *size;
	}

	int& Overlay::GetLeft()
	{
		return left;
	}

	int& Overlay::GetRight()
	{
		return right;
	}

	int& Overlay::GetTop()
	{
		return top;
	}

	int& Overlay::GetBottom()
	{
		return bottom;
	}

	void Overlay::CalculateRectangle(int& x, int& y, int& w, int& h)
	{
		if (placement == 0) {

			x = (int)std::round(position->x);
			y = (int)std::round(position->y);
			w = (int)std::round(size->x);
			h = (int)std::round(size->y);

			x -= anchor->x * size->x;
			y -= anchor->y * size->y;

			auto parent = obj->GetParent();

			if (parent != nullptr) {

				parent->GetOverlay();

				auto parentPos = parent->GetOverlay()->CalculateCenterPoint();
				x += parentPos.x;
				y += parentPos.y;
			}

		}
		else {


			int parent_left, parent_top, parent_width, parent_height;
			auto parent = obj->GetParent();


			if (parent != nullptr) {

				parent->GetOverlay()->CalculateRectangle(parent_left, parent_top, parent_width, parent_height);
			}
			else {

				parent_left = 0;
				parent_top = 0;
				parent_width = Preferences::GetData().width;
				parent_height = Preferences::GetData().height;
			}

			x = parent_left + left;
			y = parent_top + top;
			w = parent_width - right - left;
			h = parent_height - bottom - top;
		}

	}

	ImVec2 Overlay::CalculateCenterPoint()
	{
		ImVec2 center;
		if (placement == 0) {

			center = *position;

			auto parent = obj->GetParent();

			if (parent != nullptr)
			{
				auto parentCenter = parent->GetOverlay()->CalculateCenterPoint();

				center.x += parentCenter.x;
				center.y += parentCenter.y;
			}

		}
		else {

			int x, y, w, h;
			CalculateRectangle(x, y, w, h);

			center = { x + w * anchor->x, y + h * anchor->y };

		}

		return center;
	}

	void Overlay::Update()
	{

		auto components = obj->GetComponents();

		auto imgIt = components->find("OverlayImage");

		if (imgIt != components->end()) {

			auto& imgCmp = *imgIt;

			const auto& cmpPath = imgCmp.second.GetAttribute("path").value.valueString;

			if (cmpPath != image->GetPath()) {


				Texture* texture = ResourcesManager::GetInstance()->AddTexture(cmpPath, false);

				if (texture->getSDLTexture() == nullptr) {

					texture = nullptr;
				}

				image->SetTexture(cmpPath, texture);
			}

		}
		else {

			image->SetTexture("", nullptr);
		}




		auto textIt = components->find("OverlayText");

		if (textIt != components->end()) {

			auto& textCmp = *textIt;

			const auto& fnt = textCmp.second.GetAttribute("font").value.valueString;
			const auto& txt = textCmp.second.GetAttribute("text").value.valueString;
			const auto& size = textCmp.second.GetAttribute("fontSize").value.value.valueFloat;

			const auto& fit = textCmp.second.GetAttribute("fit").value.value.valueFloat;
			const auto& hAlign = textCmp.second.GetAttribute("horizontalAlignment").value.value.valueFloat;
			const auto& vAlign = textCmp.second.GetAttribute("verticalAlignment").value.value.valueFloat;

			int x, y, w, h;
			CalculateRectangle(x, y, w, h);


			text->SetText(txt, fnt, size, fit, hAlign, vAlign, w);

		}
		else if (text != nullptr) {

			text->Clear();
		}

	}

	void Overlay::Render(SDL_Renderer* renderer, int x, int y, int w, int h)
	{

		if (image != nullptr)
			image->Render(renderer, x, y, w, h);

		if (text != nullptr)
			text->Render(renderer, x, y, w, h);
		//TODO: render text
	}



	OverlayImage::OverlayImage()
	{
		texture = nullptr;
		path = "";
	}

	void OverlayImage::Render(SDL_Renderer* renderer, int x, int y, int w, int h)
	{

		if (texture != nullptr) {

			SDL_Rect dest{ x, y, w, h };
			SDL_RenderCopy(renderer, texture->getSDLTexture(), NULL, &dest);
		}
	}

	std::string OverlayImage::GetPath()
	{
		return path;
	}

	Texture* OverlayImage::GetTexture()
	{
		return texture;
	}

	void OverlayImage::SetTexture(std::string path, Texture* texture)
	{
		this->path = path;
		this->texture = texture;
	}



	OverlayText::OverlayText()
	{
		path = "";
		text = "";


		font = nullptr;
		texture = nullptr;

		fit = horizontalAlignment = verticalAlignment = 0;
		width = 0;
	}

	OverlayText::~OverlayText()
	{
		if (texture != nullptr) {

			delete texture;
			texture = nullptr;
		}

	}

	std::string OverlayText::GetPath()
	{
		return path;
	}

	std::string OverlayText::GetText()
	{
		return text;
	}

	void OverlayText::Clear()
	{
		if (texture != nullptr) {

			delete texture;
			texture = nullptr;
		}

		text = "";
		path = "";
		fontSize = 0;
		font = nullptr;
		verticalAlignment = horizontalAlignment = 0;
		fit = 0;

	}

	void OverlayText::SetText(const std::string text, const std::string path, int size, int fit, int hAlign, int vAlign, int width)
	{
		bool updateText = false;

		horizontalAlignment = hAlign;
		verticalAlignment = vAlign;


		if (this->path != path || this->fontSize != size) {

			this->path = path;
			this->fontSize = size;

			font = ResourcesManager::GetInstance()->AddFont(path, fontSize);
			updateText = true;

			if (font->getSDLFont() == nullptr && texture != nullptr) {
				delete texture;
				texture = nullptr;
			}

		}

		if (font == nullptr || font->getSDLFont() == nullptr)
			return;


		if (this->text != text) {

			this->text = text;
			updateText = true;
		}

		if (texture == nullptr)
			updateText = true;

		if (fit / 2 != this->fit / 2) {
			updateText = true;
		}
		this->fit = fit;

		if (width != this->width && (fit == (int)Fit::WrapClamp || fit == (int)Fit::WrapOverflow))
		{
			this->width == width;
			updateText = true;
		}

		if (updateText) {

			if (texture != nullptr) {

				delete texture;
				texture = nullptr;
			}

			if (fit == (int)Fit::WrapClamp || fit == (int)Fit::WrapOverflow)
				texture = font->CreateWrappedText(text, width);
			else
				texture = font->CreateText(text);

			if (texture != nullptr) {

				texture_w = texture->getWidth();
				texture_h = texture->getHeight();

			}

		}
	}

	Texture* OverlayText::GetTexture()
	{
		return texture;
	}

	void OverlayText::Render(SDL_Renderer* renderer, int x, int y, int w, int h)
	{
		if (texture == nullptr)
			return;


		SDL_Rect source{ 0, 0, texture_w, texture_h };
		SDL_Rect destination{ x, y, w, h };


		switch ((Fit)fit) {

		case Fit::WrapClamp:
		case Fit::Clamp: {

			int w = std::min(source.w, destination.w);
			int h = std::min(source.h, destination.h);


			if (horizontalAlignment != (int)HorizontalAlignment::Left) {

				float mult = horizontalAlignment == (int)HorizontalAlignment::Center ? 0.5f : 1;

				source.x = std::max(0, source.w - w) * mult;
				destination.x += std::max(0, destination.w - w) * mult;
			}

			if (verticalAlignment != (int)VerticalAlignment::Top) {

				float mult = verticalAlignment == (int)VerticalAlignment::Center ? 0.5f : 1;

				source.y = std::max(0, source.h - h) * mult;
				destination.y += std::max(0, destination.h - h) * mult;
			}


			source.w = w;
			source.h = h;

			destination.w = source.w;
			destination.h = source.h;

		}

		case Fit::WrapOverflow:
		case Fit::Overflow: {

			float multx = horizontalAlignment == (int)HorizontalAlignment::Left ? 0 : horizontalAlignment == (int)HorizontalAlignment::Center ? 0.5f : 1;
			float multy = verticalAlignment == (int)VerticalAlignment::Top ? 0 : verticalAlignment == (int)VerticalAlignment::Center ? 0.5f : 1;

			destination.x += (destination.w - source.w) * multx;
			destination.y += (destination.h - source.h) * multy;

			destination.h = source.h;

			destination.w = source.w;
			break;
		}

		default:
			break;
		}


		SDL_RenderCopy(renderer, texture->getSDLTexture(), &source, &destination);

	}
}



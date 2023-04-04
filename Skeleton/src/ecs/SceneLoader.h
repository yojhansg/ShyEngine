#pragma once

#include <string>
#include <vector>

#undef new
#include "json.hpp"
#include "CheckML.h"

namespace nlohmann {
	using jsonarray = std::vector<json>;
}

namespace ECS {

	class Scene;
	class Entity;
	class Overlay;

	class SceneLoader {

	public:

		static const std::string extension;

		SceneLoader() = delete;

		static ECS::Scene* LoadScene(std::string const& scenePath);


	private:
		static void ProcessOverlay(ECS::Scene* scene, nlohmann::json& overlay, ECS::Overlay* parent);
		static void ProcessComponent(ECS::Entity* scene, nlohmann::json& component);
	};

}


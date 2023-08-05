#pragma once


#include <string>
#include <unordered_map>

#include "ComponentInfo.h"


namespace Components {

	using cstring = std::string const&;

	class ComponentManager {


	private:

		static ComponentManager* instance;
		static void EnsureInitialised();

		std::unordered_map<std::string, Component> components;

	public:

		static void Initialise();
		static void Release();

		static void ReadComponentInfo(cstring path);

		static std::unordered_map<std::string, Component> const& GetAllComponents();
	};
}

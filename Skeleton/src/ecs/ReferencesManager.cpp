#include "ReferencesManager.h"
#include <ConsoleManager.h>
#include "Entity.h"

void ECS::ReferencesManager::AddEntityToMap(int id, Entity* e) {

	if (!map.contains(id))
		map.insert(std::make_pair(id, e));
	else
		Console::Output::PrintError("Duplicated entity ID", "The reference could not be added. An entity with that ID already exists.");
}

void ECS::ReferencesManager::RemoveEntityFromMap(int id) {

	if (map.contains(id)) {
		map.at(id) = nullptr;
		map.erase(id);
	}
	else
		Console::Output::PrintError("Entity with non-existent ID", "The reference could not be deleted. There is no entity with that ID.");

}

bool ECS::ReferencesManager::IsEntityValid(int id) {

	if (map.contains(id))
		return map.at(id) != nullptr;

	Console::Output::PrintError("Entity with non-existent ID", "The validity of the entity cannot be checked because there is no reference with that ID.");
	return false;
}

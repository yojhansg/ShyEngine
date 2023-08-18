#include "ReferencesManager.h"

void ECS::ReferencesManager::AddEntityToMap(int id, Entity* e) {

	map.insert(std::make_pair(id, e));

}

void ECS::ReferencesManager::RemoveEntityFromMap(int id) {



}

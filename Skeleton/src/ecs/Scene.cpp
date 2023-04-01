#include "Scene.h"
#include "Entity.h"

namespace ECS {

    Scene::Scene(const std::string& sce_name) {
        name = sce_name;
    }

    Scene::~Scene() {
        for (auto e : entities) {
            delete e;
        }
        entities.clear();
    }

    void Scene::init() {
        for (auto e : entities) {
            e->init();
        }
    }

    void Scene::start() {
        for (auto e : entities) {
            e->start();
        }
    }

    void Scene::update(float deltaTime) {

        for (auto it = entities.begin(); it != entities.end(); it++) {
            Entity* e = *it;

            if (e->isActive() && !e->isRemoved())
                e->udpate(deltaTime);
            else if (e->isRemoved())
                entitiesRemoved.push_back(it);
        }
    }

    void Scene::lateUpdate(float deltaTime) {
        for (auto e : entities) {
            if (e->isActive() && !e->isRemoved())
                e->lateUpdate(deltaTime);
        }
    }

    void Scene::render() {
        for (auto e : entities) {
            if (e->isActive() && !e->isRemoved())
                e->render();
        }
    }

    void Scene::fixedUpdate(float fixedDeltaTime) {
        for (auto e : entities) {
            if (e->isActive() && !e->isRemoved())
                e->fixedUpdate(fixedDeltaTime);
        }
    }

    void Scene::removeEntities() {
        for (auto it : entitiesRemoved) {
            delete* it;
            entities.erase(it);
        }
        entitiesRemoved.clear();

    }

    void Scene::onSceneUp() {
        for (auto e : entities) {
            if (e->isActive() && !e->isRemoved())
                e->onSceneUp();
        }
    }

    void Scene::onSceneDown() {
        for (auto e : entities) {
            if (e->isActive() && !e->isRemoved())
                e->onSceneDown();
        }
    }

    std::string Scene::getName() {
        return name;
    }

    void Scene::setName(const std::string& sce_name) {
        name = sce_name;
    }

    Entity* Scene::createEntity(const std::string& name) {
        
        Entity* e = new Entity(name, this);
        entities.push_back(e);

        return e;

    }

    void Scene::destroyEntity(Entity* ent) {
        ent->removeEntity();
    }

    Entity* Scene::findEntityByName(const std::string& ent_name) {
        for (auto e : entities) {
            if (e->name == name)  return e;
        }

        return nullptr;
    }
}

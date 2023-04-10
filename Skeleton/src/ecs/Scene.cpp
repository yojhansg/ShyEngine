#include "Scene.h"
#include "Entity.h"

#include "RendererManager.h"


namespace ECS {

    Scene::Scene(const std::string& sce_name) {
        name = sce_name;

        cameraPosition = { 0 ,0 };
        cameraScale = 1;
    }

    Scene::~Scene() {
        for (auto e : entities) {
            delete e;
        }
        entities.clear();
    }

    void Scene::init() {
        
        Renderer::RendererManager::instance()->SetCameraPosition(cameraPosition);
        Renderer::RendererManager::instance()->SetCameraScale(cameraScale);

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
            else if (e->isRemoved() && !e->inRemovedEntityList)
                removeEntity(e);
        }
    }

    void Scene::lateUpdate(float deltaTime) {

        for (auto it = entities.begin(); it != entities.end(); it++) {
            Entity* e = *it;

            if (e->isActive() && !e->isRemoved())
                e->lateUpdate(deltaTime);
            else if (e->isRemoved() && !e->inRemovedEntityList)
                removeEntity(e);
        }

    }

    void Scene::render() {
        for (auto e : entities) {
            if (e->isActive() && !e->isRemoved())
                e->render();
        }
    }

    void Scene::fixedUpdate(float fixedDeltaTime) {
        for (auto it = entities.begin(); it != entities.end(); it++) {
            Entity* e = *it;

            if (e->isActive() && !e->isRemoved())
                e->fixedUpdate(fixedDeltaTime);
            else if (e->isRemoved() && !e->inRemovedEntityList)
                removeEntity(e);
        }
    }

    void Scene::handleRemovedEntities() {
        for (auto it : removedEntities) {
            delete* it;
            entities.erase(it);
        }
        removedEntities.clear();

        for (auto e : entities) {
            if (!e->removedComponents.empty()) {
                e->removeComponents();
            }
        }

    }

    void Scene::onDestroy() {
        for (auto e : entities) {
            e->onDestroy();
        }
    }

    void Scene::onDestroyOnRemovedEntities() {
        for (auto e : entities) {
            if (e->isRemoved())
                e->onDestroy();
            else if (e->hasRemovedComponents) {
                e->onDestroyRemovedComponents();
                e->hasRemovedComponents = false;
            }
        }
    }

    void Scene::onSceneUp() {

        Renderer::RendererManager::instance()->SetCameraPosition(cameraPosition);
        Renderer::RendererManager::instance()->SetCameraScale(cameraScale);


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

    Entity* Scene::createEntity(const std::string& name, int renderOder) {
        
        Entity* e = new Entity(name, this, renderOder);
        entities.push_back(e);

        return e;

    }

    void Scene::removeEntity(Entity* ent) {
        std::list<Entity*>::iterator it = entities.begin();
        while (it != entities.end()) {

            if (*it == ent)
            {
                ent->inRemovedEntityList = true;
                removedEntities.push_back(it);
                return;
            }
            it++;
        }
    }

    Entity* Scene::findEntityByName(const std::string& ent_name) {
        for (auto e : entities) {
            if (e->name == name)  return e;
        }

        return nullptr;
    }

    void Scene::Event(std::string const& name)
    {
        for (auto e : entities) {
            if (e->isActive() && !e->isRemoved())
                e->Event(name);
        }
    }

}

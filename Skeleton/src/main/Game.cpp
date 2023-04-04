#include "Game.h"
#include <Entity.h>
#include <Scene.h>

#include <RendererManager.h> 
#include <PhysicsManager.h>
#include <ConsoleManager.h>
#include <SceneManager.h>

#include <Components/Overlay.h>
#include <Components/TestComponent.h>
#include <Components/OverlayImage.h>
#include <Components/OverlayText.h>
#include <Components/CircleBody.h>
#include <Components/Transform.h>
#include <Components/ChainBody.h>
#include <Components/EdgeBody.h>
#include <Components/BoxBody.h>
#include <Components/Image.h>

#include <ECSUtilities/FunctionManager.h>
#include <ECSUtilities/ClassReflection.h>
#include <Scripting/Script.h>


Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	scene = nullptr;
}

void Game::initScenes() {

	firstScene();

	sceneManager->SetScene(scene);

}

void Game::firstScene() {

	Renderer::RendererManager* renderer = Renderer::RendererManager::instance();

	// 1.- Scene
	scene = sceneManager->createScene("Default scene");

	// 2.- Entities

		// Link
		ECS::Entity* player = scene->createEntity("Player");

		auto tr = player->addComponent<ECS::Transform>();
		auto im = player->addComponent<ECS::Image>("link.png");
		auto body = player->addComponent<ECS::BoxBody>();
		auto tComp = player->addComponent<ECS::TestComponent>();

		// Ground
		ECS::Entity* ground = scene->createEntity("Ground");
	
		auto grTr = ground->addComponent<ECS::Transform>();
		auto grIm = ground->addComponent<ECS::Image>("ground.png");
		auto grBody = ground->addComponent<ECS::EdgeBody>();

		// Ball
		/*ECS::Entity* ball = scene->createEntity("Ball");

		auto trBall = ball->addComponent<ECS::Transform>();
		auto imBall = ball->addComponent<ECS::Image>("ball.png");
		auto ballBody = ball->addComponent<ECS::CircleBody>();*/

	
	// 3.- Init
	scene->init();

	// 4.- Components settings

		// Player
		tr->setPosition(Utilities::Vector2D(renderer->getWidth() / 2, renderer->getHeight() / 2));
		tr->setScale(Utilities::Vector2D(0.35f, 0.35f));
		body->setBodyType((int)ECS::PhysicBody::BODY_TYPE::DYNAMIC);
		body->setBounciness(0.8f);
		//body->setTrigger(true);

		// Ground
		grTr->setScale(Utilities::Vector2D(1, 1));
		grTr->setPosition(Utilities::Vector2D(renderer->getWidth() / 2, renderer->getHeight() / 1.2f));
		grBody->addOffSet(0, -1);

		// Ball
		/*trBall->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 4);
		trBall->setScale(0.25f, 0.25f);
		ballBody->setBodyType((int)ECS::PhysicBody::BODY_TYPE::DYNAMIC);
		ballBody->setBounciness(0.5f); */

}

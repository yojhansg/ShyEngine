#include "Game.h"

#include <SceneManager.h>
#include <RendererManager.h>
#include <PhysicsManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Components/Transform.h>
#include <Components/Image.h>
#include <Components/BoxBody.h>
#include <Components/CircleBody.h>
#include <Components/EdgeBody.h>
#include <Scripting/Script.h>
#include <Components/ChainBody.h>
#include <Components/TestComponent.h>
#include <ECSUtilities/FunctionManager.h>
#include <ECSUtilities/ClassReflection.h>

#include <map>
#include <fstream>

#include <Components/OverlayElement.h>
#include <Components/OverlayImage.h>
#include <Components/OverlayText.h>

#include <ConsoleManager.h>


Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	scene = nullptr;
}

void Game::initScenes() {

	firstScene();
	//flappyBird();

	//readScene("DefaultScene.json");
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
		body->setBounciness(0.5f);
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

	// 5.- Start

	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

}

void Game::flappyBird() {

	Renderer::RendererManager* renderer = Renderer::RendererManager::instance();

	// 1.- Scene
	scene = sceneManager->createScene("Default scene");

	// 2.- Entities

		// Fondo
		ECS::Entity* background = scene->createEntity("fondo");
		auto bgTr = background->addComponent<ECS::Transform>();
		auto bg = background->addComponent<ECS::Image>("fondo.png");
		background->addComponent<ECS::Script>()->Initialise("Parallax");

		// Pollo
		ECS::Entity* player = scene->createEntity("Player");
		auto tr = player->addComponent<ECS::Transform>();
		auto im = player->addComponent<ECS::Image>("pollo.png");
		auto body = player->addComponent<ECS::BoxBody>();

		// Tuberias
		ECS::Entity* tuberia = scene->createEntity("Tuberia");
		auto tubTr = tuberia->addComponent<ECS::Transform>();
		auto tubIm = tuberia->addComponent<ECS::Image>("tuberiaFlappyBird.png");
		auto tubBody = tuberia->addComponent<ECS::BoxBody>();

		ECS::Entity* tuberiaInv = scene->createEntity("Tuberia");
		auto tubTrInv = tuberiaInv->addComponent<ECS::Transform>();
		auto tubImInv = tuberiaInv->addComponent<ECS::Image>("tuberiaFlappyBird.png");
		auto tubBodyInv = tuberiaInv->addComponent<ECS::BoxBody>();

		player->addComponent<ECS::Script>()->Initialise("FlappyBirdJump");
		player->addComponent<ECS::Script>()->Initialise("RestartOnCollision");

		tuberia->addComponent<ECS::Script>()->Initialise("MovimientoTuberia");
		tuberiaInv->addComponent<ECS::Script>()->Initialise("MovimientoTuberia");

	// 3.- Init
	scene->init();

	// 4.- Components settings

		// Fondo
	std::map<std::string, std::string> trMap;
	trMap["position_"] = "(" + std::to_string(renderer->getWidth() / 2) + "," + std::to_string(renderer->getHeight() / 2) + ")";
	trMap["scale_"] = "(1.6f, 1.6f)";

	ClassReflection::instance()->ReflectComponent("Transform", bgTr, trMap);

		//bgTr->setScale(1.6f, 1.6f);
		//bgTr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 2);

		//// Pollo
		//tr->setPosition(200 / 2, renderer->getHeight() / 2);
		//tr->setScale(0.35f, 0.35f);

		//body->setBounciness(0.5f);
		//body->setRotationFreezed(true);

		//// Tuberias
		//tubTrInv->setRotation(180);
		//tubTrInv->setPosition(1400, 150);
		//tubTr->setPosition(1400, renderer->getHeight() - 150);

		//tubBody->setBodyType((int)ECS::PhysicBody::BODY_TYPE::KINEMATIC);
		//tubBody->setGravityScale(0);

		//tubBodyInv->setBodyType((int)ECS::PhysicBody::BODY_TYPE::KINEMATIC);
		//tubBodyInv->setGravityScale(0);

	// 5.- Start

	//Yojhan - Scripts
	//player->addComponent<ECS::Script>()->Initialise("printGameObjectName.json");
	//player->addComponent<ECS::Script>()->Initialise("printWhenKeyDown.json");
	//player->addComponent<ECS::Script>()->Initialise("moveUpWhenKeyDown.json");

	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();
}

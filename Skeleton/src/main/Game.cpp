#include "Game.h"
#include <Entity.h>
#include <Scene.h>

#include <RendererManager.h> 
#include <PhysicsManager.h>
#include <ConsoleManager.h>
#include <SceneManager.h>
#include <SoundManager.h>

#include <Components/Overlay.h>
#include <Components/LinkComponent.h>
#include <Components/BallComponent.h>
#include <Components/OverlayImage.h>
#include <Components/SoundEmitter.h>
#include <Components/MusicEmitter.h>
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

	auto renderer = Renderer::RendererManager::instance();
	auto sound = Sound::SoundManager::instance();

	sound->setMasterVolume(0.1f);

	// 1.- Scene
	scene = sceneManager->createScene("Default scene");

	// 2.- Entities

		// Link
		ECS::Entity* player = scene->createEntity("Player");

		auto tr = player->addComponent<ECS::Transform>();
		auto im = player->addComponent<ECS::Image>("images/link.png");
		auto body = player->addComponent<ECS::BoxBody>();
		auto linkEmitter = player->addComponent<ECS::SoundEmitter>("sounds/jump.mp3");
		auto lComp = player->addComponent<ECS::LinkComponent>();

		// Ground
		ECS::Entity* ground = scene->createEntity("Ground");
	
		auto grTr = ground->addComponent<ECS::Transform>();
		auto grIm = ground->addComponent<ECS::Image>("images/ground.png");
		auto grBody = ground->addComponent<ECS::EdgeBody>();

		// Ball
		ECS::Entity* ball = scene->createEntity("Ball");

		auto trBall = ball->addComponent<ECS::Transform>();
		auto imBall = ball->addComponent<ECS::Image>("images/ball.png");
		auto ballBody = ball->addComponent<ECS::CircleBody>();
		auto ballEmitter = ball->addComponent<ECS::SoundEmitter>("sounds/retro.wav");
		auto bComp = ball->addComponent<ECS::BallComponent>();

	
	// 3.- Init
	scene->init();

	// 4.- Components settings

		// Player
		tr->SetLocalPosition({ 0, renderer->getHeight() / 3.0f });
		tr->SetScale({0.35f, 0.35f});
		body->setBodyType((int) ECS::PhysicBody::BODY_TYPE::DYNAMIC);
		body->setFriction(0.5f);
		linkEmitter->shouldPlayOnStart(false);
		linkEmitter->enableSpatialSound(true);

		// Ground
		grTr->SetLocalPosition({ 0, -renderer->getHeight() / 3.0f });
		grBody->addOffSet(0, grBody->getSize().getY() / 2);
		
		// Ball
		trBall->SetLocalPosition({ renderer->getWidth() / 4.0f, renderer->getHeight() / 3.0f});
		trBall->SetScale({ 0.25f, 0.25f });
		ballBody->setBodyType((int) ECS::PhysicBody::BODY_TYPE::DYNAMIC);
		ballEmitter->shouldPlayOnStart(false);

}

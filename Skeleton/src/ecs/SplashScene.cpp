#include "SplashScene.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Components/Overlay.h"
#include "Components/OverlayText.h"
#include "Components/OverlayImage.h"

ECS::SplashScene::SplashScene()
{
	timer = 0;
	maxTime = 3;
}

ECS::SplashScene::~SplashScene()
{
}

void ECS::SplashScene::update(float dt)
{
	timer += dt;

	if (timer > maxTime) {

		SceneManager::instance()->ChangeScene("", SceneManager::POP);
	}
}

void ECS::SplashScene::SetMaxtTime(float time)
{
	maxTime = time;
}

void ECS::SplashScene::CreateSplashScreen(Scene* scene)
{
	Entity* manager = scene->createEntity("Manager");
	manager->addComponent<ECS::SplashScene>();

	auto overlay = manager->addComponent<ECS::Overlay>();

	overlay->SetStreched(0, 0, 0, 0);
	auto img = manager->addComponent<ECS::OverlayImage>();
	img->path = "fondoWindows.jpg";


	float size = 600;
	float sizey = 400;

	int fit = 1;

	Entity* center = scene->createEntity("text");
	auto textOverlay = center->addComponent<ECS::Overlay>();
	textOverlay->SetPositioned({ 0, 0 }, { size, sizey });
	textOverlay->SetParent(overlay);
	auto text = center->addComponent<ECS::OverlayText>();
	text->SetText("Phoshorus");
	text->SetFit(fit);
	text->SetPointSize(150);
	text->SetFont("Cute Notes.ttf");
	text->SetHorizontalAlignment(1);
	text->SetVerticalAlignment(1);


	center = scene->createEntity("text");
	textOverlay = center->addComponent<ECS::Overlay>();
	textOverlay->SetPositioned({ 0, 110 }, { size, sizey });
	textOverlay->SetParent(overlay);
	text = center->addComponent<ECS::OverlayText>();
	text->SetText("game engine");
	text->SetFit(fit);
	text->SetPointSize(60);
	text->SetFont("Default.ttf");
	text->SetHorizontalAlignment(1);
	text->SetVerticalAlignment(1);
}

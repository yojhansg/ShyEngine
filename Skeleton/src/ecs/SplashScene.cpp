#include "SplashScene.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Components/Overlay.h"
#include "Components/OverlayText.h"
#include "Components/OverlayImage.h"
#include "RendererManager.h"
#include "ResourcesManager.h"

#define Main_Font "Cute Notes.ttf"
#define Secondary_Font "Default.ttf"
#define Background "fondoWindows.jpg"


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
	Entity* manager = scene->createEntityNoId("Manager");
	manager->addComponent<ECS::SplashScene>();

	auto overlay = manager->addComponent<ECS::Overlay>();

	overlay->SetStreched(0, 0, 0, 0);
	auto img = manager->addComponent<ECS::OverlayImage>();
	img->path = Background;


	float size = Renderer::RendererManager::instance()->getWidth() / 3.2f;
	float sizey = size / 1.5f;

	int fit = 1;

	Entity* center = scene->createEntityNoId("text");
	auto textOverlay = center->addComponent<ECS::Overlay>();
	textOverlay->SetPositioned({ 0, 0 }, { size, sizey });
	textOverlay->SetParent(overlay);
	auto text = center->addComponent<ECS::OverlayText>();
	text->SetText("shy");
	text->SetFit(fit);
	text->SetPointSize(size / 4);
	text->SetFont(Main_Font);
	text->SetHorizontalAlignment(1);
	text->SetVerticalAlignment(1);


	center = scene->createEntityNoId("text");
	textOverlay = center->addComponent<ECS::Overlay>();
	textOverlay->SetPositioned({ 0, size / 5.4f }, { size, sizey });
	textOverlay->SetParent(overlay);
	text = center->addComponent<ECS::OverlayText>();
	text->SetText("game engine");
	text->SetFit(fit);
	text->SetPointSize(size / 10);
	text->SetFont(Secondary_Font);
	text->SetHorizontalAlignment(1);
	text->SetVerticalAlignment(1);
}


//TODO: cambiar el nombre de estos recursos
void ECS::SplashScene::LoadResources()
{
	float size = Renderer::RendererManager::instance()->getWidth() / 3.2f;
	
	Resources::ResourcesManager::instance()->addFont(Main_Font, size / 4);
	Resources::ResourcesManager::instance()->addFont(Secondary_Font, size / 10);
	Resources::ResourcesManager::instance()->addTexture(Background);
}

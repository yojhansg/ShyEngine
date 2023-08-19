#include "Animation.h"
#include "Entity.h"
#include "Image.h"

ECS::Animation::Animation()
{
	animationSheetStyle = 0;
	currentAnimation = 0;
	currentFrame = 0;
	animationColumns = 1;
	animationRows = 1;
	speed = 1;
	image = nullptr;

	animationTimer = 0;

	frameWidth = 0;
	frameHeight = 0;
	animationLenght = 0;
}

ECS::Animation::~Animation() {}

void ECS::Animation::init()
{
	image = entity->getComponent<ECS::Image>();

	if (image == nullptr) {
		printError("Animation Component", "The entity doesn't contain a valid image component.");
		this->remove();
		return;
	}

	frameWidth = image->getTextureWidth() / animationColumns;
	frameHeight = image->getTextureHeight() / animationRows;

	ChangeAnimationFrame();
}

void ECS::Animation::start() {
	ChangeAnimationFrame();
}

void ECS::Animation::update(float dt)
{
	animationTimer += dt * speed;

	if (animationTimer > 1) {

		AdvanceFrame();
		animationTimer = 0;
	}
}

void ECS::Animation::ChangeAnimationPath(cstring str, int frame, int animation, int column, int row)
{
	image->loadTexture(str);

	animationRows = row;
	animationColumns = column;

	frameWidth = image->getTextureWidth() / animationColumns;
	frameHeight = image->getTextureHeight() / animationRows;

	currentFrame = frame;
	currentAnimation = animation;

	image->setSrcRect(currentFrame * frameWidth, currentAnimation * frameHeight, frameWidth, frameHeight);
}

void ECS::Animation::AdvanceFrame()
{
	currentFrame++;
	int maxFrames = animationSheetStyle == (int)AnimationSheet::Horizontal ? animationColumns : animationRows;

	if (currentFrame >= maxFrames || currentFrame >= animationLenght) currentFrame = 0;

	ChangeAnimationFrame();
}

void ECS::Animation::AdvanceAnimation()
{
	currentAnimation++;

	int maxFrames = animationSheetStyle == (int)AnimationSheet::Horizontal ? animationRows : animationColumns;

	if (currentAnimation >= maxFrames) currentAnimation = 0;

	ChangeAnimationFrame();
}

void ECS::Animation::SetFrame(int idx)
{
	currentFrame = idx;

	ChangeAnimationFrame();
}

void ECS::Animation::SetAnimation(int idx)
{
	currentAnimation = idx;

	ChangeAnimationFrame();
}

int ECS::Animation::GetFrame()
{
	return currentFrame;
}

int ECS::Animation::GetAnimation()
{
	return currentAnimation;
}

void ECS::Animation::SetAnimationLenght(int lenght)
{
	animationLenght = lenght;
}

int ECS::Animation::getAnimationLenght()
{
	return animationLenght;
}

void ECS::Animation::ChangeAnimationFrame()
{
	if (animationSheetStyle == (int)AnimationSheet::Horizontal)
		image->setSrcRect(currentFrame * frameWidth, currentAnimation * frameHeight, frameWidth, frameHeight);
	else
		image->setSrcRect(currentAnimation * frameWidth, currentFrame * frameHeight, frameWidth, frameHeight);
}

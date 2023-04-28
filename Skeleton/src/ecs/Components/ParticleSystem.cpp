#include "ParticleSystem.h"
#include <ResourcesManager.h>
#include "EngineTime.h"
#include "Transform.h"
#include "Particle.h"
#include "Entity.h"
#include "Texture.h"
#include <cassert>
#include <ConsoleManager.h>

#include <iostream>
#include <windows.h>

namespace ECS {

	// ------------------ BURST ----------------------

	Burst::Burst(float time, int particles, int cycles, float rate, float probability) {
		this->timeToBurst = time;
		this->nParticles = particles;
		this->cycles = cycles;
		this->rate = rate;
		this->probability = probability;
	}

	// -------------- PARTICLE SYSTEM ----------------

	ParticleSystem::ParticleSystem() {
		tr = nullptr;

		duration = 0; loop = false; delay = 0; startLifeTime = 0;
		startSpeed = 0; startSize = { 0, 0 }; startRotation = 0; startPosition = { 0, 0 };
		maxParticles = 0; playOnStart = false; useImage = false; 
		fileName = ""; startColor = { 0, 0, 0 }; alpha = 0;
		flipmode = 0; renderOrder = 0; emissionRateOverTime = 0;
		emissionRateOverDistance = 0; shape = 0; gravityScale = 0;
		speedOverLifeTime = 0; colorOverLifeTime = { 0, 0 };
		sizeOverLifeTime = { 0, 0 }; rotationOverLifeTime = 0;
		inheritPosition = false; inheritRotation = false;
		inheritVelocity = false; emitting = false; useGravity = false;
		currentTime = 0; timer = 0; startDirection = { 0, 0 };
		texture = nullptr; srcX = 0; srcY = 0; nParticles = 0;
	}

	ParticleSystem::~ParticleSystem() {
		closeSystem();
	}

	void ParticleSystem::init() {

		tr = this->getEntity()->getComponent<Transform>();
		assert(tr != nullptr, "La entidad debe contener un componente Transform");

		// General system configuration
		maxParticles = 50;
		duration = 10.0f;
		loop = true;
		delay = 0.0f;
		playOnStart = true;

		// Particles START configuration
		startLifeTime = 5.0f;
		startSpeed = 50.0f;
		startSize = { 30.0f, 30.0f };
		startRotation = 0.0f;
		startPosition = tr->GetLocalPosition();
		startDirection = { 0, 1 };
		startColor = { 255, 255, 255 };

		// Physics particles configuration
		useGravity = false;
		gravityScale = 1;

		// Render configuration
		useImage = false;
		alpha = 255;
		flipmode = 0;
		renderOrder = 0;
		shape = 0;
		srcX = 0;
		srcY = 0;

		// Emission coniguration
		inheritPosition = false;
		inheritRotation = false;
		inheritVelocity = false;
		emissionRateOverTime = 1;
		emissionRateOverDistance = 0;

		// Over lifetime configuration
		speedOverLifeTime = startSpeed; 
		colorOverLifeTime = startColor;
		sizeOverLifeTime = startSize; 
		rotationOverLifeTime = startRotation;

	}

	void ParticleSystem::start() {

		changeTexture(fileName);

		for (int i = 0; i < maxParticles; i++)
			particles.push_back(new Particle(this, i));

		for (int i = maxParticles - 1; i >= 0; i--)
			particlesPool.push(i);

		if (playOnStart)
			emitting = true;

	}

	void ParticleSystem::update(float deltaTime) {

		//debugSystemInfo();

		if (!emitting) return;

		timeSinceStart += deltaTime;

		// If the system is not emitting or the current time is smaller than delay to start time
		if (timeSinceStart < delay) return;

		currentTime += deltaTime;

		// If the time since the system started is greater than the system duration
		if (!loop && (currentTime >= duration)) emitting = false;

		// Particle emission
		timer += deltaTime;
		if (timer > 1.0f / emissionRateOverTime) {
			timer = 0;

			takeParticleFromPool();
		}

		// Particles update
		for (auto p : particles)
			p->udpate(deltaTime);

		handleUnusedParticles();

	}

	void ParticleSystem::fixedUpdate(float deltaTime) {
		
	}

	void ParticleSystem::render() {

		// Particles render
		for (auto p : particles)
			p->render();

	}

	void ParticleSystem::takeParticleFromPool() {

		int particleIdx = particlesPool.top();
		particlesPool.pop();

		particles[particleIdx]->setAsUsed();

		nParticles++;

	}

	void ParticleSystem::returnParticleToPool(int idx) {

		particles[idx]->setAsUnused();

		particlesPool.push(idx);

		nParticles--;

	}

	void ParticleSystem::closeSystem() {
		for (auto b : bursts)
			delete b;

		bursts.clear();

		for (auto p : particles)
			delete p;

		particles.clear();

		unusedParticles.clear();
	}

	void ParticleSystem::handleUnusedParticles() {
		
		for (auto d : unusedParticles)
			returnParticleToPool(d->getIdx());

		unusedParticles.clear();

	}

	void ParticleSystem::debugSystemInfo() {
		system("cls");
		print("Numero de particulas: " + std::to_string(nParticles));
		print("Tiempo actual: " + std::to_string(currentTime));
		print("Tiempo desde comienzo: " + std::to_string(timeSinceStart));
	}

	void ParticleSystem::changeTexture(cstring texturePath) {
		fileName = texturePath;
		texture = Resources::ResourcesManager::instance()->addTexture(fileName);
		srcWidth = texture->getWidth(); srcHeight = texture->getHeight();
	}

	void ParticleSystem::addBurst(float time, int particles, int cycles, float rate, float probability) {
		Burst* newBurst = new Burst(time, particles, cycles, rate, probability);

		bursts.push_back(newBurst);
	}

	void ParticleSystem::setEmissionRateOverTime(float rate) {
		this->emissionRateOverTime = rate;
	}

	void ParticleSystem::startEmitting() {
		emitting = true;
	}

}

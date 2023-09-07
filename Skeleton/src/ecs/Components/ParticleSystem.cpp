#include "ParticleSystem.h"
#include <ResourcesManager.h>
#include <PhysicsManager.h>
#include <ConsoleManager.h>
#include "EngineTime.h"
#include "Transform.h"
#include "Particle.h"
#include "Entity.h"
#include "Texture.h"
#include <cassert>
#include <Random.h>

namespace ECS {

	// ------------------ BURST ----------------------

	Burst::Burst(ParticleSystem* system, float time, int particles, int cycles, float rate, float probability) {

		this->system = system;

		this->timeToBurst = time;
		this->rate = rate;
		this->nParticles = particles;
		this->cycles = cycles;
		this->rate = rate;
		this->probability = probability;

		burstTimer = 0;
		intervalTimer = 0;
		currentCycles = cycles;
	}

	void Burst::reset() {
		burstTimer = 0;
		currentCycles = cycles;
		intervalTimer = 0;
	}

	float Burst::getCurrentTime() {
		return burstTimer;
	}

	void Burst::setBurstTimer(float time) {
		burstTimer = time;
	}

	float Burst::getIntervalTime() {
		return intervalTimer;
	}

	void Burst::setIntervalTimer(float time) {
		intervalTimer = time;
	}

	float Burst::getRate() {
		return timeToBurst;
	}

	int Burst::getNumberOfParticles() {
		return nParticles;
	}

	int Burst::getCycles() {
		return currentCycles;
	}

	void Burst::decreaseCycles() {
		currentCycles--;
	}

	float Burst::getProbabilityToBurst() {
		return probability;
	}

	float Burst::getTimeBetweenBursts() {
		return rate;
	}

	// -------------- PARTICLE SYSTEM ----------------

	ParticleSystem::ParticleSystem() {

		std::srand(std::time(nullptr));

		physicsManager = Physics::PhysicsManager::instance();

		world = physicsManager->getWorld();

		screenToWorldFactor = physicsManager->getScreenToWorldFactor();

		tr = nullptr;

	}

	ParticleSystem::~ParticleSystem() {
		closeSystem();
	}

	void ParticleSystem::init() {

		tr = this->getEntity()->getComponent<Transform>();
		if (tr == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		for (int i = 0; i < maxParticles; i++)
			particles.push_back(new Particle(this, i));

		for (int i = maxParticles - 1; i >= 0; i--)
			particlesPool.push(i);
	}

	void ParticleSystem::start() {

		prevPosition = tr->GetLocalPosition();

		loadTexture(fileName);

		if (playOnStart)
			emitting = true;

	}

	void ParticleSystem::update(float deltaTime) {

		if (!emitting) return;

		timeSinceStart += deltaTime;

		// If the system is not emitting or the current time is smaller than delay to start time
		if (timeSinceStart < delay) return;

		currentTime += deltaTime;

		// If the time since the system started is greater than the system duration
		if (currentTime >= duration) {
			reset();
			return;
		}

		// Particle emission

			// Emission rate over distance
			if ((tr->GetLocalPosition() - prevPosition).magnitude() > physicsManager->getScreenToWorldFactor() / emissionRateOverDistance) {
				prevPosition = tr->GetLocalPosition();

				takeParticleFromPool();

			}

			// Emission rate over time
			timer += deltaTime;
			if (timer > 1.0f / emissionRateOverTime) {
				timer = 0;

				takeParticleFromPool();
			}

			// Bursts
			handleBursts(deltaTime);

	}

	void ParticleSystem::fixedUpdate(float fixedDeltaTime) {

		// Particles fixedupdate
		for (auto p : particles)
			p->fixedUpdate(fixedDeltaTime);

		// Particles remove
		handleUnusedParticles();

	}

	void ParticleSystem::render() {

		// Particles render
		for (auto p : particles)
			p->render();

	}

	void ParticleSystem::takeParticleFromPool() {

		if (particlesPool.empty()) return;

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

	void ParticleSystem::handleBursts(float deltaTime) {

		for (auto b : bursts) {
			// If has already burst n cycles
			if (b->getCycles() <= 0) continue;

			// If it is time to burst
			if (b->getCurrentTime() > b->getRate() && b->getIntervalTime() <= 0) {

				// Calculate probability to burst
				float rnd = Utilities::Random::RandomBetween(0.0f, 1.0f);
				if (b->getProbabilityToBurst() < rnd) continue;

				b->decreaseCycles();
				b->setIntervalTimer(b->getTimeBetweenBursts());

				// Burst
				for (int i = 0; i < b->getNumberOfParticles(); i++)
					takeParticleFromPool();
			}

			// Timers
			b->setBurstTimer(b->getCurrentTime() + deltaTime);
			b->setIntervalTimer(b->getIntervalTime() - deltaTime);

		}

	}

	void ParticleSystem::reset() {

		for (auto p : particles) {
			if (p->isBeingUsed())
				p->setAsUnused();
		}

		handleUnusedParticles();

		emitting = loop;
		currentTime = 0;
		timer = 0;

		// Reset bursts
		for (auto b : bursts)
			b->reset();

	}

	void ParticleSystem::startEmitting() {
		emitting = true;
	}

	void ParticleSystem::stopEmitting() {
		emitting = false;
	}

	bool ParticleSystem::isEmitting() {
		return emitting;
	}

	void ParticleSystem::loadTexture(cstring texturePath) {
		fileName = texturePath;
		texture = Resources::ResourcesManager::instance()->addTexture(fileName);
		srcWidth = texture->getWidth(); srcHeight = texture->getHeight();
	}

	void ParticleSystem::addBurst(float time, int particles, int cycles, float rate, float probability) {
		Burst* newBurst = new Burst(this, time, particles, cycles, rate, probability);

		bursts.push_back(newBurst);
	}

}

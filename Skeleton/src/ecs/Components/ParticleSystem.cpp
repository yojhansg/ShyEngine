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

		// General system configuration
		duration = 10.0f;
		maxParticles = 1000;
		loop = true;
		delay = 0.0f;
		playOnStart = true;

		// Particles START configuration
		lifeTime = 5.0f;
		randomLifeTimeBetweenTwoValues = false;

		speed = 50.0f;
		randomSpeedBetweenTwoValues = false;

		size = { 30.0f, 30.0f };
		randomSizeBetweenTwoValues = false;

		angle = 0.0f;
		randomAngleBetweenTwoValues = false;

		rotation = 0.0f;
		randomRotationBetweenTwoValues = false;

		color = { 255, 255, 255 };
		randomColorBetweenTwoColors = false;

		alpha = 255;
		randomAlphaBetweenTwoValues = false;

		force = 0;
		randomForceBetweenTwoValues = false;

		angularVelocity = 0;
		randomAngularVelocityBetweenTwoValues = false;

		bounciness = 1;
		randomBouncinessBetweenTwoValues = false;

		impulse = 0;
		randomImpulseBetweenTwoValues = false;

		// Physics particles configuration
		sensor = false;
		layerName = "Particles";
		gravityScale = 0;
		angularVelocity = 0;

		// Render configuration
		alpha = 255;
		flipmode = 0;
		srcX = 0;
		srcY = 0;

		// Emission coniguration
		emissionRateOverTime = 1;
		emissionRateOverDistance = 0;

		// Over lifetime configuration
		overLifeTimeSpeed = false;
		endSpeed = speed;

		overLifeTimeColor = false;
		endColor = color;

		overLifeTimeSize = false;
		endSize = size;

		overLifeTimeAlpha = false;
		endAlpha = alpha;



		// Custom values

		/*duration = 20.0f;
		playOnStart = true;
		loop = true;

		layerName = "Particles";
		gravityScale = 1;
		sensor = false;

		fileName = "images/pluma.png";

		changeTexture(fileName);

		emissionRateOverTime = 0.0f;

		color = { 255, 0, 0 };

		randomLifeTimeBetweenTwoValues = true;
		lifeTimeFirstValue = 1.0f;
		lifeTimeSecondValue = 3.0f;

		randomAngleBetweenTwoValues = true;
		angleFirstValue = 0.0f;
		angleSecondValue = 360.0f;

		randomSpeedBetweenTwoValues = true;
		speedFirstValue = 30.0f;
		speedSecondValue = 100.0f;

		randomSizeBetweenTwoValues = true;
		sizeFirstValue = { 30.0f, 30.0f };
		sizeSecondValue = { 50.0f, 50.0f };

		randomRotationBetweenTwoValues = true;
		rotationFirstValue = 0.0f;
		rotationSecondValue = 360.0f;

		randomForceBetweenTwoValues = false;
		forceFirstValue = 50.0f;
		forceSecondValue = 100.0f;

		randomAngularVelocityBetweenTwoValues = true;
		angularVelocityFirstValue = 5.0f;
		angularVelocitySecondValue = 10.0f;

		randomBouncinessBetweenTwoValues = true;
		bouncinessFirstValue = 0.0f;
		bouncinessSecondValue = 1.0f;

		randomImpulseBetweenTwoValues = true;
		impulseFirstValue = 5.0f;
		impulseSecondValue = 10.0f;

		overLifeTimeSpeed = true;
		endSpeed = 0.0f;

		overLifeTimeSize = false;
		endSize = { 0.0f, 0.0f };

		overLifeTimeAlpha = true;
		endAlpha = 0;

		addBurst(2, 50, 5, 4, 1); */

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

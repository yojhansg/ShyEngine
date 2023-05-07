#pragma once

#include "Component.h"
#include "Color.h"
#include "Vector2D.h"
#include <vector>
#include <stack>
#include <list>

namespace Renderer {
    class Texture;
}

namespace Physics {
    class PhysicsManager;
}

class b2World;

namespace ECS {

    class Transform;
    class Particle;
    class ParticleSystem;

    class Burst {

    public:

        Burst(ParticleSystem* system, float time, int particles, int cycles, float rate, float probability);

        void reset();

        // Setters & getters
        float getCurrentTime();
        void setBurstTimer(float time);

        float getIntervalTime();
        void setIntervalTimer(float time);

        float getRate();
        int getNumberOfParticles();

        int getCycles();
        void decreaseCycles();

        float getProbabilityToBurst();
        float getTimeBetweenBursts();

    private:

        // Delay time to generate the burst
        float timeToBurst;

        // Time between bursts
        float rate;

        // Particles to be generated
        int nParticles;

        // Times to generate the burst
        int cycles;

        // Probability to burst
        float probability;

        ParticleSystem* system;

        float burstTimer;
        float intervalTimer;
        int currentCycles;

    };

    class ParticleSystem : public Component {

        friend class Particle;
        friend class Burst;

    public:

        ParticleSystem();

        ~ParticleSystem();
        
        void init() override;

        void start() override;

        void update(float deltaTime) override;

        void fixedUpdate(float fixedDeltaTime) override;

        void render() override;

    publish:

        void startEmitting();

        bool isEmitting();

        void changeTexture(cstring texturePath);

        void addBurst(float time, int particles, int cycles, float rate, float probability);

    private:

        // General system configuration
        reflect float duration;
        reflect int maxParticles;
        reflect bool loop;
        reflect float delay; 
        reflect bool playOnStart; 


        // Particles START configuration

            // Lifetime
            reflect float lifeTime;
            reflect bool randomLifeTimeBetweenTwoValues;
            reflect float lifeTimeFirstValue;
            reflect float lifeTimeSecondValue;

            // Speed
            reflect float speed;
            reflect bool randomSpeedBetweenTwoValues;
            reflect float speedFirstValue;
            reflect float speedSecondValue;

            // Size
            reflect Utilities::Vector2D size;
            reflect bool randomSizeBetweenTwoValues;
            reflect Utilities::Vector2D sizeFirstValue;
            reflect Utilities::Vector2D sizeSecondValue;

            // Angle
            reflect float angle;
            reflect bool randomAngleBetweenTwoValues;
            reflect float angleFirstValue;
            reflect float angleSecondValue;
        
            // Rotation
            reflect float rotation;
            reflect bool randomRotationBetweenTwoValues;
            reflect float rotationFirstValue;
            reflect float rotationSecondValue;

            // Color
            reflect Utilities::Color color;
            reflect bool randomColorBetweenTwoColors;
            reflect Utilities::Color colorFirstValue;
            reflect Utilities::Color colorSecondValue;


        // Physics particles configuration
        reflect std::string layerName;
        reflect bool sensor;
        reflect float gravityScale;

            // Force
            reflect float force;
            reflect bool randomForceBetweenTwoValues;
            reflect float forceFirstValue;
            reflect float forceSecondValue;

            // Impulse
            reflect float impulse;
            reflect bool randomImpulseBetweenTwoValues;
            reflect float impulseFirstValue;
            reflect float impulseSecondValue;

            // Angular velocity
            reflect float angularVelocity;
            reflect bool randomAngularVelocityBetweenTwoValues;
            reflect float angularVelocityFirstValue;
            reflect float angularVelocitySecondValue;

            // Bounciness
            reflect float bounciness;
            reflect bool randomBouncinessBetweenTwoValues;
            reflect float bouncinessFirstValue;
            reflect float bouncinessSecondValue;


        // Render configuration
        reflect std::string fileName;
        reflect int flipmode;
        reflect int srcX;
        reflect int srcY;
        reflect int srcWidth;
        reflect int srcHeight;

            // Alpha
            reflect int alpha;
            reflect bool randomAlphaBetweenTwoValues;
            reflect int alphaFirstValue;
            reflect int alphaSecondValue;

        // Emission coniguration
        reflect float emissionRateOverTime;
        reflect float emissionRateOverDistance;

        // End lifetime configuration
        reflect bool overLifeTimeSpeed;
        reflect float endSpeed;

        reflect bool overLifeTimeColor;
        reflect Utilities::Color endColor;

        reflect bool overLifeTimeSize;
        reflect Utilities::Vector2D endSize;

        reflect bool overLifeTimeAlpha;
        reflect int endAlpha;

        // Private methods
        void takeParticleFromPool();
        void returnParticleToPool(int idx);
        void handleUnusedParticles();
        void handleBursts(float deltaTime);
        void closeSystem();
        void reset();

        // Private attributes
        bool emitting;
        float currentTime;
        float timeSinceStart;
        float timer;
        int nParticles;

        Transform* tr;
        Physics::PhysicsManager* physicsManager;
        float screenToWorldFactor;

        Utilities::Vector2D prevPosition;
        Renderer::Texture* texture;

        // Bursts
        std::vector<Burst*> bursts;

        // Particles pool
        std::stack<int> particlesPool;
        std::vector<Particle*> particles;
        std::list<Particle*> unusedParticles;

        // Box2D
        b2World* world;


        //TODO ANIMATOR

        //TODO TRAILS

    };

}


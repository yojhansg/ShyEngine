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

namespace ECS {

    class Transform;
    class Particle;

    class Burst {

    public:

        Burst(float time, int particles, int cycles, float rate, float probability);

    private:

        // Delay time to generate the burst
        float timeToBurst;

        // Particles to be generated
        int nParticles;

        // Times to generate the burst
        int cycles;

        // Time between bursts
        float rate;

        // Probability to burst
        float probability;

    };

    class ParticleSystem : public Component {

        friend class Particle;

    public:

        ParticleSystem();

        ~ParticleSystem();
        
        void init() override;

        void start() override;

        void update(float deltaTime) override;

        void fixedUpdate(float deltaTime) override;

        void render() override;

    publish:

        void startEmitting();

        void changeTexture(cstring texturePath);

        void addBurst(float time, int particles, int cycles, float rate, float probability);

        void setEmissionRateOverTime(float rate);

    private:

        // General system configuration
        reflect float duration;
        reflect int maxParticles;
        reflect bool loop;
        reflect float delay; 
        reflect bool playOnStart; 

        // Particles START configuration
        reflect float startLifeTime;
        reflect float startSpeed;
        reflect Utilities::Vector2D startDirection;
        reflect Utilities::Vector2D startPosition;
        reflect Utilities::Vector2D startSize;
        reflect float startRotation;
        reflect Utilities::Color startColor;

        // Physics particles configuration
        reflect bool useGravity;
        reflect float gravityScale;

        // Render configuration
        reflect bool useImage;
        reflect std::string fileName;
        reflect int alpha;
        reflect int flipmode;
        reflect int renderOrder;
        reflect int shape;
        reflect int srcX;
        reflect int srcY;
        reflect int srcWidth;
        reflect int srcHeight;

        // Emission coniguration
        reflect float emissionRateOverTime;
        reflect float emissionRateOverDistance;
        reflect bool inheritPosition;
        reflect bool inheritRotation;
        reflect bool inheritVelocity;

        // Over lifetime configuration
        reflect float speedOverLifeTime;
        reflect Utilities::Color colorOverLifeTime;
        reflect Utilities::Vector2D sizeOverLifeTime;
        reflect float rotationOverLifeTime;

        // Private methods
        void takeParticleFromPool();
        void returnParticleToPool(int idx);
        void handleUnusedParticles();
        void closeSystem();
        void debugSystemInfo();

        // Private attributes
        bool emitting;
        float currentTime;
        float timeSinceStart;
        float timer;
        int nParticles;

        Transform* tr;
        Renderer::Texture* texture;

        std::vector<Burst*> bursts;

        std::stack<int> particlesPool;
        std::vector<Particle*> particles;
        std::list<Particle*> unusedParticles;



        //TODO ANIMATOR

        //TODO TRAILS

    };

}


#pragma once
#include "Collider.h"

namespace ECS {

    class CircleCollilder : public Collider {

    public:

        CircleCollilder();

        ~CircleCollilder();

        void init() override;

        void start() override;

        void update(float deltaTime) override;

        void setRadiusToShorterAxis(bool value);

        void setRadius(float radius);
        float getRadius();

        b2CircleShape* getShape();

    private:

        float calculateRadius();

        Vector2D size;
        
        float radius;

        bool shorterAxis;

        bool customRadius;

        // Box2d circleShape
        b2CircleShape* shape;

        // Just for comfort
        b2Vec2* framePosition;

        // Frame fixture settings
        b2FixtureDef* frameFixtureDef;

    };

}


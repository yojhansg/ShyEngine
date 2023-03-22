#pragma once

#include "PhysicBody.h"

namespace ECS {

    class BoxBody : public PhysicsBody {

    public:

        BoxBody();

        ~BoxBody();

        void init() override;

        Vector2D getSize();
        void setSize(float x, float y);

        void setMass(float mass) override;

        float getArea() override;

    private:

        void scaleShape() override;

        // Box2D polygonShape
        b2PolygonShape* shape;

    };

}


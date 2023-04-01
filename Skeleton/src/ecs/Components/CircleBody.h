#pragma once

#include "PhysicBody.h"

namespace ECS {

    class CircleBody : public PhysicBody {

    public:

        CircleBody();

        ~CircleBody();

        void init() override;

        void setRadiusToShorterAxis(bool value);

        void setRadius(float radius);
        float getRadius();

        void setMass(float mass) override;

        float getArea() override;

    private:

        void scaleShape() override;

        float calculateRadius();

        float radius;

        bool shorterAxis;

        // Box2d circleShape
        b2CircleShape* shape;

    };

}


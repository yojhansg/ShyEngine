#pragma once

#include "PhysicBody.h"

namespace ECS {

    class BoxBody : public PhysicBody {

    public:

        BoxBody();

        void init() override;

    publish:

        Vector2D getSize();
        void setSize(float x, float y);

        void setMass(float mass) override;

        float getArea() override;

    private:

        void scaleShape() override;

        // Box2D polygonShape
        b2PolygonShape* shape;

        reflect Vector2D size;

    };

}


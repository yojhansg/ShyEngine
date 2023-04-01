#pragma once

#include "PhysicBody.h"

namespace ECS {

    class EdgeBody : public PhysicBody {

    public:

        EdgeBody();

        void init() override;

        void setEdgeVertices(const Vector2D& v1, const Vector2D& v2);

        Vector2D getSize();

    private:

        void setMass(float mass) override {};

        float getArea() override { return 0; };

        void scaleShape() override;

        void setBodyType(BODY_TYPE t) {};

        // Box2d circleShape
        b2EdgeShape* shape;

        b2Vec2 vertex1;
        b2Vec2 vertex2;

    };

}


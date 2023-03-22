#pragma once

#include "PhysicBody.h"
#include <vector>

class b2ChainShape;

namespace ECS {

    class ChainBody : public PhysicsBody {

    public:

        ChainBody();

        ~ChainBody();

        void init() override;

        void setVertices(const std::vector<b2Vec2>& vertices);

    private:

        void createVerticesArray();

        void setMass(float mass) override {};

        float getArea() override { return 0; };

        void scaleShape() override;

        // Box2d chainShape
        b2ChainShape* shape;

        std::vector<b2Vec2> vertices;

    };

}


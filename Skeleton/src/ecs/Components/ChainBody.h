#pragma once

#include "PhysicBody.h"
#include <vector>

class b2ChainShape;

namespace ECS {

    class ChainBody : public PhysicBody {

    public:

        ChainBody();

        ~ChainBody();

        void init() override;

        Vector2D getSize();

        void setVertices(const std::vector<b2Vec2>& vertices);

    private:

        void setMass(float mass) override {};

        float getArea() override { return 0; };

        void scaleShape() override;

        // Box2d chainShape
        b2ChainShape* shape;

        std::vector<b2Vec2> m_vertices;

        b2Vec2* verts;

    };

}


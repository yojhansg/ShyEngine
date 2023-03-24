#pragma once

#include <map>
#include <string>
#include <vector>
#include "Scripting/Variable.h"



#define ECSfunc_Version 1.0
//Creation time : Fri Mar 24 02:12:49 2023

typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);

class FunctionManager{
public: 
	static void CreateFunctionMap(std::map<std::string, CallableFunction>& map);
};



Scripting::Variable Image_getTextureWidth(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_getTextureHeight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setSrcRect(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setFlipX(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setFlipY(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setRotaionPoint(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setTrigger(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_isTrigger(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setFriction(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_getFriction(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setBounciness(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_getBounciness(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_getPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_getScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_setPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_setScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_setRotation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_translate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_rotate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_scale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_debugDraw(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_enableDebugDraw(std::vector<Scripting::Variable>const& vec);

#pragma once
#include "pch.h"
#include "InputCommands.h"
#include "DisplayObject.h"
#include "Tools.h"

#include "SQLManager.h"

class ObjectManager
{
public:
	ObjectManager() {}
	~ObjectManager() {}

	// Spawn an object at a location & add to database
	static SceneObject Spawn(OBJECT_SPAWN spawn, DirectX::SimpleMath::Vector3 position,
		std::vector<SceneObject> & sceneGraph);
};


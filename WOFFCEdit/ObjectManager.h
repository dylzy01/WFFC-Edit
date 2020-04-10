#pragma once
#include "pch.h"
#include "Game.h"
#include "InputCommands.h"
#include "DisplayObject.h"
#include "Tools.h"

#include "SQLManager.h"
#include "MouseManager.h"

class ObjectManager
{
public:
	ObjectManager() {}
	~ObjectManager() {}

	// Spawn an object at a location & add to database
	static void Spawn(OBJECT_SPAWN spawn, DirectX::SimpleMath::Vector3 position,
		std::vector<SceneObject> & sceneGraph);

	// Remove an object from scene graph & database
	static void Remove(std::vector<int> & IDs, std::vector<SceneObject> & sceneGraph, int ID = -1);

	// Temporarily store details of all objects
	static void Store(std::vector<int> IDs);

	// Transform selected objects
	static void Transform(OBJECT_FUNCTION function, OBJECT_CONSTRAINT constraint,
		std::vector<int> IDs, std::vector<SceneObject> & sceneGraph);	
	
	// Setters
	static void SetGame(Game * game) { m_game = game; }
	static void SetInput(InputCommands * input) { m_input = input; }

private:
	// Scale selected objects
	static void Scale(OBJECT_CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject> & sceneGraph);

	// Rotate selected objects
	static void Rotate(OBJECT_CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject> & sceneGraph);

	// Translate selected objects
	static void Translate(OBJECT_CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject> & sceneGraph);

	// Get S,R,T...
	static DirectX::SimpleMath::Vector3 GetScale(int i);
	static DirectX::SimpleMath::Vector3 GetRotation(int i);
	static DirectX::SimpleMath::Vector3 GetTranslation(int i);

private:
	static Game * m_game;
	static InputCommands * m_input;
	static std::vector<int> m_selectedObjectIDs;
	static std::vector<DirectX::SimpleMath::Vector3> m_storedObjectScales, m_storedObjectTranslations, m_storedObjectRotations;
};


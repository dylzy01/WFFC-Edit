#pragma once
#include "pch.h"
#include "Game.h"
#include "InputCommands.h"
#include "DisplayObject.h"
#include "Tools.h"

#include "SQLManager.h"
#include "MouseManager.h"
#include "SceneManager.h"

class ObjectManager
{
public:
	ObjectManager() {}
	~ObjectManager() {}

	// Spawn an object at a location & add to database
	static bool SpawnObject(OBJECT_TYPE objectType, DirectX::SimpleMath::Vector3 position, 
		int lightType = NULL, XMFLOAT3 diffuse = { NULL, NULL, NULL }, float constA = NULL, float linA = NULL, float quadA = NULL);

	// Remove an object from scene graph & database
	static void Remove(std::vector<int> & IDs, int ID = -1);

	// Temporarily store details of all selected objects
	static void Store(std::vector<int> IDs);

	// Transform selected objects
	static void Transform(OBJECT_FUNCTION function, CONSTRAINT constraint,
		std::vector<int> IDs);

	// Copy details of selected objects & remove from database
	static void Cut(std::vector<int> & IDs);

	// Copy details of selected objects
	static void Copy(std::vector<int> IDs);

	// Create new objects from copied
	static void Paste();

	// Replace the type of an object
	static bool ReplaceType(int ID, OBJECT_TYPE objectType,
		int lightType = NULL, XMFLOAT3 diffuse = { NULL, NULL, NULL }, float constA = NULL, float linA = NULL, float quadA = NULL);

	// Replace an entire object
	static bool ReplaceObject(SceneObject newObject);
	
	// Setters
	static void SetGame(Game * game) { m_game = game; }
	static void SetInput(InputCommands * input) { m_input = input; }

private:
	// Scale selected objects
	static void Scale(CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject> & sceneGraph);

	// Rotate selected objects
	static void Rotate(CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject> & sceneGraph);

	// Translate selected objects
	static void Translate(CONSTRAINT constraint, std::vector<int> IDs, std::vector<SceneObject> & sceneGraph);

	// Get S,R,T...
	static DirectX::SimpleMath::Vector3 GetScale(int i);
	static DirectX::SimpleMath::Vector3 GetRotation(int i);
	static DirectX::SimpleMath::Vector3 GetTranslation(int i);

	// Check scene graph for available IDs
	static std::vector<int> GetAvailableIDs(std::vector<int> IDs, std::vector<SceneObject> & sceneGraph);

	// Get a replacement object
	static SceneObject GetReplacement(SceneObject object);

private:
	static Game * m_game;
	static InputCommands * m_input;
	static std::vector<int> m_selectedObjectIDs;
	static std::vector<SceneObject> m_objectsToCopy;
	static std::vector<std::pair<DirectX::SimpleMath::Vector3, int>> m_storedObjectScales, m_storedObjectTranslations, m_storedObjectRotations;	
};


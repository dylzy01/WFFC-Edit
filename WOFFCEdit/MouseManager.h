#pragma once
#include "pch.h"
#include "Game.h"
#include "InputCommands.h"
#include "DisplayObject.h"

#define MAX_DIST 10000

class MouseManager
{
public:
	MouseManager() {}
	~MouseManager() {}

	// Picking a single object/light
	static int PickObject(PICK_TYPE type);

	// Picking multiple objects
	static void PickMultipleObjects(std::vector<int> & selectedIDs, PICK_TYPE type, bool select = true);

	// Picking a spawn point for an object
	static DirectX::SimpleMath::Vector3 PickSpawn();

	// Picking a piece of terrain
	static TERRAIN PickTerrain();

	// Getters
	static int GetIndex() { return m_index; }
	static bool GetMouseDrag() {
		return (m_input->mouseRight &&
			m_input->mousePosPrevious != m_input->mousePos);
	}

	// Setters
	static void SetGame(Game * game) { m_game = game; }
	static void SetInput(InputCommands * input) { m_input = input; }

	// Setup ray trace from given position
	static DirectX::SimpleMath::Ray RayTrace(DirectX::SimpleMath::Vector2 position);

private:
	// Check if ray trace intersects an object or a light
	static int AnyIntersection(DirectX::SimpleMath::Ray ray);
	static int ObjectIntersection(DirectX::SimpleMath::Ray ray);
	static int LightIntersection(DirectX::SimpleMath::Ray ray);

	// Check if ray trace intersects a piece of terrain
	static TERRAIN TerrainIntersection(DirectX::SimpleMath::Ray ray);

private:
	static Game * m_game;
	static InputCommands * m_input;
	static int m_index;
};


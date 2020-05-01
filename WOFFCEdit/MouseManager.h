#pragma once
#include "pch.h"
#include "Game.h"
#include "InputCommands.h"
#include "DisplayObject.h"


class MouseManager
{
public:
	MouseManager() {}
	~MouseManager() {}

	// Picking a single object
	static int PickObject();

	// Picking multiple objects
	static void PickMultipleObjects(std::vector<int> & selectedIDs, bool select = true);

	// Picking a spawn point for an object
	static DirectX::SimpleMath::Vector3 PickSpawn();

	// Get spawn point in middle of screen
	static DirectX::SimpleMath::Vector3 GetBasicSpawn();

	// Picking a piece of terrain
	static TERRAIN PickTerrain();

	// Setters
	static void SetGame(Game * game) { m_game = game; }
	static void SetInput(InputCommands * input) { m_input = input; }

	// Setup ray trace from given position
	static DirectX::SimpleMath::Ray RayTrace(DirectX::SimpleMath::Vector2 position);

private:
	// Check if ray trace intersects an object
	static int ObjectIntersection(DirectX::SimpleMath::Ray ray);

	// Check if ray trace intersects a piece of terrain
	static TERRAIN TerrainIntersection(DirectX::SimpleMath::Ray ray);

private:
	static Game * m_game;
	static InputCommands * m_input;
};


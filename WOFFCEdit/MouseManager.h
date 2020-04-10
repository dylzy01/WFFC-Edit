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

	// Picking a spawn point for an object
	static DirectX::SimpleMath::Vector3 PickSpawn();

	// Picking a piece of terrain
	static TERRAIN PickTerrain();

	// Setters
	static void SetGame(Game * game) { m_game = game; }
	static void SetInput(InputCommands * input) { m_input = input; }

	// Setup ray trace from given position
	static DirectX::SimpleMath::Ray RayTrace(DirectX::SimpleMath::Vector2 position);

private:
	// Check if ray trace intersects a piece of terrain
	static TERRAIN TerrainIntersection(DirectX::SimpleMath::Ray ray);

private:
	static Game * m_game;
	static InputCommands * m_input;
};


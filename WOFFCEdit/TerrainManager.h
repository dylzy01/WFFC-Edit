#pragma once
#include "pch.h"
#include "InputCommands.h"
#include "DisplayChunk.h"
#include "Tools.h"

#include "SQLManager.h"
#include "MouseManager.h"

class TerrainManager
{
public:
	TerrainManager() {}
	~TerrainManager() {}

	// Paint selected terrain
	static void Paint(TERRAIN_PAINT paint);

	// Sculpt selected terrain by scale factor
	static bool Sculpt(TERRAIN_SCULPT function, CONSTRAINT constraint, int row = -1, int column = -1);

	// Sculpt selected terrain by mouse drag
	static void SculptDrag(TERRAIN & terrain);/// , CONSTRAINT constraint);
	
	// Setters
	static void SetDisplayChunk(DisplayChunk * chunk) { m_displayChunk = chunk; }
	static void StorePosition(bool store) { m_store = store; }
	static void SetInput(InputCommands * input) { m_input = input; }

private:
	static std::vector<DirectX::XMFLOAT3> GetTranslation(TERRAIN terrain);

private:
	static DisplayChunk * m_displayChunk;
	static bool m_store;
	static InputCommands * m_input;
	static std::vector<DirectX::SimpleMath::Vector3> m_storedPositions;
};


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
	static void Paint(TERRAIN & terrain, TERRAIN_PAINT paint);

	// Sculpt selected terrain
	static void Sculpt(TERRAIN & terrain, TERRAIN_SCULPT function, CONSTRAINT constraint, bool single = false);
	
	// Setters
	static void SetDisplayChunk(DisplayChunk * chunk) { m_displayChunk = chunk; }
	static void StorePosition(bool store) { m_store = store; }

private:
	static DisplayChunk * m_displayChunk;
	static bool m_store;
	static std::vector<DirectX::SimpleMath::Vector3> m_storedPositions;
};


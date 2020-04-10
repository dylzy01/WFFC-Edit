#pragma once
#include "pch.h"
#include "InputCommands.h"
#include "DisplayChunk.h"
#include "Tools.h"

#include "SQLManager.h"
#include "MouseManager.h"

class LandscapeManager
{
public:
	LandscapeManager() {}
	~LandscapeManager() {}

	// Paint selected terrain
	static void Paint(TERRAIN & terrain, LANDSCAPE_PAINT paint, bool checkSurroundings = false);

	// Sculpt selected terrain
	static void Sculpt(TERRAIN & terrain, LANDSCAPE_FUNCTION function, LANDSCAPE_CONSTRAINT constraint);
	
	// Setters
	static void SetDisplayChunk(DisplayChunk * chunk) { m_displayChunk = chunk; }
	static void StorePosition(bool store) { m_store = store; }

private:
	static DisplayChunk * m_displayChunk;
	static bool m_store;
	static std::vector<DirectX::SimpleMath::Vector3> m_storedPositions;
};


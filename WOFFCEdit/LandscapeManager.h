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
	static void Paint(int row, int column, LANDSCAPE_PAINT paint, bool checkSurroundings = false);

	// Setters
	static void SetDisplayChunk(DisplayChunk * chunk) { m_displayChunk = chunk; }

private:
	static DisplayChunk * m_displayChunk;
};


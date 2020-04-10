#include "LandscapeManager.h"

DisplayChunk * LandscapeManager::m_displayChunk;

// Paint selected terrain
void LandscapeManager::Paint(int row, int column, LANDSCAPE_PAINT paint, bool checkSurroundings)
{
	// Check for duplicates
	m_displayChunk->PaintTerrain(row, column, paint, checkSurroundings);
}
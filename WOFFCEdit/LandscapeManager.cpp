#include "LandscapeManager.h"

DisplayChunk * LandscapeManager::m_displayChunk;
bool LandscapeManager::m_store;
std::vector<DirectX::SimpleMath::Vector3> LandscapeManager::m_storedPositions;

// Paint selected terrain
void LandscapeManager::Paint(TERRAIN & terrain, LANDSCAPE_PAINT paint, bool checkSurroundings)
{
	// Pick terrain
	terrain = MouseManager::PickTerrain();
	
	// If terrain is intersected
	if (terrain.intersect)
	{
		// Apply selected paint
		terrain.paint = paint;
		
		// Store selected terrain (row, column)
		DirectX::SimpleMath::Vector2 index;
		index.x = terrain.row;
		index.y = terrain.column;

		// Paint terrain the selected texture
		m_displayChunk->PaintTerrain(index.x, index.y, paint, checkSurroundings);
	}
}

// Sculpt selected terrain
void LandscapeManager::Sculpt(TERRAIN & terrain, LANDSCAPE_FUNCTION function, LANDSCAPE_CONSTRAINT constraint)
{
	// Pick terrain
	terrain = MouseManager::PickTerrain();

	// If terrain is intersected
	if (terrain.intersect)
	{
		// If function is increase or decrease
		if (function == LANDSCAPE_FUNCTION::INCREASE || function == LANDSCAPE_FUNCTION::DECREASE)
		{
			// Increase or decrease terrain
			m_displayChunk->SculptTerrain(terrain.row, terrain.column, function, constraint);
		}

		// Else, if function is flatten
		else if (function == LANDSCAPE_FUNCTION::FLATTEN)
		{
			// If first point should be stored
			if (m_store)
			{
				m_store = false;
				m_storedPositions.clear();
				m_storedPositions.push_back(m_displayChunk->GetGeometry(terrain.row, terrain.column).position);
				m_storedPositions.push_back(m_displayChunk->GetGeometry(terrain.row, terrain.column + 1).position);
				m_storedPositions.push_back(m_displayChunk->GetGeometry(terrain.row + 1, terrain.column + 1).position);
				m_storedPositions.push_back(m_displayChunk->GetGeometry(terrain.row + 1, terrain.column).position);
			}

			// Flatten height of selected terrain
			m_displayChunk->SculptTerrain(terrain.row, terrain.column, function, constraint, m_storedPositions);
		}
	}
}
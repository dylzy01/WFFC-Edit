#include "TerrainManager.h"

DisplayChunk * TerrainManager::m_displayChunk;
bool TerrainManager::m_store;
std::vector<DirectX::SimpleMath::Vector3> TerrainManager::m_storedPositions;

// Paint selected terrain
void TerrainManager::Paint(TERRAIN & terrain, TERRAIN_PAINT paint)
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
		m_displayChunk->PaintTerrain(index.x, index.y, paint);
		
		// If should blend
		//if (m_displayChunk->GetBlend())
		//{
		//	// Paint terrain the selected texture
		//	m_displayChunk->PaintBlend(index.x, index.y, paint);
		//}

		//// Else, if should overwrite
		//else
		//{
		//	// Paint terrain the selected texture
		//	m_displayChunk->PaintOpaque(index.x, index.y, paint);			
		//}	
	}
}

// Sculpt selected terrain
void TerrainManager::Sculpt(TERRAIN & terrain, TERRAIN_SCULPT function, CONSTRAINT constraint, bool single)
{
	// Pick terrain
	if (!single) { terrain = MouseManager::PickTerrain(); }	

	// If terrain is intersected
	if (terrain.intersect)
	{
		// If function is increase or decrease
		if (function == TERRAIN_SCULPT::INCREASE || function == TERRAIN_SCULPT::DECREASE)
		{
			// Increase or decrease terrain
			m_displayChunk->SculptTerrain(terrain.row, terrain.column, function, constraint);
		}

		// Else, if function is flatten
		else if (function == TERRAIN_SCULPT::FLATTEN)
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
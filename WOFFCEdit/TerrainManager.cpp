#include "TerrainManager.h"

DisplayChunk * TerrainManager::m_displayChunk;
bool TerrainManager::m_store;
InputCommands * TerrainManager::m_input;
std::vector<DirectX::SimpleMath::Vector3> TerrainManager::m_storedPositions;

// Paint selected terrain
void TerrainManager::Paint(TERRAIN_PAINT paint)
{
	// Pick terrain
	TERRAIN terrain = MouseManager::PickTerrain();
	
	// If terrain is intersected
	if (terrain.intersect)
	{
		// Paint terrain the selected texture
		m_displayChunk->PaintTerrain(terrain.row, terrain.column, paint);
	}
}

// Sculpt selected terrain via scale factor
bool TerrainManager::Sculpt(TERRAIN_SCULPT sculpt, CONSTRAINT constraint, int row, int col)
{
	// If terrain isn't specified
	if (row == -1 && col == -1)
	{
		// Pick terrain
		TERRAIN terrain = MouseManager::PickTerrain();

		// Exit function if terrain is not intersected
		if (!terrain.intersect) { return false; }

		// If terrain is intersected
		if (terrain.intersect)
		{
			// Specify terrain
			row = terrain.row;
			col = terrain.column;
		}

		// Else, exit function if not intersected
		else { return false; }
	}
	
	// If function is increase or decrease
	if (sculpt == TERRAIN_SCULPT::INCREASE || sculpt == TERRAIN_SCULPT::DECREASE)
	{
		// Increase or decrease terrain
		m_displayChunk->SculptTerrain(row, col, sculpt, constraint);
	}

	// Else, if function is flatten
	else if (sculpt == TERRAIN_SCULPT::FLATTEN)
	{
		// If first point should be stored
		if (m_store)
		{
			m_store = false;
			m_storedPositions.clear();
			m_storedPositions.push_back(m_displayChunk->GetGeometry(row, col).position);
			m_storedPositions.push_back(m_displayChunk->GetGeometry(row, col + 1).position);
			m_storedPositions.push_back(m_displayChunk->GetGeometry(row + 1, col + 1).position);
			m_storedPositions.push_back(m_displayChunk->GetGeometry(row + 1, col).position);
		}

		// Flatten height of selected terrain
		m_displayChunk->SculptTerrain(row, col, sculpt, constraint, m_storedPositions);
	}
	
	return true;
}

// Sculpt selected terrain via mouse drag
void TerrainManager::SculptDrag(TERRAIN & terrain)
{
	// Pick terrain
	///TERRAIN terrain = MouseManager::PickTerrain();

	// If terrain is intersected
	if (terrain.intersect)
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

		// Get translations from mouse position
		std::vector<DirectX::XMFLOAT3> translations = GetTranslation(terrain);

		m_displayChunk->SetPosition(terrain.row, terrain.column, translations[0].y);

		m_displayChunk->SetPosition(terrain.row, terrain.column + 1, translations[1].y);

		m_displayChunk->SetPosition(terrain.row + 1, terrain.column + 1, translations[2].y);

		m_displayChunk->SetPosition(terrain.row + 1, terrain.column, translations[3].y);
		
		// Switch between constraint
		/*switch (constraint)
		{
		case CONSTRAINT::X:
		{			
		}
		break;
		case CONSTRAINT::Y:
		{
		}
		break;
		case CONSTRAINT::Z:
		{
		}
		break;
		case CONSTRAINT::XY:
		{
		}
		break;
		case CONSTRAINT::XZ:
		{
		}
		break;
		case CONSTRAINT::YZ:
		{
		}
		break;
		case CONSTRAINT::NA:
		{
		}
		break;
		}*/
	}
}

std::vector<DirectX::XMFLOAT3> TerrainManager::GetTranslation(TERRAIN terrain)
{
	// Temp storage 
	DirectX::SimpleMath::Vector2 previous = m_input->mousePosPrevious;
	DirectX::SimpleMath::Vector2 current = m_input->mousePos;
	std::vector<DirectX::XMFLOAT3> objects;

	// Setup previous & current ray traces
	DirectX::SimpleMath::Ray pre = MouseManager::RayTrace(previous);
	DirectX::SimpleMath::Ray cur = MouseManager::RayTrace(current);

	// Loop through stored positions
	for (int i = 0; i < m_storedPositions.size(); ++i)
	{
		// Distance between previous & object
		float distP = sqrt(
			(pre.position.x - m_storedPositions[i].x) * (pre.position.x - m_storedPositions[i].x) +
			(pre.position.y - m_storedPositions[i].y) * (pre.position.y - m_storedPositions[i].y) +
			(pre.position.z - m_storedPositions[i].z) * (pre.position.z - m_storedPositions[i].z));

		// Distance between current & object
		float distC = sqrt(
			(cur.position.x - m_storedPositions[i].x) * (cur.position.x - m_storedPositions[i].x) +
			(cur.position.y - m_storedPositions[i].y) * (cur.position.y - m_storedPositions[i].y) +
			(cur.position.z - m_storedPositions[i].z) * (cur.position.z - m_storedPositions[i].z));

		// Setup temp picking points
		DirectX::SimpleMath::Vector3 pickingP = pre.position + (pre.direction * distP);
		DirectX::SimpleMath::Vector3 pickingC = cur.position + (cur.direction * distC);

		// Setup temp translation
		DirectX::XMFLOAT3 translation;

		// Define manipulated translation
		translation.x = m_storedPositions[i].x + (pickingC.x - pickingP.x);
		translation.y = m_storedPositions[i].y + (pickingC.y - pickingP.y);
		translation.z = m_storedPositions[i].z + (pickingC.z - pickingP.z);

		// Add to storage
		objects.push_back(translation);
	}

	// Return manipulated translations
	return objects;
}

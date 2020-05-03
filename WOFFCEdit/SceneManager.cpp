#include "SceneManager.h"

Game * SceneManager::m_game;
std::vector<SceneObject> * SceneManager::m_sceneGraph;
std::vector<std::vector<SceneObject>> SceneManager::m_objectHistory;
int SceneManager::m_objectHistoryID = 0;
int SceneManager::m_geometryHistoryID = 0;
int SceneManager::m_geometryFileCount = 0;
int SceneManager::m_count = 0;
bool SceneManager::m_max = false;
DisplayChunk * SceneManager::m_displayChunk;
std::vector<bool> SceneManager::m_history;

// Save current world state (scene graph + chunk)
void SceneManager::Save()
{
	// Update scene graph
	m_game->SaveDisplayList();

	// Save terrain
	m_game->SaveDisplayChunk();

	// Database query to delete all records from objects table
	SQLManager::SendQuery("DELETE FROM Objects", true);
	SQLManager::SetObjectStep();

	// Save new scene graph
	if (SQLManager::SaveObjects(m_game->GetSceneGraph())) { MessageBox(NULL, L"World Saved", L"Notification", MB_OK); }
	else { MessageBox(NULL, L"World Failed to Save", L"Notification", MB_OK); }
}

void SceneManager::QuickSave()
{
	// Update scene graph
	m_game->SaveDisplayList();

	// Save terrain
	m_game->SaveDisplayChunk();

	// Database query to delete all records from objects table
	SQLManager::SendQuery("DELETE FROM Objects", true);
	SQLManager::SetObjectStep();

	SQLManager::SaveObjects(m_game->GetSceneGraph());
}

// Undo to previous world state
std::pair<std::vector<SceneObject>, bool> SceneManager::UndoObjects()
{
	// Declare temp pair
	std::pair<std::vector<SceneObject>, bool> temp;
	temp.first = std::vector<SceneObject>();
	temp.second = false;
	
	// If current history index is an object
	if (m_history[m_count - 1])
	{
		// If storage is valid
		if (m_objectHistory.size() > 1)
		{
			// Reduce current ID if possible
			if (m_objectHistoryID != 1) { m_objectHistoryID--; }

			// Update local scene graph
			m_sceneGraph = &m_objectHistory[m_objectHistoryID - 1];

			// Setup temp pair
			temp.first = *m_sceneGraph;
			temp.second = true;
		}	
	}

	return temp;
}

bool SceneManager::UndoGeometry()
{
	// If current history index is geometry
	if (!m_history[m_count - 1])
	{
		// If storage isn't empty
		if (m_geometryFileCount > 1)
		{
			// Reduce current ID if possible
			if (m_geometryHistoryID != 1) { m_geometryHistoryID--; }

			// Replace geometry
			m_displayChunk->ReadGeometryPositions(m_geometryHistoryID - 1);

			return true;
		}
	}

	return false;
}

// Redo to proceeding world state
std::pair<std::vector<SceneObject>, bool> SceneManager::RedoObjects()
{
	// Declare temp pair
	std::pair<std::vector<SceneObject>, bool> temp;
	temp.first = std::vector<SceneObject>();
	temp.second = false;
	
	// If current history index is an object
	if (m_history[m_count - 1])
	{
		// If storage is valid
		if (m_objectHistory.size() > 1)
		{
			// Increase current ID if possible
			if (m_objectHistoryID != m_objectHistory.size() - 1) { m_objectHistoryID++; }

			// Update local scene graph
			m_sceneGraph = &m_objectHistory[m_objectHistoryID - 1];

			// Setup temp pair
			temp.first = *m_sceneGraph;
			temp.second = true;
		}
	}

	return temp;
}

bool SceneManager::RedoGeometry()
{
	// If current history index is geometry
	if (!m_history[m_count - 1])
	{
		// If storage isn't empty
		if (m_geometryFileCount > 1)
		{
			// Increase current ID if possible
			if (m_geometryHistoryID != m_geometryFileCount - 1) { m_geometryHistoryID++; }

			// Replace geometry
			m_displayChunk->ReadGeometryPositions(m_geometryHistoryID);

			return true;
		}
	}

	return false;
}

#include "SceneManager.h"

Game * SceneManager::m_game;
std::vector<SceneObject> * SceneManager::m_sceneGraph;
std::vector<std::vector<SceneObject>> SceneManager::m_sceneGraphs;
int SceneManager::m_currentID = 0;
DisplayChunk * SceneManager::m_displayChunk;

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
std::pair<std::vector<SceneObject>, bool> SceneManager::Undo()
{
	// Declare temp pair
	std::pair<std::vector<SceneObject>, bool> temp;
	
	// If storage isn't empty
	if (m_sceneGraphs.size() != 0)
	{
		// Reduce current ID if possible
		if (m_currentID != 1) { m_currentID--; }

		// Update local scene graph
		m_sceneGraph = &m_sceneGraphs[m_currentID - 1];		

		// Return local scene graph
		///return *m_sceneGraph;

		// Setup temp pair
		temp.first = *m_sceneGraph;
		temp.second = true;
	}

	// Else, if storage is empty
	else
	{
		temp.first = std::vector<SceneObject>();
		temp.second = false;
	}

	return temp;
}

// Redo to proceeding world state
std::pair<std::vector<SceneObject>, bool> SceneManager::Redo()
{
	// Declare temp pair
	std::pair<std::vector<SceneObject>, bool> temp;
	
	// If storage isn't empty
	if (m_sceneGraphs.size() != 0)
	{
		// Increase current ID if possible
		if (m_currentID != m_sceneGraphs.size() - 1) { m_currentID++; }

		// Update local scene graph
		m_sceneGraph = &m_sceneGraphs[m_currentID - 1];

		// Setup temp pair
		temp.first = *m_sceneGraph;
		temp.second = true;
	}

	// Else, if storage is empty
	else
	{
		temp.first = std::vector<SceneObject>();
		temp.second = false;
	}

	return temp;

	// Return local scene graph
	///return *m_sceneGraph;
}

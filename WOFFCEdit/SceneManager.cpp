#include "SceneManager.h"

Game * SceneManager::m_game;
std::vector<SceneObject> * SceneManager::m_sceneGraph;
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

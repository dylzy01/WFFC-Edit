#include "SceneManager.h"

Game * SceneManager::m_game;
DisplayChunk * SceneManager::m_displayChunk;
std::vector<SceneObject> * SceneManager::m_sceneGraph;
std::vector<std::vector<SceneObject>> SceneManager::m_objectHistory;
std::vector<int> SceneManager::m_history;
int SceneManager::m_count = 0;
int SceneManager::m_timer = 0;
bool SceneManager::m_autosave = false;

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
bool SceneManager::Undo()
{
	// If history storage is valid
	if (m_history.size() > 0)
	{	
		// If count is valid
		if (m_count != m_history.size() + 1)
		{
			// Decrease count if possible
			if (DecreaseCount())
			{
				// Update scene graph
				m_game->BuildDisplayList(&m_objectHistory[m_count - 1]);

				// Update geometry positions
				m_displayChunk->ReadGeometryPositions(m_count - 1);

				return true;
			}
		}
	}

	return false;
}

// Redo to proceeding world state
bool SceneManager::Redo()
{
	// If history storage is valid
	if (m_history.size() > 0)
	{		
		// If count is valid
		if (m_count != m_history.size() + 1)
		{
			// Increase count if possible
			if (IncreaseCount())
			{
				// Update scene graph
				m_game->BuildDisplayList(&m_objectHistory[m_count - 1]);

				// Update geometry positions
				m_displayChunk->ReadGeometryPositions(m_count - 1);

				return true;
			}
		}
	}

	return false;
}

// Handle autosave timer
void SceneManager::Autosave()
{
	// Display autosave on/off
	m_game->AutosaveOnOff(m_autosave);
	
	// If autosave is on
	if (m_autosave)
	{
		// Increase timer
		m_timer++;

		// If counter reaches target
		if (m_timer >= 300)
		{
			// Trigger save
			QuickSave();

			// Reset timer
			m_timer = 0;
			m_game->DisplayAutosaveTime(false, 0);
		}

		// Else, if counter reaches 10 seconds left
		else if (m_timer >= 200)
		{
			// Calculate time remaining
			int time = (300 - m_timer) / 10;

			// Display autosave counter
			m_game->DisplayAutosaveTime(true, time);
		}
	}
}

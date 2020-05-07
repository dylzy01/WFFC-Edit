#pragma once
#include "pch.h"
#include "InputCommands.h"
#include "DisplayChunk.h"
#include "Tools.h"
#include "Game.h"

#include "SQLManager.h"

class SceneManager
{
public:
	SceneManager() {}
	~SceneManager() {}

	// Save current world state (scene graph + chunk)
	static void Save();

	// Save current world state (scene graph + chunk) via a timer
	static void QuickSave();

	// Save current world state (chunk) into a new chunks table row
	static void SaveAs(std::string name);

	// Load world state (chunk) from chunks table
	static ChunkObject LoadChunk(std::string name) { return SQLManager::LoadChunk(name); }

	// Load a different height map
	static ChunkObject LoadHeightMap(ChunkObject chunk, std::string path) { return SQLManager::LoadHeightMap(chunk, path); }

	// Undo to previous world state
	static bool	Undo();

	// Redo to proceeding world state
	static bool Redo();

	// Handle autosave timer
	static void Autosave();

	// Getters
	static bool GetAutosave() { return m_autosave; }
	 
	// Setters
	static void SetGame(Game * game) { m_game = game; }
	static void SetScene(std::vector<SceneObject>* sceneGraph, DisplayChunk* chunk) {			
		// Update scene graph
		m_sceneGraph = sceneGraph;
		m_objectHistory.push_back(*sceneGraph);

		// Update display chunk
		m_displayChunk = chunk;
		m_displayChunk->SaveGeometryPositions(m_count);

		// Update history
		m_history.push_back(m_count);
		m_count++;
	}
	static void SetAutosave(bool on) { m_autosave = on; }

private:
	// Reduce current history count if possible
	static bool DecreaseCount() {
		if (m_count != 1) { m_count--; return true; }
		else { return false; }
	}

	// Increase current history count if possible
	static bool IncreaseCount() {
		if (m_count != m_history.size()) { m_count++; return true; }
		else { return false; }
	}

private:
	static Game * m_game;
	static DisplayChunk * m_displayChunk;
	static std::vector<SceneObject> * m_sceneGraph;
	static std::vector<std::vector<SceneObject>> m_objectHistory;	
	static std::vector<int> m_history;
	static int m_count;
	static int m_timer;
	static bool m_autosave;	
};


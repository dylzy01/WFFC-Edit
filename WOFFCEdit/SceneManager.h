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
	static void QuickSave();

	// Undo to previous world state
	static std::pair<std::vector<SceneObject>, bool>	UndoObjects();
	static bool											UndoGeometry();

	// Redo to proceeding world state
	static std::pair<std::vector<SceneObject>, bool>	RedoObjects();
	static bool											RedoGeometry();

	// Reduce current history count if possible
	static void ReduceCount() {
		if (m_count != 1) { m_count--; }
	}

	// Increase current history count if possible
	static void IncreaseCount() {
		if (m_count != m_history.size() - 1) { m_count++; m_max = false; }
		else { m_max = true; }
	}

	// Getters
	static bool IsMax() { return m_max; }
	 
	// Setters
	static void SetGame(Game * game) { m_game = game; }
	static void SetSceneGraph(std::vector<SceneObject> * sceneGraph, bool first = false) {
		m_sceneGraph = sceneGraph;
		m_objectHistory.push_back(*sceneGraph);
		m_objectHistoryID++;

		//if (!first) {
			m_history.push_back(true);
			m_count++;
		//}
	}
	static void SetDisplayChunk(DisplayChunk * chunk, bool first = false) {
		m_displayChunk = chunk;
		m_displayChunk->SaveGeometryPositions(m_geometryFileCount);
		m_geometryHistoryID++;
		m_geometryFileCount++;

		//if (!first) {
			m_history.push_back(false);
			m_count++;
		//}
	}

private:
	static Game * m_game;
	static std::vector<SceneObject> * m_sceneGraph;
	static std::vector<std::vector<SceneObject>> m_objectHistory;
	static int m_objectHistoryID;
	static int m_geometryHistoryID;
	static int m_geometryFileCount;
	static DisplayChunk * m_displayChunk;
	static int m_count;
	static std::vector<bool> m_history;
	static bool m_max;
};


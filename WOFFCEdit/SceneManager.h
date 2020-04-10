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

	// Undo to previous world state
	static std::vector<SceneObject> Undo();

	// Redo to proceeding world state
	static std::vector<SceneObject> Redo();
	 
	// Setters
	static void SetGame(Game * game) { m_game = game; }
	static void SetSceneGraph(std::vector<SceneObject> * sceneGraph) { 
		m_sceneGraph = sceneGraph;
		m_sceneGraphs.push_back(*sceneGraph);
		m_currentID++;
	}
	static void SetDisplayChunk(DisplayChunk * chunk) { m_displayChunk = chunk; }

private:
	static Game * m_game;
	static std::vector<SceneObject> * m_sceneGraph;
	static std::vector<std::vector<SceneObject>> m_sceneGraphs;
	static int m_currentID;
	static DisplayChunk * m_displayChunk;
};


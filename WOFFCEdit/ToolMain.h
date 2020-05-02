#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "InputCommands.h"
#include <vector>

#include "MouseManager.h"
#include "ObjectManager.h"
#include "LandscapeManager.h"
#include "SceneManager.h"
#include "Light.h"

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain() { SQLManager::Disconnect(); }

	//onAction - These are the interface to MFC
	std::vector<int>	getSelectedObjectIDs() { return m_selectedObjectIDs; }		//returns the selection numbers of currently selected objects so that It can be displayed.
	std::vector<TERRAIN>	getCurrentTerrainSelection() { return m_selectedTerrains; }		//returns the currently selected chunk so it can be displayed
	void				onActionInitialise(HWND handle, int width, int height);				//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void				onActionFocusCamera();
	void				onActionLoad();
	afx_msg void		onActionSave();
	afx_msg void		onActionSaveTerrain();												//save chunk geometry
	afx_msg void		onActionDeleteObjects();											//delete selected objects
	afx_msg void		onActionUndo();
	afx_msg void		onActionRedo();

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	// Helpers
	void ClearSelected() { m_selectedObjectIDs.clear(); m_d3dRenderer.SetSelectedObjectIDs(m_selectedObjectIDs); }

	// Getters
	EDITOR GetEditor() { return m_d3dRenderer.GetEditor(); } //return current editor
	std::vector<DisplayObject> GetLights() { return m_d3dRenderer.GetLights(); }
	std::vector<DisplayObject> GetDisplayList() { return m_d3dRenderer.GetDisplayList(); }
	DisplayChunk* GetDisplayChunk() { return m_d3dRenderer.GetDisplayChunk(); }
	std::vector<SceneObject> GetSceneGraph() { return m_d3dRenderer.GetSceneGraph(); }
	InputCommands* GetInput() { return &m_toolInputCommands; }

	// Setters
	void SetWireframe(bool wireframe) { m_d3dRenderer.SetWireframe(wireframe); }
	void SetEditor(EDITOR editor) {
		m_editor = editor;
		switch (editor)
		{
		case EDITOR::OBJECT_SPAWN:
		{
			// Reset object transform
			m_objectFunction = OBJECT_FUNCTION::NA;
			m_objectConstraint = CONSTRAINT::NA;

			// Reset landscape sculpt
			m_terrainFunction = TERRAIN_SCULPT::NA;
			m_terrainConstraint = CONSTRAINT::NA;

			// Reset landscape paint
			m_paint = TERRAIN_PAINT::NA;
		}
		break;
		case EDITOR::OBJECT_FUNCTION:
		{
			// Reset object spawn
			m_objectType = OBJECT_TYPE::NA;

			// Reset landscape sculpt
			m_terrainFunction = TERRAIN_SCULPT::NA;
			m_terrainConstraint = CONSTRAINT::NA;

			// Reset landscape paint
			m_paint = TERRAIN_PAINT::NA;
		}
		break;
		case EDITOR::TERRAIN_PAINT:
		{
			// Reset object spawn
			m_objectType = OBJECT_TYPE::NA;
			m_objectConstraint = CONSTRAINT::NA;

			// Reset object transform
			m_objectFunction = OBJECT_FUNCTION::NA;
			m_objectConstraint = CONSTRAINT::NA;

			// Reset landscape sculpt
			m_terrainFunction = TERRAIN_SCULPT::NA;
			m_terrainConstraint = CONSTRAINT::NA;
		}
		break;
		case EDITOR::SCULPT_FREELY:
		{
			// Reset object spawn
			m_objectType = OBJECT_TYPE::NA;
			m_objectConstraint = CONSTRAINT::NA;

			// Reset object transform
			m_objectFunction = OBJECT_FUNCTION::NA;
			m_objectConstraint = CONSTRAINT::NA;

			// Reset landscape paint
			m_paint = TERRAIN_PAINT::NA;
		}
		break;
		}
	}
	void SetObjectSpawn(OBJECT_TYPE spawn) { m_objectType = spawn; }
	void SetObjectFunction(OBJECT_FUNCTION function) { m_objectFunction = function; }
	void SetObjectConstraint(CONSTRAINT constraint) { m_objectConstraint = constraint; }
	void SetTerrainPaint(TERRAIN_PAINT paint) { m_paint = paint; }
	void SetTerrainSculpt(TERRAIN_SCULPT function) { m_terrainFunction = function; }
	void SetTerrainConstraint(CONSTRAINT constraint) { m_terrainConstraint = constraint; }
	void SetLights(std::vector<DisplayObject> lights) { m_d3dRenderer.SetLights(lights); }
	void SetSceneObject(SceneObject object, int index) { m_d3dRenderer.SetSceneObject(object, index); }
	void SetSelectedObjectID(int ID) 
	{ 
		m_selectedObjectIDs.clear(); 
		m_selectedObjectIDs.push_back(ID); 
		m_d3dRenderer.SetSelectedObjectIDs(m_selectedObjectIDs); 
	}
	void SetSelectedObjectIDs(std::vector<int> IDs) { m_selectedObjectIDs = IDs; m_d3dRenderer.SetSelectedObjectIDs(m_selectedObjectIDs); }
	///void SetLights(std::pair<std::vector<Light*>, std::vector<int>>& lights) { m_lights = lights; }
	void SetSelectedTerrain(TERRAIN& terrain) { m_selectedTerrain = terrain; }
	void SetFocus(int ID) { m_d3dRenderer.SetFocus(ID); }

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	std::vector<int> m_selectedObjectIDs;			//IDs of multiple current OBJECT selections
	TERRAIN m_selectedTerrain;				
	std::vector<TERRAIN> m_selectedTerrains;	//IDs of multiple current TERRAIN selections
	std::vector<int> m_objectIDsToCopy;
	///std::pair<std::vector<Light*>, std::vector<int>> m_lights;

	EDITOR m_editor;								//control which editor state is being applied
	OBJECT_TYPE m_objectType;						//control which object is to be spawned
	OBJECT_FUNCTION m_objectFunction;				//control which object function is being applied
	CONSTRAINT m_objectConstraint;			//control which constraint is applied to object functions
	TERRAIN_PAINT m_paint;				//control which paint should be applied to terrain
	TERRAIN_SCULPT m_terrainFunction;			//control which sculpt function should be applied to terrain
	CONSTRAINT m_terrainConstraint;		//control which constraint is applied to terrain functions

private:	//methods
	void	onContentAdded();

	void	SaveDisplayList(std::vector<DisplayObject> displayList);	
		
private:	//variables
	HWND	m_toolHandle;					//Handle to the  window
	Game	m_d3dRenderer;					//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;						//Window area rectangle. 
	char	m_keyArray[256];				//array of all key inputs
	sqlite3 *m_databaseConnection;			//sqldatabase handle

	int m_width;							//dimensions passed to directX
	int m_height;
	int m_currentChunk;						//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	
	// Mouse movement
	bool m_mouseR, m_mouseL;

	// Toggle controllers
	bool m_lDown = false, m_rDown = false;
};

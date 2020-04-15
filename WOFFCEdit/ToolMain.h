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

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain() { SQLManager::Disconnect(); }

	//onAction - These are the interface to MFC
	std::vector<int>	getCurrentObjectSelectionID() { return m_selectedObjectIDs; }			//returns the selection numbers of currently selected objects so that It can be displayed.
	std::vector<TERRAIN>	getCurrentTerrainSelection() { return m_selectedTerrains; }		//returns the currently selected chunk so it can be displayed
	void				onActionInitialise(HWND handle, int width, int height);				//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void				onActionFocusCamera();
	void				onActionLoad();
	void				onActionLoad_();													//load the current chunk
	afx_msg void		onActionSave();
	afx_msg	void		onActionSave_();													//save the current chunk
	afx_msg void		onActionSaveTerrain();												//save chunk geometry
	afx_msg void		onActionDeleteObjects();											//delete selected objects
	afx_msg void		onActionUndo();
	afx_msg void		onActionRedo();

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	// Getters
	EDITOR GetEditor() { return m_d3dRenderer.GetEditor(); } //return current editor

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
			m_objectConstraint = OBJECT_CONSTRAINT::NA;

			// Reset landscape sculpt
			m_landscapeFunction = LANDSCAPE_FUNCTION::NA;
			m_landscapeConstraint = LANDSCAPE_CONSTRAINT::NA;

			// Reset landscape paint
			m_landscapePaint = LANDSCAPE_PAINT::NA;
		}
		break;
		case EDITOR::OBJECT_FUNCTION:
		{
			// Reset object spawn
			m_objectSpawn = OBJECT_SPAWN::NA;

			// Reset landscape sculpt
			m_landscapeFunction = LANDSCAPE_FUNCTION::NA;
			m_landscapeConstraint = LANDSCAPE_CONSTRAINT::NA;

			// Reset landscape paint
			m_landscapePaint = LANDSCAPE_PAINT::NA;
		}
		break;
		case EDITOR::LANDSCAPE_PAINT:
		{
			// Reset object spawn
			m_objectSpawn = OBJECT_SPAWN::NA;
			m_objectConstraint = OBJECT_CONSTRAINT::NA;

			// Reset object transform
			m_objectFunction = OBJECT_FUNCTION::NA;
			m_objectConstraint = OBJECT_CONSTRAINT::NA;

			// Reset landscape sculpt
			m_landscapeFunction = LANDSCAPE_FUNCTION::NA;
			m_landscapeConstraint = LANDSCAPE_CONSTRAINT::NA;
		}
		break;
		case EDITOR::LANDSCAPE_FUNCTION:
		{
			// Reset object spawn
			m_objectSpawn = OBJECT_SPAWN::NA;
			m_objectConstraint = OBJECT_CONSTRAINT::NA;

			// Reset object transform
			m_objectFunction = OBJECT_FUNCTION::NA;
			m_objectConstraint = OBJECT_CONSTRAINT::NA;

			// Reset landscape paint
			m_landscapePaint = LANDSCAPE_PAINT::NA;
		}
		break;
		}
	}
	void SetObjectSpawn(OBJECT_SPAWN spawn) { m_objectSpawn = spawn; }
	void SetObjectFunction(OBJECT_FUNCTION function) { m_objectFunction = function; }
	void SetObjectConstraint(OBJECT_CONSTRAINT constraint) { m_objectConstraint = constraint; }
	void SetLandscapePaint(LANDSCAPE_PAINT paint) { m_landscapePaint = paint; }
	void SetLandscapeSculpt(LANDSCAPE_FUNCTION function) { m_landscapeFunction = function; }
	void SetLandscapeConstraint(LANDSCAPE_CONSTRAINT constraint) { m_landscapeConstraint = constraint; }

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	std::vector<int> m_selectedObjectIDs;			//IDs of multiple current OBJECT selections
	TERRAIN m_selectedTerrain;				
	std::vector<TERRAIN> m_selectedTerrains;	//IDs of multiple current TERRAIN selections

	EDITOR m_editor;								//control which editor state is being applied
	OBJECT_SPAWN m_objectSpawn;						//control which object is to be spawned
	OBJECT_FUNCTION m_objectFunction;				//control which object function is being applied
	OBJECT_CONSTRAINT m_objectConstraint;			//control which constraint is applied to object functions
	LANDSCAPE_PAINT m_landscapePaint;				//control which paint should be applied to terrain
	LANDSCAPE_FUNCTION m_landscapeFunction;			//control which sculpt function should be applied to terrain
	LANDSCAPE_CONSTRAINT m_landscapeConstraint;		//control which constraint is applied to terrain functions

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

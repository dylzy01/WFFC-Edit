#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include <vector>

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	std::vector<int>	getCurrentObjectSelectionID();								//returns the selection numbers of currently selected objects so that It can be displayed.
	std::vector<TERRAIN>	getCurrentChunkSelection();									//returns the currently selected chunk so it can be displayed
	void				onActionInitialise(HWND handle, int width, int height);		//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void				onActionFocusCamera();
	void				onActionLoad();												//load the current chunk
	afx_msg	void		onActionSave();												//save the current chunk
	afx_msg void		onActionSaveTerrain();										//save chunk geometry

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	// Getters
	EDITOR GetEditor() { return m_editor; } //return current editor

	// Setters
	void SetWireframe(bool wireframe) { m_d3dRenderer.SetWireframe(wireframe); }
	void SetEditor(EDITOR editor) {
		m_editor = editor;
		switch (editor)
		{
		case EDITOR::OBJECT:
		{
			m_sculptFunction = SCULPT_FUNCTION::NA;
			m_sculptConstraint = SCULPT_CONSTRAINT::NA;
		}
		break;
		case EDITOR::SCULPT:
		{
			m_objectFunction = OBJECT_FUNCTION::NA;
			m_objectConstraint = OBJECT_CONSTRAINT::NA;
		}
		break;
		}
		m_d3dRenderer.SetEditor(editor);
	}
	void SetObjectFunction(OBJECT_FUNCTION function) { m_objectFunction = function; m_d3dRenderer.SetObjectFunction(function); }
	void SetObjectConstraint(OBJECT_CONSTRAINT constraint) { m_objectConstraint = constraint; m_d3dRenderer.SetObjectConstraint(constraint); }
	void SetSculptFunction(SCULPT_FUNCTION function) { m_sculptFunction = function; m_d3dRenderer.SetSculptFunction(function); }
	void SetSculptConstraint(SCULPT_CONSTRAINT constraint) { m_sculptConstraint = constraint; m_d3dRenderer.SetSculptConstraint(constraint); }

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	std::vector<int> m_selectedObjects;			//IDs of multiple current OBJECT selections
	TERRAIN m_selectedTerrain;				
	std::vector<TERRAIN> m_selectedTerrains;		//IDs of multiple current TERRAIN selections

private:	//methods
	void	onContentAdded();
		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle
	
	// Mode controllers
	EDITOR m_editor;
	OBJECT_FUNCTION m_objectFunction = OBJECT_FUNCTION::NA;
	OBJECT_CONSTRAINT m_objectConstraint = OBJECT_CONSTRAINT::NA;
	SCULPT_FUNCTION m_sculptFunction = SCULPT_FUNCTION::NA;
	SCULPT_CONSTRAINT m_sculptConstraint = SCULPT_CONSTRAINT::NA;

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	
	// Mouse movement
	bool m_mouseR, m_mouseL;
};

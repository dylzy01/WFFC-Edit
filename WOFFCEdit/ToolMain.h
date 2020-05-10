#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "InputCommands.h"
#include <vector>

#include "MouseManager.h"
#include "ObjectManager.h"
#include "TerrainManager.h"
#include "SceneManager.h"

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain() { SQLManager::Disconnect(); }

	//onAction - These are the interface to MFC
	void				onActionInitialise(HWND handle, int width, int height);				//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void				onActionLoad();
	afx_msg void		onActionSave() { SceneManager::Save(); }
	afx_msg void		onActionSaveAs(std::string name) { SceneManager::SaveAs(name); }
	afx_msg void		onActionLoad(std::string path);
	afx_msg void		onActionDeleteObjects();											//delete selected objects
	afx_msg void		onActionUndo();
	afx_msg void		onActionRedo();

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	// Helpers
	void ClearSelected() { m_selectedIDs.clear(); m_d3dRenderer.SetSelectedObjectIDs(m_selectedIDs); }

	// Getters
	EDITOR_COMPLEX GetEditor() { return m_d3dRenderer.GetEditor(); } //return current editor
	OBJECT_TYPE GetObjectSpawn() { return m_objectType; }
	OBJECT_FUNCTION GetObjectFunction() { return m_objectFunction; }	
	CONSTRAINT GetConstraint() { return m_constraint; }
	std::vector<DisplayObject> GetDisplayList() { return m_d3dRenderer.GetDisplayList(); }
	DisplayChunk* GetDisplayChunk() { return m_d3dRenderer.GetDisplayChunk(); }
	std::vector<SceneObject> GetSceneGraph() { return m_d3dRenderer.GetSceneGraph(); }
	InputCommands* GetInput() { return &m_toolInputCommands; }
	bool GetNewSelection();
	std::vector<int> GetSelectedIDs() { return m_selectedIDs; }

	// Setters
	void SetWireframe(bool wireframe) { m_d3dRenderer.SetWireframe(wireframe); }
	void SetEditor(EDITOR_COMPLEX editor) {
		m_editor = editor;
		switch (editor)
		{
		case EDITOR_COMPLEX::SPAWN:
		{
			// Reset object transform
			m_objectFunction = OBJECT_FUNCTION::NA;
			m_constraint = CONSTRAINT::NA;

			// Reset constraint
			m_constraint = CONSTRAINT::NA;
		}
		break;
		case EDITOR_COMPLEX::OBJECTS:
		{
			// Reset object spawn
			m_objectType = OBJECT_TYPE::NA;

			// Reset constraint
			m_constraint = CONSTRAINT::NA;
		}
		break;
		case EDITOR_COMPLEX::LIGHTS:
		{
			// Reset object spawn
			m_objectType = OBJECT_TYPE::NA;

			// Reset constraint
			m_constraint = CONSTRAINT::NA;
		}
		break;
		}
	}
	void SetObjectSpawn(OBJECT_TYPE spawn) { m_objectType = spawn; }
	void SetObjectFunction(OBJECT_FUNCTION function) { m_objectFunction = function; }
	void SetConstraint(CONSTRAINT constraint) { m_constraint = constraint; }
	void SetSceneObject(SceneObject object, int index) { m_d3dRenderer.SetSceneObject(object, index); }
	void SetSelectedIDs(std::vector<int> IDs) { 
		m_selectedIDs = IDs; 
		m_d3dRenderer.SetSelectedObjectIDs(m_selectedIDs); 
	}
	void SetFocus(int ID) { m_d3dRenderer.SetFocus(ID); }

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	std::vector<int> m_selectedIDs;			//IDs of multiple current OBJECT selections
	int m_selectedIndex;

	EDITOR_COMPLEX m_editor;								//control which editor state is being applied
	OBJECT_TYPE m_objectType;						//control which object is to be spawned
	OBJECT_FUNCTION m_objectFunction;				//control which object function is being applied
	CONSTRAINT m_constraint;						//control which constraint is applied to all functions

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

	// Controllers
	bool m_lDown = false, m_rDown = false;
	bool m_doOnce = false;
	bool m_newSelection = false;
	bool m_request = false, m_mouseIsDragging = false;
	int m_counter = 0;
};

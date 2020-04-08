#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "SQL.h"
#include "InputCommands.h"
#include <vector>

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain() { SQL::Disconnect(); }

	//onAction - These are the interface to MFC
	std::vector<int>	getCurrentObjectSelectionID() { return m_selectedObjects; }			//returns the selection numbers of currently selected objects so that It can be displayed.
	std::vector<TERRAIN>	getCurrentTerrainSelection() { return m_selectedTerrains; }		//returns the currently selected chunk so it can be displayed
	void				onActionInitialise(HWND handle, int width, int height);				//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void				onActionFocusCamera();
	void				onActionLoad();
	void				onActionLoad_();													//load the current chunk
	afx_msg void		onActionSave();
	afx_msg	void		onActionSave_();													//save the current chunk
	afx_msg void		onActionSaveTerrain();												//save chunk geometry
	afx_msg void		onActionDeleteObjects();											//dekete selected objects

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);
	void	TryConnect();

	// Getters
	EDITOR GetEditor() { return m_d3dRenderer.GetEditor(); } //return current editor

	// Setters
	void SetWireframe(bool wireframe) { m_d3dRenderer.SetWireframe(wireframe); }
	void SetEditor(EDITOR editor) { m_d3dRenderer.SetEditor(editor); }
	void SetObjectSpawn(OBJECT_SPAWN spawn) { m_d3dRenderer.SetObjectSpawn(spawn); }
	void SetObjectTransform(OBJECT_FUNCTION function) { m_d3dRenderer.SetObjectTransform(function); }
	void SetObjectConstraint(OBJECT_CONSTRAINT constraint) { m_d3dRenderer.SetObjectConstraint(constraint); }
	void SetLandscapePaint(LANDSCAPE_PAINT paint) { m_d3dRenderer.SetLandscapePaint(paint); }
	void SetLandscapeSculpt(LANDSCAPE_SCULPT function) { m_d3dRenderer.SetLandscapeSculpt(function); }
	void SetLandscapeConstraint(LANDSCAPE_CONSTRAINT constraint) { m_d3dRenderer.SetLandscapeConstraint(constraint); }

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	std::vector<int> m_selectedObjects;			//IDs of multiple current OBJECT selections
	TERRAIN m_selectedTerrain;				
	std::vector<TERRAIN> m_selectedTerrains;	//IDs of multiple current TERRAIN selections

private:	//methods
	void	onContentAdded();
		
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
	DirectX::SimpleMath::Vector2 m_mousePosition = { 0.f,0.f };

	// Toggle controllers
	bool m_lDown = false, m_rDown = false;
};

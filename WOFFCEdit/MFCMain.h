#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"

#include "LightDialogue.h"
#include "TerrainDialogue.h"
#include "PaintDialogue.h"
#include "SculptDialogue.h"
#include "ObjectDialogue.h"
#include "SpawnDialogue.h"

#include "SaveDialogue.h"
#include "LoadDialogue.h"

#include "SceneManager.h"

enum class DIALOGUE_TYPE {
	LIGHT,
	OBJECT,
	SPAWN,
	TERRAIN,
	PAINT,
	SCULPT	
};

class MFCMain : public CWinApp 
{
public:
	MFCMain() {}
	~MFCMain() {}
	BOOL InitInstance();
	int  Run();

private:
	void UpdateDialogues();
	void UpdateSave();
	void UpdateLoad();

	void CheckDialogues(DIALOGUE_TYPE type);

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_toolSystem;	//Instance of Tool System that we interface to. 
	CRect m_windowRect;	//Window area rectangle. 

	LightDialogue		m_lightDialogue;
	ObjectDialogue		m_objectDialogue;
	SpawnDialogue		m_spawnDialogue;
	TerrainDialogue		m_terrainDialogue;
	PaintDialogue		m_paintDialogue;
	SculptDialogue		m_sculptDialogue;	

	SaveDialogue		m_saveDialogue;
	LoadDialogue		m_loadDialogue;

	int m_width;		
	int m_height;

	// Controllers for dialogue creation
	bool m_light = false, m_terrain = false, m_paint = false,
		m_sculpt = false, m_object = false, m_spawn = false;
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileSave();
	afx_msg void MenuFileSaveAs();
	afx_msg void MenuFileLoad();
	afx_msg void MenuFileQuit();
	afx_msg void MenuEditWireframeOn();
	afx_msg void MenuEditWireframeOff();
	afx_msg void MenuEditAutosaveOn();
	afx_msg void MenuEditAutosaveOff();
	afx_msg	void ToolBarSave();
	afx_msg void ToolBarUndo();
	afx_msg void ToolBarRedo();
	afx_msg void ToolBarLight();
	afx_msg void ToolBarObject();
	afx_msg void ToolBarSpawn();
	afx_msg void ToolBarTerrain();
	afx_msg void ToolBarPaint();
	afx_msg void ToolBarSculpt();	

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};

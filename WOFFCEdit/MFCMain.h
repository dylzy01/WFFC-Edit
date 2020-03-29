#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "ObjectDialogue.h"
#include "LandscapeDialogue.h"

class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_toolSystem;	//Instance of Tool System that we interface to. 
	CRect m_windowRect;	//Window area rectangle. 
	SelectDialogue m_toolSelectDialogue;			//for modeless dialogue, declare it here
	ObjectDialogue m_toolObjectDialogue;
	LandscapeDialogue m_toolLandscapeDialogue;

	EDITOR m_mode;	//Local mode controller

	int m_width;		
	int m_height;
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg void MenuEditEditorObject();
	afx_msg void MenuEditEditorLandscape();
	afx_msg void MenuEditWireframeOn();
	afx_msg void MenuEditWireframeOff();
	afx_msg	void ToolBarButton1();

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};

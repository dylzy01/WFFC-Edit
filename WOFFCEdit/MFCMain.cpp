#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDITOR_OBJECTEDITOR, &MFCMain::MenuEditEditorObject)
	ON_COMMAND(ID_LANDSCAPEEDITOR_NATURE, &MFCMain::MenuEditEditorLandscapeNature)
	ON_COMMAND(ID_LANDSCAPEEDITOR_WATER, &MFCMain::MenuEditEditorLandscapeWater)
	ON_COMMAND(ID_LANDSCAPEEDITOR_SCULPT, &MFCMain::MenuEditEditorLandscapeSculpt)
	ON_COMMAND(ID_LANDSCAPEEDITOR_PAINT, &MFCMain::MenuEditEditorLandscapePaint)
	ON_COMMAND(ID_WIREFRAME_ON, &MFCMain::MenuEditWireframeOn)
	ON_COMMAND(ID_WIREFRAME_OFF, &MFCMain::MenuEditWireframeOff)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarButton1)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&m_windowRect);
	m_width		= m_windowRect.Width();
	m_height	= m_windowRect.Height();

	m_toolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_toolSystem.UpdateInput(&msg);
		}
		else
		{	
			// Empty status string
			std::wstring statusString = L"";

			// Switch between current mode
			switch (m_toolSystem.GetEditor())
			{
			case EDITOR::OBJECT:
			{
				// Fill status string with selected object IDs
				std::vector<int> IDs = m_toolSystem.getCurrentObjectSelectionID();
				for (int i = 0; i < IDs.size(); ++i)
				{
					if (i != 0) { statusString += L", " + std::to_wstring(IDs[i]); }
					else { statusString += std::to_wstring(IDs[i]); }
				}
			}
			break;
			case EDITOR::SCULPT:
			{
				// Fill status string with selected chunk row,column(s)
				std::vector<TERRAIN> chunks = m_toolSystem.getCurrentChunkSelection();
				for (int i = 0; i < chunks.size(); ++i)
				{
					if (i != 0) { statusString += L", (" + std::to_wstring(chunks[i].row) + L","
							+ std::to_wstring(chunks[i].column) + L")";	}
					else { statusString += L"(" + std::to_wstring(chunks[i].row) + L","
							+ std::to_wstring(chunks[i].column) + L")"; }
				}
			}
			break;
			}

			// If object editor window is open
			if (m_toolObjectDialogue.m_active)
			{
				// Set editor mode to object
				m_toolSystem.SetEditor(EDITOR::OBJECT);

				// If translate function is selected
				if (m_toolObjectDialogue.GetFunctionTranslate()) { m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::TRANSLATE); }

				// Else, if rotate function is selected
				else if (m_toolObjectDialogue.GetFunctionRotate()) { m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::ROTATE); }

				// Else, if scale function is selected
				else if (m_toolObjectDialogue.GetFunctionScale()) { m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::SCALE); }

				// If x and y constraints are selected
				if (m_toolObjectDialogue.GetConstraintX() && m_toolObjectDialogue.GetConstraintY()) { m_toolSystem.SetObjectConstraint(OBJECT_CONSTRAINT::XY); }

				// Else, if x and z constraints are selected
				else if (m_toolObjectDialogue.GetConstraintX() && m_toolObjectDialogue.GetConstraintZ()) { m_toolSystem.SetObjectConstraint(OBJECT_CONSTRAINT::XZ); }

				// Else, if y and z constraints are selected
				else if (m_toolObjectDialogue.GetConstraintY() && m_toolObjectDialogue.GetConstraintZ()) { m_toolSystem.SetObjectConstraint(OBJECT_CONSTRAINT::YZ); }
				
				// Else, if only x constraint is selected
				else if (m_toolObjectDialogue.GetConstraintX()) { m_toolSystem.SetObjectConstraint(OBJECT_CONSTRAINT::X); }

				// Else, if only y constraint is selected
				else if (m_toolObjectDialogue.GetConstraintY()) { m_toolSystem.SetObjectConstraint(OBJECT_CONSTRAINT::Y); }

				// Else, if only z constraint is selected
				else if (m_toolObjectDialogue.GetConstraintZ()) { m_toolSystem.SetObjectConstraint(OBJECT_CONSTRAINT::Z); }

				// Else, if no constraints are selected
				else { m_toolSystem.SetObjectConstraint(OBJECT_CONSTRAINT::ALL); }
			}

			// Else, if landscape editor window is open
			else if (m_toolSculptDialogue.m_active)
			{
				m_toolSystem.SetEditor(EDITOR::SCULPT);
				m_toolSystem.SetSculptFunction(m_toolSculptDialogue.GetFunction());
				m_toolSystem.SetSculptConstraint(m_toolSculptDialogue.GetConstraint());
			}

			// Else, if no editors are open
			else
			{
				m_toolSystem.SetEditor(EDITOR::NA);
			}
			
			// Update tool system
			m_toolSystem.Tick(&msg);			

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
		}
	}

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_toolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_toolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
	m_toolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
	///m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
	m_toolSelectDialogue.SetObjectData(&m_toolSystem.m_sceneGraph, &m_toolSystem.m_selectedObjects);
}

void MFCMain::MenuEditEditorObject()
{
	// Create & display dialogue window
	m_toolObjectDialogue.Create(IDD_DIALOG2);
	m_toolObjectDialogue.ShowWindow(SW_SHOW);
	m_toolObjectDialogue.SetObjectData(&m_toolSystem.m_sceneGraph, &m_toolSystem.m_selectedObjects);
}

void MFCMain::MenuEditEditorLandscapeNature()
{
}

void MFCMain::MenuEditEditorLandscapeWater()
{
}

void MFCMain::MenuEditEditorLandscapeSculpt()
{
	// Create & display dialogue window
	m_toolSculptDialogue.Create(IDD_DIALOG3);
	m_toolSculptDialogue.ShowWindow(SW_SHOW);
	m_toolSculptDialogue.SetTerrainData(&m_toolSystem.m_sceneGraph, &m_toolSystem.m_selectedTerrains);
}

void MFCMain::MenuEditEditorLandscapePaint()
{
}

void MFCMain::MenuEditWireframeOn()
{
	m_toolSystem.SetWireframe(true);
}

void MFCMain::MenuEditWireframeOff()
{
	m_toolSystem.SetWireframe(false);
}

void MFCMain::ToolBarButton1()
{	
	m_toolSystem.onActionSave();
}

MFCMain::MFCMain()
{
}

MFCMain::~MFCMain()
{
}

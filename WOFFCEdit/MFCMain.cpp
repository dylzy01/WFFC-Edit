#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDITOR_OBJECTEDITOR, &MFCMain::MenuEditEditorObject)
	ON_COMMAND(ID_EDITOR_LANDSCAPEEDITOR, &MFCMain::MenuEditEditorLandscape)
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
			case EDITOR::OBJECT_SPAWN:
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
			case EDITOR::OBJECT_TRANSFORM:
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
			case EDITOR::LANDSCAPE_SCULPT:
			{
				// Fill status string with selected chunk row,column(s)
				std::vector<TERRAIN> chunks = m_toolSystem.getCurrentTerrainSelection();
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
				// Switch between spawning/not spawning
				switch (m_toolObjectDialogue.m_spawn)
				{
				case true:
				{
					// Set editor mode to object spawning
					m_toolSystem.SetEditor(EDITOR::OBJECT_SPAWN);

					// If cube spawn is selected
					if (m_toolObjectDialogue.GetSpawnCube()) { m_toolSystem.SetObjectSpawn(OBJECT_SPAWN::CUBE); }
				}
				break;
				case false:
				{
					// Set editor mode to object transforming
					m_toolSystem.SetEditor(EDITOR::OBJECT_TRANSFORM);

					// If translate function is selected
					if (m_toolObjectDialogue.GetFunctionTranslate()) { m_toolSystem.SetObjectTransform(OBJECT_FUNCTION::TRANSLATE); }

					// Else, if rotate function is selected
					else if (m_toolObjectDialogue.GetFunctionRotate()) { m_toolSystem.SetObjectTransform(OBJECT_FUNCTION::ROTATE); }

					// Else, if scale function is selected
					else if (m_toolObjectDialogue.GetFunctionScale()) { m_toolSystem.SetObjectTransform(OBJECT_FUNCTION::SCALE); }

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
				break;
				}			
			}

			// Else, if landscape editor window is open
			else if (m_toolLandscapeDialogue.m_active)
			{
				// Set editor mode
				m_toolSystem.SetEditor(m_toolLandscapeDialogue.GetEditor());

				// Switch between editor modes
				switch (m_toolSystem.GetEditor())
				{
				case EDITOR::LANDSCAPE_PAINT:
				{
					// If grass paint is selected
					if (m_toolLandscapeDialogue.GetPaintGrass()) { m_toolSystem.SetLandscapePaint(LANDSCAPE_PAINT::GRASS); }

					// Else, if dirt paint is selected
					else if (m_toolLandscapeDialogue.GetPaintDirt()) { m_toolSystem.SetLandscapePaint(LANDSCAPE_PAINT::DIRT); }

					// Else, if sand paint is selected
					else if (m_toolLandscapeDialogue.GetPaintSand()) { m_toolSystem.SetLandscapePaint(LANDSCAPE_PAINT::SAND); }
				}
				break;
				case EDITOR::LANDSCAPE_SCULPT:
				{
					// If increase sculpt is selected
					if (m_toolLandscapeDialogue.GetSculptIncrease()) { m_toolSystem.SetLandscapeSculpt(LANDSCAPE_SCULPT::INCREASE); }

					// Else, if decrease sculpt is selected
					else if (m_toolLandscapeDialogue.GetSculptDecrease()) { m_toolSystem.SetLandscapeSculpt(LANDSCAPE_SCULPT::DECREASE); }

					// Else, if flatten sculpt is selecteed
					else if (m_toolLandscapeDialogue.GetSculptFlatten()) { m_toolSystem.SetLandscapeSculpt(LANDSCAPE_SCULPT::FLATTEN); }

					// If x and y constraints are selected
					if (m_toolLandscapeDialogue.GetConstraintX() && m_toolLandscapeDialogue.GetConstraintY()) { m_toolSystem.SetLandscapeConstraint(LANDSCAPE_CONSTRAINT::XY); }

					// Else, if x and z constraints are selected
					else if (m_toolLandscapeDialogue.GetConstraintX() && m_toolLandscapeDialogue.GetConstraintZ()) { m_toolSystem.SetLandscapeConstraint(LANDSCAPE_CONSTRAINT::XZ); }

					// Else, if y and z constraints are selected
					else if (m_toolLandscapeDialogue.GetConstraintY() && m_toolLandscapeDialogue.GetConstraintZ()) { m_toolSystem.SetLandscapeConstraint(LANDSCAPE_CONSTRAINT::YZ); }

					// Else, if only x constraint is selected
					else if (m_toolLandscapeDialogue.GetConstraintX()) { m_toolSystem.SetLandscapeConstraint(LANDSCAPE_CONSTRAINT::X); }

					// Else, if only y constraint is selected
					else if (m_toolLandscapeDialogue.GetConstraintY()) { m_toolSystem.SetLandscapeConstraint(LANDSCAPE_CONSTRAINT::Y); }

					// Else, if only z constraint is selected
					else if (m_toolLandscapeDialogue.GetConstraintZ()) { m_toolSystem.SetLandscapeConstraint(LANDSCAPE_CONSTRAINT::Z); }

					// Else, if no constraints are selected
					else { m_toolSystem.SetLandscapeConstraint(LANDSCAPE_CONSTRAINT::ALL); }
				}
				break;
				}
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
	m_toolObjectDialogue.SetActive(true);
}

void MFCMain::MenuEditEditorLandscape()
{
	// Create & display dialogue window
	m_toolLandscapeDialogue.Create(IDD_DIALOG3);
	m_toolLandscapeDialogue.ShowWindow(SW_SHOW);
	m_toolLandscapeDialogue.SetActive(true);
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

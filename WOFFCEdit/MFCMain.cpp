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
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarSave)
	ON_COMMAND(ID_BUTTON40055, &MFCMain::ToolBarObjectSelect)
	ON_COMMAND(ID_BUTTON40050, &MFCMain::ToolBarObjectSpawn)
	ON_COMMAND(ID_BUTTON40059, &MFCMain::ToolBarObjectDelete)
	ON_COMMAND(ID_BUTTON40052, &MFCMain::ToolBarTerrainSculpt)
	ON_COMMAND(ID_BUTTON40048, &MFCMain::ToolBarTerrainPaint)
	ON_COMMAND(ID_BUTTON40053, &MFCMain::ToolBarUndo)
	ON_COMMAND(ID_BUTTON40054, &MFCMain::ToolBarRedo)
	ON_COMMAND(ID_BUTTON40060, &MFCMain::ToolBarLight)
	ON_COMMAND(ID_BUTTON40061, &MFCMain::ToolBarTerrain)
	ON_COMMAND(ID_BUTTON40062, &MFCMain::ToolBarPaint)
	ON_COMMAND(ID_BUTTON40063, &MFCMain::ToolBarSculpt)
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

			// Fill status string with selected object IDs
			std::vector<int> IDs = m_toolSystem.getCurrentObjectSelectionID();
			for (int i = 0; i < IDs.size(); ++i)
			{
				if (i != 0) { statusString += L", " + std::to_wstring(IDs[i]); }
				else { statusString += std::to_wstring(IDs[i]); }
			}

			// Check which dialogue is open & update ToolMain
			CheckDialogues();
			
			// Update tool system
			m_toolSystem.Tick(&msg);			

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
		}
	}

	return (int)msg.wParam;
}

void MFCMain::CheckDialogues()
{
	// If object editor is active
	if (m_objectEditorDialogue.GetActive())
	{
		// Set other modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_SPAWN::NA);
		m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
		m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);
		
		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::OBJECT_FUNCTION);

		// Set transform mode
		m_toolSystem.SetObjectFunction(m_objectEditorDialogue.GetFunction());

		// If x and y constraints are selected
		if (m_objectEditorDialogue.GetConstraintX() && m_objectEditorDialogue.GetConstraintY()) { m_toolSystem.SetObjectConstraint(CONSTRAINT::XY); }

		// Else, if x and z constraints are selected
		else if (m_objectEditorDialogue.GetConstraintX() && m_objectEditorDialogue.GetConstraintZ()) { m_toolSystem.SetObjectConstraint(CONSTRAINT::XZ); }

		// Else, if y and z constraints are selected
		else if (m_objectEditorDialogue.GetConstraintY() && m_objectEditorDialogue.GetConstraintZ()) { m_toolSystem.SetObjectConstraint(CONSTRAINT::YZ); }

		// Else, if only x constraint is selected
		else if (m_objectEditorDialogue.GetConstraintX()) { m_toolSystem.SetObjectConstraint(CONSTRAINT::X); }

		// Else, if only y constraint is selected
		else if (m_objectEditorDialogue.GetConstraintY()) { m_toolSystem.SetObjectConstraint(CONSTRAINT::Y); }

		// Else, if only z constraint is selected
		else if (m_objectEditorDialogue.GetConstraintZ()) { m_toolSystem.SetObjectConstraint(CONSTRAINT::Z); }

		// Else, if no constraints are selected
		else { m_toolSystem.SetObjectConstraint(CONSTRAINT::NA); }
	}
	
	// Else, if object spawner is active
	else if (m_objectSpawnDialogue.GetActive())
	{
		// Set other modes to none
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
		m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
		m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);
		
		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::OBJECT_SPAWN);

		// Set spawn mode
		m_toolSystem.SetObjectSpawn(m_objectSpawnDialogue.GetSpawn());
	}

	// Else, if terrain sculpter is active
	else if (m_terrainSculptDialogue.GetActive())
	{
		// Set other modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_SPAWN::NA);
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
		m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);
		
		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::SCULPT_FREELY);

		// Set sculpt mode
		m_toolSystem.SetTerrainSculpt(m_terrainSculptDialogue.GetSculpt());

		// If x and y constraints are selected
		if (m_terrainSculptDialogue.GetConstraintX() && m_terrainSculptDialogue.GetConstraintY()) { m_toolSystem.SetTerrainConstraint(CONSTRAINT::XY); }

		// Else, if x and z constraints are selected
		else if (m_terrainSculptDialogue.GetConstraintX() && m_terrainSculptDialogue.GetConstraintZ()) { m_toolSystem.SetTerrainConstraint(CONSTRAINT::XZ); }

		// Else, if y and z constraints are selected
		else if (m_terrainSculptDialogue.GetConstraintY() && m_terrainSculptDialogue.GetConstraintZ()) { m_toolSystem.SetTerrainConstraint(CONSTRAINT::YZ); }

		// Else, if only x constraint is selected
		else if (m_terrainSculptDialogue.GetConstraintX()) { m_toolSystem.SetTerrainConstraint(CONSTRAINT::X); }

		// Else, if only y constraint is selected
		else if (m_terrainSculptDialogue.GetConstraintY()) { m_toolSystem.SetTerrainConstraint(CONSTRAINT::Y); }

		// Else, if only z constraint is selected
		else if (m_terrainSculptDialogue.GetConstraintZ()) { m_toolSystem.SetTerrainConstraint(CONSTRAINT::Z); }

		// Else, if no constraints are selected
		else { m_toolSystem.SetTerrainConstraint(CONSTRAINT::NA); }
	}

	// Else, if terrain painter is active
	else if (m_terrainPaintDialogue.GetActive())
	{
		// Set other modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_SPAWN::NA);
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
		m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
		
		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::TERRAIN_PAINT);

		// Set paint mode
		m_toolSystem.SetTerrainPaint(m_terrainPaintDialogue.GetPaint());
	}

	
	
	
	
	
	// If light inspector is active
	else if (m_lightDialogue.GetActive())
	{
		// If scene graph should be updated
		if (m_lightDialogue.GetUpdate())
		{
			// Reset update controller
			m_lightDialogue.SetUpdate(false);

			// Get edited lights
			std::pair<std::vector<Light*>, std::vector<int>> lights = m_lightDialogue.GetLights();

			// Get scene graph
			std::vector<SceneObject> sceneGraph = m_toolSystem.GetSceneGraph();

			// Loop through scene graph
			for (int i = 0; i < sceneGraph.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < lights.first.size(); ++j)
				{
					// If scene graph ID matches light ID
					if (sceneGraph[i].ID == lights.second[j])
					{
						// Store current scene object
						SceneObject object = sceneGraph[i];						
						
						// Setup object details from matching light
						{
							object.light_diffuse_r = lights.first[j]->GetDiffuse().x;
							object.light_diffuse_g = lights.first[j]->GetDiffuse().y;
							object.light_diffuse_b = lights.first[j]->GetDiffuse().z;
							object.posX = lights.first[j]->GetPosition().x;
							object.posY = lights.first[j]->GetPosition().y;
							object.posZ = lights.first[j]->GetPosition().z;
							object.dirX = lights.first[j]->GetDirection().x;
							object.dirY = lights.first[j]->GetDirection().y;
							object.dirZ = lights.first[j]->GetDirection().z;
							object.light_type = (int)lights.first[j]->GetType();
							object.light_constant = lights.first[j]->GetConstantAttenuation();
							object.light_linear = lights.first[j]->GetLinearAttenuation();
							object.light_quadratic = lights.first[j]->GetQuadraticAttenuation();
							object.enabled = lights.first[j]->GetEnabled();
							object.ambR = lights.first[j]->GetAmbient().x;
							object.ambG = lights.first[j]->GetAmbient().y;
							object.ambB = lights.first[j]->GetAmbient().z;
						}

						// Replace scene object with new light	
						m_toolSystem.SetSceneObject(object, i);
					}
				}
			}
		}

		// Else, if light is being translated
		else if (m_lightDialogue.GetTranslating())
		{
			// Set tool editor
			m_toolSystem.SetEditor(EDITOR::OBJECT_FUNCTION);

			// Set transform mode
			m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::TRANSLATE);

			// Set constraint
			m_toolSystem.SetObjectConstraint(m_lightDialogue.GetConstraint());

			// Update selected object (light)
			m_toolSystem.SetSelectedObjectID(m_lightDialogue.GetSelectedLightID());
		}
	}

	// Else, if terrain inspector is active
	else if (m_terrainDialogue.GetActive())
	{
		// If right mouse is pressed
		if (m_toolSystem.GetInput()->mouseRight)
		{
			// If the user isn't sculpting
			if (!m_terrainDialogue.GetSculpting())
			{
				// Set tool editor to none
				m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
				
				// Get current terrain selection
				TERRAIN terrain = MouseManager::PickTerrain();

				// If terrain has been intersected
				if (terrain.intersect)
				{
					// Update dialogue data
					m_terrainDialogue.UpdateTerrain(terrain.row, terrain.column);
					m_terrainDialogue.SetTerrain(terrain);
				}				
			}

			// Else, the user is sculpting
			else
			{
				// Set tool editor
				m_toolSystem.SetEditor(EDITOR::SCULPT_SINGLE);
				
				// Update tool selected terrain to match dialogue
				TERRAIN temp = m_terrainDialogue.GetTerrain();
				m_toolSystem.SetSelectedTerrain(m_terrainDialogue.GetTerrain());

				// Set sculpt mode
				m_toolSystem.SetTerrainSculpt(m_terrainDialogue.GetSculpt());

				// Set constraint
				m_toolSystem.SetTerrainConstraint(m_terrainDialogue.GetConstraint());
			}			
		}
	}

	// Else, if paint inspector is active
	else if (m_paintDialogue.GetActive())
	{
		// Set other modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_SPAWN::NA);
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
		m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
	
		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::TERRAIN_PAINT);

		// Set paint mode
		m_toolSystem.SetTerrainPaint(m_paintDialogue.GetPaint());
	}

	// Else, if sculpt inspector is active
	else if (m_sculptDialogue.GetActive())
	{
		// Set other modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_SPAWN::NA);
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
		m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::SCULPT_FREELY);

		// Set sculpt mode
		m_toolSystem.SetTerrainSculpt(m_sculptDialogue.GetSculpt());

		// Set constraint
		m_toolSystem.SetTerrainConstraint(m_sculptDialogue.GetConstraint());
	}

	// Else, if no dialogues are active
	else
	{
		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::NA);

		// Set all modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_SPAWN::NA);
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
		m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
		m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

		// Deselect all objects
		m_toolSystem.ClearSelected();
	}
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
	m_toolSelectDialogue.SetObjectData(&m_toolSystem.m_sceneGraph, &m_toolSystem.m_selectedObjectIDs);
}

void MFCMain::MenuEditEditorObject()
{
	// Create & display dialogue window
	m_toolObjectDialogue.Create(IDD_DIALOG2);
	m_toolObjectDialogue.ShowWindow(SW_SHOW);
	m_toolObjectDialogue.SetObjectData(&m_toolSystem.m_sceneGraph, &m_toolSystem.m_selectedObjectIDs);
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

void MFCMain::ToolBarSave()
{	
	m_toolSystem.onActionSaveTerrain();
	m_toolSystem.onActionSave();	
}

void MFCMain::ToolBarObjectSpawn()
{
	// Destroy other windows
	m_objectEditorDialogue.DestroyWindow();
	m_terrainSculptDialogue.DestroyWindow();
	m_terrainPaintDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();

	// Create & display dialogue window
	m_objectSpawnDialogue.Create(IDD_DIALOG4);
	m_objectSpawnDialogue.ShowWindow(SW_SHOW);
	m_objectSpawnDialogue.SetActive(true);
}

void MFCMain::ToolBarObjectSelect()
{
	// Destroy other windows
	m_objectSpawnDialogue.DestroyWindow();
	m_terrainSculptDialogue.DestroyWindow();
	m_terrainPaintDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();

	// Create & display dialogue window
	m_objectEditorDialogue.Create(IDD_DIALOG7);
	m_objectEditorDialogue.ShowWindow(SW_SHOW);
	m_objectEditorDialogue.SetActive(true);
}

void MFCMain::ToolBarObjectDelete()
{
	m_toolSystem.onActionDeleteObjects();
}

void MFCMain::ToolBarTerrainSculpt()
{
	// Destroy other windows
	m_objectEditorDialogue.DestroyWindow();
	m_objectSpawnDialogue.DestroyWindow();
	m_terrainPaintDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();
	
	// Create & display dialogue window
	m_terrainSculptDialogue.Create(IDD_DIALOG5);
	m_terrainSculptDialogue.ShowWindow(SW_SHOW);
	m_terrainSculptDialogue.SetActive(true);
}

void MFCMain::ToolBarTerrainPaint()
{
	// Destroy other windows
	m_objectEditorDialogue.DestroyWindow();
	m_objectSpawnDialogue.DestroyWindow();
	m_terrainSculptDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();
	
	// Create & display dialogue window
	m_terrainPaintDialogue.Create(IDD_DIALOG6);
	m_terrainPaintDialogue.ShowWindow(SW_SHOW);
	m_terrainPaintDialogue.SetActive(true);
}

void MFCMain::ToolBarUndo()
{
	m_toolSystem.onActionUndo();
}

void MFCMain::ToolBarRedo()
{
	m_toolSystem.onActionRedo();
}

void MFCMain::ToolBarLight()
{
	// Destroy other windows
	m_objectEditorDialogue.DestroyWindow();
	m_objectSpawnDialogue.DestroyWindow();
	m_terrainDialogue.DestroyWindow();
	m_sculptDialogue.DestroyWindow();
	m_paintDialogue.DestroyWindow();

	// Create & display dialogue window
	m_lightDialogue.Create(IDD_DIALOG8);
	m_lightDialogue.ShowWindow(SW_SHOW);
	m_lightDialogue.SetActive(true);
	m_lightDialogue.SetLightData(&m_toolSystem.GetLights());
}

void MFCMain::ToolBarTerrain()
{
	// Destroy other windows (except terrain related)
	m_objectEditorDialogue.DestroyWindow();
	m_objectSpawnDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();
	m_sculptDialogue.DestroyWindow();
	m_paintDialogue.DestroyWindow();

	// Create & display dialogue window
	m_terrainDialogue.Create(IDD_DIALOG9);
	m_terrainDialogue.ShowWindow(SW_SHOW);
	m_terrainDialogue.SetActive(true);
	m_terrainDialogue.SetChunkData(m_toolSystem.GetDisplayChunk());
}

void MFCMain::ToolBarPaint()
{
	// Destroy other windows 
	m_objectEditorDialogue.DestroyWindow();
	m_objectSpawnDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();
	m_terrainDialogue.DestroyWindow();
	m_sculptDialogue.DestroyWindow();

	// Create & display dialogue window
	m_paintDialogue.Create(IDD_DIALOG10);
	m_paintDialogue.ShowWindow(SW_SHOW);
	m_paintDialogue.SetActive(true);
	m_paintDialogue.SetChunkData(m_toolSystem.GetDisplayChunk());
}

void MFCMain::ToolBarSculpt()
{
	// Destroy other windows (except terrain inspector)
	m_objectEditorDialogue.DestroyWindow();
	m_objectSpawnDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();
	m_terrainDialogue.DestroyWindow();
	m_paintDialogue.DestroyWindow();

	// Create & display dialogue window
	m_sculptDialogue.Create(IDD_DIALOG11);
	m_sculptDialogue.ShowWindow(SW_SHOW);
	m_sculptDialogue.SetActive(true);
	m_sculptDialogue.SetChunkData(m_toolSystem.GetDisplayChunk());
}

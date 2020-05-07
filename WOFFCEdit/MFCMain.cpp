#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_SAVE, &MFCMain::MenuFileSave)
	ON_COMMAND(ID_FILE_SAVEAS, &MFCMain::MenuFileSaveAs)
	ON_COMMAND(ID_FILE_LOAD, &MFCMain::MenuFileLoad)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_WIREFRAME_ON, &MFCMain::MenuEditWireframeOn)
	ON_COMMAND(ID_WIREFRAME_OFF, &MFCMain::MenuEditWireframeOff)
	ON_COMMAND(ID_AUTOSAVE_ON, &MFCMain::MenuEditAutosaveOn)
	ON_COMMAND(ID_AUTOSAVE_OFF, &MFCMain::MenuEditAutosaveOff)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarSave)
	ON_COMMAND(ID_BUTTON40053, &MFCMain::ToolBarUndo)
	ON_COMMAND(ID_BUTTON40054, &MFCMain::ToolBarRedo)
	ON_COMMAND(ID_BUTTON40060, &MFCMain::ToolBarLight)
	ON_COMMAND(ID_BUTTON40061, &MFCMain::ToolBarTerrain)
	ON_COMMAND(ID_BUTTON40062, &MFCMain::ToolBarPaint)
	ON_COMMAND(ID_BUTTON40063, &MFCMain::ToolBarSculpt)
	ON_COMMAND(ID_BUTTON40065, &MFCMain::ToolBarObject)
	ON_COMMAND(ID_BUTTON40050, &MFCMain::ToolBarSpawn)
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
			std::vector<int> IDs = m_toolSystem.GetSelectedObjectIDs();
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
	// If save dialogue is active
	if (m_saveDialogue.GetActive()) { UpdateSave(); }

	// Else, if load dialogue is active
	else if (m_loadDialogue.GetActive()) { UpdateLoad(); }

	/////////////////////////////////////////////////////////

	// If light inspector is active
	if (m_lightDialogue.GetActive()) { UpdateLights(); }	

	// Else, if object inspector is active
	else if (m_objectDialogue.GetActive()) { UpdateObjects(); }

	// Else, if spawn inspector is active
	else if (m_spawnDialogue.GetActive()) { UpdateSpawn(); }

	// Else, if terrain inspector is active
	else if (m_terrainDialogue.GetActive()) { UpdateTerrain(); }

	// Else, if paint inspector is active
	else if (m_paintDialogue.GetActive()) { UpdatePaint(); }	

	// Else, if sculpt inspector is active
	else if (m_sculptDialogue.GetActive()) { UpdateSculpt(); }	
	
	// Else, if no dialogues are active
	else
	{
		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::NA);

		// Set all modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_TYPE::NA);
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
		m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
		m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

		// Deselect all objects
		m_toolSystem.ClearSelected();
	}
}

// Update via individual dialogue functions /////////////////////////////

void MFCMain::UpdateSave()
{
	// If dialogue is updated
	if (m_saveDialogue.GetUpdate())
	{
		// Save current chunk with input name
		m_toolSystem.onActionSaveAs(m_saveDialogue.GetName());

		// Reset controller
		m_saveDialogue.SetUpdate(false);
	}
}

void MFCMain::UpdateLoad()
{
	// If dialogue is updated
	if (m_loadDialogue.GetUpdate())
	{
		// Save current chunk with input name
		m_toolSystem.onActionLoad(m_loadDialogue.GetPath());

		// Reset controller
		m_loadDialogue.SetUpdate(false);
	}
}

void MFCMain::UpdateLights()
{
	// Set tool editor mode
	m_toolSystem.SetEditor(EDITOR::LIGHTS);
	
	// If ToolMain selected objects (lights) should be updated
	if (m_lightDialogue.GetSelect()) { m_toolSystem.SetSelectedObjectIDs(m_lightDialogue.GetSelectedLightIDs()); }
	// Else, update dialogue selected objects (lights)
	else { m_lightDialogue.SetSelectedLightIDs(m_toolSystem.GetSelectedObjectIDs()); }
	
	// If there's been a new selection
	if (m_toolSystem.GetNewSelection())
	{
		// Update dialogue entries to match selected light
		m_lightDialogue.Update();
	}
	
	// If dialogue is requesting display list
	if (m_lightDialogue.GetRequest())
	{
		// Update display list
		m_lightDialogue.SetLightData(&m_toolSystem.GetDisplayList());

		// Reset controller
		m_lightDialogue.SetRequest(false);
	}

	// If should focus on a light
	if (m_lightDialogue.GetFocus())
	{
		// Setup temp focus ID
		int focusID = -1;

		// If more than one object is selected
		if (m_lightDialogue.GetSelectedLightIDs().size() > 1)
		{
			// Define object ID to focus on
			focusID = m_lightDialogue.GetFocusDialogue()->GetSelectedIndex();
		}

		// Else, if just one object is selected
		else if (m_lightDialogue.GetSelectedLightIDs().size() == 1)
		{
			// Define object ID to focus on
			focusID = m_lightDialogue.GetSelectedLightIDs()[0];
		}

		// Setup object ID to focus on
		m_toolSystem.SetFocus(focusID);
	}
	else { m_toolSystem.SetFocus(-1); }	

	// If scene graph should be updated
	if (m_lightDialogue.GetUpdate())
	{
		// Get edited lights
		std::vector<DisplayObject> lights = m_lightDialogue.GetLights();

		// Get scene graph
		std::vector<SceneObject> sceneGraph = m_toolSystem.GetSceneGraph();

		// Loop through scene graph
		for (int i = 0; i < sceneGraph.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < lights.size(); ++j)
			{
				// If scene graph ID matches light ID
				if (sceneGraph[i].ID == lights[j].m_ID)
				{
					// Store current scene object
					SceneObject object = sceneGraph[i];

					// Setup object details from matching light
					{
						object.light_diffuse_r = lights[j].GetDiffuse().x;
						object.light_diffuse_g = lights[j].GetDiffuse().y;
						object.light_diffuse_b = lights[j].GetDiffuse().z;
						object.posX = lights[j].GetPosition().x;
						object.posY = lights[j].GetPosition().y;
						object.posZ = lights[j].GetPosition().z;
						object.rotX = lights[j].GetDirection().x;
						object.rotY = lights[j].GetDirection().y;
						object.rotZ = lights[j].GetDirection().z;
						object.light_type = (int)lights[j].GetLightType();
						object.light_constant = lights[j].GetConstantAttenuation();
						object.light_linear = lights[j].GetLinearAttenuation();
						object.light_quadratic = lights[j].GetQuadraticAttenuation();
						object.enabled = lights[j].GetEnabled();
						object.ambR = lights[j].GetAmbient().x;
						object.ambG = lights[j].GetAmbient().y;
						object.ambB = lights[j].GetAmbient().z;
					}

					// Replace scene object with new light	
					m_toolSystem.SetSceneObject(object, i);
				}
			}
		}

		// Reset update controller
		m_lightDialogue.SetUpdate(false);
	}
	
	// If light is being deleted
	if (m_lightDialogue.GetDelete())
	{
		// Remove objects from database storage
		ObjectManager::Remove(m_lightDialogue.GetSelectedLightIDs(), m_toolSystem.GetSceneGraph());

		// Reset controller 
		m_lightDialogue.SetDelete(false);
	}

	// Else, if light is being duplicated
	else if (m_lightDialogue.GetDuplicate())
	{
		// Copy objects
		ObjectManager::Copy(m_lightDialogue.GetSelectedLightIDs(), m_toolSystem.GetSceneGraph());

		// Paste objects
		ObjectManager::Paste(m_toolSystem.GetSceneGraph());

		// Reset controller
		m_lightDialogue.SetDuplicate(false);
	}
	
	// Else, if light is being translated
	else if (m_lightDialogue.GetTranslating())
	{
		// Set transform mode
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::TRANSLATE);

		// Set constraint
		m_toolSystem.SetObjectConstraint(m_lightDialogue.GetConstraint());

		// Loop through lights
		for (int i = 0; i < m_lightDialogue.GetLights().size(); ++i)
		{
			// Loop through selection
			for (int j = 0; j < m_lightDialogue.GetSelectedLightIDs().size(); ++j)
			{
				// If light matches currently selected
				if (m_lightDialogue.GetLights()[i].m_ID == m_lightDialogue.GetSelectedLightIDs()[j])
				{
					// Update position
					m_lightDialogue.UpdateLightPosition(m_toolSystem.GetLights()[i].GetPosition());
				}
			}
		}
	}

	// Else, must be selecting lights
	else
	{
		// Set transform mode
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::SELECT);

		// Set constraint
		m_toolSystem.SetObjectConstraint(m_lightDialogue.GetConstraint());
	}	
}

void MFCMain::UpdateObjects()
{
	// If ToolMain selected objects should be updated
	if (m_objectDialogue.GetSelect()) { m_toolSystem.SetSelectedObjectIDs(m_objectDialogue.GetSelectedObjectIDs()); }
	// Else, update dialogue selected objects
	else { m_objectDialogue.SetSelectedObjects(m_toolSystem.GetSelectedObjectIDs()); }
	
	// If there's been a new selection
	if (m_toolSystem.GetNewSelection())
	{
		// Update dialogue entries to match selected object
		m_objectDialogue.Update();
	}
	
	// If dialogue is requesting scene graph
	if (m_objectDialogue.GetRequest())
	{
		// Update scene graph
		m_objectDialogue.SetObjectData(&m_toolSystem.GetSceneGraph());
		
		// Reset request
		m_objectDialogue.SetRequest(false);
	}

	// If should focus on an object
	if (m_objectDialogue.GetFocus())
	{
		// Setup temp focus ID
		int focusID = -1;

		// If more than one object is selected
		if (m_objectDialogue.GetSelectedObjectIDs().size() > 1)
		{
			// Define object ID to focus on
			focusID = m_objectDialogue.GetFocusDialogue()->GetSelectedIndex();
		}

		// Else, if just one object is selected
		else if (m_objectDialogue.GetSelectedObjectIDs().size() == 1)
		{
			// Define object ID to focus on
			focusID = m_objectDialogue.GetSelectedObjectIDs()[0];
		}

		// Setup object ID to focus on
		m_toolSystem.SetFocus(focusID);
	}
	else { m_toolSystem.SetFocus(-1); }
	
	// If objects have been setup
	if (m_objectDialogue.GetObjectsSetup())
	{
		// Update selected object
		m_toolSystem.SetSelectedObjectIDs(m_objectDialogue.GetSelectedObjectIDs());

		// Set other modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_TYPE::NA);
		m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
		m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

		// Set tool editor
		m_toolSystem.SetEditor(EDITOR::OBJECT_FUNCTION);

		// Set object function
		m_toolSystem.SetObjectFunction(m_objectDialogue.GetFunction());

		// Set constraint
		m_toolSystem.SetObjectConstraint(m_objectDialogue.GetConstraint());
	}

	// If scene graph should be updated
	if (m_objectDialogue.GetUpdate())
	{
		// Get edited objects
		std::vector<SceneObject> objects = m_objectDialogue.GetObjects();

		// Get scene graph
		std::vector<SceneObject> sceneGraph = m_toolSystem.GetSceneGraph();
				
		// Loop through objects
		for (int i = 0; i < objects.size(); ++i)
		{
			// Loop through selected object IDs
			for (int j = 0; j < m_objectDialogue.GetSelectedObjectIDs().size(); ++j)
			{
				// If IDs match
				if (objects[i].ID == m_objectDialogue.GetSelectedObjectIDs()[j])
				{
					// Replace object type through database
					ObjectManager::ReplaceObject(objects[i], sceneGraph);
					break;
				}
			}							
		}	

		// Reset update controller
		m_objectDialogue.SetUpdate(false);
	}

	// If object is being deleted
	if (m_objectDialogue.GetDelete())
	{
		// Remove objects from database storage
		ObjectManager::Remove(m_objectDialogue.GetSelectedObjectIDs(), m_toolSystem.GetSceneGraph());

		// Reset controller 
		m_objectDialogue.SetDelete(false);
	}

	// Else, if object is being duplicated
	else if (m_objectDialogue.GetDuplicate())
	{
		// Copy objects
		ObjectManager::Copy(m_objectDialogue.GetSelectedObjectIDs(), m_toolSystem.GetSceneGraph());

		// Paste objects
		ObjectManager::Paste(m_toolSystem.GetSceneGraph());

		// Reset controller
		m_objectDialogue.SetDuplicate(false);
	}
}

void MFCMain::UpdateSpawn()
{
	// Set other modes to none
	m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
	m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
	m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

	// Set tool editor
	m_toolSystem.SetEditor(EDITOR::OBJECT_SPAWN);

	// Set spawn mode
	m_toolSystem.SetObjectSpawn(m_spawnDialogue.GetSpawnType());
}

void MFCMain::UpdateTerrain()
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

void MFCMain::UpdatePaint()
{
	// Set other modes to none
	m_toolSystem.SetObjectSpawn(OBJECT_TYPE::NA);
	m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
	m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);

	// Set tool editor
	m_toolSystem.SetEditor(EDITOR::TERRAIN_PAINT);

	// Set paint mode
	m_toolSystem.SetTerrainPaint(m_paintDialogue.GetPaint());
}

void MFCMain::UpdateSculpt()
{
	// Set other modes to none
	m_toolSystem.SetObjectSpawn(OBJECT_TYPE::NA);
	m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
	m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

	// Set tool editor
	m_toolSystem.SetEditor(EDITOR::SCULPT_FREELY);

	// Set sculpt mode
	m_toolSystem.SetTerrainSculpt(m_sculptDialogue.GetSculpt());

	// Set constraint
	m_toolSystem.SetTerrainConstraint(m_sculptDialogue.GetConstraint());
}

// Message handlers (menu) //////////////////////////////////////////////

void MFCMain::MenuFileSave()
{
	m_toolSystem.onActionSave();
}

void MFCMain::MenuFileSaveAs()
{
	// Create & display dialogue window
	m_saveDialogue.Create(IDD_DIALOG3);
	m_saveDialogue.ShowWindow(SW_SHOW);
	m_saveDialogue.SetActive(true);
}

void MFCMain::MenuFileLoad()
{
	// Create & display dialogue window
	m_loadDialogue.Create(IDD_DIALOG4);
	m_loadDialogue.ShowWindow(SW_SHOW);
	m_loadDialogue.SetActive(true);
	m_loadDialogue.SetupData(4);
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuEditWireframeOn()
{
	m_toolSystem.SetWireframe(true);
}

void MFCMain::MenuEditWireframeOff()
{
	m_toolSystem.SetWireframe(false);
}

void MFCMain::MenuEditAutosaveOn()
{
	SceneManager::SetAutosave(true);
}

void MFCMain::MenuEditAutosaveOff()
{
	SceneManager::SetAutosave(false);
}

// Message handlers (toolbar) ///////////////////////////////////////////

void MFCMain::ToolBarSave()
{	
	m_toolSystem.onActionSave();	
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
	///if (m_objectDialogue.IsWindowVisible())
	m_objectDialogue.DestroyWindow();
	m_spawnDialogue.DestroyWindow();
	m_terrainDialogue.DestroyWindow();
	m_sculptDialogue.DestroyWindow();
	m_paintDialogue.DestroyWindow();

	// Create & display dialogue window
	m_lightDialogue.Create(IDD_DIALOG8);
	m_lightDialogue.ShowWindow(SW_SHOW);
	m_lightDialogue.SetActive(true);
	m_lightDialogue.SetLightData(&m_toolSystem.GetDisplayList());
	
	// Setup temp light ID container
	std::vector<int> IDs;

	// Loop through currently selected objects
	for (int i = 0; i < m_toolSystem.GetSelectedObjectIDs().size(); ++i)
	{
		// If object is a light
		if (m_toolSystem.GetSceneGraph()[m_toolSystem.GetSelectedObjectIDs()[i]].m_type == OBJECT_TYPE::LIGHT)
		{
			// Add ID to container
			IDs.push_back(m_toolSystem.GetSceneGraph()[m_toolSystem.GetSelectedObjectIDs()[i]].ID);
		}
	}

	// Set current selection
	///m_lightDialogue.SetLightData(&m_toolSystem.GetLights(), IDs);
	m_lightDialogue.SetSelectedLightIDs(IDs);
}

void MFCMain::ToolBarTerrain()
{
	// Destroy other windows 
	m_objectDialogue.DestroyWindow();
	m_spawnDialogue.DestroyWindow();
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
	m_objectDialogue.DestroyWindow();
	m_spawnDialogue.DestroyWindow();
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
	// Destroy other windows 
	m_objectDialogue.DestroyWindow();
	m_spawnDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();
	m_terrainDialogue.DestroyWindow();
	m_paintDialogue.DestroyWindow();

	// Create & display dialogue window
	m_sculptDialogue.Create(IDD_DIALOG11);
	m_sculptDialogue.ShowWindow(SW_SHOW);
	m_sculptDialogue.SetActive(true);
	m_sculptDialogue.SetChunkData(m_toolSystem.GetDisplayChunk());
}

void MFCMain::ToolBarObject()
{
	// Destroy other windows 
	m_spawnDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();
	m_terrainDialogue.DestroyWindow();
	m_paintDialogue.DestroyWindow();
	m_sculptDialogue.DestroyWindow();

	// Create & display dialogue window
	m_objectDialogue.Create(IDD_DIALOG12);
	m_objectDialogue.ShowWindow(SW_SHOW);
	m_objectDialogue.SetActive(true);
	m_objectDialogue.SetObjectData(&m_toolSystem.GetSceneGraph());
}

void MFCMain::ToolBarSpawn()
{
	// Destroy other windows 
	m_objectDialogue.DestroyWindow();
	m_lightDialogue.DestroyWindow();
	m_terrainDialogue.DestroyWindow();
	m_paintDialogue.DestroyWindow();
	m_sculptDialogue.DestroyWindow();

	// Create & display dialogue window
	m_spawnDialogue.Create(IDD_DIALOG13);
	m_spawnDialogue.ShowWindow(SW_SHOW);
	m_spawnDialogue.SetActive(true);
	m_spawnDialogue.SetObjectData(&m_toolSystem.GetSceneGraph());
}

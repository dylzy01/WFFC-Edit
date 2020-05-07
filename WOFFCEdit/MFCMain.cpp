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
	ON_COMMAND(ID_BUTTON40065, &MFCMain::ToolBarObject)
	ON_COMMAND(ID_BUTTON40050, &MFCMain::ToolBarSpawn)
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
			std::vector<int> IDs = m_toolSystem.GetSelectedIDs();
			for (int i = 0; i < IDs.size(); ++i)
			{
				if (i != 0) { statusString += L", " + std::to_wstring(IDs[i]); }
				else { statusString += std::to_wstring(IDs[i]); }
			}

			// Check which dialogue is open & update ToolMain
			UpdateDialogues();
			
			// Update tool system
			m_toolSystem.Tick(&msg);			

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
		}
	}

	return (int)msg.wParam;
}

// Update via individual dialogue functions /////////////////////////////

void MFCMain::UpdateDialogues()
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
	// If dialogue needs updated data or the mouse has been dragged
	if (m_lightDialogue.GetRequest())/// || m_toolSystem.GetRequest())
	{
		// Send updated data
		m_lightDialogue.SetLightData(&m_toolSystem.GetSceneGraph(), m_toolSystem.GetSelectedIDs());
	}
	
	// If ToolMain selected objects should be updated
	if (m_lightDialogue.GetSelect()) { m_toolSystem.SetSelectedObjectIDs(m_lightDialogue.GetSelectedIDs()); }
	// Else, update dialogue selected objects
	else { m_lightDialogue.SetSelectedIDs(m_toolSystem.GetSelectedIDs()); }

	// If there's been a new selection
	if (m_toolSystem.GetNewSelection())
	{
		// Update dialogue entries to match selected object
		m_lightDialogue.Update();
	}

	// If should focus on an object
	if (m_lightDialogue.GetFocus())
	{
		// Setup temp focus ID
		int focusID = -1;

		// If more than one object is selected
		if (m_toolSystem.GetSelectedIDs().size() > 1)
		{
			// Define object ID to focus on
			focusID = m_lightDialogue.GetFocusDialogue()->GetSelectedIndex();
		}

		// Else, if just one object is selected
		else if (m_toolSystem.GetSelectedIDs().size() == 1)
		{
			// Define object ID to focus on
			focusID = m_toolSystem.GetSelectedIDs()[0];
		}

		// Setup object ID to focus on
		m_toolSystem.SetFocus(focusID);
	}
	else { m_toolSystem.SetFocus(-1); }
	
	// If light function isn't up-to-date
	if (m_toolSystem.GetObjectFunction() != m_lightDialogue.GetFunction())
	{
		// Set light function
		m_toolSystem.SetObjectFunction(m_lightDialogue.GetFunction());
	}

	// If constraint isn't up-to-date
	if (m_toolSystem.GetConstraint() != m_lightDialogue.GetConstraint())
	{
		// Set constraint
		m_toolSystem.SetObjectConstraint(m_lightDialogue.GetConstraint());
	}
	
	/////////////////////////////////////////////////////////////////////////////////////

	// If scene graph should be updated
	//if (m_lightDialogue.GetUpdate())
	//{
	//	// Get edited lights
	//	std::vector<DisplayObject> lights = m_lightDialogue.GetLights();

	//	// Get scene graph
	//	std::vector<SceneObject> sceneGraph = m_toolSystem.GetSceneGraph();

	//	// Loop through scene graph
	//	for (int i = 0; i < sceneGraph.size(); ++i)
	//	{
	//		// Loop through lights
	//		for (int j = 0; j < lights.size(); ++j)
	//		{
	//			// If scene graph ID matches light ID
	//			if (sceneGraph[i].ID == lights[j].m_ID)
	//			{
	//				// Store current scene object
	//				SceneObject object = sceneGraph[i];

	//				// Setup object details from matching light
	//				{
	//					object.light_diffuse_r = lights[j].GetDiffuse().x;
	//					object.light_diffuse_g = lights[j].GetDiffuse().y;
	//					object.light_diffuse_b = lights[j].GetDiffuse().z;
	//					object.posX = lights[j].GetPosition().x;
	//					object.posY = lights[j].GetPosition().y;
	//					object.posZ = lights[j].GetPosition().z;
	//					object.rotX = lights[j].GetDirection().x;
	//					object.rotY = lights[j].GetDirection().y;
	//					object.rotZ = lights[j].GetDirection().z;
	//					object.light_type = (int)lights[j].GetLightType();
	//					object.light_constant = lights[j].GetConstantAttenuation();
	//					object.light_linear = lights[j].GetLinearAttenuation();
	//					object.light_quadratic = lights[j].GetQuadraticAttenuation();
	//					object.enabled = lights[j].GetEnabled();
	//					object.ambR = lights[j].GetAmbient().x;
	//					object.ambG = lights[j].GetAmbient().y;
	//					object.ambB = lights[j].GetAmbient().z;
	//				}

	//				// Replace scene object with new light	
	//				m_toolSystem.SetSceneObject(object, i);
	//			}
	//		}
	//	}

	//	// Reset update controller
	//	m_lightDialogue.SetUpdate(false);
	//}
}

void MFCMain::UpdateObjects()
{		
	// If dialogue needs updated data
	if (m_objectDialogue.GetRequest())/// || m_toolSystem.GetRequest())
	{
		// Send updated data
		m_objectDialogue.SetObjectData(&m_toolSystem.GetSceneGraph(), m_toolSystem.GetSelectedIDs());
	}
	
	// If ToolMain selected objects should be updated
	if (m_objectDialogue.GetSelect()) { m_toolSystem.SetSelectedObjectIDs(m_objectDialogue.GetSelectedIDs()); }
	// Else, update dialogue selected objects
	else { m_objectDialogue.SetSelectedIDs(m_toolSystem.GetSelectedIDs()); }

	// If there's been a new selection
	if (m_toolSystem.GetNewSelection())
	{
		// Update dialogue entries to match selected object
		m_objectDialogue.Update();
	}
	
	// If should focus on an object
	if (m_objectDialogue.GetFocus())
	{
		// Setup temp focus ID
		int focusID = -1;

		// If more than one object is selected
		///if (m_objectDialogue.GetSelectedObjectIDs().size() > 1)
		if (m_toolSystem.GetSelectedIDs().size() > 1)
		{
			// Define object ID to focus on
			focusID = m_objectDialogue.GetFocusDialogue()->GetSelectedIndex();
		}

		// Else, if just one object is selected
		///else if (m_objectDialogue.GetSelectedObjectIDs().size() == 1)
		else if (m_toolSystem.GetSelectedIDs().size() == 1)
		{
			// Define object ID to focus on
			focusID = m_toolSystem.GetSelectedIDs()[0];
		}

		// Setup object ID to focus on
		m_toolSystem.SetFocus(focusID);
	}
	else { m_toolSystem.SetFocus(-1); }
	
	// If object function isn't up-to-date
	if (m_toolSystem.GetObjectFunction() != m_objectDialogue.GetFunction())
	{
		// Set object function
		m_toolSystem.SetObjectFunction(m_objectDialogue.GetFunction());
	}

	// If constraint isn't up-to-date
	if (m_toolSystem.GetConstraint() != m_objectDialogue.GetConstraint())
	{
		// Set constraint
		m_toolSystem.SetObjectConstraint(m_objectDialogue.GetConstraint());
	}	
}

void MFCMain::UpdateSpawn()
{
	// If spawn mode isn't up-to-date
	if (m_toolSystem.GetObjectSpawn() != m_spawnDialogue.GetSpawnType())
	{
		// Set spawn mode
		m_toolSystem.SetObjectSpawn(m_spawnDialogue.GetSpawnType());
	}	
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
	// If paint mode isn't up-to-date
	if (m_toolSystem.GetTerrainPaint() != m_paintDialogue.GetPaint())
	{
		// Set paint mode
		m_toolSystem.SetTerrainPaint(m_paintDialogue.GetPaint());
	}
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

// Handle dialogue activations //////////////////////////////////////////

void MFCMain::CheckDialogues(DIALOGUE_TYPE type)
{
	// Switch between type
	switch (type)
	{
	case DIALOGUE_TYPE::LIGHT:
	{		
		// Check if object dialogue is active
		if (m_object) {
			if (m_objectDialogue.GetActive()) {
				m_objectDialogue.SetActive(false);
				m_objectDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if spawn dialogue is active
		if (m_spawn) {
			if (m_spawnDialogue.GetActive()) {
				m_spawnDialogue.SetActive(false);
				m_spawnDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if terrain dialogue is active
		if (m_terrain) {
			if (m_terrainDialogue.GetActive()) {
				m_terrainDialogue.SetActive(false);
				m_terrainDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if paint dialogue is active
		if (m_paint) {
			if (m_paintDialogue.GetActive()) {
				m_paintDialogue.SetActive(false);
				m_paintDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if sculpt dialogue is active
		if (m_sculpt) {
			if (m_sculptDialogue.GetActive()) {
				m_sculptDialogue.SetActive(false);
				m_sculptDialogue.ShowWindow(SW_HIDE);
			}
		}
	}
	break;
	case DIALOGUE_TYPE::OBJECT:
	{
		// Check if light dialogue is active
		if (m_light) {
			if (m_lightDialogue.GetActive()) {
				m_lightDialogue.SetActive(false);
				m_lightDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if spawn dialogue is active
		if (m_spawn) {
			if (m_spawnDialogue.GetActive()) {
				m_spawnDialogue.SetActive(false);
				m_spawnDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if terrain dialogue is active
		if (m_terrain) {
			if (m_terrainDialogue.GetActive()) {
				m_terrainDialogue.SetActive(false);
				m_terrainDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if paint dialogue is active
		if (m_paint) {
			if (m_paintDialogue.GetActive()) {
				m_paintDialogue.SetActive(false);
				m_paintDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if sculpt dialogue is active
		if (m_sculpt) {
			if (m_sculptDialogue.GetActive()) {
				m_sculptDialogue.SetActive(false);
				m_sculptDialogue.ShowWindow(SW_HIDE);
			}
		}
	}
	break;
	case DIALOGUE_TYPE::SPAWN:
	{
		// Check if light dialogue is active
		if (m_light) {
			if (m_lightDialogue.GetActive()) {
				m_lightDialogue.SetActive(false);
				m_lightDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if object dialogue is active
		if (m_object) {
			if (m_objectDialogue.GetActive()) {
				m_objectDialogue.SetActive(false);
				m_objectDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if terrain dialogue is active
		if (m_terrain) {
			if (m_terrainDialogue.GetActive()) {
				m_terrainDialogue.SetActive(false);
				m_terrainDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if paint dialogue is active
		if (m_paint) {
			if (m_paintDialogue.GetActive()) {
				m_paintDialogue.SetActive(false);
				m_paintDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if sculpt dialogue is active
		if (m_sculpt) {
			if (m_sculptDialogue.GetActive()) {
				m_sculptDialogue.SetActive(false);
				m_sculptDialogue.ShowWindow(SW_HIDE);
			}
		}
	}
	break;
	case DIALOGUE_TYPE::TERRAIN:
	{
		// Check if light dialogue is active
		if (m_light) {
			if (m_lightDialogue.GetActive()) {
				m_lightDialogue.SetActive(false);
				m_lightDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if object dialogue is active
		if (m_object) {
			if (m_objectDialogue.GetActive()) {
				m_objectDialogue.SetActive(false);
				m_objectDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if spawn dialogue is active
		if (m_spawn) {
			if (m_spawnDialogue.GetActive()) {
				m_spawnDialogue.SetActive(false);
				m_spawnDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if paint dialogue is active
		if (m_paint) {
			if (m_paintDialogue.GetActive()) {
				m_paintDialogue.SetActive(false);
				m_paintDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if sculpt dialogue is active
		if (m_sculpt) {
			if (m_sculptDialogue.GetActive()) {
				m_sculptDialogue.SetActive(false);
				m_sculptDialogue.ShowWindow(SW_HIDE);
			}
		}
	}
	break;
	case DIALOGUE_TYPE::PAINT:
	{
		// Check if light dialogue is active
		if (m_light) {
			if (m_lightDialogue.GetActive()) {
				m_lightDialogue.SetActive(false);
				m_lightDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if object dialogue is active
		if (m_object) {
			if (m_objectDialogue.GetActive()) {
				m_objectDialogue.SetActive(false);
				m_objectDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if spawn dialogue is active
		if (m_spawn) {
			if (m_spawnDialogue.GetActive()) {
				m_spawnDialogue.SetActive(false);
				m_spawnDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if terrain dialogue is active
		if (m_terrain) {
			if (m_terrainDialogue.GetActive()) {
				m_terrainDialogue.SetActive(false);
				m_terrainDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if sculpt dialogue is active
		if (m_sculpt) {
			if (m_sculptDialogue.GetActive()) {
				m_sculptDialogue.SetActive(false);
				m_sculptDialogue.ShowWindow(SW_HIDE);
			}
		}
	}
	break;
	case DIALOGUE_TYPE::SCULPT:
	{
		// Check if light dialogue is active
		if (m_light) {
			if (m_lightDialogue.GetActive()) {
				m_lightDialogue.SetActive(false);
				m_lightDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if object dialogue is active
		if (m_object) {
			if (m_objectDialogue.GetActive()) {
				m_objectDialogue.SetActive(false);
				m_objectDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if spawn dialogue is active
		if (m_spawn) {
			if (m_spawnDialogue.GetActive()) {
				m_spawnDialogue.SetActive(false);
				m_spawnDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if terrain dialogue is active
		if (m_terrain) {
			if (m_terrainDialogue.GetActive()) {
				m_terrainDialogue.SetActive(false);
				m_terrainDialogue.ShowWindow(SW_HIDE);
			}
		}

		// Check if paint dialogue is active
		if (m_paint) {
			if (m_paintDialogue.GetActive()) {
				m_paintDialogue.SetActive(false);
				m_paintDialogue.ShowWindow(SW_HIDE);
			}
		}
	}
	break;
	}
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
	// Set other modes to none
	m_toolSystem.SetObjectSpawn(OBJECT_TYPE::NA);
	m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
	m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

	// Set tool editor
	m_toolSystem.SetEditor(EDITOR::LIGHTS);
	
	// Destroy other windows
	CheckDialogues(DIALOGUE_TYPE::LIGHT);

	// Create & display dialogue window	
	if (!m_light) { m_lightDialogue.Create(IDD_DIALOG8); m_light = true; }
	m_lightDialogue.ShowWindow(SW_SHOW);
	m_lightDialogue.SetActive(true);
	///m_lightDialogue.SetLightData(&m_toolSystem.GetDisplayList());
	m_lightDialogue.SetLightData(&m_toolSystem.GetSceneGraph(), m_toolSystem.GetSelectedIDs());
	
	// Setup temp light ID container
	//std::vector<int> IDs;

	//// Loop through currently selected objects
	//for (int i = 0; i < m_toolSystem.GetSelectedObjectIDs().size(); ++i)
	//{
	//	// If object is a light
	//	if (m_toolSystem.GetSceneGraph()[m_toolSystem.GetSelectedObjectIDs()[i]].m_type == OBJECT_TYPE::LIGHT)
	//	{
	//		// Add ID to container
	//		IDs.push_back(m_toolSystem.GetSceneGraph()[m_toolSystem.GetSelectedObjectIDs()[i]].ID);
	//	}
	//}

	//// Set current selection
	/////m_lightDialogue.SetLightData(&m_toolSystem.GetLights(), IDs);
	//m_lightDialogue.SetSelectedLightIDs(IDs);
}

void MFCMain::ToolBarObject()
{
	// Set other modes to none
	m_toolSystem.SetObjectSpawn(OBJECT_TYPE::NA);
	m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
	m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

	// Set tool editor
	m_toolSystem.SetEditor(EDITOR::OBJECT_FUNCTION);
	
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::OBJECT);

	// Create & display dialogue window
	if (!m_object) { m_objectDialogue.Create(IDD_DIALOG12); m_object = true; }
	m_objectDialogue.ShowWindow(SW_SHOW);
	m_objectDialogue.SetActive(true);
	m_objectDialogue.SetObjectData(&m_toolSystem.GetSceneGraph(), m_toolSystem.GetSelectedIDs());
	///m_objectDialogue.SetToolSystem(&m_toolSystem);
}

void MFCMain::ToolBarSpawn()
{
	// Set other modes to none
	m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
	m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);
	m_toolSystem.SetTerrainPaint(TERRAIN_PAINT::NA);

	// Set tool editor
	m_toolSystem.SetEditor(EDITOR::OBJECT_SPAWN);
	
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::SPAWN);

	// Create & display dialogue window	
	if (!m_spawn) { m_spawnDialogue.Create(IDD_DIALOG13); m_spawn = true; }
	m_spawnDialogue.ShowWindow(SW_SHOW);
	m_spawnDialogue.SetActive(true);
	m_spawnDialogue.Setup();
}

void MFCMain::ToolBarTerrain()
{
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::TERRAIN);

	// Create & display dialogue window	
	if (!m_terrain) { m_terrainDialogue.Create(IDD_DIALOG9); m_terrain = true; }
	m_terrainDialogue.ShowWindow(SW_SHOW);
	m_terrainDialogue.SetActive(true);
	m_terrainDialogue.SetChunkData(m_toolSystem.GetDisplayChunk());
}

void MFCMain::ToolBarPaint()
{
	// Set other modes to none
	m_toolSystem.SetObjectSpawn(OBJECT_TYPE::NA);
	m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);
	m_toolSystem.SetTerrainSculpt(TERRAIN_SCULPT::NA);

	// Set tool editor
	m_toolSystem.SetEditor(EDITOR::TERRAIN_PAINT);
	
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::PAINT);

	// Create & display dialogue window	
	if (!m_paint) { m_paintDialogue.Create(IDD_DIALOG10); m_paint = true; }
	m_paintDialogue.ShowWindow(SW_SHOW);
	m_paintDialogue.SetActive(true);
	m_paintDialogue.SetChunkData(m_toolSystem.GetDisplayChunk());
}

void MFCMain::ToolBarSculpt()
{
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::SCULPT);

	// Create & display dialogue window	
	if (!m_sculpt) { m_sculptDialogue.Create(IDD_DIALOG11); m_sculpt = true; }
	m_sculptDialogue.ShowWindow(SW_SHOW);
	m_sculptDialogue.SetActive(true);
	m_sculptDialogue.SetChunkData(m_toolSystem.GetDisplayChunk());
}
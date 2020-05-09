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
	ON_COMMAND(ID_SHADER_NORMAL, &MFCMain::MenuEditShaderNormal)
	ON_COMMAND(ID_SHADER_CARTOON, &MFCMain::MenuEditShaderCartoon)
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
				// If ID is valid
				if (IDs[i] != -1)
				{
					if (i != 0) { statusString += L", " + std::to_wstring(IDs[i]); }
					else { statusString += std::to_wstring(IDs[i]); }
				}
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
	if (m_lightDialogue.GetActive()) { m_lightDialogue.UpdateTool(); }

	// Else, if object inspector is active
	else if (m_objectDialogue.GetActive()) { m_objectDialogue.UpdateTool(); }

	// Else, if spawn inspector is active
	else if (m_spawnDialogue.GetActive()) { m_spawnDialogue.UpdateTool(); }

	// Else, if terrain inspector is active
	else if (m_terrainDialogue.GetActive()) { m_terrainDialogue.UpdateTool(); }

	// Else, if paint inspector is active
	else if (m_paintDialogue.GetActive()) { m_paintDialogue.UpdateTool(); }

	// Else, if sculpt inspector is active
	else if (m_sculptDialogue.GetActive()) { m_sculptDialogue.UpdateTool(); }
	
	// Else, if no dialogues are active
	else
	{
		// Set tool editor
		m_toolSystem.SetEditor(EDITOR_COMPLEX::NA);

		// Set all modes to none
		m_toolSystem.SetObjectSpawn(OBJECT_TYPE::NA);
		m_toolSystem.SetObjectFunction(OBJECT_FUNCTION::NA);

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

void MFCMain::MenuEditShaderNormal()
{
	m_toolSystem.GetDisplayChunk()->SetShaderType(true);
}

void MFCMain::MenuEditShaderCartoon()
{
	m_toolSystem.GetDisplayChunk()->SetShaderType(false);
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
	CheckDialogues(DIALOGUE_TYPE::LIGHT);

	// Create & display dialogue window	
	if (!m_light) { m_lightDialogue.Create(IDD_DIALOG8); m_light = true; }
	m_lightDialogue.ShowWindow(SW_SHOW);
	m_lightDialogue.SetActive(true);
	m_lightDialogue.SetToolData(&m_toolSystem);
}

void MFCMain::ToolBarObject()
{	
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::OBJECT);

	// Create & display dialogue window
	if (!m_object) { m_objectDialogue.Create(IDD_DIALOG12); m_object = true; }
	m_objectDialogue.ShowWindow(SW_SHOW);
	m_objectDialogue.SetActive(true);
	m_objectDialogue.SetToolSystem(&m_toolSystem);
}

void MFCMain::ToolBarSpawn()
{	
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::SPAWN);

	// Create & display dialogue window	
	if (!m_spawn) { m_spawnDialogue.Create(IDD_DIALOG13); m_spawn = true; }
	m_spawnDialogue.ShowWindow(SW_SHOW);
	m_spawnDialogue.SetActive(true);
	m_spawnDialogue.SetToolData(&m_toolSystem);
}

void MFCMain::ToolBarTerrain()
{
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::TERRAIN);

	// Create & display dialogue window	
	if (!m_terrain) { m_terrainDialogue.Create(IDD_DIALOG9); m_terrain = true; }
	m_terrainDialogue.ShowWindow(SW_SHOW);
	m_terrainDialogue.SetActive(true);
	m_terrainDialogue.SetToolData(&m_toolSystem);
}

void MFCMain::ToolBarPaint()
{	
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::PAINT);

	// Create & display dialogue window	
	if (!m_paint) { m_paintDialogue.Create(IDD_DIALOG10); m_paint = true; }
	m_paintDialogue.ShowWindow(SW_SHOW);
	m_paintDialogue.SetActive(true);
	m_paintDialogue.SetToolData(&m_toolSystem);
}

void MFCMain::ToolBarSculpt()
{
	// Destroy other windows 
	CheckDialogues(DIALOGUE_TYPE::SCULPT);

	// Create & display dialogue window	
	if (!m_sculpt) { m_sculptDialogue.Create(IDD_DIALOG11); m_sculpt = true; }
	m_sculptDialogue.ShowWindow(SW_SHOW);
	m_sculptDialogue.SetActive(true);
	m_sculptDialogue.SetToolData(&m_toolSystem);
}


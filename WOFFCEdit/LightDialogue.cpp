// LightDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "LightDialogue.h"
#include "afxdialogex.h"


// LightDialogue dialog

IMPLEMENT_DYNAMIC(LightDialogue, CDialogEx)

LightDialogue::LightDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{
	// Update tool controllers
	m_function = OBJECT_FUNCTION::SELECT;
	m_constraint = CONSTRAINT::NA;
}

void LightDialogue::SetToolData(ToolMain * toolSystem)
{
	// Local storage
	m_toolSystem = toolSystem;
	m_selectedIDs = toolSystem->GetSelectedIDs();	

	// Setup IDs of currently available lights
	SetupLights(&toolSystem->GetSceneGraph());

	// Setup light types
	{
		std::wstring typeBoxEntry;
		typeBoxEntry = L"Directional";	m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Point";		m_boxType.AddString(typeBoxEntry.c_str());
	}

	// Setup constraint types
	{
		std::wstring constBoxEntry;
		constBoxEntry = L"N/A";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"X";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"Y";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"Z";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"XY";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"XZ";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"YZ";			m_boxConst.AddString(constBoxEntry.c_str());
	}

	UpdateDialogue();

	// Set other modes to none
	m_toolSystem->SetObjectSpawn(OBJECT_TYPE::NA);

	// Set tool editor
	m_toolSystem->SetEditor(EDITOR_COMPLEX::LIGHTS);
}

void LightDialogue::UpdateTool()
{	
	// If selected IDs should be updated
	if (m_select) { 
		m_toolSystem->SetSelectedIDs(m_selectedIDs); 
		m_select = false; 
	}
	else { m_selectedIDs = m_toolSystem->GetSelectedIDs(); }
	
	// If there's been a new selection
	if (m_toolSystem->GetNewSelection()) { 
		m_selectedIndex = MouseManager::GetIndex();
		UpdateDialogue();
	}

	// If the dialogue needs reset (after deleting or copying/pasting objects via key presses)
	if (m_toolSystem->GetUpdate()) {
		SetupLights(&m_toolSystem->GetSceneGraph());
	}
	
	// If should focus on an object
	if (m_focus)
	{
		// Setup temp focus ID
		int focusID = -1;

		// If more than one object is selected
		if (m_toolSystem->GetSelectedIDs().size() > 1)
		{
			// Define object ID to focus on
			focusID = m_focusDialogue.GetSelectedIndex();
		}

		// Else, if just one object is selected
		else if (m_toolSystem->GetSelectedIDs().size() == 1)
		{
			// Define object ID to focus on
			focusID = m_toolSystem->GetSelectedIDs()[0];
		}

		// Setup object ID to focus on
		m_toolSystem->SetFocus(focusID);
	}
	else { m_toolSystem->SetFocus(-1); }

	// If tool function isn't up-to-date
	if (m_toolSystem->GetObjectFunction() != m_function) { m_toolSystem->SetObjectFunction(m_function); }
	
	// If tool constraint isn't up-to-date
	if (m_toolSystem->GetConstraint() != m_constraint) { m_toolSystem->SetConstraint(m_constraint); }	
	
	// Update light details
	if (m_function == OBJECT_FUNCTION::TRANSLATE) {
		m_internal = true;
		UpdatePosition();
		m_internal = false;
	}
}

void LightDialogue::UpdateDialogue(int ID)
{
	// If ID is valid
	if (ID != -1) { m_boxID.SetCurSel(ID); }
	else { m_internal = true; }

	if (m_selectedIDs.size() == 1) { m_boxID.SetCurSel(0); }
	UpdateType();
	UpdateEnabled();
	UpdatePosition();
	UpdateDiffuse();
	UpdateAmbient();
	UpdateConstA();
	UpdateLinA();
	UpdateQuadA();

	m_internal = false;
}

void LightDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_boxID);

	DDX_Control(pDX, IDC_COMBO2, m_boxType);
	DDX_Control(pDX, IDC_COMBO6, m_boxConst);

	DDX_Control(pDX, IDC_EDIT1, m_ePosX);		
	DDX_Control(pDX, IDC_EDIT2, m_ePosY);		
	DDX_Control(pDX, IDC_EDIT3, m_ePosZ);		

	DDX_Control(pDX, IDC_EDIT7, m_eDifR);		
	DDX_Control(pDX, IDC_EDIT8, m_eDifG);		
	DDX_Control(pDX, IDC_EDIT9, m_eDifB);		

	DDX_Control(pDX, IDC_EDIT10, m_eAmbR);		
	DDX_Control(pDX, IDC_EDIT11, m_eAmbG);		
	DDX_Control(pDX, IDC_EDIT12, m_eAmbB);		

	DDX_Control(pDX, IDC_EDIT13, m_eConstA);	
	DDX_Control(pDX, IDC_EDIT14, m_eLinA);		
	DDX_Control(pDX, IDC_EDIT15, m_eQuadA);		
}

void LightDialogue::SetupCheckBoxes()
{
	// X constraint
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP10);
	((CButton*)GetDlgItem(IDC_CHECK23))->SetBitmap(bmp);
	bmp.Detach();

	// Y constraint
	bmp.LoadBitmap(IDB_BITMAP11);
	((CButton*)GetDlgItem(IDC_CHECK24))->SetBitmap(bmp);
	bmp.Detach();

	// Z constraint
	bmp.LoadBitmap(IDB_BITMAP12);
	((CButton*)GetDlgItem(IDC_CHECK25))->SetBitmap(bmp);
	bmp.Detach();
}

BEGIN_MESSAGE_MAP(LightDialogue, CDialogEx)
	ON_COMMAND(IDOK, &LightDialogue::End)
	ON_COMMAND(IDCANCEL, &LightDialogue::End)
	ON_BN_CLICKED(IDOK, &LightDialogue::OnBnClickedOk)

	ON_CBN_SELCHANGE(IDC_LIST1, &LightDialogue::OnCbnSelchangeID)
	ON_CBN_SELCHANGE(IDC_COMBO2, &LightDialogue::OnCbnSelchangeType)

	ON_BN_CLICKED(IDC_CHECK1, &LightDialogue::OnBnClickedEnable)
	ON_BN_CLICKED(IDC_CHECK4, &LightDialogue::OnBnClickedFocus)

	ON_EN_CHANGE(IDC_EDIT1, &LightDialogue::OnEnChangePosX)
	ON_EN_CHANGE(IDC_EDIT2, &LightDialogue::OnEnChangePosY)
	ON_EN_CHANGE(IDC_EDIT3, &LightDialogue::OnEnChangePosZ)

	ON_EN_CHANGE(IDC_EDIT7, &LightDialogue::OnEnChangeDifR)
	ON_EN_CHANGE(IDC_EDIT8, &LightDialogue::OnEnChangeDifG)
	ON_EN_CHANGE(IDC_EDIT9, &LightDialogue::OnEnChangeDifB)

	ON_EN_CHANGE(IDC_EDIT10, &LightDialogue::OnEnChangeAmbR)
	ON_EN_CHANGE(IDC_EDIT11, &LightDialogue::OnEnChangeAmbG)
	ON_EN_CHANGE(IDC_EDIT12, &LightDialogue::OnEnChangeAmbB)

	ON_EN_CHANGE(IDC_EDIT13, &LightDialogue::OnEnChangeConstA)
	ON_EN_CHANGE(IDC_EDIT14, &LightDialogue::OnEnChangeLinA)
	ON_EN_CHANGE(IDC_EDIT15, &LightDialogue::OnEnChangeQuadA)

	ON_BN_CLICKED(IDC_BUTTON1, &LightDialogue::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON2, &LightDialogue::OnBnClickedDuplicate)
	ON_BN_CLICKED(IDC_CHECK2, &LightDialogue::OnBnClickedTranslate)

	ON_BN_CLICKED(IDC_CHECK23, &LightDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK24, &LightDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK25, &LightDialogue::OnBnClickedZ)
END_MESSAGE_MAP()

// Kill the dialogue
void LightDialogue::End()
{
	Reset();
	ShowWindow(SW_HIDE);
}

// User is finished with dialogue
void LightDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// ID has been selected
void LightDialogue::OnCbnSelchangeID()
{		
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Get text selection
	CString current;
	m_boxID.GetText(ID, current);

	// Controller to check if selection exists
	bool exists = false;
	
	// Loop through selection
	for (int i = 0; i < m_selectedIDs.size(); ++i)
	{
		// If current already exists
		if (m_selectedIDs[i] == _ttoi(current))
		{
			// Inform remaining functionality the selection exists
			exists = true;
			
			// Remove from selection
			m_selectedIDs.erase(m_selectedIDs.begin() + i);
		}
	}

	// If selection doesn't exist, add to selection
	if (!exists) { m_selectedIDs.push_back(_ttoi(current)); }
	
	m_select = m_internal = true;

	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{
		// Loop through scene graph
		for (int i = 0; i < m_toolSystem->GetSceneGraph().size(); ++i)
		{
			// If IDs match
			if (m_toolSystem->GetSceneGraph()[i].ID == m_selectedIDs[0])
			{
				// Set selected index
				m_selectedIndex = i;
				break;
			}
		}
	}
	
	// Update light type box
	UpdateType();

	// Update enabled box
	UpdateEnabled();

	// Update position boxes
	UpdatePosition();

	// Update diffuse boxes
	UpdateDiffuse();

	// Update ambient boxes
	UpdateAmbient();

	// Update attenuation boxes
	UpdateConstA(); UpdateLinA(); UpdateQuadA();

	m_internal = false;
}

// Type has been changed
void LightDialogue::OnCbnSelchangeType()
{	
	// If function has been called by user
	if (!m_internal)
	{		
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store type selection
					int type = m_boxType.GetCurSel();

					// If type is different from current type
					if (m_lights->at(j).light_type != type)
					{
						// Set new light type
						m_lights->at(j).light_type = type;
						UpdateLight(m_lights->at(j));
					}						
						
					break;
				}
			}
		}		
	}
}

// Light has been enabled/disabled
void LightDialogue::OnBnClickedEnable()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// If enabled is different from current enabled
					if (m_lights->at(j).enabled != IsDlgButtonChecked(IDC_CHECK1))
					{
						// Set light enabled/disabled
						m_lights->at(j).enabled = IsDlgButtonChecked(IDC_CHECK1);
						UpdateLight(m_lights->at(j));
					}
					break;
				}
			}
		}
		
	}
}

// Light has been focussed/unfocussed
void LightDialogue::OnBnClickedFocus()
{
	// Set focus to match button state
	m_focus = IsDlgButtonChecked(IDC_CHECK4);

	// If focus is checked
	if (m_focus)
	{
		// If there's more than one object selected
		if (m_selectedIDs.size() > 1)
		{
			// Setup focus modal dialogue
			m_focusDialogue.Create(IDD_DIALOG2);
			m_focusDialogue.ShowWindow(SW_SHOW);
			m_focusDialogue.SetActive(true);
			m_focusDialogue.SetFocusData(&m_selectedIDs);
		}
	}	
}

// X position has been changed
void LightDialogue::OnEnChangePosX()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new X position
					CString string = _T("");
					m_ePosX.GetWindowTextW(string);

					// Convert to float
					float posX;
					if (!string.IsEmpty()) { posX = _ttof(string); }
					else { posX = m_lights->at(j).posX; }

					// If position is different from current position
					if (m_lights->at(j).posX != posX)
					{
						// Update X position of light
						m_lights->at(j).posX = posX;
						UpdateLight(m_lights->at(j));
					}						

					break;
				}
			}
		}		
	}
}

// Y position has been changed
void LightDialogue::OnEnChangePosY()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new Y position
					CString string = _T("");
					m_ePosY.GetWindowTextW(string);

					// Convert to float
					float posY;
					if (!string.IsEmpty()) { posY = _ttof(string); }
					else { posY = m_lights->at(j).posY; }

					// If position is different from current position
					if (m_lights->at(j).posY != posY)
					{
						// Update Y position of light
						m_lights->at(j).posY = posY;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// Z position has been changed
void LightDialogue::OnEnChangePosZ()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new Z position
					CString string = _T("");
					m_ePosZ.GetWindowTextW(string);

					// Convert to float
					float posZ;
					if (!string.IsEmpty()) { posZ = _ttof(string); }
					else { posZ = m_lights->at(j).posZ; }

					// If position is different from current position
					if (m_lights->at(j).posZ != posZ)
					{
						// Update Z position of light
						m_lights->at(j).posZ = posZ;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// R diffuse has been changed
void LightDialogue::OnEnChangeDifR()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new R diffuse
					CString string = _T("");
					m_eDifR.GetWindowTextW(string);

					// Convert to float
					float difR;
					if (!string.IsEmpty()) { difR = _ttof(string); }
					else { difR = m_lights->at(j).light_diffuse_r; }

					// If diffuse is different from current diffuse
					if (m_lights->at(j).light_diffuse_r != difR)
					{
						// Update R diffuse of light
						m_lights->at(j).light_diffuse_r = difR;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// G diffuse has been changed
void LightDialogue::OnEnChangeDifG()
{
	// If function has been called by user
	if (!m_internal)
	{		
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new G diffuse
					CString string = _T("");
					m_eDifG.GetWindowTextW(string);

					// Convert to float
					float difG;
					if (!string.IsEmpty()) { difG = _ttof(string); }
					else { difG = m_lights->at(j).light_diffuse_g; }

					// If diffuse is different from current diffuse
					if (m_lights->at(j).light_diffuse_g != difG)
					{
						// Update G diffuse of light
						m_lights->at(j).light_diffuse_g = difG;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// B diffuse has been changed
void LightDialogue::OnEnChangeDifB()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new B diffuse
					CString string = _T("");
					m_eDifB.GetWindowTextW(string);

					// Convert to float
					float difB;
					if (!string.IsEmpty()) { difB = _ttof(string); }
					else { difB = m_lights->at(j).light_diffuse_b; }

					// If diffuse is different from current diffuse
					if (m_lights->at(j).light_diffuse_b != difB)
					{
						// Update B diffuse of light
						m_lights->at(j).light_diffuse_b = difB;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// R ambient has been changed
void LightDialogue::OnEnChangeAmbR()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new R ambient
					CString string = _T("");
					m_eAmbR.GetWindowText(string);

					// Convert to float
					float ambR;
					if (!string.IsEmpty()) { ambR = _ttof(string); }
					else { ambR = m_lights->at(j).light_ambient_r; }

					// If ambient is different from current ambient
					if (m_lights->at(j).light_ambient_r != ambR)
					{
						// Update R ambient of light
						m_lights->at(j).light_ambient_r = ambR;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// G ambient has been changed
void LightDialogue::OnEnChangeAmbG()
{	
	// If function has been called by user
	if (!m_internal)
	{		
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new G ambient
					CString string = _T("");
					m_eAmbG.GetWindowText(string);

					// Convert to float
					float ambG;
					if (!string.IsEmpty()) { ambG = _ttof(string); }
					else { ambG = m_lights->at(j).light_ambient_g; }						

					// If ambient is different from current ambient
					if (m_lights->at(j).light_ambient_g != ambG)
					{
						// Update G ambient of light
						m_lights->at(j).light_ambient_g = ambG;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// B ambient has been changed
void LightDialogue::OnEnChangeAmbB()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new B ambient
					CString string = _T("");
					m_eAmbB.GetWindowText(string);

					// Convert to float
					float ambB;
					if (!string.IsEmpty()) { ambB = _ttof(string); }
					else { ambB = m_lights->at(j).light_ambient_b; }						

					// If ambient is different from current ambient
					if (m_lights->at(j).light_ambient_b != ambB)
					{
						// Update B ambient of light
						m_lights->at(j).light_ambient_b = ambB;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// Constant attenuation has been changed
void LightDialogue::OnEnChangeConstA()
{
	// If function has been called by user
	if (!m_internal)
	{	
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new constant attenuation
					CString string = _T("");
					m_eConstA.GetWindowTextW(string);

					// Convert to float
					float constA;
					if (!string.IsEmpty()) { constA = _ttof(string); }
					else { constA = m_lights->at(j).light_constant; }

					// If constant attenuation is different from current constant attenuation
					if (m_lights->at(j).light_constant != constA)
					{
						// Update constant attenuation of light
						m_lights->at(j).light_constant = constA;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// Linear attenuation has been changed
void LightDialogue::OnEnChangeLinA()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new linear attenuation
					CString string = _T("");
					m_eLinA.GetWindowTextW(string);

					// Convert to float
					float linA;
					if (!string.IsEmpty()) { linA = _ttof(string); }
					else { linA = m_lights->at(j).light_linear; }

					// If linear attenuation is different from current linear attenuation
					if (m_lights->at(j).light_linear != linA)
					{
						// Update linear attenuation of light
						m_lights->at(j).light_linear = linA;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}		
	}
}

// Quadratic attenuation has been changed
void LightDialogue::OnEnChangeQuadA()
{
	// If function has been called by user
	if (!m_internal)
	{
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through lights
			for (int j = 0; j < m_lights->size(); ++j)
			{
				// If light ID matches selection
				if (m_lights->at(j).ID == m_selectedIDs[i])
				{
					// Store new quadratic attenuation
					CString string = _T("");
					m_eQuadA.GetWindowTextW(string);

					// Convert to float
					float quadA;
					if (!string.IsEmpty()) { quadA = _ttof(string); }
					else { quadA = m_lights->at(j).light_quadratic; }

					// If quadratic attenuation is different from current quadratic attenuation
					if (m_lights->at(j).light_quadratic != quadA)
					{
						// Update quadratic attenuation of light
						m_lights->at(j).light_quadratic = quadA;
						UpdateLight(m_lights->at(j));
					}

					break;
				}
			}
		}
	}
}

// Delete has been selected
void LightDialogue::OnBnClickedDelete()
{
	// If selection is valid
	if (m_selectedIDs.size() > 0)
	{
		// Remove lights from database storage
		SetupLights(&ObjectManager::Delete(m_selectedIDs));
		m_previousIndex = m_selectedIndex;
	}
}

// Duplicate has been selected
void LightDialogue::OnBnClickedDuplicate()
{	
	// If selection is valid
	if (m_selectedIDs.size() != 0)
	{
		// Copy lights
		ObjectManager::Copy(m_selectedIDs);

		// Paste lights
		SetupLights(&ObjectManager::Paste());
	}
}

// Translate has been selected
void LightDialogue::OnBnClickedTranslate()
{
	m_translating = IsDlgButtonChecked(IDC_CHECK2);
	
	// Switch between translating
	switch (m_translating)
	{
	case true:
	{
		// Disable delete button
		GetDlgItem(IDC_BUTTON1)->EnableWindow(false);

		// Disable duplicate button
		GetDlgItem(IDC_BUTTON2)->EnableWindow(false);

		// Update function
		m_function = OBJECT_FUNCTION::TRANSLATE;
	}
	break;
	case false:
	{
		// Enable delete button
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);

		// Disable duplicate button
		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);

		// Update function
		m_function = OBJECT_FUNCTION::SELECT;
	}
	break;
	}

	// Update constraint text description
	UpdateSelectedConstraint();
}

// Constraint mode has been changed
void LightDialogue::OnBnSelchangeConstraint()
{
	// Store index of constraint value
	int index = m_boxConst.GetCurSel();

	// Set new constraint
	m_constraint = (CONSTRAINT)index;

	// Switch between constraint and check appropriate boxes
	switch (m_constraint)
	{
	case CONSTRAINT::X: CheckDlgButton(IDC_CHECK10, true); break;
	case CONSTRAINT::Y: CheckDlgButton(IDC_CHECK18, true); break;
	case CONSTRAINT::Z: CheckDlgButton(IDC_CHECK19, true); break;
	case CONSTRAINT::XY: CheckDlgButton(IDC_CHECK10, true); CheckDlgButton(IDC_CHECK18, true); break;
	case CONSTRAINT::XZ: CheckDlgButton(IDC_CHECK10, true); CheckDlgButton(IDC_CHECK19, true); break;
	case CONSTRAINT::YZ: CheckDlgButton(IDC_CHECK18, true); CheckDlgButton(IDC_CHECK19, true); break;
	case CONSTRAINT::NA: CheckDlgButton(IDC_CHECK10, true); CheckDlgButton(IDC_CHECK18, true); CheckDlgButton(IDC_CHECK19, true); break;
	}
}

// X constraint has been selected
void LightDialogue::OnBnClickedX()
{
	// Switch between checked/unchecked
	m_x = IsDlgButtonChecked(IDC_CHECK23);

	// Update constraint text description
	UpdateSelectedConstraint();
}

// Y constraint has been selected
void LightDialogue::OnBnClickedY()
{
	// Switch between checked/unchecked
	m_y = IsDlgButtonChecked(IDC_CHECK24);

	UpdateSelectedConstraint();
}

// Z constraint has been selected
void LightDialogue::OnBnClickedZ()
{
	// Switch between checked/unchecked
	m_z = IsDlgButtonChecked(IDC_CHECK25);

	UpdateSelectedConstraint();
}

// Update remaining light details when one is changed ///////////////////////////////////////////////

void LightDialogue::UpdateType()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{		
		// Set combo box
		m_boxType.SetCurSel(m_toolSystem->GetSceneGraph()[m_selectedIndex].light_type);			
	}	

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_boxType.SetCurSel(0);
		m_internal = false;
	}
}

void LightDialogue::UpdateEnabled()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{	
		// Set enabled check box to match light 
		CheckDlgButton(IDC_CHECK1, m_toolSystem->GetSceneGraph()[m_selectedIndex].enabled);		
	}	

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		CheckDlgButton(IDC_CHECK1, false);
		m_internal = false;
	}
}

void LightDialogue::UpdatePosition()
{
	// If only one object is selected and index is valid
	if (m_selectedIDs.size() == 1 && m_previousIndex != m_selectedIndex)
	{		
		// Update X position box
		CString sX; sX.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].posX);
		m_ePosX.SetWindowTextW(sX);

		// Update Y position box
		CString sY; sY.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].posY);
		m_ePosY.SetWindowTextW(sY);

		// Update Z position box
		CString sZ; sZ.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].posZ);
		m_ePosZ.SetWindowTextW(sZ);	
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_ePosX.SetWindowTextW(empty);
		m_ePosY.SetWindowTextW(empty);
		m_ePosZ.SetWindowTextW(empty);
		m_internal = false;
	}
}

void LightDialogue::UpdateDiffuse()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{	
		// Update R diffuse box
		CString sR; sR.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_diffuse_r);
		m_eDifR.SetWindowTextW(sR);

		// Update G diffuse box
		CString sG; sG.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_diffuse_g);
		m_eDifG.SetWindowTextW(sG);

		// Update B diffuse box
		CString sB; sB.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_diffuse_b);
		m_eDifB.SetWindowTextW(sB);
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_eDifR.SetWindowTextW(empty);
		m_eDifG.SetWindowTextW(empty);
		m_eDifB.SetWindowTextW(empty);
		m_internal = false;
	}
}

void LightDialogue::UpdateAmbient()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{
		// Update R ambient box
		CString sR; sR.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_ambient_r);
		m_eAmbR.SetWindowTextW(sR);

		// Update G ambient box
		CString sG; sG.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_ambient_g);
		m_eAmbG.SetWindowTextW(sG);

		// Update B ambient box
		CString sB; sB.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_ambient_b);
		m_eAmbB.SetWindowTextW(sB);
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_eAmbR.SetWindowTextW(empty);
		m_eAmbG.SetWindowTextW(empty);
		m_eAmbB.SetWindowTextW(empty);
		m_internal = false;
	}
}

void LightDialogue::UpdateConstA()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{				
		// Update constant attenuation box
		CString sA; sA.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_constant);
		m_eConstA.SetWindowTextW(sA);
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_eConstA.SetWindowTextW(empty);
		m_internal = false;
	}
}

void LightDialogue::UpdateLinA()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{
		// Update linear attenuation box
		CString sA; sA.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_linear);
		m_eLinA.SetWindowTextW(sA);
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_eLinA.SetWindowTextW(empty);
		m_internal = false;
	}
}

void LightDialogue::UpdateQuadA()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{
		// Update linear attenuation box
		CString sA; sA.Format(L"%g", m_toolSystem->GetSceneGraph()[m_selectedIndex].light_quadratic);
		m_eQuadA.SetWindowTextW(sA);
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_eQuadA.SetWindowTextW(empty);
		m_internal = false;
	}
}

void LightDialogue::UpdateSelectedConstraint()
{
	// If x, y and z constraints are selected
	if (m_x && m_y && m_z) { m_constraint = CONSTRAINT::NA; }

	// Else, if x and y constraints are selected
	else if (m_x && m_y) { m_constraint = CONSTRAINT::XY; }

	// Else, if x and z constraints are selected
	else if (m_x && m_z) { m_constraint = CONSTRAINT::XZ; }

	// Else, if y and z constraints are selected
	else if (m_y && m_z) { m_constraint = CONSTRAINT::YZ; }

	// Else, if only x constraint is selected
	else if (m_x) { m_constraint = CONSTRAINT::X; }

	// Else, if only y constraint is selected
	else if (m_y) { m_constraint = CONSTRAINT::Y; }

	// Else, if only z constraint is selected
	else if (m_z) { m_constraint = CONSTRAINT::Z; }

	// Else, if no constraints are selected
	else { m_constraint = CONSTRAINT::NA; }

	// Update constraint box
	m_boxConst.SetCurSel((int)m_constraint);
}

void LightDialogue::Reset()
{
	m_active = m_translating = m_x = m_y = m_z =
		m_select = false;

	m_function = OBJECT_FUNCTION::SELECT;
	m_constraint = CONSTRAINT::NA;
	
	CheckDlgButton(IDC_CHECK2, false);
	CheckDlgButton(IDC_CHECK23, false);
	CheckDlgButton(IDC_CHECK24, false);
	CheckDlgButton(IDC_CHECK25, false);

	GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_CHECK1)->EnableWindow(true);
}

void LightDialogue::UpdateLight(SceneObject object)
{
	// Replace object through database
	///ObjectManager::ReplaceObject(object);
	ObjectManager::ReplaceLight(object);
}

void LightDialogue::SetupLights(std::vector<SceneObject>* sceneGraph)
{
	m_internal = true;
	
	// Local storage of lights
	m_lights = new std::vector<SceneObject>();
	m_boxID.ResetContent();

	// Loop through scene graph
	for (int i = 0; i < sceneGraph->size(); ++i)
	{
		// If object is a light
		if (sceneGraph->at(i).m_type == OBJECT_TYPE::LIGHT)
		{
			// Add to storage
			m_lights->push_back(sceneGraph->at(i));

			// Add entries to ID list box
			std::wstring idBoxEntry = std::to_wstring(sceneGraph->at(i).ID);
			m_boxID.AddString(idBoxEntry.c_str());
		}
	}

	// Setup total lights in scene
	std::wstring totalLights = std::to_wstring(m_lights->size());
	SetDlgItemText(IDC_STATIC3, totalLights.c_str());

	m_internal = false;
}
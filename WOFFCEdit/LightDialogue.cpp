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
	
}

void LightDialogue::SetLightData(std::vector<DisplayObject>* displayList)
{
	// Local storage
	m_displayList = *displayList;

	// Setup IDs of currently available lights
	SetupLights();	

	// Setup light types
	{
		std::wstring typeBoxEntry;
		typeBoxEntry = L"";			m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Directional";	m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Point";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Spot";			m_boxType.AddString(typeBoxEntry.c_str());
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

	SetupCheckBoxes();
}

void LightDialogue::Update(int ID)
{
	// If ID is valid
	if (ID != -1) { m_boxID.SetCurSel(ID); }
	UpdateType();
	UpdateEnabled();
	UpdatePosition();
	UpdateDirection();
	UpdateDiffuse();
	UpdateAmbient();
	UpdateConstA();
	UpdateLinA();
	UpdateQuadA();
}

void LightDialogue::UpdateLightPosition(DirectX::XMFLOAT3 position)
{
	// Loop through current selection
	for (int i = 0; i < m_selectedLightIDs.size(); ++i)
	{
		// Loop through lights
		for (int j = 0; j < m_lights.size(); ++j)
		{
			// If light ID matches selection
			if (m_lights[j].m_ID == m_selectedLightIDs[i])
			{
				// Set current light position
				m_lights[j].SetPosition(position);
				break;
			}
		}
	}	

	// Update position boxes
	UpdatePosition();
}

void LightDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_boxID);

	DDX_Control(pDX, IDC_COMBO2, m_boxType);
	DDX_Control(pDX, IDC_COMBO6, m_boxConst);

	DDX_Control(pDX, IDC_EDIT1, m_ePosX);		DDX_Text(pDX, IDC_EDIT1, m_fPosX);
	DDX_Control(pDX, IDC_EDIT2, m_ePosY);		DDX_Text(pDX, IDC_EDIT2, m_fPosY);
	DDX_Control(pDX, IDC_EDIT3, m_ePosZ);		DDX_Text(pDX, IDC_EDIT3, m_fPosZ);

	DDX_Control(pDX, IDC_EDIT4, m_eDirX);		DDX_Text(pDX, IDC_EDIT4, m_fDirX);
	DDX_Control(pDX, IDC_EDIT5, m_eDirY);		DDX_Text(pDX, IDC_EDIT5, m_fDirX);
	DDX_Control(pDX, IDC_EDIT6, m_eDirZ);		DDX_Text(pDX, IDC_EDIT6, m_fDirX);

	DDX_Control(pDX, IDC_EDIT7, m_eDifR);		DDX_Text(pDX, IDC_EDIT7, m_fDifR);
	DDX_Control(pDX, IDC_EDIT8, m_eDifG);		DDX_Text(pDX, IDC_EDIT8, m_fDifG);
	DDX_Control(pDX, IDC_EDIT9, m_eDifB);		DDX_Text(pDX, IDC_EDIT9, m_fDifB);

	DDX_Control(pDX, IDC_EDIT10, m_eAmbR);		DDX_Text(pDX, IDC_EDIT10, m_fAmbR);
	DDX_Control(pDX, IDC_EDIT11, m_eAmbG);		DDX_Text(pDX, IDC_EDIT11, m_fAmbG);
	DDX_Control(pDX, IDC_EDIT12, m_eAmbB);		DDX_Text(pDX, IDC_EDIT12, m_fAmbB);

	DDX_Control(pDX, IDC_EDIT13, m_eConstA);	DDX_Text(pDX, IDC_EDIT13, m_fConstA);
	DDX_Control(pDX, IDC_EDIT14, m_eLinA);		DDX_Text(pDX, IDC_EDIT14, m_fLinA);
	DDX_Control(pDX, IDC_EDIT15, m_eQuadA);		DDX_Text(pDX, IDC_EDIT15, m_fQuadA);
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
	ON_EN_CHANGE(IDC_EDIT4, &LightDialogue::OnEnChangeDirX)
	ON_EN_CHANGE(IDC_EDIT5, &LightDialogue::OnEnChangeDirY)
	ON_EN_CHANGE(IDC_EDIT6, &LightDialogue::OnEnChangeDirZ)
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
	m_active = false;
	///DestroyWindow();
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
	for (int i = 0; i < m_selectedLightIDs.size(); ++i)
	{
		// If current already exists
		if (m_selectedLightIDs[i] == _ttoi(current))
		{
			// Inform remaining functionality the selection exists
			exists = true;
			
			// Remove from selection
			m_selectedLightIDs.erase(m_selectedLightIDs.begin() + i);
		}
	}

	// If selection doesn't exist, add to selection
	if (!exists) { m_selectedLightIDs.push_back(_ttoi(current)); }
	
	// Update light type box
	UpdateType();

	// Update enabled box
	UpdateEnabled();

	// Update position boxes
	UpdatePosition();

	// Update direction boxes
	UpdateDirection();

	// Update diffuse boxes
	UpdateDiffuse();

	// Update ambient boxes
	UpdateAmbient();

	// Update attenuation boxes
	UpdateConstA(); UpdateLinA(); UpdateQuadA();

	// Tell MFC/ToolMain to update scene graph & selected objects
	m_update = m_select = true;
}

// Type has been changed
void LightDialogue::OnCbnSelchangeType()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();
	
	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store type selection
						int type = m_boxType.GetCurSel();

						// Set new light type
						m_lights[j].SetLightType((LIGHT_TYPE)type);
						break;
					}
				}
			}

			// Tell MFC/ToolMain to update scene graph
			m_update = true;
		}
	}
}

// Light has been enabled/disabled
void LightDialogue::OnBnClickedEnable()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Set light enabled/disabled
						m_lights[j].SetEnabled(IsDlgButtonChecked(IDC_CHECK1));
						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
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
		if (m_selectedLightIDs.size() > 1)
		{
			// Setup focus modal dialogue
			m_focusDialogue.Create(IDD_DIALOG2);
			m_focusDialogue.ShowWindow(SW_SHOW);
			m_focusDialogue.SetActive(true);
			m_focusDialogue.SetFocusData(&m_selectedLightIDs);
		}
	}	
}

// X position has been changed
void LightDialogue::OnEnChangePosX()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new X position
						CString string = _T("");
						m_ePosX.GetWindowTextW(string);

						// Convert to float
						float posX;
						if (!string.IsEmpty()) { posX = _ttof(string); }
						else { posX = m_lights[j].GetPosition().x; }

						// Update X position of light
						m_lights[j].SetPositionX(posX);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// Y position has been changed
void LightDialogue::OnEnChangePosY()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new Y position
						CString string = _T("");
						m_ePosY.GetWindowTextW(string);

						// Convert to float
						float posY;
						if (!string.IsEmpty()) { posY = _ttof(string); }
						else { posY = m_lights[j].GetPosition().y; }

						// Update Y position of light
						m_lights[j].SetPositionY(posY);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// Z position has been changed
void LightDialogue::OnEnChangePosZ()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new Z position
						CString string = _T("");
						m_ePosZ.GetWindowTextW(string);

						// Convert to float
						float posZ;
						if (!string.IsEmpty()) { posZ = _ttof(string); }
						else { posZ = m_lights[j].GetPosition().z; }

						// Update Z position of light
						m_lights[j].SetPositionZ(posZ);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// X direction has been changed
void LightDialogue::OnEnChangeDirX()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new X direction
						CString string = _T("");
						m_eDirX.GetWindowTextW(string);

						// Convert to float
						float dirX;
						if (!string.IsEmpty()) { dirX = _ttof(string); }
						else { dirX = m_lights[j].GetDirection().x; }

						// Store as vector & normalize
						///DirectX::SimpleMath::Vector3 direction = { dirX, 1.f, 1.f };
						///direction.Normalize();

						// Update X direction of light
						m_lights[j].SetDirectionX(dirX);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// Y direction has been changed
void LightDialogue::OnEnChangeDirY()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new Y direction
						CString string = _T("");
						m_eDirY.GetWindowTextW(string);

						// Convert to float
						float dirY;
						if (!string.IsEmpty()) { dirY = _ttof(string); }
						else { dirY = m_lights[j].GetDirection().y; }

						// Store as vector & normalize
						///DirectX::SimpleMath::Vector3 direction = { 1.f, dirY, 1.f };
						///direction.Normalize();

						// Update Y direction of light
						m_lights[j].SetDirectionY(dirY);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// Z direction has been changed
void LightDialogue::OnEnChangeDirZ()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new Z direction
						CString string = _T("");
						m_eDirY.GetWindowTextW(string);

						// Convert to float
						float dirZ;
						if (!string.IsEmpty()) { dirZ = _ttof(string); }
						else { dirZ = m_lights[j].GetDirection().z; }

						// Store as vector & normalize
						///DirectX::SimpleMath::Vector3 direction = { 1.f, 1.f, dirZ };
						///direction.Normalize();

						// Update Z direction of light
						m_lights[j].SetDirectionZ(dirZ);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// R diffuse has been changed
void LightDialogue::OnEnChangeDifR()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new R diffuse
						CString string = _T("");
						m_eDifR.GetWindowTextW(string);

						// Convert to float
						float difR;
						if (!string.IsEmpty()) { difR = _ttof(string); }
						else { difR = m_lights[j].GetDiffuse().x; }

						// Update R diffuse of light
						m_lights[j].SetDiffuseR(difR);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// G diffuse has been changed
void LightDialogue::OnEnChangeDifG()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new G diffuse
						CString string = _T("");
						m_eDifR.GetWindowTextW(string);

						// Convert to float
						float difG;
						if (!string.IsEmpty()) { difG = _ttof(string); }
						else { difG = m_lights[j].GetDiffuse().y; }

						// Update G diffuse of light
						m_lights[j].SetDiffuseG(difG);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// B diffuse has been changed
void LightDialogue::OnEnChangeDifB()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new B diffuse
						CString string = _T("");
						m_eDifR.GetWindowTextW(string);

						// Convert to float
						float difB;
						if (!string.IsEmpty()) { difB = _ttof(string); }
						else { difB = m_lights[j].GetDiffuse().z; }

						// Update B diffuse of light
						m_lights[j].SetDiffuseB(difB);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// R ambient has been changed
void LightDialogue::OnEnChangeAmbR()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new R ambient
						CString string = _T("");
						m_eDifR.GetWindowText(string);

						// Convert to float
						float ambR;
						if (!string.IsEmpty()) { ambR = _ttof(string); }
						else { ambR = m_lights[j].GetAmbient().x; }
						/*if (ambR >= 4.f) { ambR /= 50.f; }
						else if (ambR >= 3.f) { ambR /= 40.f; }
						else if (ambR >= 2.f) { ambR /= 30.f; }
						else if (ambR >= 1.f) { ambR /= 20.f; }
						else if (ambR >= 0.f) { ambR /= 10.f; }*/
						ambR /= 10.f;

						// Update R ambient of light
						m_lights[j].SetAmbientR(ambR);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// G ambient has been changed
void LightDialogue::OnEnChangeAmbG()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	
	// If function has been called by user
	if (!m_internal)
	{// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new G ambient
						CString string = _T("");
						m_eDifR.GetWindowText(string);

						// Convert to float
						float ambG;
						if (!string.IsEmpty()) { ambG = _ttof(string); }
						else { ambG = m_lights[j].GetAmbient().y; }
						/*if (ambG >= 4.f) { ambG /= 50.f; }
						else if (ambG >= 3.f) { ambG /= 40.f; }
						else if (ambG >= 2.f) { ambG /= 30.f; }
						else if (ambG >= 1.f) { ambG /= 20.f; }
						else if (ambG >= 0.f) { ambG /= 10.f; }*/
						ambG /= 10.f;

						// Update G ambient of light
						m_lights[j].SetAmbientG(ambG);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// B ambient has been changed
void LightDialogue::OnEnChangeAmbB()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new B ambient
						CString string = _T("");
						m_eDifR.GetWindowText(string);

						// Convert to float
						float ambB;
						if (!string.IsEmpty()) { ambB = _ttof(string); }
						else { ambB = m_lights[j].GetAmbient().z; }
						/*if (ambB >= 4.f) { ambB /= 50.f; }
						else if (ambB >= 3.f) { ambB /= 40.f; }
						else if (ambB >= 2.f) { ambB /= 30.f; }
						else if (ambB >= 1.f) { ambB /= 20.f; }
						else if (ambB >= 0.f) { ambB /= 10.f; }*/
						ambB /= 10.f;

						// Update B ambient of light
						m_lights[j].SetAmbientG(ambB);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// Constant attenuation has been changed
void LightDialogue::OnEnChangeConstA()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new constant attenuation
						CString string = _T("");
						m_eConstA.GetWindowTextW(string);

						// Convert to float
						float constA;
						if (!string.IsEmpty()) { constA = _ttof(string); }
						else { constA = m_lights[j].GetConstantAttenuation(); }

						// Update constant attenuation of light
						m_lights[j].SetConstantAttenuation(constA);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// Linear attenuation has been changed
void LightDialogue::OnEnChangeLinA()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new linear attenuation
						CString string = _T("");
						m_eLinA.GetWindowTextW(string);

						// Convert to float
						float linA;
						if (!string.IsEmpty()) { linA = _ttof(string); }
						else { linA = m_lights[j].GetLinearAttenuation(); }
						///linA /= 10.f;

						// Update linear attenuation of light
						m_lights[j].SetLinearAttenuation(linA);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// Quadratic attenuation has been changed
void LightDialogue::OnEnChangeQuadA()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// If function has been called by user
	if (!m_internal)
	{
		// If ID is valid
		if (ID >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedLightIDs.size(); ++i)
			{
				// Loop through lights
				for (int j = 0; j < m_lights.size(); ++j)
				{
					// If light ID matches selection
					if (m_lights[j].m_ID == m_selectedLightIDs[i])
					{
						// Store new quadratic attenuation
						CString string = _T("");
						m_eQuadA.GetWindowTextW(string);

						// Convert to float
						float quadA;
						if (!string.IsEmpty()) { quadA = _ttof(string); }
						else { quadA = m_lights[j].GetQuadraticAttenuation(); }
						///quadA /= 100.f;

						// Update quadratic attenuation of light
						m_lights[j].SetQuadraticAttenuation(quadA);

						break;
					}
				}

				// Tell MFC/ToolMain to update scene graph
				m_update = true;
			}
		}
	}
}

// Delete has been selected
void LightDialogue::OnBnClickedDelete()
{
	// Delete if selection is valid
	m_delete = m_resetLights = (m_selectedLightIDs.size() > 0);
}

// Duplicate has been selected
void LightDialogue::OnBnClickedDuplicate()
{	
	// Duplicate if selection is valid
	m_duplicate = m_resetLights = (m_selectedLightIDs.size() != 0);
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
	}
	break;
	case false:
	{
		// Enable delete button
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);

		// Disable duplicate button
		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
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
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Set combo box
				m_boxType.SetCurSel((int)m_lights[i].GetLightType());
				break;
			}
		}		
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
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Set enabled check box to match light 
				CheckDlgButton(IDC_CHECK1, m_lights[i].GetEnabled());
				break;
			}
		}
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
	// If only one object is selected
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Store current position
				XMFLOAT3 position = m_lights[i].GetPosition();

				// Update X position box
				CString sX; sX.Format(L"%g", position.x);
				m_ePosX.SetWindowTextW(sX);

				// Update Y position box
				CString sY; sY.Format(L"%g", position.y);
				m_ePosY.SetWindowTextW(sY);

				// Update Z position box
				CString sZ; sZ.Format(L"%g", position.z);
				m_ePosZ.SetWindowTextW(sZ);

				break;
			}
		}
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

void LightDialogue::UpdateDirection()
{
	// If only one object is selected
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Store current direction
				XMFLOAT3 direction = m_lights[i].GetDirection();

				// Update X direction box
				CString sX; sX.Format(L"%g", direction.x);
				m_eDirX.SetWindowTextW(sX);

				// Update Y direction box
				CString sY; sY.Format(L"%g", direction.y);
				m_eDirY.SetWindowTextW(sY);

				// Update Z direction box
				CString sZ; sZ.Format(L"%g", direction.z);
				m_eDirZ.SetWindowTextW(sZ);

				break;
			}
		}
	}
	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_eDirX.SetWindowTextW(empty);
		m_eDirY.SetWindowTextW(empty);
		m_eDirZ.SetWindowTextW(empty);
		m_internal = false;
	}
}

void LightDialogue::UpdateDiffuse()
{
	// If only one object is selected
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Store current diffuse
				XMFLOAT4 diffuse = m_lights[i].GetDiffuse();

				// Update R diffuse box
				CString sR; sR.Format(L"%g", diffuse.x);
				m_eDifR.SetWindowTextW(sR);

				// Update G diffuse box
				CString sG; sG.Format(L"%g", diffuse.y);
				m_eDifG.SetWindowTextW(sG);

				// Update B diffuse box
				CString sB; sB.Format(L"%g", diffuse.z);
				m_eDifB.SetWindowTextW(sB);

				break;
			}
		}
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
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Store current ambient
				XMFLOAT4 ambient = m_lights[i].GetAmbient();

				// Update R ambient box
				CString sR; sR.Format(L"%g", ambient.x);
				m_eAmbR.SetWindowTextW(sR);

				// Update G ambient box
				CString sG; sG.Format(L"%g", ambient.y);
				m_eAmbG.SetWindowTextW(sG);

				// Update B ambient box
				CString sB; sB.Format(L"%g", ambient.z);
				m_eAmbB.SetWindowTextW(sB);

				break;
			}
		}
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
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Store current constant attenuation
				float constA = m_lights[i].GetConstantAttenuation();

				// Update constant attenuation box
				CString sA; sA.Format(L"%g", constA);
				m_eConstA.SetWindowTextW(sA);

				break;
			}
		}
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
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Store current linear attenuation
				float linA = m_lights[i].GetLinearAttenuation();

				// Update linear attenuation box
				CString sA; sA.Format(L"%g", linA);
				m_eLinA.SetWindowTextW(sA);

				break;
			}
		}
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
	if (m_selectedLightIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// If light ID matches selection
			if (m_lights[i].m_ID == m_selectedLightIDs[0])
			{
				// Store current quadratic attenuation
				float quadA = m_lights[i].GetQuadraticAttenuation();

				// Update linear attenuation box
				CString sA; sA.Format(L"%g", quadA);
				m_eQuadA.SetWindowTextW(sA);

				break;
			}
		}
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
		m_select = m_resetLights = false;

	m_constraint = CONSTRAINT::NA;
}

void LightDialogue::SetupLights()
{
	if (m_resetLights) { m_selectedLightIDs.clear(); m_resetLights = false; }
	m_lights.clear();
	m_boxID.ResetContent();

	// Count lights
	int count = 0;

	// Loop through scene graph
	for (int i = 0; i < m_displayList.size(); ++i)
	{
		// If object is a light
		if (m_displayList.at(i).m_objectType == OBJECT_TYPE::LIGHT)
		{
			// Add to local storage
			m_lights.push_back(m_displayList.at(i));
			
			// Add entries to ID list box
			std::wstring idBoxEntry = std::to_wstring(m_displayList.at(i).m_ID);
			m_boxID.AddString(idBoxEntry.c_str());

			// Increase count
			count++;
		}
	}

	// Setup total lights in scene
	std::wstring totalLights = std::to_wstring(count);
	SetDlgItemText(IDC_STATIC3, totalLights.c_str());

	m_lightSetup = true;
}

XMFLOAT3 LightDialogue::GetPosition()
{
	// Store X position
	CString stringX = _T("");
	m_ePosX.GetWindowTextW(stringX);

	// Convert to float
	float posX;
	if (!stringX.IsEmpty()) { posX = _ttof(stringX); }
	else { posX = 150.f; }

	// Store Y position
	CString stringY = _T("");
	m_ePosY.GetWindowTextW(stringY);

	// Convert to float
	float posY;
	if (!stringY.IsEmpty()) { posY = _ttof(stringY); }
	else { posY = 150.f; }

	// Store Z position
	CString stringZ = _T("");
	m_ePosZ.GetWindowTextW(stringZ);

	// Convert to float
	float posZ;
	if (!stringZ.IsEmpty()) { posZ = _ttof(stringZ); }
	else { posZ = 150.f; }

	return XMFLOAT3{ posX, posY, posZ };
}

XMFLOAT3 LightDialogue::GetDirection()
{
	// Store X direction
	CString stringX = _T("");
	m_eDirX.GetWindowTextW(stringX);

	// Convert to float
	float dirX;
	if (!stringX.IsEmpty()) { dirX = _ttof(stringX); }
	else { dirX = 0.f; }

	// Store Y direction
	CString stringY = _T("");
	m_eDirY.GetWindowTextW(stringY);

	// Convert to float
	float dirY;
	if (!stringY.IsEmpty()) { dirY = _ttof(stringY); }
	else { dirY = 1.f; }

	// Store Z direction
	CString stringZ = _T("");
	m_eDirY.GetWindowTextW(stringZ);

	// Convert to float
	float dirZ;
	if (!stringZ.IsEmpty()) { dirZ = _ttof(stringZ); }
	else { dirZ = 0.f; }

	return XMFLOAT3{ dirX, dirY, dirZ };
}

XMFLOAT3 LightDialogue::GetDiffuse()
{
	// Store R diffuse
	CString stringR = _T("");
	m_eDifR.GetWindowTextW(stringR);

	// Convert to float
	float difR;
	if (!stringR.IsEmpty()) { difR = _ttof(stringR); }
	else { difR = 2.f; }

	// Store G diffuse
	CString stringG = _T("");
	m_eDifR.GetWindowTextW(stringG);

	// Convert to float
	float difG;
	if (!stringG.IsEmpty()) { difG = _ttof(stringG); }
	else { difG = 2.f; }

	// Store B diffuse
	CString stringB = _T("");
	m_eDifR.GetWindowTextW(stringB);

	// Convert to float
	float difB;
	if (!stringB.IsEmpty()) { difB = _ttof(stringB); }
	else { difB = 2.f; }

	return XMFLOAT3{ difR, difG, difB };
}

float LightDialogue::GetConstA()
{
	// Store constant attenuation
	CString string = _T("");
	m_eConstA.GetWindowTextW(string);

	// Convert to float
	float constA;
	if (!string.IsEmpty()) { constA = _ttof(string); }
	else { constA = 8.f; }

	return constA;
}

float LightDialogue::GetLinA()
{
	// Store linear attenuation
	CString string = _T("");
	m_eLinA.GetWindowTextW(string);

	// Convert to float
	float linA;
	if (!string.IsEmpty()) { linA = _ttof(string); linA /= 10.f; }
	else { linA = 0.0125; }
	
	return linA;
}

float LightDialogue::GetQuadA()
{
	// Store quadratic attenuation
	CString string = _T("");
	m_eQuadA.GetWindowTextW(string);

	// Convert to float
	float quadA;
	if (!string.IsEmpty()) { quadA = _ttof(string); quadA /= 100.f; }
	else { quadA = 0.1f; }
	
	return quadA;
}

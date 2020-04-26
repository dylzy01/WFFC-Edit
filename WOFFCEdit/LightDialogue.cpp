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
	///m_sceneGraph = sceneGraph;
}

void LightDialogue::SetLightData(std::pair<std::vector<Light*>, std::vector<int>>* lights)
{
	// Local storage of lights
	m_lights = *lights;

	// Loop through lights and add entries to the ID combo box	
	for (int i = 0; i < m_lights.first.size(); ++i)
	{
		// Setup light IDs
		std::wstring idBoxEntry = std::to_wstring(m_lights.second.at(i));
		m_boxID.AddString(idBoxEntry.c_str());
	}

	// Setup total lights in scene
	int total = m_lights.first.size();
	std::wstring totalLights = std::to_wstring(total);
	SetDlgItemText(IDC_STATIC3, totalLights.c_str());

	// Setup light types
	std::wstring typeBoxEntry;
	typeBoxEntry = L"NA";			m_boxType.AddString(typeBoxEntry.c_str());
	typeBoxEntry = L"Directional";	m_boxType.AddString(typeBoxEntry.c_str());	
	typeBoxEntry = L"Point";		m_boxType.AddString(typeBoxEntry.c_str());	
	typeBoxEntry = L"Spot";			m_boxType.AddString(typeBoxEntry.c_str());	

	// Display first light
	Update(0);
}

void LightDialogue::Update(int index)
{
	m_boxID.SetCurSel(index);
	UpdateType(index);
	UpdateEnabled(index);
	UpdatePosition(index);
	UpdateDirection(index);
	UpdateDiffuse(index);
	UpdateAmbient(index);
	UpdateConstA(index);
	UpdateLinA(index);
	UpdateQuadA(index);
}

void LightDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_boxID);
	DDX_Control(pDX, IDC_COMBO2, m_boxType);

	DDX_Control(pDX, IDC_EDIT1, m_posX);
	DDX_Control(pDX, IDC_EDIT2, m_posY);
	DDX_Control(pDX, IDC_EDIT3, m_posZ);

	DDX_Control(pDX, IDC_EDIT4, m_dirX);
	DDX_Control(pDX, IDC_EDIT5, m_dirY);
	DDX_Control(pDX, IDC_EDIT6, m_dirZ);

	DDX_Control(pDX, IDC_EDIT7, m_difR);
	DDX_Control(pDX, IDC_EDIT8, m_difG);
	DDX_Control(pDX, IDC_EDIT9, m_difB);

	DDX_Control(pDX, IDC_EDIT10, m_ambR);
	DDX_Control(pDX, IDC_EDIT11, m_ambG);
	DDX_Control(pDX, IDC_EDIT12, m_ambB);

	DDX_Control(pDX, IDC_EDIT13, m_constA);
	DDX_Control(pDX, IDC_EDIT14, m_linA);
	DDX_Control(pDX, IDC_EDIT15, m_quadA);
}

BEGIN_MESSAGE_MAP(LightDialogue, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &LightDialogue::OnCbnSelchangeID)
	ON_CBN_SELCHANGE(IDC_COMBO2, &LightDialogue::OnCbnSelchangeType)
	ON_BN_CLICKED(IDC_CHECK1, &LightDialogue::OnBnClickedEnable)
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
END_MESSAGE_MAP()

// Kill the dialogue
void LightDialogue::End()
{
	OnBnClickedOk();
	DestroyWindow();
}

// User is finished with dialogue
void LightDialogue::OnBnClickedOk()
{
	m_active = false;
	CDialogEx::OnOK();
}

// ID has been selected
void LightDialogue::OnCbnSelchangeID()
{		
	// Store ID selection
	int ID = m_boxID.GetCurSel();
	
	// Update light type box
	UpdateType(ID);

	// Update enabled box
	UpdateEnabled(ID);

	// Update position boxes
	UpdatePosition(ID);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Type has been changed
void LightDialogue::OnCbnSelchangeType()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();
	
	// Store type selection
	int type = m_boxType.GetCurSel();	

	// Set new light type
	m_lights.first[ID]->SetType((LIGHT_TYPE)type);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Light has been enabled/disabled
void LightDialogue::OnBnClickedEnable()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Set light enabled/disabled
	m_lights.first[ID]->SetEnabled(IsDlgButtonChecked(IDC_CHECK1));

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// X position has been changed
void LightDialogue::OnEnChangePosX()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new X position
	CString string = _T("");
	m_posX.GetWindowTextW(string);

	// Convert to float
	float posX;
	if (!string.IsEmpty()) { posX = _ttof(string); }
	else { posX = m_lights.first[ID]->GetPosition().x; }

	// Update X position of light
	m_lights.first[ID]->SetPositionX(posX);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Y position has been changed
void LightDialogue::OnEnChangePosY()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new X position
	CString string = _T("");
	m_posY.GetWindowTextW(string);

	// Convert to float
	float posY;
	if (!string.IsEmpty()) { posY = _ttof(string); }
	else { posY = m_lights.first[ID]->GetPosition().y; }

	// Update X position of light
	m_lights.first[ID]->SetPositionY(posY);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Z position has been changed
void LightDialogue::OnEnChangePosZ()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new X position
	CString string = _T("");
	m_posZ.GetWindowTextW(string);

	// Convert to float
	float posZ;
	if (!string.IsEmpty()) { posZ = _ttof(string); }
	else { posZ = m_lights.first[ID]->GetPosition().z; }

	// Update X position of light
	m_lights.first[ID]->SetPositionZ(posZ);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// X direction has been changed
void LightDialogue::OnEnChangeDirX()
{
}

// Y direction has been changed
void LightDialogue::OnEnChangeDirY()
{
}

// Z direction has been changed
void LightDialogue::OnEnChangeDirZ()
{
}

// R diffuse has been changed
void LightDialogue::OnEnChangeDifR()
{
}

// G diffuse has been changed
void LightDialogue::OnEnChangeDifG()
{
}

// B diffuse has been changed
void LightDialogue::OnEnChangeDifB()
{
}

// R ambient has been changed
void LightDialogue::OnEnChangeAmbR()
{
}

// G ambient has been changed
void LightDialogue::OnEnChangeAmbG()
{
}

// B ambient has been changed
void LightDialogue::OnEnChangeAmbB()
{
}

// Constant attenuation has been changed
void LightDialogue::OnEnChangeConstA()
{
}

// Linear attenuation has been changed
void LightDialogue::OnEnChangeLinA()
{
}

// Quadratic attenuation has been changed
void LightDialogue::OnEnChangeQuadA()
{
}

// Update remaining light details when one is changed ///////////////////////////////////////////////

void LightDialogue::UpdateID(std::pair<Light*, int>* light)
{

}

void LightDialogue::UpdateType(int ID)
{
	// Switch between light type & set combo box
	switch (m_lights.first[ID]->GetType())
	{
	case LIGHT_TYPE::NA:			m_boxType.SetCurSel(0); break;	
	case LIGHT_TYPE::DIRECTIONAL:	m_boxType.SetCurSel(1); break;
	case LIGHT_TYPE::POINT:			m_boxType.SetCurSel(2); break;
	case LIGHT_TYPE::SPOT:			m_boxType.SetCurSel(3); break;
	}
}

void LightDialogue::UpdateEnabled(int ID)
{
	// Set enabled check box to match light 
	CheckDlgButton(IDC_CHECK1, m_lights.first[ID]->GetEnabled());
}

void LightDialogue::UpdatePosition(int ID)
{
	// Store current position
	XMFLOAT3 position = m_lights.first[ID]->GetPosition();

	// Update X position box
	CString sX; sX.Format(_T("%f"), position.x);
	m_posX.SetWindowTextW(sX);

	// Update Y position box
	CString sY; sY.Format(_T("%f"), position.y);
	m_posY.SetWindowTextW(sY);

	// Update Z position box
	CString sZ; sZ.Format(_T("%f"), position.z);
	m_posZ.SetWindowTextW(sZ);
}

void LightDialogue::UpdateDirection(int ID)
{
}

void LightDialogue::UpdateDiffuse(int ID)
{
}

void LightDialogue::UpdateAmbient(int ID)
{
}

void LightDialogue::UpdateConstA(int ID)
{
}

void LightDialogue::UpdateLinA(int ID)
{
}

void LightDialogue::UpdateQuadA(int ID)
{
}

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
	if (m_lights.first.size() != 0) { Update(0); }	
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
	////OnBnClickedOk();	
	DestroyWindow();
}

// User is finished with dialogue
void LightDialogue::OnBnClickedOk()
{		
	m_active = false;
	End();
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

	// Update direction boxes
	UpdateDirection(ID);

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
	m_ePosX.GetWindowTextW(string);

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

	// Store new Y position
	CString string = _T("");
	m_ePosY.GetWindowTextW(string);

	// Convert to float
	float posY;
	if (!string.IsEmpty()) { posY = _ttof(string); }
	else { posY = m_lights.first[ID]->GetPosition().y; }

	// Update Y position of light
	m_lights.first[ID]->SetPositionY(posY);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Z position has been changed
void LightDialogue::OnEnChangePosZ()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new Z position
	CString string = _T("");
	m_ePosZ.GetWindowTextW(string);

	// Convert to float
	float posZ;
	if (!string.IsEmpty()) { posZ = _ttof(string); }
	else { posZ = m_lights.first[ID]->GetPosition().z; }

	// Update Z position of light
	m_lights.first[ID]->SetPositionZ(posZ);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// X direction has been changed
void LightDialogue::OnEnChangeDirX()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new X direction
	CString string = _T("");
	m_eDirX.GetWindowTextW(string);

	// Convert to float
	float dirX;
	if (!string.IsEmpty()) { dirX = _ttof(string); }
	else { dirX = m_lights.first[ID]->GetDirection().x; }

	// Store as vector & normalize
	///DirectX::SimpleMath::Vector3 direction = { dirX, 1.f, 1.f };
	///direction.Normalize();

	// Update X direction of light
	m_lights.first[ID]->SetDirectionX(dirX);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Y direction has been changed
void LightDialogue::OnEnChangeDirY()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new Y direction
	CString string = _T("");
	m_eDirY.GetWindowTextW(string);

	// Convert to float
	float dirY;
	if (!string.IsEmpty()) { dirY = _ttof(string); }
	else { dirY = m_lights.first[ID]->GetDirection().y; }

	// Store as vector & normalize
	///DirectX::SimpleMath::Vector3 direction = { 1.f, dirY, 1.f };
	///direction.Normalize();

	// Update Y direction of light
	m_lights.first[ID]->SetDirectionY(dirY);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Z direction has been changed
void LightDialogue::OnEnChangeDirZ()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new Z direction
	CString string = _T("");
	m_eDirY.GetWindowTextW(string);

	// Convert to float
	float dirZ;
	if (!string.IsEmpty()) { dirZ = _ttof(string); }
	else { dirZ = m_lights.first[ID]->GetDirection().z; }

	// Store as vector & normalize
	///DirectX::SimpleMath::Vector3 direction = { 1.f, 1.f, dirZ };
	///direction.Normalize();

	// Update Z direction of light
	m_lights.first[ID]->SetDirectionZ(dirZ);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// R diffuse has been changed
void LightDialogue::OnEnChangeDifR()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new R diffuse
	CString string = _T("");
	m_eDifR.GetWindowTextW(string);

	// Convert to float
	float difR;
	if (!string.IsEmpty()) { difR = _ttof(string); }
	else { difR = m_lights.first[ID]->GetDiffuse().x; }
	
	// Update R diffuse of light
	m_lights.first[ID]->SetDiffuseR(difR);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// G diffuse has been changed
void LightDialogue::OnEnChangeDifG()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new G diffuse
	CString string = _T("");
	m_eDifR.GetWindowTextW(string);

	// Convert to float
	float difG;
	if (!string.IsEmpty()) { difG = _ttof(string); }
	else { difG = m_lights.first[ID]->GetDiffuse().y; }

	// Update G diffuse of light
	m_lights.first[ID]->SetDiffuseG(difG);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// B diffuse has been changed
void LightDialogue::OnEnChangeDifB()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new B diffuse
	CString string = _T("");
	m_eDifR.GetWindowTextW(string);

	// Convert to float
	float difB;
	if (!string.IsEmpty()) { difB = _ttof(string); }
	else { difB = m_lights.first[ID]->GetDiffuse().z; }

	// Update B diffuse of light
	m_lights.first[ID]->SetDiffuseB(difB);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// R ambient has been changed
void LightDialogue::OnEnChangeAmbR()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new R ambient
	CString string = _T("");
	m_eDifR.GetWindowText(string);

	// Convert to float
	float ambR;
	if (!string.IsEmpty()) { ambR = _ttof(string); }
	else { ambR = m_lights.first[ID]->GetAmbient().x; }
	/*if (ambR >= 4.f) { ambR /= 50.f; }
	else if (ambR >= 3.f) { ambR /= 40.f; }
	else if (ambR >= 2.f) { ambR /= 30.f; }
	else if (ambR >= 1.f) { ambR /= 20.f; }
	else if (ambR >= 0.f) { ambR /= 10.f; }*/
	ambR /= 10.f;

	// Update R ambient of light
	m_lights.first[ID]->SetAmbientR(ambR);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// G ambient has been changed
void LightDialogue::OnEnChangeAmbG()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new G ambient
	CString string = _T("");
	m_eDifR.GetWindowText(string);

	// Convert to float
	float ambG;
	if (!string.IsEmpty()) { ambG = _ttof(string); }
	else { ambG = m_lights.first[ID]->GetAmbient().y; }
	/*if (ambG >= 4.f) { ambG /= 50.f; }
	else if (ambG >= 3.f) { ambG /= 40.f; }
	else if (ambG >= 2.f) { ambG /= 30.f; }
	else if (ambG >= 1.f) { ambG /= 20.f; }
	else if (ambG >= 0.f) { ambG /= 10.f; }*/
	ambG /= 10.f;

	// Update G ambient of light
	m_lights.first[ID]->SetAmbientG(ambG);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// B ambient has been changed
void LightDialogue::OnEnChangeAmbB()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new B ambient
	CString string = _T("");
	m_eDifR.GetWindowText(string);

	// Convert to float
	float ambB;
	if (!string.IsEmpty()) { ambB = _ttof(string); }
	else { ambB = m_lights.first[ID]->GetAmbient().z; }
	/*if (ambB >= 4.f) { ambB /= 50.f; }
	else if (ambB >= 3.f) { ambB /= 40.f; }
	else if (ambB >= 2.f) { ambB /= 30.f; }
	else if (ambB >= 1.f) { ambB /= 20.f; }
	else if (ambB >= 0.f) { ambB /= 10.f; }*/
	ambB /= 10.f;

	// Update B ambient of light
	m_lights.first[ID]->SetAmbientG(ambB);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Constant attenuation has been changed
void LightDialogue::OnEnChangeConstA()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new constant attenuation
	CString string = _T("");
	m_eConstA.GetWindowTextW(string);

	// Convert to float
	float constA;
	if (!string.IsEmpty()) { constA = _ttof(string); }
	else { constA = m_lights.first[ID]->GetConstantAttenuation(); }

	// Update constant attenuation of light
	m_lights.first[ID]->SetConstantAttenuation(constA);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Linear attenuation has been changed
void LightDialogue::OnEnChangeLinA()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new linear attenuation
	CString string = _T("");
	m_eLinA.GetWindowTextW(string);

	// Convert to float
	float linA;
	if (!string.IsEmpty()) { linA = _ttof(string); }
	else { linA = m_lights.first[ID]->GetLinearAttenuation(); }

	// Update linear attenuation of light
	m_lights.first[ID]->SetLinearAttenuation(linA);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Quadratic attenuation has been changed
void LightDialogue::OnEnChangeQuadA()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Store new quadratic attenuation
	CString string = _T("");
	m_eQuadA.GetWindowTextW(string);

	// Convert to float
	float quadA;
	if (!string.IsEmpty()) { quadA = _ttof(string); }
	else { quadA = m_lights.first[ID]->GetQuadraticAttenuation(); }

	// Update quadratic attenuation of light
	m_lights.first[ID]->SetQuadraticAttenuation(quadA);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
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
	CString sX; sX.Format(L"%g", position.x);
	m_ePosX.SetWindowTextW(sX);

	// Update Y position box
	CString sY; sY.Format(L"%g", position.y);
	m_ePosY.SetWindowTextW(sY);

	// Update Z position box
	CString sZ; sZ.Format(L"%g", position.z);
	m_ePosZ.SetWindowTextW(sZ);
}

void LightDialogue::UpdateDirection(int ID)
{
	// Store current direction
	XMFLOAT3 direction = m_lights.first[ID]->GetDirection();	

	// Update X direction box
	CString sX; sX.Format(L"%g", direction.x);
	m_eDirX.SetWindowTextW(sX);

	// Update Y direction box
	CString sY; sY.Format(L"%g", direction.y);
	m_eDirY.SetWindowTextW(sY);

	// Update Z direction box
	CString sZ; sZ.Format(L"%g", direction.z);
	m_eDirZ.SetWindowTextW(sZ);
}

void LightDialogue::UpdateDiffuse(int ID)
{
	// Store current diffuse
	XMFLOAT4 diffuse = m_lights.first[ID]->GetDiffuse();

	// Update R diffuse box
	CString sR; sR.Format(L"%g", diffuse.x);
	m_eDifR.SetWindowTextW(sR);

	// Update G diffuse box
	CString sG; sG.Format(L"%g", diffuse.y);
	m_eDifG.SetWindowTextW(sG);

	// Update B diffuse box
	CString sB; sB.Format(L"%g", diffuse.z);
	m_eDifB.SetWindowTextW(sB);
}

void LightDialogue::UpdateAmbient(int ID)
{
	// Store current ambient
	XMFLOAT4 ambient = m_lights.first[ID]->GetAmbient();

	// Update R ambient box
	CString sR; sR.Format(L"%g", ambient.x);
	m_eAmbR.SetWindowTextW(sR);

	// Update G ambient box
	CString sG; sG.Format(L"%g", ambient.y);
	m_eAmbG.SetWindowTextW(sG);

	// Update B ambient box
	CString sB; sB.Format(L"%g", ambient.z);
	m_eAmbB.SetWindowTextW(sB);
}

void LightDialogue::UpdateConstA(int ID)
{
	// Store current constant attenuation
	float constA = m_lights.first[ID]->GetConstantAttenuation();

	// Update constant attenuation box
	CString sA; sA.Format(L"%g", constA);
	m_eConstA.SetWindowTextW(sA);
}

void LightDialogue::UpdateLinA(int ID)
{
	// Store current linear attenuation
	float linA = m_lights.first[ID]->GetLinearAttenuation();

	// Update linear attenuation box
	CString sA; sA.Format(L"%g", linA);
	m_eLinA.SetWindowTextW(sA);
}

void LightDialogue::UpdateQuadA(int ID)
{
	// Store current quadratic attenuation
	float quadA = m_lights.first[ID]->GetQuadraticAttenuation();

	// Update linear attenuation box
	CString sA; sA.Format(L"%g", quadA);
	m_eQuadA.SetWindowTextW(sA);
}
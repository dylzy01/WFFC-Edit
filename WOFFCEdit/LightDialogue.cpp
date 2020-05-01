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

void LightDialogue::SetLightData(std::vector<SceneObject>* sceneGraph, std::vector<DisplayObject>* lights)
{
	// Local storage
	m_sceneGraph = *sceneGraph;
	m_lights = *lights;

	// Loop through lights and add entries to the ID combo box	
	for (int i = 0; i < m_lights.size(); ++i)
	{
		// Setup light IDs
		std::wstring idBoxEntry = std::to_wstring(m_lights.at(i).m_ID);
		m_boxID.AddString(idBoxEntry.c_str());
	}

	// Setup total lights in scene
	int total = m_lights.size();
	std::wstring totalLights = std::to_wstring(total);
	SetDlgItemText(IDC_STATIC3, totalLights.c_str());

	// Setup light types
	{
		std::wstring typeBoxEntry;
		///typeBoxEntry = L"NA";			m_boxType.AddString(typeBoxEntry.c_str());
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

	// Display first light
	if (m_lights.size() != 0) { Update(0); }	

	// Set constraint to display N/A
	m_boxConst.SetCurSel(0);

	SetupCheckBoxes();
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

void LightDialogue::UpdateLightPosition(DirectX::XMFLOAT3 position)
{
	// Get current ID
	int ID = m_boxID.GetCurSel();

	// Set current light position
	m_lights[ID].SetPosition(position);

	// Update position boxes
	UpdatePosition(ID);
}

void LightDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_boxID);
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
	ON_BN_CLICKED(IDOK, &LightDialogue::OnBnClickedOk)
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
	///ON_BN_CLICKED(IDC_CHECK11, &LightDialogue::OnBnClickedSpawn)
	ON_BN_CLICKED(IDC_CHECK2, &LightDialogue::OnBnClickedTranslate)
	ON_BN_CLICKED(IDC_CHECK23, &LightDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK24, &LightDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK25, &LightDialogue::OnBnClickedZ)
END_MESSAGE_MAP()

// Kill the dialogue
void LightDialogue::End()
{
	m_active = false;
	DestroyWindow();
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
	int type = m_boxType.GetCurSel() + 1;	

	// Set new light type
	m_lights[ID].SetLightType((LIGHT_TYPE)type);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Light has been enabled/disabled
void LightDialogue::OnBnClickedEnable()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Set light enabled/disabled
	m_lights[ID].SetEnabled(IsDlgButtonChecked(IDC_CHECK1));

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
	else { posX = m_lights[ID].GetPosition().x; }

	// Update X position of light
	m_lights[ID].SetPositionX(posX);

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
	else { posY = m_lights[ID].GetPosition().y; }

	// Update Y position of light
	m_lights[ID].SetPositionY(posY);

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
	else { posZ = m_lights[ID].GetPosition().z; }

	// Update Z position of light
	m_lights[ID].SetPositionZ(posZ);

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
	else { dirX = m_lights[ID].GetDirection().x; }

	// Store as vector & normalize
	///DirectX::SimpleMath::Vector3 direction = { dirX, 1.f, 1.f };
	///direction.Normalize();

	// Update X direction of light
	m_lights[ID].SetDirectionX(dirX);

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
	else { dirY = m_lights[ID].GetDirection().y; }

	// Store as vector & normalize
	///DirectX::SimpleMath::Vector3 direction = { 1.f, dirY, 1.f };
	///direction.Normalize();

	// Update Y direction of light
	m_lights[ID].SetDirectionY(dirY);

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
	else { dirZ = m_lights[ID].GetDirection().z; }

	// Store as vector & normalize
	///DirectX::SimpleMath::Vector3 direction = { 1.f, 1.f, dirZ };
	///direction.Normalize();

	// Update Z direction of light
	m_lights[ID].SetDirectionZ(dirZ);

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
	else { difR = m_lights[ID].GetDiffuse().x; }
	
	// Update R diffuse of light
	m_lights[ID].SetDiffuseR(difR);

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
	else { difG = m_lights[ID].GetDiffuse().y; }

	// Update G diffuse of light
	m_lights[ID].SetDiffuseG(difG);

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
	else { difB = m_lights[ID].GetDiffuse().z; }

	// Update B diffuse of light
	m_lights[ID].SetDiffuseB(difB);

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
	else { ambR = m_lights[ID].GetAmbient().x; }
	/*if (ambR >= 4.f) { ambR /= 50.f; }
	else if (ambR >= 3.f) { ambR /= 40.f; }
	else if (ambR >= 2.f) { ambR /= 30.f; }
	else if (ambR >= 1.f) { ambR /= 20.f; }
	else if (ambR >= 0.f) { ambR /= 10.f; }*/
	ambR /= 10.f;

	// Update R ambient of light
	m_lights[ID].SetAmbientR(ambR);

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
	else { ambG = m_lights[ID].GetAmbient().y; }
	/*if (ambG >= 4.f) { ambG /= 50.f; }
	else if (ambG >= 3.f) { ambG /= 40.f; }
	else if (ambG >= 2.f) { ambG /= 30.f; }
	else if (ambG >= 1.f) { ambG /= 20.f; }
	else if (ambG >= 0.f) { ambG /= 10.f; }*/
	ambG /= 10.f;

	// Update G ambient of light
	m_lights[ID].SetAmbientG(ambG);

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
	else { ambB = m_lights[ID].GetAmbient().z; }
	/*if (ambB >= 4.f) { ambB /= 50.f; }
	else if (ambB >= 3.f) { ambB /= 40.f; }
	else if (ambB >= 2.f) { ambB /= 30.f; }
	else if (ambB >= 1.f) { ambB /= 20.f; }
	else if (ambB >= 0.f) { ambB /= 10.f; }*/
	ambB /= 10.f;

	// Update B ambient of light
	m_lights[ID].SetAmbientG(ambB);

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
	else { constA = m_lights[ID].GetConstantAttenuation(); }

	// Update constant attenuation of light
	m_lights[ID].SetConstantAttenuation(constA);

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
	else { linA = m_lights[ID].GetLinearAttenuation(); }
	///linA /= 10.f;

	// Update linear attenuation of light
	m_lights[ID].SetLinearAttenuation(linA);

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
	else { quadA = m_lights[ID].GetQuadraticAttenuation(); }
	///quadA /= 100.f;

	// Update quadratic attenuation of light
	m_lights[ID].SetQuadraticAttenuation(quadA);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Spawn has been selected
void LightDialogue::OnBnClickedSpawn()
{
	// Spawn a light
	ObjectManager::SpawnObject(OBJECT_TYPE::LIGHT, MouseManager::GetBasicSpawn(), m_sceneGraph, m_boxType.GetCurSel(), GetDiffuse(), GetConstA(), GetLinA(), GetQuadA());


}

// Translate has been selected
void LightDialogue::OnBnClickedTranslate()
{
	m_translating = IsDlgButtonChecked(IDC_CHECK2);
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

void LightDialogue::UpdateType(int ID)
{
	// Set combo box
	m_boxType.SetCurSel((int)m_lights[ID].GetLightType() - 1);
}

void LightDialogue::UpdateEnabled(int ID)
{
	// Set enabled check box to match light 
	CheckDlgButton(IDC_CHECK1, m_lights[ID].GetEnabled());
}

void LightDialogue::UpdatePosition(int ID)
{
	// Store current position
	XMFLOAT3 position = m_lights[ID].GetPosition();

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
	XMFLOAT3 direction = m_lights[ID].GetDirection();	

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
	XMFLOAT4 diffuse = m_lights[ID].GetDiffuse();

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
	XMFLOAT4 ambient = m_lights[ID].GetAmbient();

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
	float constA = m_lights[ID].GetConstantAttenuation();

	// Update constant attenuation box
	CString sA; sA.Format(L"%g", constA);
	m_eConstA.SetWindowTextW(sA);
}

void LightDialogue::UpdateLinA(int ID)
{
	// Store current linear attenuation
	float linA = m_lights[ID].GetLinearAttenuation();

	// Update linear attenuation box
	CString sA; sA.Format(L"%g", linA);
	m_eLinA.SetWindowTextW(sA);
}

void LightDialogue::UpdateQuadA(int ID)
{
	// Store current quadratic attenuation
	float quadA = m_lights[ID].GetQuadraticAttenuation();

	// Update linear attenuation box
	CString sA; sA.Format(L"%g", quadA);
	m_eQuadA.SetWindowTextW(sA);
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

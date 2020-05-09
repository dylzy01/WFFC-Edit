// SculptDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "SculptDialogue.h"
#include "afxdialogex.h"


// SculptDialogue dialog

IMPLEMENT_DYNAMIC(SculptDialogue, CDialogEx)

SculptDialogue::SculptDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG11, pParent)
{

}

void SculptDialogue::SetToolData(ToolMain * toolSystem)
{
	// Local storage 
	m_toolSystem = toolSystem;
	
	// Setup checkbox images
	SetupCheckBoxes();

	// Setup sculpt types
	std::wstring sculptBoxEntry;
	sculptBoxEntry = L"N/A";		m_boxSculpt.AddString(sculptBoxEntry.c_str());
	sculptBoxEntry = L"Increase";	m_boxSculpt.AddString(sculptBoxEntry.c_str());
	sculptBoxEntry = L"Flatten";	m_boxSculpt.AddString(sculptBoxEntry.c_str());
	sculptBoxEntry = L"Decrease";	m_boxSculpt.AddString(sculptBoxEntry.c_str());

	// Setup constraint types
	std::wstring constBoxEntry;
	constBoxEntry = L"N/A";			m_boxConst.AddString(constBoxEntry.c_str());
	constBoxEntry = L"X";			m_boxConst.AddString(constBoxEntry.c_str());
	constBoxEntry = L"Y";			m_boxConst.AddString(constBoxEntry.c_str());
	constBoxEntry = L"Z";			m_boxConst.AddString(constBoxEntry.c_str());
	constBoxEntry = L"XY";			m_boxConst.AddString(constBoxEntry.c_str());
	constBoxEntry = L"XZ";			m_boxConst.AddString(constBoxEntry.c_str());
	constBoxEntry = L"YZ";			m_boxConst.AddString(constBoxEntry.c_str());

	// Set both boxes to N/A
	m_boxSculpt.SetCurSel(0);
	m_boxConst.SetCurSel(0);

	// Set initial scale factor
	CString scale; scale.Format(L"%g", toolSystem->GetDisplayChunk()->GetScaleFactor());
	m_eScale.SetWindowTextW(scale);

	// Reset scale limit
	toolSystem->GetDisplayChunk()->SetScaleFactor(1);

	// Set other modes to none
	toolSystem->SetObjectSpawn(OBJECT_TYPE::NA);
	toolSystem->SetObjectFunction(OBJECT_FUNCTION::NA);

	// Set tool editor
	toolSystem->SetEditor(EDITOR_COMPLEX::NA);
}

void SculptDialogue::UpdateTool()
{
	// If user is pressing the right mouse button
	if (m_toolSystem->GetInput()->mouseRight)
	{
		// Sculpt terrain
		TerrainManager::Sculpt(m_sculpt, m_constraint);
	}	
}

void SculptDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO3, m_boxSculpt);
	DDX_Control(pDX, IDC_COMBO4, m_boxConst);
	DDX_Control(pDX, IDC_EDIT19, m_eScale);
}

void SculptDialogue::SetupCheckBoxes()
{
	// Increase
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP7);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetBitmap(bmp);
	bmp.Detach();

	// Flatten
	bmp.LoadBitmap(IDB_BITMAP9);
	((CButton*)GetDlgItem(IDC_CHECK14))->SetBitmap(bmp);
	bmp.Detach();

	// Decrease
	bmp.LoadBitmap(IDB_BITMAP8);
	((CButton*)GetDlgItem(IDC_CHECK15))->SetBitmap(bmp);
	bmp.Detach();

	// X constraint
	bmp.LoadBitmap(IDB_BITMAP10);
	((CButton*)GetDlgItem(IDC_CHECK9))->SetBitmap(bmp);
	bmp.Detach();

	// Y constraint
	bmp.LoadBitmap(IDB_BITMAP11);
	((CButton*)GetDlgItem(IDC_CHECK17))->SetBitmap(bmp);
	bmp.Detach();

	// Z constraint
	bmp.LoadBitmap(IDB_BITMAP12);
	((CButton*)GetDlgItem(IDC_CHECK18))->SetBitmap(bmp);
	bmp.Detach();
}


BEGIN_MESSAGE_MAP(SculptDialogue, CDialogEx)
	ON_COMMAND(IDOK, &SculptDialogue::End)
	ON_COMMAND(IDCANCEL, &SculptDialogue::End)
	ON_BN_CLICKED(IDOK, &SculptDialogue::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO3, &SculptDialogue::OnBnSelchangeSculpt)
	ON_CBN_SELCHANGE(IDC_COMBO4, &SculptDialogue::OnBnSelchangeConstraint)
	ON_BN_CLICKED(IDC_CHECK2, &SculptDialogue::OnBnClickedIncrease)
	ON_BN_CLICKED(IDC_CHECK14, &SculptDialogue::OnBnClickedFlatten)
	ON_BN_CLICKED(IDC_CHECK15, &SculptDialogue::OnBnClickedDecrease)
	ON_BN_CLICKED(IDC_CHECK9, &SculptDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK17, &SculptDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK18, &SculptDialogue::OnBnClickedZ)
	///ON_BN_CLICKED(IDC_CHECK3, &SculptDialogue::OnBnClickedPlateau)
	ON_EN_CHANGE(IDC_EDIT19, &SculptDialogue::OnEnChangeScale)
END_MESSAGE_MAP()


// SculptDialogue message handlers

// Kill the dialogue
void SculptDialogue::End()
{
	Reset();
	ShowWindow(SW_HIDE);
}

// User is finished with dialogue
void SculptDialogue::OnBnClickedOk()
{
	m_active = false;
	End();
	CDialogEx::OnOK();
}

// Sculpt mode has been changed
void SculptDialogue::OnBnSelchangeSculpt()
{
	// Store index of selection
	int index = m_boxSculpt.GetCurSel();

	// Set new sculpt value
	m_sculpt = (TERRAIN_SCULPT)index;

	// Uncheck previous sculpt
	Uncheck();

	// Switch between sculpt and check appropriate box
	switch (m_sculpt)
	{
	case TERRAIN_SCULPT::INCREASE: CheckDlgButton(IDC_CHECK2, true); break;
	case TERRAIN_SCULPT::FLATTEN: CheckDlgButton(IDC_CHECK14, true); break;
	case TERRAIN_SCULPT::DECREASE: CheckDlgButton(IDC_CHECK15, true); break;
	case TERRAIN_SCULPT::NA: CheckDlgButton(IDC_CHECK2, false); CheckDlgButton(IDC_CHECK14, false); CheckDlgButton(IDC_CHECK15, false); break;
	}
}

// Constraint mode has been changed
void SculptDialogue::OnBnSelchangeConstraint()
{
	// Store index of constraint value
	int index = m_boxConst.GetCurSel();

	// Set new constraint
	m_constraint = (CONSTRAINT)index;

	// Switch between constraint and check appropriate boxes
	switch (m_constraint)
	{
	case CONSTRAINT::X: CheckDlgButton(IDC_CHECK9, true); break;
	case CONSTRAINT::Y: CheckDlgButton(IDC_CHECK17, true); break;
	case CONSTRAINT::Z: CheckDlgButton(IDC_CHECK18, true); break;
	case CONSTRAINT::XY: CheckDlgButton(IDC_CHECK9, true); CheckDlgButton(IDC_CHECK17, true); break;
	case CONSTRAINT::XZ: CheckDlgButton(IDC_CHECK9, true); CheckDlgButton(IDC_CHECK18, true); break;
	case CONSTRAINT::YZ: CheckDlgButton(IDC_CHECK17, true); CheckDlgButton(IDC_CHECK18, true); break;
	case CONSTRAINT::NA: CheckDlgButton(IDC_CHECK9, true); CheckDlgButton(IDC_CHECK17, true); CheckDlgButton(IDC_CHECK18, true); break;
	}
}

// Increase mode has been selected
void SculptDialogue::OnBnClickedIncrease()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK2))
	{
	case true:
	{
		// Set sculpt
		m_sculpt = TERRAIN_SCULPT::INCREASE;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset sculpt
		m_sculpt = TERRAIN_SCULPT::NA;
	}
	break;
	}

	// Update all data
	Update();
}

// Flatten mode has been selected
void SculptDialogue::OnBnClickedFlatten()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK14))
	{
	case true:
	{
		// Set sculpt
		m_sculpt = TERRAIN_SCULPT::FLATTEN;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset sculpt
		m_sculpt = TERRAIN_SCULPT::NA;
	}
	break;
	}

	// Update all data
	Update();
}

// Decrease mode has been selected
void SculptDialogue::OnBnClickedDecrease()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK15))
	{
	case true:
	{
		// Set sculpt
		m_sculpt = TERRAIN_SCULPT::DECREASE;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset sculpt
		m_sculpt = TERRAIN_SCULPT::NA;
	}
	break;
	}

	// Update all data
	Update();
}

// X constraint has been selected
void SculptDialogue::OnBnClickedX()
{
	// Switch between checked/unchecked
	m_x = IsDlgButtonChecked(IDC_CHECK9);

	// Update all data
	Update();
}

// Y constraint has been selected
void SculptDialogue::OnBnClickedY()
{
	// Switch between checked/unchecked
	m_y = IsDlgButtonChecked(IDC_CHECK17);

	// Update all data
	Update();
}

// Z constraint has been selected
void SculptDialogue::OnBnClickedZ()
{
	// Switch between checked/unchecked
	m_z = IsDlgButtonChecked(IDC_CHECK18);

	// Update all data
	Update();
}

// Plateau has been selected
//void SculptDialogue::OnBnClickedPlateau()
//{
//	// Enable/disable plateau
//	m_displayChunk->SetPlateau(IsDlgButtonChecked(IDC_CHECK3));
//}

// Scale factor has been changed
void SculptDialogue::OnEnChangeScale()
{
	// Store new limit
	CString string = _T("");
	m_eScale.GetWindowTextW(string);

	// Convert to float
	float scale;
	if (!string.IsEmpty()) { scale = _ttof(string); }
	else { scale = 5.f; }

	// Update limit
	m_toolSystem->GetDisplayChunk()->SetScaleFactor(scale);
}

// Update remaining sculpt details when one is changed //////////////////////////////////////////////

// Uncheck other buttons
void SculptDialogue::Uncheck()
{
	// Switch between current sculpt
	switch (m_sculpt)
	{
	case TERRAIN_SCULPT::INCREASE:
	{
		// Uncheck flatten button
		CheckDlgButton(IDC_CHECK14, false);

		// Uncheck decrease button
		CheckDlgButton(IDC_CHECK15, false);
	}
	break;
	case TERRAIN_SCULPT::FLATTEN:
	{
		// Uncheck increase button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck decrease button
		CheckDlgButton(IDC_CHECK15, false);
	}
	break;
	case TERRAIN_SCULPT::DECREASE:
	{
		// Uncheck increase button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck flatten button
		CheckDlgButton(IDC_CHECK14, false);
	}
	break;
	case TERRAIN_SCULPT::NA:
	{
		// Uncheck increase button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck flatten button
		CheckDlgButton(IDC_CHECK14, false);

		// Uncheck decrease button
		CheckDlgButton(IDC_CHECK15, false);
	}
	break;
	}
}

// Update all data
void SculptDialogue::Update()
{
	// Update sculpt box
	UpdateSelectedSculpt((int)m_sculpt);

	// Update constraint box & apply
	UpdateSelectedConstraint();
}

// Update selected sculpt details
void SculptDialogue::UpdateSelectedSculpt(int sculpt)
{
	m_boxSculpt.SetCurSel(sculpt);
}

// Update selected constraint details
void SculptDialogue::UpdateSelectedConstraint()
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

	// Disable plateau if more than one constraint is active	
	/*GetDlgItem(IDC_CHECK3)->EnableWindow(
		m_constraint != CONSTRAINT::XY ||
		m_constraint != CONSTRAINT::XZ ||
		m_constraint != CONSTRAINT::YZ ||
		m_constraint != CONSTRAINT::NA);*/

	// Update constraint box
	m_boxConst.SetCurSel((int)m_constraint);
}

// Reset all variables
void SculptDialogue::Reset()
{
	m_active = m_x = m_y = m_z = false;
	m_scale = 1.f;

	CheckDlgButton(IDC_CHECK2, false);
	CheckDlgButton(IDC_CHECK14, false);
	CheckDlgButton(IDC_CHECK15, false);
	CheckDlgButton(IDC_CHECK9, false);
	CheckDlgButton(IDC_CHECK17, false);
	CheckDlgButton(IDC_CHECK18, false);
}

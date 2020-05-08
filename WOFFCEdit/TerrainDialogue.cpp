// TerrainDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainDialogue.h"
#include "afxdialogex.h"


// TerrainDialogue dialog

IMPLEMENT_DYNAMIC(TerrainDialogue, CDialogEx)

TerrainDialogue::TerrainDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG9, pParent)
{

}

void TerrainDialogue::SetToolData(ToolMain * toolSystem)
{
	// Local storage
	m_toolSystem = toolSystem;

	// Setup indexes
	for (int i = 0; i < TERRAINRESOLUTION - 1; ++i)
	{
		std::wstring indexBoxEntry;
		indexBoxEntry = std::to_wstring(i);
		m_boxRow.AddString(indexBoxEntry.c_str());
		m_boxCol.AddString(indexBoxEntry.c_str());
	}

	// Setup texture types
	{
		std::wstring texBoxEntry;
		texBoxEntry = L"NA";			m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Grass";			m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Dirt";			m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Sand";			m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Stone";			m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Snow";			m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Grass/Dirt";	m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Grass/Sand";	m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Grass/Stone";	m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Grass/Snow";	m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Dirt/Sand";		m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Dirt/Stone";	m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Dirt/Snow";		m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Sand/Stone";	m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Sand/Snow";		m_boxTex.AddString(texBoxEntry.c_str());
		texBoxEntry = L"Stone/Snow";	m_boxTex.AddString(texBoxEntry.c_str());
	}

	// Setup sculpt types
	{
		std::wstring sculptBoxEntry;
		sculptBoxEntry = L"N/A";		m_boxSculpt.AddString(sculptBoxEntry.c_str());
		sculptBoxEntry = L"Increase";	m_boxSculpt.AddString(sculptBoxEntry.c_str());
		sculptBoxEntry = L"Flatten";	m_boxSculpt.AddString(sculptBoxEntry.c_str());
		sculptBoxEntry = L"Decrease";	m_boxSculpt.AddString(sculptBoxEntry.c_str());
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

	// Set initial scale factor
	CString scale; scale.Format(L"%g", toolSystem->GetDisplayChunk()->GetScaleFactor());
	m_eScale.SetWindowTextW(scale);

	// Reset scale limit
	toolSystem->GetDisplayChunk()->SetScaleFactor(1);

	// Display first terrain
	TERRAIN temp;
	temp.row = 0;
	temp.column = 0;
	UpdateTerrain(temp);

	SetupCheckBoxes();

	// Set other modes to none
	toolSystem->SetObjectSpawn(OBJECT_TYPE::NA);
	toolSystem->SetObjectFunction(OBJECT_FUNCTION::NA);
	toolSystem->SetTerrainPaint(TERRAIN_PAINT::NA);

	// Set tool editor
	toolSystem->SetEditor(EDITOR::SCULPT_SINGLE);
}

void TerrainDialogue::UpdateTool()
{
	// If user is pressing the right mouse button
	if (m_toolSystem->GetInput()->mouseRight)
	{
		// If the user isn't sculpting
		if (!m_sculpting)
		{
			// Set editor to none
			m_toolSystem->SetTerrainSculpt(TERRAIN_SCULPT::NA);

			// Get current terrain selection
			TERRAIN terrain = MouseManager::PickTerrain();

			// If terrain has been intersected
			if (terrain.intersect)
			{
				// Update dialogue data
				UpdateTerrain(terrain);
			}
		}

		// Else, if the user is sculpting
		else
		{
			// Set tool editor
			m_toolSystem->SetEditor(EDITOR::SCULPT_SINGLE);

			// Update tool selected terrrain
			m_toolSystem->SetSelectedTerrain(m_terrain);

			// Set sculpt mode
			m_toolSystem->SetTerrainSculpt(m_sculpt);

			// Set constraint
			m_toolSystem->SetConstraint(m_constraint);
		}
	}
}

// Update current terrain with dialogue values/vice versa
void TerrainDialogue::UpdateTerrain(TERRAIN terrain)
{
	m_terrain = terrain;
	
	m_boxRow.SetCurSel(terrain.row);
	m_boxCol.SetCurSel(terrain.column);
	UpdateCoordinates(terrain.row, terrain.column);
	UpdateTexture(terrain.row, terrain.column);

	// Set sculpt & constraint box to N/A
	m_boxSculpt.SetCurSel(0);
	m_boxConst.SetCurSel(0);
}

void TerrainDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO3, m_boxRow);
	DDX_Control(pDX, IDC_COMBO4, m_boxCol);
	DDX_Control(pDX, IDC_EDIT16, m_eCoordX);
	DDX_Control(pDX, IDC_EDIT17, m_eCoordY);
	DDX_Control(pDX, IDC_EDIT18, m_eCoordZ);
	DDX_Control(pDX, IDC_COMBO1, m_boxTex);
	DDX_Control(pDX, IDC_COMBO5, m_boxSculpt);
	DDX_Control(pDX, IDC_COMBO6, m_boxConst);
	DDX_Control(pDX, IDC_EDIT20, m_eScale);
}

void TerrainDialogue::SetupCheckBoxes()
{
	// Increase
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP7);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetBitmap(bmp);
	bmp.Detach();

	// Flatten
	bmp.LoadBitmap(IDB_BITMAP9);
	((CButton*)GetDlgItem(IDC_CHECK15))->SetBitmap(bmp);
	bmp.Detach();

	// Decrease
	bmp.LoadBitmap(IDB_BITMAP8);
	((CButton*)GetDlgItem(IDC_CHECK16))->SetBitmap(bmp);
	bmp.Detach();

	// X constraint
	bmp.LoadBitmap(IDB_BITMAP10);
	((CButton*)GetDlgItem(IDC_CHECK10))->SetBitmap(bmp);
	bmp.Detach();

	// Y constraint
	bmp.LoadBitmap(IDB_BITMAP11);
	((CButton*)GetDlgItem(IDC_CHECK18))->SetBitmap(bmp);
	bmp.Detach();

	// Z constraint
	bmp.LoadBitmap(IDB_BITMAP12);
	((CButton*)GetDlgItem(IDC_CHECK19))->SetBitmap(bmp);
	bmp.Detach();
}


BEGIN_MESSAGE_MAP(TerrainDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TerrainDialogue::End)
	ON_COMMAND(IDCANCEL, &TerrainDialogue::End)
	ON_BN_CLICKED(IDOK, &TerrainDialogue::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO3, &TerrainDialogue::OnBnSelchangeIndex)
	ON_CBN_SELCHANGE(IDC_COMBO4, &TerrainDialogue::OnBnSelchangeIndex)
	ON_CBN_SELCHANGE(IDC_COMBO1, &TerrainDialogue::OnBnSelchangeTexture)	
	ON_CBN_SELCHANGE(IDC_COMBO5, &TerrainDialogue::OnBnSelchangeSculpt)
	ON_CBN_SELCHANGE(IDC_COMBO6, &TerrainDialogue::OnBnSelchangeConstraint)
	ON_BN_CLICKED(IDC_CHECK2, &TerrainDialogue::OnBnClickedIncrease)
	ON_BN_CLICKED(IDC_CHECK15, &TerrainDialogue::OnBnClickedFlatten)
	ON_BN_CLICKED(IDC_CHECK16, &TerrainDialogue::OnBnClickedDecrease)
	ON_BN_CLICKED(IDC_CHECK10, &TerrainDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK18, &TerrainDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK19, &TerrainDialogue::OnBnClickedZ)
	ON_EN_CHANGE(IDC_EDIT20, &TerrainDialogue::OnEnChangeScale)
END_MESSAGE_MAP()


// TerrainDialogue message handlers

// Kill the dialogue
void TerrainDialogue::End()
{
	Reset();
	ShowWindow(SW_HIDE);
}

// User is finished with dialogue
void TerrainDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// Row or Column has been changed
void TerrainDialogue::OnBnSelchangeIndex()
{
	// Store index selection
	int row = m_boxRow.GetCurSel();
	int col = m_boxCol.GetCurSel();

	// Update coordinates
	UpdateCoordinates(row, col);

	// Update texture type
	UpdateTexture(row, col);
}

// Texture has been selected
void TerrainDialogue::OnBnSelchangeTexture()
{
	// Store index selection
	int row = m_boxRow.GetCurSel();
	int col = m_boxCol.GetCurSel();

	// Store texture selection
	int texture = m_boxTex.GetCurSel();

	// Set new paint
	m_toolSystem->GetDisplayChunk()->OverwritePaint(row, col, (TERRAIN_PAINT)texture);	
}

// Sculpt mode has been changed
void TerrainDialogue::OnBnSelchangeSculpt()
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
	case TERRAIN_SCULPT::FLATTEN: CheckDlgButton(IDC_CHECK15, true); break;
	case TERRAIN_SCULPT::DECREASE: CheckDlgButton(IDC_CHECK16, true); break;
	case TERRAIN_SCULPT::NA: CheckDlgButton(IDC_CHECK2, false); CheckDlgButton(IDC_CHECK15, false); CheckDlgButton(IDC_CHECK16, false); break;
	}
}

// Constraint mode has been changed
void TerrainDialogue::OnBnSelchangeConstraint()
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

// Increase mode has been selected
void TerrainDialogue::OnBnClickedIncrease()
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
	UpdateSculpt();
}

// Flatten mode has been selected
void TerrainDialogue::OnBnClickedFlatten()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK15))
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
	UpdateSculpt();
}

// Decrease mode has been selected
void TerrainDialogue::OnBnClickedDecrease()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK16))
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
	UpdateSculpt();
}

// X constraint has been selected
void TerrainDialogue::OnBnClickedX()
{
	// Switch between checked/unchecked
	m_x = IsDlgButtonChecked(IDC_CHECK10);

	// Update all data
	UpdateSculpt();
}

// Y constraint has been selected
void TerrainDialogue::OnBnClickedY()
{
	// Switch between checked/unchecked
	m_y = IsDlgButtonChecked(IDC_CHECK18);

	// Update all data
	UpdateSculpt();
}

// Z constraint has been selected
void TerrainDialogue::OnBnClickedZ()
{
	// Switch between checked/unchecked
	m_z = IsDlgButtonChecked(IDC_CHECK19);

	// Update all data
	UpdateSculpt();
}

// Scale factor has been changed
void TerrainDialogue::OnEnChangeScale()
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

// Update remaining terrain details when one is changed /////////////////////////////////////////////

void TerrainDialogue::Uncheck()
{
	// Switch between current sculpt
	switch (m_sculpt)
	{
	case TERRAIN_SCULPT::INCREASE:
	{
		m_sculpting = true;

		// Uncheck flatten button
		CheckDlgButton(IDC_CHECK15, false);

		// Uncheck decrease button
		CheckDlgButton(IDC_CHECK16, false);
	}
	break;
	case TERRAIN_SCULPT::FLATTEN:
	{
		m_sculpting = true;

		// Uncheck increase button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck decrease button
		CheckDlgButton(IDC_CHECK16, false);
	}
	break;
	case TERRAIN_SCULPT::DECREASE:
	{
		m_sculpting = true;

		// Uncheck increase button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck flatten button
		CheckDlgButton(IDC_CHECK15, false);
	}
	break;
	case TERRAIN_SCULPT::NA:
	{
		m_sculpting = false;
		
		// Uncheck increase button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck flatten button
		CheckDlgButton(IDC_CHECK15, false);

		// Uncheck decrease button
		CheckDlgButton(IDC_CHECK16, false);
	}
	break;
	}
}

// Display updated coordinates
void TerrainDialogue::UpdateCoordinates(int row, int column)
{
	// Store position of selected terrain
	DirectX::XMFLOAT3 coords = m_toolSystem->GetDisplayChunk()->GetGeometry(row, column).position;

	// Update X coord box
	CString sX; sX.Format(L"%g", coords.x);
	m_eCoordX.SetWindowTextW(sX);

	// Update Y coord box
	CString sY; sY.Format(L"%g", coords.y);
	m_eCoordY.SetWindowTextW(sY);

	// Update Z coord box
	CString sZ; sZ.Format(L"%g", coords.z);
	m_eCoordZ.SetWindowTextW(sZ);
}

// Display updated texture
void TerrainDialogue::UpdateTexture(int row, int column)
{
	// Store texture of selected terrain
	TERRAIN_PAINT paint = m_toolSystem->GetDisplayChunk()->GetPaint(row, column);

	// Set combo box
	m_boxTex.SetCurSel((int)paint);
}

// Handle all sculpt updates
void TerrainDialogue::UpdateSculpt()
{
	// Update sculpt box
	UpdateSelectedSculpt((int)m_sculpt);

	// Update constraint box & apply
	UpdateSelectedConstraint();
}

// Display updated sculpt mode
void TerrainDialogue::UpdateSelectedSculpt(int sculpt)
{
	m_boxSculpt.SetCurSel(sculpt);

	if (sculpt == 0) { m_sculpting = false; }
}

// Display updated constraint
void TerrainDialogue::UpdateSelectedConstraint()
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

void TerrainDialogue::Reset()
{
	m_active = m_sculpting = m_x = m_y = m_z = false;
	m_scale = 1.f;

	m_sculpt = TERRAIN_SCULPT::NA;
	m_constraint = CONSTRAINT::NA;

	CheckDlgButton(IDC_CHECK2, false);
	CheckDlgButton(IDC_CHECK15, false);
	CheckDlgButton(IDC_CHECK16, false);
	CheckDlgButton(IDC_CHECK10, false);
	CheckDlgButton(IDC_CHECK18, false);
	CheckDlgButton(IDC_CHECK19, false);
}

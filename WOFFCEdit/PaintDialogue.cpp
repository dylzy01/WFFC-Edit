// PaintDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "PaintDialogue.h"
#include "afxdialogex.h"


// PaintDialogue dialog

IMPLEMENT_DYNAMIC(PaintDialogue, CDialogEx)

PaintDialogue::PaintDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG10, pParent)
{

}

void PaintDialogue::SetToolData(ToolMain * toolSystem)
{
	// Local storage
	m_toolSystem = toolSystem;

	// Setup checkbox images
	SetupCheckBoxes();

	// Setup paint types
	std::wstring selectBoxEntry;
	selectBoxEntry = L"NA";				m_boxSelect.AddString(selectBoxEntry.c_str());
	selectBoxEntry = L"Grass";			m_boxSelect.AddString(selectBoxEntry.c_str());
	selectBoxEntry = L"Dirt";			m_boxSelect.AddString(selectBoxEntry.c_str());
	selectBoxEntry = L"Sand";			m_boxSelect.AddString(selectBoxEntry.c_str());
	selectBoxEntry = L"Stone";			m_boxSelect.AddString(selectBoxEntry.c_str());
	selectBoxEntry = L"Snow";			m_boxSelect.AddString(selectBoxEntry.c_str());

	// Set other modes to none
	toolSystem->SetObjectSpawn(OBJECT_TYPE::NA);
	toolSystem->SetObjectFunction(OBJECT_FUNCTION::NA);

	// Set tool editor
	toolSystem->SetEditor(EDITOR_COMPLEX::NA);
}

void PaintDialogue::UpdateTool()
{
	// If user is pressing the right mouse button
	if (m_toolSystem->GetInput()->mouseRight)
	{
		// Paint terrain
		TerrainManager::Paint(m_paint);
	}
}

void PaintDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO3, m_boxSelect);
}

void PaintDialogue::SetupCheckBoxes()
{
	// Grass
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP1);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetBitmap(bmp);
	bmp.Detach();

	// Dirt
	bmp.LoadBitmap(IDB_BITMAP2);
	((CButton*)GetDlgItem(IDC_CHECK14))->SetBitmap(bmp);
	bmp.Detach();

	// Sand
	bmp.LoadBitmap(IDB_BITMAP3);
	((CButton*)GetDlgItem(IDC_CHECK15))->SetBitmap(bmp);
	bmp.Detach();

	// Stone
	bmp.LoadBitmap(IDB_BITMAP4);
	((CButton*)GetDlgItem(IDC_CHECK9))->SetBitmap(bmp);
	bmp.Detach();

	// Snow
	bmp.LoadBitmap(IDB_BITMAP5);
	((CButton*)GetDlgItem(IDC_CHECK17))->SetBitmap(bmp);
	bmp.Detach();

	// N/A
	bmp.LoadBitmap(IDB_BITMAP6);
	((CButton*)GetDlgItem(IDC_CHECK18))->SetBitmap(bmp);
	bmp.Detach();
}


BEGIN_MESSAGE_MAP(PaintDialogue, CDialogEx)
	ON_COMMAND(IDOK, &PaintDialogue::End)
	ON_COMMAND(IDCANCEL, &PaintDialogue::End)
	ON_BN_CLICKED(IDOK, &PaintDialogue::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO3, &PaintDialogue::OnBnSelchangePaint)
	ON_BN_CLICKED(IDC_CHECK2, &PaintDialogue::OnBnClickedGrass)
	ON_BN_CLICKED(IDC_CHECK14, &PaintDialogue::OnBnClickedDirt)
	ON_BN_CLICKED(IDC_CHECK15, &PaintDialogue::OnBnClickedSand)
	ON_BN_CLICKED(IDC_CHECK9, &PaintDialogue::OnBnClickedStone)
	ON_BN_CLICKED(IDC_CHECK17, &PaintDialogue::OnBnClickedSnow)
	ON_BN_CLICKED(IDC_CHECK18, &PaintDialogue::OnBnClickedNA)
	ON_BN_CLICKED(IDC_CHECK3, &PaintDialogue::OnBnClickedBlend)

END_MESSAGE_MAP()


// PaintDialogue message handlers

// Kill the dialogue
void PaintDialogue::End()
{
	Reset();
	ShowWindow(SW_HIDE);
}

// User is finished with dialogue
void PaintDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// Select paint has been changed via combo box
void PaintDialogue::OnBnSelchangePaint()
{	
	// Store index of selection
	int index = m_boxSelect.GetCurSel();

	// Set new paint value
	m_paint = (TERRAIN_PAINT)index;

	// Uncheck previous paint
	Uncheck();

	// Switch between paint and check appropriate box
	switch (m_paint)
	{
	case TERRAIN_PAINT::GRASS: CheckDlgButton(IDC_CHECK2, true); break;
	case TERRAIN_PAINT::DIRT: CheckDlgButton(IDC_CHECK14, true); break;
	case TERRAIN_PAINT::SAND: CheckDlgButton(IDC_CHECK15, true); break;
	case TERRAIN_PAINT::STONE: CheckDlgButton(IDC_CHECK9, true); break;
	case TERRAIN_PAINT::SNOW: CheckDlgButton(IDC_CHECK17, true); break;
	case TERRAIN_PAINT::NA: CheckDlgButton(IDC_CHECK18, true); break;
	}
}

// Grass has been selected
void PaintDialogue::OnBnClickedGrass()
{
	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK2))
	{
	case true:
	{
		// Set paint
		m_paint = TERRAIN_PAINT::GRASS;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset paint
		m_paint = TERRAIN_PAINT::NA;

		// Check N/A button & apply
		CheckDlgButton(IDC_CHECK18, true);
		OnBnClickedNA();
	}
	break;
	}

	// Update selected box
	Update((int)m_paint);
}

// Dirt has been selected
void PaintDialogue::OnBnClickedDirt()
{
	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK14))
	{		
	case true: 
	{
		// Set paint
		m_paint = TERRAIN_PAINT::DIRT;
		
		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false: 
	{
		// Reset paint
		m_paint = TERRAIN_PAINT::NA;

		// Check N/A button & apply
		CheckDlgButton(IDC_CHECK18, true);
		OnBnClickedNA();
	}
	break;
	}

	// Update selected box
	Update((int)m_paint);
}

// Sand has been selected
void PaintDialogue::OnBnClickedSand()
{
	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK15))
	{
	case true:
	{
		// Set paint
		m_paint = TERRAIN_PAINT::SAND;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset paint
		m_paint = TERRAIN_PAINT::NA;

		// Check N/A button & apply
		CheckDlgButton(IDC_CHECK18, true);
		OnBnClickedNA();
	}
	break;
	}

	// Update selected box
	Update((int)m_paint);
}

// Stone has been selected
void PaintDialogue::OnBnClickedStone()
{
	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK9))
	{
	case true:
	{
		// Set paint
		m_paint = TERRAIN_PAINT::STONE;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset paint
		m_paint = TERRAIN_PAINT::NA;

		// Check N/A button & apply
		CheckDlgButton(IDC_CHECK18, true);
		OnBnClickedNA();
	}
	break;
	}

	// Update selected box
	Update((int)m_paint);
}

// Snow has been selected
void PaintDialogue::OnBnClickedSnow()
{
	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK17))
	{
	case true:
	{
		// Set paint
		m_paint = TERRAIN_PAINT::SNOW;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset paint
		m_paint = TERRAIN_PAINT::NA;

		// Check N/A button & apply
		CheckDlgButton(IDC_CHECK18, true);
		OnBnClickedNA();
	}
	break;
	}

	// Update selected box
	Update((int)m_paint);
}

// N/A has been selected
void PaintDialogue::OnBnClickedNA()
{
	// If dialogue is checked
	if (IsDlgButtonChecked(IDC_CHECK18))
	{
		// Set paint
		m_paint = TERRAIN_PAINT::NA;

		// Uncheck other buttons
		Uncheck();
	}

	// Update selected box
	Update((int)m_paint);
}

// Blend paints has been checked
void PaintDialogue::OnBnClickedBlend()
{
	// Update blend controller of display chunk based on button state
	m_toolSystem->GetDisplayChunk()->SetBlend(IsDlgButtonChecked(IDC_CHECK3));
}

// Update remaining paint details when one is changed ///////////////////////////////////////////////

// Uncheck other buttons
void PaintDialogue::Uncheck()
{	
	// Switch between current paint
	switch (m_paint)
	{
	case TERRAIN_PAINT::GRASS:
	{
		// Uncheck dirt button
		CheckDlgButton(IDC_CHECK14, false);

		// Uncheck sand button
		CheckDlgButton(IDC_CHECK15, false);

		// Uncheck stone button
		CheckDlgButton(IDC_CHECK9, false);

		// Uncheck snow button
		CheckDlgButton(IDC_CHECK17, false);

		// Uncheck NA button
		CheckDlgButton(IDC_CHECK18, false);
	}
	break;
	case TERRAIN_PAINT::DIRT:
	{
		// Uncheck grass button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck sand button
		CheckDlgButton(IDC_CHECK15, false);

		// Uncheck stone button
		CheckDlgButton(IDC_CHECK9, false);

		// Uncheck snow button
		CheckDlgButton(IDC_CHECK17, false);

		// Uncheck NA button
		CheckDlgButton(IDC_CHECK18, false);
	}
	break;
	case TERRAIN_PAINT::SAND:
	{
		// Uncheck grass button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck dirt button
		CheckDlgButton(IDC_CHECK14, false);

		// Uncheck stone button
		CheckDlgButton(IDC_CHECK9, false);

		// Uncheck snow button
		CheckDlgButton(IDC_CHECK17, false);

		// Uncheck NA button
		CheckDlgButton(IDC_CHECK18, false);
	}
	break;
	case TERRAIN_PAINT::STONE:
	{
		// Uncheck grass button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck dirt button
		CheckDlgButton(IDC_CHECK14, false);

		// Uncheck sand button
		CheckDlgButton(IDC_CHECK15, false);

		// Uncheck snow button
		CheckDlgButton(IDC_CHECK17, false);

		// Uncheck NA button
		CheckDlgButton(IDC_CHECK18, false);
	}
	break;
	case TERRAIN_PAINT::SNOW:
	{
		// Uncheck grass button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck dirt button
		CheckDlgButton(IDC_CHECK14, false);

		// Uncheck sand button
		CheckDlgButton(IDC_CHECK15, false);

		// Uncheck stone button
		CheckDlgButton(IDC_CHECK9, false);

		// Uncheck NA button
		CheckDlgButton(IDC_CHECK18, false);
	}
	break;
	case TERRAIN_PAINT::NA:
	{
		// Uncheck grass button
		CheckDlgButton(IDC_CHECK2, false);

		// Uncheck dirt button
		CheckDlgButton(IDC_CHECK14, false);

		// Uncheck sand button
		CheckDlgButton(IDC_CHECK15, false);

		// Uncheck stone button
		CheckDlgButton(IDC_CHECK9, false);

		// Uncheck snow button
		CheckDlgButton(IDC_CHECK17, false);
	}
	break;
	}
}

// Update dialogue details
void PaintDialogue::Update(int paint)
{
	// Set selected paint combo box
	m_boxSelect.SetCurSel(paint);
}

void PaintDialogue::Reset()
{
	m_active = m_surround = m_blend = false;
	m_paint = TERRAIN_PAINT::NA;

	CheckDlgButton(IDC_CHECK2, false);
	CheckDlgButton(IDC_CHECK14, false);
	CheckDlgButton(IDC_CHECK15, false);
	CheckDlgButton(IDC_CHECK18, false);
	CheckDlgButton(IDC_CHECK17, false);
	CheckDlgButton(IDC_CHECK3, false);
}

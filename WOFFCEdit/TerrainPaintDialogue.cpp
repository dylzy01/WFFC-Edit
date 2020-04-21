// TerrainPaintDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainPaintDialogue.h"
#include "afxdialogex.h"


// TerrainPaintDialogue dialog

IMPLEMENT_DYNAMIC(TerrainPaintDialogue, CDialogEx)

TerrainPaintDialogue::TerrainPaintDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

TerrainPaintDialogue::~TerrainPaintDialogue()
{
}

void TerrainPaintDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void TerrainPaintDialogue::UpdateButtons(bool enable)
{
	// Update paint
	if (enable) { m_paint = LANDSCAPE_PAINT::NA; }

	// Switch between paints
	switch (m_paint)
	{
	case LANDSCAPE_PAINT::GRASS:
	{
		// Enable/disable dirt button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable sand button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable stone button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable snow button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_PAINT::DIRT:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable sand button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable stone button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable snow button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_PAINT::SAND:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable dirt button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable stone button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable snow button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_PAINT::STONE:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable dirt button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable sand button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable snow button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_PAINT::SNOW:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable dirt button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable sand button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable stone button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_PAINT::NA:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable dirt button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable sand button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable stone button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable snow button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	}
}


BEGIN_MESSAGE_MAP(TerrainPaintDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TerrainPaintDialogue::End)
	ON_BN_CLICKED(IDOK, &TerrainPaintDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK5, &TerrainPaintDialogue::OnBnClickedGrass)
	ON_BN_CLICKED(IDC_CHECK6, &TerrainPaintDialogue::OnBnClickedDirt)
	ON_BN_CLICKED(IDC_CHECK7, &TerrainPaintDialogue::OnBnClickedSand)
	ON_BN_CLICKED(IDC_CHECK8, &TerrainPaintDialogue::OnBnClickedStone)
	ON_BN_CLICKED(IDC_CHECK13, &TerrainPaintDialogue::OnBnClickedSnow)
END_MESSAGE_MAP()


// TerrainPaintDialogue message handlers

// Kill the dialogue
void TerrainPaintDialogue::End()
{
	OnBnClickedOk();
	DestroyWindow();
}

// Set paint to none & de-activate
void TerrainPaintDialogue::OnBnClickedOk() 
{ 
	m_active = false; 
	UpdateButtons(true);
	CDialogEx::OnOK();
}

// Set paint to grass
void TerrainPaintDialogue::OnBnClickedGrass() 
{ 
	m_paint = LANDSCAPE_PAINT::GRASS; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK5))
	{
	// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
	// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set paint to dirt
void TerrainPaintDialogue::OnBnClickedDirt() 
{ 
	m_paint = LANDSCAPE_PAINT::DIRT; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK6))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set paint to sand
void TerrainPaintDialogue::OnBnClickedSand() 
{ 
	m_paint = LANDSCAPE_PAINT::SAND; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK7))
	{
	// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
	// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set paint to stone
void TerrainPaintDialogue::OnBnClickedStone() 
{ 
	m_paint = LANDSCAPE_PAINT::STONE; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK8))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set paint to snow
void TerrainPaintDialogue::OnBnClickedSnow() 
{ 
	m_paint = LANDSCAPE_PAINT::SNOW; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK13))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}
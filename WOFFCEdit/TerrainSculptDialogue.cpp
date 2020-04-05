// TerrainSculptDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainSculptDialogue.h"
#include "afxdialogex.h"


// TerrainSculptDialogue dialog

IMPLEMENT_DYNAMIC(TerrainSculptDialogue, CDialogEx)

TerrainSculptDialogue::TerrainSculptDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

TerrainSculptDialogue::~TerrainSculptDialogue()
{
}

void TerrainSculptDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void TerrainSculptDialogue::UpdateButtons(bool enable)
{
	// Update sculpt
	if (enable) { m_sculpt = LANDSCAPE_SCULPT::NA; }
	
	// Switch between sculpts
	switch (m_sculpt)
	{
	case LANDSCAPE_SCULPT::INCREASE:
	{
		// Enable/disable flatten button
		if (!enable) { CheckDlgButton(IDC_CHECK2, enable); }
		GetDlgItem(IDC_CHECK2)->EnableWindow(enable);

		// Enable/disable decrease button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_SCULPT::FLATTEN:
	{
		// Enable/disable increase button
		if (!enable) { CheckDlgButton(IDC_CHECK1, enable); }
		GetDlgItem(IDC_CHECK1)->EnableWindow(enable);
		
		// Enable/disable decrease button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_SCULPT::DECREASE:
	{
		// Enable/disable increase button
		if (!enable) { CheckDlgButton(IDC_CHECK1, enable); }
		GetDlgItem(IDC_CHECK1)->EnableWindow(enable);

		// Enable/disable flatten button
		if (!enable) { CheckDlgButton(IDC_CHECK2, enable); }
		GetDlgItem(IDC_CHECK2)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_SCULPT::NA:
	{
		// Enable/disable increase button
		if (!enable) { CheckDlgButton(IDC_CHECK1, enable); }
		GetDlgItem(IDC_CHECK1)->EnableWindow(enable);

		// Enable/disable flatten button
		if (!enable) { CheckDlgButton(IDC_CHECK2, enable); }
		GetDlgItem(IDC_CHECK2)->EnableWindow(enable);

		// Enable/disable decrease button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);
	}
	break;
	}	
}

BEGIN_MESSAGE_MAP(TerrainSculptDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TerrainSculptDialogue::End)
	ON_BN_CLICKED(IDOK, &TerrainSculptDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &TerrainSculptDialogue::OnBnClickedIncrease)
	ON_BN_CLICKED(IDC_CHECK2, &TerrainSculptDialogue::OnBnClickedFlatten)
	ON_BN_CLICKED(IDC_CHECK7, &TerrainSculptDialogue::OnBnClickedDecrease)
	ON_BN_CLICKED(IDC_CHECK8, &TerrainSculptDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK13, &TerrainSculptDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK14, &TerrainSculptDialogue::OnBnClickedZ)
END_MESSAGE_MAP()


// TerrainSculptDialogue message handlers

// Kill the dialogue
void TerrainSculptDialogue::End()
{
	DestroyWindow();
}

// Set sculpt to none & de-activate
void TerrainSculptDialogue::OnBnClickedOk() 
{ 
	m_active = m_x = m_y = m_z = false;
	UpdateButtons(true);
	CDialogEx::OnOK();
}

// Set sculpt to increase
void TerrainSculptDialogue::OnBnClickedIncrease() 
{ 
	m_sculpt = LANDSCAPE_SCULPT::INCREASE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK1))
	{
	// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
	// Enable other buttons & reset constraints
	case false: UpdateButtons(true); ResetConstraints(); break;
	}
}

// Set sculpt to flatten
void TerrainSculptDialogue::OnBnClickedFlatten() 
{ 
	m_sculpt = LANDSCAPE_SCULPT::FLATTEN;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK2))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons & reset constraints
	case false: UpdateButtons(true); ResetConstraints(); break;
	}
}

// Set sculpt to decrease
void TerrainSculptDialogue::OnBnClickedDecrease() 
{ 
	m_sculpt = LANDSCAPE_SCULPT::DECREASE; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK7))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons & reset constraints
	case false: UpdateButtons(true); ResetConstraints(); break;
	}
}

// Check/uncheck X constraint
void TerrainSculptDialogue::OnBnClickedX() { m_x = IsDlgButtonChecked(IDC_CHECK8); }

// Check/uncheck Y constraint
void TerrainSculptDialogue::OnBnClickedY() { m_y = IsDlgButtonChecked(IDC_CHECK13); }

// Check/uncheck Z constraint
void TerrainSculptDialogue::OnBnClickedZ() { m_z = IsDlgButtonChecked(IDC_CHECK14); }
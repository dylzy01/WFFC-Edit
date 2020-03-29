// LandscapeDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "LandscapeDialogue.h"
#include "afxdialogex.h"


// LandscapeDialogue dialog

IMPLEMENT_DYNAMIC(LandscapeDialogue, CDialogEx)

LandscapeDialogue::LandscapeDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

LandscapeDialogue::~LandscapeDialogue()
{
}

void LandscapeDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// Enable/disable other sculpts
void LandscapeDialogue::UpdateSculpt(LANDSCAPE_SCULPT function, bool enable)
{
	// Switch between functions
	switch (function)
	{
	case LANDSCAPE_SCULPT::INCREASE:
	{
		// Enable/disable decrease function
		m_decrease = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable flatten function
		m_flatten = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_SCULPT::DECREASE:
	{
		// Enable/disable increase function
		m_increase = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);
		
		// Enable/disable flatten function
		m_flatten = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_SCULPT::FLATTEN:
	{
		// Enable/disable increase function
		m_increase = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable decrease function
		m_decrease = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);
	}
	break;
	}
}

// Enable/disable other paints
void LandscapeDialogue::UpdatePaint(LANDSCAPE_PAINT paint, bool enable)
{
	// Switch between paints
	switch (paint)
	{
	case LANDSCAPE_PAINT::GRASS:
	{
		// Enable/disable dirt paint
		m_dirt = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable sand paint
		m_sand = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		GetDlgItem(IDC_CHECK14)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_PAINT::DIRT:
	{
		// Enable/disable grass paint
		m_grass = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK12, enable); }
		GetDlgItem(IDC_CHECK12)->EnableWindow(enable);
		
		// Enable/disable sand paint
		m_sand = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		GetDlgItem(IDC_CHECK14)->EnableWindow(enable);
	}
	break;
	case LANDSCAPE_PAINT::SAND:
	{		
		// Enable/disable grass paint
		m_grass = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK12, enable); }
		GetDlgItem(IDC_CHECK12)->EnableWindow(enable);

		// Enable/disable dirt paint
		m_dirt = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	}
}

BEGIN_MESSAGE_MAP(LandscapeDialogue, CDialogEx)
	ON_COMMAND(IDOK, &LandscapeDialogue::End)
	ON_BN_CLICKED(IDOK, &LandscapeDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK12, &LandscapeDialogue::OnBnClickedGrass)
	ON_BN_CLICKED(IDC_CHECK13, &LandscapeDialogue::OnBnClickedDirt)
	ON_BN_CLICKED(IDC_CHECK14, &LandscapeDialogue::OnBnClickedSand)
	ON_BN_CLICKED(IDC_CHECK6, &LandscapeDialogue::OnBnClickedIncrease)	
	ON_BN_CLICKED(IDC_CHECK7, &LandscapeDialogue::OnBnClickedDecrease)
	ON_BN_CLICKED(IDC_CHECK8, &LandscapeDialogue::OnBnClickedFlatten)
	ON_BN_CLICKED(IDC_CHECK3, &LandscapeDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK4, &LandscapeDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK5, &LandscapeDialogue::OnBnClickedZ)
END_MESSAGE_MAP()


// LandscapeDialogue message handlers

// Destroy window & set unactive
void LandscapeDialogue::End()
{
	m_active = m_increase = m_decrease = m_flatten = false;
	DestroyWindow();
}

void LandscapeDialogue::OnBnClickedOk()
{
	// Parent
	CDialogEx::OnOK();

	// Save object manipulation...
}

// Set painting to grass
void LandscapeDialogue::OnBnClickedGrass()
{
	// Set editor to paint
	m_editor = EDITOR::LANDSCAPE_PAINT;

	// Check/uncheck button
	m_grass = IsDlgButtonChecked(IDC_CHECK12);

	// Switch between checked/unchecked
	switch (m_grass)
	{
	case true:
	{
		// Uncheck & disable other paints
		UpdatePaint(LANDSCAPE_PAINT::GRASS, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other paints
		UpdatePaint(LANDSCAPE_PAINT::GRASS, true);
	}
	break;
	}
}

// Set painting to dirt
void LandscapeDialogue::OnBnClickedDirt()
{
	// Set editor to paint
	m_editor = EDITOR::LANDSCAPE_PAINT;

	// Check/uncheck button
	m_dirt = IsDlgButtonChecked(IDC_CHECK13);
	
	// Switch between checked/unchecked
	switch (m_dirt)
	{
	case true:
	{
		// Uncheck & disable other paints
		UpdatePaint(LANDSCAPE_PAINT::DIRT, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other paints
		UpdatePaint(LANDSCAPE_PAINT::DIRT, true);
	}
	break;
	}
}

// Set painting to sand
void LandscapeDialogue::OnBnClickedSand()
{
	// Set editor to paint
	m_editor = EDITOR::LANDSCAPE_PAINT;

	// Check/uncheck button
	m_sand = IsDlgButtonChecked(IDC_CHECK14);
	
	// Switch between checked/unchecked
	switch (m_sand)
	{
	case true:
	{
		// Uncheck & disable other paints
		UpdatePaint(LANDSCAPE_PAINT::SAND, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other paints
		UpdatePaint(LANDSCAPE_PAINT::SAND, true);
	}
	break;
	}
}

// Set sculpting to increase
void LandscapeDialogue::OnBnClickedIncrease()
{
	// Set editor to sculpt
	m_editor = EDITOR::LANDSCAPE_SCULPT;
	
	// Check/uncheck button
	m_increase = IsDlgButtonChecked(IDC_CHECK6);
	
	// Switch between checked/unchecked
	switch (m_increase)
	{
	case true:
	{
		// Uncheck & disable other functions
		UpdateSculpt(LANDSCAPE_SCULPT::INCREASE, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateSculpt(LANDSCAPE_SCULPT::INCREASE, true);
	}
	break;
	}
}

// Set sculpting to decrease
void LandscapeDialogue::OnBnClickedDecrease()
{
	// Set editor to sculpt
	m_editor = EDITOR::LANDSCAPE_SCULPT;
	
	// Check/uncheck button
	m_decrease = IsDlgButtonChecked(IDC_CHECK7);
	
	// Switch between checked/unchecked
	switch (m_decrease)
	{
	case true:
	{
		// Uncheck & disable other functions
		UpdateSculpt(LANDSCAPE_SCULPT::DECREASE, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateSculpt(LANDSCAPE_SCULPT::DECREASE, true);
	}
	break;
	}
}

// Set sculpting to flatten
void LandscapeDialogue::OnBnClickedFlatten()
{
	// Set editor to sculpt
	m_editor = EDITOR::LANDSCAPE_SCULPT;
	
	// Check/uncheck button
	m_flatten = IsDlgButtonChecked(IDC_CHECK8);
		
	// Switch between checked/unchecked
	switch (m_flatten)
	{
	case true:
	{
		// Uncheck & disable other functions
		UpdateSculpt(LANDSCAPE_SCULPT::FLATTEN, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateSculpt(LANDSCAPE_SCULPT::FLATTEN, true);
	}
	break;
	}
}

// Constrain sculpting to X axis
void LandscapeDialogue::OnBnClickedX()
{
	// Check/uncheck constraint
	m_x = IsDlgButtonChecked(IDC_CHECK3);
}

// Constrain sculpting to Y axis
void LandscapeDialogue::OnBnClickedY()
{
	// Check/uncheck constraint
	m_y = IsDlgButtonChecked(IDC_CHECK4);
}

// Constrain sculpting to Z axis
void LandscapeDialogue::OnBnClickedZ()
{
	// Check/uncheck constraint
	m_z = IsDlgButtonChecked(IDC_CHECK5);
}

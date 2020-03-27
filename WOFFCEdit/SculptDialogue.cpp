// SculptDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "SculptDialogue.h"
#include "afxdialogex.h"


// SculptDialogue dialog

IMPLEMENT_DYNAMIC(SculptDialogue, CDialogEx)

SculptDialogue::SculptDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

SculptDialogue::~SculptDialogue()
{
}

void SculptDialogue::SetTerrainData(std::vector<SceneObject>* sceneGraph, std::vector<TERRAIN>* terrain)
{
	m_sceneGraph = sceneGraph;
	m_terrain = terrain;
	m_active = true;
}

void SculptDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// Enable/disable other functions
void SculptDialogue::UpdateFunctions(SCULPT_FUNCTION function, bool enable)
{
	// Switch between functions
	switch (function)
	{
	case SCULPT_FUNCTION::INCREASE:
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
	case SCULPT_FUNCTION::DECREASE:
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
	case SCULPT_FUNCTION::FLATTEN:
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

BEGIN_MESSAGE_MAP(SculptDialogue, CDialogEx)
	ON_COMMAND(IDOK, &SculptDialogue::End)
	ON_BN_CLICKED(IDOK, &SculptDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK6, &SculptDialogue::OnBnClickedIncrease)	
	ON_BN_CLICKED(IDC_CHECK7, &SculptDialogue::OnBnClickedDecrease)
	ON_BN_CLICKED(IDC_CHECK8, &SculptDialogue::OnBnClickedFlatten)
	ON_BN_CLICKED(IDC_CHECK3, &SculptDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK4, &SculptDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK5, &SculptDialogue::OnBnClickedZ)
END_MESSAGE_MAP()


// SculptDialogue message handlers

// Destroy window & set unactive
void SculptDialogue::End()
{
	m_active = m_increase = m_decrease = m_flatten = false;
	DestroyWindow();
}

void SculptDialogue::OnBnClickedOk()
{
	// Parent
	CDialogEx::OnOK();

	// Save object manipulation...
}

// Set sculpting to increase
void SculptDialogue::OnBnClickedIncrease()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK6))
	{
	case true:
	{
		// Check button
		m_increase = true;

		// Uncheck & disable other functions
		UpdateFunctions(SCULPT_FUNCTION::INCREASE, false);
	}
	break;
	case false:
	{
		// Uncheck button
		m_increase = false;

		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateFunctions(SCULPT_FUNCTION::INCREASE, true);
	}
	break;
	}
}

// Set sculpting to decrease
void SculptDialogue::OnBnClickedDecrease()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK7))
	{
	case true:
	{
		// Check button
		m_decrease = true;

		// Uncheck & disable other functions
		UpdateFunctions(SCULPT_FUNCTION::DECREASE, false);
	}
	break;
	case false:
	{
		// Uncheck button
		m_decrease = false;

		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateFunctions(SCULPT_FUNCTION::DECREASE, true);
	}
	break;
	}
}

// Set sculpting to flatten
void SculptDialogue::OnBnClickedFlatten()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK8))
	{
	case true:
	{
		// Check button
		m_flatten = true;

		// Uncheck & disable other functions
		UpdateFunctions(SCULPT_FUNCTION::FLATTEN, false);
	}
	break;
	case false:
	{
		// Uncheck button
		m_flatten = false;

		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateFunctions(SCULPT_FUNCTION::FLATTEN, true);
	}
	break;
	}
}

// Constrain sculpting to X axis
void SculptDialogue::OnBnClickedX()
{
	// Check/uncheck constraint
	m_x = IsDlgButtonChecked(IDC_CHECK3);
}

// Constrain sculpting to Y axis
void SculptDialogue::OnBnClickedY()
{
	// Check/uncheck constraint
	m_y = IsDlgButtonChecked(IDC_CHECK4);
}

// Constrain sculpting to Z axis
void SculptDialogue::OnBnClickedZ()
{
	// Check/uncheck constraint
	m_z = IsDlgButtonChecked(IDC_CHECK5);
}

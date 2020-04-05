// ResidentialObjectDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "ResidentialObjectDialogue.h"
#include "afxdialogex.h"


// ResidentialObjectDialogue dialog

IMPLEMENT_DYNAMIC(ResidentialObjectDialogue, CDialogEx)

ResidentialObjectDialogue::ResidentialObjectDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{

}

ResidentialObjectDialogue::~ResidentialObjectDialogue()
{
}

void ResidentialObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void ResidentialObjectDialogue::UpdateButtons(bool enable)
{
	// Update spawn
	if (enable) { m_residence = RESIDENTIAL_SPAWN::NA; }

	// Switch between spawns
	switch (m_residence)
	{
	case RESIDENTIAL_SPAWN::HOUSE_ONE:
	{
		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
	}
	break;
	case RESIDENTIAL_SPAWN::HOUSE_TWO:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		
		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
	}
	break;
	case RESIDENTIAL_SPAWN::CAVE:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
	}
	break;
	case RESIDENTIAL_SPAWN::NA:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
	}
	break;
	}
}


BEGIN_MESSAGE_MAP(ResidentialObjectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ResidentialObjectDialogue::End)
	ON_BN_CLICKED(IDOK, &ResidentialObjectDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK8, &ResidentialObjectDialogue::OnBnClickedHouseOne)
	ON_BN_CLICKED(IDC_CHECK13, &ResidentialObjectDialogue::OnBnClickedHouseTwo)
	ON_BN_CLICKED(IDC_CHECK14, &ResidentialObjectDialogue::OnBnClickedCave)
END_MESSAGE_MAP()


// ResidentialObjectDialogue message handlers

// Kill the dialogue
void ResidentialObjectDialogue::End()
{
	DestroyWindow();
}

// Set spawn to none & deactivate
void ResidentialObjectDialogue::OnBnClickedOk()
{
	m_active = false;
	UpdateButtons(false);
	CDialog::OnOK();
}

// Set spawn to house #1
void ResidentialObjectDialogue::OnBnClickedHouseOne()
{
	m_residence = RESIDENTIAL_SPAWN::HOUSE_ONE;

	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK8))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to house #2
void ResidentialObjectDialogue::OnBnClickedHouseTwo()
{
	m_residence = RESIDENTIAL_SPAWN::HOUSE_TWO;

	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK13))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to cave
void ResidentialObjectDialogue::OnBnClickedCave()
{
	m_residence = RESIDENTIAL_SPAWN::CAVE;

	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK14))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}
// NatureObjectDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "NatureObjectDialogue.h"
#include "afxdialogex.h"


// NatureObjectDialogue dialog

IMPLEMENT_DYNAMIC(NatureObjectDialogue, CDialogEx)

NatureObjectDialogue::NatureObjectDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG9, pParent)
{

}

NatureObjectDialogue::~NatureObjectDialogue()
{
}

void NatureObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void NatureObjectDialogue::UpdateButtons(bool enable)
{
	// Update spawn
	if (enable) { m_nature = NATURE_SPAWN::NA; }

	// Switch between spawns
	switch (m_nature)
	{
	case NATURE_SPAWN::GRASS:
	{
		// Enable/disable bush button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable palm tree button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable pine tree button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		GetDlgItem(IDC_CHECK15)->EnableWindow(enable);
	}
	break;
	case NATURE_SPAWN::BUSH:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable palm tree button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable pine tree button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		GetDlgItem(IDC_CHECK15)->EnableWindow(enable);
	}
	break;
	case NATURE_SPAWN::TREE_ONE:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable bush button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable pine tree button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		GetDlgItem(IDC_CHECK15)->EnableWindow(enable);
	}
	break;
	case NATURE_SPAWN::TREE_TWO:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable bush button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable palm tree button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		GetDlgItem(IDC_CHECK15)->EnableWindow(enable);
	}
	break;
	case NATURE_SPAWN::WATER:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable bush button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable palm tree button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable pine tree button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		GetDlgItem(IDC_CHECK16)->EnableWindow(enable);
	}
	break;
	case NATURE_SPAWN::NA:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable bush button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable palm tree button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable pine tree button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		GetDlgItem(IDC_CHECK15)->EnableWindow(enable);
	}
	break;
	}
}


BEGIN_MESSAGE_MAP(NatureObjectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &NatureObjectDialogue::End)
	ON_BN_CLICKED(IDOK, &NatureObjectDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK8, &NatureObjectDialogue::OnBnClickedGrass)
	ON_BN_CLICKED(IDC_CHECK13, &NatureObjectDialogue::OnBnClickedBush)
	ON_BN_CLICKED(IDC_CHECK14, &NatureObjectDialogue::OnBnClickedTreeOne)
	ON_BN_CLICKED(IDC_CHECK16, &NatureObjectDialogue::OnBnClickedTreeTwo)
	ON_BN_CLICKED(IDC_CHECK15, &NatureObjectDialogue::OnBnClickedWater)
END_MESSAGE_MAP()


// NatureObjectDialogue message handlers

// Kill the dialogue
void NatureObjectDialogue::End()
{
	DestroyWindow();
}

// Set spawn to none and de-activate
void NatureObjectDialogue::OnBnClickedOk()
{
	m_active = false;
	UpdateButtons(false);
	CDialog::OnOK();
}

// Set spawn to grass
void NatureObjectDialogue::OnBnClickedGrass()
{
	m_nature = NATURE_SPAWN::GRASS;

	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK8))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to bush
void NatureObjectDialogue::OnBnClickedBush()
{
	m_nature = NATURE_SPAWN::BUSH;

	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK13))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to palm tree
void NatureObjectDialogue::OnBnClickedTreeOne()
{
	m_nature = NATURE_SPAWN::TREE_ONE;

	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK14))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to pine tree
void NatureObjectDialogue::OnBnClickedTreeTwo()
{
	m_nature = NATURE_SPAWN::TREE_TWO;

	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK16))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to water
void NatureObjectDialogue::OnBnClickedWater()
{
	m_nature = NATURE_SPAWN::WATER;

	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK15))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

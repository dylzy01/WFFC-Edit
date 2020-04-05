// ObjectSpawnDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectSpawnDialogue.h"
#include "afxdialogex.h"


// ObjectSpawnDialogue dialog

IMPLEMENT_DYNAMIC(ObjectSpawnDialogue, CDialogEx)

ObjectSpawnDialogue::ObjectSpawnDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

ObjectSpawnDialogue::~ObjectSpawnDialogue()
{
}

void ObjectSpawnDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void ObjectSpawnDialogue::UpdateButtons(bool enable)
{
	// Update spawn
	if (enable) { m_spawn = OBJECT_SPAWN::NA; }

	// Switch between spawns
	switch (m_spawn)
	{
	case OBJECT_SPAWN::GRASS:
	{
		// Enable/disable tree button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable house button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	case OBJECT_SPAWN::TREE_ONE:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable house button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	case OBJECT_SPAWN::WATER:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable house button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	//case OBJECT_SPAWN::HOUSE:
	//{
	//	// Enable/disable grass button
	//	if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
	//	GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

	//	// Enable/disable tree button
	//	if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
	//	GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

	//	// Enable/disable water button
	//	if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
	//	GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

	//	// Enable/disable cave button
	//	if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
	//	GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	//}
	//break;
	case OBJECT_SPAWN::CAVE:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable house button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case OBJECT_SPAWN::NA:
	{
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable house button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);
	}
	break;
	}
}


BEGIN_MESSAGE_MAP(ObjectSpawnDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ObjectSpawnDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectSpawnDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK5, &ObjectSpawnDialogue::OnBnClickedGrass)
	ON_BN_CLICKED(IDC_CHECK6, &ObjectSpawnDialogue::OnBnClickedTree)
	ON_BN_CLICKED(IDC_CHECK7, &ObjectSpawnDialogue::OnBnClickedWater)
	ON_BN_CLICKED(IDC_BUTTON1, &ObjectSpawnDialogue::OnBnClickedResidential)
	ON_BN_CLICKED(IDC_BUTTON2, &ObjectSpawnDialogue::OnBnClickedNature)
	///ON_BN_CLICKED(IDC_CHECK14, &ObjectSpawnDialogue::OnBnClickedCave)
END_MESSAGE_MAP()


// ObjectSpawnDialogue message handlers

// Kill the dialogue
void ObjectSpawnDialogue::End()
{
	DestroyWindow();
}

// Set spawn to none & de-activate
void ObjectSpawnDialogue::OnBnClickedOk() 
{ 
	m_active = false;
	UpdateButtons(true);
	CDialogEx::OnOK();
}

// Set spawn to grass
void ObjectSpawnDialogue::OnBnClickedGrass() 
{ 
	m_spawn = OBJECT_SPAWN::GRASS; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK5))
	{
	// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
	// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to tree
void ObjectSpawnDialogue::OnBnClickedTree() 
{ 
	m_spawn = OBJECT_SPAWN::TREE_ONE; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK6))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to water
void ObjectSpawnDialogue::OnBnClickedWater() 
{ 
	m_spawn = OBJECT_SPAWN::WATER; 

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK7))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to house
void ObjectSpawnDialogue::OnBnClickedHouse()
{
	//m_spawn = OBJECT_SPAWN::HOUSE;

	//// Switch between button checked/unchecked
	//switch (IsDlgButtonChecked(IDC_CHECK8))
	//{
	//	// Uncheck & disable other buttons
	//case true: UpdateButtons(false); break;
	//	// Enable other buttons
	//case false: UpdateButtons(true); break;
	//}
}

// Set spawn to cave
void ObjectSpawnDialogue::OnBnClickedCave()
{
	m_spawn = OBJECT_SPAWN::CAVE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK13))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Open residential spawner
void ObjectSpawnDialogue::OnBnClickedResidential()
{
	m_spawn = OBJECT_SPAWN::RESIDENTIAL;
	
	// Create & display dialogue window
	m_residentialObjectDialogue.Create(IDD_DIALOG8);
	m_residentialObjectDialogue.ShowWindow(SW_SHOW);
	m_residentialObjectDialogue.SetActive(true);
}

// Open nature spawner
void ObjectSpawnDialogue::OnBnClickedNature()
{
	m_spawn = OBJECT_SPAWN::NATURE;

	// Create & display dialogue window
	m_natureObjectDialogue.Create(IDD_DIALOG9);
	m_natureObjectDialogue.ShowWindow(SW_SHOW);
	m_natureObjectDialogue.SetActive(true);
}

// Open prop spawner
void ObjectSpawnDialogue::OnBnClickedProps()
{
}

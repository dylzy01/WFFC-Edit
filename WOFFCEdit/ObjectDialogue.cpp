// ObjectDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectDialogue.h"
#include "afxdialogex.h"


// ObjectDialogue dialog

IMPLEMENT_DYNAMIC(ObjectDialogue, CDialogEx)

ObjectDialogue::ObjectDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

ObjectDialogue::~ObjectDialogue()
{
}

void ObjectDialogue::SetObjectData(std::vector<SceneObject>* sceneGraph, std::vector<int>* objectIDs)
{
	m_sceneGraph = sceneGraph;
	m_objectIDs = objectIDs;
	m_active = true;
}

void ObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// Enable/disable other functions
void ObjectDialogue::UpdateFunctions(OBJECT_FUNCTION function, bool enable)
{
	// Switch between functions
	switch (function)
	{
	case OBJECT_FUNCTION::NA:
	{
		// Enable/disable translate function
		m_translate = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable rotate function
		m_rotate = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable scale function
		m_scale = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case OBJECT_FUNCTION::TRANSLATE:
	{
		// Enable/disable rotate function
		m_rotate = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable scale function
		m_scale = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case OBJECT_FUNCTION::ROTATE:
	{
		// Enable/disable translate function
		m_translate = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable scale function
		m_scale = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case OBJECT_FUNCTION::SCALE:
	{
		// Enable/disable translate function
		m_translate = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable rotate function
		m_rotate = enable;
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);
	}
	break;
	}
}

// Enable/disable other spawns
void ObjectDialogue::UpdateSpawns(OBJECT_SPAWN spawn, bool enable)
{
}

BEGIN_MESSAGE_MAP(ObjectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ObjectDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK9, &ObjectDialogue::OnBnClickedCube)
	ON_BN_CLICKED(IDC_CHECK8, &ObjectDialogue::OnBnClickedScale)
	ON_BN_CLICKED(IDC_CHECK6, &ObjectDialogue::OnBnClickedTranslate)
	ON_BN_CLICKED(IDC_CHECK7, &ObjectDialogue::OnBnClickedRotate)
	ON_BN_CLICKED(IDC_CHECK3, &ObjectDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK4, &ObjectDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK5, &ObjectDialogue::OnBnClickedZ)
END_MESSAGE_MAP()


// ObjectDialogue message handlers

// Destroy window & set unactive
void ObjectDialogue::End()
{
	m_active = m_translate = m_rotate = m_scale = false;
	DestroyWindow();
}

void ObjectDialogue::OnBnClickedOk()
{
	// Parent
	CDialogEx::OnOK();

	// Save object manipulation...
}

// Set spawn to cube
void ObjectDialogue::OnBnClickedCube()
{
	// Set spawning
	m_spawn = true;
	
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK9))
	{
	case true:
	{
		// Check button
		m_cube = true;

		// Uncheck & disable other spawns
		UpdateSpawns(OBJECT_SPAWN::CUBE, false);

		// Uncheck & disable other functions
		UpdateFunctions(OBJECT_FUNCTION::NA, false);
	}
	break;
	case false:
	{
		// Uncheck button
		m_cube = false;
		
		// Enable other spawns
		UpdateSpawns(OBJECT_SPAWN::CUBE, true);

		// Enable other functions
		UpdateFunctions(OBJECT_FUNCTION::NA, true);
	}
	break;
	}
}

// Set object manipulation to scaling
void ObjectDialogue::OnBnClickedScale()
{
	// Set not spawning
	m_spawn = false;
	
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK8))
	{
	case true:
	{
		// Check button
		m_scale = true;

		// Uncheck & disable other functions
		UpdateFunctions(OBJECT_FUNCTION::SCALE, false);
	}
	break;
	case false:
	{
		// Uncheck button
		m_scale = false;

		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateFunctions(OBJECT_FUNCTION::SCALE, true);
	}
	break;
	}
}

// Set object manipulation to translating
void ObjectDialogue::OnBnClickedTranslate()
{
	// Set not spawning
	m_spawn = false;
	
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK6))
	{
	case true:
	{
		// Check button
		m_translate = true;

		// Uncheck & disable other functions
		UpdateFunctions(OBJECT_FUNCTION::TRANSLATE, false);
	}
	break;
	case false:
	{
		// Uncheck button
		m_translate = false;

		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateFunctions(OBJECT_FUNCTION::TRANSLATE, true);
	}
	break;
	}
}

// Set object manipulation to rotating
void ObjectDialogue::OnBnClickedRotate()
{
	// Set not spawning
	m_spawn = false;
	
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK7))
	{
	case true:
	{
		// Check button
		m_rotate = true;

		// Uncheck & disable other functions
		UpdateFunctions(OBJECT_FUNCTION::ROTATE, false);
	}
	break;
	case false:
	{
		// Uncheck button
		m_rotate = false;

		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateFunctions(OBJECT_FUNCTION::ROTATE, true);
	}
	break;
	}
}

// Constrain object manipulation to X axis
void ObjectDialogue::OnBnClickedX()
{
	// Set not spawning
	m_spawn = false;
	
	// Check/uncheck constraint
	m_x = IsDlgButtonChecked(IDC_CHECK3);
}

// Constrain object manipulation to Y axis
void ObjectDialogue::OnBnClickedY()
{
	// Set not spawning
	m_spawn = false;
	
	// Check/uncheck constraint
	m_y = IsDlgButtonChecked(IDC_CHECK4);
}

// Constrain object manipulation to Z axis
void ObjectDialogue::OnBnClickedZ()
{
	// Set not spawning
	m_spawn = false;
	
	// Check/uncheck constraint
	m_z = IsDlgButtonChecked(IDC_CHECK5);
}

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
}

void ObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// Enable/disable other functions
void ObjectDialogue::UpdateTransform(OBJECT_FUNCTION transform, bool enable)
{
	if (enable) { m_transform = OBJECT_FUNCTION::NA; }
	else { m_transform = transform; }
	
	// Switch between functions
	switch (transform)
	{
	case OBJECT_FUNCTION::NA:
	{
		// Enable/disable translate function
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable rotate function
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable scale function
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case OBJECT_FUNCTION::TRANSLATE:
	{
		// Enable/disable rotate function
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable scale function
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case OBJECT_FUNCTION::ROTATE:
	{
		// Enable/disable translate function
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable scale function
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);
	}
	break;
	case OBJECT_FUNCTION::SCALE:
	{
		// Enable/disable translate function
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable rotate function
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);
	}
	break;
	}
}

// Enable/disable other spawns
void ObjectDialogue::UpdateSpawns(OBJECT_SPAWN spawn, bool enable)
{
	if (enable) { m_spawn = OBJECT_SPAWN::NA; }
	else { m_spawn = spawn; }
	
	// Switch between spawns
	switch (spawn)
	{
	case OBJECT_SPAWN::GRASS:
	{
		// Enable/disable tree spawn
		if (!enable) { CheckDlgButton(IDC_CHECK10, enable); }
		GetDlgItem(IDC_CHECK10)->EnableWindow(enable);
		
		// Enable/disable water spawn
		if (!enable) { CheckDlgButton(IDC_CHECK11, enable); }
		GetDlgItem(IDC_CHECK11)->EnableWindow(enable);
	}
	break;
	case OBJECT_SPAWN::TREE_ONE:
	{
		// Enable/disable grass spawn
		if (!enable) { CheckDlgButton(IDC_CHECK9, enable); }
		GetDlgItem(IDC_CHECK9)->EnableWindow(enable);

		// Enable/disable water spawn
		if (!enable) { CheckDlgButton(IDC_CHECK11, enable); }
		GetDlgItem(IDC_CHECK11)->EnableWindow(enable);
	}
	break;
	case OBJECT_SPAWN::WATER:
	{
		// Enable/disable grass spawn
		if (!enable) { CheckDlgButton(IDC_CHECK9, enable); }
		GetDlgItem(IDC_CHECK9)->EnableWindow(enable);

		// Enable/disable tree spawn
		if (!enable) { CheckDlgButton(IDC_CHECK10, enable); }
		GetDlgItem(IDC_CHECK10)->EnableWindow(enable);
	}
	break;
	}
}

BEGIN_MESSAGE_MAP(ObjectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ObjectDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK9, &ObjectDialogue::OnBnClickedGrass)
	ON_BN_CLICKED(IDC_CHECK10, &ObjectDialogue::OnBnClickedTree)
	ON_BN_CLICKED(IDC_CHECK11, &ObjectDialogue::OnBnClickedWater)
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
	m_active = false;
	ResetConstraints();
	DestroyWindow();
}

// Close window
void ObjectDialogue::OnBnClickedOk()
{
	// Parent
	CDialogEx::OnOK();

	// Save object manipulation... ?

	// End window
	End();
}

// Set spawn to grass
void ObjectDialogue::OnBnClickedGrass()
{
	// Set spawning
	m_editor = EDITOR::OBJECT_SPAWN;
	
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK9))
	{
	case true:
	{
		// Uncheck & disable other spawns
		UpdateSpawns(OBJECT_SPAWN::GRASS, false);

		// Uncheck & disable other functions
		UpdateTransform(OBJECT_FUNCTION::NA, false);
	}
	break;
	case false:
	{		
		// Enable other spawns
		UpdateSpawns(OBJECT_SPAWN::GRASS, true);

		// Enable other functions
		UpdateTransform(OBJECT_FUNCTION::NA, true);
	}
	break;
	}
}

// Set spawn to tree
void ObjectDialogue::OnBnClickedTree()
{
	// Set spawning
	m_editor = EDITOR::OBJECT_SPAWN;

	// Switch between check/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK10))
	{
	case true:
	{
		// Uncheck & disable other spawns
		UpdateSpawns(OBJECT_SPAWN::TREE_ONE, false);

		// Uncheck & disable other functions
		UpdateTransform(OBJECT_FUNCTION::NA, false);
	}
	break;
	case false:
	{
		// Enable other spawns
		UpdateSpawns(OBJECT_SPAWN::TREE_ONE, true);

		// Enable other functions
		UpdateTransform(OBJECT_FUNCTION::NA, true);
	}
	break;
	}
}

// Set spawn to water
void ObjectDialogue::OnBnClickedWater()
{
	// Set spawning
	m_editor = EDITOR::OBJECT_SPAWN;

	// Switch between check/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK11))
	{
	case true:
	{
		// Uncheck & disable other spawns
		UpdateSpawns(OBJECT_SPAWN::WATER, false);

		// Uncheck & disable other functions
		UpdateTransform(OBJECT_FUNCTION::NA, false);
	}
	break;
	case false:
	{
		// Enable other spawns
		UpdateSpawns(OBJECT_SPAWN::WATER, true);

		// Enable other functions
		UpdateTransform(OBJECT_FUNCTION::NA, true);
	}
	break;
	}
}

// Set object manipulation to scaling
void ObjectDialogue::OnBnClickedScale()
{
	// Set transforming
	m_editor = EDITOR::OBJECT_TRANSFORM;
	
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK8))
	{
	case true:
	{
		// Uncheck & disable other functions
		UpdateTransform(OBJECT_FUNCTION::SCALE, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateTransform(OBJECT_FUNCTION::SCALE, true);
	}
	break;
	}
}

// Set object manipulation to translating
void ObjectDialogue::OnBnClickedTranslate()
{
	// Set transforming
	m_editor = EDITOR::OBJECT_TRANSFORM;
	
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK6))
	{
	case true:
	{
		// Uncheck & disable other functions
		UpdateTransform(OBJECT_FUNCTION::TRANSLATE, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateTransform(OBJECT_FUNCTION::TRANSLATE, true);
	}
	break;
	}
}

// Set object manipulation to rotating
void ObjectDialogue::OnBnClickedRotate()
{
	// Set transforming
	m_editor = EDITOR::OBJECT_TRANSFORM;
	
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK7))
	{
	case true:
	{
		// Uncheck & disable other functions
		UpdateTransform(OBJECT_FUNCTION::ROTATE, false);
	}
	break;
	case false:
	{
		// Uncheck any active constraints
		ResetConstraints();

		// Enable other functions
		UpdateTransform(OBJECT_FUNCTION::ROTATE, true);
	}
	break;
	}
}

// Constrain object manipulation to X axis
void ObjectDialogue::OnBnClickedX()
{
	// Set transforming
	m_editor = EDITOR::OBJECT_TRANSFORM;
	
	// Check/uncheck constraint
	m_x = IsDlgButtonChecked(IDC_CHECK3);
}

// Constrain object manipulation to Y axis
void ObjectDialogue::OnBnClickedY()
{
	// Set transforming
	m_editor = EDITOR::OBJECT_TRANSFORM;
	
	// Check/uncheck constraint
	m_y = IsDlgButtonChecked(IDC_CHECK4);
}

// Constrain object manipulation to Z axis
void ObjectDialogue::OnBnClickedZ()
{
	// Set transforming
	m_editor = EDITOR::OBJECT_TRANSFORM;
	
	// Check/uncheck constraint
	m_z = IsDlgButtonChecked(IDC_CHECK5);
}

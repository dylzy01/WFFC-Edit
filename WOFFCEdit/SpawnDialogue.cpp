// SpawnDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "SpawnDialogue.h"
#include "afxdialogex.h"


// SpawnDialogue dialog

IMPLEMENT_DYNAMIC(SpawnDialogue, CDialogEx)

SpawnDialogue::SpawnDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG13, pParent)
{

}

void SpawnDialogue::SetToolData(ToolMain * toolSystem)
{
	// Local storage
	m_toolSystem = toolSystem;
	
	// Setup spawn types
	{
		std::wstring typeBoxEntry;
		typeBoxEntry = L"House 1";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"House 2";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Cave";			m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Bridge";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Fence";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Boat";			m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Grass";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Palm Tree";	m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Pine Tree";	m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Cube";			m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Cylinder";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Cone";			m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Light";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Water";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"N/A";			m_boxType.AddString(typeBoxEntry.c_str());
	}

	SetupCheckBoxes();

	// Set other modes to none
	toolSystem->SetObjectFunction(OBJECT_FUNCTION::NA);
	toolSystem->SetTerrainSculpt(TERRAIN_SCULPT::NA);
	toolSystem->SetTerrainPaint(TERRAIN_PAINT::NA);

	// Set tool editor
	toolSystem->SetEditor(EDITOR::OBJECT_SPAWN);
}

void SpawnDialogue::UpdateTool()
{
	// Keep spawn mode up-to-date
	if (m_toolSystem->GetObjectSpawn() != m_spawner) { m_toolSystem->SetObjectSpawn(m_spawner); }
}

void SpawnDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_boxType);
}

void SpawnDialogue::SetupCheckBoxes()
{
}


BEGIN_MESSAGE_MAP(SpawnDialogue, CDialogEx)
	ON_COMMAND(IDOK, &SpawnDialogue::End)
	ON_COMMAND(IDCANCEL, &SpawnDialogue::End)
	ON_BN_CLICKED(IDOK, &SpawnDialogue::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &SpawnDialogue::OnCbnSelchangeType)
	ON_BN_CLICKED(IDC_CHECK26, &SpawnDialogue::OnBnClickedHouseOne)
	ON_BN_CLICKED(IDC_CHECK27, &SpawnDialogue::OnBnClickedHouseTwo)
	ON_BN_CLICKED(IDC_CHECK28, &SpawnDialogue::OnBnClickedCave)
	ON_BN_CLICKED(IDC_CHECK29, &SpawnDialogue::OnBnClickedGrass)
	ON_BN_CLICKED(IDC_CHECK30, &SpawnDialogue::OnBnClickedTreeOne)
	ON_BN_CLICKED(IDC_CHECK31, &SpawnDialogue::OnBnClickedTreeTwo)
	ON_BN_CLICKED(IDC_CHECK32, &SpawnDialogue::OnBnClickedBridge)
	ON_BN_CLICKED(IDC_CHECK33, &SpawnDialogue::OnBnClickedFence)
	ON_BN_CLICKED(IDC_CHECK34, &SpawnDialogue::OnBnClickedBoat)
	ON_BN_CLICKED(IDC_CHECK35, &SpawnDialogue::OnBnClickedCube)
	ON_BN_CLICKED(IDC_CHECK36, &SpawnDialogue::OnBnClickedCylinder)
	ON_BN_CLICKED(IDC_CHECK37, &SpawnDialogue::OnBnClickedCone)
	ON_BN_CLICKED(IDC_CHECK38, &SpawnDialogue::OnBnClickedLight)
	ON_BN_CLICKED(IDC_CHECK39, &SpawnDialogue::OnBnClickedWater)
END_MESSAGE_MAP()


// SpawnDialogue message handlers

// Kill the dialogue
void SpawnDialogue::End()
{
	Reset();
	ShowWindow(SW_HIDE);
}

// User is finished with dialogue
void SpawnDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// Spawn type has been changed
void SpawnDialogue::OnCbnSelchangeType()
{
	// Store type selection
	int index = m_boxType.GetCurSel();

	// Set new spawner value
	m_spawner = (OBJECT_TYPE)index;

	// Uncheck previous spawn
	Uncheck();

	// Switch between spawner and check appropriate box
	switch (m_spawner)
	{
	case OBJECT_TYPE::HOUSE_ONE:	CheckDlgButton(IDC_CHECK26, true); break;
	case OBJECT_TYPE::HOUSE_TWO:	CheckDlgButton(IDC_CHECK27, true); break;
	case OBJECT_TYPE::CAVE:			CheckDlgButton(IDC_CHECK28, true); break;
	case OBJECT_TYPE::GRASS:		CheckDlgButton(IDC_CHECK29, true); break;
	case OBJECT_TYPE::TREE_ONE:		CheckDlgButton(IDC_CHECK30, true); break;
	case OBJECT_TYPE::TREE_TWO:		CheckDlgButton(IDC_CHECK31, true); break;
	case OBJECT_TYPE::BRIDGE:		CheckDlgButton(IDC_CHECK32, true); break;
	case OBJECT_TYPE::FENCE:		CheckDlgButton(IDC_CHECK33, true); break;
	case OBJECT_TYPE::BOAT:			CheckDlgButton(IDC_CHECK34, true); break;
	case OBJECT_TYPE::CUBE:			CheckDlgButton(IDC_CHECK35, true); break;
	case OBJECT_TYPE::CYLINDER:		CheckDlgButton(IDC_CHECK36, true); break;
	case OBJECT_TYPE::CONE:			CheckDlgButton(IDC_CHECK37, true); break;
	case OBJECT_TYPE::LIGHT:		CheckDlgButton(IDC_CHECK38, true); break;
	case OBJECT_TYPE::WATER:		CheckDlgButton(IDC_CHECK39, true); break;
	}
}

// House one has been selected
void SpawnDialogue::OnBnClickedHouseOne()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK26))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::HOUSE_ONE);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// House two has been selected
void SpawnDialogue::OnBnClickedHouseTwo()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK27))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::HOUSE_TWO);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Cave has been selected
void SpawnDialogue::OnBnClickedCave()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK28))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::CAVE);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Grass has been selected
void SpawnDialogue::OnBnClickedGrass()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK29))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::GRASS);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Tree one has been selected
void SpawnDialogue::OnBnClickedTreeOne()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK30))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::TREE_ONE);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Tree two has been selected
void SpawnDialogue::OnBnClickedTreeTwo()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK31))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::TREE_TWO);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Bridge has been selected
void SpawnDialogue::OnBnClickedBridge()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK32))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::BRIDGE);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Fence has been selected
void SpawnDialogue::OnBnClickedFence()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK33))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::FENCE);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Boat has been selected
void SpawnDialogue::OnBnClickedBoat()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK34))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::BOAT);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Cube has been selected
void SpawnDialogue::OnBnClickedCube()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK35))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::CUBE);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Cylinder has been selected
void SpawnDialogue::OnBnClickedCylinder()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK36))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::CYLINDER);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Cone has been selected
void SpawnDialogue::OnBnClickedCone()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK37))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::CONE);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Light has been selected
void SpawnDialogue::OnBnClickedLight()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK38))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::LIGHT);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Water has been selected
void SpawnDialogue::OnBnClickedWater()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK39))
	{
	case true:
	{
		// Update spawner
		Update((int)OBJECT_TYPE::WATER);

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset spawner
		Update((int)OBJECT_TYPE::NA);
	}
	break;
	}

	// Update all data
	UpdateTypeBox();
}

// Update remaining spawn details when one is changed ///////////////////////////////////////////////

void SpawnDialogue::Update(int type)
{
	m_spawner = (OBJECT_TYPE)type;
}

void SpawnDialogue::UpdateTypeBox()
{
	// Update type box
	m_boxType.SetCurSel((int)m_spawner);
}

void SpawnDialogue::Uncheck()
{
	// Switch between spawner and check appropriate box
	switch (m_spawner)
	{
	case OBJECT_TYPE::HOUSE_ONE:
	{
		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::HOUSE_TWO:
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::CAVE:			
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::GRASS:		
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::TREE_ONE:		
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::TREE_TWO:		
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::BRIDGE:		
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::FENCE:		
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::BOAT:			
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::CUBE:			
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::CYLINDER:		
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::CONE:			
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::LIGHT:		
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	case OBJECT_TYPE::WATER:		
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);
	}
	break;
	case OBJECT_TYPE::NA:
	{
		// Uncheck house one
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck house two
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck cave
		CheckDlgButton(IDC_CHECK28, false);

		// Uncheck grass
		CheckDlgButton(IDC_CHECK29, false);

		// Uncheck tree one
		CheckDlgButton(IDC_CHECK30, false);

		// Uncheck tree two
		CheckDlgButton(IDC_CHECK31, false);

		// Uncheck bridge
		CheckDlgButton(IDC_CHECK32, false);

		// Uncheck fence
		CheckDlgButton(IDC_CHECK33, false);

		// Uncheck boat
		CheckDlgButton(IDC_CHECK34, false);

		// Uncheck cube
		CheckDlgButton(IDC_CHECK35, false);

		// Uncheck cylinder
		CheckDlgButton(IDC_CHECK36, false);

		// Uncheck cone
		CheckDlgButton(IDC_CHECK37, false);

		// Uncheck light
		CheckDlgButton(IDC_CHECK38, false);

		// Uncheck water
		CheckDlgButton(IDC_CHECK39, false);
	}
	break;
	}
}

void SpawnDialogue::Reset()
{
	m_active = false;
	m_spawner = OBJECT_TYPE::NA;

	CheckDlgButton(IDC_CHECK26, false);
	CheckDlgButton(IDC_CHECK27, false);
	CheckDlgButton(IDC_CHECK28, false);
	CheckDlgButton(IDC_CHECK29, false);
	CheckDlgButton(IDC_CHECK30, false);
	CheckDlgButton(IDC_CHECK31, false);
	CheckDlgButton(IDC_CHECK32, false);
	CheckDlgButton(IDC_CHECK33, false);
	CheckDlgButton(IDC_CHECK34, false);
	CheckDlgButton(IDC_CHECK35, false);
	CheckDlgButton(IDC_CHECK36, false);
	CheckDlgButton(IDC_CHECK37, false);
	CheckDlgButton(IDC_CHECK38, false);
	CheckDlgButton(IDC_CHECK39, false);
}
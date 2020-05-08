// ObjectDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectDialogue.h"
#include "afxdialogex.h"


// ObjectDialogue dialog

IMPLEMENT_DYNAMIC(ObjectDialogue, CDialogEx)

ObjectDialogue::ObjectDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG12, pParent)
{
	m_function = OBJECT_FUNCTION::SELECT;
	m_constraint = CONSTRAINT::NA;
}

void ObjectDialogue::SetToolSystem(ToolMain * toolSystem)
{
	// Local storage
	m_toolSystem = toolSystem;
	m_selectedIDs = toolSystem->GetSelectedIDs();

	// Setup IDs of currently available objects
	SetupObjects(&toolSystem->GetSceneGraph());

	// Setup object types
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
		typeBoxEntry = L"Water";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"";				m_boxType.AddString(typeBoxEntry.c_str());
	}

	// Setup constraint types
	{
		std::wstring constBoxEntry;
		constBoxEntry = L"N/A";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"X";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"Y";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"Z";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"XY";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"XZ";			m_boxConst.AddString(constBoxEntry.c_str());
		constBoxEntry = L"YZ";			m_boxConst.AddString(constBoxEntry.c_str());
	}

	SetupCheckBoxes();

	// Set other modes to none
	m_toolSystem->SetObjectSpawn(OBJECT_TYPE::NA);
	m_toolSystem->SetTerrainSculpt(TERRAIN_SCULPT::NA);
	m_toolSystem->SetTerrainPaint(TERRAIN_PAINT::NA);

	// Set tool editor
	m_toolSystem->SetEditor(EDITOR::OBJECT_FUNCTION);
}

void ObjectDialogue::UpdateTool()
{
	// If selected IDs should be updated
	if (m_select) { m_toolSystem->SetSelectedObjectIDs(m_selectedIDs); m_select = false; }
	else { m_selectedIDs = m_toolSystem->GetSelectedIDs(); }

	// If there's been a new selection
	if (m_toolSystem->GetNewSelection()) {
		UpdateDialogue();
	}

	// If should focus on an object
	if (m_focus)
	{
		// Setup temp focus ID
		int focusID = -1;

		// If more than one object is selected
		if (m_toolSystem->GetSelectedIDs().size() > 1)
		{
			// Define object ID to focus on
			focusID = m_focusDialogue.GetSelectedIndex();
		}

		// Else, if just one object is selected
		else if (m_toolSystem->GetSelectedIDs().size() == 1)
		{
			// Define object ID to focus on
			focusID = m_toolSystem->GetSelectedIDs()[0];
		}

		// Setup object ID to focus on
		m_toolSystem->SetFocus(focusID);
	}
	else { m_toolSystem->SetFocus(-1); }

	// If tool function isn't up-to-date
	if (m_toolSystem->GetObjectFunction() != m_function) { m_toolSystem->SetObjectFunction(m_function); }

	// If tool constraint isn't up-to-date
	if (m_toolSystem->GetConstraint() != m_constraint) { m_toolSystem->SetConstraint(m_constraint); }
}

void ObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_boxID);

	DDX_Control(pDX, IDC_COMBO2, m_boxType);
	DDX_Control(pDX, IDC_COMBO6, m_boxConst);

	DDX_Control(pDX, IDC_EDIT1, m_eScaX);		
	DDX_Control(pDX, IDC_EDIT2, m_eScaY);		
	DDX_Control(pDX, IDC_EDIT3, m_eScaZ);		

	DDX_Control(pDX, IDC_EDIT4, m_eRotX);		
	DDX_Control(pDX, IDC_EDIT5, m_eRotY);		
	DDX_Control(pDX, IDC_EDIT6, m_eRotZ);		

	DDX_Control(pDX, IDC_EDIT7, m_ePosX);		
	DDX_Control(pDX, IDC_EDIT8, m_ePosY);		
	DDX_Control(pDX, IDC_EDIT9, m_ePosZ);	

	DDX_Control(pDX, IDC_EDIT10, m_eSnap);
}

void ObjectDialogue::SetupCheckBoxes()
{	
	// X constraint
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP10);
	((CButton*)GetDlgItem(IDC_CHECK23))->SetBitmap(bmp);
	bmp.Detach();

	// Y constraint
	bmp.LoadBitmap(IDB_BITMAP11);
	((CButton*)GetDlgItem(IDC_CHECK24))->SetBitmap(bmp);
	bmp.Detach();

	// Z constraint
	bmp.LoadBitmap(IDB_BITMAP12);
	((CButton*)GetDlgItem(IDC_CHECK25))->SetBitmap(bmp);
	bmp.Detach();
}


BEGIN_MESSAGE_MAP(ObjectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ObjectDialogue::End)
	ON_COMMAND(IDCANCEL, &ObjectDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectDialogue::OnBnClickedOk)

	ON_CBN_SELCHANGE(IDC_LIST1, &ObjectDialogue::OnCbnSelchangeID)
	ON_CBN_SELCHANGE(IDC_COMBO2, &ObjectDialogue::OnCbnSelchangeType)
	ON_BN_CLICKED(IDC_CHECK1, &ObjectDialogue::OnBnClickedFocus)

	ON_EN_CHANGE(IDC_EDIT1, &ObjectDialogue::OnEnChangeScaX)
	ON_EN_CHANGE(IDC_EDIT2, &ObjectDialogue::OnEnChangeScaY)
	ON_EN_CHANGE(IDC_EDIT3, &ObjectDialogue::OnEnChangeScaZ)

	ON_EN_CHANGE(IDC_EDIT4, &ObjectDialogue::OnEnChangeRotX)
	ON_EN_CHANGE(IDC_EDIT5, &ObjectDialogue::OnEnChangeRotY)
	ON_EN_CHANGE(IDC_EDIT6, &ObjectDialogue::OnEnChangeRotZ)

	ON_EN_CHANGE(IDC_EDIT7, &ObjectDialogue::OnEnChangePosX)
	ON_EN_CHANGE(IDC_EDIT8, &ObjectDialogue::OnEnChangePosY)
	ON_EN_CHANGE(IDC_EDIT9, &ObjectDialogue::OnEnChangePosZ)

	ON_BN_CLICKED(IDC_CHECK26, &ObjectDialogue::OnBnClickedScale)
	ON_BN_CLICKED(IDC_CHECK27, &ObjectDialogue::OnBnClickedRotate)
	ON_BN_CLICKED(IDC_CHECK28, &ObjectDialogue::OnBnClickedTranslate)

	ON_BN_CLICKED(IDC_CHECK23, &ObjectDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK24, &ObjectDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK25, &ObjectDialogue::OnBnClickedZ)

	ON_BN_CLICKED(IDC_CHECK4, &ObjectDialogue::OnBnClickedSnapTerrain)
	ON_BN_CLICKED(IDC_CHECK5, &ObjectDialogue::OnBnClickedSnapValue)
	ON_EN_CHANGE(IDC_EDIT10, &ObjectDialogue::OnEnChangeSnapValue)

	ON_BN_CLICKED(IDC_BUTTON1, &ObjectDialogue::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON2, &ObjectDialogue::OnBnClickedDuplicate)
END_MESSAGE_MAP()


// ObjectDialogue message handlers

// Kill the dialogue
void ObjectDialogue::End()
{
	Reset();
	ShowWindow(SW_HIDE);
}

// User is finished with dialogue
void ObjectDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// ID has been selected
void ObjectDialogue::OnCbnSelchangeID()
{
	// Store ID selection
	int ID = m_boxID.GetCurSel();

	// Get text selection
	CString current;
	m_boxID.GetText(ID, current);

	// Controller to check if selection exists
	bool exists = false;

	// Loop through selection
	for (int i = 0; i < m_selectedIDs.size(); ++i)
	{
		// If current already exists
		if (m_selectedIDs.at(i) == _ttoi(current))
		{
			// Inform remaining functionality the selection exists
			exists = true;

			// Remove from selection
			m_selectedIDs.erase(m_selectedIDs.begin() + i);
		}
	}

	// If selection doesn't exist, add to selection
	if (!exists) { 
		m_selectedIDs.push_back(_ttoi(current));
		m_select = true;
	}

	// Update object type box
	UpdateType();

	// Update object scale boxes
	UpdateScale();

	// Update object rotation boxes
	UpdateRotation();
	
	// Update object position boxes
	UpdatePosition();
}

// Type has been changed
void ObjectDialogue::OnCbnSelchangeType()
{	
	// If function has been called by user
	if (!m_internal)
	{		
		// Loop through selected IDs
		for (int i = 0; i < m_selectedIDs.size(); ++i)
		{
			// Loop through objects
			for (int j = 0; j < m_objects->size(); ++j)
			{
				// If object ID matches selected ID
				if (m_objects->at(j).ID == m_selectedIDs.at(i))
				{
					// Store type selection
					int type = m_boxType.GetCurSel();

					// If type is different from current type
					if (m_objects->at(j).m_type != (OBJECT_TYPE)type)
					{
						// Replace object type
						m_objects->at(j).m_type = (OBJECT_TYPE)type;
						ObjectManager::ReplaceType(m_objects->at(j));
					}
					
					break;
				}					
			}
		}		
	}	
}

// Object should be focussed/unfocussed
void ObjectDialogue::OnBnClickedFocus()
{
	m_focus = IsDlgButtonChecked(IDC_CHECK1);

	// If focus is checked
	if (m_focus)
	{
		// If there's more than one object selected
		if (m_selectedIDs.size() > 1)
		{			
			// Setup focus modal dialogue
			if (!m_focusCreated) { m_focusDialogue.Create(IDD_DIALOG2); m_focusCreated = true; }
			m_focusDialogue.ShowWindow(SW_SHOW);
			m_focusDialogue.SetActive(true);
			m_focusDialogue.SetFocusData(&m_selectedIDs);
		}
	}
}

// X scale has been changed
void ObjectDialogue::OnEnChangeScaX()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_boxID.GetCurSel() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new X scale
						CString string = _T("");
						m_eScaX.GetWindowTextW(string);

						// Convert to float
						float scaX;
						if (!string.IsEmpty()) { scaX = _ttof(string); }
						else { scaX = m_objects->at(j).scaX; }

						// If scale is different from current scale
						if (m_objects->at(j).scaX != scaX)
						{
							// Update X scale of object
							m_objects->at(j).scaX = scaX;
							UpdateObject(m_objects->at(j));
						}

						break;
					}
				}
			}
		}
	}
}

// Y scale has been changed
void ObjectDialogue::OnEnChangeScaY()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_boxID.GetCurSel() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new Y scale
						CString string = _T("");
						m_eScaY.GetWindowTextW(string);

						// Convert to float
						float scaY;
						if (!string.IsEmpty()) { scaY = _ttof(string); }
						else { scaY = m_objects->at(j).scaY; }

						// If scale is different from current scale
						if (m_objects->at(j).scaY != scaY)
						{
							// Update Y scale of object
							m_objects->at(j).scaY = scaY;
							UpdateObject(m_objects->at(j));
						}

						break;
					}
				}
			}
		}
	}
}

// Z scale has been changed
void ObjectDialogue::OnEnChangeScaZ()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_boxID.GetCurSel() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new Z scale
						CString string = _T("");
						m_eScaZ.GetWindowTextW(string);

						// Convert to float
						float scaZ;
						if (!string.IsEmpty()) { scaZ = _ttof(string); }
						else { scaZ = m_objects->at(j).scaZ; }

						// If scale is different from current scale
						if (m_objects->at(j).scaZ != scaZ)
						{
							// Update Z scale of object
							m_objects->at(j).scaZ = scaZ;
							UpdateObject(m_objects->at(j));
						}

						break;
					}
				}
			}
		}
	}
}

// X rotation has been changed
void ObjectDialogue::OnEnChangeRotX()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_boxID.GetCurSel() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new X rotation
						CString string = _T("");
						m_eRotX.GetWindowTextW(string);

						// Convert to float
						float rotX;
						if (!string.IsEmpty()) { rotX = _ttof(string); }
						else { rotX = m_objects->at(j).rotX; }

						// If rotation is different from current rotation
						if (m_objects->at(j).rotX != rotX)
						{
							// Update X rotation of object
							m_objects->at(j).rotX = rotX;
							UpdateObject(m_objects->at(j));
						}

						break;
					}
				}
			}
		}
	}
}

// Y rotation has been changed
void ObjectDialogue::OnEnChangeRotY()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_boxID.GetCurSel() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new Y rotation
						CString string = _T("");
						m_eRotY.GetWindowTextW(string);

						// Convert to float
						float rotY;
						if (!string.IsEmpty()) { rotY = _ttof(string); }
						else { rotY = m_objects->at(j).rotY; }

						// If rotation is different from current rotation
						if (m_objects->at(j).rotY != rotY)
						{
							// Update Y rotation of object
							m_objects->at(j).rotY = rotY;
							UpdateObject(m_objects->at(j));
						}

						break;
					}
				}
			}
		}
	}
}

// Z rotation has been changed
void ObjectDialogue::OnEnChangeRotZ()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_boxID.GetCurSel() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new Z rotation
						CString string = _T("");
						m_eRotZ.GetWindowTextW(string);

						// Convert to float
						float rotZ;
						if (!string.IsEmpty()) { rotZ = _ttof(string); }
						else { rotZ = m_objects->at(j).rotZ; }

						// If rotation is different from current rotation
						if (m_objects->at(j).rotZ != rotZ)
						{
							// Update Z rotation of object
							m_objects->at(j).rotZ = rotZ;
							UpdateObject(m_objects->at(j));
						}

						break;
					}
				}
			}
		}
	}
}

// X position has been changed
void ObjectDialogue::OnEnChangePosX()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_boxID.GetCurSel() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new X position
						CString string = _T("");
						m_ePosX.GetWindowTextW(string);

						// Convert to float
						float posX;
						if (!string.IsEmpty()) { posX = _ttof(string); }
						else { posX = m_objects->at(j).posX; }

						// If position is different from current position
						if (m_objects->at(j).posX != posX)
						{
							// Update X position of object
							m_objects->at(j).posX = posX;
							UpdateObject(m_objects->at(j));
						}

						break;
					}
				}
			}
		}
	}
}

// Y position has been changed
void ObjectDialogue::OnEnChangePosY()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_selectedIDs.size() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new Y position
						CString string = _T("");
						m_ePosY.GetWindowTextW(string);

						// Convert to float
						float posY;
						if (!string.IsEmpty()) { posY = _ttof(string); }
						else { posY = m_objects->at(j).posY; }

						// If position is different from current position
						if (m_objects->at(j).posY != posY)
						{
							// Update Y position of object
							m_objects->at(j).posY = posY;
							UpdateObject(m_objects->at(j));
						}						
						
						break;
					}
				}
			}
		}
	}
}

// Z position has been changed
void ObjectDialogue::OnEnChangePosZ()
{
	// If function has been called by user
	if (!m_internal)
	{
		// If selection is valid
		if (m_boxID.GetCurSel() >= 0)
		{
			// Loop through selected IDs
			for (int i = 0; i < m_selectedIDs.size(); ++i)
			{
				// Loop through objects
				for (int j = 0; j < m_objects->size(); ++j)
				{
					// If object ID matches selected ID
					if (m_objects->at(j).ID == m_selectedIDs.at(i))
					{
						// Store new Z position
						CString string = _T("");
						m_ePosX.GetWindowTextW(string);

						// Convert to float
						float posZ;
						if (!string.IsEmpty()) { posZ = _ttof(string); }
						else { posZ = m_objects->at(j).posZ; }

						// If position is different from current position
						if (m_objects->at(j).posZ != posZ)
						{
							// Update Z position of object
							m_objects->at(j).posZ = posZ;
							UpdateObject(m_objects->at(j));
						}

						break;
					}
				}
			}
		}
	}
}

// Scale transform has been selected
void ObjectDialogue::OnBnClickedScale()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK26))
	{
	case true:
	{
		// Set function
		m_function = OBJECT_FUNCTION::SCALE;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset function
		m_function = OBJECT_FUNCTION::SELECT;
	}
	break;
	}

	// Update constraint text description
	UpdateSelectedConstraint();
}

// Rotate transform has been selected
void ObjectDialogue::OnBnClickedRotate()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK27))
	{
	case true:
	{
		// Set function
		m_function = OBJECT_FUNCTION::ROTATE;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset function
		m_function = OBJECT_FUNCTION::SELECT;
	}
	break;
	}

	// Update constraint text description
	UpdateSelectedConstraint();
}

// Translate transform has been selected
void ObjectDialogue::OnBnClickedTranslate()
{
	// Switch between checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK28))
	{
	case true:
	{
		// Set function
		m_function = OBJECT_FUNCTION::TRANSLATE;

		// Uncheck other buttons
		Uncheck();
	}
	break;
	case false:
	{
		// Reset function
		m_function = OBJECT_FUNCTION::SELECT;
	}
	break;
	}

	// Update constraint text description
	UpdateSelectedConstraint();
}

// X constraint has been selected
void ObjectDialogue::OnBnClickedX()
{
	// Switch between checked/unchecked
	m_x = IsDlgButtonChecked(IDC_CHECK23);

	// Update scale data
	UpdateSelectedConstraint();
}

// Y constraint has been selected
void ObjectDialogue::OnBnClickedY()
{
	// Switch between checked/unchecked
	m_y = IsDlgButtonChecked(IDC_CHECK24);

	// Update scale data
	UpdateSelectedConstraint();
}

// Z constraint has been selected
void ObjectDialogue::OnBnClickedZ()
{
	// Switch between checked/unchecked
	m_z = IsDlgButtonChecked(IDC_CHECK25);

	// Update scale data
	UpdateSelectedConstraint();
}

// Snap to terrain has been selected
void ObjectDialogue::OnBnClickedSnapTerrain()
{
	m_snapTerrain = IsDlgButtonChecked(IDC_CHECK4);

	// Switch between checked/unchecked
	switch (m_snapTerrain)
	{
	case true:
	{
		// Disable snap by value
		GetDlgItem(IDC_CHECK5)->EnableWindow(false);
		m_eSnap.EnableWindow(false);
	}
	break;
	case false:
	{
		// Enable snap by value
		GetDlgItem(IDC_CHECK5)->EnableWindow(true);
		m_eSnap.EnableWindow(true);
	}
	break;
	}
}

// Snap by value has been selected
void ObjectDialogue::OnBnClickedSnapValue()
{
	m_snapValue = IsDlgButtonChecked(IDC_CHECK5);

	// Switch between checked/unchecked
	switch (m_snapValue)
	{
	case true:
	{
		// Disable snap to terrain
		GetDlgItem(IDC_CHECK4)->EnableWindow(false);
	}
	break;
	case false:
	{
		// Enable snap to terrain
		GetDlgItem(IDC_CHECK4)->EnableWindow(true);
	}
	break;
	}
}

// Snap value has been changed
void ObjectDialogue::OnEnChangeSnapValue()
{

}

// Delete has been selected
void ObjectDialogue::OnBnClickedDelete()
{
	// If selection is valid
	if (m_selectedIDs.size() > 0)
	{
		// Remove objects from database storage
		SetupObjects(&ObjectManager::Remove(m_selectedIDs));
	}
}

// Duplicate has been selected
void ObjectDialogue::OnBnClickedDuplicate()
{
	// If selection is valid
	if (m_selectedIDs.size() != 0)
	{
		// Copy objects
		ObjectManager::Copy(m_selectedIDs);

		// Paste objects
		SetupObjects(&ObjectManager::Paste());	
	}
}

// Update remaining object details when one is changed //////////////////////////////////////////////

// Setup IDs of currently available objects
void ObjectDialogue::SetupObjects(std::vector<SceneObject>* sceneGraph)
{
	m_internal = true;
	
	// Local storage of objects
	m_objects = new std::vector<SceneObject>();

	// Loop through scene graph
	for (int i = 0; i < sceneGraph->size(); ++i)
	{
		// If object isn't a light
		if (sceneGraph->at(i).m_type != OBJECT_TYPE::LIGHT)
		{
			// Add to storage
			m_objects->push_back(sceneGraph->at(i));
		}
	}
	
	m_boxID.ResetContent();

	// Loop through objects in scene graph
	for (int i = 0; i < m_objects->size(); ++i)
	{		
		// Add entries to ID combo box
		std::wstring idBoxEntry = std::to_wstring(m_objects->at(i).ID);
		m_boxID.AddString(idBoxEntry.c_str());		
	}

	// Setup total objects in scene
	std::wstring totalObjects = std::to_wstring(m_objects->size());
	SetDlgItemText(IDC_STATIC3, totalObjects.c_str());

	m_internal = false;
}

void ObjectDialogue::Uncheck()
{
	// Switch between function
	switch (m_function)
	{
	case OBJECT_FUNCTION::SELECT:
	{
		m_transforming = false;

		// Uncheck scale button
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck rotate button
		CheckDlgButton(IDC_CHECK27, false);
		
		// Uncheck translate button
		CheckDlgButton(IDC_CHECK28, false);
	}
	break;
	case OBJECT_FUNCTION::SCALE:
	{
		m_transforming = true;

		// Uncheck rotate button
		CheckDlgButton(IDC_CHECK27, false);

		// Uncheck translate button
		CheckDlgButton(IDC_CHECK28, false);
	}
	break;
	case OBJECT_FUNCTION::ROTATE:
	{
		m_transforming = true;

		// Uncheck scale button
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck translate button
		CheckDlgButton(IDC_CHECK28, false);
	}
	break;
	case OBJECT_FUNCTION::TRANSLATE:
	{
		m_transforming = true;

		// Uncheck scale button
		CheckDlgButton(IDC_CHECK26, false);

		// Uncheck rotate button
		CheckDlgButton(IDC_CHECK27, false);
	}
	break;
	}
}

// Update current object with dialogue values
void ObjectDialogue::UpdateDialogue(int ID)
{
	// If ID is valid
	if (ID != -1) { m_boxID.SetCurSel(ID); }
	else if (m_selectedIDs.size() == 1) { m_boxID.SetCurSel(0); }
	UpdateType();
	UpdateScale();
	UpdateRotation();
	UpdatePosition();
}

void ObjectDialogue::UpdateType()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{
		// Loop through objects
		for (int i = 0; i < m_objects->size(); ++i)
		{
			// If object ID matches selection
			if (m_objects->at(i).ID == m_selectedIDs.at(0))
			{
				// Set combo box
				m_boxType.SetCurSel((int)m_objects->at(i).m_type);
				break;
			}
		}
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_boxType.SetCurSel((int)OBJECT_TYPE::LIGHT);
		m_internal = false;
	}
}

void ObjectDialogue::UpdateScale()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_objects->size(); ++i)
		{
			// If light ID matches selection
			if (m_objects->at(i).ID == m_selectedIDs.at(0))
			{
				// Store current scale
				float x = m_objects->at(i).scaX;
				float y = m_objects->at(i).scaY;
				float z = m_objects->at(i).scaZ;

				// Update X scale box
				CString sX; sX.Format(L"%g", x);
				m_eScaX.SetWindowTextW(sX);

				// Update Y scale box
				CString sY; sY.Format(L"%g", y);
				m_eScaY.SetWindowTextW(sY);

				// Update Z scale box
				CString sZ; sZ.Format(L"%g", z);
				m_eScaZ.SetWindowTextW(sZ);
			}
		}
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_eScaX.SetWindowTextW(empty);
		m_eScaY.SetWindowTextW(empty);
		m_eScaZ.SetWindowTextW(empty);
		m_internal = false;
	}
}

void ObjectDialogue::UpdateRotation()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{
		// Loop through lights
		for (int i = 0; i < m_objects->size(); ++i)
		{
			// If light ID matches selection
			if (m_objects->at(i).ID == m_selectedIDs.at(0))
			{
				// Store current rotation
				float x = m_objects->at(i).rotX;
				float y = m_objects->at(i).rotY;
				float z = m_objects->at(i).rotZ;

				// Update X rotation box
				CString sX; sX.Format(L"%g", x);
				m_eRotX.SetWindowTextW(sX);

				// Update Y rotation box
				CString sY; sY.Format(L"%g", y);
				m_eRotY.SetWindowTextW(sY);

				// Update Z rotation box
				CString sZ; sZ.Format(L"%g", z);
				m_eRotZ.SetWindowTextW(sZ);
			}
		}
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_eRotX.SetWindowTextW(empty);
		m_eRotY.SetWindowTextW(empty);
		m_eRotZ.SetWindowTextW(empty);
		m_internal = false;
	}
}

void ObjectDialogue::UpdatePosition()
{
	// If only one object is selected
	if (m_selectedIDs.size() == 1)
	{
		// Loop through objects
		for (int i = 0; i < m_objects->size(); ++i)
		{
			// If light ID matches selection
			if (m_objects->at(i).ID == m_selectedIDs.at(0))
			{
				// Store current position
				float x = m_objects->at(i).posX;
				float y = m_objects->at(i).posY;
				float z = m_objects->at(i).posZ;

				// Update X position box
				CString sX; sX.Format(L"%g", x);
				m_ePosX.SetWindowTextW(sX);

				// Update Y position box
				CString sY; sY.Format(L"%g", y);
				m_ePosY.SetWindowTextW(sY);

				// Update Z position box
				CString sZ; sZ.Format(L"%g", z);
				m_ePosZ.SetWindowTextW(sZ);
			}
		}
	}

	// Else, multiple objects are selected
	else
	{
		// Setup & apply empty string
		CString empty = "";
		m_internal = true;
		m_ePosX.SetWindowTextW(empty);
		m_ePosY.SetWindowTextW(empty);
		m_ePosZ.SetWindowTextW(empty);
		m_internal = false;
	}
}

void ObjectDialogue::UpdateSelectedConstraint()
{
	// If x, y and z constraints are selected
	if (m_x && m_y && m_z) { m_constraint = CONSTRAINT::NA; }

	// Else, if x and y constraints are selected
	else if (m_x && m_y) { m_constraint = CONSTRAINT::XY; }

	// Else, if x and z constraints are selected
	else if (m_x && m_z) { m_constraint = CONSTRAINT::XZ; }

	// Else, if y and z constraints are selected
	else if (m_y && m_z) { m_constraint = CONSTRAINT::YZ; }

	// Else, if only x constraint is selected
	else if (m_x) { m_constraint = CONSTRAINT::X; }

	// Else, if only y constraint is selected
	else if (m_y) { m_constraint = CONSTRAINT::Y; }

	// Else, if only z constraint is selected
	else if (m_z) { m_constraint = CONSTRAINT::Z; }

	// Else, if no constraints are selected
	else { m_constraint = CONSTRAINT::NA; }

	// Update constraint box
	m_boxConst.SetCurSel((int)m_constraint);
}

void ObjectDialogue::UpdateObject(SceneObject object)
{
	// Replace object through database
	ObjectManager::ReplaceObject(object);
}

void ObjectDialogue::Reset()
{
	m_active = m_focus = m_transforming =
		m_x = m_y = m_z = m_snapTerrain = m_snapValue = false;

	m_function = OBJECT_FUNCTION::SELECT;
	m_constraint = CONSTRAINT::NA;


	CheckDlgButton(IDC_CHECK26, false);
	CheckDlgButton(IDC_CHECK27, false);
	CheckDlgButton(IDC_CHECK28, false);
	CheckDlgButton(IDC_CHECK23, false);
	CheckDlgButton(IDC_CHECK24, false);
	CheckDlgButton(IDC_CHECK25, false);
}

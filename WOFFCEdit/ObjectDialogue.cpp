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

}

void ObjectDialogue::SetObjectData(std::vector<SceneObject> sceneGraph)
{
	// Local storage of scene graph
	m_sceneGraph = sceneGraph;

	// Loop through objects and add entries to the ID combo box
	for (int i = 0; i < m_sceneGraph.size(); ++i)
	{
		// Setup object IDs
		std::wstring idBoxEntry = std::to_wstring(m_sceneGraph.at(i).ID);
		m_boxID.AddString(idBoxEntry.c_str());
	}

	// Setup total objects in scene
	int total = m_sceneGraph.size();
	std::wstring totalObjects = std::to_wstring(total);
	SetDlgItemText(IDC_STATIC3, totalObjects.c_str());

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
		typeBoxEntry = L"Water";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Light";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Cube";			m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Cylinder";		m_boxType.AddString(typeBoxEntry.c_str());
		typeBoxEntry = L"Cone";			m_boxType.AddString(typeBoxEntry.c_str());
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

	// Display first object
	if (total != 0) { Update(0); }

	// Set constraint to display N/A
	m_boxConst.SetCurSel(0);

	SetupCheckBoxes();
}

// Update current object with dialogue values
void ObjectDialogue::Update(int ID)
{
	m_boxID.SetCurSel(ID);
	UpdateType(ID);
	UpdateScale(ID);
	UpdateRotation(ID);
	UpdatePosition(ID);
}

void ObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_boxID);
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
	ON_BN_CLICKED(IDOK, &ObjectDialogue::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &ObjectDialogue::OnCbnSelchangeID)
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
END_MESSAGE_MAP()


// ObjectDialogue message handlers

// Kill the dialogue
void ObjectDialogue::End()
{
	m_active = false;
	DestroyWindow();
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
	m_objectID = m_boxID.GetCurSel();

	// Update object type box
	UpdateType(m_objectID);

	// Update object scale boxes
	UpdateScale(m_objectID);

	// Update object rotation boxes
	UpdateRotation(m_objectID);
	
	// Update object position boxes
	UpdatePosition(m_objectID);

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Type has been changed
void ObjectDialogue::OnCbnSelchangeType()
{
	// Store type selection
	int type = m_boxType.GetCurSel();

	// Replace object with new type
	ObjectManager::Replace(m_objectID, m_sceneGraph, (OBJECT_TYPE)type);
}

// Object should be focussed/unfocussed
void ObjectDialogue::OnBnClickedFocus()
{
	m_focus = IsDlgButtonChecked(IDC_CHECK3);

	// Tell MFC/ToolMain to update
	m_update = true;
}

// X scale has been changed
void ObjectDialogue::OnEnChangeScaX()
{
	// Store new X scale
	CString string = _T("");
	m_eScaX.GetWindowTextW(string);

	// Convert to float
	float scaX;
	if (!string.IsEmpty()) { scaX = _ttof(string); }
	else { scaX = m_sceneGraph.at(m_objectID).scaX; }

	// Update X scale of object
	m_sceneGraph.at(m_objectID).scaX = scaX;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;

	// Add ID to vector
	///std::vector<int> IDs;
	///IDs.push_back(m_objectID);

	// Update object scale
	///ObjectManager::Transform(OBJECT_FUNCTION::SCALE, m_constraint, IDs, *m_sceneGraph);
}

// Y scale has been changed
void ObjectDialogue::OnEnChangeScaY()
{
	// Store new Y scale
	CString string = _T("");
	m_eScaY.GetWindowTextW(string);

	// Convert to float
	float scaY;
	if (!string.IsEmpty()) { scaY = _ttof(string); }
	else { scaY = m_sceneGraph.at(m_objectID).scaY; }

	// Update Y scale of object
	m_sceneGraph.at(m_objectID).scaY = scaY;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Z scale has been changed
void ObjectDialogue::OnEnChangeScaZ()
{
	// Store new Z scale
	CString string = _T("");
	m_eScaZ.GetWindowTextW(string);

	// Convert to float
	float scaZ;
	if (!string.IsEmpty()) { scaZ = _ttof(string); }
	else { scaZ = m_sceneGraph.at(m_objectID).scaZ; }

	// Update Z scale of object
	m_sceneGraph.at(m_objectID).scaZ = scaZ;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// X rotation has been changed
void ObjectDialogue::OnEnChangeRotX()
{
	// Store new X rotation
	CString string = _T("");
	m_eRotX.GetWindowTextW(string);

	// Convert to float
	float rotX;
	if (!string.IsEmpty()) { rotX = _ttof(string); }
	else { rotX = m_sceneGraph.at(m_objectID).rotX; }

	// Update X rotation of object
	m_sceneGraph.at(m_objectID).rotX = rotX;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Y rotation has been changed
void ObjectDialogue::OnEnChangeRotY()
{
	// Store new Y rotation
	CString string = _T("");
	m_eRotY.GetWindowTextW(string);

	// Convert to float
	float rotY;
	if (!string.IsEmpty()) { rotY = _ttof(string); }
	else { rotY = m_sceneGraph.at(m_objectID).rotY; }

	// Update Y rotation of object
	m_sceneGraph.at(m_objectID).rotY = rotY;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Z rotation has been changed
void ObjectDialogue::OnEnChangeRotZ()
{
	// Store new Z rotation
	CString string = _T("");
	m_eRotZ.GetWindowTextW(string);

	// Convert to float
	float rotZ;
	if (!string.IsEmpty()) { rotZ = _ttof(string); }
	else { rotZ = m_sceneGraph.at(m_objectID).rotZ; }

	// Update Z rotation of object
	m_sceneGraph.at(m_objectID).rotZ = rotZ;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// X position has been changed
void ObjectDialogue::OnEnChangePosX()
{
	// Store new X position
	CString string = _T("");
	m_ePosX.GetWindowTextW(string);

	// Convert to float
	float posX;
	if (!string.IsEmpty()) { posX = _ttof(string); }
	else { posX = m_sceneGraph.at(m_objectID).posX; }

	// Update X position of object
	m_sceneGraph.at(m_objectID).posX = posX;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Y position has been changed
void ObjectDialogue::OnEnChangePosY()
{
	// Store new Y position
	CString string = _T("");
	m_ePosY.GetWindowTextW(string);

	// Convert to float
	float posY;
	if (!string.IsEmpty()) { posY = _ttof(string); }
	else { posY = m_sceneGraph.at(m_objectID).posY; }

	// Update Y position of object
	m_sceneGraph.at(m_objectID).posY = posY;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
}

// Z position has been changed
void ObjectDialogue::OnEnChangePosZ()
{
	// Store new Z position
	CString string = _T("");
	m_ePosX.GetWindowTextW(string);

	// Convert to float
	float posZ;
	if (!string.IsEmpty()) { posZ = _ttof(string); }
	else { posZ = m_sceneGraph.at(m_objectID).posZ; }

	// Update Z position of object
	m_sceneGraph.at(m_objectID).posZ = posZ;

	// Tell MFC/ToolMain to update scene graph
	m_update = true;
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
	// Add object ID to vector
	std::vector<int> IDs;
	IDs.push_back(m_objectID);

	// Remove object from storage
	ObjectManager::Remove(IDs, m_sceneGraph, m_objectID);
}

// Duplicate has been selected
void ObjectDialogue::OnBnClickedDuplicate()
{
	// Add object ID to vector
	std::vector<int> IDs;
	IDs.push_back(m_objectID);

	// Copy current object
	ObjectManager::Copy(IDs, m_sceneGraph);

	// Paste current object
	ObjectManager::Paste(m_sceneGraph);
}

// Update remaining object details when one is changed //////////////////////////////////////////////

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

void ObjectDialogue::UpdateType(int ID)
{
	// Set object type
	m_boxType.SetCurSel((int)m_sceneGraph.at(ID).m_type);
}

void ObjectDialogue::UpdateScale(int ID)
{
	// Store current scale
	float x = m_sceneGraph.at(ID).scaX;
	float y = m_sceneGraph.at(ID).scaY;
	float z = m_sceneGraph.at(ID).scaZ;

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

void ObjectDialogue::UpdateRotation(int ID)
{
	// Store current rotation
	float x = m_sceneGraph.at(ID).rotX;
	float y = m_sceneGraph.at(ID).rotY;
	float z = m_sceneGraph.at(ID).rotZ;

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

void ObjectDialogue::UpdatePosition(int ID)
{
	// Store current position
	float x = m_sceneGraph.at(ID).posX;
	float y = m_sceneGraph.at(ID).posY;
	float z = m_sceneGraph.at(ID).posZ;

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

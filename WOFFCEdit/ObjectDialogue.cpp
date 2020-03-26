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

// Destroy window & set unactive
void ObjectDialogue::End()
{
	m_function = OBJECT_FUNCTION::NA;
	m_constraint = OBJECT_CONSTRAINT::NA;
	m_active = false;
	DestroyWindow();
}

void ObjectDialogue::OnBnClickedOk()
{
	// Parent
	CDialogEx::OnOK();

	// Save object manipulation...
}

// Set object manipulation to selecting objects
void ObjectDialogue::OnBnClickedSelect()
{
	m_function = OBJECT_FUNCTION::SELECT;

	
}

void ObjectDialogue::UpdateFunctionText(OBJECT_FUNCTION function)
{
	//wchar_t text[20];

	//// Switch between function type
	//switch (function)
	//{
	//case OBJECT_FUNCTION::NA:
	//{
	//	text += L"NA";
	//}
	//break;
	//case OBJECT_FUNCTION::SELECT:
	//{

	//}
	//break;
	//case OBJECT_FUNCTION::SCALE:
	//{

	//}
	//break;
	//case OBJECT_FUNCTION::TRANSLATE:
	//{

	//}
	//break;
	//case OBJECT_FUNCTION::ROTATE:
	//{

	//}
	//break;
	//}

	//swprintf_s(text, L"%d", )
}

void ObjectDialogue::UpdateConstraintText(OBJECT_CONSTRAINT constraint)
{

}

// Set object manipulation to scaling
void ObjectDialogue::OnBnClickedScale()
{
	m_function = OBJECT_FUNCTION::SCALE;
}

// Set object manipulation to translating
void ObjectDialogue::OnBnClickedTranslate()
{
	m_function = OBJECT_FUNCTION::TRANSLATE;
}

// Set object manipulation to rotating
void ObjectDialogue::OnBnClickedRotate()
{
	m_function = OBJECT_FUNCTION::ROTATE;
}

// Constrain object manipulation to X axis
void ObjectDialogue::OnBnClickedX()
{
	m_constraint = OBJECT_CONSTRAINT::X;
}

// Constrain object manipulation to Y axis
void ObjectDialogue::OnBnClickedY()
{
	m_constraint = OBJECT_CONSTRAINT::Y;
}

// Constrain object manipulation to Z axis
void ObjectDialogue::OnBnClickedZ()
{
	m_constraint = OBJECT_CONSTRAINT::Z;
}

// Constrain object manipulation to all axes
void ObjectDialogue::OnBnClickedAll()
{
	m_constraint = OBJECT_CONSTRAINT::ALL;
}


BEGIN_MESSAGE_MAP(ObjectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ObjectDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON8, &ObjectDialogue::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_BUTTON1, &ObjectDialogue::OnBnClickedScale)
	ON_BN_CLICKED(IDC_BUTTON2, &ObjectDialogue::OnBnClickedTranslate)
	ON_BN_CLICKED(IDC_BUTTON3, &ObjectDialogue::OnBnClickedRotate)
	ON_BN_CLICKED(IDC_BUTTON4, &ObjectDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_BUTTON5, &ObjectDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_BUTTON6, &ObjectDialogue::OnBnClickedZ)
	ON_BN_CLICKED(IDC_BUTTON7, &ObjectDialogue::OnBnClickedAll)
END_MESSAGE_MAP()


// ObjectDialogue message handlers

//INT_PTR SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	return INT_PTR();
//}

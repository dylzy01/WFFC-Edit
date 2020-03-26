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

// Destroy window & set unactive
void SculptDialogue::End()
{
	m_function = SCULPT_FUNCTION::NA;
	m_constraint = SCULPT_CONSTRAINT::NA;
	m_active = false;
	DestroyWindow();
}

void SculptDialogue::Select()
{
}

BEGIN_MESSAGE_MAP(SculptDialogue, CDialogEx)
	ON_COMMAND(IDOK, &SculptDialogue::End)
	ON_BN_CLICKED(IDOK, &SculptDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON8, &SculptDialogue::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_BUTTON1, &SculptDialogue::OnBnClickedIncrease)
	ON_BN_CLICKED(IDC_BUTTON2, &SculptDialogue::OnBnClickedFlatten)
	ON_BN_CLICKED(IDC_BUTTON3, &SculptDialogue::OnBnClickedDecrease)
	ON_BN_CLICKED(IDC_BUTTON4, &SculptDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_BUTTON5, &SculptDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_BUTTON6, &SculptDialogue::OnBnClickedZ)
	ON_BN_CLICKED(IDC_BUTTON7, &SculptDialogue::OnBnClickedAll)
END_MESSAGE_MAP()


// SculptDialogue message handlers

void SculptDialogue::OnBnClickedOk()
{
	// Parent
	CDialogEx::OnOK();

	// Save object manipulation...
}

// Set sculpting to selecting terrain
void SculptDialogue::OnBnClickedSelect()
{
}

// Set sculpting to increase
void SculptDialogue::OnBnClickedIncrease()
{
	m_function = SCULPT_FUNCTION::INCREASE;
}

// Set sculpting to flatten
void SculptDialogue::OnBnClickedFlatten()
{
	m_function = SCULPT_FUNCTION::FLATTEN;
}

// Set sculpting to decrease
void SculptDialogue::OnBnClickedDecrease()
{
	m_function = SCULPT_FUNCTION::DECREASE;
}

// Constrain sculpting to X axis
void SculptDialogue::OnBnClickedX()
{
	m_constraint = SCULPT_CONSTRAINT::X;
}

// Constrain sculpting to Y axis
void SculptDialogue::OnBnClickedY()
{
	m_constraint = SCULPT_CONSTRAINT::Y;
}

// Constrain sculpting to Z axis
void SculptDialogue::OnBnClickedZ()
{
	m_constraint = SCULPT_CONSTRAINT::Z;
}

// Constrain sculpting to all axes
void SculptDialogue::OnBnClickedAll()
{
	m_constraint = SCULPT_CONSTRAINT::ALL;
}

// FocusDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "FocusDialogue.h"
#include "afxdialogex.h"


// FocusDialogue dialog

IMPLEMENT_DYNAMIC(FocusDialogue, CDialogEx)

FocusDialogue::FocusDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_selectedID = -1;
}

void FocusDialogue::SetSelectData(std::vector<int>* selection)
{	
	m_selectedIDs = *selection;
	
	// Loop through selection
	for (int i = 0; i < m_selectedIDs.size(); ++i)
	{			
		// Add entries to ID list box
		std::wstring idBoxEntry = std::to_wstring(m_selectedIDs.at(i));
		m_boxID.AddString(idBoxEntry.c_str());
	}	
}

void FocusDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_boxID);
}


BEGIN_MESSAGE_MAP(FocusDialogue, CDialogEx)
	ON_COMMAND(IDOK, &FocusDialogue::End)							
	ON_BN_CLICKED(IDOK, &FocusDialogue::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &FocusDialogue::OnCbnSelchangeID)	
END_MESSAGE_MAP()


// FocusDialogue message handlers

// Kill the dialogue
void FocusDialogue::End()
{
	m_active = false;
	DestroyWindow();
}

// User is finished with dialogue
void FocusDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
	m_update = true;	
}

// ID has been selected
void FocusDialogue::OnCbnSelchangeID()
{
	// Apply selected ID
	m_selectedID = m_selectedIDs[m_boxID.GetCurSel()];
}
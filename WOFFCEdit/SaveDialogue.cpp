// SaveDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "SaveDialogue.h"
#include "afxdialogex.h"


// SaveDialogue dialog

IMPLEMENT_DYNAMIC(SaveDialogue, CDialogEx)

SaveDialogue::SaveDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	
}

void SaveDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_boxName);
}


BEGIN_MESSAGE_MAP(SaveDialogue, CDialogEx)
	ON_COMMAND(IDOK, &SaveDialogue::End)
	ON_BN_CLICKED(IDOK, &SaveDialogue::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &SaveDialogue::OnEnChangeInput)
	ON_BN_CLICKED(IDC_BUTTON1, &SaveDialogue::OnBnClickedSave)
END_MESSAGE_MAP()


// SaveDialogue message handlers

// Kill the dialogue
void SaveDialogue::End()
{
	m_active = false;
	DestroyWindow();
}

// User is finished with dialogue
void SaveDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// Text has been inputted
void SaveDialogue::OnEnChangeInput()
{
	// Store input
	CString string = _T("");
	m_boxName.GetWindowTextW(string);

	// Convert to std::string
	m_input = std::string(CW2A(string.GetString()));
}

void SaveDialogue::OnBnClickedSave()
{
	// Set name pointer to input string
	m_name = m_input;

	// Tell MFC/ToolMain to update saving
	m_update = true;
}

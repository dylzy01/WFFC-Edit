// LoadDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "LoadDialogue.h"
#include "afxdialogex.h"


// LoadDialogue dialog

IMPLEMENT_DYNAMIC(LoadDialogue, CDialogEx)

LoadDialogue::LoadDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

// Setup chunk name data
void LoadDialogue::SetupData(int count)
{		
	// Loop through names
	for (int i = 0; i < count; ++i)
	{
		// Add paths to container
		m_paths.push_back("database/data/hm/heightmap_" + std::to_string(count) + ".raw");
		
		// Add entries to the name list box
		std::wstring nameBoxEntry = L"Height Map " + std::to_wstring(i + 1);
		m_boxName.AddString(nameBoxEntry.c_str());
	}
}

void LoadDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_boxName);
}


BEGIN_MESSAGE_MAP(LoadDialogue, CDialogEx)
	ON_COMMAND(IDOK, &LoadDialogue::End)
	ON_BN_CLICKED(IDOK, &LoadDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &LoadDialogue::OnBnClickedLoad)
END_MESSAGE_MAP()


// LoadDialogue message handlers

// Kill the dialogue
void LoadDialogue::End()
{
	m_active = false;
	DestroyWindow();
}

// User is finished with dialogue
void LoadDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// User has selected a chunk to load
void LoadDialogue::OnBnClickedLoad()
{
	// Tell MFC/ToolMain to update saving
	m_update = true;
}
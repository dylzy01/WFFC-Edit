#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include <fstream>

// LoadDialogue dialog

class LoadDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(LoadDialogue)

public:
	LoadDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~LoadDialogue() {}

	// Setup height map name data
	void SetupData(int count);

	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }
	std::string GetPath() { return m_paths[m_boxName.GetCurSel()]; }

	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();									// Kill the dialogue

	// Controllers
	bool m_active;
	bool m_update;
	std::vector<std::string> m_paths;

	DECLARE_MESSAGE_MAP()
public:
	// Control variable for more efficient use of the box
	CListBox m_boxName;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLoad();
};
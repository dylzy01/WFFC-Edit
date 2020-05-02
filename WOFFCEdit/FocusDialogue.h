#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "SceneObject.h"

// FocusDialogue dialog

class FocusDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(FocusDialogue)

public:
	FocusDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FocusDialogue() {}

	// Passing in pointers the class will operate on
	void SetSelectData(std::vector<int> * selection);

	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }
	int GetSelectedIndex() { return m_selectedID; }

	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();									// Kill the dialogue
	
	// Controllers
	bool m_active;
	bool m_update;
	std::vector<int> m_selectedIDs;
	int m_selectedID;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient uses of the box
	CListBox m_boxID;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeID();
};

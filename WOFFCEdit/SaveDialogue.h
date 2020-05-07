#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>

// SaveDialogue dialog

class SaveDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SaveDialogue)

public:
	SaveDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SaveDialogue() {}

	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }
	std::string GetName() { return m_name; }

	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();									// Kill the dialogue

	// Controllers
	bool m_active;
	bool m_update;
	std::string m_name;
	std::string m_input;

	DECLARE_MESSAGE_MAP()
public:
	// Control variable for more efficient use of the box
	CEdit m_boxName;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeInput();
	afx_msg void OnBnClickedSave();
};

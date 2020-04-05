#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "Tools.h"

// ResidentialObjectDialogue dialog

class ResidentialObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ResidentialObjectDialogue)

public:
	ResidentialObjectDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ResidentialObjectDialogue();

	// Getters
	bool GetActive() { return m_active; }
	RESIDENTIAL_SPAWN GetSpawn() { return m_residence; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	// Enable/disable other buttons
	void UpdateButtons(bool enable);

	// Controllers
	bool m_active = false;
	RESIDENTIAL_SPAWN m_residence = RESIDENTIAL_SPAWN::NA;

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void End();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHouseOne();
	afx_msg void OnBnClickedHouseTwo();
	afx_msg void OnBnClickedCave();
};

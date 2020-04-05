#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "Tools.h"

// NatureObjectDialogue dialog

class NatureObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(NatureObjectDialogue)

public:
	NatureObjectDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~NatureObjectDialogue();

	// Getters
	bool GetActive() { return m_active; }
	NATURE_SPAWN GetSpawn() { return m_nature; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	// Enable/disable other buttons
	void UpdateButtons(bool enable);

	// Controllers
	bool m_active = false;
	NATURE_SPAWN m_nature = NATURE_SPAWN::NA;

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void End();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGrass();
	afx_msg void OnBnClickedBush();
	afx_msg void OnBnClickedTreeOne();
	afx_msg void OnBnClickedTreeTwo();	
	afx_msg void OnBnClickedWater();
};

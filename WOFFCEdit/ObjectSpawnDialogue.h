#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "Tools.h"

#include "ResidentialObjectDialogue.h"
#include "NatureObjectDialogue.h"

// ObjectSpawnDialogue dialog

class ObjectSpawnDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectSpawnDialogue)

public:
	ObjectSpawnDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectSpawnDialogue();

	// Getters
	bool GetActive() { return m_active; }
	OBJECT_SPAWN GetSpawn() { return m_spawn; }
	ResidentialObjectDialogue* GetResidentialObjectDialogue() { return &m_residentialObjectDialogue; }
	NatureObjectDialogue* GetNatureObjectDialogue() { return &m_natureObjectDialogue; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	// Enable/disable other buttons
	void UpdateButtons(bool enable);

	// Controllers
	bool m_active = false;
	OBJECT_SPAWN m_spawn = OBJECT_SPAWN::NA;

	ResidentialObjectDialogue m_residentialObjectDialogue;
	NatureObjectDialogue m_natureObjectDialogue;

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void End();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGrass();
	afx_msg void OnBnClickedTree();
	afx_msg void OnBnClickedWater();
	afx_msg void OnBnClickedHouse();
	afx_msg void OnBnClickedCave();

	afx_msg void OnBnClickedResidential();
	afx_msg void OnBnClickedNature();
	afx_msg void OnBnClickedProps();
};

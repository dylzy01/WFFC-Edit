#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "Tools.h"

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

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGrass();
	afx_msg void OnBnClickedTree();
	afx_msg void OnBnClickedWater();
};

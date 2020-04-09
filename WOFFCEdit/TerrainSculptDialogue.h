#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "Tools.h"

// TerrainSculptDialogue dialog

class TerrainSculptDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TerrainSculptDialogue)

public:
	TerrainSculptDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TerrainSculptDialogue();

	// Getters
	bool GetActive() { return m_active; }
	LANDSCAPE_FUNCTION GetSculpt() { return m_sculpt; }
	bool GetConstraintX() { return m_x; }
	bool GetConstraintY() { return m_y; }
	bool GetConstraintZ() { return m_z; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	// Enable/disable other buttons
	void UpdateButtons(bool enable);

	// Reset all constraints
	void ResetConstraints() { m_x = m_y = m_z = false; }

	// Controllers
	bool m_active = false;
	LANDSCAPE_FUNCTION m_sculpt = LANDSCAPE_FUNCTION::NA;
	bool m_x = false, m_y = false, m_z = false;

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void End();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedIncrease();
	afx_msg void OnBnClickedFlatten();
	afx_msg void OnBnClickedDecrease();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();
};

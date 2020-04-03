#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "Tools.h"

// LandscapeDialogue dialog

class LandscapeDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(LandscapeDialogue)

public:
	LandscapeDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~LandscapeDialogue();

	// Getters
	EDITOR GetEditor() { return m_editor; }
	LANDSCAPE_SCULPT GetSculpt() { return m_sculpt; }
	bool GetPaintGrass() { return m_grass; }
	bool GetPaintDirt() { return m_dirt; }
	bool GetPaintSand() { return m_sand; }
	bool GetConstraintX() { return m_x; }
	bool GetConstraintY() { return m_y; }
	bool GetConstraintZ() { return m_z; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Kill dialogue
	afx_msg void End();

	// Reset constraints after each function change
	void ResetConstraints() { m_x = m_y = m_z = false; }

	// Enable/disable other sculpts
	void UpdateSculpt(LANDSCAPE_SCULPT function, bool enable);
	
	// Enable/disable other paints
	void UpdatePaint(LANDSCAPE_PAINT paint, bool enable);

protected:
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<TERRAIN>* m_terrain;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGrass();
	afx_msg void OnBnClickedDirt();
	afx_msg void OnBnClickedSand();
	afx_msg void OnBnClickedIncrease();	
	afx_msg void OnBnClickedDecrease();
	afx_msg void OnBnClickedFlatten();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();

	// Controllers
	EDITOR m_editor = EDITOR::NA;
	LANDSCAPE_SCULPT m_sculpt = LANDSCAPE_SCULPT::NA;
	bool m_active = false;
	bool m_grass = false, m_dirt = false, m_sand = false;
	bool m_x = false, m_y = false, m_z = false;

	DECLARE_MESSAGE_MAP()
};

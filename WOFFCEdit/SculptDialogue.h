#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "DisplayChunk.h"
#include "ToolMain.h"

// SculptDialogue dialog

class SculptDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SculptDialogue)

public:
	SculptDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SculptDialogue() {}

	// Pass in a pointer for the class to operate on
	void SetToolData(ToolMain* toolSystem);

	// Update ToolMain from this dialogue
	void UpdateTool();

	// Getters
	bool GetActive() { return m_active; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG11 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue

	// Local storage
	ToolMain* m_toolSystem;

	// Controllers
	bool m_active;/// , m_plateau;
	bool m_x, m_y, m_z;
	float m_scale;
	TERRAIN_SCULPT m_sculpt;
	CONSTRAINT m_constraint;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient use of boxes
	CComboBox m_boxSculpt, m_boxConst;
	CEdit m_eScale;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnSelchangeSculpt();
	afx_msg void OnBnSelchangeConstraint();
	afx_msg void OnBnClickedIncrease();
	afx_msg void OnBnClickedFlatten();
	afx_msg void OnBnClickedDecrease();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();
	afx_msg void OnEnChangeScale();

private:
	// Uncheck other buttons
	void Uncheck();

	// Update sculpt details
	void Update();
	void UpdateSelectedSculpt(int sculpt);
	void UpdateSelectedConstraint();

	// Reset all variables
	void Reset();
};

#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "DisplayChunk.h"
#include "ToolMain.h"

// PaintDialogue dialog

class PaintDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(PaintDialogue)

public:
	PaintDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PaintDialogue() {}
	
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
	enum { IDD = IDD_DIALOG10 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue

	// Local storage
	ToolMain* m_toolSystem;

	// Controllers
	bool m_active;
	bool m_surround;
	bool m_blend;
	TERRAIN_PAINT m_paint;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient use of the box
	CComboBox m_boxSelect;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnSelchangePaint();
	afx_msg void OnBnClickedGrass();
	afx_msg void OnBnClickedDirt();
	afx_msg void OnBnClickedSand();
	afx_msg void OnBnClickedStone();
	afx_msg void OnBnClickedSnow();
	afx_msg void OnBnClickedNA();
	afx_msg void OnBnClickedBlend();

private:
	// Uncheck other buttons
	void Uncheck();

	// Update dialogue details
	void Update(int paint);

	// Reset all variables
	void Reset();
};

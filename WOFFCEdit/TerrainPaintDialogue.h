#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "Tools.h"

// TerrainPaintDialogue dialog

class TerrainPaintDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TerrainPaintDialogue)

public:
	TerrainPaintDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TerrainPaintDialogue();

	// Getters
	bool GetActive() { return m_active; }
	LANDSCAPE_PAINT GetPaint() { return m_paint; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	// Enable/disable other buttons
	void UpdateButtons(bool enable);

	// Controllers
	bool m_active = false;
	LANDSCAPE_PAINT m_paint = LANDSCAPE_PAINT::NA;

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void End();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGrass();
	afx_msg void OnBnClickedDirt();
	afx_msg void OnBnClickedSand();
	afx_msg void OnBnClickedStone();
	afx_msg void OnBnClickedSnow();
};

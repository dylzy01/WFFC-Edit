#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "DisplayChunk.h"

// PaintDialogue dialog

class PaintDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(PaintDialogue)

public:
	PaintDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PaintDialogue() {}

	// Pass in data pointers the class will operate on
	void SetChunkData(DisplayChunk* displayChunk);

	// Getters
	bool GetActive() { return m_active; }
	bool GetSurround() { return m_surround; }
	bool GetBlend() { return m_blend; }
	TERRAIN_PAINT GetPaint() { return m_paint; }

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
	DisplayChunk* m_displayChunk;

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

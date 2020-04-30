#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "DisplayChunk.h"

// TerrainDialogue dialog

class TerrainDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TerrainDialogue)

public:
	TerrainDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TerrainDialogue() {}

	// Pass in data pointers the class will operate on
	void SetChunkData(DisplayChunk* displayChunk);

	// Update current terrain with dialogue values/vice versa
	void Update(int row, int column);
	
	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }

	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();									// Kill the dialogue

	// Local storage
	DisplayChunk* m_displayChunk;
														
	// Controllers
	bool m_active;
	bool m_update;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient use of the boxes
	CEdit m_eCoordX, m_eCoordY, m_eCoordZ;
	CComboBox m_boxRow, m_boxCol, m_boxTex;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnSelchangeIndex();
	afx_msg void OnBnSelchangeTexture();

private:
	// Update terrain details
	void UpdateCoordinates(int row, int column);
	void UpdateTexture(int row, int column);
};

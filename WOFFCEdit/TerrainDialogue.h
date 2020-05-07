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
	void UpdateTerrain(int row, int column);
	
	// Getters
	bool GetActive() { return m_active; }
	bool GetSculpting() { return m_sculpting; }
	TERRAIN GetTerrain() { return m_terrain; }
	TERRAIN_SCULPT GetSculpt() { return m_sculpt; }
	CONSTRAINT GetConstraint() { return m_constraint; }

	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetSculpting(bool sculpting) { m_sculpting = sculpting; }
	void SetTerrain(TERRAIN terrain) { m_terrain = terrain; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue

	// Local storage
	DisplayChunk* m_displayChunk;
	TERRAIN m_terrain;
														
	// Controllers
	bool m_active;
	bool m_sculpting = false;
	bool m_x, m_y, m_z;
	float m_scale;
	TERRAIN_SCULPT m_sculpt;
	CONSTRAINT m_constraint;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient use of the boxes
	CEdit m_eCoordX, m_eCoordY, m_eCoordZ;
	CComboBox m_boxRow, m_boxCol, m_boxTex;
	CComboBox m_boxSculpt, m_boxConst;
	CEdit m_eScale;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnSelchangeIndex();
	afx_msg void OnBnSelchangeTexture();
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
	// Uncheck other sculpt buttons
	void Uncheck();

	// Update terrain details
	void UpdateCoordinates(int row, int column);
	void UpdateTexture(int row, int column);

	// Update sculpt details
	void UpdateSculpt();
	void UpdateSelectedSculpt(int sculpt);
	void UpdateSelectedConstraint();

	// Reset all variables
	void Reset();
};

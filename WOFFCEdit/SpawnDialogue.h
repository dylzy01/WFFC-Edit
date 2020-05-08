#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "SceneObject.h"
#include "ObjectManager.h"
#include "ToolMain.h"

// SpawnDialogue dialog

class SpawnDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SpawnDialogue)

public:
	SpawnDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SpawnDialogue() {}

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
	enum { IDD = IDD_DIALOG13 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue
					
	// Local storage
	ToolMain* m_toolSystem;

	// Controllers
	bool m_active;
	OBJECT_TYPE m_spawner;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient uses of the boxes
	CComboBox m_boxType;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedHouseOne();
	afx_msg void OnBnClickedHouseTwo();
	afx_msg void OnBnClickedCave();
	afx_msg void OnBnClickedGrass();
	afx_msg void OnBnClickedTreeOne();
	afx_msg void OnBnClickedTreeTwo();
	afx_msg void OnBnClickedBridge();
	afx_msg void OnBnClickedFence();
	afx_msg void OnBnClickedBoat();
	afx_msg void OnBnClickedCube();
	afx_msg void OnBnClickedCylinder();
	afx_msg void OnBnClickedCone();
	afx_msg void OnBnClickedLight();
	afx_msg void OnBnClickedWater();

private:
	// Update spawner
	void Update(int type);
	void UpdateTypeBox();

	// Uncheck other spawn buttons
	void Uncheck();

	// Reset all variables
	void Reset();
};

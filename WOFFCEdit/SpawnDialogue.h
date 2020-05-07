#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "SceneObject.h"
#include "ObjectManager.h"

// SpawnDialogue dialog

class SpawnDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SpawnDialogue)

public:
	SpawnDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SpawnDialogue() {}

	// Pass in data pointers the class will operate on
	void Setup();

	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }
	OBJECT_TYPE GetSpawnType() { return m_spawner; }

	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG13 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue
														
	// Controllers
	bool m_active;
	bool m_update;
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
};

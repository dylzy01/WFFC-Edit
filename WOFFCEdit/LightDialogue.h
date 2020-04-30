#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "Tools.h"
#include "Light.h"

// LightDialogue dialog

class LightDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(LightDialogue)

public:
	LightDialogue(CWnd* pParent = NULL);
	virtual ~LightDialogue() {}

	// Pass in data pointers the class will operate on
	void SetLightData(std::pair<std::vector<Light*>, std::vector<int>>* lights);

	// Update current light with dialogue values
	void Update(int ID);
	
	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }
	std::pair<std::vector<Light*>, std::vector<int>> GetLights() { return m_lights; }

	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();									// Kill the dialogue

	// Local storage 
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int>* m_IDs;
	std::pair<std::vector<Light*>, std::vector<int>> m_lights;

	// Active controller
	bool m_active;
	bool m_update;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient uses of the boxes
	CComboBox m_boxID, m_boxType;
	CEdit m_ePosX, m_ePosY, m_ePosZ;
	CEdit m_eDirX, m_eDirY, m_eDirZ;
	CEdit m_eDifR, m_eDifG, m_eDifB;
	CEdit m_eAmbR, m_eAmbG, m_eAmbB;
	CEdit m_eConstA, m_eLinA, m_eQuadA;
	float m_fPosX, m_fPosY, m_fPosZ;
	float m_fDirX, m_fDirY, m_fDirZ;
	float m_fDifR, m_fDifG, m_fDifB;
	float m_fAmbR, m_fAmbG, m_fAmbB;
	float m_fConstA, m_fLinA, m_fQuadA;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeID();
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedEnable();
	afx_msg void OnEnChangePosX();
	afx_msg void OnEnChangePosY();
	afx_msg void OnEnChangePosZ();
	afx_msg void OnEnChangeDirX();
	afx_msg void OnEnChangeDirY();
	afx_msg void OnEnChangeDirZ();
	afx_msg void OnEnChangeDifR();
	afx_msg void OnEnChangeDifG();
	afx_msg void OnEnChangeDifB();
	afx_msg void OnEnChangeAmbR();
	afx_msg void OnEnChangeAmbG();
	afx_msg void OnEnChangeAmbB();
	afx_msg void OnEnChangeConstA();
	afx_msg void OnEnChangeLinA();
	afx_msg void OnEnChangeQuadA();

private:
	// Update light details
	void UpdateID(std::pair<Light*, int>* light);
	void UpdateType(int ID);
	void UpdateEnabled(int ID);
	void UpdatePosition(int ID);
	void UpdateDirection(int ID);
	void UpdateDiffuse(int ID);
	void UpdateAmbient(int ID);
	void UpdateConstA(int ID);
	void UpdateLinA(int ID);
	void UpdateQuadA(int ID);
};

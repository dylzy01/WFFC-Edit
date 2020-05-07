#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "Light.h"
#include "ObjectManager.h"
#include "FocusDialogue.h"
#include "SceneManager.h"

// LightDialogue dialog

class LightDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(LightDialogue)

public:
	LightDialogue(CWnd* pParent = NULL);
	virtual ~LightDialogue() {}

	// Pass in data pointers the class will operate on
	void SetLightData(std::vector<DisplayObject>* displayList);

	// Update current light with dialogue values
	void Update(int ID = -1);

	// Update current light position
	void UpdateLightPosition(DirectX::XMFLOAT3 position);	

	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }
	bool GetTranslating() { return m_translating; }
	bool GetLightSetup() { return m_lightSetup; }
	bool GetRequest() { return m_requestDisplayList; }
	bool GetFocus() { return m_focus; }
	bool GetDelete() { return m_delete; }
	bool GetDuplicate() { return m_duplicate; }
	bool GetSelect() { return m_select; }
	std::vector<int> GetSelectedLightIDs() { return m_selectedLightIDs; }
	std::vector<DisplayObject> GetLights() { return m_lights; }
	CONSTRAINT GetConstraint() { return m_constraint; }
	// Focus
	FocusDialogue* GetFocusDialogue() { return &m_focusDialogue; }

	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }
	void SetSelectedLightIDs(std::vector<int> selection) { m_selectedLightIDs = selection; }
	void SetRequest(bool request) { m_requestDisplayList = request; SetupLights(); }
	///void SetSceneGraph(std::vector<SceneObject>* sceneGraph) { m_sceneGraph = sceneGraph; }
	void SetDelete(bool del) { m_delete = del; m_requestDisplayList = !del; }
	void SetDuplicate(bool dup) { m_duplicate = dup; m_requestDisplayList = !dup; }
	void SetSelect(bool select) { m_select = select; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue

	// Local storage 
	std::vector<DisplayObject> m_displayList;
	std::vector<DisplayObject> m_lights;
	std::vector<int> m_selectedLightIDs;

	// Controllers
	bool m_active;
	bool m_update;
	bool m_translating;
	bool m_x, m_y, m_z;
	bool m_internal = false;
	bool m_requestDisplayList = false;
	bool m_focus = false;
	bool m_delete = false, m_duplicate = false;
	bool m_select = false;
	bool m_resetLights = false;
	CONSTRAINT m_constraint;

	// Focus 
	FocusDialogue m_focusDialogue;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient uses of the boxes
	CListBox m_boxID;
	CComboBox m_boxType, m_boxConst;
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
	bool m_lightSetup = false;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeID();
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedEnable();
	afx_msg void OnBnClickedFocus();
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
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedDuplicate();
	afx_msg void OnBnClickedTranslate();
	afx_msg void OnBnSelchangeConstraint();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();

private:
	// Setup IDs of currently available lights
	void SetupLights();
	
	// Update light details
	void UpdateType();
	void UpdateEnabled();
	void UpdatePosition();
	void UpdateDirection();
	void UpdateDiffuse();
	void UpdateAmbient();
	void UpdateConstA();
	void UpdateLinA();
	void UpdateQuadA();
	void UpdateSelectedConstraint();
	
	// Reset all variables
	void Reset();

private:
	// Getters
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetDirection();
	XMFLOAT3 GetDiffuse();
	float GetConstA(), GetLinA(), GetQuadA();
};
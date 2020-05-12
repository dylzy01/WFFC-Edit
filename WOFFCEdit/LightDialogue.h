#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "FocusDialogue.h"
#include "ToolMain.h"


// LightDialogue dialog

class LightDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(LightDialogue)

public:
	LightDialogue(CWnd* pParent = NULL);
	virtual ~LightDialogue() {}

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
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue

	// Local storage 
	ToolMain* m_toolSystem;
	std::vector<SceneObject>* m_lights;
	std::vector<int> m_selectedIDs;	
	int m_selectedIndex, m_previousIndex;

	// Controllers
	bool m_active;
	bool m_translating;
	bool m_x, m_y, m_z;
	bool m_internal = false;
	bool m_focus = false;
	bool m_select = false;
	OBJECT_FUNCTION m_function;
	CONSTRAINT m_constraint;

	// Focus 
	FocusDialogue m_focusDialogue;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient uses of the boxes
	CListBox m_boxID;
	CComboBox m_boxType, m_boxConst;
	CEdit m_ePosX, m_ePosY, m_ePosZ;
	CEdit m_eDifR, m_eDifG, m_eDifB;
	CEdit m_eAmbR, m_eAmbG, m_eAmbB;
	CEdit m_eConstA, m_eLinA, m_eQuadA;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeID();
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedEnable();
	afx_msg void OnBnClickedFocus();
	afx_msg void OnEnChangePosX();
	afx_msg void OnEnChangePosY();
	afx_msg void OnEnChangePosZ();
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
	void SetupLights(std::vector<SceneObject>* sceneGraph);
	
	// Update light details
	void UpdateDialogue(int ID = -1);
	void UpdateType();
	void UpdateEnabled();
	void UpdatePosition();
	void UpdateDiffuse();
	void UpdateAmbient();
	void UpdateConstA();
	void UpdateLinA();
	void UpdateQuadA();
	void UpdateSelectedConstraint();
	
	// Reset all variables
	void Reset();

	// Update object in scene graph
	void UpdateLight(SceneObject object);
};
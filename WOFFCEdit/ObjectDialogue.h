#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "SceneObject.h"
#include "ObjectManager.h"
#include "FocusDialogue.h"
#include "ToolMain.h"

// ObjectDialogue dialog

class ObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDialogue)

public:
	ObjectDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectDialogue() {}

	// Pass in a pointer for the class to operate on
	void SetToolSystem(ToolMain* toolSystem);	

	// Update ToolMain from this dialogue
	void UpdateTool();
	
	// Getters
	bool GetActive() { return m_active; }
	
	// Setters
	void SetActive(bool active) { m_active = active; }	

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG12 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue

	// Local storage
	ToolMain* m_toolSystem;
	std::vector<SceneObject>* m_objects;
	std::vector<int> m_selectedIDs;
	int m_selectedIndex;
	
	// Controllers
	bool m_active;
	bool m_focus = false, m_focusCreated = false;
	bool m_transforming = false;
	bool m_x, m_y, m_z;		
	bool m_internal = false;
	bool m_select = false;
	float m_snapFactor;
	OBJECT_FUNCTION m_function;
	CONSTRAINT m_constraint;

	// Focus 
	FocusDialogue m_focusDialogue;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient uses of the boxes
	CListBox m_boxID;
	CComboBox m_boxType, m_boxConst;
	CEdit m_eScaX, m_eScaY, m_eScaZ;
	CEdit m_eRotX, m_eRotY, m_eRotZ;
	CEdit m_ePosX, m_ePosY, m_ePosZ;

	// Message handlers
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeID();
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedFocus();
	afx_msg void OnEnChangeScaX();
	afx_msg void OnEnChangeScaY();
	afx_msg void OnEnChangeScaZ();
	afx_msg void OnEnChangeRotX();
	afx_msg void OnEnChangeRotY();
	afx_msg void OnEnChangeRotZ();
	afx_msg void OnEnChangePosX();
	afx_msg void OnEnChangePosY();
	afx_msg void OnEnChangePosZ();
	afx_msg void OnBnClickedScale();
	afx_msg void OnBnClickedRotate();
	afx_msg void OnBnClickedTranslate();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedDuplicate();

private:
	// Setup IDs of currently available objects
	void SetupObjects(std::vector<SceneObject>* sceneGraph);

	// Uncheck other function buttons
	void Uncheck();

	// Update object details
	void UpdateDialogue(int ID = -1);
	void UpdateType();	
	void UpdateScale();
	void UpdateRotation();
	void UpdatePosition();
	void UpdateTransform() {
		UpdateScale();
		UpdateRotation();
		UpdatePosition();
	}
	
	// Update constraint details
	void UpdateSelectedConstraint();

	// Update object in scene graph
	void UpdateObject(SceneObject object);

	// Reset all variables
	void Reset();
};
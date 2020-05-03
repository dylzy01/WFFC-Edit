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

// ObjectDialogue dialog

class ObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDialogue)

public:
	ObjectDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectDialogue() {}

	// Pass in data pointers the class will operate on
	void SetObjectData(std::vector<SceneObject>* sceneGraph);

	// Update current object with dialogue values
	void Update(int ID = -1);

	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }
	bool GetFocus() { return m_focus; }
	bool GetTransforming() { return m_transforming; }
	bool GetObjectsSetup() { return m_objectSetup; }
	bool GetRequest() { return m_requestSceneGraph; }
	float GetSnap() { if (m_snapTerrain) { return -1; } else if (m_snapValue) { return m_snapScale; } }
	bool GetDelete() { return m_delete; }
	bool GetDuplicate() { return m_duplicate; }
	bool GetSelect() { return m_select; }
	std::vector<int> GetSelectedObjectIDs() { return m_selectedObjectIDs; }
	std::vector<SceneObject> GetObjects() { return m_objects; }
	OBJECT_FUNCTION GetFunction() { return m_function; }
	CONSTRAINT GetConstraint() { return m_constraint; }
	// Focus
	FocusDialogue* GetFocusDialogue() { return &m_focusDialogue; }
	
	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }
	void SetTransforming(bool transforming) { m_transforming = transforming; }
	void SetSelectedObjects(std::vector<int> IDs) { m_selectedObjectIDs = IDs; }
	void SetRequest(bool request) { m_requestSceneGraph = request; SetupObjects(); }
	void SetDelete(bool del) { m_delete = del; m_requestSceneGraph = !del; }
	void SetDuplicate(bool dup) { m_duplicate = dup; m_requestSceneGraph = !dup; }
	void SetSelect(bool select) { m_select = select; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG12 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupCheckBoxes();
	afx_msg void End();									// Kill the dialogue

	// Local storage
	std::vector<SceneObject> m_sceneGraph;
	std::vector<SceneObject> m_objects;
	std::vector<int> m_selectedObjectIDs;
	
	// Controllers
	bool m_active;
	bool m_update;
	bool m_focus;
	bool m_transforming = false;
	bool m_objectSetup = false;
	bool m_x, m_y, m_z;		
	bool m_snapTerrain, m_snapValue;	
	bool m_internal = false;
	bool m_requestSceneGraph = false;
	float m_snapScale;
	bool m_delete = false, m_duplicate = false;
	bool m_select = false;
	bool m_resetObjects = false;
	///int m_selection;	
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
	CEdit m_eSnap;

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
	afx_msg void OnBnClickedSnapTerrain();
	afx_msg void OnBnClickedSnapValue();
	afx_msg void OnEnChangeSnapValue();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedDuplicate();

private:
	// Setup IDs of currently available objects
	void SetupObjects();

	// Uncheck other function buttons
	void Uncheck();

	// Update object details
	void UpdateType();
	void UpdateScale();
	void UpdateRotation();
	void UpdatePosition();
	
	// Update constraint details
	void UpdateSelectedConstraint();
};
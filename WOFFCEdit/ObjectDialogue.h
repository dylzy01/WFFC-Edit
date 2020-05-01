#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include <string>
#include "Tools.h"
#include "SceneObject.h"
#include "ObjectManager.h"

// ObjectDialogue dialog

class ObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDialogue)

public:
	ObjectDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectDialogue() {}

	// Pass in data pointers the class will operate on
	void SetObjectData(std::vector<SceneObject>* sceneGraph);

	// Get currently selected object ID 
	int GetSelectedObjectID() { 
		if (m_objects.size() != 0 && m_boxID.GetCurSel() >= 0 && m_boxID.GetCurSel() <= m_objects.size()) {
			return m_objects[m_boxID.GetCurSel()].ID;
		}		
	}

	// Update selected object
	void UpdateSelection(int ID) { m_selection = ID; }

	// Update current object with dialogue values
	void Update(int ID);

	// Getters
	bool GetActive() { return m_active; }
	bool GetUpdate() { return m_update; }
	bool GetFocus() { return m_focus; }
	bool GetTransforming() { return m_transforming; }
	bool GetObjectsSetup() { return m_objectSetup; }
	float GetSnap() { if (m_snapTerrain) { return -1; } else if (m_snapValue) { return m_snapScale; } }
	int GetSelection() { return m_selection; }
	std::vector<SceneObject> GetSceneGraph() { return m_objects; }
	OBJECT_FUNCTION GetFunction() { return m_function; }
	CONSTRAINT GetConstraint() { return m_constraint; }
	
	// Setters
	void SetActive(bool active) { m_active = active; }
	void SetUpdate(bool update) { m_update = update; }
	void SetTransforming(bool transforming) { m_transforming = transforming; }

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
	
	// Controllers
	bool m_active;
	bool m_update;
	bool m_focus;
	bool m_transforming = false;
	bool m_objectSetup = false;
	bool m_x, m_y, m_z;
	bool m_snapTerrain, m_snapValue;
	float m_snapScale;
	int m_selection;
	OBJECT_FUNCTION m_function;
	CONSTRAINT m_constraint;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient uses of the boxes
	CComboBox m_boxID, m_boxType, m_boxConst;
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
	void UpdateType(int ID);
	void UpdateScale(int ID);
	void UpdateRotation(int ID);
	void UpdatePosition(int ID);
	
	// Update constraint details
	void UpdateSelectedConstraint();
};
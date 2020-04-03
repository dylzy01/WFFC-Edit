#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "Tools.h"

// ObjectDialogue dialog

class ObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDialogue)

public:
	ObjectDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectDialogue();

	// Setup entire list of objects
	void SetObjectData(std::vector<SceneObject>* sceneGraph, std::vector<int> * objects);	

	// Getters
	bool GetActive() { return m_active; }
	EDITOR GetEditor() { return m_editor; }
	OBJECT_SPAWN GetSpawn() { return m_spawn; }
	OBJECT_FUNCTION GetTransform() { return m_transform; }
	bool GetConstraintX() { return m_x; }
	bool GetConstraintY() { return m_y; }
	bool GetConstraintZ() { return m_z; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Kill dialogue
	afx_msg void End();

	// Reset constraints after each function change
	void ResetConstraints() { m_x = m_y = m_z = false; }

	// Enable/disable other functions
	void UpdateTransform(OBJECT_FUNCTION function, bool enable);

	// Enable/disable other spawns
	void UpdateSpawns(OBJECT_SPAWN spawn, bool enable);

protected:
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int>* m_objectIDs;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGrass();
	afx_msg void OnBnClickedTree();
	afx_msg void OnBnClickedWater();
	afx_msg void OnBnClickedScale();
	afx_msg void OnBnClickedTranslate();
	afx_msg void OnBnClickedRotate();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();

	// Controllers
	bool m_active = false;
	EDITOR m_editor = EDITOR::NA;
	OBJECT_SPAWN m_spawn = OBJECT_SPAWN::NA;
	OBJECT_FUNCTION m_transform = OBJECT_FUNCTION::NA;
	bool m_x = false, m_y = false, m_z = false;

	DECLARE_MESSAGE_MAP()
};

///INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
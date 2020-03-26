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
	OBJECT_FUNCTION GetFunction() { return m_function; }
	OBJECT_CONSTRAINT GetConstraint() { return m_constraint; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Kill dialogue
	afx_msg void End();
	
	// Select object
	afx_msg void OnBnClickedSelect();

	// Update function display text
	void UpdateFunctionText(OBJECT_FUNCTION function);

	// Update constraint display text
	void UpdateConstraintText(OBJECT_CONSTRAINT constraint);

protected:
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int>* m_objectIDs;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedScale();
	afx_msg void OnBnClickedTranslate();
	afx_msg void OnBnClickedRotate();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();
	afx_msg void OnBnClickedAll();

	// Controllers
	bool m_active = false;
	OBJECT_FUNCTION m_function = OBJECT_FUNCTION::NA;
	OBJECT_CONSTRAINT m_constraint = OBJECT_CONSTRAINT::NA;

	DECLARE_MESSAGE_MAP()
};

///INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
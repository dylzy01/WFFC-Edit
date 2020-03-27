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
	bool GetFunctionTranslate() { return m_translate; }
	bool GetFunctionRotate() { return m_rotate; }
	bool GetFunctionScale() { return m_scale; }
	bool GetConstraintX() { return m_x; }
	bool GetConstraintY() { return m_y; }
	bool GetConstraintZ() { return m_z; }

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
	void UpdateFunctions(OBJECT_FUNCTION function, bool enable);

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

	// Controllers
	bool m_active = false;
	bool m_translate = false, m_rotate = false, m_scale = false;
	bool m_x = false, m_y = false, m_z = false;

	DECLARE_MESSAGE_MAP()
};

///INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
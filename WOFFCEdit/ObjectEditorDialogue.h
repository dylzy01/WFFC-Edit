#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "Tools.h"

// ObjectEditorDialogue dialog

class ObjectEditorDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectEditorDialogue)

public:
	ObjectEditorDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectEditorDialogue();

	// Getters
	bool GetActive() { return m_active; }
	OBJECT_FUNCTION GetFunction() { return m_function; }
	bool GetConstraintX() { return m_x; }
	bool GetConstraintY() { return m_y; }
	bool GetConstraintZ() { return m_z; }

	// Setters
	void SetActive(bool active) { m_active = active; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	// Enable/disable other buttons
	void UpdateButtons(bool enable);

	// Reset all constraints
	void ResetConstraints() { m_x = m_y = m_z = false; }

	// Controllers
	bool m_active = false;
	OBJECT_FUNCTION m_function = OBJECT_FUNCTION::NA;
	bool m_x = false, m_y = false, m_z = false;

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedTranslate();
	afx_msg void OnBnClickedRotate();
	afx_msg void OnBnClickedScale();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();
};

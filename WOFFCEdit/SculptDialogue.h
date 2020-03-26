#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "Tools.h"

// SculptDialogue dialog

class SculptDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SculptDialogue)

public:
	SculptDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SculptDialogue();

	// Setup entire list of objects
	void SetTerrainData(std::vector<SceneObject>* sceneGraph, std::vector<TERRAIN> * terrain);

	// Getters
	SCULPT_FUNCTION GetFunction() { return m_function; }
	SCULPT_CONSTRAINT GetConstraint() { return m_constraint; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Kill dialogue
	afx_msg void End();

	// Select object
	afx_msg void Select();

protected:
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<TERRAIN>* m_terrain;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedIncrease();
	afx_msg void OnBnClickedFlatten();
	afx_msg void OnBnClickedDecrease();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();
	afx_msg void OnBnClickedAll();

	// Controllers
	bool m_active = false;
	SCULPT_FUNCTION m_function = SCULPT_FUNCTION::NA;
	SCULPT_CONSTRAINT m_constraint = SCULPT_CONSTRAINT::NA;

	DECLARE_MESSAGE_MAP()
};

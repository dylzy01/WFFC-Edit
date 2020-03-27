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
	bool GetFunctionIncrease() { return m_increase; }
	bool GetFunctionDecrease() { return m_decrease; }
	bool GetFunctionFlatten() { return m_flatten; }
	bool GetConstraintX() { return m_x; }
	bool GetConstraintY() { return m_y; }
	bool GetConstraintZ() { return m_z; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Kill dialogue
	afx_msg void End();

	// Reset constraints after each function change
	void ResetConstraints() { m_x = m_y = m_z = false; }

	// Enable/disable other functions
	void UpdateFunctions(SCULPT_FUNCTION function, bool enable);

protected:
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<TERRAIN>* m_terrain;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedIncrease();	
	afx_msg void OnBnClickedDecrease();
	afx_msg void OnBnClickedFlatten();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();

	// Controllers
	bool m_active = false;
	bool m_increase = false, m_decrease = false, m_flatten = false;
	bool m_x = false, m_y = false, m_z = false;

	DECLARE_MESSAGE_MAP()
};

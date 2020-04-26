#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "Tools.h"
#include "Light.h"

// LightDialogue dialog

class LightDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(LightDialogue)

public:
	LightDialogue(CWnd* pParent = NULL);
	virtual ~LightDialogue() {}

	// Pass in data pointers the class will operate on
	void SetLightData(std::pair<std::vector<Light*>, std::vector<int>> lights);

	// Update current light with dialogue values
	void Update();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();									// Kill the dialogue
	afx_msg void Select();								// Light has been selected

	// Local storage 
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int>* m_IDs;
	std::pair<std::vector<Light*>, std::vector<int>> m_lights;

	DECLARE_MESSAGE_MAP()
public:
	// Control variables for more efficient uses of the boxes
	CComboBox m_boxID, m_boxType;
	afx_msg void OnBnClickedOk();

};

// LightDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "LightDialogue.h"
#include "afxdialogex.h"


// LightDialogue dialog

IMPLEMENT_DYNAMIC(LightDialogue, CDialogEx)

LightDialogue::LightDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{
	///m_sceneGraph = sceneGraph;
}

void LightDialogue::SetLightData(std::pair<std::vector<Light*>, std::vector<int>> lights)
{
	m_lights = lights;

	// Loop through lights and add an entry to the ID/type combo boxes
	for (int i = 0; i < m_lights.first.size(); ++i)
	{
		// ID
		std::wstring idBoxEntry = std::to_wstring(m_lights.second[i]);
		m_boxID.AddString(idBoxEntry.c_str());

		// Type
		std::wstring typeBoxEntry = std::to_wstring((int)m_lights.first[i]->GetType());
		switch (m_lights.first[i]->GetType())
		{
		case LIGHT_TYPE::DIRECTIONAL:	typeBoxEntry = L"Directional";	break;		
		case LIGHT_TYPE::POINT:			typeBoxEntry = L"Point";		break;		
		case LIGHT_TYPE::SPOT:			typeBoxEntry = L"Spot";			break;		
		}
		m_boxType.AddString(typeBoxEntry.c_str());
	}

	// Setup total lights in scene
	int total = m_lights.first.size();
	std::wstring totalLights = std::to_wstring(total);
	SetDlgItemText(IDC_STATIC3, totalLights.c_str());
}

void LightDialogue::Update()
{
}

void LightDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_boxID);
	DDX_Control(pDX, IDC_COMBO2, m_boxType);
}

BEGIN_MESSAGE_MAP(LightDialogue, CDialogEx)
END_MESSAGE_MAP()

void LightDialogue::End()
{
}

void LightDialogue::Select()
{
}

void LightDialogue::OnBnClickedOk()
{
}
// TerrainDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainDialogue.h"
#include "afxdialogex.h"


// TerrainDialogue dialog

IMPLEMENT_DYNAMIC(TerrainDialogue, CDialogEx)

TerrainDialogue::TerrainDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG9, pParent)
{

}

// Pass in data pointers the class will operate on
void TerrainDialogue::SetChunkData(DisplayChunk* displayChunk)
{
	// Local storage 
	m_displayChunk = displayChunk;

	// Setup indexes
	for (int i = 0; i < TERRAINRESOLUTION - 1; ++i)
	{
		std::wstring indexBoxEntry;
		indexBoxEntry = std::to_wstring(i);
		m_boxRow.AddString(indexBoxEntry.c_str());
		m_boxCol.AddString(indexBoxEntry.c_str());
	}

	// Setup texture types
	std::wstring texBoxEntry;
	texBoxEntry = L"NA";			m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Grass";			m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Dirt";			m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Sand";			m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Stone";			m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Snow";			m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Grass/Dirt";	m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Grass/Sand";	m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Grass/Stone";	m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Grass/Snow";	m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Dirt/Sand";		m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Dirt/Stone";	m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Dirt/Snow";		m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Sand/Stone";	m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Sand/Snow";		m_boxTex.AddString(texBoxEntry.c_str());
	texBoxEntry = L"Stone/Snow";	m_boxTex.AddString(texBoxEntry.c_str());

	// Display first terrain
	Update(0, 0);
}

// Update current terrain with dialogue values/vice versa
void TerrainDialogue::Update(int row, int column)
{
	m_boxRow.SetCurSel(row);
	m_boxCol.SetCurSel(column);
	UpdateCoordinates(row, column);
	UpdateTexture(row, column);
}

void TerrainDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO3, m_boxRow);
	DDX_Control(pDX, IDC_COMBO4, m_boxCol);
	DDX_Control(pDX, IDC_EDIT16, m_eCoordX);
	DDX_Control(pDX, IDC_EDIT17, m_eCoordY);
	DDX_Control(pDX, IDC_EDIT18, m_eCoordZ);
	DDX_Control(pDX, IDC_COMBO1, m_boxTex);
}


BEGIN_MESSAGE_MAP(TerrainDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TerrainDialogue::End)
	ON_BN_CLICKED(IDOK, &TerrainDialogue::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO3, &TerrainDialogue::OnBnSelchangeIndex)
	ON_CBN_SELCHANGE(IDC_COMBO4, &TerrainDialogue::OnBnSelchangeIndex)
	ON_CBN_SELCHANGE(IDC_COMBO1, &TerrainDialogue::OnBnSelchangeTexture)	
END_MESSAGE_MAP()


// TerrainDialogue message handlers

// Kill the dialogue
void TerrainDialogue::End()
{
	DestroyWindow();
}

// User is finished with dialogue
void TerrainDialogue::OnBnClickedOk()
{
	m_active = false;
	End();
	CDialogEx::OnOK();
}

// Row or Column has been changed
void TerrainDialogue::OnBnSelchangeIndex()
{
	// Store index selection
	int row = m_boxRow.GetCurSel();
	int col = m_boxCol.GetCurSel();

	// Update coordinates
	UpdateCoordinates(row, col);

	// Update texture type
	UpdateTexture(row, col);

	// Tell MFC/ToolMain to update display chunk
	m_update = true;
}

// Texture has been selected
void TerrainDialogue::OnBnSelchangeTexture()
{
	// Store index selection
	int row = m_boxRow.GetCurSel();
	int col = m_boxCol.GetCurSel();

	// Store texture selection
	int texture = m_boxTex.GetCurSel();

	// Set new paint
	m_displayChunk->OverwritePaint(row, col, (TERRAIN_PAINT)texture);

	// Tell MFC/ToolMain to update display chunk
	m_update = true;
}

// Update remaining terrain details when one is changed /////////////////////////////////////////////

void TerrainDialogue::UpdateCoordinates(int row, int column)
{
	// Store position of selected terrain
	DirectX::XMFLOAT3 coords = m_displayChunk->GetGeometry(row, column).position;

	// Update X coord box
	CString sX; sX.Format(L"%g", coords.x);
	m_eCoordX.SetWindowTextW(sX);

	// Update Y coord box
	CString sY; sY.Format(L"%g", coords.y);
	m_eCoordY.SetWindowTextW(sY);

	// Update Z coord box
	CString sZ; sZ.Format(L"%g", coords.z);
	m_eCoordZ.SetWindowTextW(sZ);
}

void TerrainDialogue::UpdateTexture(int row, int column)
{
	// Store texture of selected terrain
	TERRAIN_PAINT paint = m_displayChunk->GetPaint(row, column);

	// Set combo box
	m_boxTex.SetCurSel((int)paint);
}
// ObjectEditorDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditorDialogue.h"
#include "afxdialogex.h"


// ObjectEditorDialogue dialog

IMPLEMENT_DYNAMIC(ObjectEditorDialogue, CDialogEx)

ObjectEditorDialogue::ObjectEditorDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{
	
}

ObjectEditorDialogue::~ObjectEditorDialogue()
{
}

void ObjectEditorDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void ObjectEditorDialogue::UpdateButtons(bool enable)
{
	// Update function
	if (enable) { m_function = OBJECT_FUNCTION::NA; }

	// Switch between functions
	switch (m_function)
	{
	case OBJECT_FUNCTION::SELECT:
	{
		// Enable/disable translate button
		if (!enable) { CheckDlgButton(IDC_CHECK1, enable); }
		GetDlgItem(IDC_CHECK1)->EnableWindow(enable);

		// Enable/disable rotate button
		if (!enable) { CheckDlgButton(IDC_CHECK2, enable); }
		GetDlgItem(IDC_CHECK2)->EnableWindow(enable);

		// Enable/disable scale button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable X button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable Y button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable Z button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		GetDlgItem(IDC_CHECK14)->EnableWindow(enable);
	}
	break;
	case OBJECT_FUNCTION::TRANSLATE:
	{
		// Enable/disable select button
		if (!enable) { CheckDlgButton(IDC_CHECK4, enable); }

		// Enable/disable rotate button
		if (!enable) { CheckDlgButton(IDC_CHECK2, enable); }

		// Enable/disable scale button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }

		// Enable X button
		GetDlgItem(IDC_CHECK8)->EnableWindow(true);

		// Enable Y button
		GetDlgItem(IDC_CHECK13)->EnableWindow(true);

		// Enable Z button
		GetDlgItem(IDC_CHECK14)->EnableWindow(true);
	}
	break;
	case OBJECT_FUNCTION::ROTATE:
	{
		// Enable/disable select button
		if (!enable) { CheckDlgButton(IDC_CHECK4, enable); }

		// Enable/disable translate button
		if (!enable) { CheckDlgButton(IDC_CHECK1, enable); }

		// Enable/disable scale button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }

		// Enable X button
		GetDlgItem(IDC_CHECK8)->EnableWindow(true);

		// Enable Y button
		GetDlgItem(IDC_CHECK13)->EnableWindow(true);

		// Enable Z button
		GetDlgItem(IDC_CHECK14)->EnableWindow(true);
	}
	break;
	case OBJECT_FUNCTION::SCALE:
	{
		// Enable/disable select button
		if (!enable) { CheckDlgButton(IDC_CHECK4, enable); }

		// Enable/disable translate button
		if (!enable) { CheckDlgButton(IDC_CHECK1, enable); }

		// Enable/disable rotate button
		if (!enable) { CheckDlgButton(IDC_CHECK2, enable); }

		// Enable X button
		GetDlgItem(IDC_CHECK8)->EnableWindow(true);

		// Enable Y button
		GetDlgItem(IDC_CHECK13)->EnableWindow(true);

		// Enable Z button
		GetDlgItem(IDC_CHECK14)->EnableWindow(true);
	}
	break;
	case OBJECT_FUNCTION::NA:
	{
		// Enable/disable select button
		if (!enable) { CheckDlgButton(IDC_CHECK4, enable); }

		// Enable/disable translate button
		if (!enable) { CheckDlgButton(IDC_CHECK1, enable); }

		// Enable/disable rotate button
		if (!enable) { CheckDlgButton(IDC_CHECK2, enable); }

		// Enable/disable scale button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }

		// Enable X button
		GetDlgItem(IDC_CHECK8)->EnableWindow(true);

		// Enable Y button
		GetDlgItem(IDC_CHECK13)->EnableWindow(true);

		// Enable Z button
		GetDlgItem(IDC_CHECK14)->EnableWindow(true);
	}
	break;
	}
}


BEGIN_MESSAGE_MAP(ObjectEditorDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ObjectEditorDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectEditorDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK4, &ObjectEditorDialogue::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_CHECK1, &ObjectEditorDialogue::OnBnClickedTranslate)
	ON_BN_CLICKED(IDC_CHECK2, &ObjectEditorDialogue::OnBnClickedRotate)
	ON_BN_CLICKED(IDC_CHECK7, &ObjectEditorDialogue::OnBnClickedScale)
	ON_BN_CLICKED(IDC_CHECK8, &ObjectEditorDialogue::OnBnClickedX)
	ON_BN_CLICKED(IDC_CHECK13, &ObjectEditorDialogue::OnBnClickedY)
	ON_BN_CLICKED(IDC_CHECK14, &ObjectEditorDialogue::OnBnClickedZ)
END_MESSAGE_MAP()


// ObjectEditorDialogue message handlers

// Kill the dialogue
void ObjectEditorDialogue::End()
{
	DestroyWindow();
}

// Set function to none & de-activate
void ObjectEditorDialogue::OnBnClickedOk()
{
	m_active = m_x = m_y = m_z = false;
	m_function = OBJECT_FUNCTION::NA;
	UpdateButtons(false);
	CDialogEx::OnOK();
}

// Set function to select
void ObjectEditorDialogue::OnBnClickedSelect()
{
	m_function = OBJECT_FUNCTION::SELECT;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK4))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons 
	case false: UpdateButtons(true); break;
	}
}

// Set function to translate
void ObjectEditorDialogue::OnBnClickedTranslate()
{
	m_function = OBJECT_FUNCTION::TRANSLATE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK1))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons & reset contraints
	case false: UpdateButtons(true); ResetConstraints(); break;
	}
}

// Set function to rotate
void ObjectEditorDialogue::OnBnClickedRotate()
{
	m_function = OBJECT_FUNCTION::ROTATE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK2))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons & reset contraints
	case false: UpdateButtons(true); ResetConstraints(); break;
	}
}

// Set function to scale
void ObjectEditorDialogue::OnBnClickedScale()
{
	m_function = OBJECT_FUNCTION::SCALE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK7))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons & reset contraints
	case false: UpdateButtons(true); ResetConstraints(); break;
	}
}

// Check/uncheck X constraint
void ObjectEditorDialogue::OnBnClickedX() { m_x = IsDlgButtonChecked(IDC_CHECK8); }

// Check/uncheck Y constraint
void ObjectEditorDialogue::OnBnClickedY() { m_y = IsDlgButtonChecked(IDC_CHECK13); }

// Check/uncheck Z constraint
void ObjectEditorDialogue::OnBnClickedZ() { m_z = IsDlgButtonChecked(IDC_CHECK14); }
// ObjectSpawnDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectSpawnDialogue.h"
#include "afxdialogex.h"


// ObjectSpawnDialogue dialog

IMPLEMENT_DYNAMIC(ObjectSpawnDialogue, CDialogEx)

ObjectSpawnDialogue::ObjectSpawnDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

ObjectSpawnDialogue::~ObjectSpawnDialogue()
{
}

void ObjectSpawnDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void ObjectSpawnDialogue::UpdateButtons(bool enable)
{
	// Update spawn
	if (enable) { m_spawn = OBJECT_TYPE::NA; }

	// Switch between spawns
	switch (m_spawn)
	{
	// Residential /////////////////////////////////////////////////////////////
	case OBJECT_TYPE::HOUSE_ONE:
	{
		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::HOUSE_TWO:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::CAVE:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	// Props ///////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::BRIDGE:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::FENCE:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::BOAT:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	// Nature //////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::GRASS:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::TREE_ONE:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::TREE_TWO:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::WATER:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;	
	// Misc ////////////////////////////////////////////////////////////////////
	case OBJECT_TYPE::LIGHT:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::CUBE:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);	

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::CYLINDER:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);	

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	case OBJECT_TYPE::CONE:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);

		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);	

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }
	}
	break;
	case OBJECT_TYPE::NA:
	{
		// Enable/disable house #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK8, enable); }
		///GetDlgItem(IDC_CHECK8)->EnableWindow(enable);

		// Enable/disable house #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK13, enable); }
		///GetDlgItem(IDC_CHECK13)->EnableWindow(enable);

		// Enable/disable cave button
		if (!enable) { CheckDlgButton(IDC_CHECK16, enable); }
		///GetDlgItem(IDC_CHECK16)->EnableWindow(enable);

		// Enable/disable bridge button
		if (!enable) { CheckDlgButton(IDC_CHECK14, enable); }
		///GetDlgItem(IDC_CHECK14)->EnableWindow(enable);

		// Enable/disable fence button
		if (!enable) { CheckDlgButton(IDC_CHECK17, enable); }
		///GetDlgItem(IDC_CHECK17)->EnableWindow(enable);

		// Enable/disable boat button
		if (!enable) { CheckDlgButton(IDC_CHECK18, enable); }
		///GetDlgItem(IDC_CHECK18)->EnableWindow(enable);
		
		// Enable/disable grass button
		if (!enable) { CheckDlgButton(IDC_CHECK5, enable); }
		///GetDlgItem(IDC_CHECK5)->EnableWindow(enable);

		// Enable/disable tree #1 button
		if (!enable) { CheckDlgButton(IDC_CHECK6, enable); }
		///GetDlgItem(IDC_CHECK6)->EnableWindow(enable);

		// Enable/disable tree #2 button
		if (!enable) { CheckDlgButton(IDC_CHECK15, enable); }
		///GetDlgItem(IDC_CHECK15)->EnableWindow(enable);

		// Enable/disable water button
		if (!enable) { CheckDlgButton(IDC_CHECK7, enable); }
		///GetDlgItem(IDC_CHECK7)->EnableWindow(enable);	

		// Enable/disable light button
		if (!enable) { CheckDlgButton(IDC_CHECK19, enable); }

		// Enable/disable cube button
		if (!enable) { CheckDlgButton(IDC_CHECK20, enable); }

		// Enable/disable cylinder button
		if (!enable) { CheckDlgButton(IDC_CHECK22, enable); }

		// Enable/disable cone button
		if (!enable) { CheckDlgButton(IDC_CHECK21, enable); }
	}
	break;
	}
}


BEGIN_MESSAGE_MAP(ObjectSpawnDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ObjectSpawnDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectSpawnDialogue::OnBnClickedOk)

	ON_BN_CLICKED(IDC_CHECK8, &ObjectSpawnDialogue::OnBnClickedHouseOne)
	ON_BN_CLICKED(IDC_CHECK13, &ObjectSpawnDialogue::OnBnClickedHouseTwo)
	ON_BN_CLICKED(IDC_CHECK16, &ObjectSpawnDialogue::OnBnClickedCave)

	ON_BN_CLICKED(IDC_CHECK14, &ObjectSpawnDialogue::OnBnClickedBridge)
	ON_BN_CLICKED(IDC_CHECK17, &ObjectSpawnDialogue::OnBnClickedFence)
	ON_BN_CLICKED(IDC_CHECK18, &ObjectSpawnDialogue::OnBnClickedBoat)

	ON_BN_CLICKED(IDC_CHECK5, &ObjectSpawnDialogue::OnBnClickedGrass)
	ON_BN_CLICKED(IDC_CHECK6, &ObjectSpawnDialogue::OnBnClickedTreeOne)
	ON_BN_CLICKED(IDC_CHECK15, &ObjectSpawnDialogue::OnBnClickedTreeTwo)
	ON_BN_CLICKED(IDC_CHECK7, &ObjectSpawnDialogue::OnBnClickedWater)


	ON_BN_CLICKED(IDC_CHECK19, &ObjectSpawnDialogue::OnBnClickedLight)
	ON_BN_CLICKED(IDC_CHECK20, &ObjectSpawnDialogue::OnBnClickedCube)
	ON_BN_CLICKED(IDC_CHECK22, &ObjectSpawnDialogue::OnBnClickedCylinder)
	ON_BN_CLICKED(IDC_CHECK21, &ObjectSpawnDialogue::OnBnClickedCone)
END_MESSAGE_MAP()


// ObjectSpawnDialogue message handlers

// Kill the dialogue
void ObjectSpawnDialogue::End()
{
	OnBnClickedOk();
	DestroyWindow();
}

// Set spawn to none & de-activate
void ObjectSpawnDialogue::OnBnClickedOk() 
{ 
	m_active = false;
	UpdateButtons(true);
	CDialogEx::OnOK();
}

// Residential /////////////////////////////////////////////////////////////////////

// Set spawn to house #1
void ObjectSpawnDialogue::OnBnClickedHouseOne()
{
	m_spawn = OBJECT_TYPE::HOUSE_ONE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK8))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to house #2
void ObjectSpawnDialogue::OnBnClickedHouseTwo()
{
	m_spawn = OBJECT_TYPE::HOUSE_TWO;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK13))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to cave
void ObjectSpawnDialogue::OnBnClickedCave()
{
	m_spawn = OBJECT_TYPE::CAVE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK16))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Props ///////////////////////////////////////////////////////////////////////////

// Set spawn to bridge
void ObjectSpawnDialogue::OnBnClickedBridge()
{
	m_spawn = OBJECT_TYPE::BRIDGE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK14))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to fence
void ObjectSpawnDialogue::OnBnClickedFence()
{
	m_spawn = OBJECT_TYPE::FENCE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK17))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to boat
void ObjectSpawnDialogue::OnBnClickedBoat()
{
	m_spawn = OBJECT_TYPE::BOAT;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK18))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Nature //////////////////////////////////////////////////////////////////////////

// Set spawn to grass
void ObjectSpawnDialogue::OnBnClickedGrass()
{
	m_spawn = OBJECT_TYPE::GRASS;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK5))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to tree #1
void ObjectSpawnDialogue::OnBnClickedTreeOne()
{
	m_spawn = OBJECT_TYPE::TREE_ONE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK6))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to tree #2
void ObjectSpawnDialogue::OnBnClickedTreeTwo()
{
	m_spawn = OBJECT_TYPE::TREE_TWO;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK15))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to water
void ObjectSpawnDialogue::OnBnClickedWater()
{
	m_spawn = OBJECT_TYPE::WATER;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK7))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to light
void ObjectSpawnDialogue::OnBnClickedLight()
{
	m_spawn = OBJECT_TYPE::LIGHT;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK19))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to cube
void ObjectSpawnDialogue::OnBnClickedCube()
{
	m_spawn = OBJECT_TYPE::CUBE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK20))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to cylinder
void ObjectSpawnDialogue::OnBnClickedCylinder()
{
	m_spawn = OBJECT_TYPE::CYLINDER;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK22))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Set spawn to cone
void ObjectSpawnDialogue::OnBnClickedCone()
{
	m_spawn = OBJECT_TYPE::CONE;

	// Switch between button checked/unchecked
	switch (IsDlgButtonChecked(IDC_CHECK21))
	{
		// Uncheck & disable other buttons
	case true: UpdateButtons(false); break;
		// Enable other buttons
	case false: UpdateButtons(true); break;
	}
}

// Open residential spawner
//void ObjectSpawnDialogue::OnBnClickedResidential()
//{
//	m_spawn = OBJECT_TYPE::RESIDENTIAL;
//	
//	// Create & display dialogue window
//	m_residentialObjectDialogue.Create(IDD_DIALOG8);
//	m_residentialObjectDialogue.ShowWindow(SW_SHOW);
//	m_residentialObjectDialogue.SetActive(true);
//}
//
//// Open nature spawner
//void ObjectSpawnDialogue::OnBnClickedNature()
//{
//	m_spawn = OBJECT_TYPE::NATURE;
//
//	// Create & display dialogue window
//	m_natureObjectDialogue.Create(IDD_DIALOG9);
//	m_natureObjectDialogue.ShowWindow(SW_SHOW);
//	m_natureObjectDialogue.SetActive(true);
//}
//
//// Open prop spawner
//void ObjectSpawnDialogue::OnBnClickedProps()
//{
//}

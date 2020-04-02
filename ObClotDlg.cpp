/*===========================================================================
 *
 * File:	Obclotdlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObClotDlg.h"
#include "windows/obrecordlistctrl.h"


/*===========================================================================
 *
 * Begin Local Definitions
 *
 *=========================================================================*/
#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif
/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObClotDlg, CObRecordDialog)
	//{{AFX_MSG_MAP(CObClotDlg)
	ON_BN_CLICKED(IDC_EDIT_ENCHANTMENT, OnEditEnchantment)
	ON_BN_CLICKED(IDC_EDIT_SCRIPT, OnEditScript)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Begin UI Field Map
 *
 *=========================================================================*/
BEGIN_OBRECUIFIELDS(CObClotDlg)
	ADD_OBRECUIFIELDS( OB_FIELD_EDITORID,	   IDC_EDITORID)
	ADD_OBRECUIFIELDS( OB_FIELD_FORMID,	   IDC_FORMID)
	ADD_OBRECUIFIELDS( OB_FIELD_ITEMNAME,	   IDC_NAME)
	ADD_OBRECUIFIELDS( OB_FIELD_VALUE,	   IDC_VALUE)
	ADD_OBRECUIFIELDS( OB_FIELD_WEIGHT,	   IDC_WEIGHT)
	ADD_OBRECUIFIELDS( OB_FIELD_ENCHANTMENT,   IDC_ENCHANTMENT)
	ADD_OBRECUIFIELDS( OB_FIELD_SCRIPT,	   IDC_SCRIPT)
	ADD_OBRECUIFIELDS( OB_FIELD_PLAYABLE,      IDC_PLAYABLE)
	ADD_OBRECUIFIELDS( OB_FIELD_HIDERINGS,     IDC_HIDERINGS)
	ADD_OBRECUIFIELDS( OB_FIELD_HIDEAMULET,    IDC_HIDEAMULET)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL2,        IDC_MALEWORLD)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL,         IDC_MALEBIPED)
	ADD_OBRECUIFIELDS( OB_FIELD_ICON,          IDC_MALEICON)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL4,        IDC_FEMALEWORLD)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL3,        IDC_FEMALEBIPED)
	ADD_OBRECUIFIELDS( OB_FIELD_ICON2,         IDC_FEMALEICON)
	ADD_OBRECUIFIELDS( OB_FIELD_BIPED,         IDC_BIPEDPARTS)
	ADD_OBRECUIFIELDS( OB_FIELD_USERDATA,      IDC_USERDATA2)
END_OBRECUIFIELDS()
/*===========================================================================
 *		End of UI Field Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObClotDlg Constructor
 *
 *=========================================================================*/
CObClotDlg::CObClotDlg(CWnd* pParent) : CObRecordDialog(CObClotDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObClotDlg)
  //}}AFX_DATA_INIT
 }
/*===========================================================================
 *		End of Class CObClotDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObClotDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObClotDlg::DoDataExchange(CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CObClotDlg)
	DDX_Control(pDX, IDC_USERDATA2, m_UserData);
	DDX_Control(pDX, IDC_FEMALEICON, m_FemaleIcon);
	DDX_Control(pDX, IDC_FEMALEBIPED, m_FemaleBiped);
	DDX_Control(pDX, IDC_FEMALEWORLD, m_FemaleWorld);
	DDX_Control(pDX, IDC_MALEICON, m_MaleIcon);
	DDX_Control(pDX, IDC_MALEWORLD, m_MaleWorld);
	DDX_Control(pDX, IDC_MALEBIPED, m_MaleBiped);
	DDX_Control(pDX, IDC_PLAYABLE, m_Playable);
	DDX_Control(pDX, IDC_HIDERINGS, m_HideRings);
	DDX_Control(pDX, IDC_HIDEAMULET, m_HideAmulet);
	DDX_Control(pDX, IDC_VALUE, m_Value);
	DDX_Control(pDX, IDC_WEIGHT, m_Weight);
	DDX_Control(pDX, IDC_SCRIPT, m_Script);
	DDX_Control(pDX, IDC_ENCHANTMENT, m_Enchantment);
	DDX_Control(pDX, IDC_BIPEDPARTS, m_BipedParts);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_EDITORID, m_EditorID);
	DDX_Control(pDX, IDC_FORMID, m_FormID);
	//}}AFX_DATA_MAP
}
/*===========================================================================
 *		End of Class Method CObClotDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObClotDlg Event - void OnEditEnchantment ();
 *
 *=========================================================================*/
void CObClotDlg::OnEditEnchantment() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetEnchantment());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObClotDlg::OnEditEnchantment()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObClotDlg Event - void OnEditScript ();
 *
 *=========================================================================*/
void CObClotDlg::OnEditScript() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetScript());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObClotDlg::OnEditScript()
 *=========================================================================*/

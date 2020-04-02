/*===========================================================================
 *
 * File:	Obarmodlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObArmoDlg.h"
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
BEGIN_MESSAGE_MAP(CObArmoDlg, CObRecordDialog)
	//{{AFX_MSG_MAP(CObArmoDlg)
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
BEGIN_OBRECUIFIELDS(CObArmoDlg)
	ADD_OBRECUIFIELDS( OB_FIELD_EDITORID,	   IDC_EDITORID)
	ADD_OBRECUIFIELDS( OB_FIELD_FORMID,	   IDC_FORMID)
	ADD_OBRECUIFIELDS( OB_FIELD_ITEMNAME,	   IDC_NAME)
	ADD_OBRECUIFIELDS( OB_FIELD_TYPE,	   IDC_TYPE)
	ADD_OBRECUIFIELDS( OB_FIELD_ARMOR,	   IDC_ARMOR)
	ADD_OBRECUIFIELDS( OB_FIELD_VALUE,	   IDC_VALUE)
	ADD_OBRECUIFIELDS( OB_FIELD_WEIGHT,	   IDC_WEIGHT)
	ADD_OBRECUIFIELDS( OB_FIELD_HEALTH,	   IDC_HEALTH)
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
	ADD_OBRECUIFIELDS( OB_FIELD_USERDATA,      IDC_USERDATA3)
END_OBRECUIFIELDS()
/*===========================================================================
 *		End of UI Field Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObArmoDlg Constructor
 *
 *=========================================================================*/
CObArmoDlg::CObArmoDlg(CWnd* pParent) : CObRecordDialog(CObArmoDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObArmoDlg)
  //}}AFX_DATA_INIT
 }
/*===========================================================================
 *		End of Class CObArmoDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObArmoDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObArmoDlg::DoDataExchange(CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CObArmoDlg)
	DDX_Control(pDX, IDC_USERDATA3, m_UserData);
	DDX_Control(pDX, IDC_FEMALEICON, m_FemaleIcon);
	DDX_Control(pDX, IDC_FEMALEWORLD, m_FemaleWorld);
	DDX_Control(pDX, IDC_MALEICON, m_MaleIcon);
	DDX_Control(pDX, IDC_MALEWORLD, m_MaleWorld);
	DDX_Control(pDX, IDC_MALEBIPED, m_MaleBiped);
	DDX_Control(pDX, IDC_PLAYABLE, m_Playable);
	DDX_Control(pDX, IDC_HIDERINGS, m_HideRings);
	DDX_Control(pDX, IDC_HIDEAMULET, m_HideAmulet);
	DDX_Control(pDX, IDC_HEALTH, m_Health);
	DDX_Control(pDX, IDC_ARMOR, m_Armor);
	DDX_Control(pDX, IDC_VALUE, m_Value);
	DDX_Control(pDX, IDC_WEIGHT, m_Weight);
	DDX_Control(pDX, IDC_SCRIPT, m_Script);
	DDX_Control(pDX, IDC_ENCHANTMENT, m_Enchantment);
	DDX_Control(pDX, IDC_TYPE, m_Type);
	DDX_Control(pDX, IDC_BIPEDPARTS, m_BipedParts);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_EDITORID, m_EditorID);
	DDX_Control(pDX, IDC_FORMID, m_FormID);
	DDX_Control(pDX, IDC_FEMALEBIPED, m_FemaleBiped);
	//}}AFX_DATA_MAP
}
/*===========================================================================
 *		End of Class Method CObArmoDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObArmoDlg Event - void OnEditEnchantment ();
 *
 *=========================================================================*/
void CObArmoDlg::OnEditEnchantment() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetEnchantment());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObArmoDlg::OnEditEnchantment()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObArmoDlg Event - void OnEditScript ();
 *
 *=========================================================================*/
void CObArmoDlg::OnEditScript() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetScript());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObArmoDlg::OnEditScript()
 *=========================================================================*/

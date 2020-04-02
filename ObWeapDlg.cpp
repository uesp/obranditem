/*===========================================================================
 *
 * File:	Obweapdlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 9, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObWeapDlg.h"
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
BEGIN_MESSAGE_MAP(CObWeapDlg, CObRecordDialog)
	//{{AFX_MSG_MAP(CObWeapDlg)
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
BEGIN_OBRECUIFIELDS(CObWeapDlg)
	ADD_OBRECUIFIELDS( OB_FIELD_EDITORID,	   IDC_EDITORID)
	ADD_OBRECUIFIELDS( OB_FIELD_FORMID,	   IDC_FORMID)
	ADD_OBRECUIFIELDS( OB_FIELD_ITEMNAME,	   IDC_NAME)
	ADD_OBRECUIFIELDS( OB_FIELD_TYPE,	   IDC_TYPE)
	ADD_OBRECUIFIELDS( OB_FIELD_REACH,	   IDC_REACH)
	ADD_OBRECUIFIELDS( OB_FIELD_SPEED,	   IDC_SPEED)
	ADD_OBRECUIFIELDS( OB_FIELD_WEIGHT,	   IDC_WEIGHT)
	ADD_OBRECUIFIELDS( OB_FIELD_VALUE,	   IDC_VALUE)
	ADD_OBRECUIFIELDS( OB_FIELD_DAMAGE,	   IDC_DAMAGE)
	ADD_OBRECUIFIELDS( OB_FIELD_HEALTH,	   IDC_HEALTH)
	ADD_OBRECUIFIELDS( OB_FIELD_ENCHANTPOINTS, IDC_ENCHANTCHARGE)
	ADD_OBRECUIFIELDS( OB_FIELD_ENCHANTMENT,   IDC_ENCHANTMENT)
	ADD_OBRECUIFIELDS( OB_FIELD_SCRIPT,	   IDC_SCRIPT)
	ADD_OBRECUIFIELDS( OB_FIELD_IGNORERESIST,  IDC_IGNORERESIST)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL,         IDC_MODEL)
	ADD_OBRECUIFIELDS( OB_FIELD_ICON,          IDC_ICONTEXT)
	ADD_OBRECUIFIELDS( OB_FIELD_USERDATA,      IDC_USERDATA)
END_OBRECUIFIELDS()
/*===========================================================================
 *		End of UI Field Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObWeapDlg Constructor
 *
 *=========================================================================*/
CObWeapDlg::CObWeapDlg(CWnd* pParent) : CObRecordDialog(CObWeapDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObWeapDlg)
  //}}AFX_DATA_INIT
  m_pRecord = NULL;
 }
/*===========================================================================
 *		End of Class CObWeapDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObWeapDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObWeapDlg::DoDataExchange (CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CObWeapDlg)
	DDX_Control(pDX, IDC_USERDATA, m_UserData);
	DDX_Control(pDX, IDC_FORMID, m_FormID);
	DDX_Control(pDX, IDC_IGNORERESIST, m_IgnoreResist);
	DDX_Control(pDX, IDC_ICONTEXT, m_Icon);
	DDX_Control(pDX, IDC_MODEL, m_Model);
	DDX_Control(pDX, IDC_REACH, m_Reach);
	DDX_Control(pDX, IDC_SPEED, m_Speed);
	DDX_Control(pDX, IDC_DAMAGE, m_Damage);
	DDX_Control(pDX, IDC_HEALTH, m_Health);
	DDX_Control(pDX, IDC_VALUE, m_Value);
	DDX_Control(pDX, IDC_WEIGHT, m_Weight);
	DDX_Control(pDX, IDC_SCRIPT, m_Script);
	DDX_Control(pDX, IDC_ENCHANTCHARGE, m_Charge);
	DDX_Control(pDX, IDC_ENCHANTMENT, m_Enchantment);
	DDX_Control(pDX, IDC_TYPE, m_Type);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_EDITORID, m_EditorID);
	//}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObWeapDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObWeapDlg Event - void OnEditEnchantment ();
 *
 *=========================================================================*/
void CObWeapDlg::OnEditEnchantment() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetEnchantment());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObWeapDlg::OnEditEnchantment()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObWeapDlg Event - void OnEditScript ();
 *
 *=========================================================================*/
void CObWeapDlg::OnEditScript() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetScript());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObWeapDlg::OnEditScript()
 *=========================================================================*/

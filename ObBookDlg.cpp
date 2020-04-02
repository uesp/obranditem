/*===========================================================================
 *
 * File:	Obbookdlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObBookDlg.h"
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
BEGIN_MESSAGE_MAP(CObBookDlg, CObRecordDialog)
	//{{AFX_MSG_MAP(CObBookDlg)
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
BEGIN_OBRECUIFIELDS(CObBookDlg)
	ADD_OBRECUIFIELDS( OB_FIELD_EDITORID,	   IDC_EDITORID)
	ADD_OBRECUIFIELDS( OB_FIELD_FORMID,	   IDC_FORMID)
	ADD_OBRECUIFIELDS( OB_FIELD_ITEMNAME,	   IDC_NAME)
	ADD_OBRECUIFIELDS( OB_FIELD_WEIGHT,	   IDC_WEIGHT2)
	ADD_OBRECUIFIELDS( OB_FIELD_VALUE,	   IDC_VALUE2)
	ADD_OBRECUIFIELDS( OB_FIELD_ENCHANTMENT,   IDC_ENCHANTMENT)
	ADD_OBRECUIFIELDS( OB_FIELD_SCRIPT,	   IDC_SCRIPT)
	ADD_OBRECUIFIELDS( OB_FIELD_SCROLL,        IDC_SCROLL)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL,         IDC_MODEL)
	ADD_OBRECUIFIELDS( OB_FIELD_ICON,          IDC_ICONTEXT)
	ADD_OBRECUIFIELDS( OB_FIELD_TEACHES,       IDC_TEACHES)
	ADD_OBRECUIFIELDS( OB_FIELD_TEXT,          IDC_TEXT)
	ADD_OBRECUIFIELDS( OB_FIELD_USERDATA,      IDC_USERDATA)
END_OBRECUIFIELDS()
/*===========================================================================
 *		End of UI Field Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObBookDlg Constructor
 *
 *=========================================================================*/
CObBookDlg::CObBookDlg(CWnd* pParent) : CObRecordDialog(CObBookDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObBookDlg)
  //}}AFX_DATA_INIT
  m_pRecord = NULL;
 }
/*===========================================================================
 *		End of Class CObBookDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObBookDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObBookDlg::DoDataExchange (CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CObBookDlg)
	DDX_Control(pDX, IDC_FORMID, m_FormID);
	DDX_Control(pDX, IDC_TEXT, m_Text);
	DDX_Control(pDX, IDC_EDITORID, m_EditorID);
	DDX_Control(pDX, IDC_USERDATA, m_UserData);
	DDX_Control(pDX, IDC_SCROLL, m_IsScroll);
	DDX_Control(pDX, IDC_ICONTEXT, m_Icon);
	DDX_Control(pDX, IDC_MODEL, m_Model);
	DDX_Control(pDX, IDC_VALUE2, m_Value);
	DDX_Control(pDX, IDC_WEIGHT2, m_Weight);
	DDX_Control(pDX, IDC_SCRIPT, m_Script);
	DDX_Control(pDX, IDC_ENCHANTMENT, m_Enchantment);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_TEACHES,m_Teaches);
	//}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObBookDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObBookDlg Event - BOOL OnInitDialog ();
 *
 *=========================================================================*/
BOOL CObBookDlg::OnInitDialog() {
  CObRecordDialog::OnInitDialog();

  m_ScriptFont.CreatePointFont(9, "FixedSys");

  m_Text.SetFont(&m_ScriptFont);
	
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObBookDlg::OnInitDialog()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObBookDlg Event - void OnEditEnchantment ();
 *
 *=========================================================================*/
void CObBookDlg::OnEditEnchantment() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetEnchantment());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObBookDlg::OnEditEnchantment()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObBookDlg Event - void OnEditScript ();
 *
 *=========================================================================*/
void CObBookDlg::OnEditScript() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetScript());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObBookDlg::OnEditScript()
 *=========================================================================*/


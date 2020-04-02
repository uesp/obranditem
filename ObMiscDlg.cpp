/*===========================================================================
 *
 * File:	Obmiscdlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObMiscDlg.h"
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
BEGIN_MESSAGE_MAP(CObMiscDlg, CObRecordDialog)
	//{{AFX_MSG_MAP(CObMiscDlg)
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
BEGIN_OBRECUIFIELDS(CObMiscDlg)
	ADD_OBRECUIFIELDS( OB_FIELD_EDITORID,	   IDC_EDITORID)
	ADD_OBRECUIFIELDS( OB_FIELD_FORMID,	   IDC_FORMID)
	ADD_OBRECUIFIELDS( OB_FIELD_ITEMNAME,	   IDC_NAME)
	ADD_OBRECUIFIELDS( OB_FIELD_WEIGHT,	   IDC_WEIGHT)
	ADD_OBRECUIFIELDS( OB_FIELD_VALUE,	   IDC_VALUE)
	ADD_OBRECUIFIELDS( OB_FIELD_SCRIPT,	   IDC_SCRIPT)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL,         IDC_MODEL)
	ADD_OBRECUIFIELDS( OB_FIELD_ICON,          IDC_ICONTEXT)
	ADD_OBRECUIFIELDS( OB_FIELD_USERDATA,      IDC_USERDATA)
END_OBRECUIFIELDS()
/*===========================================================================
 *		End of UI Field Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObMiscDlg Constructor
 *
 *=========================================================================*/
CObMiscDlg::CObMiscDlg(CWnd* pParent) : CObRecordDialog(CObMiscDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObMiscDlg)
  //}}AFX_DATA_INIT
  m_pRecord = NULL;
 }
/*===========================================================================
 *		End of Class CObMiscDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObMiscDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObMiscDlg::DoDataExchange (CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CObMiscDlg)
	DDX_Control(pDX, IDC_USERDATA, m_UserData);
	DDX_Control(pDX, IDC_FORMID, m_FormID);
	DDX_Control(pDX, IDC_ICONTEXT, m_Icon);
	DDX_Control(pDX, IDC_MODEL, m_Model);
	DDX_Control(pDX, IDC_VALUE, m_Value);
	DDX_Control(pDX, IDC_WEIGHT, m_Weight);
	DDX_Control(pDX, IDC_SCRIPT, m_Script);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_EDITORID, m_EditorID);
	//}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObMiscDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObMiscDlg Event - void OnEditScript ();
 *
 *=========================================================================*/
void CObMiscDlg::OnEditScript() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetScript());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObMiscDlg::OnEditScript()
 *=========================================================================*/

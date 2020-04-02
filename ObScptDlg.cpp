/*===========================================================================
 *
 * File:	Obscptdlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObScptDlg.h"
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
BEGIN_MESSAGE_MAP(CObScptDlg, CObRecordDialog)
	//{{AFX_MSG_MAP(CObScptDlg)
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
BEGIN_OBRECUIFIELDS(CObScptDlg)
	ADD_OBRECUIFIELDS( OB_FIELD_EDITORID,	   IDC_EDITORID)
	ADD_OBRECUIFIELDS( OB_FIELD_FORMID,	   IDC_FORMID)
	ADD_OBRECUIFIELDS( OB_FIELD_TYPE,          IDC_TYPE)
	ADD_OBRECUIFIELDS( OB_FIELD_SCRIPTTEXT,    IDC_TEXT)
END_OBRECUIFIELDS()
/*===========================================================================
 *		End of UI Field Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObScptDlg Constructor
 *
 *=========================================================================*/
CObScptDlg::CObScptDlg(CWnd* pParent) : CObRecordDialog(CObScptDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObScptDlg)
  //}}AFX_DATA_INIT
  m_pRecord = NULL;
 }
/*===========================================================================
 *		End of Class CObScptDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObScptDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObScptDlg::DoDataExchange (CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CObScptDlg)
	DDX_Control(pDX, IDC_TYPE, m_Type);
	DDX_Control(pDX, IDC_FORMID, m_FormID);
	DDX_Control(pDX, IDC_TEXT, m_Text);
	DDX_Control(pDX, IDC_EDITORID, m_EditorID);
	//}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObScptDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObScptDlg Event - BOOL OnInitDialog ();
 *
 *=========================================================================*/
BOOL CObScptDlg::OnInitDialog() {
  CObRecordDialog::OnInitDialog();

  m_ScriptFont.CreatePointFont(9, "FixedSys");

  m_Text.SetFont(&m_ScriptFont);
	
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObScptDlg::OnInitDialog()
 *=========================================================================*/

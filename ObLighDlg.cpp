/*===========================================================================
 *
 * File:	Oblighdlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 23, 2006
 *
 * Description
 *
 *=========================================================================*/

		/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObLighDlg.h"
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
BEGIN_MESSAGE_MAP(CObLighDlg, CObRecordDialog)
	//{{AFX_MSG_MAP(CObLighDlg)
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
BEGIN_OBRECUIFIELDS(CObLighDlg)
	ADD_OBRECUIFIELDS( OB_FIELD_EDITORID,	IDC_EDITORID)
	ADD_OBRECUIFIELDS( OB_FIELD_FORMID,	IDC_FORMID)
	ADD_OBRECUIFIELDS( OB_FIELD_ITEMNAME,	IDC_NAME)
	ADD_OBRECUIFIELDS( OB_FIELD_WEIGHT,	IDC_WEIGHT)
	ADD_OBRECUIFIELDS( OB_FIELD_VALUE,	IDC_VALUE)
	ADD_OBRECUIFIELDS( OB_FIELD_SCRIPT,	IDC_SCRIPT)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL,	IDC_MODEL)
	ADD_OBRECUIFIELDS( OB_FIELD_ICON,	IDC_ICONTEXT)
	ADD_OBRECUIFIELDS( OB_FIELD_USERDATA,	IDC_USERDATA)
	ADD_OBRECUIFIELDS( OB_FIELD_CARRIED,	IDC_CARRIED)
	ADD_OBRECUIFIELDS( OB_FIELD_DYNAMIC,	IDC_DYNAMIC)
	ADD_OBRECUIFIELDS( OB_FIELD_NEGATIVE,	IDC_NEGATIVE)
	ADD_OBRECUIFIELDS( OB_FIELD_SPOTLIGHT,	IDC_SPOTLIGHT)
	ADD_OBRECUIFIELDS( OB_FIELD_SPOTSHADOW,	IDC_SPOTSHADOW)
	ADD_OBRECUIFIELDS( OB_FIELD_DEFAULTOFF,	IDC_DEFAULTOFF)
	ADD_OBRECUIFIELDS( OB_FIELD_COLOR,	IDC_COLOR)
	ADD_OBRECUIFIELDS( OB_FIELD_FADE,	IDC_FADE)
	ADD_OBRECUIFIELDS( OB_FIELD_TIME,	IDC_TIME)
	ADD_OBRECUIFIELDS( OB_FIELD_RADIUS,	IDC_RADIUS)
	ADD_OBRECUIFIELDS( OB_FIELD_FALLOFF,	IDC_FALLOFF)
	ADD_OBRECUIFIELDS( OB_FIELD_FOV,	IDC_FOV)
	ADD_OBRECUIFIELDS( OB_FIELD_TYPE,	IDC_FLICKER)
	ADD_OBRECUIFIELDS( OB_FIELD_SOUND,	IDC_SOUNDTEXT)
END_OBRECUIFIELDS()
/*===========================================================================
 *		End of UI Field Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLighDlg Constructor
 *
 *=========================================================================*/
CObLighDlg::CObLighDlg(CWnd* pParent) : CObRecordDialog(CObLighDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObLighDlg)
  //}}AFX_DATA_INIT
  m_pRecord = NULL;
 }
/*===========================================================================
 *		End of Class CObLighDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLighDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObLighDlg::DoDataExchange (CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CObLighDlg)
	DDX_Control(pDX, IDC_SPOTSHADOW, m_SpotShadow);
	DDX_Control(pDX, IDC_SPOTLIGHT, m_SpotLight);
	DDX_Control(pDX, IDC_DYNAMIC, m_Dynamic);
	DDX_Control(pDX, IDC_NEGATIVE, m_Negative);
	DDX_Control(pDX, IDC_DEFAULTOFF, m_DefaultOff);
	DDX_Control(pDX, IDC_CARRIED, m_Carried);
	DDX_Control(pDX, IDC_FADE, m_Fade);
	DDX_Control(pDX, IDC_COLOR, m_Color);
	DDX_Control(pDX, IDC_RADIUS, m_Radius);
	DDX_Control(pDX, IDC_FOV, m_FOV);
	DDX_Control(pDX, IDC_FALLOFF, m_Falloff);
	DDX_Control(pDX, IDC_TIME, m_Time);
	DDX_Control(pDX, IDC_FLICKER, m_Flicker);
	DDX_Control(pDX, IDC_SOUNDTEXT, m_Sound);
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
 *		End of Class Method CObLighDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLighDlg Event - void OnEditScript ();
 *
 *=========================================================================*/
void CObLighDlg::OnEditScript() {
  CObBaseRecord* pBaseRecord;
  CObRecord*     pRecord;

  if (m_pRecord == NULL) return;

  pBaseRecord = m_pRecord->GetParentFile()->FindFormID(m_pRecord->GetScript());
  if (pBaseRecord == NULL) return;

  pRecord = ObCastClass(CObRecord, pBaseRecord);
  if (pRecord != NULL) GetOwner()->PostMessage(OBMSG_RECORDLIST_ACTIVATERECORD, (WPARAM) pRecord, 0);
 }
/*===========================================================================
 *		End of Class Event CObLighDlg::OnEditScript()
 *=========================================================================*/

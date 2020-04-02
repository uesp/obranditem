/*===========================================================================
 *
 * File:	Obrioptionsdlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 23, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObriOptionsDlg.h"
#include "obranditemDoc.h"
#include "obranditemview.h"


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
BEGIN_MESSAGE_MAP(CObriOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CObriOptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriOptionsDlg Constructor
 *
 *=========================================================================*/
CObriOptionsDlg::CObriOptionsDlg(CWnd* pParent)	: CDialog(CObriOptionsDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CObriOptionsDlg)
	//}}AFX_DATA_INIT
 }
/*===========================================================================
 *		End of Class CObriOptionsDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriOptionsDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObriOptionsDlg::DoDataExchange (CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CObriOptionsDlg)
	DDX_Control(pDX, IDC_MATERIALCHANCE_TEXT, m_MaterialChance);
	DDX_Control(pDX, IDC_CREATELISTPARENTS_CHECK, m_CreateListParents);
	DDX_Control(pDX, IDC_HIDEPREFIX2_CHECK, m_HidePrefix2);
	DDX_Control(pDX, IDC_OFSTRING_TEXT, m_OfString);
	DDX_Control(pDX, IDC_NAMEDELIMITER_TEXT, m_NameDelimiter);
	DDX_Control(pDX, IDC_CHECKUPLICATES_CHECK, m_CheckDuplicates);
	DDX_Control(pDX, IDC_REMOVEDUPLICATES_CHECK, m_RemoveDuplicates);
	DDX_Control(pDX, IDC_MAXNAMELENGTH_TEXT, m_MaxNameLength);
	DDX_Control(pDX, IDC_REMOVENORMAL_CHECK, m_RemoveNormalCheck);
	DDX_Control(pDX, IDC_RANDOMBELL_TEXT, m_RandomBellText);
	DDX_Control(pDX, IDC_RANDOMBELL_CHECK, m_RandomBellCheck);
	//}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObriOptionsDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriOptionsDlg Method - void GetControlData (void);
 *
 *=========================================================================*/
void CObriOptionsDlg::GetControlData (void) {
  CObriGenerate* pGenerate = &GetDocument()->GetGenerate();
  CString        Buffer;
  int            Value;

  if (m_MaxNameLength.GetModify()) {
    m_MaxNameLength.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_MaxNameLength.GetWindowText(Buffer);
    Value = atoi(Buffer);
    if (Value > 0) pGenerate->GetOptions().MaxNameLength = Value;
   }

  if (m_RemoveNormalCheck.GetCheck() != ((int) pGenerate->GetOptions().RemoveNormalItems)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().RemoveNormalItems = (m_RemoveNormalCheck.GetCheck() != 0);
   }

  if (m_CreateListParents.GetCheck() != ((int) pGenerate->GetOptions().CreateListParents)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().CreateListParents = (m_CreateListParents.GetCheck() != 0);
   }

  if (m_HidePrefix2.GetCheck() != ((int) pGenerate->GetOptions().HidePrefix2)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().HidePrefix2 = (m_HidePrefix2.GetCheck() != 0);
   }

  if (m_RemoveDuplicates.GetCheck() != (pGenerate->GetOptions().RemoveDuplicates != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().RemoveDuplicates = (m_RemoveDuplicates.GetCheck() != 0);
   }

  if (m_CheckDuplicates.GetCheck() != (pGenerate->GetOptions().CheckDuplicates != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().CheckDuplicates = (m_CheckDuplicates.GetCheck() != 0);
   }

  if (m_RandomBellCheck.GetCheck() != (pGenerate->GetOptions().UseBell != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().UseBell = (m_RandomBellCheck.GetCheck() != 0);
   }

  if (m_MaterialChance.GetModify()) {
    m_MaterialChance.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_MaterialChance.GetWindowText(Buffer);
    pGenerate->GetOptions().MaterialChance = (float) atof(Buffer) / 100.0f;
   }

  if (m_RandomBellText.GetModify()) {
    m_RandomBellText.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_RandomBellText.GetWindowText(Buffer);
    pGenerate->GetOptions().RandomBell = (float) atof(Buffer);
   }

  if (m_OfString.GetModify()) {
    m_OfString.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_OfString.GetWindowText(Buffer);
    pGenerate->GetOptions().OfString = Buffer;
   }

  if (m_NameDelimiter.GetModify()) {
    m_OfString.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_NameDelimiter.GetWindowText(Buffer);
    pGenerate->GetOptions().NameDelimiter = Buffer;
   }

 }
/*===========================================================================
 *		End of Class Method CObriOptionsDlg::GetControlData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriOptionsDlg Method - CObranditemDoc* GetDocument (void);
 *
 *=========================================================================*/
CObranditemDoc* CObriOptionsDlg::GetDocument (void) {
  if (m_pParentView != NULL) return m_pParentView->GetDocument();
  return (NULL);
 }
/*===========================================================================
 *		End of Class Method CObriOptionsDlg::GetDocument()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriOptionsDlg Event - BOOL OnInitDialog ();
 *
 *=========================================================================*/
BOOL CObriOptionsDlg::OnInitDialog() {
  CDialog::OnInitDialog();

  m_RandomBellText.SetLimitText(6);
  m_MaxNameLength.SetLimitText(4);
	
  SetControlData();
  return TRUE;
 }
/*===========================================================================
 *		End of Class Event CObriOptionsDlg::OnInitDialog()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriOptionsDlg Event - void OnOK ();
 *
 *=========================================================================*/
void CObriOptionsDlg::OnOK() {
  GetControlData();
  CDialog::OnOK();
 }
/*===========================================================================
 *		End of Class Event CObriOptionsDlg::OnOK()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriOptionsDlg Method - void SetControlData (void);
 *
 *=========================================================================*/
void CObriOptionsDlg::SetControlData (void) {
  CObriGenerate* pGenerate = &GetDocument()->GetGenerate();
  CString        Buffer;

  m_OfString.SetWindowText(pGenerate->GetOptions().OfString);
  m_OfString.SetModify(FALSE);

  m_NameDelimiter.SetWindowText(pGenerate->GetOptions().NameDelimiter);
  m_NameDelimiter.SetModify(FALSE);
  
  Buffer.Format("%g", pGenerate->GetOptions().RandomBell);
  m_RandomBellText.SetWindowText(Buffer);
  m_RandomBellText.SetModify(FALSE);

  Buffer.Format("%g", pGenerate->GetOptions().MaterialChance * 100.0f);
  m_MaterialChance.SetWindowText(Buffer);
  m_MaterialChance.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().MaxNameLength);
  m_MaxNameLength.SetWindowText(Buffer);
  m_MaxNameLength.SetModify(FALSE);

  m_RandomBellCheck.SetCheck(pGenerate->GetOptions().UseBell != 0);
  m_RemoveNormalCheck.SetCheck(pGenerate->GetOptions().RemoveNormalItems != 0);
  m_RemoveDuplicates.SetCheck(pGenerate->GetOptions().RemoveDuplicates != 0);
  m_CheckDuplicates.SetCheck(pGenerate->GetOptions().CheckDuplicates != 0);
  m_CreateListParents.SetCheck(pGenerate->GetOptions().CreateListParents != 0);
  m_HidePrefix2.SetCheck(pGenerate->GetOptions().HidePrefix2 != 0);
 }
/*===========================================================================
 *		End of Class Method CObriOptionsDlg::SetControlData()
 *=========================================================================*/

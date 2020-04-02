/*===========================================================================
 *
 * File:	Oblvlidlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 7, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObLvliDlg.h"


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
 * Special callback functions for list sorting.
 *
 *=========================================================================*/
static int CALLBACK s_ItemCountRecListSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
  OBRL_SORTFUNC_GETPARAMS(lParam1, lParam2, lParamSort);
  CObLvloSubrecord* pItem1 = ObCastClass(CObLvloSubrecord, pCustomData1->pSubrecords[0]);
  CObLvloSubrecord* pItem2 = ObCastClass(CObLvloSubrecord, pCustomData2->pSubrecords[0]);
  
  if (pSortData->Reverse) return -(pItem1->GetCount() - pItem2->GetCount());
  return (pItem1->GetCount() - pItem2->GetCount());
 }


static int CALLBACK s_ItemLevelRecListSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
  OBRL_SORTFUNC_GETPARAMS(lParam1, lParam2, lParamSort);
  CObLvloSubrecord* pItem1 = ObCastClass(CObLvloSubrecord, pCustomData1->pSubrecords[0]);
  CObLvloSubrecord* pItem2 = ObCastClass(CObLvloSubrecord, pCustomData2->pSubrecords[0]);
  
  if (pSortData->Reverse) return -(pItem1->GetLevel() - pItem2->GetLevel());
  return (pItem1->GetLevel() - pItem2->GetLevel());
 }
/*===========================================================================
 *		End of Function CALLBACK s_DefaultRecListSort()
 *=========================================================================*/


/*===========================================================================
 *
 * Begin List Column Definitions
 *
 *=========================================================================*/
static obreclistcolinit_t s_LevelListInit[] = {
	{ OB_FIELD_EDITORID,	100,	LVCFMT_LEFT },
	{ OB_FIELD_FORMID,	5,	LVCFMT_LEFT },
	{ OB_FIELD_LEVEL,	50,	LVCFMT_CENTER, s_ItemLevelRecListSort},
	{ OB_FIELD_ITEMCOUNT,	50,	LVCFMT_CENTER, s_ItemCountRecListSort},
	{ OB_FIELD_ITEMNAME,	210,	LVCFMT_LEFT },
	{ OB_FIELD_NAME,	60,	LVCFMT_CENTER },
	{ OB_FIELD_NONE, 0, 0 }
 };

static obrecfield_t s_LevelListFields[] = {
	{ "Item Name",	OB_FIELD_ITEMNAME,  0, NULL },
	{ "Count",	OB_FIELD_ITEMCOUNT, 0, NULL },
	{ "Level",	OB_FIELD_LEVEL,     0, NULL },
	{ NULL,		OB_FIELD_NONE,      0, NULL }
 };
/*===========================================================================
 *		End of List Column Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObLvliDlg, CDialog)
	//{{AFX_MSG_MAP(CObLvliDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLvliDlg Constructor
 *
 *=========================================================================*/
CObLvliDlg::CObLvliDlg(CWnd* pParent) : CDialog(CObLvliDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObLvliDlg)
  //}}AFX_DATA_INIT

  m_pRecord = NULL;
 }
/*===========================================================================
 *		End of Class CObLvliDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLvliDlg Method - void ClearControlData (void);
 *
 *=========================================================================*/
void CObLvliDlg::ClearControlData (void) { 
  SetTitle(NULL);
 }
/*===========================================================================
 *		End of Class Method CObLvliDlg::ClearControlData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLvliDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObLvliDlg::DoDataExchange(CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CObLvliDlg)
	DDX_Control(pDX, IDC_USERDATA, m_UserData);
  DDX_Control(pDX, IDC_ITEM_LIST, m_ItemList);
  DDX_Control(pDX, IDC_CALCULATEEACH, m_CalculateEach);
  DDX_Control(pDX, IDC_CALCULATEALL, m_CalculateAll);
  DDX_Control(pDX, IDC_CHANCENONE, m_ChanceNone);
  DDX_Control(pDX, IDC_EDITORID, m_EditorID);
  DDX_Control(pDX, IDC_FORMID2, m_FormID);
	//}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObLvliDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLvliDlg Event - BOOL OnInitDialog ();
 *
 *=========================================================================*/
BOOL CObLvliDlg::OnInitDialog() {
  CDialog::OnInitDialog();

	/* Setup the list */
  m_ItemList.DefaultSettings();
  m_ItemList.SetupCustomList(s_LevelListInit, &CObIdRecord::s_FieldMap, s_LevelListFields);
  m_ItemList.SetOwner(GetOwner());
 
  SetControlData();	 
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObLvliDlg::OnInitDialog()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLvliDlg Method - void SetControlData (void);
 *
 *=========================================================================*/
void CObLvliDlg::SetControlData (void) { 
  CObLvloSubrecord* pItem;
  CObBaseRecord*    pBaseRecord;
  CObIdRecord*	    pIdRecord;
  CString           Buffer;
  int               ItemPos;
  int		    ListIndex;
  
  if (m_pRecord == NULL) {
    ClearControlData();
    return;
   }

  SetTitle(m_pRecord->GetEditorID());

  Buffer.Format("%08X",m_pRecord->GetFormID());
  m_FormID.SetWindowText(Buffer);

  Buffer.Format("%d",m_pRecord->GetUserData());
  m_UserData.SetWindowText(Buffer);

  m_EditorID.SetWindowText(m_pRecord->GetEditorID());

  Buffer.Format("%d", m_pRecord->GetChanceNone());
  m_ChanceNone.SetWindowText(Buffer);

  m_CalculateEach.SetCheck(m_pRecord->IsCalculateEachItem());
  m_CalculateAll.SetCheck(m_pRecord->IsCalculateAllLevels());

	/* Add items */
  for (pItem = m_pRecord->GetFirstItem(ItemPos);  pItem != NULL; pItem = m_pRecord->GetNextItem(ItemPos)) {
    pBaseRecord = m_pRecord->GetParentFile()->FindFormID(pItem->GetFormID());

    if (pBaseRecord == NULL) continue;
    pIdRecord = ObCastClass(CObIdRecord, pBaseRecord);
    if (pIdRecord == NULL) continue;

    ListIndex = m_ItemList.AddCustomRecord(pIdRecord, pItem, 0);
    if (ListIndex < 0) continue;

		/* Set custom fields */
    Buffer.Format("%u", (dword) pItem->GetLevel());
    m_ItemList.SetCustomField(ListIndex, OB_FIELD_LEVEL, Buffer);

    Buffer.Format("%u", (dword) pItem->GetCount());
    m_ItemList.SetCustomField(ListIndex, OB_FIELD_ITEMCOUNT, Buffer);
   }

 }
/*===========================================================================
 *		End of Class Method CObLvliDlg::SetControlData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObLvliDlg Method - void SetTitle (pEditorID);
 *
 *=========================================================================*/
void CObLvliDlg::SetTitle (const SSCHAR* pEditorID) {
  CString Buffer;

  Buffer.Format("Leveled Item -- %s", pEditorID == NULL ? "" : pEditorID);
  SetWindowText(Buffer);
 }
/*===========================================================================
 *		End of Class Method CObLvliDlg::SetTitle()
 *=========================================================================*/

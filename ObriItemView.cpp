/*===========================================================================
 *
 * File:	Obriitemview.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 26, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObriItemView.h"
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

  IMPLEMENT_DYNCREATE(CObriItemView, CPropertyPage)
/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin CObriItemView Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObriItemView, CPropertyPage)
	//{{AFX_MSG_MAP(CObriItemView)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREECTRL, OnSelchangedTreectrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LISTMENU_GENERATIONDETAILS, OnListmenuGenerationdetails)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of CObriItemView Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Constructor
 *
 *=========================================================================*/
CObriItemView::CObriItemView() : CPropertyPage(CObriItemView::IDD) {
  //{{AFX_DATA_INIT(CObriItemView)
  //}}AFX_DATA_INIT
  m_IsInitialized  = false;
  m_pParentView    = NULL;
  m_pCurrentFilter = NULL;
 }
/*===========================================================================
 *		End of Class CObriItemView Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Destructor
 *
 *=========================================================================*/
CObriItemView::~CObriItemView() {
 }
/*===========================================================================
 *		End of Class CObriItemView Destructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObriItemView::DoDataExchange(CDataExchange* pDX) {
  CPropertyPage::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CObriItemView)
  DDX_Control(pDX, IDC_TREECTRL, m_TreeCtrl);
  DDX_Control(pDX, IDC_RECORDLIST, m_RecordList);
  //}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObriItemView::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Method - CObranditemDoc* GetDocument (void);
 *
 *=========================================================================*/
CObranditemDoc* CObriItemView::GetDocument (void) {
  if (m_pParentView != NULL) return m_pParentView->GetDocument();
  return (NULL);
 }
/*===========================================================================
 *		End of Class Method CObriItemView::GetDocument()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Event - BOOL OnInitDialog ();
 *
 *=========================================================================*/
BOOL CObriItemView::OnInitDialog() {
  CPropertyPage::OnInitDialog();

  m_TreeCtrl.Initialize("displayfilters.dat");
  	
  m_RecordList.DefaultSettings();
  m_RecordList.SetupList(OB_NAME_ARMO);
  m_RecordList.SetOwner(m_pParentView);

  m_RecordList.SetAutoResizeOffset();
  m_RecordList.SetAutoResize(true);

  m_IsInitialized = true;
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObriItemView::OnInitDialog()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Event - void OnSize (nType, cx, cy);
 *
 *=========================================================================*/
void CObriItemView::OnSize (UINT nType, int cx, int cy) {
  CPropertyPage::OnSize(nType, cx, cy);
	
  if (m_IsInitialized) {
    m_RecordList.AutoResize();
    m_TreeCtrl.SetWindowPos(NULL, 0, 0, 170, cy-7, SWP_NOMOVE | SWP_NOZORDER);
   }
	
 }
/*===========================================================================
 *		End of Class Event CObriItemView::OnSize()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Event - void OnSelchangedTreectrl (pNMHDR, pResult);
 * 
 *=========================================================================*/
void CObriItemView::OnSelchangedTreectrl (NMHDR* pNMHDR, LRESULT* pResult) {
  NM_TREEVIEW*     pNMTreeView = (NM_TREEVIEW*)pNMHDR;
  CObRecordFilter* pFilter;
  DWORD            Value;

  *pResult = 0;
  Value = m_TreeCtrl.GetItemData(pNMTreeView->itemNew.hItem);
  if (Value == 0) return;

  pFilter = (CObRecordFilter *) Value;
  if (pFilter == m_pCurrentFilter) return;

  m_pCurrentFilter = pFilter;
  
  m_RecordList.SetupList(m_pCurrentFilter);
  m_RecordList.AddAllRecords(GetDocument()->GetGenerate().GetEspFile());
 }
/*===========================================================================
 *		End of Class Event CObriItemView::OnSelchangedTreectrl()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Method - void UpdateRecordList (void);
 *
 *=========================================================================*/
void CObriItemView::UpdateRecordList (void) {
  m_RecordList.DeleteAllItems();
  m_RecordList.AddAllRecords(GetDocument()->GetGenerate().GetEspFile());	

  m_TreeCtrl.UpdateFilterCounts(GetDocument()->GetGenerate().GetEspFile());
 }
/*===========================================================================
 *		End of Class Method CObriItemView::UpdateRecordList()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Event - void OnContextMenu (pWnd, Point);
 *
 *=========================================================================*/
void CObriItemView::OnContextMenu(CWnd* pWnd, CPoint Point) {
  CMenu  Menu;
  CMenu* pPopup;
  int    Result;

  Result = Menu.LoadMenu(IDR_LIST_MENU);
  if (!Result) return;

  pPopup = Menu.GetSubMenu(0);
  if (pPopup == NULL) return;

  pPopup->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this);
 }
/*===========================================================================
 *		End of Class Event CObriItemView::OnContextMenu()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemView Event - void OnListmenuGenerationdetails ();
 *
 *=========================================================================*/
void CObriItemView::OnListmenuGenerationdetails() {
  POSITION        SelPos;
  CObRecord*      pRecord;
  CObIdRecord*    pIDRecord;
  obri_itemkey_t* pItemKey;
  CString         Buffer;
  int             ListIndex;
    
  SelPos = m_RecordList.GetFirstSelectedItemPosition();
  if (SelPos == NULL) return;
  ListIndex = m_RecordList.GetNextSelectedItem(SelPos);
  if (ListIndex < 0) return;

  pRecord = m_RecordList.GetRecord(ListIndex);
  if (pRecord == NULL) return;

  pIDRecord = ObCastClass(CObIdRecord, pRecord);
  if (pIDRecord == NULL) return;

  pItemKey = m_pParentView->GetDocument()->GetGenerate().FindItemKey(pIDRecord);

  if (pItemKey == NULL) { 
    MessageBox("No generation data available!", "Generation Details", MB_OK | MB_ICONEXCLAMATION);
   }
  else {
    Buffer.Format("Generation details for item %s:\r\nItem\t= 0x%08X\r\nPrefix1\t= 0x%08X\r\nPrefix2\t= 0x%08X\r\nSuffix\t= 0x%08X\r\nUnique\t= 0x%08X\r\nRatio\t= %.4g\r\nLevel\t= %d", 
			pIDRecord->GetEditorID(),
    			(dword) pItemKey->pItem,   (dword) pItemKey->pPrefix1, (dword) pItemKey->pPrefix2, 
			(dword) pItemKey->pSuffix, (dword) pItemKey->pUnique,  pItemKey->Ratio, pItemKey->Level);
    MessageBox(Buffer, "Generation Details", MB_OK);
   }

 }
/*===========================================================================
 *		End of Class Event CObriItemView::OnListmenuGenerationdetails()
 *=========================================================================*/

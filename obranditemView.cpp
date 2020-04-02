/*===========================================================================
 *
 * File:	obranditemView.CPP
 * Author:	Dave Humphrey (uesp@m0use.net)
 * Created On:	Wednesday, 19 April, 2006
 *
 * Implementation of the CObranditemView class.
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "obranditemDoc.h"
#include "obranditemView.h"
#include "modfile/obespfile.h"
#include "oblvlidlg.h"
#include "obenchdlg.h"
#include "obweapdlg.h"
#include "obarmodlg.h"
#include "obclotdlg.h"
#include "obmiscdlg.h"
#include "obscptdlg.h"
#include "obbookdlg.h"
#include "oblighdlg.h"
#include "obsgstdlg.h"
#include "obrioptionsdlg.h"


	/* Debug definitions */
#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/*===========================================================================
 *
 * Begin Local Definitions
 *
 *=========================================================================*/

	/* Callback used when creating items */
  int s_GenerateCallback (obri_cbdata_t Data);

  IMPLEMENT_DYNCREATE(CObranditemView, CFormView)
/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObranditemView Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObranditemView, CFormView)
	//{{AFX_MSG_MAP(CObranditemView)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_SAVEESP, OnFileSaveesp)
	ON_MESSAGE(OBMSG_RECORDLIST_ACTIVATERECORD, OnActivateRecord)
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_COMMAND(ID_FILE_RUNBATCH, OnFileRunbatch)
	ON_COMMAND(ID_VIEW_TEST, OnViewTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Class CObranditemView Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Constructor
 *
 *=========================================================================*/
CObranditemView::CObranditemView() : CFormView(CObranditemView::IDD) {
  //{{AFX_DATA_INIT(CObranditemView)
  //}}AFX_DATA_INIT
  m_IsInitialized = false;

  m_GenerateView.SetParentView(this);
  m_ItemView.SetParentView(this);

  m_TabFont.CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "MS Sans Serif");

 }
/*===========================================================================
 *		End of Class CObranditemView Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Destructor
 *
 *=========================================================================*/
CObranditemView::~CObranditemView() {
 }
/*===========================================================================
 *		End of Class CObranditemView Destructor
 *=========================================================================*/


/*==========================================================================
 *
 * Class CObranditemView Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObranditemView::DoDataExchange(CDataExchange* pDX) {
  CFormView::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CObranditemView)
  DDX_Control(pDX, IDC_TABCTRL, m_TabCtrl);
  //}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObranditemView::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Method - BOOL PreCreateWindow (cs);
 *
 *=========================================================================*/
BOOL CObranditemView::PreCreateWindow (CREATESTRUCT& cs) {
  return CFormView::PreCreateWindow(cs);
 }
/*===========================================================================
 *		End of Class Method CObranditemView::PreCreateWindow()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Event - void OnInitialUpdate ();
 *
 *=========================================================================*/
void CObranditemView::OnInitialUpdate() {
  if (m_IsInitialized) return;

  CFormView::OnInitialUpdate();
  GetParentFrame()->RecalcLayout();
  ResizeParentToFit();

  m_TabCtrl.SetFont(&m_TabFont);

  m_GenerateView.Create(IDD_GENERATEVIEW, &m_TabCtrl);
  m_ItemView.Create(IDD_ITEMVIEW, &m_TabCtrl);

  m_TabCtrl.AddTab("Generate Items", &m_GenerateView);
  //m_TabCtrl.AddTab("Base Items",     NULL);
  //m_TabCtrl.AddTab("Suffixes",       NULL);
  //m_TabCtrl.AddTab("Prefixes",       NULL);
  //m_TabCtrl.AddTab("Uniques",        NULL);
  //m_TabCtrl.AddTab("Level Lists",    NULL);
  //m_TabCtrl.AddTab("Scripts",        NULL);
  //m_TabCtrl.AddTab("Spells",         NULL);
  m_TabCtrl.AddTab("Item View",      &m_ItemView);
  m_TabCtrl.SetPage(0);

  m_TabCtrl.SetAutoResizeOffset();
  m_TabCtrl.SetAutoResize(true);
   
  m_IsInitialized = true;

  	/* Disable the view scroll bars */
  SetScrollSizes(MM_TEXT, CSize(0, 0) );

  
  GetDocument()->GetGenerate().InitializeSoundRecords();
 }
/*===========================================================================
 *		End of Class Event CObranditemView::OnInitialUpdate()
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class Diagnostics
 *
 *=========================================================================*/
#ifdef _DEBUG

void CObranditemView::AssertValid() const {
  CFormView::AssertValid();
 }

void CObranditemView::Dump(CDumpContext& dc) const  {
  CFormView::Dump(dc);
 }

CObranditemDoc* CObranditemView::GetDocument() {
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CObranditemDoc)));
  return (CObranditemDoc*)m_pDocument;
 }

#endif
/*===========================================================================
 *		End of Class Diagnostics
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Event - void OnActivateRecord (pRecord);
 *
 *=========================================================================*/
void CObranditemView::OnActivateRecord (CObRecord* pRecord) {
  CDialog*   pDialog;
  CObLvliDlg LvliDlg;
  CObEnchDlg EnchDlg;
  CObWeapDlg WeapDlg;
  CObArmoDlg ArmoDlg;
  CObClotDlg ClotDlg;
  CObMiscDlg MiscDlg;
  CObScptDlg ScptDlg;
  CObBookDlg BookDlg;
  CObLighDlg LighDlg;
  CObSgstDlg SgstDlg;
 
  if (pRecord->GetName() == OB_NAME_LVLI) {
    pDialog = &LvliDlg;
    LvliDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_ENCH) {
    pDialog = &EnchDlg;
    EnchDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_LIGH) {
    pDialog = &LighDlg;
    LighDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_SGST) {
    pDialog = &SgstDlg;
    SgstDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_WEAP) {
    pDialog = &WeapDlg;
    WeapDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_ARMO) {
    pDialog = &ArmoDlg;
    ArmoDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_CLOT) {
    pDialog = &ClotDlg;
    ClotDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_MISC) {
    pDialog = &MiscDlg;
    MiscDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_SCPT) {
    pDialog = &ScptDlg;
    ScptDlg.SetRecord(pRecord);
   }
  else if (pRecord->GetName() == OB_NAME_BOOK) {
    pDialog = &BookDlg;
    BookDlg.SetRecord(pRecord);
   }
  else {
    return;
   }

  pDialog->SetOwner(this);
  pDialog->DoModal();
 }
/*===========================================================================
 *		End of Class Event CObranditemView::OnActivateRecord()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Event - void OnActivateRecord (pRecord);
 *
 *=========================================================================*/
LRESULT CObranditemView::OnActivateRecord (WPARAM wParam, LPARAM lParam) {
  CObRecord* pRecord = (CObRecord *) wParam;

  if (pRecord == NULL) return (0);
  OnActivateRecord(pRecord);

  return (0);
 }
/*===========================================================================
 *		End of Class Event CObranditemView::OnActivateRecord()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Event - void OnSize (nType, cx, cy);
 *
 *=========================================================================*/
void CObranditemView::OnSize(UINT nType, int cx, int cy) {
  CFormView::OnSize(nType, cx, cy);

  if (m_IsInitialized) {
    m_TabCtrl.AutoResize();
   }
 }
/*===========================================================================
 *		End of Class Event CObranditemView::OnSize()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Event - void OnFileSaveesp ();
 *
 *=========================================================================*/
void CObranditemView::OnFileSaveesp() {

	/* Ignore if nothing to save */
  if (GetDocument()->GetGenerate().GetNumNewItems() == 0) {
    MessageBox("Nothing to save!", "ObRandItem Error", MB_OK | MB_ICONEXCLAMATION);
    return;
   }

  CFileDialog BrowseFile(FALSE, ".esp", GetDocument()->GetGenerate().GetProjectFile().GetOutputEsp(), 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Plugin Files (*.esp)|*.esp|All Files (*.*)|*.*||", this);
  int         Result;

  BrowseFile.m_ofn.lpstrTitle = "Save Mod File";
  Result = BrowseFile.DoModal();
  if (Result != IDOK) return;

  Result = GetDocument()->GetGenerate().GetEspFile().Save(BrowseFile.GetPathName());
  if (Result) SystemLog.Printf("Saved items to '%s'.", BrowseFile.GetPathName());

 }
/*===========================================================================
 *		End of Class Event CObranditemView::OnFileSaveesp()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Event - void OnUpdate (pSender, lHint, pHint);
 *
 *=========================================================================*/
void CObranditemView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) {

  if (!m_IsInitialized) return;

  if (lHint == OBRI_DOC_HINT_UPDATEALL) {
    m_ItemView.UpdateRecordList();
    m_GenerateView.SetControlData();
   }
  else if (lHint == OBRI_DOC_HINT_GETDATA) {
    m_GenerateView.GetControlData();
   }
 }
/*===========================================================================
 *		End of Class Event CObranditemView::OnUpdate()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Method - void UpdateRecordList (void);
 *
 *=========================================================================*/
void CObranditemView::UpdateRecordList (void) {
  m_ItemView.UpdateRecordList();
 }
/*===========================================================================
 *		End of Class Method CObranditemView::UpdateRecordList()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void ShowObriError (pWnd, pString, ...);
 *
 * Standard function to display an error message box.
 *
 *=========================================================================*/
void ShowObriError (CWnd* pWnd, const SSCHAR* pString, ...) {
  va_list Args;
  CString Buffer;

  va_start(Args, pString);
  Buffer.FormatV(pString, Args);
  va_end(Args);

  CObError* pError = ObErrorHandler->GetLastError();

  if (pError != NULL) {
    Buffer += "\n\r\t";
    Buffer += pError->GetErrorString();
    Buffer += "\n\r\t";
    Buffer += pError->GetErrorMessage();
   }

  pWnd->MessageBox(Buffer, "ObRandItem Error", MB_OK | MB_ICONEXCLAMATION);
 }
/*===========================================================================
 *		End of Function ShowObriError()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - int FindComboListItem (ComboBox, ItemData, Select);
 *
 * Finds an item in a combo list box with the given item data.  Returns the
 * items index or -1 on any error. 
 *
 *=========================================================================*/
int FindComboListItem (CComboBox& ComboBox, const DWORD ItemData, const bool Select) {
  int Index;

  for (Index = 0; Index < ComboBox.GetCount(); Index++) {
    if (ComboBox.GetItemData(Index) == ItemData) {
      if (Select) ComboBox.SetCurSel(Index);
      return (Index);
     }
   }

	/* No item found */
  if (Select) ComboBox.SetCurSel(-1);
  return (-1);
 }
/*===========================================================================
 *		End of Function FindComboListItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - int FindListItem (ListBox, ItemData, Select);
 *
 * Finds an item in a list box with the given item data.  Returns the
 * items index or -1 on any error. 
 *
 *=========================================================================*/
int FindListItem (CListBox& ListBox, const DWORD ItemData, const bool Select) {
  int Index;

  for (Index = 0; Index < ListBox.GetCount(); Index++) {
    if (ListBox.GetItemData(Index) == ItemData) {
      if (Select) ListBox.SetCurSel(Index);
      return (Index);
     }
   }

	/* No item found */
  if (Select) ListBox.SetCurSel(-1);
  return (-1);
 }
/*===========================================================================
 *		End of Function FindListItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - HTREEITEM FindTreeItem (pName, TreeCtrl, Root);
 *
 * Finds an item in a tree control with the given name under the given
 * tree level.  If the input Root is NULL, the root level is used, otherwise
 * the search is conducted using the children of the given root node.  Returns
 * the tree item if found, or NULL otherwise.
 *
 *=========================================================================*/
HTREEITEM FindTreeItem (const TCHAR* pName, CTreeCtrl& TreeCtrl, HTREEITEM Root) {
  HTREEITEM hFind = Root;
  CString   Buffer;

  if (hFind == NULL) 
    hFind = TreeCtrl.GetRootItem();
  else
    hFind = TreeCtrl.GetChildItem(hFind);

  while (hFind != NULL) {
    Buffer = TreeCtrl.GetItemText(hFind);
    if (Buffer.CompareNoCase(pName) == 0) return (hFind);
    hFind = TreeCtrl.GetNextItem(hFind, TVGN_NEXT);
   }

  return (NULL);
 }
/*===========================================================================
 *		End of Function FindTreeItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Event - void OnViewOptions ();
 *
 *=========================================================================*/
void CObranditemView::OnViewOptions() {
  CObriOptionsDlg OptionsDlg;

  OptionsDlg.SetParentView(this);
  OptionsDlg.DoModal();
 }
/*===========================================================================
 *		End of Class Event CObranditemView::OnViewOptions()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemView Event - void OnFileRunbatch ();
 *
 *=========================================================================*/
void CObranditemView::OnFileRunbatch() {
  CObriProjectFile TempProject;
  CFileDialog	   FileDlg(TRUE, ".lst", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "LST Files (*.lst)|*.lst|All Files (*.*)|*.*||", this);
  int		   Result;

	/* Save the current data to a temporary object */
  //m_GenerateView.GetControlData();
  //TempProject = GetDocument()->GetGenerate().GetProjectFile();

  if (!GetDocument()->SaveModified()) return;
  GetDocument()->OnNewDocument();
  
	/* Display the file dialog */
  FileDlg.m_ofn.lpstrTitle = "Select Batch File to Run";
  Result = FileDlg.DoModal();
  if (Result != IDOK) return;

  m_GenerateView.UpdateProgress(0);
  GetDocument()->GetGenerate().SetCallback(s_GenerateCallback, (dword) (void *)&m_GenerateView);
  SetCursor(LoadCursor(NULL, IDC_WAIT));
   
	/* Create all items specified in the batch project files */
  GetDocument()->RunBatch(FileDlg.GetPathName());
  
  SetCursor(LoadCursor(NULL, IDC_ARROW));
  m_GenerateView.UpdateProgress(0);
  
	/* Restore the original project data */
  //GetDocument()->GetGenerate().GetProjectFile() = TempProject;
  //m_GenerateView.SetControlData();
  GetDocument()->SetModifiedFlag(FALSE);
  //GetDocument()->OnNewDocument();
  m_GenerateView.SetControlData();

	/* Update the record list display */
  m_ItemView.UpdateRecordList();
 }
/*===========================================================================
 *		End of Class Event CObranditemView::OnFileRunbatch()
 *=========================================================================*/

void CObranditemView::OnViewTest() {
  CObEspFile InputFile;

  InputFile.Load("h:\\oblivion\\data\\oblivion.esm");

  //GetDocument()->GetGenerate().ExportLevelLists(InputFile, "levellists.csv");
  GetDocument()->GetGenerate().ExportSounds(InputFile, "sounds.csv");
	
}

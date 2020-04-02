/*===========================================================================
 *
 * File:	MainFrm.CPP
 * Author:	Dave Humphrey (uesp@m0use.net)
 * Created On:	Wednesday, July 03, 2002
 *
 * Implementation of the CMainFrame class.
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "MainFrm.h"
#include "obrierrorview.h"


/*===========================================================================
 *
 * Local Definitions
 *
 *=========================================================================*/

	/* Debug defines */
#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

	/* Status bar setup */
  static UINT indicators[] = {
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
   };

  IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Class CMainFrame Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CMainFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_HELP_SIWIKIARTICLE, OnHelpSiwikiarticle)
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Class CMainFrame Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CMainFrame Constructor
 *
 *=========================================================================*/
CMainFrame::CMainFrame() {
 }
/*===========================================================================
 *		End of Class CMainFrame Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CMainFrame Destructor
 *
 *=========================================================================*/
CMainFrame::~CMainFrame() {
 }
/*===========================================================================
 *		End of Class CMainFrame Destructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CMainFrame Event - int OnCreate (lpCreateStruct);
 *
 *=========================================================================*/
int CMainFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {

	/* Call the base class create method first */
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return (-1);

  if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
    TRACE0("Failed to create toolbar\n");
    return (-1);
   }

  if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT))) {
    TRACE0("Failed to create status bar\n");
    return (-1);
   }

  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

  return (0);
 }
/*===========================================================================
 *		End of Class Event CMainFrame::OnCreate()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CMainFrame Method - BOOL PreCreateWindow (cs);
 *
 *=========================================================================*/
BOOL CMainFrame::PreCreateWindow (CREATESTRUCT& cs) {
  if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;

  return TRUE;
 }
/*===========================================================================
 *		End of Class Method CMainFrame::PreCreateWindow()
 *=========================================================================*/


#ifdef _DEBUG
/*===========================================================================
 *
 * Class CMainFrame Diagnostics
 *
 *=========================================================================*/
void CMainFrame::AssertValid() const {
  CFrameWnd::AssertValid();
 }

void CMainFrame::Dump(CDumpContext& dc) const {
  CFrameWnd::Dump(dc);
 }
/*===========================================================================
 *		End of Class CMainFrame Diagnostics
 *=========================================================================*/
#endif


/*===========================================================================
 *
 * Class CMainFrame Event - void OnHelpSiwikiarticle ();
 *
 *=========================================================================*/
void CMainFrame::OnHelpSiwikiarticle() {

  ShellExecute(m_hWnd, "open", "http://www.uesp.net/wiki/Oblivion:Split_Infinity", NULL, NULL, SW_SHOWNORMAL );
 }
/*===========================================================================
 *		End of Class Event CMainFrame::OnHelpSiwikiarticle()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CMainFrame Event - BOOL OnCreateClient (lpcs, pContext);
 *
 *=========================================================================*/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) {

	/* Create the splitter window */
  m_Splitter.CreateStatic(this, 2, 1);
  m_Splitter.SetScrollStyle(WS_HSCROLL);

	/* Add the first splitter pane - the default view in row 0 */
  if (!m_Splitter.CreateView(0, 0, pContext->m_pNewViewClass, CSize(0, 100), pContext)) {
    TRACE0("Failed to create first pane\n");
    return FALSE;
    }

	/* Add the second splitter pane - an error list view in row 1 */
  if (!m_Splitter.CreateView(1, 0, RUNTIME_CLASS(CObriErrorView), CSize(0, 50), pContext)) {
    TRACE0("Failed to create second pane\n");
    return FALSE;
   }

  m_Splitter.SetRowInfo(0, 440, 0);
  m_Splitter.SetRowInfo(1, 75, 0);
  
  m_Splitter.RecalcLayout();

  SetActiveView((CView *)m_Splitter.GetPane(0,0));	
	
  //return CFrameWnd::OnCreateClient(lpcs, pContext);
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CMainFrame::OnCreateClient()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CMainFrame Event - void OnSize (nType, cx, cy);
 *
 *=========================================================================*/
void CMainFrame::OnSize(UINT nType, int cx, int cy) {
  CFrameWnd::OnSize(nType, cx, cy);
 }
/*===========================================================================
 *		End of Class Event CMainFrame::OnSize()
 *=========================================================================*/

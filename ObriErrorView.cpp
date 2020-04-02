/*===========================================================================
 *
 * File:	Obrierrorview.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 28, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObriErrorView.h"
#include "common/oblogfile.h"


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

  IMPLEMENT_DYNCREATE(CObriErrorView, CFormView)
/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin CObriErrorView Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObriErrorView, CFormView)
	//{{AFX_MSG_MAP(CObriErrorView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of CObriErrorView Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void l_ErrorViewCallback (pError, UserData);
 *
 *=========================================================================*/
void l_ErrorViewCallback (CObError* pError, const dword UserData) {
  CObriErrorView* pErrorView = (CObriErrorView *) UserData;

  pErrorView->AddError(pError);
 }


void l_ErrorViewLogCallback (const SSCHAR* pString, va_list Args, const dword UserData) {
  CObriErrorView* pErrorView = (CObriErrorView *) UserData;

  pErrorView->AddLog(pString, Args);
 }
/*===========================================================================
 *		End of Function l_ErrorViewCallback()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriErrorView Constructor
 *
 *=========================================================================*/
CObriErrorView::CObriErrorView() : CFormView(CObriErrorView::IDD) {
  //{{AFX_DATA_INIT(CObriErrorView)
  //}}AFX_DATA_INIT

  ObErrorHandler->AddCallback(l_ErrorViewCallback, (dword) (void *) this);
  SystemLog.SetCallback(l_ErrorViewLogCallback, (dword) (void *) this);
 }
/*===========================================================================
 *		End of Class CObriErrorView Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriErrorView Destructor
 *
 *=========================================================================*/
CObriErrorView::~CObriErrorView() {
  ObErrorHandler->RemoveCallback(l_ErrorViewCallback);
  SystemLog.SetCallback(NULL, 0);
 }
/*===========================================================================
 *		End of Class CObriErrorView Destructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriErrorView Method - void AddError (pError);
 *
 * Adds the given error to the displayed list.
 *
 *=========================================================================*/
void CObriErrorView::AddError (CObError* pError) {
  CString Buffer;
  int     Length;

  if (!IsWindow(m_ErrorText.m_hWnd)) return;

  Length = m_ErrorText.GetWindowTextLength();

  m_ErrorText.SetSel(Length, Length, FALSE);

  Buffer.Format("\r\nError: %s", pError->GetErrorMessage());
  m_ErrorText.ReplaceSel(Buffer);

  if (Length > 20000) {
    m_ErrorText.SetSel(0, 5000, TRUE);
    m_ErrorText.ReplaceSel("");
   }
 }
/*===========================================================================
 *		End of Class Method CObriErrorView::AddError()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriErrorView Method - void AddLog (pString, Args);
 *
 * Adds the given log message to the displayed list.
 *
 *=========================================================================*/
void CObriErrorView::AddLog (const SSCHAR* pString, va_list Args) {
  CString Buffer;
  int     Length;

  if (!IsWindow(m_ErrorText.m_hWnd)) return;

  Length = m_ErrorText.GetWindowTextLength();

  m_ErrorText.SetSel(Length, Length, FALSE);

  Buffer.FormatV(pString, Args);
  Buffer = "\r\n" + Buffer;
  m_ErrorText.ReplaceSel(Buffer);

  if (Length > 20000) {
    m_ErrorText.SetSel(0, 5000, TRUE);
    m_ErrorText.ReplaceSel("");
   }
 }
/*===========================================================================
 *		End of Class Method CObriErrorView::AddLog()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriErrorView Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObriErrorView::DoDataExchange (CDataExchange* pDX) {
  CFormView::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CObriErrorView)
  DDX_Control(pDX, IDC_ERRORTEXT, m_ErrorText);
  //}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObriErrorView::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Debug Methods
 *
 *=========================================================================*/
#ifdef _DEBUG

void CObriErrorView::AssertValid() const {
  CFormView::AssertValid();
 }

void CObriErrorView::Dump(CDumpContext& dc) const {
  CFormView::Dump(dc);
 }

#endif
/*===========================================================================
 *		End of Debug Methods
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriErrorView Event - void OnSize (nType, cx, cy);
 *
 *=========================================================================*/
void CObriErrorView::OnSize(UINT nType, int cx, int cy) {
  CFormView::OnSize(nType, cx, cy);

  if (IsWindow(m_ErrorText.m_hWnd)) {
    m_ErrorText.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
   }
	
 }
/*===========================================================================
 *		End of Class Event CObriErrorView::OnSize()
 *=========================================================================*/

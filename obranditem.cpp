/*===========================================================================
 *
 * File:	obranditem.CPP
 * Author:	Dave Humphrey (uesp@m0use.net)
 * Created On:	Wednesday, 19 April, 2006
 *
 * Defines the class behaviors for the application.
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "MainFrm.h"
#include "obranditemDoc.h"
#include "obranditemView.h"
#include "modfile/obespfile.h"


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

	/* Main application object */
  CObranditemApp theApp;


/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin CObranditemApp Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObranditemApp, CWinApp)
  //{{AFX_MSG_MAP(CObranditemApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  //}}AFX_MSG_MAP

	/* Standard file based document commands */
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)

END_MESSAGE_MAP()
/*===========================================================================
 *		End of CObranditemApp Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemApp Constructor
 *
 *=========================================================================*/
CObranditemApp::CObranditemApp() {
 }
/*===========================================================================
 *		End of Class CObranditemApp Constructor
 *=========================================================================*/



/*===========================================================================
 *
 * Class CObranditemApp Method - BOOL InitInstance ();
 *
 * Description
 *
 *=========================================================================*/
BOOL CObranditemApp::InitInstance() {
  SystemLog.Open("ObRandItem.log");

  AfxEnableControlContainer();


#ifdef _AFXDLL
  Enable3dControls();
#else
  Enable3dControlsStatic();
#endif

	/* Change the registry key under which our settings are stored. */
  SetRegistryKey(_T("UESP"));

	/* Load standard INI file options (including MRU) */
  LoadStdProfileSettings(); 

	/* Register document templates */
  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CObranditemDoc),
		RUNTIME_CLASS(CMainFrame),       /* Main SDI frame window */
		RUNTIME_CLASS(CObranditemView));
  AddDocTemplate(pDocTemplate);

	/* Enable DDE Execute open */
  EnableShellOpen();
  RegisterShellFileTypes(TRUE);

	/* Parse command line for standard shell commands, DDE, file open */
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

	/* Dispatch commands specified on the command line */
  if (!ProcessShellCommand(cmdInfo)) return FALSE;

  //m_pMainWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 700, 600,  SWP_FRAMECHANGED | SWP_NOSIZE);
  //m_pMainWnd->ShowWindow(SW_SHOW);
  //m_pMainWnd->UpdateWindow();
  m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
  //m_pMainWnd->ShowWindow(SW_SHOWNOACTIVATE );

	/* Enable drag/drop open */
  m_pMainWnd->DragAcceptFiles();

  return TRUE;
 }
/*===========================================================================
 *		End of Class Method CObranditemApp::InitInstance()
 *=========================================================================*/




/*===========================================================================
 *
 * Begin Class CAboutDlg Definition
 *
 *=========================================================================*/
class CAboutDlg : public CDialog {

public:
  CAboutDlg();

	/* Dialog Data */
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

	/* Implementation */
protected:
  //{{AFX_MSG(CAboutDlg)
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CAboutDlg Definition
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CAboutDlg Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Class CAboutDlg Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CAboutDlg Constructor
 *
 *=========================================================================*/
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
 }
/*===========================================================================
 *		End of Class CAboutDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CAboutDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CAboutDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemApp Event - void OnAppAbout ();
 *
 * Application command to run the about dialog.
 *
 *=========================================================================*/
void CObranditemApp::OnAppAbout() {
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
 }

/*===========================================================================
 *		End of Class Event CObranditemApp::OnAppAbout()
 *=========================================================================*/

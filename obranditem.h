/*===========================================================================
 *
 * File:	obranditem.H
 * Author:	Dave Humphrey (uesp@m0use.net)
 * Created On:	Wednesday, 19 April, 2006
 *
 * Main header file for the OBRANDITEM application.
 *
 *=========================================================================*/
#ifndef __OBRANDITEM_H
#define __OBRANDITEM_H


/*===========================================================================
 *
 * Begin Required Include Files
 *
 *=========================================================================*/
#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

  #include "resource.h"
/*===========================================================================
 *		End of Required Include Files
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObranditemApp Definition
 *
 *=========================================================================*/
class CObranditemApp : public CWinApp {

  /*---------- Begin Protected Class Members -----------------------*/
protected:



  /*--------- Begin Public Class Methods ---------------------------*/
public:

	/* Class Constructor */
  CObranditemApp();


	/* ClassWizard generated virtual function overrides */
//{{AFX_VIRTUAL(CObranditemApp)
public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL


  //{{AFX_MSG(CObranditemApp)
public:
  afx_msg void OnAppAbout();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
 };
/*===========================================================================
 *		End of Class CObranditemApp Definition
 *=========================================================================*/




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//}}AFX_INSERT_LOCATION

#endif
/*===========================================================================
 *		End of File Root.H
 *=========================================================================*/

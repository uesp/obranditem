/*===========================================================================
 *
 * File:	MainFrm.H
 * Author:	Dave Humphrey (uesp@m0use.net)
 * Created On:	Wednesday, 19 April, 2006
 *
 * Defines the interface of the CMainFrame class.
 *
 *=========================================================================*/
#ifndef __MainFrm_H
#define __MainFrm_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
   #include "obsplitterwnd.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/
  	

/*===========================================================================
 *
 * Class CMainFrame Definition
 *
 *=========================================================================*/
class CMainFrame : public CFrameWnd {
	
protected:
  CMainFrame();
  DECLARE_DYNCREATE(CMainFrame)

  /*---------- Begin Protected Class Members ----------------------*/
protected:
  CStatusBar		m_wndStatusBar;
  CToolBar		m_wndToolBar;
  CObSplitterWnd	m_Splitter;


  /*---------- Begin Public Class Methods ------------------------*/
public:

	/* Destructor */
  virtual ~CMainFrame();

	/* Access the split error view */
  CWnd* GetErrorView (void) { return m_Splitter.GetPane(1, 0); }


	/* Diagnostics */
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif


	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CMainFrame)
public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  //}}AFX_VIRTUAL

	/* Generated message map functions */
protected:
  //{{AFX_MSG(CMainFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnHelpSiwikiarticle();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CMainFrame Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File MainFrm.H
 *=========================================================================*/

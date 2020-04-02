/*===========================================================================
 *
 * File:	obranditemView.H
 * Author:	Dave Humphrey (uesp@m0use.net)
 * Created On:	Wednesday, 19 April, 2006
 *
 * Interface of the CObranditemView class.
 *
 *=========================================================================*/
#ifndef __obranditemView_H
#define __obranditemView_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "obrigenerateview.h"
  #include "obriitemview.h"
  #include "modfile/obespfile.h"
  #include "tabctrlsheet.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/
	

/*===========================================================================
 *
 * Begin Class CObranditemView Definition
 *
 *=========================================================================*/
class CObranditemView : public CFormView {

  /*---------- Begin Protected Class Members ----------------------*/
protected:
  bool			m_IsInitialized;

  CObriGenerateView	m_GenerateView;
  CObriItemView		m_ItemView;

  CFont			m_TabFont;


  /*---------- Begin Protected Class Methods ----------------------*/
protected: 

	/* Create from serialization only */
  CObranditemView();
  DECLARE_DYNCREATE(CObranditemView)


public:
  //{{AFX_DATA(CObranditemView)
  enum { IDD = IDD_OBRANDITEM_FORM };
  CTabCtrlSheet	m_TabCtrl;
  //}}AFX_DATA



  /*---------- Begin Public Class Methods --------------------------*/
public:

	/* Class destructor */
  virtual ~CObranditemView();

	/* Public events */
  void OnActivateRecord (CObRecord* pRecord);

	/* Update display */
  void UpdateRecordList (void);

	/* Diagnostics */
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  CObranditemDoc* GetDocument();

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObranditemView)
public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual void OnInitialUpdate();
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL


	/* Generated message map functions */
protected:
  //{{AFX_MSG(CObranditemView)
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnFileSaveesp();
  afx_msg LRESULT OnActivateRecord (WPARAM wParam, LPARAM lParam);
  afx_msg void OnViewOptions();
  afx_msg void OnFileRunbatch();
  afx_msg void OnViewTest();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObranditemView Definition
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Inline Class Methods
 *
 *=========================================================================*/
#ifndef _DEBUG
  inline CObranditemDoc* CObranditemView::GetDocument() {
    return (CObranditemDoc*)m_pDocument; }
#endif
/*===========================================================================
 *		End of Inline Class Methods
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Function Definitions
 *
 *=========================================================================*/

  int       FindComboListItem (CComboBox& ComboBox, const DWORD ItemData, const bool Select);
  int       FindListItem      (CListBox& ListBox, const DWORD ItemData, const bool Select);
  HTREEITEM FindTreeItem      (const TCHAR* pName, CTreeCtrl& TreeCtrl, HTREEITEM Root);

  void ShowObriError (CWnd* pWnd, const SSCHAR* pString, ...);

/*===========================================================================
 *		End of Function Definitions
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File View.H
 *=========================================================================*/

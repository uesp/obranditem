/*===========================================================================
 *
 * File:	Obriitemview.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 26, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBRIITEMVIEW_H
#define __OBRIITEMVIEW_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "windows/obrecordlistctrl.h"
  #include "windows/obrecordtreectrl.h"
  #include "windows/obrecordvirtuallistctrl.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Forward class definitions */
  class CObranditemView;
  class CObranditemDoc;

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriItemView Definition
 *
 *=========================================================================*/
class CObriItemView : public CPropertyPage {
  DECLARE_DYNCREATE(CObriItemView)

protected:
  CObranditemView*	m_pParentView;

  obrectype_t		m_CurrentName;
  CObRecordFilter*	m_pCurrentFilter;
  bool			m_IsInitialized;


public:

	/* Construction/Destruction */
  CObriItemView();
  ~CObriItemView();

  	/* Get document object */
  CObranditemDoc* GetDocument (void);

	/* Set class members */
  void SetParentView (CObranditemView* pParent) { m_pParentView = pParent; }

	/* Update display */
  void UpdateRecordList (void);

	/* Dialog Data */
  //{{AFX_DATA(CObriItemView)
  enum { IDD = IDD_ITEMVIEW };
  CObRecordTreeCtrl		m_TreeCtrl;
  CObRecordVirtualListCtrl	m_RecordList;
  //}}AFX_DATA

	/* ClassWizard generate virtual function overrides */
  //{{AFX_VIRTUAL(CObriItemView)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObriItemView)
  virtual BOOL OnInitDialog();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnSelchangedTreectrl(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnListmenuGenerationdetails();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP();

 };
/*===========================================================================
 *		End of Class CObriItemView Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obriitemview.H
 *=========================================================================*/

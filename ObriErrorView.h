/*===========================================================================
 *
 * File:	Obrierrorview.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 28, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBRIERRORVIEW_H
#define __OBRIERRORVIEW_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
#ifndef __AFXEXT_H__
  #include <afxext.h>
#endif
  #include "common/oberrorhandler.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriErrorView Definition
 *
 *=========================================================================*/
class CObriErrorView : public CFormView {

protected:

	/* Protected constructor used by dynamic creation */
  CObriErrorView();
  virtual ~CObriErrorView();
  DECLARE_DYNCREATE(CObriErrorView)


public:

	/* Form Data */
  //{{AFX_DATA(CObriErrorView)
  enum { IDD = IDD_ERRORVIEW };
  CEdit		m_ErrorText;
  //}}AFX_DATA

	/* Adds a message to the view */
  void AddError (CObError* pError);
  void AddLog   (const SSCHAR* pString, va_list Args);

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObriErrorView)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL


protected:
	
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

	/* Generated message map functions */
  //{{AFX_MSG(CObriErrorView)
  afx_msg void OnSize(UINT nType, int cx, int cy);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

};
/*===========================================================================
 *		End of Class CObriErrorView Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obrierrorview.H
 *=========================================================================*/

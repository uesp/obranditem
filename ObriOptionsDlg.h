/*===========================================================================
 *
 * File:	Obrioptionsdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 23, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBRIOPTIONSDLG_H
#define __OBRIOPTIONSDLG_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/

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
 * Begin Class CObriOptionsDlg
 *
 *=========================================================================*/
class CObriOptionsDlg : public CDialog {

protected:
  CObranditemView*	m_pParentView;
  bool			m_IsInitialized;


public:

	/* Construction */
  CObriOptionsDlg(CWnd* pParent = NULL);

	/* Get document object */
  CObranditemDoc* GetDocument (void);

	/* Set class members */
  void SetParentView (CObranditemView* pParent) { m_pParentView = pParent; }

	/* Update the view from the current data */
  void GetControlData   (void);
  void SetControlData   (void);

	/* Dialog Data */
  //{{AFX_DATA(CObriOptionsDlg)
	enum { IDD = IDD_OPTIONS_DLG };
	CEdit	m_MaterialChance;
	CButton	m_CreateListParents;
	CButton	m_HidePrefix2;
	CEdit	m_OfString;
	CEdit	m_NameDelimiter;
	CButton	m_CheckDuplicates;
	CButton	m_RemoveDuplicates;
	CEdit	m_MaxNameLength;
	CButton	m_RemoveNormalCheck;
	CEdit	m_RandomBellText;
	CButton	m_RandomBellCheck;
	//}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObriOptionsDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObriOptionsDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObriOptionsDlg
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obrioptionsdlg.H
 *=========================================================================*/

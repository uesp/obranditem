/*===========================================================================
 *
 * File:	Obmiscdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBMISCDLG_H
#define __OBMISCDLG_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "windows/obrecdialog.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObMiscDlg Definition
 *
 *=========================================================================*/
class CObMiscDlg : public CObRecordDialog {
  DECLARE_OBRECUIFIELDS();

protected:
  CObMiscRecord*	m_pRecord;


public:

	/* Construction */
  CObMiscDlg (CWnd* pParent = NULL);

	/* Access the dialog record object */
  virtual CObRecord* GetRecord (void) { return (m_pRecord); }

  	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return ("Misc"); }

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObMiscRecord, pRecord); }

	/* Dialog Data */
  //{{AFX_DATA(CObMiscDlg)
  enum { IDD = IDD_MISC_DLG };
  CEdit		m_UserData;
  CEdit		m_FormID;
  CEdit		m_Icon;
  CEdit		m_Model;
  CEdit		m_Value;
  CEdit		m_Weight;
  CEdit		m_Script;
  CEdit		m_Name;
  CEdit		m_EditorID;
	//}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObMiscDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObMiscDlg)
  afx_msg void OnEditScript();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObMiscDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION

#endif
/*===========================================================================
 *		End of File Obmiscdlg.H
 *=========================================================================*/

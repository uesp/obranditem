/*===========================================================================
 *
 * File:	Obscptdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBSCPTDLG_H
#define __OBSCPTDLG_H


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
 * Begin Class CObScptDlg Definition
 *
 *=========================================================================*/
class CObScptDlg : public CObRecordDialog {
  DECLARE_OBRECUIFIELDS();

protected:
  CObScptRecord*	m_pRecord;
  CFont			m_ScriptFont;


public:

	/* Construction */
  CObScptDlg (CWnd* pParent = NULL);

	/* Access the dialog record object */
  virtual CObRecord* GetRecord (void) { return (m_pRecord); }

  	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return ("Script"); }

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObScptRecord, pRecord); }

	/* Dialog Data */
  //{{AFX_DATA(CObScptDlg)
  enum { IDD = IDD_SCPT_DLG };
  CEdit		m_FormID;
  CEdit		m_Type;
  CEdit		m_EditorID;
  CEdit		m_Text;
  //}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObScptDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObScptDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObScptDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obscptdlg.H
 *=========================================================================*/

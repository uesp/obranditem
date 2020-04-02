/*===========================================================================
 *
 * File:	Obbookdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBBOOKDLG_H
#define __OBBOOKDLG_H


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
 * Begin Class CObBookDlg Definition
 *
 *=========================================================================*/
class CObBookDlg : public CObRecordDialog {
  DECLARE_OBRECUIFIELDS();

protected:
  CObBookRecord*	m_pRecord;
  CFont			m_ScriptFont;


public:

	/* Construction */
  CObBookDlg (CWnd* pParent = NULL);

	/* Access the dialog record object */
  virtual CObRecord* GetRecord (void) { return (m_pRecord); }

  	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return ("Book"); }

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObBookRecord, pRecord); }

	/* Dialog Data */
  //{{AFX_DATA(CObBookDlg)
  enum { IDD = IDD_BOOK_DLG };
  CEdit		m_FormID;
  CEdit		m_Type;
  CEdit		m_EditorID;
  CEdit		m_Name;
  CEdit		m_Enchantment;
  CEdit		m_Script;
  CEdit		m_Model;
  CEdit		m_Icon;
  CEdit		m_UserData;
  CEdit		m_Weight;
  CEdit		m_Value;
  CEdit		m_Teaches;
  CButton	m_IsScroll;
  CEdit		m_Text;
  //}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObBookDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObBookDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditEnchantment();
	afx_msg void OnEditScript();
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObBookDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obbookdlg.H
 *=========================================================================*/

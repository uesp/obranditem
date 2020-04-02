/*===========================================================================
 *
 * File:	Obsgstdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 23, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBSGSTDLG_H
#define __OBSGSTDLG_H



/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "windows/obrecdialog.h"
  #include "windows/obrecordlistctrl.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObSgstDlg Definition
 *
 *=========================================================================*/
class CObSgstDlg : public CObRecordDialog {
  DECLARE_OBRECUIFIELDS();

protected:
  CObSgstRecord*	m_pRecord;


public:

	/* Construction */
  CObSgstDlg(CWnd* pParent = NULL);

	/* Access the dialog record object */
  virtual CObRecord* GetRecord (void) { return (m_pRecord); }

  	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return ("Sigil Stone"); }

	/* Set and get control data */
  void ClearControlData (void);
  void SetControlData   (void);
  void SetEffectList    (void);

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObSgstRecord, pRecord); }

	/* Dialog Data */
  //{{AFX_DATA(CObSgstDlg)
	enum { IDD = IDD_SGST_DLG };
	CEdit	m_Icon;
	CEdit	m_Model;
	CEdit	m_Value;
	CEdit	m_Weight;
	CEdit	m_Uses;
	CEdit	m_Script;
	CEdit	m_ItemName;
  CEdit			m_FormID;
  CObRecordListCtrl	m_EffectList;
  CEdit			m_EditorID;
	//}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObSgstDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObSgstDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnEditScript();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObSgstDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obsgstdlg.H
 *=========================================================================*/

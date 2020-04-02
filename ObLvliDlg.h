/*===========================================================================
 *
 * File:	Oblvlidlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 7, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBLVLIDLG_H
#define __OBLVLIDLG_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "modfile/records/oblvlirecord.h"
  #include "windows/obrecordlistctrl.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CobLvliDlg Definition
 *
 *=========================================================================*/
class CObLvliDlg : public CDialog {

protected:
  CObLvliRecord*	m_pRecord;


public:
	/* Construction */
  CObLvliDlg(CWnd* pParent = NULL);

	/* Set and get control data */
  void ClearControlData (void);
  void SetControlData   (void);
  void SetTitle         (const SSCHAR* pEditorID);

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObLvliRecord, pRecord); }

	/* Dialog Data */
  //{{AFX_DATA(CObLvliDlg)
	enum { IDD = IDD_LVLI_DLG };
	CEdit	m_UserData;
  CObRecordListCtrl	m_ItemList;
  CButton		m_CalculateEach;
  CButton		m_CalculateAll;
  CEdit			m_ChanceNone;
  CEdit			m_EditorID;
  CEdit			m_FormID;
	//}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObLvliDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObLvliDlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CobLvliDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Oblvlidlg.H
 *=========================================================================*/

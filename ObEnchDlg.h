/*===========================================================================
 *
 * File:	Obenchdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 8, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBENCHDLG_H
#define __OBENCHDLG_H


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
 * Begin Class CObEnchDlg Definition
 *
 *=========================================================================*/
class CObEnchDlg : public CObRecordDialog {
  DECLARE_OBRECUIFIELDS();

protected:
  CObEnchRecord*	m_pRecord;


public:

	/* Construction */
  CObEnchDlg(CWnd* pParent = NULL);

	/* Access the dialog record object */
  virtual CObRecord* GetRecord (void) { return (m_pRecord); }

  	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return ("Enchantment"); }

	/* Set and get control data */
  void ClearControlData (void);
  void SetControlData   (void);
  void SetEffectList    (void);

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObEnchRecord, pRecord); }

	/* Dialog Data */
  //{{AFX_DATA(CObEnchDlg)
  enum { IDD = IDD_ENCH_DLG };
  CEdit			m_FormID;
  CButton		m_AutoCalc;
  CObRecordListCtrl	m_EffectList;
  CEdit			m_Cost;
  CEdit			m_Charge;
  CEdit			m_Type;
  CEdit			m_EditorID;
	//}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObEnchDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObEnchDlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObEnchDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obenchdlg.H
 *=========================================================================*/

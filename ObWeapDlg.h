/*===========================================================================
 *
 * File:	Obweapdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 9, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBWEAPDLG_H
#define __OBWEAPDLG_H


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
 * Begin Class CObWeapDlg Definition
 *
 *=========================================================================*/
class CObWeapDlg : public CObRecordDialog {
  DECLARE_OBRECUIFIELDS();

protected:
  CObWeapRecord*	m_pRecord;


public:

	/* Construction */
  CObWeapDlg (CWnd* pParent = NULL);

	/* Access the dialog record object */
  virtual CObRecord* GetRecord (void) { return (m_pRecord); }

  	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return ("Weapon"); }

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObWeapRecord, pRecord); }

	/* Dialog Data */
  //{{AFX_DATA(CObWeapDlg)
	enum { IDD = IDD_WEAP_DLG };
	CEdit	m_UserData;
  CEdit		m_FormID;
  CButton	m_IgnoreResist;
  CEdit		m_Icon;
  CEdit		m_Model;
  CEdit		m_Reach;
  CEdit		m_Speed;
  CEdit		m_Damage;
  CEdit		m_Health;
  CEdit		m_Value;
  CEdit		m_Weight;
  CEdit		m_Script;
  CEdit		m_Charge;
  CEdit		m_Enchantment;
  CEdit		m_Type;
  CEdit		m_Name;
  CEdit		m_EditorID;
	//}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObWeapDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObWeapDlg)
  afx_msg void OnEditEnchantment();
  afx_msg void OnEditScript();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObWeapDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obweapdlg.H
 *=========================================================================*/

/*===========================================================================
 *
 * File:	Obclotdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 11, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBCLOTDLG_H
#define __OBCLOTDLG_H


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
 * Begin Class CObClotDlg Definition
 *
 *=========================================================================*/
class CObClotDlg : public CObRecordDialog {
 DECLARE_OBRECUIFIELDS();

protected:
  CObClotRecord*	m_pRecord;


public:

	/* Construction */
  CObClotDlg(CWnd* pParent = NULL);

	/* Access the dialog record object */
  virtual CObRecord* GetRecord (void) { return (m_pRecord); }

  	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return ("Clothing"); }

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObClotRecord, pRecord); }


	/* Dialog Data */
  //{{AFX_DATA(CObClotDlg)
	enum { IDD = IDD_CLOT_DLG };
	CEdit	m_UserData;
  CEdit		m_FemaleIcon;
  CEdit		m_FemaleBiped;
  CEdit		m_FemaleWorld;
  CEdit		m_MaleIcon;
  CEdit		m_MaleWorld;
  CEdit		m_MaleBiped;
  CButton	m_Playable;
  CButton	m_HideRings;
  CButton	m_HideAmulet;
  CEdit		m_Value;
  CEdit		m_Weight;
  CEdit		m_Script;
  CEdit		m_Enchantment;
  CEdit		m_BipedParts;
  CEdit		m_Name;
  CEdit		m_EditorID;
  CEdit		m_FormID;
	//}}AFX_DATA

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObClotDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObClotDlg)
  afx_msg void OnEditEnchantment();
  afx_msg void OnEditScript();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP();

 };
/*===========================================================================
 *		End of Class CObClotDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION
#endif
/*===========================================================================
 *		End of File Obclotdlg.H
 *=========================================================================*/

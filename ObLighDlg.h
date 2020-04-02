/*===========================================================================
 *
 * File:	Oblighdlg.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 23, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBLIGHDLG_H
#define __OBLIGHDLG_H


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
 * Begin Class CObLighDlg Definition
 *
 *=========================================================================*/
class CObLighDlg : public CObRecordDialog {
  DECLARE_OBRECUIFIELDS();

protected:
  CObLighRecord*	m_pRecord;


public:

	/* Construction */
  CObLighDlg (CWnd* pParent = NULL);

	/* Access the dialog record object */
  virtual CObRecord* GetRecord (void) { return (m_pRecord); }

  	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return ("Light"); }

	/* Set class members */
  void SetRecord (CObRecord* pRecord) { m_pRecord = ObCastClass(CObLighRecord, pRecord); }

	/* Dialog Data */
  //{{AFX_DATA(CObLighDlg)
	enum { IDD = IDD_LIGH_DLG };
	CButton	m_SpotShadow;
	CButton	m_SpotLight;
	CButton	m_Dynamic;
	CButton	m_Negative;
	CButton	m_DefaultOff;
	CButton	m_Carried;
	CEdit	m_Fade;
	CEdit	m_Color;
	CEdit	m_Radius;
	CEdit	m_FOV;
	CEdit	m_Falloff;
	CEdit	m_Time;
	CEdit	m_Flicker;
	CEdit	m_Sound;
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
  //{{AFX_VIRTUAL(CObLighDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObLighDlg)
  afx_msg void OnEditScript();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObLighDlg Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION

#endif
/*===========================================================================
 *		End of File Oblighdlg.H
 *=========================================================================*/

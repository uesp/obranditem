/*===========================================================================
 *
 * File:	Obrigenerateview.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 26, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBRIGENERATEVIEW_H
#define __OBRIGENERATEVIEW_H


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
 * Begin Class CObriGenerateView Definition
 *
 *=========================================================================*/
class CObriGenerateView : public CPropertyPage {
  DECLARE_DYNCREATE(CObriGenerateView)

protected:
  CObranditemView*	m_pParentView;
  bool			m_IsInitialized;

public:

	/* Construction/Destruction */
  CObriGenerateView();
  ~CObriGenerateView();

	/* Get document object */
  CObranditemDoc* GetDocument (void);

	/* Set class members */
  void SetParentView (CObranditemView* pParent) { m_pParentView = pParent; }

	/* Update the view from the current data */
  void GetControlData   (void);
  void SetControlData   (void);
  void EnableItemCounts (const BOOL Enable);

	/* Update the progress bar */
  void UpdateProgress (const float Percent);
 

	/* Dialog Data */
  //{{AFX_DATA(CObriGenerateView)
	enum { IDD = IDD_GENERATEVIEW };
	CEdit	m_ExtIdText;
  CEdit	m_LightCount;
  CButton	m_LightCheck;
  CButton	m_SigilCheck;
  CEdit		m_SigilCount;
  CEdit		m_WeaponCount;
  CEdit		m_StaffCount;
  CEdit		m_ScrollCount;
  CEdit		m_MiscCount;
  CEdit		m_ClothingCount;
  CEdit		m_ArmorCount;
  CButton	m_NumItemsCheck;
  CEdit		m_MergedEspFileText;
  CEdit		m_LevelListCount;
  CEdit		m_LevelListsText;
  CEdit		m_EspFileText;
  CButton	m_StaffCheck;
  CButton	m_MiscCheck;
  CButton	m_CreateListCheck;
  CButton	m_WeaponCheck;
  CButton	m_ScrollCheck;
  CButton	m_ClothingCheck;
  CButton	m_ArmorCheck;
  CEdit		m_MaxLevelText;
  CEdit		m_MinLevelText;
  CEdit		m_NumItemsText;
  CComboBox	m_IDTypeList;
  CEdit		m_IDPrefixText;
  CProgressCtrl	m_ProgressBar;
  CEdit		m_UniqueChance;
  CEdit		m_SuffixChance;
  CEdit		m_PrefixChance;
  CEdit		m_NewItemCount;
  CEdit		m_UniqueCount;
  CEdit		m_PrefixCount;
  CEdit		m_SuffixCount;
  CEdit		m_BaseItemCount;
  CEdit		m_UniquesText;
  CEdit		m_EffectsText;
  CEdit		m_BaseItemsText;
	//}}AFX_DATA

	/* ClassWizard generate virtual function overrides */
  //{{AFX_VIRTUAL(CObriGenerateView)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObriGenerateView)
  virtual BOOL OnInitDialog();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnGenerateButton();
  afx_msg void OnBrowseBaseitems();
  afx_msg void OnBrowseEffects();
  afx_msg void OnBrowseUniques();
  afx_msg void OnBrowseEspfile();
  afx_msg void OnBrowseLevellists();
	afx_msg void OnBrowseMergedespfile();
	afx_msg void OnItemsCheck();
	afx_msg void OnOptionsButton();
	afx_msg void OnGeneratemoreButton();
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObriGenerateView Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File Obrigenerateview.H
 *=========================================================================*/

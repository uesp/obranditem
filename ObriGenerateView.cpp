/*===========================================================================
 *
 * File:	Obrigenerateview.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 26, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObriGenerateView.h"
#include "obranditemDoc.h"
#include "obranditemview.h"
#include <iostream>
#include <map>
#include "obrioptionsdlg.h"



/*===========================================================================
 *
 * Begin Local Definitions
 *
 *=========================================================================*/
#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

  IMPLEMENT_DYNCREATE(CObriGenerateView, CPropertyPage)
/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriGenerateView Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObriGenerateView, CPropertyPage)
	//{{AFX_MSG_MAP(CObriGenerateView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_GENERATE_BUTTON, OnGenerateButton)
	ON_BN_CLICKED(IDC_BROWSE_BASEITEMS, OnBrowseBaseitems)
	ON_BN_CLICKED(IDC_BROWSE_EFFECTS, OnBrowseEffects)
	ON_BN_CLICKED(IDC_BROWSE_UNIQUES, OnBrowseUniques)
	ON_BN_CLICKED(IDC_BROWSE_ESPFILE, OnBrowseEspfile)
	ON_BN_CLICKED(IDC_BROWSE_LEVELLISTS, OnBrowseLevellists)
	ON_BN_CLICKED(IDC_BROWSE_MERGEDESPFILE, OnBrowseMergedespfile)
	ON_BN_CLICKED(IDC_ITEMS_CHECK, OnItemsCheck)
	ON_BN_CLICKED(IDC_OPTIONS_BUTTON, OnOptionsButton)
	ON_BN_CLICKED(IDC_GENERATEMORE_BUTTON, OnGeneratemoreButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Class CObriGenerateView Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Function - int s_GenerateCallback (Data);
 *
 *=========================================================================*/
int s_GenerateCallback (obri_cbdata_t Data) {
  CObriGenerateView* pView = (CObriGenerateView *) Data.UserData;

  pView->UpdateProgress(Data.Completed);
  return (0);
 }
/*===========================================================================
 *		End of Function s_GenerateCallback()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Constructor
 *
 *=========================================================================*/
CObriGenerateView::CObriGenerateView() : CPropertyPage(CObriGenerateView::IDD) {
  //{{AFX_DATA_INIT(CObriGenerateView)
  //}}AFX_DATA_INIT

  m_pParentView   = NULL;
  m_IsInitialized = false;
 }
/*===========================================================================
 *		End of Class CObriGenerateView Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Destructor
 *
 *=========================================================================*/
CObriGenerateView::~CObriGenerateView() {
 }
/*===========================================================================
 *		End of Class CObriGenerateView Destructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObriGenerateView::DoDataExchange (CDataExchange* pDX) {
  CPropertyPage::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CObriGenerateView)
	DDX_Control(pDX, IDC_EXTID_TEXT, m_ExtIdText);
	DDX_Control(pDX, IDC_LIGHTCOUNT_TEXT, m_LightCount);
	DDX_Control(pDX, IDC_LIGHT_CHECK, m_LightCheck);
	DDX_Control(pDX, IDC_SIGIL_CHECK, m_SigilCheck);
	DDX_Control(pDX, IDC_SIGILLCOUNT_TEXT, m_SigilCount);
	DDX_Control(pDX, IDC_WEAPONCOUNT_TEXT, m_WeaponCount);
	DDX_Control(pDX, IDC_STAFFCOUNT_TEXT, m_StaffCount);
	DDX_Control(pDX, IDC_SCROLLCOUNT_TEXT, m_ScrollCount);
	DDX_Control(pDX, IDC_MISCCOUNT_TEXT, m_MiscCount);
	DDX_Control(pDX, IDC_CLOTHINGCOUNT_TEXT, m_ClothingCount);
	DDX_Control(pDX, IDC_ARMORCOUNT_TEXT, m_ArmorCount);
	DDX_Control(pDX, IDC_ITEMS_CHECK, m_NumItemsCheck);
	DDX_Control(pDX, IDC_MERGEDESPFILE_TEXT, m_MergedEspFileText);
	DDX_Control(pDX, IDC_LEVELLIST_COUNT, m_LevelListCount);
	DDX_Control(pDX, IDC_LEVELLISTS_TEXT, m_LevelListsText);
	DDX_Control(pDX, IDC_ESPFILE_TEXT, m_EspFileText);
	DDX_Control(pDX, IDC_STAFF_CHECK, m_StaffCheck);
	DDX_Control(pDX, IDC_MISC_CHECK, m_MiscCheck);
	DDX_Control(pDX, IDC_CREATELIST_CHECK, m_CreateListCheck);
	DDX_Control(pDX, IDC_WEAPON_CHECK, m_WeaponCheck);
	DDX_Control(pDX, IDC_SCROLL_CHECK, m_ScrollCheck);
	DDX_Control(pDX, IDC_CLOTHING_CHECK, m_ClothingCheck);
	DDX_Control(pDX, IDC_ARMOR_CHECK, m_ArmorCheck);
	DDX_Control(pDX, IDC_MAXLEVEL_TEXT, m_MaxLevelText);
	DDX_Control(pDX, IDC_MINLEVEL_TEXT, m_MinLevelText);
	DDX_Control(pDX, IDC_NUMITEMS_TEXT, m_NumItemsText);
	DDX_Control(pDX, IDC_IDTYPE_LIST, m_IDTypeList);
	DDX_Control(pDX, IDC_IDPREFIX_TEXT, m_IDPrefixText);
	DDX_Control(pDX, IDC_PROGRESSBAR, m_ProgressBar);
	DDX_Control(pDX, IDC_UNIQUE_CHANCE, m_UniqueChance);
	DDX_Control(pDX, IDC_SUFFIX_CHANCE, m_SuffixChance);
	DDX_Control(pDX, IDC_PREFIX_CHANCE, m_PrefixChance);
	DDX_Control(pDX, IDC_NEWITEMS_COUNT, m_NewItemCount);
	DDX_Control(pDX, IDC_UNIQUES_COUNT, m_UniqueCount);
	DDX_Control(pDX, IDC_PREFIX_COUNT, m_PrefixCount);
	DDX_Control(pDX, IDC_SUFFIX_COUNT, m_SuffixCount);
	DDX_Control(pDX, IDC_BASEITEM_COUNT, m_BaseItemCount);
	DDX_Control(pDX, IDC_UNIQUES_TEXT, m_UniquesText);
	DDX_Control(pDX, IDC_EFFECTS_TEXT, m_EffectsText);
	DDX_Control(pDX, IDC_BASEITEMS_TEXT, m_BaseItemsText);
	//}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObriGenerateView::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Method - CObranditemDoc* GetDocument (void);
 *
 *=========================================================================*/
CObranditemDoc* CObriGenerateView::GetDocument (void) {
  if (m_pParentView != NULL) return m_pParentView->GetDocument();
  return (NULL);
 }
/*===========================================================================
 *		End of Class Method CObriGenerateView::GetDocument()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Method - void GetControlData (void);
 *
 * Get current control data and store it in the project file.
 *
 *=========================================================================*/
void CObriGenerateView::GetControlData (void) {
  CObriGenerate* pGenerate = &GetDocument()->GetGenerate();
  CString        Buffer;
  int		 ListIndex;

  ListIndex = m_IDTypeList.GetCurSel();

  if (ListIndex >= 0) {
    if (pGenerate->GetOptions().IDType != (int) m_IDTypeList.GetItemData(ListIndex)) GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().IDType = m_IDTypeList.GetItemData(ListIndex);
   }

  if (m_PrefixChance.GetModify()) {
    m_PrefixChance.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_PrefixChance.GetWindowText(Buffer);
    pGenerate->GetOptions().PrefixChance = (float) atof(Buffer) / 100.0f;
   }

  if (m_SuffixChance.GetModify()) {
    m_SuffixChance.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_SuffixChance.GetWindowText(Buffer);
    pGenerate->GetOptions().SuffixChance = (float) atof(Buffer) / 100.0f;
   }

  if (m_UniqueChance.GetModify()) {
    m_UniqueChance.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_UniqueChance.GetWindowText(Buffer);
    pGenerate->GetOptions().UniqueChance = (float) atof(Buffer) / 100.0f;
   }

  if (m_IDPrefixText.GetModify()) {
    m_IDPrefixText.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_IDPrefixText.GetWindowText(Buffer);
    strnncpy(pGenerate->GetOptions().IDPrefix, Buffer, OBRI_MAX_NAMESIZE);
   }

  if (m_ExtIdText.GetModify()) {
    m_ExtIdText.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_ExtIdText.GetWindowText(Buffer);
    pGenerate->GetOptions().ExtendedID = Buffer;
   }

  if (m_NumItemsText.GetModify()) {
    m_NumItemsText.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_NumItemsText.GetWindowText(Buffer);
    pGenerate->GetOptions().NumItems = atoi(Buffer);
   }

  if (m_ArmorCount.GetModify()) {
    m_ArmorCount.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_ArmorCount.GetWindowText(Buffer);
    pGenerate->GetOptions().NumArmor = atoi(Buffer);
   }

  if (m_ClothingCount.GetModify()) {
    m_ClothingCount.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_ClothingCount.GetWindowText(Buffer);
    pGenerate->GetOptions().NumClothing = atoi(Buffer);
   }

  if (m_LightCount.GetModify()) {
    m_LightCount.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_LightCount.GetWindowText(Buffer);
    pGenerate->GetOptions().NumLights = atoi(Buffer);
   }

  if (m_MiscCount.GetModify()) {
    m_MiscCount.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_MiscCount.GetWindowText(Buffer);
    pGenerate->GetOptions().NumMisc = atoi(Buffer);
   }

  if (m_ScrollCount.GetModify()) {
    m_ScrollCount.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_ScrollCount.GetWindowText(Buffer);
    pGenerate->GetOptions().NumScrolls = atoi(Buffer);
   }

  if (m_SigilCount.GetModify()) {
    m_SigilCount.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_SigilCount.GetWindowText(Buffer);
    pGenerate->GetOptions().NumSigils = atoi(Buffer);
   }

  if (m_StaffCount.GetModify()) {
    m_StaffCount.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_StaffCount.GetWindowText(Buffer);
    pGenerate->GetOptions().NumStaffs = atoi(Buffer);
   }


  if (m_WeaponCount.GetModify()) {
    m_WeaponCount.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_WeaponCount.GetWindowText(Buffer);
    pGenerate->GetOptions().NumWeapons = atoi(Buffer);
   }

  if (m_MinLevelText.GetModify()) {
    m_MinLevelText.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_MinLevelText.GetWindowText(Buffer);
    pGenerate->GetOptions().MinLevel = atoi(Buffer);
   }

  if (m_MaxLevelText.GetModify()) {
    m_MaxLevelText.SetModify(FALSE);
    GetDocument()->SetModifiedFlag();
    m_MaxLevelText.GetWindowText(Buffer);
    pGenerate->GetOptions().MaxLevel = atoi(Buffer);
   }
 
  if (m_ArmorCheck.GetCheck() != ((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_ARMOR) != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().ItemMask &= ~OBRI_ITEMTYPE_ARMOR;
    if (m_ArmorCheck.GetCheck()) pGenerate->GetOptions().ItemMask |= OBRI_ITEMTYPE_ARMOR;
   }

  if (m_ClothingCheck.GetCheck() != ((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_CLOTHING) != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().ItemMask &= ~OBRI_ITEMTYPE_CLOTHING;
    if (m_ClothingCheck.GetCheck()) pGenerate->GetOptions().ItemMask |= OBRI_ITEMTYPE_CLOTHING;
   }

  if (m_LightCheck.GetCheck() != ((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_LIGHT) != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().ItemMask &= ~OBRI_ITEMTYPE_LIGHT;
    if (m_LightCheck.GetCheck()) pGenerate->GetOptions().ItemMask |= OBRI_ITEMTYPE_LIGHT;
   }

  if (m_MiscCheck.GetCheck() != ((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_MISC) != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().ItemMask &= ~OBRI_ITEMTYPE_MISC;
    if (m_MiscCheck.GetCheck()) pGenerate->GetOptions().ItemMask |= OBRI_ITEMTYPE_MISC;
   }

  if (m_ScrollCheck.GetCheck() != ((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_SCROLL) != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().ItemMask &= ~OBRI_ITEMTYPE_SCROLL;
    if (m_ScrollCheck.GetCheck()) pGenerate->GetOptions().ItemMask |= OBRI_ITEMTYPE_SCROLL;
   }

  if (m_SigilCheck.GetCheck() != ((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_SIGIL) != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().ItemMask &= ~OBRI_ITEMTYPE_SIGIL;
    if (m_SigilCheck.GetCheck()) pGenerate->GetOptions().ItemMask |= OBRI_ITEMTYPE_SIGIL;
   }

  if (m_StaffCheck.GetCheck() != ((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_STAFF) != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().ItemMask &= ~OBRI_ITEMTYPE_STAFF;
    if (m_StaffCheck.GetCheck()) pGenerate->GetOptions().ItemMask |= OBRI_ITEMTYPE_STAFF;
   }

  if (m_WeaponCheck.GetCheck() != ((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_WEAPON) != 0)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().ItemMask &= ~OBRI_ITEMTYPE_WEAPON;
    if (m_WeaponCheck.GetCheck()) pGenerate->GetOptions().ItemMask |= OBRI_ITEMTYPE_WEAPON;
   }

  if (m_CreateListCheck.GetCheck() != ((int) pGenerate->GetOptions().CreateLevelLists)) {
    GetDocument()->SetModifiedFlag();
    pGenerate->GetOptions().CreateLevelLists = (m_CreateListCheck.GetCheck() != 0);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerateView::GetControlData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - BOOL OnInitDialog ();
 *
 *=========================================================================*/
BOOL CObriGenerateView::OnInitDialog() {
  int ListIndex;

  CPropertyPage::OnInitDialog();
  
	/* Create the ID type list */
  ListIndex = m_IDTypeList.AddString("Long");
  if (ListIndex >= 0) m_IDTypeList.SetItemData(ListIndex, OBRI_IDTYPE_LONG);
  ListIndex = m_IDTypeList.AddString("Short8");
  if (ListIndex >= 0) m_IDTypeList.SetItemData(ListIndex, OBRI_IDTYPE_SHORT8);
  ListIndex = m_IDTypeList.AddString("Short6");
  if (ListIndex >= 0) m_IDTypeList.SetItemData(ListIndex, OBRI_IDTYPE_SHORT6);
  ListIndex = m_IDTypeList.AddString("Short4");
  if (ListIndex >= 0) m_IDTypeList.SetItemData(ListIndex, OBRI_IDTYPE_SHORT4);
  ListIndex = m_IDTypeList.AddString("Short");
  if (ListIndex >= 0) m_IDTypeList.SetItemData(ListIndex, OBRI_IDTYPE_SHORT);
  ListIndex = m_IDTypeList.AddString("Extended");
  if (ListIndex >= 0) m_IDTypeList.SetItemData(ListIndex, OBRI_IDTYPE_EXT);

	/* Set text control limits */
  m_IDPrefixText.SetLimitText(24);
  m_SuffixChance.SetLimitText(6);
  m_PrefixChance.SetLimitText(6);
  m_UniqueChance.SetLimitText(6);
  m_NumItemsText.SetLimitText(8);
  m_ArmorCount.SetLimitText(8);
  m_ClothingCount.SetLimitText(8);
  m_LightCount.SetLimitText(8);
  m_MiscCount.SetLimitText(8);
  m_ScrollCount.SetLimitText(8);
  m_SigilCount.SetLimitText(8);
  m_StaffCount.SetLimitText(8);
  m_WeaponCount.SetLimitText(8);
  m_MinLevelText.SetLimitText(4);
  m_MaxLevelText.SetLimitText(4);
      
  m_ProgressBar.SetRange(0, 100);
	
  m_IsInitialized = true;
  SetControlData();
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnInitDialog()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnSize (nType, cx, cy);
 *
 *=========================================================================*/
void CObriGenerateView::OnSize(UINT nType, int cx, int cy) {
  CPropertyPage::OnSize(nType, cx, cy);

 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnSize()
 *=========================================================================*/

  
/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnGenerateButton ();
 *
 *=========================================================================*/
void CObriGenerateView::OnGenerateButton() {
  CObriGenerate* pGenerate = &GetDocument()->GetGenerate();

  GetControlData();

  m_ProgressBar.SetPos(0);
  pGenerate->SetCallback(s_GenerateCallback, (dword) (void *)this);
  
  SetCursor(LoadCursor(NULL, IDC_WAIT));
   
  pGenerate->CreateItems();

  SetCursor(LoadCursor(NULL, IDC_ARROW));

  m_ProgressBar.SetPos(0);
  GetDocument()->UpdateAllViews(NULL, OBRI_DOC_HINT_UPDATEALL, NULL);
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnGenerateButton()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnGeneratemoreButton ();
 *
 *=========================================================================*/
void CObriGenerateView::OnGeneratemoreButton() {
  CObriGenerate* pGenerate = &GetDocument()->GetGenerate();

  GetControlData();

  m_ProgressBar.SetPos(0);
  pGenerate->SetCallback(s_GenerateCallback, (dword) (void *)this);
  
  SetCursor(LoadCursor(NULL, IDC_WAIT));
   
  pGenerate->CreateMoreItems();

  SetCursor(LoadCursor(NULL, IDC_ARROW));

  m_ProgressBar.SetPos(0);
  GetDocument()->UpdateAllViews(NULL, OBRI_DOC_HINT_UPDATEALL, NULL);
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnGeneratemoreButton()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnBrowseBaseitems ();
 *
 *=========================================================================*/
void CObriGenerateView::OnBrowseBaseitems() {
  CFileDialog BrowseFile(TRUE, ".dat", NULL,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, ".DAT Files (*.dat)|*.dat|All Files (*.*)|*.*||", this);
  int         Result;

  BrowseFile.m_ofn.lpstrTitle = "Select Base Item Data File";
  Result = BrowseFile.DoModal();
  if (Result != IDOK) return;

  Result = GetDocument()->GetGenerate().LoadBaseItems(BrowseFile.GetPathName());

  if (!Result) {
    ShowObriError(this, "Error loading the base item file '%s'!", BrowseFile.GetPathName());
    return;
   }

  GetDocument()->SetModifiedFlag();
  SetControlData();
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnBrowseBaseitems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnBrowseEffects ();
 *
 *=========================================================================*/
void CObriGenerateView::OnBrowseEffects() {
  CFileDialog BrowseFile(TRUE, ".dat", NULL,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, ".DAT Files (*.dat)|*.dat|All Files (*.*)|*.*||", this);
  int         Result;

  BrowseFile.m_ofn.lpstrTitle = "Select Effect Data File";
  Result = BrowseFile.DoModal();
  if (Result != IDOK) return;

  Result = GetDocument()->GetGenerate().LoadEffects(BrowseFile.GetPathName());

  if (!Result) {
    ShowObriError(this, "Error loading the effects file '%s'!", BrowseFile.GetPathName());
    return;
   }


  GetDocument()->SetModifiedFlag();
  SetControlData();
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnBrowseEffects()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnBrowseUniques ();
 *
 *=========================================================================*/
void CObriGenerateView::OnBrowseUniques() {
  CFileDialog BrowseFile(TRUE, ".dat", NULL,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, ".DAT Files (*.dat)|*.dat|All Files (*.*)|*.*||", this);
  int         Result;

  BrowseFile.m_ofn.lpstrTitle = "Select Unique Data File";
  Result = BrowseFile.DoModal();
  if (Result != IDOK) return;

  Result = GetDocument()->GetGenerate().LoadUniques(BrowseFile.GetPathName());

  if (!Result) {
    ShowObriError(this, "Error loading the uniques file '%s'!", BrowseFile.GetPathName());
    return;
   }

  GetDocument()->SetModifiedFlag();
  SetControlData();
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnBrowseUniques()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnBrowseLevellists ();
 *
 *=========================================================================*/
void CObriGenerateView::OnBrowseLevellists() {
  CFileDialog BrowseFile(TRUE, ".dat", NULL,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, ".DAT Files (*.dat)|*.dat|All Files (*.*)|*.*||", this);
  int         Result;

  BrowseFile.m_ofn.lpstrTitle = "Select Level Lists Data File";
  Result = BrowseFile.DoModal();
  if (Result != IDOK) return;

  Result = GetDocument()->GetGenerate().LoadLevelLists(BrowseFile.GetPathName());

  if (!Result) {
    ShowObriError(this, "Error loading the level lists file '%s'!", BrowseFile.GetPathName());
    return;
   }

  GetDocument()->SetModifiedFlag();
  SetControlData();
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnBrowseLevellists()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnBrowseEspfile ();
 *
 *=========================================================================*/
void CObriGenerateView::OnBrowseEspfile() {
  CFileDialog BrowseFile(FALSE, ".esp", GetDocument()->GetGenerate().GetProjectFile().GetOutputEsp(), 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Plugin Files (*.esp)|*.esp|Master Files (*.es.mp)|*.esm|All Files (*.*)|*.*||", this);
  int         Result;

  BrowseFile.m_ofn.lpstrTitle = "Select Output Mod File";
  Result = BrowseFile.DoModal();
  if (Result != IDOK) return;

  GetDocument()->GetGenerate().GetProjectFile().SetOutputEsp(BrowseFile.GetPathName());

  GetDocument()->SetModifiedFlag();
  SetControlData();
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnBrowseEspfile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnBrowseMergedespfile ();
 *
 *=========================================================================*/
void CObriGenerateView::OnBrowseMergedespfile() {
  CFileDialog BrowseFile(TRUE, ".esp", NULL, 
			OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "Plugin Files (*.esp)|*.esp|All Files (*.*)|*.*||", this);
  int         Result;

  BrowseFile.m_ofn.lpstrTitle = "Select Merged Plugin File";
  Result = BrowseFile.DoModal();
  if (Result != IDOK) return;

  GetDocument()->GetGenerate().GetProjectFile().DeleteAllMergedEspFiles();
  GetDocument()->GetGenerate().GetProjectFile().AddEspFile(BrowseFile.GetPathName());

  GetDocument()->SetModifiedFlag();
  SetControlData();
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnBrowseMergedespfile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Method - void EnableItemCounts (Enable);
 *
 *=========================================================================*/
void CObriGenerateView::EnableItemCounts (const BOOL Enable) {
  m_NumItemsText.EnableWindow(!Enable);

  m_ArmorCount.EnableWindow(Enable);
  m_ClothingCount.EnableWindow(Enable);
  m_LightCount.EnableWindow(Enable);
  m_MiscCount.EnableWindow(Enable);
  m_ScrollCount.EnableWindow(Enable);
  m_SigilCount.EnableWindow(Enable);
  m_StaffCount.EnableWindow(Enable);
  m_WeaponCount.EnableWindow(Enable);
 }
/*===========================================================================
 *		End of Class Method CObriGenerateView::EnableItemCounts()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnItemsCheck ();
 *
 *=========================================================================*/
void CObriGenerateView::OnItemsCheck() {
  GetDocument()->SetModifiedFlag();
  GetDocument()->GetGenerate().GetOptions().MakeTotalItems = (m_NumItemsCheck.GetCheck() != 0);
  EnableItemCounts(m_NumItemsCheck.GetCheck() == 0);
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnItemsCheck()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Method - void SetControlData (void);
 *
 *=========================================================================*/
void CObriGenerateView::SetControlData (void) {
  CObriGenerate* pGenerate = &GetDocument()->GetGenerate();
  CString        Buffer;

  m_BaseItemsText.SetWindowText(pGenerate->GetProjectFile().GetBaseItems());
  m_EffectsText.SetWindowText(pGenerate->GetProjectFile().GetEffects());
  m_UniquesText.SetWindowText(pGenerate->GetProjectFile().GetUniques());
  m_LevelListsText.SetWindowText(pGenerate->GetProjectFile().GetLevelLists());

  Buffer.Format("%d", pGenerate->GetNumBaseItems());
  m_BaseItemCount.SetWindowText(Buffer);

  Buffer.Format("%d", pGenerate->GetNumSuffixes());
  m_SuffixCount.SetWindowText(Buffer);
  
  Buffer.Format("%d", pGenerate->GetNumPrefixes());
  m_PrefixCount.SetWindowText(Buffer);
  
  Buffer.Format("%d", pGenerate->GetNumUniques());
  m_UniqueCount.SetWindowText(Buffer);
  
  Buffer.Format("%d", pGenerate->GetNumNewItems());
  m_NewItemCount.SetWindowText(Buffer);

  Buffer.Format("%d", pGenerate->GetNumLevelLists());
  m_LevelListCount.SetWindowText(Buffer);
  
  Buffer.Format("%g", pGenerate->GetOptions().PrefixChance * 100.0f);
  m_PrefixChance.SetWindowText(Buffer);
  m_PrefixChance.SetModify(FALSE);

  Buffer.Format("%g", pGenerate->GetOptions().SuffixChance * 100.0f);
  m_SuffixChance.SetWindowText(Buffer);
  m_SuffixChance.SetModify(FALSE);

  Buffer.Format("%g", pGenerate->GetOptions().UniqueChance * 100.0f);
  m_UniqueChance.SetWindowText(Buffer);
  m_UniqueChance.SetModify(FALSE);

  m_IDPrefixText.SetWindowText(pGenerate->GetOptions().IDPrefix);
  m_IDPrefixText.SetModify(FALSE);

  m_ExtIdText.SetWindowText(pGenerate->GetOptions().ExtendedID);
  m_ExtIdText.SetModify(FALSE);

  FindComboListItem(m_IDTypeList, pGenerate->GetOptions().IDType, true);

  Buffer.Format("%d", pGenerate->GetOptions().NumItems);
  m_NumItemsText.SetWindowText(Buffer);
  m_NumItemsText.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().NumArmor);
  m_ArmorCount.SetWindowText(Buffer);
  m_ArmorCount.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().NumClothing);
  m_ClothingCount.SetWindowText(Buffer);
  m_ClothingCount.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().NumLights);
  m_LightCount.SetWindowText(Buffer);
  m_LightCount.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().NumMisc);
  m_MiscCount.SetWindowText(Buffer);
  m_MiscCount.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().NumScrolls);
  m_ScrollCount.SetWindowText(Buffer);
  m_ScrollCount.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().NumSigils);
  m_SigilCount.SetWindowText(Buffer);
  m_SigilCount.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().NumStaffs);
  m_StaffCount.SetWindowText(Buffer);
  m_StaffCount.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().NumWeapons);
  m_WeaponCount.SetWindowText(Buffer);
  m_WeaponCount.SetModify(FALSE);

  Buffer.Format("%d", pGenerate->GetOptions().MinLevel);
  m_MinLevelText.SetWindowText(Buffer);
  m_MinLevelText.SetModify(FALSE);
  
  Buffer.Format("%d", pGenerate->GetOptions().MaxLevel);
  m_MaxLevelText.SetWindowText(Buffer);
  m_MaxLevelText.SetModify(FALSE);

  m_NumItemsCheck.SetCheck(pGenerate->GetOptions().MakeTotalItems);
  m_ArmorCheck.SetCheck((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_ARMOR) != 0);
  m_ClothingCheck.SetCheck((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_CLOTHING) != 0);
  m_LightCheck.SetCheck((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_LIGHT) != 0);
  m_MiscCheck.SetCheck((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_MISC) != 0);
  m_ScrollCheck.SetCheck((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_SCROLL) != 0);
  m_SigilCheck.SetCheck((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_SIGIL) != 0);
  m_StaffCheck.SetCheck((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_STAFF) != 0);
  m_WeaponCheck.SetCheck((pGenerate->GetOptions().ItemMask & OBRI_ITEMTYPE_WEAPON) != 0);
  
  m_CreateListCheck.SetCheck(pGenerate->GetOptions().CreateLevelLists != 0);

  m_EspFileText.SetWindowText(GetDocument()->GetGenerate().GetProjectFile().GetOutputEsp());

  if (GetDocument()->GetGenerate().GetProjectFile().GetNumEspFiles() > 0)
    m_MergedEspFileText.SetWindowText(GetDocument()->GetGenerate().GetProjectFile().GetEspFile(0));
  else
    m_MergedEspFileText.SetWindowText("");

  EnableItemCounts(!pGenerate->GetOptions().MakeTotalItems);
 }
/*===========================================================================
 *		End of Class Method CObriGenerateView::SetControlData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Method - void UpdateProgress (Percent);
 *
 *=========================================================================*/
void CObriGenerateView::UpdateProgress (const float Percent) {
//CMap<int, int, int, int> Test;

  m_ProgressBar.SetPos((int)Percent);
 }
/*===========================================================================
 *		End of Class Method CObriGenerateView::UpdateProgress()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerateView Event - void OnOptionsButton ();
 *
 *=========================================================================*/
void CObriGenerateView::OnOptionsButton() {
  CObriOptionsDlg OptionsDlg;

  OptionsDlg.SetParentView(m_pParentView);
  OptionsDlg.DoModal();
 }
/*===========================================================================
 *		End of Class Event CObriGenerateView::OnOptionsButton()
 *=========================================================================*/



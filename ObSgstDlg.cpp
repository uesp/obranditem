/*===========================================================================
 *
 * File:	Obsgstdlg.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 23, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "ObSgstDlg.h"


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
/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObSgstDlg, CObRecordDialog)
	//{{AFX_MSG_MAP(CObSgstDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Begin UI Field Map
 *
 *=========================================================================*/
BEGIN_OBRECUIFIELDS(CObSgstDlg)
	ADD_OBRECUIFIELDS( OB_FIELD_EDITORID,	IDC_EDITORID)
	ADD_OBRECUIFIELDS( OB_FIELD_FORMID,	IDC_FORMID)
	ADD_OBRECUIFIELDS( OB_FIELD_ITEMNAME,	IDC_ITEMNAME)
	ADD_OBRECUIFIELDS( OB_FIELD_SCRIPT,	IDC_SCRIPT)
	ADD_OBRECUIFIELDS( OB_FIELD_USES,	IDC_USES)
	ADD_OBRECUIFIELDS( OB_FIELD_VALUE,	IDC_VALUE)
	ADD_OBRECUIFIELDS( OB_FIELD_WEIGHT,	IDC_WEIGHT)
	ADD_OBRECUIFIELDS( OB_FIELD_MODEL,	IDC_MODEL)
	ADD_OBRECUIFIELDS( OB_FIELD_ICON,	IDC_ICONTEXT)
END_OBRECUIFIELDS()
/*===========================================================================
 *		End of UI Field Map
 *=========================================================================*/


/*===========================================================================
 *
 * Special callback functions for list sorting.
 *
 *=========================================================================*/
static int CALLBACK s_EffectRecListSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
  OBRL_SORTFUNC_GETPARAMS(lParam1, lParam2, lParamSort);
  //CObSgstRecord*    pSigil1  = ObCastClass(CObSgsthRecord, pRecord1); 
  //CObSgstRecord*    pSigil2  = ObCastClass(CObSgstRecord, pRecord2); 
  CObEfidSubrecord* pEffect1 = ObCastClass(CObEfidSubrecord, pCustomData1->pSubrecords[0]);
  CObEfidSubrecord* pEffect2 = ObCastClass(CObEfidSubrecord, pCustomData2->pSubrecords[0]);
  int               Value = 0;

  switch (pSortData->FieldID) {
    case OB_FIELD_SCRIPT: 
        if (pCustomData1->pSubrecords[2] == NULL) {
	  Value = (pCustomData2->pSubrecords[2] == NULL) ? 0 : -1;
	 }
	else if (pCustomData2->pSubrecords[2] == NULL) {
	  Value = 1;
	 }
        else {
	  CObScitSubrecord* pScit1 = ObCastClass(CObScitSubrecord, pCustomData1->pSubrecords[2]);
	  CObScitSubrecord* pScit2 = ObCastClass(CObScitSubrecord, pCustomData2->pSubrecords[2]);

          if (pScit1 == NULL) {
	    Value = (pScit2 == NULL) ? 0 : -1;
	   }
          else if (pScit2 == NULL) {
	    Value = 1;
	   }
	  else {	    
	    Value = pScit1->GetScriptFormID() - pScit1->GetScriptFormID();
	   }
         }
        break;
    };
  
  if (pSortData->Reverse) return (-Value);
  return (Value);
 }
/*===========================================================================
 *		End of Function CALLBACK s_DefaultRecListSort()
 *=========================================================================*/


/*===========================================================================
 *
 * Begin List Column Definitions
 *
 *=========================================================================*/
static obreclistcolinit_t s_EffectInit[] = {
	{ OB_FIELD_EFFECTID,	 5,	LVCFMT_LEFT },
	{ OB_FIELD_EFFECTDESC,	 120,	LVCFMT_CENTER },
	{ OB_FIELD_MAGNITUDE,	 40,	LVCFMT_CENTER },
	{ OB_FIELD_AREA,	 40,	LVCFMT_CENTER },
	{ OB_FIELD_DURATION,	 40,	LVCFMT_CENTER },
	{ OB_FIELD_RANGE,	 50,	LVCFMT_CENTER },
	{ OB_FIELD_ACTORVALUE,	 75,	LVCFMT_CENTER },
	{ OB_FIELD_SCRIPT,	 90,	LVCFMT_CENTER,	s_EffectRecListSort },
	{ OB_FIELD_EFFECTNAME,	 90,	LVCFMT_CENTER },
	{ OB_FIELD_SCRIPTSCHOOL, 75,	LVCFMT_CENTER },
	{ OB_FIELD_VISUALNAME,   45,	LVCFMT_CENTER },
	{ OB_FIELD_HOSTILE,      45,	LVCFMT_CENTER },
	//{ OB_FIELD_COST,	 60,	LVCFMT_CENTER },
	//{ OB_FIELD_SCHOOL,	 60,	LVCFMT_CENTER },
	{ OB_FIELD_NONE, 0, 0 }
 };

static obrecfield_t s_EffectFields[] = {
	{ "Effect",		OB_FIELD_EFFECTID,	0, NULL },
	{ "Effect Description", OB_FIELD_EFFECTDESC,	0, NULL },
	{ "Mag",		OB_FIELD_MAGNITUDE,	0, NULL },
	{ "Area",		OB_FIELD_AREA,		0, NULL },
	{ "Dur",		OB_FIELD_DURATION,	0, NULL },
	{ "Range",		OB_FIELD_RANGE,		0, NULL },
	{ "Cost",		OB_FIELD_COST,		0, NULL },
	{ "School",		OB_FIELD_SCHOOL,	0, NULL },
	{ "Actor Value",	OB_FIELD_ACTORVALUE,	0, NULL },
	{ "Effect Name",	OB_FIELD_EFFECTNAME,	0, NULL },
	{ "Script School",	OB_FIELD_SCRIPTSCHOOL,	0, NULL },
	{ "Visual",		OB_FIELD_VISUALNAME,	0, NULL },
	{ "Hostile",		OB_FIELD_HOSTILE,	0, NULL },
	{ "Script",		OB_FIELD_SCRIPT,	0, NULL },
	{ NULL,			OB_FIELD_NONE,		0, NULL }
 };
/*===========================================================================
 *		End of List Column Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObSgstDlg Constructor
 *
 *=========================================================================*/
CObSgstDlg::CObSgstDlg(CWnd* pParent) : CObRecordDialog(CObSgstDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CObSgstDlg)
  //}}AFX_DATA_INIT

  m_pRecord = NULL;
 }
/*===========================================================================
 *		End of Class CObSgstDlg Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObSgstDlg Method - void ClearControlData (void);
 *
 *=========================================================================*/
void CObSgstDlg::ClearControlData (void) { 

  m_EffectList.DeleteAllItems();

  CObRecordDialog::ClearControlData();
 }
/*===========================================================================
 *		End of Class Method CObSgstDlg::ClearControlData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObSgstDlg Method - void DoDataExchange (pDX);
 *
 *=========================================================================*/
void CObSgstDlg::DoDataExchange(CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CObSgstDlg)
	DDX_Control(pDX, IDC_ICONTEXT, m_Icon);
	DDX_Control(pDX, IDC_MODEL, m_Model);
	DDX_Control(pDX, IDC_VALUE, m_Value);
	DDX_Control(pDX, IDC_WEIGHT, m_Weight);
	DDX_Control(pDX, IDC_USES, m_Uses);
	DDX_Control(pDX, IDC_SCRIPT, m_Script);
	DDX_Control(pDX, IDC_ITEMNAME, m_ItemName);
	DDX_Control(pDX, IDC_FORMID, m_FormID);
	DDX_Control(pDX, IDC_EFFECT_LIST, m_EffectList);
	DDX_Control(pDX, IDC_EDITORID, m_EditorID);
	//}}AFX_DATA_MAP
 }
/*===========================================================================
 *		End of Class Method CObSgstDlg::DoDataExchange()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObSgstDlg Event - BOOL OnInitDialog ();
 *
 *=========================================================================*/
BOOL CObSgstDlg::OnInitDialog() {

  CObRecordDialog::OnInitDialog();

  	/* Setup the list */
  m_EffectList.DefaultSettings();
  m_EffectList.SetupCustomList(s_EffectInit, NULL, s_EffectFields);
  m_EffectList.SetOwner(GetOwner());
  m_EffectList.SetActivateType(OB_RLACTIVATE_NONE);
 
  SetEffectList();	 
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObSgstDlg::OnInitDialog()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObSgstDlg Method - void SetControlData (void);
 *
 *=========================================================================*/
void CObSgstDlg::SetControlData (void) { 
  CObRecordDialog::SetControlData();
  SetEffectList();
 }
/*===========================================================================
 *		End of Class Method CObSgstDlg::SetControlData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObSgstDlg Method - void SetEffectList (void);
 *
 *=========================================================================*/
void CObSgstDlg::SetEffectList (void) {
  CObEfidSubrecord*     pEffect;
  CObEfitSubrecord*     pEfit;
  CObScitSubrecord*     pScit;
  CObFullSubrecord*     pFull;
  CObBaseRecord*        pBaseRecord;
  CObScptRecord*        pScript;
  obrlcustomdata_t	CustomData;
  CString               Buffer;
  int                   ItemPos;
  int		        ListIndex;

  if (m_pRecord == NULL) return;

	/* Add all effects in enchantment */
  CustomData.pRecord = m_pRecord;

  for (pEffect = m_pRecord->GetFirstEffect(ItemPos);  pEffect != NULL; pEffect = m_pRecord->GetNextEffect(ItemPos)) {

		/* Get the effect data subrecord */
    pEfit = m_pRecord->GetEffectData(pEffect);
    if (pEfit == NULL) continue;

		/* Setup the custom data structure for the list */
    CustomData.pSubrecords[0] = pEffect;
    CustomData.pSubrecords[1] = pEfit;
    
    if (pEffect->GetDataName() == OB_MGEF_SEFF) {
      pScit = m_pRecord->GetScriptEffectData(pEffect);
      pFull = m_pRecord->GetScriptEffectNameData(pEffect);   
     }
    else {
      pScit = NULL;
      pFull = NULL;
     }

    CustomData.pSubrecords[2] = pScit;
    CustomData.pSubrecords[3] = pFull;

		/* Add the custom record to the list */
    ListIndex = m_EffectList.AddCustomRecord(CustomData);
    if (ListIndex < 0) continue;

		/* Set the script editor ID if required */
    if (pScit == NULL) continue;
    pBaseRecord = m_pRecord->GetParentFile()->FindFormID(pScit->GetScriptFormID());

    if (pBaseRecord != NULL) {
      pScript = ObCastClass(CObScptRecord, pBaseRecord);
      if (pScript != NULL) m_EffectList.SetCustomField(ListIndex, OB_FIELD_SCRIPT, pScript->GetEditorID());
     }
   }

 }
/*===========================================================================
 *		End of Class Method CObSgstDlg::SetEffectList()
 *=========================================================================*/

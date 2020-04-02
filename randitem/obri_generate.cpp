/*===========================================================================
 *
 * File:	Obri_Generate.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 21, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_generate.h"
#include "direct.h"
#include "math.h"
#include "windows.h"


/*===========================================================================
 *
 * Begin Local Definitions
 *
 *=========================================================================*/
  #define OBRI_COMPRESS false
  #define OBRI_OLD_ITEMMAP 0
  #define OBRI_RANDITEM_METHOD 2
  
  #define OBRI_NAMEWEIGHT_MAX		100
  #define OBRI_NAMEWEIGHT_ADJUST1	20

  #define OBRI_NAMEWEIGHT_NONE		0
  #define OBRI_NAMEWEIGHT_MATERIAL	1
  #define OBRI_NAMEWEIGHT_PREFIX	2
  #define OBRI_NAMEWEIGHT_PREFIX2	3
  #define OBRI_NAMEWEIGHT_SUFFIX	4

/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Constructor
 *
 *=========================================================================*/
CObriGenerate::CObriGenerate () : m_EspFile(500000) {

  DefaultObriGenerateOptions(m_GenerateData);

  m_EspFile.InitializeNew();
  m_RandUniques.SetEffects(&m_RandEffects);

  m_LevelLists.SetEspFile(&m_EspFile);

  m_EnchantMap.InitHashTable(50000);
  m_ItemMap.InitHashTable(50000);

  m_NumNewItems     = 0;
  m_NumFailedItems  = 0;
  m_NumEnchantments = 0;
  m_NumDupEnchants  = 0;
  m_NumDupItems     = 0;
  m_HasMergedFile   = false;

  m_Callback.Function = NULL;
  m_Callback.UserData = 0;
  memset(&m_LevelCounts, 0, sizeof(m_LevelCounts));
  m_AverageLevelSum   = 0;

	/* Save the initial directory */
  //SSCHAR Buffer[_MAX_PATH + 1];
  //_getcwd(Buffer, _MAX_PATH);
  //m_InitialPath = Buffer;
 }
/*===========================================================================
 *		End of Class CObriGenerate Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void Destroy (void);
 *
 *=========================================================================*/
void CObriGenerate::Destroy (void) {
  m_RandUniques.Destroy();
  m_RandEffects.Destroy();
  m_RandItems.Destroy();
  m_LevelLists.Destroy();

  m_EnchantMap.Destroy();
  m_ItemMap.Destroy();

  InitializeEspFile();

	/* Reset the ID counter */
  l_ItemIDCount = 1;
  
  m_NumNewItems     = 0;
  m_NumFailedItems  = 0;
  m_NumEnchantments = 0;
  m_NumDupEnchants  = 0;
  m_NumDupItems     = 0;
  m_HasMergedFile   = false;
  
  ObriResetNextFormID1(GetOptions().FirstFormID1);
  ObriResetNextFormID2(GetOptions().FirstFormID2);
  
  memset(&m_LevelCounts, 0, sizeof(m_LevelCounts));
  m_AverageLevelSum = 0;

  DestroyNewRecords();

  m_ProjectFile.Destroy();

  DefaultObriGenerateOptions(m_GenerateData);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::Destroy()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void AddEnchantEffect (pEnchant, Effect, EnchantEffect);
 *
 * Adds a single effect to the given enchantment or spell record.
 *
 *=========================================================================*/
void CObriGenerate::AddEnchantEffect (CObEffectItemRecord* pEnchant, obri_effect_t &Effect, obri_encheff_t& EnchantEffect) {
  CObEfidSubrecord* pEffect;
  CObIdRecord*      pIdRecord;
  CObScptRecord*    pScript;
  obformid_t	    FormID;
  long		    Magnitude;
  long		    Area;
  long		    Duration;

	/* Add a new effect to the enchantment */
  pEffect = pEnchant->AddEffect(*EnchantEffect.pEffectID);
  if (pEffect == NULL) return;

  	/* Generate the random effect properties */
  Area      = SplitValue(EnchantEffect.MinArea,      EnchantEffect.MaxArea,      m_GenerateData.CurrentRatio);
  Duration  = SplitValue(EnchantEffect.MinDuration,  EnchantEffect.MaxDuration,  m_GenerateData.CurrentRatio);
  Magnitude = SplitValue(EnchantEffect.MinMagnitude, EnchantEffect.MaxMagnitude, m_GenerateData.CurrentRatio);

	/* Set the effect properties */
  pEnchant->SetEffectData(pEffect, EnchantEffect.RangeType, Magnitude, Area, Duration, EnchantEffect.ActorValue);

	/* Set the script effect properties if required */
  if (*EnchantEffect.pEffectID == OB_MGEF_SEFF) {
    FormID = OB_FORMID_NULL;
    
    pIdRecord = m_EspFile.FindEditorID(EnchantEffect.Script);

    if (pIdRecord == NULL) {
      AddObGeneralError("The script '%s' used in effect %s was not found!", EnchantEffect.Script, Effect.NameID);
     }
    else {
      pScript = ObCastClass(CObScptRecord, pIdRecord);

      if (pScript == NULL)
        AddObGeneralError("The record '%s' used in effect %s is not a script!", EnchantEffect.Script, Effect.NameID);
      else {
        if (pScript->GetType() != OB_SCRIPTTYPE_MAGICEFFECT) {
	  AddObGeneralError("The script '%s' used in effect %s is not a magic effect script!", EnchantEffect.Script, Effect.NameID);
         }

        FormID = pIdRecord->GetFormID();
       }
     }

    pEnchant->SetScriptEffectName(pEffect, EnchantEffect.EffectName);
    pEnchant->SetScriptEffectData(pEffect, FormID, EnchantEffect.School, *EnchantEffect.pVisualEffectID, EnchantEffect.IsHostile);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::AddEnchantEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void AddLevelListItem (CObRecord* pRecord);
 *
 * Adds a new item record to be added to the level lists depending on the
 * current options.
 *
 *=========================================================================*/
void CObriGenerate::AddLevelListItem (CObRecord* pRecord) {

	/* Ignore if not creating any level lists */
  if (!GetOptions().CreateLevelLists) return;

  m_LevelLists.AddRecord(pRecord, m_GenerateData.pCurrentItem->CustomType, m_GenerateData.pCurrentPrefix, m_GenerateData.pCurrentPrefix2, m_GenerateData.pCurrentSuffix);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::AddLevelListItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - CObRecord* CreateNewRecord (void);
 *
 * Allocates a new record according to the current settings. Adds the
 * new record to the array and returns it.
 *
 *=========================================================================*/
CObRecord* CObriGenerate::CreateNewRecord (void) {
  CObBaseRecord* pNewRecord = NULL;
  CObRecord*     pRecord    = NULL;

	/* Create the object according to the base item type */
  switch (m_GenerateData.pCurrentItem->ItemType) {
    case OBRI_ITEMTYPE_ARMOR:
        pNewRecord = m_EspFile.CreateNewRecord(OB_NAME_ARMO);
	break;
    case OBRI_ITEMTYPE_CLOTHING:
        pNewRecord = m_EspFile.CreateNewRecord(OB_NAME_CLOT);
	break;
    case OBRI_ITEMTYPE_LIGHT:
        pNewRecord = m_EspFile.CreateNewRecord(OB_NAME_LIGH);
	break;
    case OBRI_ITEMTYPE_MISC:
        pNewRecord = m_EspFile.CreateNewRecord(OB_NAME_MISC);
	break;
    case OBRI_ITEMTYPE_SCROLL:
        pNewRecord = m_EspFile.CreateNewRecord(OB_NAME_BOOK);
	break;
    case OBRI_ITEMTYPE_SIGIL:
        pNewRecord = m_EspFile.CreateNewRecord(OB_NAME_SGST);
	break;
    case OBRI_ITEMTYPE_WEAPON:
    case OBRI_ITEMTYPE_STAFF:
        pNewRecord = m_EspFile.CreateNewRecord(OB_NAME_WEAP);
	break;
    default:
	assert(false);
	break;
    }
  
  if (pNewRecord == NULL) return (NULL);
  pRecord = ObCastClass(CObRecord, pNewRecord);
  if (pRecord != NULL) pRecord->SetCompressed(OBRI_COMPRESS);
  return (pRecord);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateNewRecord()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckItemLevel (Level);
 *
 * Returns true if the given base item level is valid or not.
 *
 *=========================================================================*/
bool CObriGenerate::CheckItemLevel (const int Level) {

	/* Include more lower level items */
  //if (m_GenerateData.CurrentItemLevel < 15 && Level <= m_GenerateData.CurrentItemLevel) return (true);
  //return (Level >= (int)(m_GenerateData.CurrentItemLevel*OBRI_MINLEVEL_FACTOR) && Level <= m_GenerateData.CurrentItemLevel);
  return (Level <= m_GenerateData.CurrentItemLevel);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckItemLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckItemLevel2 (pItem);
 *
 * Returns true if the given base item level is valid or not.
 *
 *=========================================================================*/
bool CObriGenerate::CheckItemLevel2 (obri_item_t* pItem) {

  if (pItem->MinEffectLevel > m_GenerateData.MaxItemLevel) return (false);
  if (pItem->MaxEffectLevel < m_GenerateData.MinItemLevel) return (false);

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckItemLevel2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckEffectLevel (Level);
 *
 * Returns true if the given effect level is valid or not.
 *
 *=========================================================================*/
bool CObriGenerate::CheckEffectLevel (const int Level) {

#if (OBRI_EFFECTLEVELCHECK)
  //return (Level >= m_GenerateData.CurrentItemLevel/2 && Level <= m_GenerateData.CurrentItemLevel);
  //return (Level <= m_GenerateData.CurrentItemLevel);

  if (Level < GetOptions().MinLevel) return (false);
  if (Level > GetOptions().MaxLevel) return (false);

  return (Level >= m_GenerateData.pCurrentItem->MinEffectLevel && Level <= m_GenerateData.pCurrentItem->MaxEffectLevel);

#else
  return (true);
#endif
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckEffectLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckEffectLevel2 (Level);
 *
 * Returns true if the given effect level is valid or not.
 *
 *=========================================================================*/
bool CObriGenerate::CheckEffectLevel2 (const int Level) {

  return (Level >= m_GenerateData.MinItemLevel && Level <= m_GenerateData.MaxItemLevel);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckEffectLevel2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckCursedLevel (Level);
 *
 *=========================================================================*/
bool CObriGenerate::CheckCursedLevel (const int Level) {

#if (OBRI_EFFECTLEVELCHECK)
  return (Level <= m_GenerateData.CurrentItemLevel);
#else
  return (true);
#endif
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckCursedLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckFinalItemLevel (Level);
 *
 * Returns true if the given final item level is valid or not.
 *
 *=========================================================================*/
bool CObriGenerate::CheckFinalItemLevel (const int Level) {

	/* Include more lower level items */
  //if (m_GenerateData.CurrentItemLevel < 15 && Level <= m_GenerateData.CurrentItemLevel) return (true);
  //return (Level >= (int)(m_GenerateData.CurrentItemLevel*OBRI_MINLEVEL_FACTOR) && Level <= m_GenerateData.CurrentItemLevel);
  if (!GetOptions().ForceMinLevel) return (true);
  return (Level >= m_GenerateData.CurrentItemLevel);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckFinalItemLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckFinalItemLevel2 (Level);
 *
 * Returns true if the given final item level is valid or not.
 *
 *=========================================================================*/
bool CObriGenerate::CheckFinalItemLevel2 (const int Level) {

  if (!GetOptions().ForceMinLevel) return (true);
  return (Level >= m_GenerateData.MinItemLevel);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckFinalItemLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CheckValidSuffixes (void);
 *
 * Checks the current list of suffixes and removes any that do not match
 * the currently selected prefix.
 *
 *=========================================================================*/
void CObriGenerate::CheckValidSuffixes (void) {
  obri_effect_t* pEffect;
  dword		 Index;
  bool		 IsEnchant = false;

	/* Ignore if no current prefix */
  if (m_GenerateData.pCurrentPrefix == NULL) return;

	/* Ignore if the prefix types are't an enchantment */
  IsEnchant = (m_GenerateData.pCurrentPrefix->EffectType == OBRI_EFFTYPE_ENCHANT);
  if (m_GenerateData.pCurrentPrefix2 != NULL) IsEnchant |= (m_GenerateData.pCurrentPrefix2->EffectType == OBRI_EFFTYPE_ENCHANT);
  if (!IsEnchant) return;

	/* Check each suffix */
  for (Index = 0; Index < m_ValidSuffixes.GetSize(); ++Index) {
    pEffect = m_ValidSuffixes[Index];
    if (pEffect == NULL) return;
    if (pEffect->EffectType != OBRI_EFFTYPE_ENCHANT) continue;
    if (pEffect->Enchantment.EnchantType == m_GenerateData.pCurrentPrefix->Enchantment.EnchantType) continue;

    if (m_GenerateData.pCurrentPrefix2 != NULL) {
      if (pEffect->Enchantment.EnchantType == m_GenerateData.pCurrentPrefix2->Enchantment.EnchantType) continue;
     }

		/* Remove the suffix from the list */
    m_ValidSuffixes.Delete(Index);
    --Index;
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckValidSuffixes()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckPrefix2 (void);
 *
 * Checks if the current second prefix is valid. If not it returns false 
 * and clears the prefix2 object.
 *
 *=========================================================================*/
bool CObriGenerate::CheckPrefix2 (void) {

	/* Ensure we have a prefix */
  if (m_GenerateData.pCurrentPrefix == NULL || m_GenerateData.pCurrentPrefix2 == NULL) return (false);

	/* Ensure the second prefix doesn't have the same effect type */
  if (m_GenerateData.pCurrentPrefix->EffectType == m_GenerateData.pCurrentPrefix2->EffectType) {
    m_GenerateData.pCurrentPrefix2 = NULL;
    return (false);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckPrefix2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void ClearValidItems (void);
 *
 *=========================================================================*/
void CObriGenerate::ClearValidItems (void) {
  m_ValidItems.Destroy();
  m_ValidPrefixes.Destroy();
  m_ValidSuffixes.Destroy();
  m_ValidUniques.Destroy();
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ClearValidItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CompareEffects (pEnchant1, pEffect1, pEnchant2, pEffect2);
 *
 * Compares two enchantment effects and returns true if the two can be 
 * considered identical.
 *
 *=========================================================================*/
bool CObriGenerate::CompareEffects (CObEnchRecord* pEnchant1, CObEfidSubrecord* pEffect1, 
				    CObEnchRecord* pEnchant2, CObEfidSubrecord* pEffect2) {
  CObEfitSubrecord* pEfit1;
  CObEfitSubrecord* pEfit2;
  CObScitSubrecord* pScit1;
  CObScitSubrecord* pScit2;
  
	/* Ensure the effect types are the same */
  if (pEffect1->GetDataName() != pEffect2->GetDataName()) return (false);

	/* Get the effect data */
  pEfit1 = pEnchant1->GetEffectData(pEffect1);
  pEfit2 = pEnchant2->GetEffectData(pEffect2);
  if (pEfit1 == NULL || pEfit2 == NULL) return (false);

	/* Compare the effect data */
  if (pEfit1->GetArea()       != pEfit2->GetArea())       return (false);
  if (pEfit1->GetDuration()   != pEfit2->GetDuration())   return (false);
  if (pEfit1->GetMagnitude()  != pEfit2->GetMagnitude())  return (false);
  if (pEfit1->GetEffectType() != pEfit2->GetEffectType()) return (false);
  if (pEfit1->GetActorValue() != pEfit2->GetActorValue()) return (false);

	/* Compare script effects */
  if (pEffect1->GetDataName() == OB_MGEF_SEFF) {
    pScit1 = pEnchant1->GetScriptEffectData(pEffect1);
    pScit2 = pEnchant2->GetScriptEffectData(pEffect2);
    if (pScit1 == NULL || pScit2 == NULL) return (false);

    if (pScit1->IsHostile()       != pScit2->IsHostile())       return (false);
    if (pScit1->GetSchool()       != pScit2->GetSchool())       return (false);
    if (pScit1->GetScriptFormID() != pScit2->GetScriptFormID()) return (false);
    if (pScit1->GetVisualEffect() != pScit2->GetVisualEffect()) return (false);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CompareEffects()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CompareEnchantments (pEnchant1, pEnchant2);
 *
 * Compares two enchantments and returns true if the two can be considered
 * identical.
 *
 *=========================================================================*/
bool CObriGenerate::CompareEnchantments (CObEnchRecord* pEnchant1, CObEnchRecord* pEnchant2) {
  CObEfidSubrecord* pEffect1;
  CObEfidSubrecord* pEffect2;
  bool              Result;
  int		    Pos1;
  int		    Pos2;

	/* Ignore invalid input */
  if (pEnchant1 == NULL || pEnchant2 == NULL) return (false);

	/* Check base enchantment properties */
  if (pEnchant1->IsAutoCalc() != pEnchant2->IsAutoCalc()) return (false);
  if (pEnchant1->GetCharge()  != pEnchant2->GetCharge())  return (false);
  if (pEnchant1->GetCost()    != pEnchant2->GetCost())    return (false);
  if (pEnchant1->GetType()    != pEnchant2->GetType())    return (false);

	/* Check effects */
  pEffect1 = pEnchant1->GetFirstEffect(Pos1);
  pEffect2 = pEnchant2->GetFirstEffect(Pos2);

  while (pEffect1 != NULL && pEffect2 != NULL) {
    Result = CompareEffects (pEnchant1, pEffect1, pEnchant2, pEffect2);
    if (!Result) return (false);

    pEffect1 = pEnchant1->GetNextEffect(Pos1);
    pEffect2 = pEnchant2->GetNextEffect(Pos2);
   }
    
	/* Ensure the enchantments have the same number of effects */
  if (pEffect1 == NULL && pEffect1 != NULL) return (false);
  if (pEffect1 != NULL && pEffect1 == NULL) return (false);
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CompareEnchantments()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void ComputeCurrentEffects (void);
 *
 * Computes any current effects for the current item using the current list
 * of valid suffics, prefixes, and uniques.
 *
 *=========================================================================*/
void CObriGenerate::ComputeCurrentEffects (void) {
  bool  HasPrefix  = false;
  bool  HasPrefix2 = false;
  bool  HasSuffix  = false;
  bool  IsUnique   = false;
  float Chance;
  dword Index;

	/* Get prefix chance */
  Chance = MakeRandItemRatio();
  if (Chance <= GetOptions().PrefixChance) HasPrefix = true;

  	/* Get suffix chance */
  Chance = MakeRandItemRatio();
  if (Chance <= GetOptions().SuffixChance) HasSuffix = true;

	/* Get unique chance */
  if (HasSuffix && HasPrefix) {
    Chance = MakeRandItemRatio();
    if (Chance <= GetOptions().UniqueChance) IsUnique = true;
   }

	/* Get prefix2 chance */
  if (HasPrefix) {
    Chance = MakeRandItemRatio();
    if (Chance <= GetOptions().PrefixChance) HasPrefix2 = true;
   }

	/* Clear current effect objects */
  m_GenerateData.pCurrentPrefix  = NULL;
  m_GenerateData.pCurrentPrefix2 = NULL;
  m_GenerateData.pCurrentSuffix  = NULL;
  m_GenerateData.pCurrentUnique  = NULL;
  m_GenerateData.HasEnchantment  = false;
    
	/* Ignore if the item isn't allowed to have any effects */
  if (m_GenerateData.pCurrentItem->EffectMask == OBRI_EFFTYPE_NONE) return;

	/* Select appropriate unique object */
  if (IsUnique) {
    Index = MakeRandItemIndex(m_ValidUniques.GetSize());
    m_GenerateData.pCurrentUnique = m_ValidUniques.GetAt(Index);
    HasPrefix  = false;
    HasPrefix2 = false;
    HasSuffix  = false;
   }

	/* Select random prefix object */
  if (HasPrefix) {
    Index = MakeRandItemIndex(m_ValidPrefixes.GetSize());
    m_GenerateData.pCurrentPrefix = m_ValidPrefixes.GetAt(Index);
   }

	/* Select random prefix2 object */
  if (HasPrefix2) {
    Index = MakeRandItemIndex(m_ValidPrefixes.GetSize());
    m_GenerateData.pCurrentPrefix2 = m_ValidPrefixes.GetAt(Index);
    CheckPrefix2();
   }

	/* Select random suffix object */
  if (HasSuffix) {
    if (HasPrefix) CheckValidSuffixes();

    if (m_ValidSuffixes.GetSize() == 0) {
      m_GenerateData.pCurrentSuffix = NULL;
      HasSuffix = false;
     }
    else {
      Index = MakeRandItemIndex(m_ValidSuffixes.GetSize());
      m_GenerateData.pCurrentSuffix = m_ValidSuffixes.GetAt(Index);
     }
   }
 
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ComputeCurrentEffects()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void ComputeCurrentEffects2 (void);
 *
 * Computes any current effects for the current item using the current list
 * of valid suffics, prefixes, and uniques.
 *
 *=========================================================================*/
void CObriGenerate::ComputeCurrentEffects2 (void) {
  bool  HasPrefix  = false;
  bool  HasPrefix2 = false;
  bool  HasSuffix  = false;
  bool  IsUnique   = false;
  float Chance;
  dword Index;

	/* Get prefix chance */
  Chance = MakeRandItemRatio();
  if (Chance <= GetOptions().PrefixChance) HasPrefix = true;

  	/* Get suffix chance */
  Chance = MakeRandItemRatio();
  if (Chance <= GetOptions().SuffixChance) HasSuffix = true;

	/* Get unique chance */
  if (HasSuffix && HasPrefix) {
    Chance = MakeRandItemRatio();
    if (Chance <= GetOptions().UniqueChance) IsUnique = true;
   }

	/* Get prefix2 chance */
  if (HasPrefix) {
    Chance = MakeRandItemRatio();
    if (Chance <= GetOptions().PrefixChance) HasPrefix2 = true;
   }

	/* Clear current effect objects */
  m_GenerateData.pCurrentPrefix  = NULL;
  m_GenerateData.pCurrentPrefix2 = NULL;
  m_GenerateData.pCurrentSuffix  = NULL;
  m_GenerateData.pCurrentUnique  = NULL;
  m_GenerateData.HasEnchantment  = false;
    
	/* Ignore if the item isn't allowed to have any effects */
  if (m_GenerateData.pCurrentItem->EffectMask == OBRI_EFFTYPE_NONE) return;

	/* Select appropriate unique object */
  if (IsUnique) {
    Index = MakeRandItemIndex(m_ValidUniques.GetSize());
    m_GenerateData.pCurrentUnique = m_ValidUniques.GetAt(Index);
    HasPrefix  = false;
    HasPrefix2 = false;
    HasSuffix  = false;
   }

	/* Select random prefix object */
  if (HasPrefix) {
    Index = MakeRandItemIndex(m_ValidPrefixes.GetSize());
    m_GenerateData.pCurrentPrefix = m_ValidPrefixes.GetAt(Index);
   }

	/* Select random prefix2 object */
  if (HasPrefix2) {
    Index = MakeRandItemIndex(m_ValidPrefixes.GetSize());
    m_GenerateData.pCurrentPrefix2 = m_ValidPrefixes.GetAt(Index);
    CheckPrefix2();
   }

	/* Select random suffix object */
  if (HasSuffix) {
    if (HasPrefix) CheckValidSuffixes();

    if (m_ValidSuffixes.GetSize() == 0) {
      m_GenerateData.pCurrentSuffix = NULL;
      HasSuffix = false;
     }
    else {
      Index = MakeRandItemIndex(m_ValidSuffixes.GetSize());
      m_GenerateData.pCurrentSuffix = m_ValidSuffixes.GetAt(Index);
     }
   }
 
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ComputeCurrentEffects2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void ComputeItemLevel (pItem);
 *
 * Estimates and returns an item level for the given item record (1-100).
 *
 *=========================================================================*/
int CObriGenerate::ComputeItemLevel (CObRecord* pItem) { 
  int ItemLevel;

  if (pItem->GetName() == OB_NAME_ARMO) {
    CObArmoRecord* pArmor = ObCastClass(CObArmoRecord, pItem);
    if (pArmor == NULL) return (0);

    ItemLevel = (int) (sqrt((float) pArmor->GetValue()) / 2);
   }
  else if (pItem->GetName() == OB_NAME_CLOT) {
    CObClotRecord* pClothing = ObCastClass(CObClotRecord, pItem);
    if (pClothing == NULL) return (0);

    ItemLevel = (int) (sqrt((float) pClothing->GetValue()) / 2);
   }
  else if (pItem->GetName() == OB_NAME_MISC) {
    CObMiscRecord* pMisc = ObCastClass(CObMiscRecord, pItem);
    if (pMisc == NULL) return (0);

    ItemLevel = (int) (sqrt((float) pMisc->GetValue()) / 2);
   }
  else if (pItem->GetName() == OB_NAME_BOOK) {
    CObBookRecord* pBook = ObCastClass(CObBookRecord, pItem);
    if (pBook == NULL) return (0);

    ItemLevel = (int) (sqrt((float) pBook->GetValue()) / 2);
   }
  else if (pItem->GetName() == OB_NAME_WEAP) {
    CObWeapRecord* pWeapon = ObCastClass(CObWeapRecord, pItem);
    if (pWeapon == NULL) return (0);

    ItemLevel = (int) (sqrt((float) pWeapon->GetValue()) / 2);
   }
  
  if (ItemLevel <= 0)   ItemLevel = 1;
  if (ItemLevel >= 100) ItemLevel = 100;
  return (ItemLevel);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ComputeItemLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void ComputeItemLevel (void);
 *
 * Uses the current settings and computes a new item level value, storing
 * the result in the options structure. Protected class method.
 *
 *=========================================================================*/
void CObriGenerate::ComputeItemLevel (void) {

  if (GetOptions().UseBell) {
    //m_GenerateData.CurrentItemLevel = (int)(MakeRandItemNumber((float)GetOptions().MinLevel, (float)GetOptions().MaxLevel, GetOptions().RandomBell / 100.0f * (GetOptions().MaxLevel - GetOptions().MinLevel) + GetOptions().MinLevel) + 0.5f);
    m_GenerateData.CurrentItemLevel = (int)(MakeRandItemGaussian((float)GetOptions().MinLevel, (float)GetOptions().MaxLevel, GetOptions().RandomBell) + 0.5f);
   }
  else
    m_GenerateData.CurrentItemLevel = (int)(MakeRandItemNumber((float)GetOptions().MinLevel, (float)GetOptions().MaxLevel) + 0.5f);

  m_GenerateData.CalcItemLevel = m_GenerateData.CurrentItemLevel;
  //SystemLog.Printf("Level = %d",  m_GenerateData.CurrentItemLevel);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ComputeItemLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void ComputeItemLevel2 (void);
 *
 *=========================================================================*/
void CObriGenerate::ComputeItemLevel2 (void) {

  //if (m_GenerateData.LastFailed) return;

  if (GetOptions().UseBell) {
    //m_GenerateData.CurrentItemLevel = (int)(MakeRandItemNumber((float)GetOptions().MinLevel, (float)GetOptions().MaxLevel, GetOptions().RandomBell / 100.0f * (GetOptions().MaxLevel - GetOptions().MinLevel) + GetOptions().MinLevel) + 0.5f);
    m_GenerateData.CurrentItemLevel = (int)(MakeRandItemGaussian((float)GetOptions().MinLevel, (float)GetOptions().MaxLevel, GetOptions().RandomBell) + 0.5f);
   }
  else
    m_GenerateData.CurrentItemLevel = (int)(MakeRandItemNumber((float)GetOptions().MinLevel, (float)GetOptions().MaxLevel) + 0.5f);

  m_GenerateData.MinItemLevel = m_GenerateData.CurrentItemLevel - OBRI_LEVEL_VARIANCE;
  m_GenerateData.MaxItemLevel = m_GenerateData.CurrentItemLevel + OBRI_LEVEL_VARIANCE;

  //SystemLog.Printf("\tComputeItemLevel2() = %d to %d",  m_GenerateData.MinItemLevel, m_GenerateData.MaxItemLevel);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ComputeItemLevel2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void ComputeQualityLevel (void);
 *
 *=========================================================================*/
void CObriGenerate::ComputeQualityLevel (void) {
  m_GenerateData.CurrentRatio = MakeRandItemNumber(0.0f, 1.0f);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ComputeQualityLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool MergeEspFiles (void);
 *
 * Merge all the initial ESP files as defined in the project file. Returns
 * false on any error.
 *
 *=========================================================================*/
bool CObriGenerate::MergeEspFiles (void) {
  bool  Result;

  m_HasMergedFile = false;

  if (m_ProjectFile.GetNumEspFiles() == 0) {
    m_LevelLists.CreateParents();
    return (true);
   }

  m_EspFile.Destroy();

	/* Load the first file */
  Result = m_EspFile.Load(m_ProjectFile.GetEspFile(0));
  m_HasMergedFile = true;

  if (!Result) {
    InitializeEspFile();
    m_LevelLists.CreateParents();
   }
  else {
    m_LevelLists.CreateParents();
    m_LevelLists.ResolveInsertItems();
    m_LevelLists.AddInsertItems();
   }
	
	/* TODO: Only merge the first file for now */
  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::MergeEspFiles()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CreateItems (DoFullReset);
 *
 * Creates all items according to the current settings.
 *
 *=========================================================================*/
bool CObriGenerate::CreateItems (const bool DoFullReset) {
  LARGE_INTEGER StartTime;
  LARGE_INTEGER EndTime;
  LARGE_INTEGER Freq;
  bool		Result;

  QueryPerformanceCounter(&StartTime);

	/* Reset the item generation */
  if (DoFullReset) {
    Reset();
   }
  else {
    PartialReset();
   }

	/* Prepare the initial merged files if required */
  if (DoFullReset || !m_HasMergedFile) {
    MergeEspFiles();
   }
  else {
    m_LevelLists.CreateParents();
   }
  
	/* Make all the random items depending on the generation type */
  if (GetOptions().MakeTotalItems) {
    Result = MakeCountItems(GetOptions().NumItems);
    if (!Result) return (false);
   }
  else {
    Result = MakeIndividualItems();
    if (!Result) return (false);
   }
 
	/* Distribute the leveled lists if required */
  if (GetOptions().DistributeLists) m_LevelLists.DistributeLists();

	/* Remove duplicates if required */
  if (GetOptions().RemoveDuplicates) {
    SystemLog.Printf("Reused %d duplicate enchantments. Ignored %d duplicate items.", m_NumDupEnchants, m_NumDupItems);
   }
  else if (GetOptions().CheckDuplicates) {    
    SystemLog.Printf("Found %d duplicate enchantments and %d duplicate items.", m_NumDupEnchants, m_NumDupItems);
   }

  QueryPerformanceCounter(&EndTime);
  QueryPerformanceFrequency(&Freq);
  
  SystemLog.Printf ("Created %u new items (%u failed), %d enchantments, %d level lists, and %d scripts in %f secs. Average level of %.1f.", 
	m_NumNewItems, m_NumFailedItems, m_NumEnchantments, GetNumLevelLists(), m_NumScripts, 
	(double) (EndTime.QuadPart - StartTime.QuadPart) / (double) Freq.QuadPart, (float)m_AverageLevelSum/(m_NumNewItems + 0.01f));

	/* Save the ESP file if required */
  if (m_ProjectFile.GetOutputEsp()[0] != NULL_CHAR) {

		/* Decide whether to save as a master or not */
    m_EspFile.GetHeader()->SetMaster(ObCheckExtension(m_ProjectFile.GetOutputEsp(), "ESM"));

    Result = m_EspFile.Save(m_ProjectFile.GetOutputEsp());
    if (Result) SystemLog.Printf("Saved items to '%s'.", m_ProjectFile.GetOutputEsp());
   }

  OutputLevelCounts("levelcounts.csv");
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CreateMoreItems (void);
 *
 * Creates more items according to the current settings.
 *
 *=========================================================================*/
bool CObriGenerate::CreateMoreItems (void) {
  return CreateItems(false);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateMoreItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateDamage (pRecord, pEffect);
 *
 * Adjusts the overall weapon's damage by a given percentage.
 *
 *=========================================================================*/
void CObriGenerate::CreateDamage (CObRecord* pRecord, obri_effect_t* pEffect) {
  CObWeapRecord* pWeapon;
  float	         Factor;
  float		 Damage;

	/* Ignore if the record isn't a weapon */
  pWeapon = ObCastClass(CObWeapRecord, pRecord);
  if (pWeapon == NULL) return;

  Factor = SplitValue(pEffect->MinEffect, pEffect->MaxEffect, m_GenerateData.CurrentRatio) / 100.0f;
  
	/* Modify weapon damages */
  Damage = pWeapon->GetDamage() * (Factor + 1.0f);
  if (Damage < 0)     Damage = 0;
  if (Damage > 65535) Damage = 65535;

  pWeapon->SetDamage((word) Damage);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateDamage()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateEnchantPts (pRecord, pEffect);
 *
 * Creates a enchant point % modification effect on the given record.
 *
 *=========================================================================*/
void CObriGenerate::CreateEnchantPts (CObRecord* pRecord, obri_effect_t* pEffect) {
  CObItem2Record* pItem2;
  float		  EnchantPts;
  float		  Factor;

	/* Ignore if the record doesn't have enchant points */
  pItem2 = ObCastClass(CObItem2Record, pRecord);
  if (pItem2 == NULL) return;
    
	/* Compute the new value */
  Factor = SplitValue(pEffect->MinEffect, pEffect->MaxEffect, m_GenerateData.CurrentRatio) / 100.0f;

  EnchantPts = pItem2->GetEnchantPoints() * (Factor + 1.0f);
  if (EnchantPts <= 0)     EnchantPts = 1;
  if (EnchantPts >  65535) EnchantPts = 65535;

  pItem2->SetEnchantPoints((word) EnchantPts);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateEnchantPts()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateWeight (pRecord, pEffect);
 *
 * Creates a weight % modification effect on the given record.
 *
 *=========================================================================*/
void CObriGenerate::CreateWeight (CObRecord* pRecord, obri_effect_t* pEffect) {
  CObWeapRecord* pWeapon;
  CObArmoRecord* pArmor;
  CObClotRecord* pClothing;
  CObLighRecord* pLight;
  CObMiscRecord* pMisc;
  CObSgstRecord* pSigil;
  CObBookRecord* pBook;
  float          Weight;
  float	         Factor;

  if (pRecord->GetName() == OB_NAME_ARMO) { 
    pArmor = ObCastClass(CObArmoRecord, pRecord);
    if (pArmor == NULL) return;
    Weight = pArmor->GetWeight();
   }
  else if (pRecord->GetName() == OB_NAME_BOOK) {
    pBook = ObCastClass(CObBookRecord, pRecord);
    if (pBook == NULL) return;
    Weight = pBook->GetWeight();
   }
  else if (pRecord->GetName() == OB_NAME_CLOT) {
    pClothing = ObCastClass(CObClotRecord, pRecord);
    if (pClothing == NULL) return;
    Weight = pClothing->GetWeight();
   }
  else if (pRecord->GetName() == OB_NAME_LIGH) {
    pLight = ObCastClass(CObLighRecord, pRecord);
    if (pLight == NULL) return;
    Weight = pLight->GetWeight();
   }
  else if (pRecord->GetName() == OB_NAME_MISC) {
    pMisc = ObCastClass(CObMiscRecord, pRecord);
    if (pMisc == NULL) return;
    Weight = pMisc->GetWeight();
   }
  else if (pRecord->GetName() == OB_NAME_SGST) {
    pSigil = ObCastClass(CObSgstRecord, pRecord);
    if (pSigil == NULL) return;
    Weight = pSigil->GetWeight();
   }
  else if (pRecord->GetName() == OB_NAME_WEAP) {
    pWeapon = ObCastClass(CObWeapRecord, pRecord);
    if (pWeapon == NULL) return;
    Weight = pWeapon->GetWeight();
   }
  else
    return;
  
	/* Compute the new weight */
  Factor = SplitValue(pEffect->MinEffect, pEffect->MaxEffect, m_GenerateData.CurrentRatio) / 100.0f;
  Weight *= (Factor + 1.0f);
  if (Weight <= 0) Weight = 0.1f;

  	/* Round the value to the desired accuracy */
  Weight = OBRI_ROUND_WEIGHT * (int)(Weight / OBRI_ROUND_WEIGHT);

	/* Set weight */
  if (pRecord->GetName() == OB_NAME_ARMO) { 
    pArmor->SetWeight(Weight);
   }
  else if (pRecord->GetName() == OB_NAME_BOOK) {
    pBook->SetWeight(Weight);
   }
  else if (pRecord->GetName() == OB_NAME_CLOT) {
    pClothing->SetWeight(Weight);
   }
  else if (pRecord->GetName() == OB_NAME_LIGH) {
    pLight->SetWeight(Weight);
   }
  else if (pRecord->GetName() == OB_NAME_MISC) {
    pMisc->SetWeight(Weight);
   }
  else if (pRecord->GetName() == OB_NAME_SGST) {
    pSigil->SetWeight(Weight);
   }
  else if (pRecord->GetName() == OB_NAME_WEAP) {
    pWeapon->SetWeight(Weight);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateWeight()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateReach (pRecord, pEffect);
 *
 * Creates a reach % modification effect on the given record.
 *
 *=========================================================================*/
void CObriGenerate::CreateReach (CObRecord* pRecord, obri_effect_t* pEffect) {
  CObWeapRecord* pWeapon;
  float	         Factor;
  float		 Reach;

	/* Ignore if the record isn't a weapon */
  pWeapon = ObCastClass(CObWeapRecord, pRecord);
  if (pWeapon == NULL) return;

  Factor = SplitValue(pEffect->MinEffect, pEffect->MaxEffect, m_GenerateData.CurrentRatio) / 100.0f;
  Reach = pWeapon->GetReach() * (Factor + 1.0f);
  if (Reach < 0) Reach = 1;

  	/* Round the value to the desired accuracy */
  Reach = OBRI_ROUND_REACH * (int)(Reach / OBRI_ROUND_REACH);

  pWeapon->SetReach(Reach);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateReach()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateSpeed (pRecord, pEffect);
 *
 * Creates a spee % modification effect on the given record.
 *
 *=========================================================================*/
void CObriGenerate::CreateSpeed (CObRecord* pRecord, obri_effect_t* pEffect) {
  CObWeapRecord* pWeapon;
  float	         Factor;
  float		 Speed;

	/* Ignore if the record isn't a weapon */
  pWeapon = ObCastClass(CObWeapRecord, pRecord);
  if (pWeapon == NULL) return;

  Factor = SplitValue(pEffect->MinEffect, pEffect->MaxEffect, m_GenerateData.CurrentRatio) / 100.0f;
  Speed = pWeapon->GetSpeed() * (Factor + 1.0f);
  if (Speed < 0) Speed = 0;

	/* Round the value to the desired accuracy */
  Speed = OBRI_ROUND_SPEED * (int)(Speed / OBRI_ROUND_SPEED);

  pWeapon->SetSpeed(Speed);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateSpeed()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateHealth (pRecord, pEffect);
 *
 * Creates a health % modification effect on the given record.
 *
 *=========================================================================*/
void CObriGenerate::CreateHealth (CObRecord* pRecord, obri_effect_t* pEffect) {
  CObWeapRecord* pWeapon;
  CObArmoRecord* pArmor;
  float		 Health;
  float		 Factor;

	/* Get the current record condition */
  if (pRecord->GetName() == OB_NAME_ARMO) { 
    pArmor = ObCastClass(CObArmoRecord, pRecord);
    if (pArmor == NULL) return;
    Health = (float) pArmor->GetHealth();
   }
  else if (pRecord->GetName() == OB_NAME_WEAP) {
    pWeapon = ObCastClass(CObWeapRecord, pRecord);
    if (pWeapon == NULL) return;
    Health = (float)  pWeapon->GetHealth();
   }
  else
    return;
  
	/* Compute the new condition */
  Factor = SplitValue(pEffect->MinEffect, pEffect->MaxEffect, m_GenerateData.CurrentRatio) / 100.0f;

  Health *= Factor + 1.0f;
  if (Health < 0) Health = 0;

	/* Set the item condition */
  if (pRecord->GetName() == OB_NAME_ARMO) { 
    pArmor->SetHealth((dword) Health);
   }
  else if (pRecord->GetName() == OB_NAME_WEAP) {
    pWeapon->SetHealth((dword) Health);
   }
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateCondition()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateArmor (pRecord, pEffect);
 *
 * Adjusts the armor's AC value by a given percentage.
 *
 *=========================================================================*/
void CObriGenerate::CreateArmor (CObRecord* pRecord, obri_effect_t* pEffect) {
  CObArmoRecord* pArmor;
  float	     Armor;
  float	     Factor;

	/* Ignore if the record isn't a armor */
  pArmor = ObCastClass(CObArmoRecord, pRecord);
  if (pArmor == NULL) return;

  Factor = SplitValue(pEffect->MinEffect, pEffect->MaxEffect, m_GenerateData.CurrentRatio) / 100.0f;
  
	/* Modify armor class */
  Armor = pArmor->GetArmor() * (Factor + 1.0f);
  if (Armor < 0)     Armor = 0;
  if (Armor > 65535) Armor = 65535;

  pArmor->SetArmor((word) Armor);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateArmor()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateValue (pRecord, pEffect);
 *
 * Adjusts the items base value by a given percentage.
 *
 *=========================================================================*/
void CObriGenerate::CreateValue (CObRecord* pRecord, obri_effect_t* pEffect) {
  float Factor;
  
  Factor  = SplitValue(pEffect->MinEffect, pEffect->MaxEffect, m_GenerateData.CurrentRatio) / 100.0f;
  Factor += 1.0f;
  
	/* Modify base item price */
  if (Factor <= 0)
    m_GenerateData.ItemValue = 0;
  else
    m_GenerateData.ItemValue = (dword) (m_GenerateData.ItemValue * Factor);

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateValue()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateScript (pRecord, pEffect);
 *
 * Creates a script effect by adding a script to the record.
 *
 *=========================================================================*/
void CObriGenerate::CreateScript (CObRecord* pRecord, obri_effect_t* pEffect) {
  CObItem1Record* pItem1;
  CObBaseRecord*  pBaseRecord;
  CObScptRecord*  pScript;

	/* Ensure the record is the correct type to accept a script */
  pItem1 = ObCastClass(CObItem1Record, pRecord);
  if (pItem1 == NULL) return;

	/* Ensure a valid script editor ID */
  if (pEffect->Script[0] == NULL_CHAR) {
    AddObGeneralError("No script editor ID specified for effect %s!", pEffect->NameID);
    return;
   }

	/* Find the script formid in the current file */
  pBaseRecord = m_EspFile.FindEditorID(pEffect->Script);

  if (pBaseRecord == NULL) {
    AddObGeneralError("The script '%s' used in effect %s does not exist!", pEffect->Script, pEffect->NameID);
    return;
   }

  pScript = ObCastClass(CObScptRecord, pBaseRecord);

  if (pScript == NULL) {
    AddObGeneralError("The record '%s' used in effect %s is not a script!", pEffect->Script, pEffect->NameID);
   }
  else {
    if (pScript->GetType() != OB_SCRIPTTYPE_OBJECT) {
      AddObGeneralError("The script '%s' used in effect %s is not an object!", pEffect->Script, pEffect->NameID);
     }

    pItem1->SetScript(pScript->GetFormID());
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateScript()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateEffect (pRecord, pEffect, ppEnchant, ForceUnique);
 *
 * Creates and adds the given effect to the record. Creates the enchantment
 * object only if required.
 *
 *=========================================================================*/
void CObriGenerate::CreateEffect (CObRecord* pRecord, obri_effect_t* pEffect,
				  CObEnchRecord** ppEnchant, const bool ForceUnique) {

	/* Add the effect to the object */
  switch (pEffect->EffectType) {
    case OBRI_EFFTYPE_NONE:
	break;
    case OBRI_EFFTYPE_VALUE:
        CreateValue(pRecord, pEffect);
	break;
    case OBRI_EFFTYPE_ENCHANT:
        CreateEnchantment(pRecord, pEffect, ppEnchant, ForceUnique);
	break;
    case OBRI_EFFTYPE_WEIGHT:
        CreateWeight(pRecord, pEffect);
	break;
    case OBRI_EFFTYPE_DAMAGE:
        CreateDamage(pRecord, pEffect);
	break;
    case OBRI_EFFTYPE_REACH:
	CreateReach(pRecord, pEffect);
	break;
    case OBRI_EFFTYPE_SPEED:
	CreateSpeed(pRecord, pEffect);
	break;
    case OBRI_EFFTYPE_HEALTH:
	CreateHealth(pRecord, pEffect);
	break;
    case OBRI_EFFTYPE_ENCHANTPTS:
	CreateEnchantPts(pRecord, pEffect);
	break;
    case OBRI_EFFTYPE_ARMOR:
    	CreateArmor(pRecord, pEffect);
	break;
    case OBRI_EFFTYPE_SCRIPT:
    	CreateScript(pRecord, pEffect);
	break;
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateSigilEnchant (pSigil, pEffect);
 *
 * Adds an enchantment effect to the sigil stone item.
 *
 *=========================================================================*/
void CObriGenerate::CreateSigilEnchant (CObSgstRecord* pSigil, obri_effect_t* pEffect) {
  dword Index;

	/* Ensure valid input */
  if (pSigil == NULL) return;

	/* Add individual effects */
  for (Index = 0; Index < pEffect->Enchantment.NumEffects; ++Index) {
    AddEnchantEffect(pSigil, *pEffect, pEffect->Enchantment.Effects[Index]);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateSigilEnchant()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void CreateEnchantment (pRecord, pEffect, ppEnchant, ForceUnique);
 *
 * Creates and adds the given enchantment effect to the record. Creates the
 * enchantment object only if required.
 *
 *=========================================================================*/
void CObriGenerate::CreateEnchantment (CObRecord* pRecord, obri_effect_t* pEffect,
				       CObEnchRecord** ppEnchant, const bool ForceUnique) {
  CObEnchRecord*  pDupEnchant;
  CObItem2Record* pItem2;
  dword		  Index;
  long		  Charge;
  long		  Cost;
  bool		  FoundDupEnchant = false;

  	/* Special case for a sigil stone */
  if (pRecord->GetName() == OB_NAME_SGST) {
    *ppEnchant = NULL;
    CreateSigilEnchant(ObCastClass(CObSgstRecord, pRecord), pEffect);
    return;
   }

	/* Re-use duplicate enchantments that are identical if required */
  if (!ForceUnique && (GetOptions().RemoveDuplicates || GetOptions().CheckDuplicates)) {
    *ppEnchant = m_EnchantMap.FindEnchantment(pEffect, m_GenerateData.CurrentRatio);

    if (*ppEnchant != NULL) {
      ++m_NumDupEnchants;

      if (GetOptions().RemoveDuplicates) { 
        m_GenerateData.HasEnchantment = false;
        pItem2 = ObCastClass(CObItem2Record, pRecord);
	if (pItem2 == NULL) return;

		/* Assign the enchantment to the record */
        pItem2->SetEnchantment((*ppEnchant)->GetFormID());
        pItem2->SetEnchantPoints((*ppEnchant)->GetCharge());
        return;
       }

      FoundDupEnchant = true;
      pDupEnchant     = *ppEnchant;
      *ppEnchant      = NULL;
     } 
   }
	
	/* Allocate and initialize the enchantment record if required */
  if (*ppEnchant == NULL) MakeEnchantment(pRecord, ppEnchant);
  if (*ppEnchant == NULL) return;
  //(*ppEnchant)->SetUserData(m_GenerateData.CurrentItemLevel);
  (*ppEnchant)->SetUserData(pEffect->ItemLevel);

	/* Set the basic enchantment settings */
  Charge = SplitValue(pEffect->Enchantment.MinCharge, pEffect->Enchantment.MaxCharge, m_GenerateData.CurrentRatio);
  Cost   = SplitValue(pEffect->Enchantment.MinCost,   pEffect->Enchantment.MaxCost,   m_GenerateData.CurrentRatio);

	/* Set the modified enchantment charge */
  Charge = Charge*Cost + (*ppEnchant)->GetCharge();
  if (Charge < 0) Charge = 0;
  (*ppEnchant)->SetCharge(Charge);

	/* Set the modified enchantment cost */
  Cost += (*ppEnchant)->GetCost();
  if (Cost < 0) Cost = 0;
  (*ppEnchant)->SetCost(Cost);

	/* For weapons we need to set the enchantment charge */
  if (pRecord->GetName() == OB_NAME_WEAP) {
    CObWeapRecord* pWeapon;

    pWeapon = ObCastClass(CObWeapRecord, pRecord);
    if (pWeapon != NULL) pWeapon->SetEnchantPoints((word)Charge);
   }
  
	/* Set other enchantment parameters */
  (*ppEnchant)->SetType(pEffect->Enchantment.EnchantType);
  (*ppEnchant)->SetAutoCalc(pEffect->Enchantment.AutoCalc);

	/* Add individual effects */
  for (Index = 0; Index < pEffect->Enchantment.NumEffects; ++Index) {
    AddEnchantEffect(*ppEnchant, *pEffect, pEffect->Enchantment.Effects[Index]);
   }

  if (!FoundDupEnchant) {
    m_EnchantMap.AddEnchantment(pEffect, m_GenerateData.CurrentRatio, *ppEnchant);
   }
  else if (GetOptions().CheckDuplicates) {
    SystemLog.Printf("%d) Duplicate enchantment: %s == %s", m_NumDupEnchants, (*ppEnchant)->GetEditorID(), pDupEnchant->GetEditorID());
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CreateItem (CheckDups);
 *
 * Creates the actual item record according to the current settings.
 * Returns false if the item was not created.
 *
 *=========================================================================*/
bool CObriGenerate::CreateItem (const bool CheckDups) {
  CObRecord* pNewRecord;
  CObRecord* pDupRecord;
  bool	     DuplicateItem = false;

	/* Check for duplicate items */  
#if OBRI_OLD_ITEMMAP
  if (CheckDups && (GetOptions().CheckDuplicates || GetOptions().RemoveDuplicates)) {
    pDupRecord = m_ItemMap.FindItem(m_GenerateData.pCurrentItem, m_GenerateData.pCurrentPrefix, 
				    m_GenerateData.pCurrentPrefix2, m_GenerateData.pCurrentSuffix, 
				    m_GenerateData.pCurrentUnique, m_GenerateData.CurrentRatio,
				    m_GenerateData.CurrentItemLevel, NULL);

    if (pDupRecord != NULL) {
      ++m_NumDupItems;

      if (GetOptions().RemoveDuplicates) {
        ++m_NumFailedItems;
        return (false);
       }
    
      DuplicateItem = true;
     }
   } 
#endif

	/* Create the new record and add it to the array */
  pNewRecord = CreateNewRecord();
  if (pNewRecord == NULL) return (false);

#if OBRI_OLD_ITEMMAP
  if (CheckDups && !DuplicateItem) {
    m_ItemMap.AddItem(m_GenerateData.pCurrentItem, m_GenerateData.pCurrentPrefix, 
			    m_GenerateData.pCurrentPrefix2, m_GenerateData.pCurrentSuffix, 
			    m_GenerateData.pCurrentUnique, m_GenerateData.CurrentRatio, m_GenerateData.CurrentItemLevel, pNewRecord);
   } 
#endif

	/* Set the item level */  
  pNewRecord->SetUserData(m_GenerateData.CalcItemLevel);

	/* Set the new record parameters */
  SetRecordItemParams(pNewRecord);
  SetRecordEffects(pNewRecord);

#if OBRI_OLD_ITEMMAP

	/* Output duplicate item message if required */
  if (CheckDups && DuplicateItem && GetOptions().CheckDuplicates) {
    CObIdRecord* pIdRecord1 = ObCastClass(CObIdRecord, pNewRecord);
    CObIdRecord* pIdRecord2 = ObCastClass(CObIdRecord, pDupRecord);

    if (pIdRecord1 != NULL && pIdRecord2 != NULL) {
      SystemLog.Printf("%d) Duplicate item: %s == %s", m_NumDupItems, pIdRecord1->GetEditorID(), pIdRecord2->GetEditorID());
     }
   } 

#else

  if (CheckDups && (GetOptions().CheckDuplicates || GetOptions().RemoveDuplicates)) {
    pDupRecord = m_ItemMap.FindItem(m_GenerateData.pCurrentItem, m_GenerateData.pCurrentPrefix, 
				    m_GenerateData.pCurrentPrefix2, m_GenerateData.pCurrentSuffix, 
				    m_GenerateData.pCurrentUnique, m_GenerateData.CurrentRatio,
				    m_GenerateData.CurrentItemLevel, pNewRecord);

    if (pDupRecord != NULL) {
      ++m_NumDupItems; 

      if (GetOptions().CheckDuplicates) {
        CObIdRecord* pIdRecord1 = ObCastClass(CObIdRecord, pNewRecord);
        CObIdRecord* pIdRecord2 = ObCastClass(CObIdRecord, pDupRecord);

        if (pIdRecord1 != NULL && pIdRecord2 != NULL) {
          SystemLog.Printf("%d) Duplicate item: %s == %s", m_NumDupItems, pIdRecord1->GetEditorID(), pIdRecord2->GetEditorID());
         }
       }
      else {
        m_EspFile.DeleteRecord(pNewRecord);
	++m_NumFailedItems;
        return (false);
       }

     }
    else {
      m_ItemMap.AddItem(m_GenerateData.pCurrentItem, m_GenerateData.pCurrentPrefix, 
			    m_GenerateData.pCurrentPrefix2, m_GenerateData.pCurrentSuffix, 
			    m_GenerateData.pCurrentUnique, m_GenerateData.CurrentRatio,
			    m_GenerateData.CurrentItemLevel, pNewRecord);
     }
   }

#endif

  m_NewRecords.Add(pNewRecord);
  if (m_GenerateData.HasEnchantment) ++m_NumEnchantments;
  ++m_NumNewItems;

  if (m_GenerateData.CalcItemLevel >= 0 && m_GenerateData.CalcItemLevel <= OBRI_MAX_LEVELS) ++m_LevelCounts[m_GenerateData.CalcItemLevel];
  m_AverageLevelSum += m_GenerateData.CalcItemLevel;
  
		/* Add the item to the level lists */
  AddLevelListItem(pNewRecord);

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CreateItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - obri_itemkey_t* FindItemKey (pIdRecord);
 *
 *=========================================================================*/
obri_itemkey_t* CObriGenerate::FindItemKey (CObIdRecord* pIdRecord) {
  return m_ItemMap.FindItem(pIdRecord);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::FindItemKey()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool IsNegativeEffect (pEffect);
 *
 *=========================================================================*/
bool CObriGenerate::IsNegativeEffect (obri_effect_t* pEffect) {
  if (pEffect == NULL) return (false);
  return (pEffect->Multiplier < 0);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::IsNegativeEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - int GetCurrentItemLevel (void);
 *
 * Gets the item level for the current item. Uses the maximum level of
 * item and any effects.
 *
 *=========================================================================*/
int CObriGenerate::GetCurrentItemLevel (void) {
  int   MaxPositiveLevel = 0;
  int   MaxNegativeLevel = 0;
  int   MaxLevel         = 0;
  int   PositiveLevels   = 0;
  int   NegativeLevels   = 0;
  float LevelMod;

	/* Factor in the item level */
  if (m_GenerateData.pCurrentItem != NULL) {
    if (m_GenerateData.pCurrentItem->ItemLevel > MaxPositiveLevel) MaxPositiveLevel = m_GenerateData.pCurrentItem->ItemLevel;

    if (m_GenerateData.pCurrentItem->ItemLevel > 0)
      PositiveLevels += m_GenerateData.pCurrentItem->ItemLevel;
    else
      NegativeLevels += -m_GenerateData.pCurrentItem->ItemLevel;
   }

	/* Factor in the first prefix effect level */
  if (m_GenerateData.pCurrentPrefix != NULL) {

    if (IsNegativeEffect(m_GenerateData.pCurrentPrefix)) {
      if (m_GenerateData.pCurrentPrefix->ItemLevel > MaxNegativeLevel) {
        NegativeLevels  -= MaxNegativeLevel;
        MaxNegativeLevel = m_GenerateData.pCurrentPrefix->ItemLevel;
       }

      NegativeLevels += m_GenerateData.pCurrentPrefix->ItemLevel;
     }
    else {
      if (m_GenerateData.pCurrentPrefix->ItemLevel > MaxPositiveLevel) {
        PositiveLevels  -= MaxPositiveLevel;
        MaxPositiveLevel = m_GenerateData.pCurrentPrefix->ItemLevel;
       }

      PositiveLevels += m_GenerateData.pCurrentPrefix->ItemLevel;
     }
   }

	/* Factor in the second prefix effect level */
  if (m_GenerateData.pCurrentPrefix2 != NULL) {

   if (IsNegativeEffect(m_GenerateData.pCurrentPrefix2)) {
      if (m_GenerateData.pCurrentPrefix2->ItemLevel > MaxNegativeLevel) {
        NegativeLevels  -= MaxNegativeLevel;
        MaxNegativeLevel = m_GenerateData.pCurrentPrefix2->ItemLevel;
       }

      NegativeLevels += m_GenerateData.pCurrentPrefix2->ItemLevel;
     }
    else {
      if (m_GenerateData.pCurrentPrefix2->ItemLevel > MaxPositiveLevel) {
        PositiveLevels  -= MaxPositiveLevel;
        MaxPositiveLevel = m_GenerateData.pCurrentPrefix2->ItemLevel;
       }

      PositiveLevels += m_GenerateData.pCurrentPrefix2->ItemLevel;
     }
   }

	/* Factor in the suffix effect level */
  if (m_GenerateData.pCurrentSuffix != NULL) {

    if (IsNegativeEffect(m_GenerateData.pCurrentSuffix)) {
      if (m_GenerateData.pCurrentSuffix->ItemLevel > MaxNegativeLevel) {
        NegativeLevels  -= MaxNegativeLevel;
        MaxNegativeLevel = m_GenerateData.pCurrentSuffix->ItemLevel;
       }

      NegativeLevels += m_GenerateData.pCurrentSuffix->ItemLevel;
     }
    else {
      if (m_GenerateData.pCurrentSuffix->ItemLevel > MaxPositiveLevel) {
        PositiveLevels  -= MaxPositiveLevel;
        MaxPositiveLevel = m_GenerateData.pCurrentSuffix->ItemLevel;
       }

      PositiveLevels += m_GenerateData.pCurrentSuffix->ItemLevel;
     }
   }

	/* Factor in the unique effect level */
  if (m_GenerateData.pCurrentUnique != NULL) {

    if (m_GenerateData.pCurrentUnique->ItemLevel > PositiveLevels) {
      PositiveLevels  -= MaxNegativeLevel;
      MaxPositiveLevel = m_GenerateData.pCurrentUnique->ItemLevel;
     }
   }
	
	/* Choose the highest level found */
  if (MaxNegativeLevel > MaxPositiveLevel) {
    MaxLevel = MaxPositiveLevel;
   }
  else {  
    PositiveLevels -= MaxPositiveLevel;
    MaxLevel        = MaxPositiveLevel;
   }

	/* Compute the item level modification */
  LevelMod = PositiveLevels * GetOptions().LevelFactor - (float) NegativeLevels;
  if (LevelMod > 0.4) MaxLevel = MaxLevel + (int) LevelMod;
  if (MaxLevel <   0) MaxLevel = 0;

  //SystemLog.Printf("Level Max = %d / %d,   Levels = %d / %d,    Mod = %g", MaxNegativeLevel, MaxPositiveLevel, NegativeLevels, PositiveLevels, LevelMod);
	
  return (MaxLevel);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::GetCurrentItemLevel()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void DestroyNewRecords (void);
 *
 * Delete all records currently in the new records array.
 *
 *=========================================================================*/
void CObriGenerate::DestroyNewRecords (void) {
  m_NewRecords.Destroy();
  m_NewEnchantments.Destroy();
  m_EnchantMap.Destroy();
  m_ItemMap.Destroy();
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::DestroyNewRecords()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportItemData (pFilename, pOutputFile);
 *
 * Attempt to export unenchanted items from the given ESM/ESP file to a
 * base item DAT text file. Uses the current settings to determine which
 * items to export.  Returns false on any error.
 *
 *=========================================================================*/
bool CObriGenerate::ExportItemData (const SSCHAR* pFilename, const SSCHAR* pOutputFile) {
  CObEspFile InputFile;
  bool       Result;

	/* Attempt to input the given ESM/ESP file */
  Result = InputFile.Load(pFilename);
  if (!Result) return (false);

	/* Attempt to open the output file */
  return ExportItemData(InputFile, pOutputFile);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportItemData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportItemData (File, pOutputFile);
 *
 * Attempt to export unenchanted items from the given ESM/ESP file to a
 * base item DAT text file. Uses the current settings to determine which
 * items to export.  Returns false on any error.
 *
 *=========================================================================*/
bool CObriGenerate::ExportItemData (CObEspFile& InputFile, const SSCHAR* pOutputFile) {
  CObFile        OutputFile;
  CObBaseRecord* pRecord;
  dword	         RecordPos;
  bool           Result;

	/* Attempt to open the output file */
  Result = OutputFile.Open(pOutputFile, "wt");
  if (!Result) return (false);
  m_ExportCount = 0;

	/* Output all records as required */
  pRecord = InputFile.GetFirstRecord(RecordPos);

  while (pRecord != NULL) {
  
		/* Output record by object type */
    if (pRecord->GetName() == OB_NAME_ARMO && (GetOptions().ItemMask & OBRI_ITEMTYPE_ARMOR) != 0) {
      Result = ExportArmor(OutputFile, ObCastClass(CObArmoRecord, pRecord));
      if (!Result) return (false);
     }
    else if (pRecord->GetName() == OB_NAME_WEAP) {
      CObWeapRecord* pWeapon = ObCastClass(CObWeapRecord, pRecord);

      if (pWeapon->GetType() == OB_WEAPTYPE_STAFF) {
        if ((GetOptions().ItemMask & OBRI_ITEMTYPE_STAFF) != 0) {
          Result = ExportWeapon(OutputFile, pWeapon);
          if (!Result) return (false);
         }
       }
      else if ((GetOptions().ItemMask & OBRI_ITEMTYPE_WEAPON) != 0) {
        Result = ExportWeapon(OutputFile, pWeapon);
        if (!Result) return (false);
       }
     }
    else if (pRecord->GetName() == OB_NAME_CLOT && (GetOptions().ItemMask & OBRI_ITEMTYPE_CLOTHING) != 0) {
      Result = ExportClothing(OutputFile, ObCastClass(CObClotRecord, pRecord));
      if (!Result) return (false);
     }
    else if (pRecord->GetName() == OB_NAME_MISC && (GetOptions().ItemMask & OBRI_ITEMTYPE_MISC) != 0) {
      Result = ExportMisc(OutputFile, ObCastClass(CObMiscRecord, pRecord));
      if (!Result) return (false);
     }
    else if (pRecord->GetName() == OB_NAME_BOOK && (GetOptions().ItemMask & OBRI_ITEMTYPE_SCROLL) != 0) {
      Result = ExportBook(OutputFile, ObCastClass(CObBookRecord, pRecord));
      if (!Result) return (false);
     }

    pRecord = InputFile.GetNextRecord(RecordPos);
   }

  SystemLog.Printf("ExportCount = %ld", m_ExportCount);
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportItemData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportLevelLists (File, pOutputFile);
 *
 * Attempt to export level list data from the given ESM/ESP file to a
 * CSV text file.   Returns false on any error.
 *
 *=========================================================================*/
bool CObriGenerate::ExportLevelLists (CObEspFile& InputFile, const SSCHAR* pOutputFile) {
  CObFile        OutputFile;
  CObBaseRecord* pRecord;
  CObLvliRecord* pLevelList;
  dword	         Index;
  bool           Result;
  dword		 Count = 0;

	/* Attempt to open the output file */
  Result = OutputFile.Open(pOutputFile, "wt");
  if (!Result) return (false);
  Count = 0;

	/* Output all records as required */
  CObTypeGroup* pListGroup = InputFile.GetTypeGroup(OB_NAME_LVLI);
  if (pListGroup == NULL) return (false);

  for (Index = 0; Index < pListGroup->GetNumRecords(); ++Index) {
    pRecord = pListGroup->GetRecord(Index);
    if (pRecord == NULL) continue;
  
		/* Output record by object type */
    if (pRecord->GetName() == OB_NAME_LVLI) {
      pLevelList = ObCastClass(CObLvliRecord, pRecord);
      if (pLevelList != NULL) ExportLevelList(OutputFile, InputFile, pLevelList);
      ++Count;
     }
   }

  SystemLog.Printf("Export Level List Count = %ld", Count);
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportLevelLists()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportLevelList (OutputFile, InputFile, pLevelList);
 *
 * Export a level list record to the given CSV file in the format:
 *
 *	ListID, ItemID, ItemLevel, ItemCount
 *
 *=========================================================================*/
bool CObriGenerate::ExportLevelList (CObFile& OutputFile, CObEspFile& InputFile, CObLvliRecord* pLevelList) {
  CObLvloSubrecord* pItem;
  CObBaseRecord*    pRecord;
  CObRecord*        pRecord1;
  CObIdRecord*      pIdRecord;
  int		    ItemPos;

  pItem = pLevelList->GetFirstItem(ItemPos);

  while (pItem != NULL) {
    pRecord = InputFile.FindFormID(pItem->GetFormID());

    if (pRecord != NULL) {
      pIdRecord = ObCastClass(CObIdRecord, pRecord);

      if (pIdRecord != NULL) 
        OutputFile.Printf("%s, %s, %d, %d\n", pLevelList->GetEditorID(), pIdRecord->GetEditorID(), pItem->GetLevel(), pItem->GetCount());
      else {
        pRecord1 = ObCastClass(CObRecord, pRecord);

	if (pRecord1 != NULL) {
	  CObSubrecord* pSubrecord = pRecord1->FindSubrecord(OB_NAME_EDID);

	  if (pSubrecord != NULL) {
	    SSCHAR Buffer[128];

	    if (pSubrecord->GetRecordSize() < 100)
	      strnncpy(Buffer, (char *)pSubrecord->GetData(), pSubrecord->GetRecordSize());
            else
	      strnncpy(Buffer, (char *)pSubrecord->GetData(), 100);

	    OutputFile.Printf("%s, %s, %d, %d\n", pLevelList->GetEditorID(), Buffer, pItem->GetLevel(), pItem->GetCount());
	   }
          else {
	   }
	 }
	else {
          AddObGeneralError("Record 0x%08X is not an ID or regular record!", pRecord->GetFormID());
         }
       }
     }
    else {
      AddObGeneralError("Failed to find the editor ID for record 0x%08X!", pRecord->GetFormID());
     }

    pItem = pLevelList->GetNextItem(ItemPos);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportLevelList()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportSounds (File, pOutputFile);
 *
 * Attempt to export sound data from the given ESM/ESP file to a
 * CSV text file.   Returns false on any error.
 *
 *=========================================================================*/
bool CObriGenerate::ExportSounds (CObEspFile& InputFile, const SSCHAR* pOutputFile) {
  CObFile        OutputFile;
  CObBaseRecord* pRecord;
  CObIdRecord*   pSound;
  dword	         Index;
  bool           Result;
  dword		 Count = 0;

	/* Attempt to open the output file */
  Result = OutputFile.Open(pOutputFile, "wt");
  if (!Result) return (false);
  Count = 0;

	/* Output all records as required */
  CObTypeGroup* pListGroup = InputFile.GetTypeGroup(OB_NAME_SOUN);
  if (pListGroup == NULL) return (false);

  for (Index = 0; Index < pListGroup->GetNumRecords(); ++Index) {
    pRecord = pListGroup->GetRecord(Index);
    if (pRecord == NULL) continue;
  
		/* Output record by object type */
    if (pRecord->GetName() == OB_NAME_SOUN) {
      pSound = ObCastClass(CObIdRecord, pRecord);

      if (pSound != NULL) {
        OutputFile.Printf("%s, 0x%08X\n", pSound->GetEditorID(), pSound->GetFormID());
        ++Count;
       }
     }
   }

  SystemLog.Printf("Export Sound Count = %ld", Count);
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportSounds()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportArmor (OutputFile, pArmor);
 *
 * Output armor data record to a text base item file. Returns false on any
 * error. Protected class method.
 *
 *=========================================================================*/
bool CObriGenerate::ExportArmor (CObFile& OutputFile, CObArmoRecord* pArmor) {

	/* Only output unenchanted armor pieces */
  if (pArmor == NULL) return (false);
  if (pArmor->GetEnchantment() != OB_FORMID_NULL) return (true);

  OutputFile.Printf("Item\n");
  OutputFile.Printf("\tItemType = Armor\n");
  OutputFile.Printf("\tName = \"%s\"\n", pArmor->GetItemName());
  OutputFile.Printf("\tMIcon = \"%s\"\n", pArmor->GetIcon());
  if (pArmor->HasIcon2()) OutputFile.Printf("\tFIcon = \"%s\"\n", pArmor->GetIcon2());
  OutputFile.Printf("\tMBModel = \"%s\"\n", pArmor->GetModel());
  if (pArmor->HasModel2()) OutputFile.Printf("\tMWModel = \"%s\"\n", pArmor->GetModel2());
  if (pArmor->HasModel3()) OutputFile.Printf("\tFBModel = \"%s\"\n", pArmor->GetModel3());
  if (pArmor->HasModel4()) OutputFile.Printf("\tFWModel = \"%s\"\n", pArmor->GetModel4());
  OutputFile.Printf("\tBasePrice = %ld\n", pArmor->GetValue());
  OutputFile.Printf("\tItemLevel = %d\n", ComputeItemLevel(pArmor));
  OutputFile.Printf("\tWeight = %g\n", pArmor->GetWeight());
  OutputFile.Printf("\tArmor = %d\n", pArmor->GetArmor());
  OutputFile.Printf("\tArmorType = \"%s\"\n", pArmor->IsLightArmor() ? "Light" : "Heavy");
  OutputFile.Printf("\tHealth = %d\n", pArmor->GetArmorData()->Health);

  ExportBipedParts(OutputFile, pArmor->GetBipedFlags());

  OutputFile.Printf("End\n");
  ++m_ExportCount;
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportItemData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportBipedParts (OutputFile, BipedFlags);
 *
 * Export any biped flags in the given record to the item output file.
 *
 *=========================================================================*/
bool CObriGenerate::ExportBipedParts (CObFile& OutputFile, const dword BipedFlags) {
  CSString Buffer;

  MakeObBipedFlagString(Buffer, BipedFlags);

  OutputFile.Printf("\tBipedParts = %s\n", Buffer);
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportBodyParts()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportBook (OutputFile, pBook);
 *
 * Output misc data record to a text base item file. Returns false on any
 * error. Protected class method.
 *
 *=========================================================================*/
bool CObriGenerate::ExportBook (CObFile& OutputFile, CObBookRecord* pBook) {

  if (pBook == NULL) return (false);
  if (!pBook->IsScroll()) return (true);

  OutputFile.Printf("Item\n");
  OutputFile.Printf("\tItemType = Scroll\n");
  OutputFile.Printf("\tName = \"%s\"\n", pBook->GetItemName());
  OutputFile.Printf("\tIcon = \"%s\"\n", pBook->GetIcon());
  OutputFile.Printf("\tModel = \"%s\"\n", pBook->GetModel());
  OutputFile.Printf("\tBasePrice = %ld\n", pBook->GetValue());
  OutputFile.Printf("\tItemLevel = %d\n", ComputeItemLevel(pBook));
  OutputFile.Printf("\tWeight = %g\n", pBook->GetWeight());

  OutputFile.Printf(("End\n"));
  ++m_ExportCount;
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportBook()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportClothing (OutputFile, pClothing);
 *
 * Output clothing data record to a text base item file. Returns false on any
 * error. Protected class method.
 *
 *=========================================================================*/
bool CObriGenerate::ExportClothing (CObFile& OutputFile, CObClotRecord* pClothing) {

	/* Only output unenchanted clothing pieces */
  if (pClothing == NULL) return (false);
  if (pClothing->GetEnchantment() != OB_FORMID_NULL) return (true);

  OutputFile.Printf("Item\n");
  OutputFile.Printf("\tItemType = Clothing\n");
  OutputFile.Printf("\tName = \"%s\"\n", pClothing->GetItemName());
  OutputFile.Printf("\tMIcon = \"%s\"\n", pClothing->GetIcon());
  if (pClothing->HasIcon2()) OutputFile.Printf("\tFIcon = \"%s\"\n", pClothing->GetIcon2());
  OutputFile.Printf("\tMBModel = \"%s\"\n", pClothing->GetModel());
  if (pClothing->HasModel2()) OutputFile.Printf("\tMWModel = \"%s\"\n", pClothing->GetModel2());
  if (pClothing->HasModel3()) OutputFile.Printf("\tFBModel = \"%s\"\n", pClothing->GetModel3());
  if (pClothing->HasModel4()) OutputFile.Printf("\tFWModel = \"%s\"\n", pClothing->GetModel4());
  OutputFile.Printf("\tBasePrice = %ld\n", pClothing->GetValue());
  OutputFile.Printf("\tItemLevel = %d\n", ComputeItemLevel(pClothing));
  OutputFile.Printf("\tWeight = %g\n", pClothing->GetWeight());
  
  ExportBipedParts(OutputFile, pClothing->GetBipedFlags());

  OutputFile.Printf("End\n");
  ++m_ExportCount;
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportItemData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportMisc (OutputFile, pMisc);
 *
 * Output misc data record to a text base item file. Returns false on any
 * error. Protected class method.
 *
 *=========================================================================*/
bool CObriGenerate::ExportMisc (CObFile& OutputFile, CObMiscRecord* pMisc) {

  if (pMisc == NULL) return (false);

  OutputFile.Printf("Item\n");
  OutputFile.Printf("\tItemType = Misc\n");
  OutputFile.Printf("\tName = \"%s\"\n", pMisc->GetItemName());
  OutputFile.Printf("\tIcon = \"%s\"\n", pMisc->GetIcon());
  OutputFile.Printf("\tModel = \"%s\"\n", pMisc->GetModel());
  OutputFile.Printf("\tBasePrice = %ld\n", pMisc->GetValue());
  OutputFile.Printf("\tItemLevel = %d\n", ComputeItemLevel(pMisc));
  OutputFile.Printf("\tWeight = %g\n", pMisc->GetWeight());

  OutputFile.Printf(("End\n"));
  ++m_ExportCount;
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportMisc()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool ExportWeapon (OutputFile, pWeapon);
 *
 * Output clothing data record to a text base item file. Returns false on any
 * error. Protected class method.
 *
 *=========================================================================*/
bool CObriGenerate::ExportWeapon (CObFile& OutputFile, CObWeapRecord* pWeapon) {

	/* Determine if we wish to export the record */
  if (pWeapon == NULL) return (false);
  if (pWeapon->GetEnchantment() != OB_FORMID_NULL) return (true);
    
  OutputFile.Printf("Item\n");

  if (pWeapon->GetType() == OB_WEAPTYPE_STAFF)
    OutputFile.Printf("\tItemType = Staff\n");
  else
    OutputFile.Printf("\tItemType = Weapon\n");

  OutputFile.Printf("\tName = \"%s\"\n", pWeapon->GetItemName());
  OutputFile.Printf("\tIcon = \"%s\"\n", pWeapon->GetIcon());
  OutputFile.Printf("\tModel = \"%s\"\n", pWeapon->GetModel());
  OutputFile.Printf("\tBasePrice = %ld\n", pWeapon->GetValue());
  OutputFile.Printf("\tItemLevel = %d\n", ComputeItemLevel(pWeapon));
  OutputFile.Printf("\tWeight = %g\n", pWeapon->GetWeight());
  OutputFile.Printf("\tEnchantPoints = %d\n", pWeapon->GetEnchantPoints());
  OutputFile.Printf("\tType = \"%s\"\n", GetObWeaponTypeString(pWeapon->GetType()));
  OutputFile.Printf("\tReach = %g\n", pWeapon->GetReach());
  OutputFile.Printf("\tSpeed = %g\n", pWeapon->GetSpeed());
  OutputFile.Printf("\tHealth = %d\n", (int)pWeapon->GetHealth());
  OutputFile.Printf("\tDamage = %d\n", (int)pWeapon->GetDamage());
  OutputFile.Printf("\tIgnoreResist = %s\n", BooleanToString(pWeapon->IsIgnoreResist()));

  OutputFile.Printf("End\n");
  ++m_ExportCount;
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::ExportWeapon()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool IsValidEffect (pEffect, pItem);
 *
 * Returns true if the given effect/item combination is valid. Does not
 * check the effect or item level.
 *
 *=========================================================================*/
bool CObriGenerate::IsValidEffect (obri_effect_t* pEffect, obri_item_t* pItem) {
  dword Index;

	/* Check the effect/enchantment type */
  if ((pItem->EffectMask & pEffect->EffectType) == 0) return (false);

	/* Check the enchantment mask */
  if (pEffect->EffectType == OBRI_EFFTYPE_ENCHANT) {
    switch (pEffect->Enchantment.EnchantType) {
      case OB_ENCHTYPE_SCROLL:
        if ((pItem->EnchantMask & OBRI_ENCTYPE_ONCE) == 0) return (false);
	break;
      case OB_ENCHTYPE_APPAREL:
        if ((pItem->EnchantMask & OBRI_ENCTYPE_CONSTANT) == 0) return (false);
	break;
      case OB_ENCHTYPE_WEAPON:
        if ((pItem->EnchantMask & OBRI_ENCTYPE_STRIKES) == 0) return (false);
	break;
      case OB_ENCHTYPE_STAFF:
        if ((pItem->EnchantMask & OBRI_ENCTYPE_TARGET) == 0) return (false);
	break;
     }
   }
	/* Special case for reach effects on bows */
  else if (pEffect->EffectType == OBRI_EFFTYPE_REACH && pItem->ItemType == OBRI_ITEMTYPE_WEAPON) {
    if (pItem->Weapon.Type == OB_WEAPTYPE_BOW) return (false);
   }

	/* Check the effect name masks if required */
  if (pEffect->NameMasks.GetSize() > 0) {
    bool Found = false;

    for (Index = 0; Index < pEffect->NameMasks.GetSize(); ++Index) {
      if (stristr(pItem->Name, *pEffect->NameMasks[Index]) != NULL) {
        Found = true;
	break;
       }
     }

    if (!Found) return (false);  
   }

   	/* Check the effect custom types if required */
  if (pEffect->CustomTypes.GetSize() > 0) {
    bool Found = false;

    for (Index = 0; Index < pEffect->CustomTypes.GetSize(); ++Index) {
      if (stricmp(pItem->CustomType, *pEffect->CustomTypes[Index]) == 0) {
        Found = true;
	break;
       }
     }

    if (!Found) return (false);  
   }


  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::IsValidEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool LoadDataFiles (ForceClear);
 *
 * Attempt to load the data files. Returns false on any error.
 *
 *=========================================================================*/
bool CObriGenerate::LoadDataFiles (const bool ForceClear) {
  bool Result = true;

  if (ForceClear) {
    m_RandItems.Destroy();
    m_RandEffects.Destroy();
    m_RandUniques.Destroy();
    m_LevelLists.Destroy();
    m_EnchantMap.Destroy();
    m_ItemMap.Destroy();
   }

  if (m_ProjectFile.HasBaseItems())  {
    m_RandItems.Destroy();

#if OBRI_OLD_ITEMMAP
    m_ItemMap.Destroy();
#endif

    Result &= m_RandItems.ReadItemFile(m_ProjectFile.GetBaseItems());
   }

  if (m_ProjectFile.HasEffects()) {
    m_RandEffects.Destroy();

#if OBRI_OLD_ITEMMAP
    m_ItemMap.Destroy();
#endif

    m_EnchantMap.Destroy();
    Result &= m_RandEffects.ReadEffectFile(m_ProjectFile.GetEffects());
   }

  if (m_ProjectFile.HasUniques()) {
    m_RandUniques.Destroy();

#if OBRI_OLD_ITEMMAP
    m_ItemMap.Destroy();
#endif
    Result &= m_RandUniques.ReadUniqueFile(m_ProjectFile.GetUniques());
   }

  if (m_ProjectFile.HasLevelLists()) {
    m_LevelLists.Destroy();
    Result &= m_LevelLists.Load(m_ProjectFile.GetLevelLists());
   }

  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::LoadDataFiles()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool LoadBaseItems (pFilename);
 *
 *=========================================================================*/
bool CObriGenerate::LoadBaseItems (const SSCHAR* pFilename) {
  bool Result;

  m_RandItems.Destroy();

#if OBRI_OLD_ITEMMAP
  m_ItemMap.Destroy();
#endif

  Result = m_RandItems.ReadItemFile(pFilename);
  if (Result) m_ProjectFile.SetBaseItems(pFilename);
  
  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::LoadBaseItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool LoadEffects (pFilename);
 *
 *=========================================================================*/
bool CObriGenerate::LoadEffects (const SSCHAR* pFilename) {
  bool Result;

  m_RandEffects.Destroy();
  m_EnchantMap.Destroy();

#if OBRI_OLD_ITEMMAP
  m_ItemMap.Destroy();
#endif

  Result = m_RandEffects.ReadEffectFile(pFilename);
  if (Result) m_ProjectFile.SetEffects(pFilename);
  
  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::LoadEffects()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool LoadUniques (pFilename);
 *
 *=========================================================================*/
bool CObriGenerate::LoadUniques (const SSCHAR* pFilename) {
  bool Result;

  m_RandUniques.Destroy();

#if OBRI_OLD_ITEMMAP
  m_ItemMap.Destroy();
#endif

  Result = m_RandUniques.ReadUniqueFile(pFilename);
  if (Result) m_ProjectFile.SetUniques(pFilename);
  
  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::LoadUniques()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool LoadLevelLists (pFilename);
 *
 *=========================================================================*/
bool CObriGenerate::LoadLevelLists (const SSCHAR* pFilename) {
  bool Result;

  m_LevelLists.Destroy();

  Result = m_LevelLists.Load(pFilename);
  if (Result) m_ProjectFile.SetLevelLists(pFilename);
  
  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::LoadLevelLists()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void InitializeEspFile (void);
 *
 *=========================================================================*/
void CObriGenerate::InitializeEspFile (void) {

  m_EspFile.Destroy();
  m_EspFile.InitializeNew();

  m_EspFile.CreateTopLevelGroup(OB_NAME_SCPT);
  m_EspFile.CreateTopLevelGroup(OB_NAME_ENCH);
  m_EspFile.CreateTopLevelGroup(OB_NAME_SPEL);
  m_EspFile.CreateTopLevelGroup(OB_NAME_ARMO);
  m_EspFile.CreateTopLevelGroup(OB_NAME_BOOK);
  m_EspFile.CreateTopLevelGroup(OB_NAME_CLOT);
  m_EspFile.CreateTopLevelGroup(OB_NAME_LIGH);
  m_EspFile.CreateTopLevelGroup(OB_NAME_MISC);
  m_EspFile.CreateTopLevelGroup(OB_NAME_WEAP);
  m_EspFile.CreateTopLevelGroup(OB_NAME_SGST);
  m_EspFile.CreateTopLevelGroup(OB_NAME_LVLI);
  
 }  
/*===========================================================================
 *		End of Class Method CObriGenerate::InitializeEspFile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void InitializeSoundRecords (void);
 *
 * Initializes the default set of sound records as found in Oblivion.esm.
 *
 *=========================================================================*/
void CObriGenerate::InitializeSoundRecords (void) {
  CObBaseRecord* pRecord;
  CObIdRecord*   pSound;
  CObFile	 File;
  CSString       Buffer;
  CSString	 EditorID;
  CSString	 FormIDString;
  char*		 pError;
  obformid_t	 FormID;
  bool           Result;

  m_SoundRecords.InitializeNew();
  Result = File.Open("Sounds.csv", "rt");
  if (!Result) return;

  while (!File.IsEOF()) {
    Result = File.ReadLine(Buffer);
    if (!Result) break;

    Result = Buffer.SeperateVarValue (EditorID, FormIDString, ',');
    if (!Result) continue;

    FormID = strtoul(FormIDString, &pError, 0);
    if (*pError != NULL_CHAR) continue;

    pRecord = m_SoundRecords.CreateNewRecord(OB_NAME_SOUN);
    if (pRecord == NULL) continue;

    pSound = ObCastClass(CObIdRecord, pRecord);

    if (pSound != NULL) {
      pSound->SetEditorID(EditorID);
      pSound->SetFormID(FormID);
      m_SoundRecords.IndexRecord(pSound);
     }
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::InitializeSoundRecords()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool LoadProjectFile (pFilename);
 *
 * Attempt to load the given project file.
 *
 *=========================================================================*/
bool CObriGenerate::LoadProjectFile (const SSCHAR* pFilename) {
  bool Result;

	/* Attempt to load the group file */
  Result = m_ProjectFile.Load(pFilename);
  if (!Result) return (false);

  	/* Clear the current file data */
  InitializeEspFile();
  
	/* Change to the file's directory */
  char Dir[_MAX_DIR + 1];
  char Drive[_MAX_DRIVE + 1];
  char Buffer[_MAX_PATH + 1];
  _splitpath(pFilename, Drive, Dir, NULL, NULL);
  _makepath(Buffer, Drive, Dir, NULL, NULL);
  m_InitialPath = Buffer;

  _chdir(m_InitialPath);

	/* Reload the data files */
  ObErrorHandler->ClearErrors();
  Result = LoadDataFiles();
  
  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::LoadProjectFile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool MergeProjectFile (pFilename);
 *
 * Similar to load except only parts contained in the project file are
 * loaded. Any missing parts keep their current value.
 *
 *=========================================================================*/
bool CObriGenerate::MergeProjectFile (const SSCHAR* pFilename) {
  bool Result;

	/* Attempt to load the group file */
  Result = m_ProjectFile.Load(pFilename, false);
  if (!Result) return (false);

	/* Change to the file's directory */
  char Dir[_MAX_DIR + 1];
  char Drive[_MAX_DRIVE + 1];
  char Buffer[_MAX_PATH + 1];
  _splitpath(pFilename, Drive, Dir, NULL, NULL);
  _makepath(Buffer, Drive, Dir, NULL, NULL);
  m_InitialPath = Buffer;

  _chdir(m_InitialPath);

	/* Reload the data files */
  Result = LoadDataFiles(false);
  
  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::MergeProjectFile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGeneration Method - bool MakeCountItems (const dword ItemCount);
 *
 * Make the requested total number of items.
 *
 *=========================================================================*/
bool CObriGenerate::MakeCountItems (const dword ItemCount) {
  obri_cbdata_t CallbackData;
  obri_item_t*  pItem;
  dword         Index = 0;
  dword		TotalItems = 0;
  dword		InitialItemCount;
  dword		InitialFailedCount;
  int		Result;

	/* Ignore if nothing to do */
  if (ItemCount == 0) return (true);

  CallbackData.Completed = 0.0f;
  CallbackData.UserData  = m_Callback.UserData;
  InitialItemCount       = m_NumNewItems;
  InitialFailedCount     = m_NumFailedItems;

	/* Do an initial check to ensure there is at least one valid base item */
  for (Index = 0; Index < m_RandItems.GetNumItems(); ++Index) {
    pItem = m_RandItems.GetItem(Index);
    if ((pItem->ItemType & GetOptions().ItemMask) == 0) continue;
    if (pItem->ItemLevel < GetOptions().MinLevel || pItem->ItemLevel > GetOptions().MaxLevel) continue;
    ++TotalItems;
   }
  
	/* Ignore if no items to create with */
  if (TotalItems == 0) {
    AddObGeneralError("No base items match the requested item types (%s) and level range (%d to %d)!", 
		ObriItemMaskToString(GetOptions().ItemMask), GetOptions().MinLevel, GetOptions().MaxLevel);
    return (true);
   }

  SystemLog.Printf("Generating %u items using %u base items with matching type (%s) and level (%d to %d)...", ItemCount, TotalItems,
			ObriItemMaskToString(GetOptions().ItemMask), GetOptions().MinLevel, GetOptions().MaxLevel);
  Index = 0;
  
	/* Make the requested number of items */
  while (m_NumNewItems - InitialItemCount < ItemCount) {

#if OBRI_RANDITEM_METHOD == 1
    Result = MakeRandomItem();
#elif OBRI_RANDITEM_METHOD == 2
    Result = MakeRandomItem2();
#endif

    if (Result) {
      m_GenerateData.LastFailed = true;
     }
    else {
      m_GenerateData.LastFailed = false;
     }

    ++Index;

    if (Index % 100 == 0 && m_Callback.Function != NULL) {
      CallbackData.Completed = (float)(m_NumNewItems - InitialItemCount) * 100.0f / (float) ItemCount;
      Result = m_Callback.Function(CallbackData);
      if (Result < 0) return (false);
     }

		/* Give up after a while to prevent an infinite loop */
    if (Index > ItemCount*100) {
      AddObGeneralError("Aborting item generation due to too many failed creation attempts (%u)!", m_NumFailedItems - InitialFailedCount);
      break;
     }
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGeneration::MakeCountItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - MakeEnchantment (pRecord, ppEnchant);
 *
 * Creates an enchantment object, adding it to the given record.
 *
 *=========================================================================*/
void CObriGenerate::MakeEnchantment (CObRecord* pRecord, CObEnchRecord** ppEnchant) {
  CObBaseRecord*  pBaseRecord;
  CObItem2Record* pItem2;

	/* Ignore invalid input */
  if (pRecord == NULL || ppEnchant == NULL) return;
  pItem2 = ObCastClass(CObItem2Record, pRecord);
  if (pItem2 == NULL) return;

	/* Allocate the new enchant object */
  pBaseRecord = m_EspFile.CreateNewRecord(OB_NAME_ENCH);
  if (pBaseRecord == NULL) return;

  (*ppEnchant) = ObCastClass(CObEnchRecord, pBaseRecord);
  if ((*ppEnchant) == NULL) return;
  m_NewEnchantments.Add(*ppEnchant);
  (*ppEnchant)->SetCompressed(OBRI_COMPRESS);    

	/* Initialize the enchantment */
  (*ppEnchant)->SetEditorID(MakeItemID());
  (*ppEnchant)->SetFormID(MakeFormID());
  m_EspFile.IndexRecord(*ppEnchant);

  (*ppEnchant)->SetAutoCalc(false);
  (*ppEnchant)->SetCharge(0);
  (*ppEnchant)->SetCost(0);
  (*ppEnchant)->SetType(OB_ENCHTYPE_APPAREL);
  
	/* Assign the enchantment to the record */
  pItem2->SetEnchantment((*ppEnchant)->GetFormID());
  pItem2->SetEnchantPoints(1);
  m_GenerateData.HasEnchantment = true;
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::MakeEnchantment()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGeneration Method - bool MakeIndividualItems (void);
 *
 * Makes the requested amount of each item type.
 *
 *=========================================================================*/
bool CObriGenerate::MakeIndividualItems (void) {
  int	   OriginalItemMask;
  bool	   Result;

  OriginalItemMask = GetOptions().ItemMask;
  
	/* Armor items */
  if (OriginalItemMask & OBRI_ITEMTYPE_ARMOR) {
    GetOptions().ItemMask = OBRI_ITEMTYPE_ARMOR;

    Result = MakeCountItems(GetOptions().NumArmor);

    GetOptions().ItemMask = OriginalItemMask;
    if (!Result) return (false);
   }

	/* Clothing items */
  if (OriginalItemMask & OBRI_ITEMTYPE_CLOTHING) {
    GetOptions().ItemMask = OBRI_ITEMTYPE_CLOTHING;

    Result = MakeCountItems(GetOptions().NumClothing);

    GetOptions().ItemMask = OriginalItemMask;
    if (!Result) return (false);
   }

	/* Light items */
  if (OriginalItemMask & OBRI_ITEMTYPE_LIGHT) {
    GetOptions().ItemMask = OBRI_ITEMTYPE_LIGHT;

    Result = MakeCountItems(GetOptions().NumLights);

    GetOptions().ItemMask = OriginalItemMask;
    if (!Result) return (false);
   }

	/* Misc items */
  if (OriginalItemMask & OBRI_ITEMTYPE_MISC) {
    GetOptions().ItemMask = OBRI_ITEMTYPE_MISC;

    Result = MakeCountItems(GetOptions().NumMisc);

    GetOptions().ItemMask = OriginalItemMask;
    if (!Result) return (false);
   }

	/* Scroll items */
  if (OriginalItemMask & OBRI_ITEMTYPE_SCROLL) {
    GetOptions().ItemMask = OBRI_ITEMTYPE_SCROLL;

    Result = MakeCountItems(GetOptions().NumScrolls);

    GetOptions().ItemMask = OriginalItemMask;
    if (!Result) return (false);
   }

	/* Sigil stone items */
  if (OriginalItemMask & OBRI_ITEMTYPE_SIGIL) {
    GetOptions().ItemMask = OBRI_ITEMTYPE_SIGIL;

    Result = MakeCountItems(GetOptions().NumSigils);

    GetOptions().ItemMask = OriginalItemMask;
    if (!Result) return (false);
   }

	/* Staff items */
  if (OriginalItemMask & OBRI_ITEMTYPE_STAFF) {
    GetOptions().ItemMask = OBRI_ITEMTYPE_STAFF;

    Result = MakeCountItems(GetOptions().NumStaffs);

    GetOptions().ItemMask = OriginalItemMask;
    if (!Result) return (false);
   }


	/* Weapon items */
  if (OriginalItemMask & OBRI_ITEMTYPE_WEAPON) {
    GetOptions().ItemMask = OBRI_ITEMTYPE_WEAPON;

    Result = MakeCountItems(GetOptions().NumWeapons);

    GetOptions().ItemMask = OriginalItemMask;
    if (!Result) return (false);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGeneration::MakeIndividualItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class SSCHAR* CObriGenerate Method - const MakeExtItemId (void);
 *
 * Creates an extended item ID based on the current item and options.
 *
 *=========================================================================*/
const SSCHAR* CObriGenerate::MakeExtItemId (void) {
  static CSString s_Buffer;
  CSString	  Buffer;
  CSString        TempFormat;
  CSString	  TempBuffer;
  const SSCHAR*   pParse;
  const SSCHAR*   pFormat;
  dword		  Length;
  
  pParse  = GetOptions().ExtendedID;
  pFormat = NULL;
  s_Buffer.Empty();
  
  while (*pParse != NULL_CHAR) {

    if (pFormat != NULL) {
      ++Length;

      if (isalpha(*pParse)) {
        TempFormat.Copy(pFormat, Length - 1);
	
	switch (tolower(*pParse)) {
	  case 'l':
		TempFormat += 'd';
		TempBuffer.Format(TempFormat, m_GenerateData.CalcItemLevel);
		Buffer += TempBuffer;
		break;
          case 'i':
		TempFormat += 'u';
		TempBuffer.Format(TempFormat, l_ItemIDCount);
		++l_ItemIDCount;
		Buffer += TempBuffer;
		break;
          case 'x':
		TempFormat += 'X';
		TempBuffer.Format(TempFormat, l_ItemIDCount);
		++l_ItemIDCount;
		Buffer += TempBuffer;
		break;
          default:
		TempFormat += *pParse;
		Buffer += TempFormat;
		break;
	 }
	
	pFormat = NULL;
       }
      else if (isdigit(*pParse)) {
       }
      else {
        Buffer.Append(pFormat, Length);
        pFormat = NULL;
       }
     }
    else if (*pParse == '%') {
      pFormat = pParse;
      Length = 1;
     }
    else if (isalnum(*pParse)) {
      Buffer += *pParse;
     }

    ++pParse;
   }

	/* Remove all 'bad' characters from final string */
  pParse = Buffer;

  while (*pParse != NULL_CHAR) {
    if (isalnum(*pParse)) {
      s_Buffer += *pParse;
     }

    ++pParse;
   }

  return (s_Buffer);
 }
/*===========================================================================
 *		End of Class Method SSCHAR* CObriGenerate::MakeExtItemId()
 *=========================================================================*/


/*===========================================================================
 *
 * Class SSCHAR* CObriGenerate Method - const MakeItemID (void);
 *
 *=========================================================================*/
const SSCHAR* CObriGenerate::MakeItemID (void) {
  CObIdRecord*  pRecord;
  CSString      LastExtID;
  const SSCHAR* pID;
  dword	        Count = 0;

  do {

    switch (GetOptions().IDType) {
      case OBRI_IDTYPE_EXT:
        pID = MakeExtItemId();

	if (LastExtID.CompareNoCase(pID) == 0) {
	  pID = MakeNumberItemIdShort(GetOptions().IDPrefix);
	 }
        else {
  	  LastExtID = pID;
         }

	break;
      case OBRI_IDTYPE_SHORT8: 
	pID = MakeNumberItemId8(GetOptions().IDPrefix);
	break;
      case OBRI_IDTYPE_SHORT6: 
	pID = MakeNumberItemId6(GetOptions().IDPrefix);
	break;
      case OBRI_IDTYPE_SHORT4: 
        pID = MakeNumberItemId4(GetOptions().IDPrefix);
	break;
      case OBRI_IDTYPE_SHORT:  
	pID = MakeNumberItemIdShort(GetOptions().IDPrefix);
	break;
      default:
      case OBRI_IDTYPE_LONG:   
        pID = MakeRandomItemIdLong(GetOptions().IDPrefix);
	break;
     }

   pRecord = m_EspFile.FindEditorID(pID);
  } while (pRecord != NULL);

  return (pID);
 }
/*===========================================================================
 *		End of Class Method SSCHAR* CObriGenerate::MakeItemID()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - obformid_t MakeFormID (void);
 *
 *=========================================================================*/
obformid_t CObriGenerate::MakeFormID (void) {
  return ObriGetNextFormID1(&m_EspFile);
 }

	
obformid_t CObriGenerate::MakeListFormID (void) {
  return ObriGetNextFormID2(&m_EspFile);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::MakeFormID()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - const SSCHAR* MakeUniqueItemName (void);
 *
 *=========================================================================*/
const SSCHAR* CObriGenerate::MakeUniqueItemName (void) {
  static SSCHAR s_Name[256] = "";
     
  if (m_GenerateData.pCurrentUnique->NameType == OBRI_UNIQUE_NAMETYPE_SUFFIX) 
    SSSNPRINTF (s_Name, 255, "%s of %s",  m_GenerateData.pCurrentItem->Name, m_GenerateData.pCurrentUnique->Name);
  else if (m_GenerateData.pCurrentUnique->NameType == OBRI_UNIQUE_NAMETYPE_PREFIX)
    SSSNPRINTF (s_Name, 255, "%s %s", m_GenerateData.pCurrentUnique->Name, m_GenerateData.pCurrentItem->Name);
  else
    SSSNPRINTF (s_Name, 255, "%s", m_GenerateData.pCurrentUnique->Name);

  return (s_Name);
 }
/*===========================================================================
 *		End of Class Method SSCHAR* CObriGenerate::MakeUniqueItemName()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - const SSCHAR* MakeItemName (void);
 *
 *=========================================================================*/
const SSCHAR* CObriGenerate::MakeItemName (void) {
  //static CSString s_Name = "";
  static SSCHAR s_Name[256] = "";
  bool  HasSuffixName  = false;
  bool  HasPrefixName  = false;
  bool  HasPrefix2Name = false;
  bool  HasMaterial    = false;
  int   SuffixWeight   = 0;
  int   PrefixWeight   = 0;
  int   Prefix2Weight  = 0;
  int   MaterialWeight = 0;
  int   Length;
  int   MinNameWeight;
  int   MinWeight;

  	/* Special case for a unique name */
  if (m_GenerateData.pCurrentUnique != NULL) return MakeUniqueItemName();
 
	/* Set the material name weight factor */
  HasMaterial = (m_GenerateData.pCurrentItem->Material[0] != NULL_CHAR);

  if (HasMaterial) {
    MaterialWeight = OBRI_NAMEWEIGHT_MAX;
    if (GetOptions().MaterialChance < MakeRandItemRatio()) MaterialWeight = 0;
   }

	/* Set the prefix name weight factor */
  if (m_GenerateData.pCurrentPrefix != NULL) {
    HasPrefixName = (m_GenerateData.pCurrentPrefix->Name[0] != NULL_CHAR);

    if (HasPrefixName) {
      PrefixWeight    = OBRI_NAMEWEIGHT_MAX + m_GenerateData.pCurrentPrefix->ItemLevel;
      //MaterialWeight -= OBRI_NAMEWEIGHT_ADJUST1 + m_GenerateData.pCurrentPrefix->ItemLevel;
      MaterialWeight -= OBRI_NAMEWEIGHT_ADJUST1;
     }
   }

	/* Set the prefix2 name weight factor */
  if (m_GenerateData.pCurrentPrefix2 != NULL) {
    HasPrefix2Name = (m_GenerateData.pCurrentPrefix2->Name[0] != NULL_CHAR);

    if (HasPrefix2Name) {
      Prefix2Weight  = OBRI_NAMEWEIGHT_MAX/2 + m_GenerateData.pCurrentPrefix2->ItemLevel;
      //MaterialWeight = -OBRI_NAMEWEIGHT_MAX;
      MaterialWeight -= OBRI_NAMEWEIGHT_ADJUST1;
      if (GetOptions().HidePrefix2) Prefix2Weight = 0;
     }
   }

	/* Set the suffix name weight factor */
  if (m_GenerateData.pCurrentSuffix != NULL) {
    HasSuffixName = (m_GenerateData.pCurrentSuffix->Name[0] != NULL_CHAR);

    if (HasSuffixName) {
      SuffixWeight    = OBRI_NAMEWEIGHT_MAX + m_GenerateData.pCurrentSuffix->ItemLevel;
      //MaterialWeight -= OBRI_NAMEWEIGHT_ADJUST1 + m_GenerateData.pCurrentSuffix->ItemLevel;
      MaterialWeight -= OBRI_NAMEWEIGHT_ADJUST1;
     }
   }

	/* Make the item name several times adjusting the weight of names each time in
	 * order to make a name within the desired maximum length. */
  do {
    SSSNPRINTF (s_Name, 255, "%s%s%s%s%s%s%s%s%s%s%s", 
		GetOptions().NameDelimiter.c_str(),
		PrefixWeight   > 0 ? m_GenerateData.pCurrentPrefix->Name  : "", 
		PrefixWeight   > 0 ? " " : "", 
		Prefix2Weight  > 0 ? m_GenerateData.pCurrentPrefix2->Name : "", 
		Prefix2Weight  > 0 ? " " : "", 
		MaterialWeight > 0 ? m_GenerateData.pCurrentItem->Material : "", 
		MaterialWeight > 0 ? " " : "", 
		m_GenerateData.pCurrentItem->Name, 
		SuffixWeight   > 0 ? GetOptions().OfString : "",
		SuffixWeight   > 0 ? m_GenerateData.pCurrentSuffix->Name : "",
		GetOptions().NameDelimiter.c_str());

    Length = strlen(s_Name);
    if (Length <= GetOptions().MaxNameLength) return (s_Name);
    MinWeight = 200;

	/* Find the minimum weighted name part to remove */
    MinNameWeight = OBRI_NAMEWEIGHT_NONE;

    if (MaterialWeight > 0 && MaterialWeight < MinWeight) {
      MinNameWeight = OBRI_NAMEWEIGHT_MATERIAL;
      MinWeight     = MaterialWeight;
     }

    if (PrefixWeight > 0 && PrefixWeight < MinWeight) {
      MinNameWeight = OBRI_NAMEWEIGHT_PREFIX;
      MinWeight     = PrefixWeight;
     }

    if (Prefix2Weight > 0 && Prefix2Weight < MinWeight) {
      MinNameWeight = OBRI_NAMEWEIGHT_PREFIX2;
      MinWeight     = Prefix2Weight;
     }

    if (SuffixWeight > 0 && SuffixWeight < MinWeight) {
      MinNameWeight = OBRI_NAMEWEIGHT_SUFFIX;
      MinWeight     = SuffixWeight;
     }
     
		/* Remove the lowest weighted part */
    switch (MinNameWeight) {
      case OBRI_NAMEWEIGHT_MATERIAL:
        MaterialWeight = 0;
	break;
      case OBRI_NAMEWEIGHT_PREFIX:
        PrefixWeight = 0;
	break;
      case OBRI_NAMEWEIGHT_PREFIX2:
        Prefix2Weight = 0;
	break;
      case OBRI_NAMEWEIGHT_SUFFIX:
        SuffixWeight = 0;
	break;
     }

  } while (MinNameWeight != OBRI_NAMEWEIGHT_NONE);

  return (s_Name);
 }
/*===========================================================================
 *		End of Class Method SSCHAR* CObriGenerate::MakeItemName()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool CheckNormalItem (void);
 *
 * Returns true if the item is valid and does not have an effect of 
 * some form. Returns true if the item is "normal".
 *
 *=========================================================================*/
bool CObriGenerate::CheckNormalItem (void) {

  if (m_GenerateData.pCurrentPrefix  == NULL && 
		m_GenerateData.pCurrentPrefix2 == NULL && 
		m_GenerateData.pCurrentSuffix  == NULL && 
		m_GenerateData.pCurrentUnique  == NULL) {
      return (true);
   }

		/* Special case for sigil stones */
  if (m_GenerateData.pCurrentItem->ItemType == OBRI_ITEMTYPE_SIGIL) {
      bool HasEnchant = false;
 
      if (m_GenerateData.pCurrentPrefix  != NULL && m_GenerateData.pCurrentPrefix->EffectType  == OBRI_EFFTYPE_ENCHANT) HasEnchant = true;
      if (m_GenerateData.pCurrentPrefix2 != NULL && m_GenerateData.pCurrentPrefix2->EffectType == OBRI_EFFTYPE_ENCHANT) HasEnchant = true;
      if (m_GenerateData.pCurrentSuffix  != NULL && m_GenerateData.pCurrentSuffix->EffectType  == OBRI_EFFTYPE_ENCHANT) HasEnchant = true;
      if (m_GenerateData.pCurrentUnique  != NULL) HasEnchant = true; /* Assumed */
   
      if (!HasEnchant) {
        return (true);
       }
   }

  return (false);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::CheckNormalItem()
 *=========================================================================*/
  
 
/*===========================================================================
 *
 * Class CObriGenerate Method - bool MakeRandomItem (void);
 *
 * Attempts to make a new random item using the current settings. The new
 * record is stored in the new records array. Returns false on any error.
 *
 *=========================================================================*/
bool CObriGenerate::MakeRandomItem (void) {
  bool Result;
  //int  Index;

	/* Compute the desired item level */
  ComputeItemLevel();

	/* Select all valid items based on current setting */
  SelectValidItems();

	/* Choose a base item to use */
  SelectBaseItem();

	/* Ensure the current item is valid */
  if (m_GenerateData.pCurrentItem == NULL) {
    ++m_NumFailedItems;
    
    if (m_NumFailedItems < 10 || 
	(m_NumFailedItems < 100 && m_NumFailedItems % 10 == 0) ||
	(m_NumFailedItems % 100 == 0)) {
      AddObUserError(OBERR_USER_BADINPUT, "No base items match the desired level (%d to %d)!", 
		(int)(m_GenerateData.CurrentItemLevel * OBRI_MINLEVEL_FACTOR), m_GenerateData.CurrentItemLevel);
     }

    return (false);
   }

	/* Select all effects based on current item */
  SelectValidPrefixes();
  SelectValidSuffixes();
  SelectValidUniques();

	/* Choose the magical effects for the current item */
  ComputeCurrentEffects();

	/* Check to ensure item/effect pair is valid for a no effect item */
  if ((m_GenerateData.pCurrentItem->EffectMask & OBRI_EFFTYPE_NONE) == 0 && 
		m_GenerateData.pCurrentPrefix  == NULL && 
		m_GenerateData.pCurrentPrefix2 == NULL && 
		m_GenerateData.pCurrentSuffix  == NULL && 
		m_GenerateData.pCurrentUnique  == NULL) {
    ++m_NumFailedItems;
    return (false);
   }

	/* Don't bother creating regular items if not required */
  if (GetOptions().RemoveNormalItems && CheckNormalItem()) {
    ++m_NumFailedItems;
    return (false);
   }

   	/* Ensure the final item level is valid */
  m_GenerateData.CalcItemLevel = GetCurrentItemLevel();

  if (!CheckFinalItemLevel(m_GenerateData.CalcItemLevel)) {
    ++m_NumFailedItems;
    return (false);
   }

	/* Create the item object and save it to the array */
  //for (Index = 0, Result = true; Index < m_GenerateData.pCurrentItem->Frequency; ++Index) {
    //Result &= CreateItem(Index == 0);
  // }

  Result = CreateItem(true);
  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::MakeRandomItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool MakeRandomItem2 (void);
 *
 *=========================================================================*/
bool CObriGenerate::MakeRandomItem2 (void) {
  bool Result;
  //int  Index;

	/* Compute the desired item level */
  ComputeItemLevel2();

  SelectValidPrefixes2();
  SelectValidSuffixes2();
  SelectValidUniques2();

	/* Select all valid items based on current setting */
  SelectValidItems2();

	/* Choose a base item to use */
  SelectBaseItem2();

	/* Ensure the current item is valid */
  if (m_GenerateData.pCurrentItem == NULL) {
    ++m_NumFailedItems;
    //SystemLog.Printf("\tFailed1");

    if (m_NumFailedItems < 10 || 
	(m_NumFailedItems < 100 && m_NumFailedItems % 10 == 0) ||
	(m_NumFailedItems % 100 == 0)) {
      AddObUserError(OBERR_USER_BADINPUT, "No base items match the desired level (%d to %d)!", 
		(int)(m_GenerateData.CurrentItemLevel * OBRI_MINLEVEL_FACTOR), m_GenerateData.CurrentItemLevel);
     }

    return (false);
   }

	/* Narrow down the effect lists */
  SelectValidPrefixes2a();
  SelectValidSuffixes2a();
  SelectValidUniques2a();

	/* Choose the magical effects for the current item */
  ComputeCurrentEffects2();

	/* Check to ensure item/effect pair is valid for a no effect item */
  if ((m_GenerateData.pCurrentItem->EffectMask & OBRI_EFFTYPE_NONE) == 0 && 
		m_GenerateData.pCurrentPrefix  == NULL && 
		m_GenerateData.pCurrentPrefix2 == NULL && 
		m_GenerateData.pCurrentSuffix  == NULL && 
		m_GenerateData.pCurrentUnique  == NULL) {
    ++m_NumFailedItems;
    //SystemLog.Printf("\tFailed2");
    return (false);
   }

	/* Don't bother creating regular items if not required */
  if (GetOptions().RemoveNormalItems && CheckNormalItem()) {
    ++m_NumFailedItems;
    //SystemLog.Printf("\tFailed3");
    return (false);
   }

   	/* Ensure the final item level is valid */
  m_GenerateData.CalcItemLevel = GetCurrentItemLevel();

  if (!CheckFinalItemLevel2(m_GenerateData.CalcItemLevel)) {
    ++m_NumFailedItems;
    //SystemLog.Printf("\tFailed4 (%d , %d )", m_GenerateData.CalcItemLevel, m_GenerateData.MinItemLevel);
    return (false);
   }

	/* Create the item object and save it to the array */
  //for (Index = 0, Result = true; Index < m_GenerateData.pCurrentItem->Frequency; ++Index) {
    //Result &= CreateItem(Index == 0);
   //}
  Result = CreateItem(true);

  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::MakeRandomItem2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool OutputLevelCounts (pFilename);
 *
 * Outputs the current level counts to the given file in a CSV format.
 *
 *=========================================================================*/
bool CObriGenerate::OutputLevelCounts (const SSCHAR* pFilename) {
  CObFile File;
  dword   Index;
  dword   LevelSum = 0;
  dword   RunningSum = 0;
  bool    Result;

  Result = File.Open(pFilename, "wt");
  if (!Result) return (false);

	/* Compute initial sum */
  for (Index = 0; Index <= OBRI_MAX_LEVELS; ++Index) {
    LevelSum += m_LevelCounts[Index];
   }

  for (Index = 0; Index <= OBRI_MAX_LEVELS; ++Index) {
    RunningSum += m_LevelCounts[Index];
    File.Printf("%d, %d, %.3g\n", Index, m_LevelCounts[Index], (float) RunningSum / (float) LevelSum * 100.0f );
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::OutputLevelCounts()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void CObriGenerate:Reset (void);
 *
 * Resets generation settings.
 *
 *=========================================================================*/
void CObriGenerate::Reset (void) {

  DestroyNewRecords();
  InitializeEspFile();

	/* Reset the number of items/objects created */
  m_NumNewItems     = 0;
  m_NumFailedItems  = 0;
  m_NumEnchantments = 0;
  m_NumDupEnchants  = 0;
  m_NumDupItems     = 0;
  m_NumScripts	    = 0;
  m_HasMergedFile   = false;
  
	/* Reset the ID counters */
  ObriResetNextFormID1(GetOptions().FirstFormID1);
  ObriResetNextFormID2(GetOptions().FirstFormID2);

  l_ItemIDCount     = 0;
  memset(&m_LevelCounts, 0, sizeof(m_LevelCounts));
  m_AverageLevelSum = 0;

	/* Reset the level list data */
  m_LevelLists.Reset();
 }
/*===========================================================================
 *		End of Function CObriGenerate:Reset()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void PartialReset (void);
 *
 *=========================================================================*/
void CObriGenerate::PartialReset (void) {

  //DestroyNewRecords();

	/* Reset the number of items/objects created */
  m_NumNewItems     = 0;
  m_NumFailedItems  = 0;
  m_NumEnchantments = 0;
  m_NumDupEnchants  = 0;
  m_NumDupItems     = 0;
  m_NumScripts	    = 0;

  m_AverageLevelSum = 0;

 }
/*===========================================================================
 *		End of Function CObriGenerate:PartialReset()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - bool SaveProjectFile (pFilename);
 *
 * Attempt to save the current group data file
 *
 *=========================================================================*/
bool CObriGenerate::SaveProjectFile (const SSCHAR* pFilename) {
  bool Result;

	/* Attempt to save the group file */
  Result = m_ProjectFile.Save(pFilename);
  if (!Result) return (false);
  
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SaveProjectFile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectBaseItem (void);
 *
 * Selects a base item from the current list of valid items to be used 
 * in creating a random item.
 *
 *=========================================================================*/
void CObriGenerate::SelectBaseItem (void) {
  int Index;

	/* Check for no valid items */
  if (m_ValidItems.GetSize() <= 0) {
    m_GenerateData.pCurrentItem = NULL;
   }
  else {
    Index = MakeRandItemIndex(m_ValidItems.GetSize());
    m_GenerateData.pCurrentItem = m_ValidItems.GetAt(Index);
   }
  
	/* Set the initial item value */
  if (m_GenerateData.pCurrentItem == NULL)
    m_GenerateData.ItemValue = 0;
  else
    m_GenerateData.ItemValue = m_GenerateData.pCurrentItem->BasePrice;

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectBaseItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectBaseItem2 (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectBaseItem2 (void) {
  int Index;

	/* Check for no valid items */
  if (m_ValidItems.GetSize() <= 0) {
    m_GenerateData.pCurrentItem = NULL;
   }
  else {
    Index = MakeRandItemIndex(m_ValidItems.GetSize());
    m_GenerateData.pCurrentItem = m_ValidItems.GetAt(Index);
   }
  
	/* Set the initial item value */
  if (m_GenerateData.pCurrentItem == NULL)
    m_GenerateData.ItemValue = 0;
  else
    m_GenerateData.ItemValue = m_GenerateData.pCurrentItem->BasePrice;


  //SystemLog.Printf("\tSelectBaseItem2() = %s", m_GenerateData.pCurrentItem == NULL ? "NULL" : m_GenerateData.pCurrentItem->Name);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectBaseItem2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidItems (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidItems (void) {
  obri_item_t*	pItem;
  dword		Index;

	/* Clear the current valid items */
  m_ValidItems.Destroy();

	/* Select all valid items */
  for (Index = 0; Index < m_RandItems.GetNumItems(); ++Index) {
    pItem = m_RandItems.GetItem(Index);

		/* Check item type and level */
    if ((pItem->ItemType & GetOptions().ItemMask) == 0) continue;
    if (!CheckItemLevel(pItem->ItemLevel)) continue;

    m_ValidItems.Add(pItem);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidItems2 (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidItems2 (void) {
  obri_item_t*	pItem;
  dword		Index;

	/* Clear the current valid items */
  m_ValidItems.Destroy();

	/* Select all valid items */
  for (Index = 0; Index < m_RandItems.GetNumItems(); ++Index) {
    pItem = m_RandItems.GetItem(Index);

		/* Check item type and level */
    if ((pItem->ItemType & GetOptions().ItemMask) == 0) continue;
    if (!CheckItemLevel2(pItem)) continue;

    m_ValidItems.Add(pItem);
   }

  //SystemLog.Printf("\tSelectValidItems2() = %d", m_ValidItems.GetSize());
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidItems2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidPrefixes (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidPrefixes (void) {
  obri_effect_t* pEffect;
  dword          Index;

	/* Clear the current valid effects */
  m_ValidPrefixes.Destroy();

	/* Select all valid prefixes */
  for (Index = 0; Index < m_RandEffects.GetNumPrefixes(); ++Index) {
    pEffect = m_RandEffects.GetPrefix(Index);

		/* Check effect type and level */
    if ((pEffect->ItemMask & GetOptions().ItemMask) == 0) continue;
    if ((pEffect->ItemMask & m_GenerateData.pCurrentItem->ItemType) == 0) continue;
    if (!IsValidEffect(pEffect, m_GenerateData.pCurrentItem)) continue;

    if (pEffect->Cursed) {
      if (!CheckCursedLevel(pEffect->ItemLevel)) continue;
     }
    else {
      if (!CheckEffectLevel(pEffect->ItemLevel)) continue;
     }

    m_ValidPrefixes.Add(pEffect);
   }
 }


void CObriGenerate::SelectValidSuffixes (void) {
  obri_effect_t* pEffect;
  dword          Index;

  m_ValidSuffixes.Destroy();

	/* Select all valid suffixes */
  for (Index = 0; Index < m_RandEffects.GetNumSuffixes(); ++Index) {
    pEffect = m_RandEffects.GetSuffix(Index);

		/* Check effect type and level */
    if ((pEffect->ItemMask & GetOptions().ItemMask) == 0) continue;
    if ((pEffect->ItemMask & m_GenerateData.pCurrentItem->ItemType) == 0) continue;
    if (!CheckEffectLevel(pEffect->ItemLevel)) continue;

    if (!IsValidEffect(pEffect, m_GenerateData.pCurrentItem)) continue;

    m_ValidSuffixes.Add(pEffect);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidEffects()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidUniques (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidUniques (void) {
  obri_unique_t* pUnique;
  dword		 Index;
  dword		 EffectIndex;
  bool		 ValidUnique;

	/* Clear the current valid uniques */
  m_ValidUniques.Destroy();

	/* Select all valid uniques */
  for (Index = 0; Index < m_RandUniques.GetNumUniques(); ++Index) {
    pUnique = m_RandUniques.GetUnique(Index);

		/* Check effect type and level */
    //CheckItemMasks(pUnique->ItemMask)
    if ((pUnique->ItemMask & GetOptions().ItemMask) == 0) continue;
    if ((pUnique->ItemMask & m_GenerateData.pCurrentItem->ItemType) == 0) continue;
    if (!CheckEffectLevel(pUnique->ItemLevel)) continue;
    ValidUnique = true;

    for (EffectIndex = 0; EffectIndex < pUnique->NumEffects; ++EffectIndex) {

      if (!IsValidEffect(pUnique->pEffects[EffectIndex], m_GenerateData.pCurrentItem)) {
        ValidUnique = false;
        break;
       }

     }

    if (ValidUnique) m_ValidUniques.Add(pUnique);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidUniques()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidPrefixes2 (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidPrefixes2 (void) {
  obri_effect_t* pEffect;
  dword          Index;

	/* Clear the current valid effects */
  m_ValidPrefixes.Destroy();

	/* Select all valid prefixes */
  for (Index = 0; Index < m_RandEffects.GetNumPrefixes(); ++Index) {
    pEffect = m_RandEffects.GetPrefix(Index);

		/* Check effect type and level */
    if ((pEffect->ItemMask & GetOptions().ItemMask) == 0) continue;
    if (!CheckEffectLevel2(pEffect->ItemLevel)) continue;

    m_ValidPrefixes.Add(pEffect);
   }

  //SystemLog.Printf("\tSelectValidPrefixes2() = %d", m_ValidPrefixes.GetSize());
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidPrefixes2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidPrefixes2a (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidPrefixes2a (void) {
  obri_effect_t* pEffect;
  bool		 DeleteEffect;
  int            Index;

	/* Compare all all current prefixes */
  for (Index = m_ValidPrefixes.GetSize() - 1; Index >= 0 ; --Index) {
    pEffect      = m_ValidPrefixes.GetAt(Index);
    DeleteEffect = false;

    if ((pEffect->ItemMask & m_GenerateData.pCurrentItem->ItemType) == 0) DeleteEffect = true;
    if (!IsValidEffect(pEffect, m_GenerateData.pCurrentItem)) DeleteEffect = true;

    if (DeleteEffect) m_ValidPrefixes.Delete(Index);
   }

  //SystemLog.Printf("\tSelectValidPrefixes2a() = %d", m_ValidPrefixes.GetSize());
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidPrefixes2a()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidSuffixes2 (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidSuffixes2 (void) {
  obri_effect_t* pEffect;
  dword          Index;

  m_ValidSuffixes.Destroy();

	/* Select all valid suffixes */
  for (Index = 0; Index < m_RandEffects.GetNumSuffixes(); ++Index) {
    pEffect = m_RandEffects.GetSuffix(Index);

		/* Check effect type and level */
    if ((pEffect->ItemMask & GetOptions().ItemMask) == 0) continue;
    if (!CheckEffectLevel2(pEffect->ItemLevel)) continue;

    m_ValidSuffixes.Add(pEffect);
   }

  //SystemLog.Printf("\tSelectValidSuffixes2() = %d", m_ValidSuffixes.GetSize());
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidSuffixes2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidSuffixes2a (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidSuffixes2a (void) {
  obri_effect_t* pEffect;
  bool		 DeleteEffect;
  int            Index;

	/* Compare all all current prefixes */
  for (Index = m_ValidSuffixes.GetSize() - 1; Index >= 0 ; --Index) {
    pEffect      = m_ValidSuffixes.GetAt(Index);
    DeleteEffect = false;

    if ((pEffect->ItemMask & m_GenerateData.pCurrentItem->ItemType) == 0) DeleteEffect = true;
    if (!IsValidEffect(pEffect, m_GenerateData.pCurrentItem)) DeleteEffect = true;

    if (DeleteEffect) m_ValidSuffixes.Delete(Index);
   }

  //SystemLog.Printf("\tSelectValidSuffixes2a() = %d", m_ValidSuffixes.GetSize());
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidSuffixes2a()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidUniques2 (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidUniques2 (void) {
  obri_unique_t* pUnique;
  dword		 Index;

	/* Clear the current valid uniques */
  m_ValidUniques.Destroy();

	/* Select all valid uniques */
  for (Index = 0; Index < m_RandUniques.GetNumUniques(); ++Index) {
    pUnique = m_RandUniques.GetUnique(Index);

		/* Check effect type and level */
    if ((pUnique->ItemMask & GetOptions().ItemMask) == 0) continue;
    if (!CheckEffectLevel(pUnique->ItemLevel)) continue;

    m_ValidUniques.Add(pUnique);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidUniques2()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SelectValidUniques2a (void);
 *
 *=========================================================================*/
void CObriGenerate::SelectValidUniques2a (void) {
  obri_unique_t* pUnique;
  int		 Index;
  dword		 EffectIndex;
  bool		 ValidUnique;
	
	/* Compare all all current prefixes */
  for (Index = m_ValidUniques.GetSize() - 1; Index >= 0 ; --Index) {
    pUnique = m_ValidUniques.GetAt(Index);

		/* Check effect type and level */
    if ((pUnique->ItemMask & m_GenerateData.pCurrentItem->ItemType) == 0) {
      m_ValidUniques.Delete(Index);
      continue;
     }

    ValidUnique = true;

    for (EffectIndex = 0; EffectIndex < pUnique->NumEffects; ++EffectIndex) {

      if (!IsValidEffect(pUnique->pEffects[EffectIndex], m_GenerateData.pCurrentItem)) {
        ValidUnique = false;
        break;
       }

     }

    if (!ValidUnique) m_ValidUniques.Delete(Index);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SelectValidUniques2a()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetRecordEffects (pRecord);
 *
 * Adds any effects to the input record according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetRecordEffects (CObRecord* pRecord) {
  CObEnchRecord* pEnchant = NULL;
  dword		 Index;
  long		 MultiplierSum = 0;
  long		 QualitySum = 0;
  long		 Value;

	/* Ignore if there are no effects to add */
  if (m_GenerateData.pCurrentUnique  == NULL && 
      m_GenerateData.pCurrentPrefix  == NULL && 
      m_GenerateData.pCurrentPrefix2 == NULL && 
      m_GenerateData.pCurrentSuffix  == NULL) return;

	/* Compute the quality level for the effects */
  ComputeQualityLevel();

	/* Add the unique effects */
  if (m_GenerateData.pCurrentUnique != NULL) {

    for (Index = 0; Index < m_GenerateData.pCurrentUnique->NumEffects; ++Index) {
      CreateEffect(pRecord, m_GenerateData.pCurrentUnique->pEffects[Index], &pEnchant, true);
      QualitySum += SplitValue(m_GenerateData.pCurrentUnique->pEffects[Index]->MinQuality,
			       m_GenerateData.pCurrentUnique->pEffects[Index]->MaxQuality,
			       m_GenerateData.CurrentRatio);
      MultiplierSum += m_GenerateData.pCurrentUnique->pEffects[Index]->Multiplier;
     }
   }

	/* Add the prefix effects */
  if (m_GenerateData.pCurrentPrefix != NULL) {
    CreateEffect(pRecord, m_GenerateData.pCurrentPrefix, &pEnchant);
    QualitySum += SplitValue (m_GenerateData.pCurrentPrefix->MinQuality,
			      m_GenerateData.pCurrentPrefix->MaxQuality,
			      m_GenerateData.CurrentRatio);
    MultiplierSum += m_GenerateData.pCurrentPrefix->Multiplier;
   }

	/* Add the prefix2 effects */
  if (m_GenerateData.pCurrentPrefix2 != NULL) {
    CreateEffect(pRecord, m_GenerateData.pCurrentPrefix2, &pEnchant);
    QualitySum += SplitValue (m_GenerateData.pCurrentPrefix2->MinQuality,
			      m_GenerateData.pCurrentPrefix2->MaxQuality,
			      m_GenerateData.CurrentRatio);
    MultiplierSum += m_GenerateData.pCurrentPrefix2->Multiplier;
   }

	/* Add the suffix effect */
  if (m_GenerateData.pCurrentSuffix != NULL) {
    CreateEffect(pRecord, m_GenerateData.pCurrentSuffix, &pEnchant);
    QualitySum += SplitValue (m_GenerateData.pCurrentSuffix->MinQuality,
			      m_GenerateData.pCurrentSuffix->MaxQuality,
			      m_GenerateData.CurrentRatio);
    MultiplierSum += m_GenerateData.pCurrentSuffix->Multiplier;
   }

	/* Compute the final item value */
#if OBRI_USEVALUEMULTIPLIER

  if (MultiplierSum < 0)
    Value = abs(QualitySum + m_GenerateData.ItemValue / MultiplierSum);
  else
    Value = QualitySum + m_GenerateData.ItemValue * MultiplierSum;

#else

  Value = QualitySum + m_GenerateData.ItemValue;

#endif

  if (Value <= 0) {
    Value = 1;
   }

	/* Save the item value */
  SetItemValue(pRecord, Value);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetRecordEffects()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetItemValue (pRecord, Value);
 *
 * Sets the value of any item.
 *
 *=========================================================================*/
void CObriGenerate::SetItemValue (CObRecord* pRecord, const dword Value) {

	/* Ignore invalid input */
  if (pRecord == NULL) return;

  if (pRecord->GetName() == OB_NAME_ARMO) {
    CObArmoRecord* pArmor = ObCastClass(CObArmoRecord, pRecord);
    if (pArmor != NULL) pArmor->SetValue(Value);
   }
  else if (pRecord->GetName() == OB_NAME_BOOK) {
    CObBookRecord* pBook = ObCastClass(CObBookRecord, pRecord);
    if (pBook != NULL) pBook->SetValue(Value);
   }
  else if (pRecord->GetName() == OB_NAME_CLOT) {
    CObClotRecord* pClothing = ObCastClass(CObClotRecord, pRecord);
    if (pClothing != NULL) pClothing->SetValue(Value);
   }
  else if (pRecord->GetName() == OB_NAME_LIGH) {
    CObLighRecord* pLight = ObCastClass(CObLighRecord, pRecord);
    if (pLight != NULL) pLight->SetValue(Value);
   }
  else if (pRecord->GetName() == OB_NAME_MISC) {
    CObMiscRecord* pMisc = ObCastClass(CObMiscRecord, pRecord);
    if (pMisc != NULL) pMisc->SetValue(Value);
   }
  else if (pRecord->GetName() == OB_NAME_SGST) {
    CObSgstRecord* pSigil = ObCastClass(CObSgstRecord, pRecord);
    if (pSigil != NULL) pSigil->SetValue(Value);
   }
  else if (pRecord->GetName() == OB_NAME_WEAP) {
    CObWeapRecord* pWeapon = ObCastClass(CObWeapRecord, pRecord);
    if (pWeapon != NULL) pWeapon->SetValue(Value);
   }
  
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetItemValue()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetRecordItemParams (pRecord);
 *
 * Sets the parameters of the input record according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetRecordItemParams (CObRecord* pRecord) {
  CObItem1Record* pItem1;

	/* Record ID and name */
  pItem1 = ObCastClass(CObItem1Record, pRecord);
  if (pItem1 == NULL) return;
  pItem1->SetFormID(MakeFormID());
  pItem1->SetEditorID(MakeItemID());

  //pItem1->SetItemName(pItem1->GetEditorID());
  pItem1->SetItemName(MakeItemName());

  m_EspFile.IndexRecord(pItem1);

	/* Set parameters according to item type */
  switch (m_GenerateData.pCurrentItem->ItemType) {
    case OBRI_ITEMTYPE_ARMOR:
        SetArmorParams(ObCastClass(CObArmoRecord, pRecord));
	break;
    case OBRI_ITEMTYPE_CLOTHING:
        SetClothingParams(ObCastClass(CObClotRecord, pRecord));
	break;
    case OBRI_ITEMTYPE_LIGHT:
        SetLightParams(ObCastClass(CObLighRecord, pRecord));
	break;
    case OBRI_ITEMTYPE_MISC:
        SetMiscParams(ObCastClass(CObMiscRecord, pRecord));
	break;
    case OBRI_ITEMTYPE_SCROLL:
        SetBookParams(ObCastClass(CObBookRecord, pRecord));
	break;
    case OBRI_ITEMTYPE_SIGIL:
        SetSigilParams(ObCastClass(CObSgstRecord, pRecord));
	break;
    case OBRI_ITEMTYPE_STAFF:
	SetWeaponParams(ObCastClass(CObWeapRecord, pRecord));
	break;
    case OBRI_ITEMTYPE_WEAPON:
        SetWeaponParams(ObCastClass(CObWeapRecord, pRecord));
	break;
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetRecordItemParams()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetBipedParts (pRecord);
 *
 * Adds any biped part flag to the given record.
 *
 *=========================================================================*/
void CObriGenerate::SetBipedParts (CObRecord* pRecord) {

  if (pRecord == NULL) return;

  if (pRecord->GetName() == OB_NAME_ARMO) {
    CObArmoRecord* pArmor = ObCastClass(CObArmoRecord, pRecord);
    if (pArmor == NULL) return;
    pArmor->SetBipedFlags(m_GenerateData.pCurrentItem->Armor.BipedParts);
   }
  else if (pRecord->GetName() == OB_NAME_CLOT) {
    CObClotRecord* pClothing = ObCastClass(CObClotRecord, pRecord);
    if (pClothing == NULL) return;
    pClothing->SetBipedFlags(m_GenerateData.pCurrentItem->Clothing.BipedParts);
   }

 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetBodyParts()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetArmorParams (pArmor);
 *
 * Sets the parameters of the input armor according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetArmorParams (CObArmoRecord* pArmor) {
  
  if (pArmor == NULL) return;

  pArmor->SetWeight(m_GenerateData.pCurrentItem->Weight);
  pArmor->SetHeavyArmor(m_GenerateData.pCurrentItem->Armor.HeavyArmor);
  pArmor->SetArmor(m_GenerateData.pCurrentItem->Armor.Armor);
  pArmor->SetValue(m_GenerateData.pCurrentItem->BasePrice);
  pArmor->SetHealth(m_GenerateData.pCurrentItem->Armor.Health);
  pArmor->SetHideAmulet(m_GenerateData.pCurrentItem->Armor.HideAmulet);
  pArmor->SetHideRings(m_GenerateData.pCurrentItem->Armor.HideRings);

  if (m_GenerateData.pCurrentItem->MIcon[0]) pArmor->SetIcon(m_GenerateData.pCurrentItem->MIcon);
  if (m_GenerateData.pCurrentItem->FIcon[0]) pArmor->SetIcon2(m_GenerateData.pCurrentItem->FIcon);
  if (m_GenerateData.pCurrentItem->MBModel[0]) pArmor->SetModel(m_GenerateData.pCurrentItem->MBModel);
  if (m_GenerateData.pCurrentItem->MWModel[0]) pArmor->SetModel2(m_GenerateData.pCurrentItem->MWModel);
  if (m_GenerateData.pCurrentItem->FBModel[0]) pArmor->SetModel3(m_GenerateData.pCurrentItem->FBModel);
  if (m_GenerateData.pCurrentItem->FWModel[0]) pArmor->SetModel4(m_GenerateData.pCurrentItem->FWModel);
  
  SetBipedParts(pArmor);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetArmorParams()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetBookParams (pClothing);
 *
 * Sets the parameters of the input book according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetBookParams (CObBookRecord* pBook) {

  if (pBook == NULL) return;

  pBook->SetWeight(m_GenerateData.pCurrentItem->Weight);
  pBook->SetValue(m_GenerateData.pCurrentItem->BasePrice);
  pBook->SetTeaches(m_GenerateData.pCurrentItem->Scroll.Teaches);
  pBook->SetIsScroll(m_GenerateData.pCurrentItem->Scroll.IsScroll);

  pBook->SetIcon(m_GenerateData.pCurrentItem->Icon);
  pBook->SetModel(m_GenerateData.pCurrentItem->Model);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetBookParams()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetClothingParams (pClothing);
 *
 * Sets the parameters of the input clothing according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetClothingParams (CObClotRecord* pClothing) {

  if (pClothing == NULL) return;

  pClothing->SetWeight(m_GenerateData.pCurrentItem->Weight);
  pClothing->SetValue(m_GenerateData.pCurrentItem->BasePrice);

  if (m_GenerateData.pCurrentItem->MIcon[0]) pClothing->SetIcon(m_GenerateData.pCurrentItem->MIcon);
  if (m_GenerateData.pCurrentItem->FIcon[0]) pClothing->SetIcon2(m_GenerateData.pCurrentItem->FIcon);
  if (m_GenerateData.pCurrentItem->MBModel[0]) pClothing->SetModel(m_GenerateData.pCurrentItem->MBModel);
  if (m_GenerateData.pCurrentItem->MWModel[0]) pClothing->SetModel2(m_GenerateData.pCurrentItem->MWModel);
  if (m_GenerateData.pCurrentItem->FBModel[0]) pClothing->SetModel3(m_GenerateData.pCurrentItem->FBModel);
  if (m_GenerateData.pCurrentItem->FWModel[0]) pClothing->SetModel4(m_GenerateData.pCurrentItem->FWModel);

  SetBipedParts(pClothing);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetClothingParams()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetLightParams (pLight);
 *
 * Sets the parameters of the input light according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetLightParams (CObLighRecord* pLight) {
  
  if (pLight == NULL) return;

  pLight->SetWeight(m_GenerateData.pCurrentItem->Weight);
  pLight->SetValue(m_GenerateData.pCurrentItem->BasePrice);
  pLight->SetCarried(true);
  pLight->SetColor(m_GenerateData.pCurrentItem->Light.Color);
  pLight->SetDefaultOff(m_GenerateData.pCurrentItem->Light.DefaultOff);
  pLight->SetDynamic(m_GenerateData.pCurrentItem->Light.Dynamic);
  pLight->SetFalloff(m_GenerateData.pCurrentItem->Light.Falloff);
  pLight->SetFOV(m_GenerateData.pCurrentItem->Light.FOV);
  pLight->SetFade(m_GenerateData.pCurrentItem->Light.Fade);
  pLight->SetLightType(m_GenerateData.pCurrentItem->Light.Type);
  pLight->SetNegative(m_GenerateData.pCurrentItem->Light.Negative);
  pLight->SetRadius(m_GenerateData.pCurrentItem->Light.Radius);
  pLight->SetSpotLight(m_GenerateData.pCurrentItem->Light.SpotLight);
  pLight->SetSpotShadow(m_GenerateData.pCurrentItem->Light.SpotShadow);
  pLight->SetTime(m_GenerateData.pCurrentItem->Light.Time);
  pLight->SetCarried(m_GenerateData.pCurrentItem->Light.Carried);

	/* Set light sound */
  if (m_GenerateData.pCurrentItem->Light.Sound[0] != NULL_CHAR) {
    CObIdRecord* pRecord;
    obformid_t   FormID = OB_FORMID_NULL;

		/* Try and find the sound in the existing file */
    pRecord = m_EspFile.FindEditorID(m_GenerateData.pCurrentItem->Light.Sound);

		/* Look for the sound in the default oblivion.esm */
    if (pRecord == NULL) {
      pRecord = m_SoundRecords.FindEditorID(m_GenerateData.pCurrentItem->Light.Sound);
      if (pRecord == NULL) AddObGeneralError("The sound '%s' was not found!", m_GenerateData.pCurrentItem->Light.Sound);
     }

    if (pRecord != NULL) {
      FormID = pRecord->GetFormID();
     }

    if (FormID != OB_FORMID_NULL) pLight->SetSound(FormID);
   }
  
  if (m_GenerateData.pCurrentItem->Icon[0]) pLight->SetIcon(m_GenerateData.pCurrentItem->Icon);
  if (m_GenerateData.pCurrentItem->Model[0]) pLight->SetModel(m_GenerateData.pCurrentItem->Model);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetLightParams()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetMiscParams (pMisc);
 *
 * Sets the parameters of the input misc item according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetMiscParams (CObMiscRecord* pMisc) {

  if (pMisc == NULL) return;

  pMisc->SetValue(m_GenerateData.pCurrentItem->BasePrice);
  pMisc->SetWeight(m_GenerateData.pCurrentItem->Weight);

  pMisc->SetIcon(m_GenerateData.pCurrentItem->Icon);
  pMisc->SetModel(m_GenerateData.pCurrentItem->Model);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetMiscParams()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetSigilParams (pSigil);
 *
 * Sets the parameters of the input sigil stone according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetSigilParams (CObSgstRecord* pSigil) {

  if (pSigil == NULL) return;

  pSigil->SetWeight(m_GenerateData.pCurrentItem->Weight);
  pSigil->SetValue(m_GenerateData.pCurrentItem->BasePrice);

  pSigil->SetUses(m_GenerateData.pCurrentItem->Sigil.Uses);

  pSigil->SetIcon(m_GenerateData.pCurrentItem->Icon);
  pSigil->SetModel(m_GenerateData.pCurrentItem->Model);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetSigilParams()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriGenerate Method - void SetWeaponParams (pWeapon);
 *
 * Sets the parameters of the input weapon according to the current
 * item settings.
 *
 *=========================================================================*/
void CObriGenerate::SetWeaponParams (CObWeapRecord* pWeapon) {
  
  if (pWeapon == NULL) return;

  pWeapon->SetIgnoreResist(m_GenerateData.pCurrentItem->Weapon.IgnoreResist);
  pWeapon->SetWeight(m_GenerateData.pCurrentItem->Weight);
  pWeapon->SetEnchantPoints(m_GenerateData.pCurrentItem->Weapon.EnchantCharge);
  pWeapon->SetReach(m_GenerateData.pCurrentItem->Weapon.Reach);
  pWeapon->SetDamage(m_GenerateData.pCurrentItem->Weapon.Damage);
  pWeapon->SetSpeed(m_GenerateData.pCurrentItem->Weapon.Speed);
  pWeapon->SetType(m_GenerateData.pCurrentItem->Weapon.Type);
  pWeapon->SetValue(m_GenerateData.pCurrentItem->BasePrice);
  pWeapon->SetHealth(m_GenerateData.pCurrentItem->Weapon.Health);
  pWeapon->SetIcon(m_GenerateData.pCurrentItem->Icon);
  pWeapon->SetModel(m_GenerateData.pCurrentItem->Model);
 }
/*===========================================================================
 *		End of Class Method CObriGenerate::SetWeaponParams()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriGenerateOptions (Options);
 *
 *=========================================================================*/
void DefaultObriGenerateOptions (obri_generate_t& Options) {
  Options.LastFailed      = false;
  Options.HasEnchantment  = false;
  Options.pCurrentItem    = NULL;
  Options.pCurrentPrefix  = NULL;
  Options.pCurrentPrefix2 = NULL;
  Options.pCurrentSuffix  = NULL;
  Options.pCurrentUnique  = NULL;
 }
/*===========================================================================
 *		End of Function DefaultObriGenerateOptions()
 *=========================================================================*/



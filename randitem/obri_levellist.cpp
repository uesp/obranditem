/*===========================================================================
 *
 * File:	Obri_levellist.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 4, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_levellist.h"
#include "math.h"




/*===========================================================================
 *
 * Begin Value-Name Arrays
 *
 *=========================================================================*/
BEGIN_STRINGVALUE(l_ObriItemEffectTypes)
	ADD_STRINGVALUE( OBRI_ITEMEFFECT_NONE,		"None")
	ADD_STRINGVALUE( OBRI_ITEMEFFECT_PREFIX,	"Prefix")
	ADD_STRINGVALUE( OBRI_ITEMEFFECT_SUFFIX,	"Suffix")
	ADD_STRINGVALUE( OBRI_ITEMEFFECT_ONLYPREFIX,	"OnlyPrefix")
	ADD_STRINGVALUE( OBRI_ITEMEFFECT_ONLYSUFFIX,	"OnlySuffix")
	ADD_STRINGVALUE( OBRI_ITEMEFFECT_ANY,		"Any")
END_STRINGVALUE()
/*===========================================================================
 *		End of Value-Name Arrays
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Constructor
 *
 *=========================================================================*/
CObriLevelList::CObriLevelList () {
  m_pLevelList     = NULL;
  m_pEspFile       = NULL;
  m_IsEmpty        = false;
  m_MaxRecords     = 100;
  m_MinLevel       = 0;
  m_MaxLevel       = 100;
  m_ChanceNone     = 0;
  m_CalculateEach  = true;
  m_CalculateAll   = true;
  m_ItemTypes      = 0;
  m_BipedParts     = 0;
  m_NumWeaponTypes = 0;
  m_NumArmorTypes  = 0;
  m_BipedCompare   = OB_RECFILTER_BIPED_EQUAL;
  m_ListMask       = OBRI_LLMASK_NONE;
  m_IDCount        = OBRI_LLID_START;
  m_CreationCount  = 0;
  m_ItemCount      = 0;
  m_ResetLevels    = false;
 }
/*===========================================================================
*		End of Class CObriLevelList Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - void Destroy (void);
 *
 * Do not reset the ESP file for the object.
 *
 *=========================================================================*/
void CObriLevelList::Destroy (void) {
  m_Name.Empty();
  m_NameFilter.Empty();
  m_Parents.Empty();

  m_LevelLists.Destroy();
  m_CustomTypes.Empty();

  m_pLevelList     = NULL;
  m_NumWeaponTypes = 0;
  m_NumArmorTypes  = 0;
  m_ListMask       = OBRI_LLMASK_NONE;
  m_IDCount        = OBRI_LLID_START;
  m_CreationCount  = 0;
  m_ItemCount      = 0;
  m_ResetLevels    = false;
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::Destroy()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool AddParent (pValue);
 *
 * Adds a new parent to the list with the given name.
 *
 *=========================================================================*/
bool CObriLevelList::AddParent (const SSCHAR* pValue) {
  obri_listparent_t* pNewParent;

  pNewParent = new obri_listparent_t;
  m_Parents.Add(pNewParent);

  pNewParent->Name = pValue;

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::AddParent()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool AddRecord (pRecord);
 *
 * Adds the given record to the level list. Does not check if the record
 * belongs in the list or not. Creates a new list and any parents as required.
 *
 *=========================================================================*/
bool CObriLevelList::AddRecord (CObRecord* pRecord) {
  return AddRecord(pRecord->GetFormID(), pRecord->GetUserData(), 1);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::AddRecord()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool AddRecord (FormID, Level, Count);
 *
 *=========================================================================*/
bool CObriLevelList::AddRecord (const obformid_t FormID, int Level, int Count) {
  bool Result;
 
	/* See if we need to make a new list */
  if (m_pLevelList == NULL || m_pLevelList->GetItemCount() >= m_MaxRecords) {
    Result = MakeNewList();
    if (!Result) return (false);
   }

	/* Ensure the correct item level is used */
  if (m_ResetLevels) {
    Level = 1;
   }
  else {
    if (Level <= 0) Level = 1;
    if (Level > 65535) Level = 65535;
   }

  if (Count < 1) Count = 1;
  if (Count > 65535) Count = 65535;

	/* Add the item to the list */
  ++m_ItemCount;
  m_pLevelList->AddItem(FormID, (word) Level, (word) Count);

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::AddRecord()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool CheckItemEffects (pRecord, pPrefix, pPrefix1, pSuffix);
 *
 *=========================================================================*/
bool CObriLevelList::CheckItemEffects (CObRecord* pRecord, const void* pPrefix, const void* pPrefix1, const void* pSuffix) {
  bool Result = false;

	/* Special case for allowing any item type */
  if ((m_ItemEffectType & OBRI_ITEMEFFECT_ANY) != 0 || m_ItemEffectType == 0) {
    Result = true;
   }

	/* Include only items without any effects */
  if ((m_ItemEffectType & OBRI_ITEMEFFECT_NONE) != 0) {
    if (pPrefix == NULL && pPrefix1 == NULL && pSuffix == NULL) Result = true;
   }

	/* Include items that have a suffix */
  if ((m_ItemEffectType & OBRI_ITEMEFFECT_SUFFIX) != 0) {
    if (pSuffix != NULL) Result = true;
   }

	/* Include items that have a prefix */
  if ((m_ItemEffectType & OBRI_ITEMEFFECT_PREFIX) != 0) {
    if (pPrefix != NULL || pPrefix1 != NULL) Result = true;
   }

	/* Include items that have a suffix but not a prefix */
  if ((m_ItemEffectType & OBRI_ITEMEFFECT_ONLYSUFFIX) != 0) {
    if (pSuffix != NULL && pPrefix == NULL && pPrefix1 == NULL) Result = true;
   }

	/* Include items that have a prefix but not a suffix */
  if ((m_ItemEffectType & OBRI_ITEMEFFECT_ONLYPREFIX) != 0) {
    if ((pPrefix != NULL || pPrefix1 == NULL) && pSuffix == NULL) Result = true;
   }

  return (Result);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::CheckItemEffects()
 *=========================================================================*/



/*===========================================================================
 *
 * Class CObriLevelList Method - bool CheckItemType (pRecord);
 *
 * Checks the type of the given record to see if it matches that of the
 * list definition. Returns true if it matches.
 *
 *=========================================================================*/
bool CObriLevelList::CheckItemType (CObRecord* pRecord) {

  if ((m_ItemTypes & OBRI_ITEMTYPE_ARMOR) != 0) {
    return (pRecord->GetName() == OB_NAME_ARMO);
   }
  else if ((m_ItemTypes & OBRI_ITEMTYPE_WEAPON) != 0) {
    if (pRecord->GetName() != OB_NAME_WEAP) return (false);
    CObWeapRecord* pWeapon = ObCastClass(CObWeapRecord, pRecord);
    if (pWeapon == NULL) return (false);
    return (pWeapon->GetType() != OB_WEAPTYPE_STAFF);
   }
  else if ((m_ItemTypes & OBRI_ITEMTYPE_CLOTHING) != 0) {
    return (pRecord->GetName() == OB_NAME_CLOT);
   }
  else if ((m_ItemTypes & OBRI_ITEMTYPE_MISC) != 0) {
    return (pRecord->GetName() == OB_NAME_MISC);
   }
  else if ((m_ItemTypes & OBRI_ITEMTYPE_SCROLL) != 0) {
    return (pRecord->GetName() == OB_NAME_BOOK);
   }
  else if ((m_ItemTypes & OBRI_ITEMTYPE_LIGHT) != 0) {
    return (pRecord->GetName() == OB_NAME_LIGH);
   }
  else if ((m_ItemTypes & OBRI_ITEMTYPE_STAFF) != 0) {
    if (pRecord->GetName() != OB_NAME_WEAP) return (false);
    CObWeapRecord* pWeapon = ObCastClass(CObWeapRecord, pRecord);
    if (pWeapon == NULL) return (false);
    return (pWeapon->GetType() == OB_WEAPTYPE_STAFF);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::CheckItemType()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool CheckArmorTypes (pRecord);
 *
 * Checks the armor type of the given record to see if it matches that 
 * of the list definition. Returns true if it matches.
 *
 *=========================================================================*/
bool CObriLevelList::CheckArmorTypes (CObRecord* pRecord) {
  CObArmoRecord* pArmor;
  dword          Index;

	/* Ensure the input record is the correct type */
  pArmor = ObCastClass(CObArmoRecord, pRecord);
  if (pArmor == NULL) return (false);

  for (Index = 0; Index < m_NumArmorTypes; ++Index) {
    if (pArmor->IsLightArmor() && m_ArmorTypes[Index] == OBRI_LLARMOR_LIGHT) return (true);
    if (pArmor->IsHeavyArmor() && m_ArmorTypes[Index] == OBRI_LLARMOR_HEAVY) return (true);
   }

  return (false);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::CheckArmorTypes()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool CheckWeaponTypes (pRecord);
 *
 * Checks the weapon type of the given record to see if it matches that 
 * of the list definition. Returns true if it matches.
 *
 *=========================================================================*/
bool CObriLevelList::CheckWeaponTypes (CObRecord* pRecord) {
  CObWeapRecord* pWeapon;
  dword          Index;

	/* Ensure the input record is the correct type */
  pWeapon = ObCastClass(CObWeapRecord, pRecord);
  if (pWeapon == NULL) return (false);

  for (Index = 0; Index < m_NumWeaponTypes; ++Index) {
    if (pWeapon->GetType() == m_WeaponTypes[Index]) return (true);
   }

  return (false);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::CheckWeaponTypes()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool CheckItemMasks (pRecord, pCustomType);
 *
 * Checks the item masks of the given record to see if it matches that 
 * of the list definition. Returns true if it matches.
 *
 *=========================================================================*/
bool CObriLevelList::CheckItemMasks (CObRecord* pRecord, const SSCHAR* pCustomType) {
  dword Index;

  for (Index = 0; Index < m_CustomTypes.GetSize(); ++Index) {
    if (stricmp(pCustomType, *m_CustomTypes[Index]) == 0) return (true);
   }

	/* No matches */
  return (false);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::CheckItemMasks()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool CheckRecord (pRecord, pCustomType, pPrefix, pPrefix1, pSuffix);
 *
 * Checks if the given record can be added to the list. Returns true
 * if it can.
 *
 *=========================================================================*/
bool CObriLevelList::CheckRecord (CObRecord* pRecord, const SSCHAR* pCustomType, const void* pPrefix, const void* pPrefix1, const void* pSuffix) {
  CObItem1Record* pItem1;
  CObArmoRecord*  pArmor;
  CObClotRecord*  pClothing;
  dword		  BipedFlags;
  bool		  Result;

	/* Ignore invalid input */
  if (pRecord == NULL) return (false);
  if (m_IsEmpty)       return (false);

	/* Do the record types match? */
  if (IsMaskItemType()) {
    Result = CheckItemType(pRecord);
    if (!Result) return (false);
   }

	/* Do the item names match? */
  if (IsMaskItemName()) {
    pItem1 = ObCastClass(CObItem1Record, pRecord);
    if (pItem1 == NULL) return (false);
    if (stristr(m_NameFilter, pItem1->GetItemName()) != 0) return (false);
   }

	/* Do the weapon types match? */
  if (IsMaskWeaponTypes()) {
    Result = CheckWeaponTypes(pRecord);
    if (!Result) return (false);
   }

	/* Do the armor types match? */
  if (IsMaskArmorTypes()) {
    Result = CheckArmorTypes(pRecord);
    if (!Result) return (false);
   }

	/* Do the biped parts match? */
  if (IsMaskBipedParts()) {
    if (pRecord->GetName() == OB_NAME_ARMO) {
      pArmor = ObCastClass(CObArmoRecord, pRecord);
      if (pArmor == NULL) return (false);
      BipedFlags = pArmor->GetBipedFlags();
     }
    else if (pRecord->GetName() == OB_NAME_CLOT) {
      pClothing = ObCastClass(CObClotRecord, pRecord);
      if (pClothing == NULL) return (false);
      BipedFlags = pClothing->GetBipedFlags();
     }
    else {
      return (false);
     }

    switch (m_BipedCompare) {
      case OB_RECFILTER_BIPED_AND:
	if ((BipedFlags & m_BipedParts) != m_BipedParts) return (false);
	break;
      case OB_RECFILTER_BIPED_OR:
	if ((BipedFlags & m_BipedParts) == 0) return (false);
	break;
      case OB_RECFILTER_BIPED_EQUAL:
      default:
	if (BipedFlags != m_BipedParts) return (false);
	break;
     }
   }

	/* Do any item masks match? */
  if (IsMaskItemMasks()) {
    Result = CheckItemMasks(pRecord, pCustomType);
    if (!Result) return (false);
   }

	/* Does the item level match? */
  if (IsMaskLevel()) {
    if (pRecord->GetUserData() < m_MinLevel || pRecord->GetUserData() > m_MaxLevel) return (false);
   }
  else if (IsMaskMinLevel()) {
    if (pRecord->GetUserData() < m_MinLevel) return (false);
   }
  else if (IsMaskMaxLevel()) {
    if (pRecord->GetUserData() > m_MaxLevel) return (false);
   }

	/* Check item effect types */
  if (IsMaskItemEffects()) {
    Result = CheckItemEffects(pRecord, pPrefix, pPrefix1, pSuffix);
    if (!Result) return (false);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::CheckRecord()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - void DistributeLists (void);
 *
 * Ensures all the lists have approximately the same number of items
 * in them.
 *
 *=========================================================================*/
void CObriLevelList::DistributeLists (void) {
  CObLvliRecord*    pLevelList;
  CObLvliRecord*    pLastList;
  CObLvloSubrecord* pItem;
  dword             Index;
  int               Index1;
  float             DesiredCount;
  float             RunningCount;
  int		    ExtraCount;

  if (m_LevelLists.GetSize() <= 1) return;

  DesiredCount =  (float) m_ItemCount / m_CreationCount;
  pLastList    = m_LevelLists.GetAt(m_LevelLists.GetSize() - 1);
  RunningCount = 0;

	/* Move extra items from all lists into the last list */
  for (Index = 0; Index < m_LevelLists.GetSize() - 1; ++Index) {
    pLevelList = m_LevelLists.GetAt(Index);

    RunningCount += DesiredCount;
    ExtraCount    = (int) ceil(pLevelList->GetItemCount() - RunningCount);
    RunningCount  = RunningCount - (float) pLevelList->GetItemCount() + ExtraCount;

    for (Index1 = 0; Index1 < ExtraCount; ++Index1) {
      pItem = pLevelList->FindItem(pLevelList->GetItemCount() - 1);
      if (pItem == NULL) continue;

		/* Add the item to the last list */
      pLastList->AddItem(pItem->GetFormID(), pItem->GetLevel(), pItem->GetCount());

		/* Delete the item from the current list */
      pLevelList->DeleteItem(pItem);
     }

   }

 }
/*===========================================================================
 *		End of Class Method CObriLevelList::DistributeLists()
 *=========================================================================*/


/*===========================================================================
 *
 * Class SSCHAR* CObriLevelList Method - const GetParentName (Index);
 *
 *=========================================================================*/
const SSCHAR* CObriLevelList::GetParentName (const dword Index) { 
  obri_listparent_t* pParent = m_Parents[Index];

  if (pParent == NULL) return (NULL);
  return (pParent->Name);
 }
/*===========================================================================
 *		End of Class Method SSCHAR* CObriLevelList::GetParentName()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool MakeNewList (void);
 *
 * Protected class method to make a new level list record. Returns false
 * on any error.
 *
 *=========================================================================*/
bool CObriLevelList::MakeNewList (void) {
  CObBaseRecord* pRecord;

	/* Create a new list record */
  if (m_pEspFile == NULL) return (false);
  pRecord = m_pEspFile->CreateNewRecord(OB_NAME_LVLI);
  if (pRecord == NULL) return (false);
  m_pLevelList = ObCastClass(CObLvliRecord, pRecord);
  if (m_pLevelList == NULL) return (false);

  ++m_CreationCount;
  m_LevelLists.Add(m_pLevelList);

	/* Initialize the new record */
  m_pLevelList->SetFormID(ObriGetNextFormID2(m_pEspFile));
  m_pLevelList->SetEditorID(MakeListEditorID());
  m_pEspFile->IndexRecord(m_pLevelList);

  m_pLevelList->SetChanceNone(m_ChanceNone);
  m_pLevelList->SetCalculateAllLevels(m_CalculateAll);
  m_pLevelList->SetCalculateEachItem(m_CalculateEach);

	/* Add list to any parents */
  return AddToParents(m_pLevelList);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::MakeNewList()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool AddToParents (pRecord);
 *
 * Adds the given record to all parents of the list.
 *
 *=========================================================================*/
bool CObriLevelList::AddToParents (CObRecord* pRecord) {
  obri_listparent_t* pListParent;
  dword Index;

  for (Index = 0; Index < m_Parents.GetSize(); ++Index) { 
    pListParent = m_Parents[Index];
    if (pListParent->pParent != NULL) pListParent->pParent->AddRecord(pRecord);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::AddToParents()
 *=========================================================================*/


/*===========================================================================
 *
 * Class SSCHAR* CObriLevelList Method - const MakeListEditorID (void);
 *
 * Makes a new list ID checking with the file to avoid duplicates.
 *
 *=========================================================================*/
const SSCHAR* CObriLevelList::MakeListEditorID (void) {
  static CSString s_Buffer;
  CObIdRecord* pRecord = NULL;

  do {
    if (m_IDCount == OBRI_LLID_START && m_IsEmpty) {
      s_Buffer = m_Name;
     }
    else {
      s_Buffer.Format("%s%d", m_Name, m_IDCount);
     }
  
    ++m_IDCount;
    if (m_pEspFile != NULL) pRecord = m_pEspFile->FindEditorID(s_Buffer);
  } while (pRecord != NULL);

  return (s_Buffer);
 }
/*===========================================================================
 *		End of Class Method SSCHAR* CObriLevelList::MakeListEditorID()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool ParseArmorTypes (pString);
 *
 * Protected class method to parse an armor type string of the form:
 *
 *	Type | Type
 *
 *=========================================================================*/
bool CObriLevelList::ParseArmorTypes (const SSCHAR* pString) {
  SSCHAR   Buffer[OBRI_MAX_BUFFERSIZE + 1];
  SSCHAR*  pParse;
  
  strnncpy(Buffer, pString, OBRI_MAX_BUFFERSIZE);
  pParse = strtok(Buffer, OBRI_ITEMSEPSTRING);

  while (pParse != NULL) {
    pParse = trim(pParse);

    if (m_NumArmorTypes >= OBRI_LLNUM_TYPES) {
      AddObGeneralError("Exceeded the maximum number of armor types (%d)!", OBRI_LLNUM_TYPES);
      return (false);
     }

    if (stricmp(pParse, "Light") == 0) {
      m_ArmorTypes[m_NumArmorTypes] = OBRI_LLARMOR_LIGHT;
      ++m_NumArmorTypes;
     }
    else if (stricmp(pParse, "Heavy") == 0) {
      m_ArmorTypes[m_NumArmorTypes] = OBRI_LLARMOR_HEAVY;
      ++m_NumArmorTypes;
     }
    else {
      AddObGeneralError("Unknown armor type '%s'!", pParse);
     }
    
    pParse = strtok(NULL, OBRI_ITEMSEPSTRING);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::ParseArmorTypes()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool ParseWeaponTypes (pString);
 *
 * Protected class method to parse a weapon type string of the form:
 *
 *	Type | Type
 *
 *=========================================================================*/
bool CObriLevelList::ParseWeaponTypes (const SSCHAR* pString) {
  SSCHAR   Buffer[OBRI_MAX_BUFFERSIZE + 1];
  SSCHAR*  pParse;
  int      Type;
  bool     Result;
  
  strnncpy(Buffer, pString, OBRI_MAX_BUFFERSIZE);
  pParse = strtok(Buffer, OBRI_ITEMSEPSTRING);

  while (pParse != NULL) {
    pParse = trim(pParse);

    Result = GetObWeaponTypeValue(Type, pParse);

    if (Result) {
      if (m_NumWeaponTypes >= OBRI_LLNUM_TYPES) {
        AddObGeneralError("Exceeded the maximum number of weapons types (%d)!", OBRI_LLNUM_TYPES);
        return (false);
       }
      
      m_WeaponTypes[m_NumWeaponTypes] = Type;
      ++m_NumWeaponTypes;
     }
    else {
      AddObGeneralError("Unknown weapon type '%s'!", pParse);
     }

    pParse = strtok(NULL, OBRI_ITEMSEPSTRING);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::ParseWeaponTypes()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - void Reset (void);
 *
 * Called before a new set of items is generated.
 *
 *=========================================================================*/
void CObriLevelList::Reset (void) {
  m_IDCount       = OBRI_LLID_START;
  m_CreationCount = 0;
  m_ItemCount     = 0;
  m_pLevelList    = NULL;

  m_LevelLists.Destroy();

 }
/*===========================================================================
 *		End of Class Method CObriLevelList::Reset()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelList Method - bool SetParameter (pVariable, pValue);
 *
 * Sets a level list parameter based on the input values.
 *
 *=========================================================================*/
bool CObriLevelList::SetParameter (const SSCHAR* pVariable, const SSCHAR* pValue) {
  bool Result;
  int  Value;

  if (stricmp(pVariable, "ItemType") == 0) {
    m_ItemTypes = StringToObriItemMask(pValue);
    m_ListMask |= OBRI_LLMASK_ITEMTYPES;
   }
  else if (stricmp(pVariable, "Name") == 0) {
    m_Name = pValue;
   }
  else if (stricmp(pVariable, "NameFilter") == 0) {
    m_NameFilter = pValue;
    m_ListMask |= OBRI_LLMASK_ITEMNAME;
   }
  else if (stricmp(pVariable, "Parent") == 0) {
    
    if (stricmp(pValue, "NULL") != 0) {
      AddParent(pValue);
     }
   }
  else if (stricmp(pVariable, "CustomTypes") == 0) {
    CSString* pString = m_CustomTypes.AddNew();
    *pString = pValue;
    m_ListMask |= OBRI_LLMASK_CUSTOMTYPES;
   }
  else if (stricmp(pVariable, "MaxRecords") == 0) {
    Value = atol(pValue);

    if (Value <= 0) 
      AddObGeneralError("Invalid MaxRecords value '%s'!", pValue);
    else
      m_MaxRecords = (dword)Value;
   }
  else if (stricmp(pVariable, "ChanceNone") == 0) {
    Value = atol(pValue);

    if (Value < 0) 
      AddObGeneralError("Invalid ChanceNone value '%s'!", pValue);
    else
      m_ChanceNone = (dword)Value;
   }
  else if (stricmp(pVariable, "CalculateEach") == 0) {
    Result = StringToBoolean(m_CalculateEach, pValue);
    if (!Result) AddObGeneralError("Invalid boolean value '%s'!", pValue);
   }
  else if (stricmp(pVariable, "CalculateAll") == 0) {
    Result = StringToBoolean(m_CalculateAll, pValue);
    if (!Result) AddObGeneralError("Invalid boolean value '%s'!", pValue);
   }
  else if (stricmp(pVariable, "MinLevel") == 0) {
    m_MinLevel = atol(pValue);
    m_ListMask |= OBRI_LLMASK_MINLEVEL;
   }
  else if (stricmp(pVariable, "MaxLevel") == 0) {
    m_MaxLevel = atol(pValue);
    m_ListMask |= OBRI_LLMASK_MAXLEVEL;
   }
  else if (stricmp(pVariable, "Empty") == 0) {
    Result = StringToBoolean(m_IsEmpty, pValue);
    if (!Result) AddObGeneralError("Invalid boolean value '%s'!", pValue);
   }
  else if (stricmp(pVariable, "ResetLevels") == 0) {
    Result = StringToBoolean(m_ResetLevels, pValue);
    if (!Result) AddObGeneralError("Invalid boolean value '%s'!", pValue);
   }
  else if (stricmp(pVariable, "ArmorTypes") == 0) {
    ParseArmorTypes(pValue);
    m_ListMask |= OBRI_LLMASK_ARMORTYPES;
   }
  else if (stricmp(pVariable, "WeaponTypes") == 0) {
    ParseWeaponTypes(pValue);
    m_ListMask |= OBRI_LLMASK_WEAPONTYPES;
   }
  else if (stricmp(pVariable, "BipedParts") == 0) {
    m_BipedParts = ParseObBipedFlagString(pValue);
    m_ListMask |= OBRI_LLMASK_BIPEDPARTS;
   }
  else if (stricmp(pVariable, "BipedCompare") == 0) {
    if (stricmp(pValue, "AND") == 0) {
      m_BipedCompare = OB_RECFILTER_BIPED_AND;
     }
    else if (stricmp(pValue, "OR") == 0) {
      m_BipedCompare = OB_RECFILTER_BIPED_OR;
     }
    else if (stricmp(pValue, "Equal") == 0) {
      m_BipedCompare = OB_RECFILTER_BIPED_EQUAL;
     }
    else {
      AddObGeneralError("Invalid biped compare type '%s'!", pValue);
     }
   }
  else if (stricmp(pVariable, "ItemEffects") == 0) {
    SSCHAR Buffer[512];
    SSCHAR* pParse;
    strnncpy(Buffer, pValue, 500);

    pParse = strtok(Buffer, "|");

    while (pParse != NULL) {
      pParse = trim(pParse);

      Result = FindStringValue(m_ItemEffectType, pParse, l_ObriItemEffectTypes);

      if (!Result) 
        AddObGeneralError("Invalid level list item effect type '%s'!", pParse);
      else
        m_ListMask |= OBRI_LLMASK_ITEMEFFECT;

      pParse = strtok(NULL, "|");
     }
  
   
   }
  else {
    AddObGeneralError("Unknown level list variable '%s'!", pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelList::SetParameter()
 *=========================================================================*/


/*===========================================================================
 *
 * Class obri_listinsert_t Method - bool SetParameter (pVariable, pValue);
 *
 *=========================================================================*/
bool obri_listinsert_t::SetParameter (const SSCHAR* pVariable, const SSCHAR* pValue) {
  
  if (stricmp(pVariable, "EditorID") == 0) {
    EditorID = pValue;
   }
  else if (stricmp(pVariable, "Count") == 0) {
    Count = atoi(pValue);
   }
  else if (stricmp(pVariable, "Level") == 0) {
    Level = atoi(pValue);
   }
  else if (stricmp(pVariable, "Parent") == 0) {
    obri_listparent_t* pParent;

    pParent = new obri_listparent_t;
    Lists.Add(pParent);
    pParent->Name = pValue;
   }
  else {
    AddObGeneralError("Unknown level list variable '%s'!", pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method obri_listinser_t::SetParameter()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Constructor
 *
 *=========================================================================*/
CObriLevelLists::CObriLevelLists () {
  m_pEspFile = NULL;
 }
/*===========================================================================
 *		End of Class CObriLevelLists Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - void Destroy (void);
 *
 *=========================================================================*/
void CObriLevelLists::Destroy (void) {
  m_ListInserts.Destroy();
  m_LevelLists.Destroy();
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::Destroy()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - bool AddInsertItems (pListInsert);
 *
 *=========================================================================*/
bool CObriLevelLists::AddInsertItems (obri_listinsert_t* pListInsert) { 
  obri_listparent_t* pListParent;
  dword		     Index;
  
  if (pListInsert == NULL) return (false);
  if (pListInsert->FormID == OB_FORMID_NULL) return (false);

  for (Index = 0; Index < pListInsert->Lists.GetSize(); ++Index) { 
    pListParent = pListInsert->Lists[Index];
    if (pListParent == NULL || pListParent->pParent == NULL) continue;
 
    pListParent->pParent->AddRecord(pListInsert->FormID, pListInsert->Level, pListInsert->Count);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::AddInsertItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - bool AddInsertItems (void);
 *
 * Adds all predefined items to the lists.
 *
 *=========================================================================*/
bool CObriLevelLists::AddInsertItems (void) { 
  dword Index;

  for (Index = 0; Index < m_ListInserts.GetSize(); ++Index) {
    AddInsertItems(m_ListInserts[Index]);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::AddInsertItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - int AddRecord (pRecord, pCustomType, pPrefix, pPrefix1, pSuffix);
 *
 * Adds the given record to any matching list. Returns the number of
 * lists the item was added to. Uses the given ItemMask for the record.
 *
 *=========================================================================*/
int CObriLevelLists::AddRecord (CObRecord* pRecord, const SSCHAR* pCustomType, const void* pPrefix, const void* pPrefix1, const void* pSuffix) {
  CObriLevelList* pLevelList;
  dword           Index;
  bool		  Result;
  int             ListCount = 0;

  for (Index = 0; Index < m_LevelLists.GetSize(); ++Index) { 
    pLevelList = m_LevelLists[Index];

    if (pLevelList->CheckRecord(pRecord, pCustomType, pPrefix, pPrefix1, pSuffix)) {
      Result = pLevelList->AddRecord(pRecord);
      if (Result) ++ListCount;
     }
   }

  return (ListCount);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::AddRecord()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - bool CreateParents (void);
 *
 * Creates all top level parent lists (defined as Empty=true).
 *
 *=========================================================================*/
bool CObriLevelLists::CreateParents (void) {
  CObriLevelList* pList;
  dword		  Index;

	/* Find and create parents */
  for (Index = 0; Index < m_LevelLists.GetSize(); ++Index) {
    pList = m_LevelLists[Index];

    if (pList->IsEmpty()) {
      if (pList->GetLevelList() == NULL) pList->MakeNewList();
     }
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::ResolveParents()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - void DistributeLists (void);
 *
 *=========================================================================*/
void CObriLevelLists::DistributeLists (void) {
  dword Index;

  for (Index = 0; Index < m_LevelLists.GetSize(); ++Index) {
    m_LevelLists[Index]->DistributeLists();
   }

 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::DistributeLists()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - CObriLevelList* Find (pString);
 *
 * Find a list based on its name. Returns NULL if it is not found;
 *
 *=========================================================================*/
CObriLevelList* CObriLevelLists::Find (const SSCHAR* pString) {
  CObriLevelList* pList;
  dword           Index;

  for (Index = 0; Index < m_LevelLists.GetSize(); ++Index) {
    pList = m_LevelLists[Index];
    if (stricmp(pList->GetName(), pString) == 0) return (pList);
   }

	/* Not found */
  return (NULL);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::Find()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - dword GetCreationCount (void);
 *
 * Gets the total number of created lists.
 *
 *=========================================================================*/
dword CObriLevelLists::GetCreationCount (void) {
  dword Index;
  dword TotalCount = 0;

  for (Index = 0; Index < m_LevelLists.GetSize(); ++Index) {
    TotalCount += m_LevelLists[Index]->GetCreationCount();
   }

  return (TotalCount);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::GetCreationCount()
 *=========================================================================*/


/*===========================================================================
 *
 *  Class CObriLevelLists Method - bool Load (pFilename);
 *
 *=========================================================================*/
bool CObriLevelLists::Load (const SSCHAR* pFilename) {
  CObriLevelList*    pLevelList;
  obri_listinsert_t* pListInsert;
  CSString           Buffer;
  CSString	     Variable;
  CSString	     Value;
  CObFile	     File;
  dword		     InputLine;
  long		     Filesize;
  bool		     Result;
  bool		     SkipList;

	/* Clear the current list data, if any */
  m_LevelLists.Destroy();
  m_ListInserts.Destroy();

	/* Attempt to open the file for input */
  Result = File.Open(pFilename, "rt");
  if (!Result) return (false);

  Filesize    = File.GetFileSize();
  InputLine   = 0;
  pLevelList  = NULL;
  pListInsert = NULL;
  SkipList    = false;

	/* Read and parse entire file */
  while (!File.IsEOF() && File.Tell() < Filesize) {
    Result = File.ReadLine(Buffer);
    if (!Result) return (false);
    ++InputLine;

    Result = Buffer.SeperateVarValue(Variable, Value);

    if (pLevelList != NULL) {
      if (Variable.CompareNoCase("End") == 0) {
	pLevelList  = NULL;
	pListInsert = NULL;
	SkipList    = false;
       }
     else if (pLevelList != NULL && Result && !SkipList) {

		/* Check for a duplicate list name */
       if (Variable.CompareNoCase("Name") == 0) {
         if (Find(Value) != NULL) {
	   AddObGeneralError("%5lu: Duplicate list name found (%s)!", InputLine, Value.c_str());
           SkipList = true;
           continue;
	  }
        }

       Result = pLevelList->SetParameter(Variable, Value);
       //if (!Result) AddObGeneralError("%5lu: Invalid filter variable/value found: %s = %s", InputLine, Variable.c_str(), Value.c_str());
      }
     else {
       AddObGeneralError("%5lu: Unknown variable/value pair found: %s = %s", InputLine, Variable.c_str(), Value.c_str());
      }

     }
    else if (pListInsert != NULL && Result) {
      Result = pListInsert->SetParameter(Variable, Value);
     }
    else if (Variable.CompareNoCase("LevelList") == 0) {
      pLevelList = new CObriLevelList;
      m_LevelLists.Add(pLevelList);
      pLevelList->SetEspFile(m_pEspFile);
      SkipList    = false;
      pListInsert = NULL;
     }
    else if (Variable.CompareNoCase("Insertion") == 0) {
      pListInsert = new obri_listinsert_t;
      m_ListInserts.Add(pListInsert); 
      pLevelList = NULL;
     }
   }

  if (pLevelList != NULL) {
    AddObGeneralError("%5lu: Missing 'End' tag!", InputLine);
   }

	/* Set the parent links for all lists */
  ResolveParents();

  SystemLog.Printf("Loaded %d level list definitions", m_LevelLists.GetSize());
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::Load()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - void Reset (void);
 *
 * Called before a new set of items is generated.
 *
 *=========================================================================*/
void CObriLevelLists::Reset (void) {
  dword Index;

  for (Index = 0; Index < m_LevelLists.GetSize(); ++Index) {
    m_LevelLists[Index]->Reset();
   }

 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::Reset()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - bool ResolveInsertItems (void);
 *
 * Resolves all insert items form IDs.
 *
 *=========================================================================*/
bool CObriLevelLists::ResolveInsertItems (void) {
  obri_listinsert_t* pListInsert;
  CObIdRecord*       pRecord;
  dword		     Index;

	/* Ensure the file is valid */
  if (m_pEspFile == NULL) return (false);

	/* Item insert parents */
  for (Index = 0; Index < m_ListInserts.GetSize(); ++Index) {
    pListInsert = m_ListInserts[Index];
    if (pListInsert == NULL) continue;

    pRecord = m_pEspFile->FindEditorID(pListInsert->EditorID);

    if (pRecord == NULL) {
      pListInsert->FormID = OB_FORMID_NULL;
      pRecord->SetUserData(0);
     }
    else {
      pListInsert->FormID = pRecord->GetFormID();
      pRecord->SetUserData(pListInsert->Level);
     }

   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::ResolveInsertItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - bool ResolveInsertParents (pListInsert);
 *
 * Resolves a parents for a single insert list.
 *
 *=========================================================================*/
bool CObriLevelLists::ResolveInsertParents (obri_listinsert_t* pListInsert) {
  obri_listparent_t* pListParent;
  CObriLevelList*    pParent;
  dword		     Index;
   
	/* Resolve all parent lists in the insert definition */
  for (Index = 0; Index < pListInsert->Lists.GetSize(); ++Index) {
    pListParent = pListInsert->Lists[Index];
    if (pListParent == NULL) continue;

    pParent = Find(pListParent->Name);

    if (pParent == NULL) {
      AddObGeneralError("The list insert's parent '%s' was not found!", pListParent->Name.c_str());
      pListParent->pParent = NULL;
     }
    else { 
      pListParent->pParent = pParent;
     }
   }    

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::ResolveInsertParents()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - bool ResolveListParents (pListParent);
 *
 * Resolves a parents for a single list.
 *
 *=========================================================================*/
bool CObriLevelLists::ResolveListParents (CObriLevelList* pList) {
  obri_listparent_t* pListParent;
  CObriLevelList*    pParent;
  dword		     Index;
  
  for (Index = 0; Index < pList->GetNumParents(); ++Index) {
    pListParent = pList->GetParent(Index);
    if (pListParent == NULL) continue;

    pParent = Find(pListParent->Name);

    if (pParent == NULL) {
      AddObGeneralError("The level list's parent '%s' was not found!", pListParent->Name.c_str());
      pListParent->pParent = NULL;
     }
    else { 
      pListParent->pParent = pParent;
     }
   }    

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::ResolveListParents()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriLevelLists Method - bool ResolveParents (void);
 *
 * Resolves the parent links in newly loaded data.
 *
 *=========================================================================*/
bool CObriLevelLists::ResolveParents (void) {
  dword Index;

	/* List parents */
  for (Index = 0; Index < m_LevelLists.GetSize(); ++Index) {
    ResolveListParents(m_LevelLists[Index]);
   }

	/* Item insert parents */
  for (Index = 0; Index < m_ListInserts.GetSize(); ++Index) {
    ResolveInsertParents(m_ListInserts[Index]);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriLevelLists::ResolveParents()
 *=========================================================================*/






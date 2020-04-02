/*===========================================================================
 *
 * File:	Obri_BaseItems.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	20 April 2006
 *
 * Defines the CObriBaseItems class for handling base item data.
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_baseitems.h"


/*===========================================================================
 *
 * Class CObriBaseItems Constructor
 *
 *=========================================================================*/
CObriBaseItems::CObriBaseItems () {
  m_InputLine = 0;
 }
/*===========================================================================
 *		End of Class CObriBaseItems Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - void Destroy (void);
 *
 *=========================================================================*/
void CObriBaseItems::Destroy (void) {

	/* Clear the array */
  m_BaseItems.Destroy();
  m_InputLine = 0;
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::Destroy()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - int FindBodyPart (pString);
 *
 * Find and return a body part index of the given string within the 
 * l_MwItemBipeds array. Returns -1 if the part is not found.
 *
 *=========================================================================*/
int CObriBaseItems::FindBodyPart (const SSCHAR* pString) {
  int  FlagValue;
  bool Result;

  Result = GetObBipedFlagValue(FlagValue, pString);

  if (!Result) {
    AddObGeneralError("%5ld: Unknown body part '%s'!", m_InputLine, pString);
    return (OB_BIPEDFLAG_HEAD);
   }

  return (FlagValue);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::FindBodyPart()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - dword ParseBipedFlag (SSCHAR* pValue);
 *
 * Parses and returns the biped flagvalue from the given string.
 *
 *=========================================================================*/
dword CObriBaseItems::ParseBipedFlag (const SSCHAR* pValue) {
  SSCHAR  Buffer[OBRI_MAX_BUFFERSIZE+1];
  SSCHAR* pParse;
  SSCHAR* pParse1;
  dword   Mask = 0;
  int     PartValue;
  bool    Result;

	/* Create a copy of the input value for parsing */
  strnncpy(Buffer, pValue, OBRI_MAX_BUFFERSIZE);
  strlwr(Buffer);

	/* Parse the entire string */
  pParse = strtok(Buffer, OBRI_ITEMSEPSTRING);

  while (pParse != NULL) {
    pParse1 = trim(pParse);
    
		/* Find the effect mask */
    Result = GetObBipedFlagValue(PartValue, pParse1);

    if (!Result)
      AddObGeneralError("%5ld: Unknown biped part '%s'!", m_InputLine, pParse1);
    else
      Mask |= (dword) PartValue;

    pParse = strtok(NULL, OBRI_ITEMSEPSTRING);
   }

  return (Mask);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::ParseBipedFlag()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - dword ParseEffectMask (SSCHAR* pValue);
 *
 * Parses and returns the effect mask value from the given string.
 *
 *=========================================================================*/
dword CObriBaseItems::ParseEffectMask (const SSCHAR* pValue) {
  SSCHAR  Buffer[OBRI_MAX_BUFFERSIZE+1];
  SSCHAR* pParse;
  SSCHAR* pParse1;
  dword   Mask = 0;
  int     MaskValue;
  bool    Result;

	/* Create a copy of the input value for parsing */
  strnncpy(Buffer, pValue, OBRI_MAX_BUFFERSIZE);
  strlwr(Buffer);

	/* Parse the entire string */
  pParse = strtok(Buffer, OBRI_ITEMSEPSTRING);

  while (pParse != NULL) {
    pParse1 = trim(pParse);
    
		/* Find the effect mask */
    Result = StringToObriEffectType(MaskValue, pParse1);

    if (!Result)
      AddObGeneralError("%5ld: Unknown item effect mask '%s'!", m_InputLine, pParse1);
    else
      Mask |= (dword) MaskValue;

    pParse = strtok(NULL, OBRI_ITEMSEPSTRING);
   }

  return (Mask);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::ParseEffectMask()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - long ParseEnchantMask (pValue);
 *
 * Parses and returns the enchantment mask value from the given string.
 *
 *=========================================================================*/
dword CObriBaseItems::ParseEnchantMask (const SSCHAR* pValue) {
  SSCHAR  Buffer[OBRI_MAX_BUFFERSIZE+1];
  SSCHAR* pParse;
  SSCHAR* pParse1;
  dword   Mask = 0;
  int     MaskValue;
  bool    Result;

	/* Create a copy of the input value for parsing */
  strnncpy(Buffer, pValue, OBRI_MAX_BUFFERSIZE);
  strlwr(Buffer);

	/* Parse the entire string */
  pParse = strtok(Buffer, OBRI_ITEMSEPSTRING);

  while (pParse != NULL) {
    pParse1 = trim(pParse);

		/* Find the effect mask */
    Result = StringToObriEnchantType(MaskValue, pParse1);

    if (!Result)
      AddObGeneralError("%5ld: Unknown item enchantment mask '%s'!", m_InputLine, pParse1);
    else
      Mask |= MaskValue;

    pParse = strtok(NULL, OBRI_ITEMSEPSTRING);
   }

  return (Mask);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::ParseEnchantMask()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool ReadItem (File, pNewItem);
 *
 * Inputs an item section from the given file and stores the results in the
 * given item. Returns false on any error. Protected class method.
 *
 *=========================================================================*/
bool CObriBaseItems::ReadItem (CObFile& File, obri_item_t* pNewItem) {
  CSString  LineBuffer;
  CSString  Variable;
  CSString  Value;
  bool      ParseResult;
  bool	    Result;

	/* Input until end of file or end of item */
  while (!File.IsEOF()) {

		/* Input a single line of text from the file */
    Result = File.ReadLine(LineBuffer);
    if (!Result) return (false);
    ++m_InputLine;

		/* Parse input, ignore comments, whitespace trim, etc... */
    ParseResult = LineBuffer.SeperateVarValue(Variable, Value);

		/* Determine input action */
    if (Variable.CompareNoCase("End") == 0) {
      return (true);
     }
    else if (ParseResult) {
      Result = SetItemParam(pNewItem, Variable, Value);
      if (!Result) return (false);
     }
    
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::ReadItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool ReadItem (File);
 *
 * Protected class method which reads an item section from the given file.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::ReadItem (CObFile& File) {
  obri_item_t* pNewItem;
  obri_item_t* pDupItem;
  bool         Result;
  int          Index;

	/* Create and initialize the new item */
  pNewItem = new obri_item_t;
  DefaultObriBaseItem(*pNewItem);
  m_BaseItems.Add(pNewItem);

  Result = ReadItem(File, pNewItem);
  if (!Result) return (false);

	/* Create additional copies if required */
  for (Index = 1; Index < pNewItem->Frequency; ++Index) {
    pDupItem = new obri_item_t;
    DefaultObriBaseItem(*pDupItem);
    m_BaseItems.Add(pDupItem);

    *pDupItem = *pNewItem;
    pDupItem->Weight += (float) (Index + 1) * 0.01f;
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::ReadItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool ReadItemFile (pFilename);
 *
 * Attempts to input item data from the given file. Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::ReadItemFile (const SSCHAR* pFilename) {
  CSString  LineBuffer;
  CSString  Variable;
  CSString  Value;
  CObFile   File;
  long      Filesize;
  bool	    Result;

	/* Attempt to open the file for reading */
  Result = File.Open(pFilename, "rt ");
  if (!Result) return (false);
  Filesize = File.GetFileSize();

	/* Input until end of file or end of effect */
  while (!File.IsEOF() && File.Tell() < Filesize) {

		/* Input a single line of text from the file */
    Result = File.ReadLine(LineBuffer);
    if (!Result) return (false);
    ++m_InputLine;

		/* Parse input, ignore comments, whitespace trim, etc... */
    LineBuffer.SeperateVarValue(Variable, Value);

		/* Start of item section */
    if (Variable.CompareNoCase("Item") == 0) {
      Result = ReadItem(File);
      if (!Result) return (false);
     }
   }

  SystemLog.Printf("Loaded %d base items", m_BaseItems.GetSize());
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::ReadItemFile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool SetItemParam (pNewItem, pVariable, pValue);
 *
 * Sets a parameter of the given item record using the input strings.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::SetItemParam (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue) {
  int Value;

  if (stricmp(pVariable, "Name") == 0) {
    strnncpy(pNewItem->Name, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "CustomType") == 0) {
    strnncpy(pNewItem->CustomType, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Material") == 0) {
    strnncpy(pNewItem->Material, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Icon") == 0) {
    strnncpy(pNewItem->Icon, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Icon2") == 0) {
    strnncpy(pNewItem->FIcon, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Model") == 0) {
    strnncpy(pNewItem->Model, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Model2") == 0) {
    strnncpy(pNewItem->MWModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Model3") == 0) {
    strnncpy(pNewItem->FBModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Model4") == 0) {
    strnncpy(pNewItem->FWModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "ItemType") == 0) {
    pNewItem->ItemType = StringToObriItemType(pValue);

    switch (pNewItem->ItemType) {
	case OBRI_ITEMTYPE_ARMOR:    DefaultObriArmor(*pNewItem); break;
	case OBRI_ITEMTYPE_CLOTHING: DefaultObriClothing(*pNewItem); break;
	case OBRI_ITEMTYPE_LIGHT:    DefaultObriLight(*pNewItem); break;
	case OBRI_ITEMTYPE_MISC:     DefaultObriMisc(*pNewItem); break;
	case OBRI_ITEMTYPE_SCROLL:   DefaultObriScroll(*pNewItem); break;
	case OBRI_ITEMTYPE_STAFF:    DefaultObriStaff(*pNewItem); break;
	case OBRI_ITEMTYPE_SIGIL:    DefaultObriSigil(*pNewItem); break;
	case OBRI_ITEMTYPE_WEAPON:   DefaultObriWeapon(*pNewItem); break;
	default: AddObGeneralError("%5ld: Unknown item type '%s'!", m_InputLine, pValue); break;
     }
   }
  else if (stricmp(pVariable, "BasePrice") == 0 || stricmp(pVariable, "Value") == 0) {
    pNewItem->BasePrice = atoi(pValue);
   }
  else if (stricmp(pVariable, "Weight") == 0) {
    pNewItem->Weight = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "ItemLevel") == 0) {
    pNewItem->ItemLevel = atoi(pValue);
   }
  else if (stricmp(pVariable, "Frequency") == 0) {
    pNewItem->Frequency = atoi(pValue);
    if (pNewItem->Frequency <= 0) pNewItem->Frequency = 1;
   }
  else if (stricmp(pVariable, "EffectMask") == 0) {
    pNewItem->EffectMask = ParseEffectMask(pValue);
   }
  else if (stricmp(pVariable, "EnchantMask") == 0) {
    pNewItem->EnchantMask = ParseEnchantMask(pValue);
   }
  else if (stricmp(pVariable, "MinEffectLevel") == 0) {
    Value = atoi(pValue);

    if (Value < 0) 
      AddObGeneralError("%5ld: Invalid MinEffectLevel value '%s'!", m_InputLine, pValue);
    else
      pNewItem->MinEffectLevel = Value;
   }
  else if (stricmp(pVariable, "MaxEffectLevel") == 0) {
    Value = atoi(pValue);

    if (Value < 0) 
      AddObGeneralError("%5ld: Invalid MaxEffectLevel value '%s'!", m_InputLine, pValue);
    else
      pNewItem->MaxEffectLevel = Value;
   }
  else if (pNewItem->ItemType == OBRI_ITEMTYPE_ARMOR) {
    SetArmorParam(pNewItem, pVariable, pValue);
   }
  else if (pNewItem->ItemType == OBRI_ITEMTYPE_CLOTHING) {
    SetClothingParam(pNewItem, pVariable, pValue);
   }
  else if (pNewItem->ItemType == OBRI_ITEMTYPE_WEAPON) {
    SetWeaponParam(pNewItem, pVariable, pValue);
   }
  else if (pNewItem->ItemType == OBRI_ITEMTYPE_STAFF) {
    SetWeaponParam(pNewItem, pVariable, pValue);
   }
  else if (pNewItem->ItemType == OBRI_ITEMTYPE_SCROLL) {
    SetScrollParam(pNewItem, pVariable, pValue);
   }
  else if (pNewItem->ItemType == OBRI_ITEMTYPE_MISC) {
    SetMiscParam(pNewItem, pVariable, pValue);
   }
  else if (pNewItem->ItemType == OBRI_ITEMTYPE_SIGIL) {
    SetSigilParam(pNewItem, pVariable, pValue);
   }
  else if (pNewItem->ItemType == OBRI_ITEMTYPE_LIGHT) {
    SetLightParam(pNewItem, pVariable, pValue);
   }
  else {
    AddObGeneralError("%5ld: Unknown item variable '%s'!", m_InputLine, pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::SetItemParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool SetArmorParam (pNewItem, pVariable, pValue);
 *
 * Sets a armor parameter of the given item record using the input strings.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::SetArmorParam (obri_item_t* pNewItem, const SSCHAR* pVariable,
				    const SSCHAR* pValue) {
  bool Result;

  if (stricmp(pVariable, "Health") == 0 || stricmp(pVariable, "Condition") == 0) {
    pNewItem->Armor.Health = atoi(pValue);
   }
  else if (stricmp(pVariable, "Armor") == 0 || stricmp(pVariable, "AC") == 0) {
    pNewItem->Armor.Armor = atoi(pValue);
   }
  else if (stricmp(pVariable, "ArmorType") == 0) {
    if (stricmp(pValue, "Light") == 0) {
      pNewItem->Armor.HeavyArmor = false;
     }
    else if (stricmp(pValue, "Heavy") == 0) {
      pNewItem->Armor.HeavyArmor = true;
     }
    else {
      AddObGeneralError("%5ld: Unknown armor type value '%s'!", m_InputLine, pValue);
     }
   }
  else if (stricmp(pVariable, "HideAmulet") == 0) {
    Result = StringToBoolean(pNewItem->Armor.HideAmulet, pValue);
    if (!Result) AddObGeneralError("%5ld: Unknown boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "HideRings") == 0) {
    Result = StringToBoolean(pNewItem->Armor.HideRings, pValue);
    if (!Result) AddObGeneralError("%5ld: Unknown boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "BipedParts") == 0) {
    pNewItem->Armor.BipedParts = ParseBipedFlag(pValue);
   }
  else if (stricmp(pVariable, "MIcon") == 0) {
    strnncpy(pNewItem->MIcon, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "FIcon") == 0) {
    strnncpy(pNewItem->FIcon, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "MWModel") == 0) {
    strnncpy(pNewItem->MWModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "FWModel") == 0) {
    strnncpy(pNewItem->FWModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "MBModel") == 0) {
    strnncpy(pNewItem->MBModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "FBModel") == 0) {
    strnncpy(pNewItem->FBModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else {
    AddObGeneralError("%5ld: Unknown armor variable '%s'!", m_InputLine, pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::SetArmorParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool SetClothingParam (pNewItem, pVariable, pValue);
 *
 * Sets a clothing parameter of the given item record using the input strings.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::SetClothingParam (obri_item_t* pNewItem, const SSCHAR* pVariable,
				       const SSCHAR* pValue) {
  bool Result;

  if (stricmp(pVariable, "HideAmulet") == 0) {
    Result = StringToBoolean(pNewItem->Clothing.HideAmulet, pValue);
    if (!Result) AddObGeneralError("%5ld: Unknown boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "HideRings") == 0) {
    Result = StringToBoolean(pNewItem->Clothing.HideRings, pValue);
    if (!Result) AddObGeneralError("%5ld: Unknown boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "BipedParts") == 0) {
    pNewItem->Clothing.BipedParts = ParseBipedFlag(pValue);
   }
  else if (stricmp(pVariable, "MIcon") == 0) {
    strnncpy(pNewItem->MIcon, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "FIcon") == 0) {
    strnncpy(pNewItem->FIcon, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "MWModel") == 0) {
    strnncpy(pNewItem->MWModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "FWModel") == 0) {
    strnncpy(pNewItem->FWModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "MBModel") == 0) {
    strnncpy(pNewItem->MBModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "FBModel") == 0) {
    strnncpy(pNewItem->FBModel, pValue, OBRI_MAX_NAMESIZE);
   }
  else {
    AddObGeneralError("%5ld: Unknown clothing variable '%s'!", m_InputLine, pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::SetClothingParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool SetLightParam (pNewItem, pVariable, pValue);
 *
 * Sets a light parameter of the given item record using the input strings.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::SetLightParam (obri_item_t* pNewItem, const SSCHAR* pVariable,
				     const SSCHAR* pValue) {
  bool  Result;
  dword RevColor;
  int   Value;

  if (stricmp(pVariable, "Color") == 0) {
    RevColor = strtoul(pValue, NULL, 16);

    pNewItem->Light.Color.A = (byte) ((RevColor >> 24) & 0xFF);
    pNewItem->Light.Color.R = (byte) ((RevColor >> 16) & 0xFF);
    pNewItem->Light.Color.G = (byte) ((RevColor >> 8) & 0xFF);
    pNewItem->Light.Color.B = (byte) (RevColor & 0xFF);
   }
  else if (stricmp(pVariable, "Falloff") == 0) {
    pNewItem->Light.Falloff = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "Sound") == 0) {
    strnncpy(pNewItem->Light.Sound, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "FOV") == 0) {
    pNewItem->Light.FOV = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "Fade") == 0) {
    pNewItem->Light.Fade = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "Time") == 0) {
    pNewItem->Light.Time = atoi(pValue);
   }
  else if (stricmp(pVariable, "Radius") == 0) {
    Value = atoi(pValue);

    if (Value < 0)
      AddObGeneralError("%5ld: Invalid light radius value '%s'!", m_InputLine, pVariable);  
    else
      pNewItem->Light.Radius = (dword) Value;
   }
  else if (stricmp(pVariable, "Type") == 0) {
    Result = GetObLightTypeValue(pNewItem->Light.Type, pValue);
    if (!Result) AddObGeneralError("%5ld: Invalid light type '%s'!", m_InputLine, pVariable);  
   }
  else if (stricmp(pVariable, "Dynamic") == 0) {
    Result = StringToBoolean(pNewItem->Light.Dynamic, pValue);
    if (!Result) AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pVariable);
   }
  else if (stricmp(pVariable, "Negative") == 0) {
    Result = StringToBoolean(pNewItem->Light.Negative, pValue);
    if (!Result) AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pVariable);
   }
  else if (stricmp(pVariable, "DefaultOff") == 0) {
    Result = StringToBoolean(pNewItem->Light.DefaultOff, pValue);
    if (!Result) AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pVariable);
   }
  else if (stricmp(pVariable, "SpotLight") == 0) {
    Result = StringToBoolean(pNewItem->Light.SpotLight, pValue);
    if (!Result) AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pVariable);
   }
  else if (stricmp(pVariable, "SpotShadow") == 0) {
    Result = StringToBoolean(pNewItem->Light.SpotShadow, pValue);
    if (!Result) AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pVariable);
   }
  else if (stricmp(pVariable, "Carried") == 0) {
    Result = StringToBoolean(pNewItem->Light.Carried, pValue);
    if (!Result) AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pVariable);
   }
  else {  
    AddObGeneralError("%5ld: Unknown light variable '%s'!", m_InputLine, pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::SetLightParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool SetMiscParam (pNewItem, pVariable, pValue);
 *
 * Sets a misc parameter of the given item record using the input strings.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::SetMiscParam (obri_item_t* pNewItem, const SSCHAR* pVariable,
				     const SSCHAR* pValue) {
   
  AddObGeneralError("%5ld: Unknown misc variable '%s'!", m_InputLine, pVariable);
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::SetMiscParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool SetSigilParam (pNewItem, pVariable, pValue);
 *
 * Sets a sigil parameter of the given item record using the input strings.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::SetSigilParam (obri_item_t* pNewItem, const SSCHAR* pVariable,
				     const SSCHAR* pValue) {
  int Value;

  if (stricmp(pVariable, "Uses") == 0) {
    Value = atoi(pValue);

    if (Value < 0 || Value > 255) 
      AddObGeneralError("%5ld: Invalid sigil stone uses value '%s'!", m_InputLine, pValue);
    else
      pNewItem->Sigil.Uses = (byte) Value;

   }
  else {  
    AddObGeneralError("%5ld: Unknown sigil variable '%s'!", m_InputLine, pVariable);
   }

  return (true);
 }
/*==========================================================================
 *		End of Class Method CObriBaseItems::SetSigilParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool SetScrollParam (pNewItem, pVariable, pValue);
 *
 * Sets a scroll parameter of the given item record using the input strings.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::SetScrollParam (obri_item_t* pNewItem, const SSCHAR* pVariable,
				     const SSCHAR* pValue) {
  bool Result;
  int  SkillValue;

  if (stricmp(pVariable, "Teaches") == 0) {
    Result = GetObSkillValue (SkillValue, pValue);

    if (!Result) {
      pNewItem->Scroll.Teaches = OB_SKILL_NONE;
      AddObGeneralError("%5ld: Unknown teaches skill value '%s'!", m_InputLine, pValue); 
     }
    else {
      pNewItem->Scroll.Teaches = SkillValue;
     }
   }
  else if (stricmp(pVariable, "IsScroll") == 0) {
    Result = StringToBoolean(pNewItem->Scroll.IsScroll, pValue);
    if (!Result) AddObGeneralError("%5ld: Unknown boolean value '%s'!", m_InputLine, pValue); 
   }
  else {
    AddObGeneralError("%5ld: Unknown scroll variable '%s'!", m_InputLine, pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::SetScrollParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriBaseItems Method - bool SetWeaponParam (NewItem, pVariable, pValue);
 *
 * Sets a weapon parameter of the given item record using the input strings.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriBaseItems::SetWeaponParam (obri_item_t* pNewItem, const SSCHAR* pVariable,
				   const SSCHAR* pValue) {
  bool Result;
  int  WeaponType;

  if (stricmp(pVariable, "Reach") == 0) {
    pNewItem->Weapon.Reach = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "Speed") == 0) {
    pNewItem->Weapon.Speed = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "Damage") == 0) {
    pNewItem->Weapon.Damage = (word) (atoi(pValue) & 0xFFFF);
   }
  else if (stricmp(pVariable, "Health") == 0 || stricmp(pVariable, "Condition") == 0) {
    pNewItem->Weapon.Health = atoi(pValue);
   }
  else if (stricmp(pVariable, "EnchantCharge") == 0 || stricmp(pVariable, "EnchantPts") == 0 || stricmp(pVariable, "EnchantPoints") == 0) {
    pNewItem->Weapon.EnchantCharge = atoi(pValue);
   }
  else if (stricmp(pVariable, "Type") == 0) {
    Result = GetObWeaponTypeValue(WeaponType, pValue);

    if (!Result) {
      pNewItem->Weapon.Type = OB_WEAPTYPE_BLADEONEHAND;
      AddObGeneralError("%5ld: Unknown weapon type '%s'!", m_InputLine, pValue);
     }
    else {
      pNewItem->Weapon.Type = WeaponType;
     }
   }
  else if (stricmp(pVariable, "IgnoreResist") == 0) {
    Result = StringToBoolean(pNewItem->Weapon.IgnoreResist, pValue);
    if (!Result) AddObGeneralError("%5ld: Unknown boolean value '%s'!", m_InputLine, pValue);
   }
  else {
    AddObGeneralError("%5ld: Unknown weapon variable '%s'!", m_InputLine, pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriBaseItems::SetWeaponParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriBaseItem (Item);
 *
 *=========================================================================*/
void DefaultObriBaseItem (obri_item_t& Item) {
  memset(&Item, 0, sizeof(obri_item_t));

  Item.BasePrice      = 50;
  Item.ItemLevel      = 1;
  Item.ItemType       = OBRI_ITEMTYPE_WEAPON;
  Item.Weight         = 1.0f;
  Item.EffectMask     = OBRI_EFFTYPE_ALL;
  Item.EnchantMask    = OBRI_ENCTYPE_ALL;
  Item.MinEffectLevel = 0;
  Item.MaxEffectLevel = 1000;
  Item.Frequency      = 1;
 }  
/*===========================================================================
 *		End of Function DefaultObriBaseItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriArmor (Item);
 *
 *=========================================================================*/
void DefaultObriArmor (obri_item_t& Item) {
  Item.Armor.Health     = 100;
  Item.Armor.Armor      = 10;
  Item.Armor.HideAmulet = false;
  Item.Armor.HideRings  = false;
  Item.Armor.HeavyArmor = false;

  Item.EnchantMask = OBRI_ENCTYPE_CONSTANT;
 }
/*===========================================================================
 *		End of Function DefaultObriArmor()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriClothing (Item);
 *
 *=========================================================================*/
void DefaultObriClothing (obri_item_t& Item) {
  Item.Clothing.HideAmulet = false;
  Item.Clothing.HideRings  = false;

  Item.EnchantMask = OBRI_ENCTYPE_CONSTANT;
 }
/*===========================================================================
 *		End of Function DefaultObriClothing()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriLight (Item);
 *
 *=========================================================================*/
void DefaultObriLight (obri_item_t& Item) {
  Item.Light.Color      = OB_COLOR_NULL;
  Item.Light.DefaultOff = false;
  Item.Light.Dynamic    = false;
  Item.Light.Negative   = false;
  Item.Light.SpotLight  = false;
  Item.Light.SpotShadow = false;
  Item.Light.Carried    = true;
  Item.Light.Falloff    = 0.0f;
  Item.Light.FOV        = 90.0f;
  Item.Light.Radius     = 100;
  Item.Light.Time       = -1;
  Item.Light.Fade       = 1;
  Item.Light.Type       = OB_LIGHTYPE_NONE;
  Item.Light.Sound[0]   = NULL_CHAR;
  
  Item.EnchantMask = 0;
  Item.EffectMask  = OBRI_EFFTYPE_NONE | OBRI_EFFTYPE_WEIGHT | OBRI_EFFTYPE_VALUE | OBRI_EFFTYPE_SCRIPT;
 }
/*===========================================================================
 *		End of Function DefaultObriLight()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriMisc (Item);
 *
 *=========================================================================*/
void DefaultObriMisc (obri_item_t& Item) {
  Item.EnchantMask = 0;
 }
/*===========================================================================
 *		End of Function DefaultObriMisc()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriScroll (Item);
 *
 *=========================================================================*/
void DefaultObriScroll (obri_item_t& Item) {
  Item.Scroll.IsScroll = true;
  Item.Scroll.Teaches  = OB_SKILL_NONE;

  Item.EnchantMask = OBRI_ENCTYPE_ONCE;
 }
/*===========================================================================
 *		End of Function DefaultObriScroll()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriSigil (Item);
 *
 *=========================================================================*/
void DefaultObriSigil (obri_item_t& Item) {
  Item.Sigil.Uses = 1;

  Item.EnchantMask = OBRI_ENCTYPE_STRIKES | OBRI_ENCTYPE_CONSTANT;
  Item.EffectMask  = OBRI_EFFTYPE_SCRIPT | OBRI_EFFTYPE_ENCHANT;
 }
/*===========================================================================
 *		End of Function DefaultObriSigil()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriStaff (Item);
 *
 *=========================================================================*/
void DefaultObriStaff (obri_item_t& Item) {
  Item.Weapon.Health       = 100;
  Item.Weapon.Damage       = 1;
  Item.Weapon.Speed        = 1.0f;
  Item.Weapon.Reach        = 1.0f;
  Item.Weapon.Type         = OB_WEAPTYPE_STAFF;
  Item.Weapon.IgnoreResist = false;

  Item.EnchantMask = OBRI_ENCTYPE_TARGET;
 }
/*===========================================================================
 *		End of Function DefaultObriStaff()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriWeapon (Item);
 *
 *=========================================================================*/
void DefaultObriWeapon (obri_item_t& Item) {
  Item.Weapon.Health       = 100;
  Item.Weapon.Damage       = 1;
  Item.Weapon.Speed        = 1.0f;
  Item.Weapon.Reach        = 1.0f;
  Item.Weapon.Type         = OB_WEAPTYPE_BLADEONEHAND;
  Item.Weapon.IgnoreResist = false;

  Item.EnchantMask = OBRI_ENCTYPE_STRIKES;
 }
/*===========================================================================
 *		End of Function DefaultObriWeapon()
 *=========================================================================*/

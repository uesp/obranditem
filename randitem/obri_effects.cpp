/*===========================================================================
 *
 * File:	Obri_Effects.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	21 April 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_effects.h"


/*===========================================================================
 *
 * Begin Local Definitions
 *
 *=========================================================================*/
/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Value String Arrays
 *
 *=========================================================================*/

/*===========================================================================
 *		End of Value String Arrays
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Constructor
 *
 *=========================================================================*/
CObriEffects::CObriEffects () {
  m_InputLine = 0;
 }
/*===========================================================================
 *		End of Class CObriEffects Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - void Destroy (void);
 *
 *=========================================================================*/
void CObriEffects::Destroy (void) {

	/* Clear the arrays and unallocate contents */
  m_Prefixes.Destroy();
  m_Suffixes.Destroy();

  m_InputLine = 0;
 }
/*===========================================================================
 *		End of Class Method CObriEffects::Destroy()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - obri_effect_t* FindEffect (pName);
 *
 * Searches the suffix and prefix arrays for an effect with the matching 
 * name ID (not case sensitive). Returns NULL on any error.
 *
 *=========================================================================*/
obri_effect_t* CObriEffects::FindEffect (const SSCHAR* pName) {
  obri_effect_t* pEffect;
  dword		 Index;

	/* Ignore invalid input */
  if (pName == NULL) return (NULL);

	/* Search the prefix array */
  for (Index = 0; Index < m_Prefixes.GetSize(); ++Index) {
    pEffect = m_Prefixes.GetAt(Index);
    if (stricmp(pEffect->NameID, pName) == 0) return (pEffect);
   }

	/* Search the suffix array */
  for (Index = 0; Index < m_Suffixes.GetSize(); ++Index) {
    pEffect = m_Suffixes.GetAt(Index);
    if (stricmp(pEffect->NameID, pName) == 0) return (pEffect);
   }

 
  return (NULL);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::FindEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool ReadEffect (File, pNewEffect);
 *
 * Inputs a new record from the current position in the given file, storing
 * the data in the given object. Protected class method. Returns false on
 * any error.
 *
 *=========================================================================*/
bool CObriEffects::ReadEffect (CObFile& File, obri_effect_t* pNewEffect) {
  CSString LineBuffer;
  CSString Variable;
  CSString Value;
  bool     IsReading = true;
  bool     ParseResult;
  bool	   Result;
  
	/* Input until end of file or end of effect */
  while (!File.IsEOF() && IsReading) {

		/* Input a single line of text from the file */
    Result = File.ReadLine(LineBuffer);
    if (!Result) return (false);
    ++m_InputLine;

		/* Parse input, ignore comments, whitespace trim, etc... */
    ParseResult = LineBuffer.SeperateVarValue(Variable, Value);

		/* Determine input action */
    if (Variable.CompareNoCase("End") == 0) {
      IsReading = false;
     }
		/* Input an enchantment section */
    else if (Variable.CompareNoCase("Enchant") == 0) {
      Result = ReadEnchant(File, pNewEffect);
      if (!Result) return (false);
     }
    else if (ParseResult) {
      Result = SetEffectParam(pNewEffect, Variable, Value);
      if (!Result) return (false);
     }
    
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::ReadEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool ReadEffectFile (pFilename);
 *
 * Attempts to input effect data from the given file. Returns false on any error.
 *
 *=========================================================================*/
bool CObriEffects::ReadEffectFile (const SSCHAR* pFilename) {
  CSString LineBuffer;
  CSString Variable;
  CSString Value;
  CObFile  File;
  long     Filesize;
  bool     IsReading = true;
  bool	   Result;

	/* Attempt to open the file for reading */
  Result = File.Open(pFilename, "rt");
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

		/* Start of effect sections */
    if (Variable.CompareNoCase("Suffix") == 0) {
      Result = ReadSuffix(File);
      if (!Result) return (false);
     }
    else if (Variable.CompareNoCase("Prefix") == 0) {
      Result = ReadPrefix(File);
      if (!Result) return (false);
     }
   }

  SystemLog.Printf("Loaded %d prefixes", m_Prefixes.GetSize());
  SystemLog.Printf("Loaded %d suffixes", m_Suffixes.GetSize());
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::ReadEffectFile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool ReadEnchant (File, pNewEffect);
 *
 * Protected class method to input an enchantment block starting at the
 * current position in the given file. Returns false on any error.
 *
 *=========================================================================*/
bool CObriEffects::ReadEnchant (CObFile& File, obri_effect_t* pNewEffect) {
  CSString LineBuffer;
  CSString Variable;
  CSString Value;
  bool     IsReading = true;
  bool     ParseResult;
  bool	   Result;

	/* Input until end of file or end of effect */
  while (!File.IsEOF() && IsReading) {

		/* Input a single line of text from the file */
    Result = File.ReadLine(LineBuffer);
    if (!Result) return (false);
    ++m_InputLine;
		/* Parse input, ignore comments, whitespace trim, etc... */
    ParseResult = LineBuffer.SeperateVarValue(Variable, Value);

		/* Determine input action */
    if (Variable.CompareNoCase("End") == 0) {
      IsReading = false;
     }
		/* Input an effect section */
    else if (Variable.CompareNoCase("Effect") == 0) {

		/* Ensure we don't exceed the effect array size */
      if (pNewEffect->Enchantment.NumEffects < OBRI_MAX_ENCHEFFECTS) {
        DefaultObriEnchEffect(pNewEffect->Enchantment.Effects[pNewEffect->Enchantment.NumEffects]);
	++pNewEffect->Enchantment.NumEffects;
        Result = ReadEnchantEffect(File, pNewEffect);
	if (!Result) return (false);
       }
      else {
        AddObUserError(OBERR_USER_MAXINDEX, "Exceeded the effect array size of %d!", OBRI_MAX_ENCHEFFECTS);
        return (false);
       }
     }
    else if (ParseResult) {
      Result = SetEnchantParam(pNewEffect, Variable, Value);
      if (!Result) return (false);
     }
    
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::ReadEnchant()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool ReadEnchantEffect (File, pNewEffect);
 *
 * Protected class method to input an effect enchantment block starting at the
 * current position in the given file. Returns false on any error.
 *
 *=========================================================================*/
bool CObriEffects::ReadEnchantEffect (CObFile& File, obri_effect_t* pNewEffect) {
  CSString LineBuffer;
  CSString Variable;
  CSString Value;
  bool     IsReading = true;
  bool     ParseResult;
  bool	   Result;

	/* Input until end of file or end of effect */
  while (!File.IsEOF() && IsReading) {

		/* Input a single line of text from the file */
    Result = File.ReadLine(LineBuffer);
    if (!Result) return (false);
    ++m_InputLine;

		/* Parse input, ignore comments, whitespace trim, etc... */
    ParseResult = LineBuffer.SeperateVarValue(Variable, Value);

		/* Determine input action */
    if (Variable.CompareNoCase("End") == 0) {
      IsReading = false;
     }
    else if (ParseResult) {
      Result = SetEnchantEffectParam(pNewEffect, Variable, Value);
      if (!Result) return (false);
     }
    
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::ReadEnchantEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool ReadPrefix (File);
 *
 * Input effect data from the current position in the given file. Protected
 * class method. Assumes that the 'Prefix' line has already been input.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriEffects::ReadPrefix (CObFile& File) {
  obri_effect_t* pNewPrefix;

	/* Create and initialize the new effect object */
  pNewPrefix = new obri_effect_t;
  m_Prefixes.Add(pNewPrefix);
  DefaultObriEffect(*pNewPrefix);

  return ReadEffect(File, pNewPrefix);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::ReadPrefix()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool ReadSuffix (File);
 *
 * Input effect data from the current position in the given file. Protected
 * class method. Assumes that the 'Suffix' line has already been input.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriEffects::ReadSuffix (CObFile& File) {
  obri_effect_t* pNewSuffix;

	/* Create and initialize the new effect object */
  pNewSuffix = new obri_effect_t;
  m_Suffixes.Add(pNewSuffix);
  DefaultObriEffect(*pNewSuffix);

  return ReadEffect(File, pNewSuffix);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::ReadSuffix()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool ParseNameMask (pNewEffect, pString);
 *
 *=========================================================================*/
bool CObriEffects::ParseNameMask (obri_effect_t* pNewEffect, const SSCHAR* pString) {
  CSString* pNewMask;
  SSCHAR    Buffer[512];
  SSCHAR*   pParse;

	/* Create a temporary working copy */
  strnncpy(Buffer, pString, 511);
  pParse = strtok(Buffer, "|");

  while (pParse != NULL) {
    pParse = trim(pParse);

    if (*pParse != NULL_CHAR) {
      pNewMask = pNewEffect->NameMasks.AddNew();
      *pNewMask = pParse;
     }
 
    pParse = strtok(NULL, "|");
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::ParseNameMask()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool ParseCustomTypes (pNewEffect, pString);
 *
 *=========================================================================*/
bool CObriEffects::ParseCustomTypes (obri_effect_t* pNewEffect, const SSCHAR* pString) {
  CSString* pNewMask;
  SSCHAR    Buffer[512];
  SSCHAR*   pParse;

	/* Create a temporary working copy */
  strnncpy(Buffer, pString, 511);
  pParse = strtok(Buffer, "|");

  while (pParse != NULL) {
    pParse = trim(pParse);

    if (*pParse != NULL_CHAR) {
      pNewMask = pNewEffect->CustomTypes.AddNew();
      *pNewMask = pParse;
     }
 
    pParse = strtok(NULL, "|");
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriEffects::ParseCustomTypes()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool SetEffectParam (pNewEffect, pVariable, pValue);
 *
 * Sets the input effect parameter according to the value of the variable
 * and value strings. Returns false on any error. Protected class method.
 *
 *=========================================================================*/
bool CObriEffects::SetEffectParam (obri_effect_t* pNewEffect, const SSCHAR* pVariable, 
				   const SSCHAR* pValue) {
  bool Result;

  if (stricmp(pVariable, "Name") == 0) {
    strnncpy(pNewEffect->Name, pValue, OBRI_MAX_NAMESIZE);

    if (pNewEffect->NameID[0] == NULL_CHAR) {
      strnncpy(pNewEffect->NameID, pValue, OBRI_MAX_NAMESIZE);  
     }
   }
  else if (stricmp(pVariable, "NameID") == 0) {
    strnncpy(pNewEffect->NameID, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "NameMask") == 0) {
    ParseNameMask(pNewEffect, pValue);
   }
  else if (stricmp(pVariable, "CustomTypes") == 0) {
    ParseCustomTypes(pNewEffect, pValue);
   }
  else if (stricmp(pVariable, "Script") == 0) {
    strnncpy(pNewEffect->Script, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Cursed") == 0) {
    Result = StringToBoolean(pNewEffect->Cursed, pValue);
    if (!Result) AddObGeneralError("%5ld: Unknown boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "EffectType") == 0) {
    Result = StringToObriEffectType(pNewEffect->EffectType, pValue);
    if (!Result) AddObGeneralError("%5ld: Unknown effect type '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "ItemLevel") == 0) {
    pNewEffect->ItemLevel = atoi(pValue);
   }
  else if (stricmp(pVariable, "ItemMask") == 0) {
    pNewEffect->ItemMask = StringToObriItemMask(pValue);
   }
  else if (stricmp(pVariable, "MinEffect") == 0) {
    pNewEffect->MinEffect = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "MaxEffect") == 0) {
    pNewEffect->MaxEffect = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "MinQuality") == 0) {
    pNewEffect->MinQuality = atoi(pValue);
   }
  else if (stricmp(pVariable, "MaxQuality") == 0) {
    pNewEffect->MaxQuality = atoi(pValue);
   }
  else if (stricmp(pVariable, "Multiplier") == 0) {
    pNewEffect->Multiplier = atoi(pValue);
   }
  else {
    AddObGeneralError ("%5ld: Unknown effect variable '%s'!", m_InputLine, pVariable);
   }

  return (true);				       
 }
/*===========================================================================
 *		End of Class Method CObriEffects::SetEffectParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool SetEnchantParam (pNewEffect, pVariable, pValue);
 *
 * Sets the input enchantment parameter according to the value of the variable
 * and value strings. Returns false on any error. Protected class method.
 *
 *=========================================================================*/
bool CObriEffects::SetEnchantParam (obri_effect_t* pNewEffect, const SSCHAR* pVariable, 
				      const SSCHAR* pValue) {
  bool Result;

  if (stricmp(pVariable, "MinCharge") == 0) {
    pNewEffect->Enchantment.MinCharge = atoi(pValue);
   }
  else if (stricmp(pVariable, "MaxCharge") == 0) {
    pNewEffect->Enchantment.MaxCharge = atoi(pValue);
   }
  else if (stricmp(pVariable, "MinCost") == 0) {
    pNewEffect->Enchantment.MinCost = atoi(pValue);
   }
  else if (stricmp(pVariable, "MaxCost") == 0) {
    pNewEffect->Enchantment.MaxCost = atoi(pValue);
   }
  else if (stricmp(pVariable, "EnchantType") == 0) {
    //Result = StringToObriEnchantType(pNewEffect->Enchantment.EnchantType, pValue);
    Result = GetObEnchantTypeValue(pNewEffect->Enchantment.EnchantType, pValue);
    if (!Result) AddObGeneralError ("%5ld: Unknown enchant type '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "AutoCalc") == 0) {
    Result = StringToBoolean(pNewEffect->Enchantment.AutoCalc, pValue);
    if (!Result) AddObGeneralError ("%5ld: Unknown boolean value '%s'!", m_InputLine, pValue);
   }
  else {
    AddObGeneralError ("%5ld: Unknown enchantment variable '%s'!", m_InputLine, pVariable);
   }

  return (true);				       
 }
/*===========================================================================
 *		End of Class Method CObriEffects::SetEnchantParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEffects Method - bool SetEnchantEffectParam (pNewEffect, pVariable, pValue);
 *
 * Sets the input enchantment effect parameter according to the value 
 * of the variable and value strings. Returns false on any error. 
 * Protected class method.
 *
 *=========================================================================*/
bool CObriEffects::SetEnchantEffectParam (obri_effect_t* pNewEffect,
					    const SSCHAR* pVariable, 
				            const SSCHAR* pValue) {
  const obeffectdata_t* pEffect;
  bool		        Result;
  int		        EffectIndex;
  int		        OutputValue;
  
	/* Ensure a valid effect index */
  EffectIndex = pNewEffect->Enchantment.NumEffects - 1;
  if (EffectIndex < 0 || EffectIndex >= OBRI_MAX_ENCHEFFECTS) return (false);

  if (stricmp(pVariable, "EffectID") == 0) {
    pEffect = FindObEffectData(pValue);

    if (pEffect == NULL)
      AddObGeneralError ("%5ld: Unknown effect ID '%s'!", m_InputLine, pValue);
    else
      pNewEffect->Enchantment.Effects[EffectIndex].pEffectID = pEffect->pName;
   }
  else if (stricmp(pVariable, "ActorValue") == 0) {
    Result = GetObActorValueValue(OutputValue, pValue);

    if (!Result) 
      AddObGeneralError ("%5ld: Unknown skill ID '%s'!", m_InputLine, pValue);
    else
      pNewEffect->Enchantment.Effects[EffectIndex].ActorValue = OutputValue;
   }
  else if (stricmp(pVariable, "RangeType") == 0) {
    Result = GetObEnchantRangeValue(OutputValue, pValue);

    if (!Result) 
      AddObGeneralError ("%5ld: Unknown enchant range type '%s'!", m_InputLine, pValue);
    else
      pNewEffect->Enchantment.Effects[EffectIndex].RangeType = OutputValue;
   }
  else if (stricmp(pVariable, "MinArea") == 0) {
    pNewEffect->Enchantment.Effects[EffectIndex].MinArea = atoi(pValue);
   }
  else if (stricmp(pVariable, "MaxArea") == 0) {
    pNewEffect->Enchantment.Effects[EffectIndex].MaxArea = atoi(pValue);
   }
  else if (stricmp(pVariable, "MinDuration") == 0) {
    pNewEffect->Enchantment.Effects[EffectIndex].MinDuration = atoi(pValue);
   }
  else if (stricmp(pVariable, "MaxDuration") == 0) {
    pNewEffect->Enchantment.Effects[EffectIndex].MaxDuration = atoi(pValue);
   }
  else if (stricmp(pVariable, "MinMagnitude") == 0) {
    pNewEffect->Enchantment.Effects[EffectIndex].MinMagnitude = atoi(pValue);
   }
  else if (stricmp(pVariable, "MaxMagnitude") == 0) {
    pNewEffect->Enchantment.Effects[EffectIndex].MaxMagnitude = atoi(pValue);
   }
  else if (stricmp(pVariable, "VisualEffectID") == 0) {
    pEffect = FindObEffectData(pValue);

    if (pEffect == NULL)
      AddObGeneralError ("%5ld: Unknown visual effect ID '%s'!", m_InputLine, pValue);
    else
      pNewEffect->Enchantment.Effects[EffectIndex].pVisualEffectID = pEffect->pName;
   }
  else if (stricmp(pVariable, "Script") == 0) {
    strnncpy(pNewEffect->Enchantment.Effects[EffectIndex].Script, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "EffectName") == 0) {
    strnncpy(pNewEffect->Enchantment.Effects[EffectIndex].EffectName, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "School") == 0) {
    Result = GetObSpellSchoolValue(pNewEffect->Enchantment.Effects[EffectIndex].School, pValue);
    if (!Result) AddObGeneralError ("%5ld: Invalid spell school '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "Hostile") == 0) {
    Result = StringToBoolean(pNewEffect->Enchantment.Effects[EffectIndex].IsHostile, pValue);
    if (!Result) AddObGeneralError ("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else {
    AddObGeneralError ("%5ld: Unknown enchantment effect variable '%s'!", m_InputLine, pVariable);
   }

  return (true);				       
 }
/*===========================================================================
 *		End of Class Method CObriEffects::SetEnchantEffectParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriEffect (Effect);
 *
 *=========================================================================*/
void DefaultObriEffect (obri_effect_t& Effect) {
  Effect.Cursed       = false;
  Effect.EffectType   = OBRI_EFFTYPE_NONE;
  Effect.ItemLevel    = 1;
  Effect.ItemMask     = OBRI_ITEMTYPE_ALL;
  Effect.MinEffect    = 1;
  Effect.MaxEffect    = 10;
  Effect.MinQuality   = 10;
  Effect.MaxQuality   = 100;
  Effect.Multiplier   = 1;
  Effect.Name[0]      = NULL_CHAR;
  Effect.NameID[0]    = NULL_CHAR;
  Effect.Script[0]    = NULL_CHAR;

  Effect.NameMasks.Empty();
  Effect.CustomTypes.Empty();

  DefaultObriEnchant(Effect.Enchantment);
 }
/*===========================================================================
 *		End of Function DefaultObriEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriEnchant (Enchant);
 *
 *=========================================================================*/
void DefaultObriEnchant (obri_enchant_t& Enchant) {
  Enchant.NumEffects     = 0;
  Enchant.MinCharge      = 100;
  Enchant.MaxCharge      = 100;
  Enchant.MinCost        = 10;
  Enchant.MaxCost        = 10;
  Enchant.EnchantType    = OB_ENCHTYPE_SCROLL;
  Enchant.AutoCalc       = false;
 }
/*===========================================================================
 *		End of Function DefaultObriEnchant()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriEnchEffect (EnchEffect);
 *
 *=========================================================================*/
void DefaultObriEnchEffect (obri_encheff_t& EnchEffect) {
  EnchEffect.MinArea         = 0;
  EnchEffect.MaxArea         = 0;
  EnchEffect.MinDuration     = 10;
  EnchEffect.MaxDuration     = 10;
  EnchEffect.RangeType       = 0;
  EnchEffect.MinMagnitude    = 1;
  EnchEffect.MaxMagnitude    = 5;
  EnchEffect.ActorValue      = OB_ACTORVALUE_NONE;
  EnchEffect.pEffectID       = &OB_NAME_NULL;
  EnchEffect.pVisualEffectID = &OB_NAME_NULL;
  EnchEffect.Script[0]       = NULL_CHAR;
  EnchEffect.School          = 0;
  EnchEffect.IsHostile       = true;
  EnchEffect.EffectName[0]   = NULL_CHAR;
 }
/*===========================================================================
 *		End of Function DefaultObriEnchEffect()
 *=========================================================================*/

  


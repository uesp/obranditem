/*===========================================================================
 *
 * File:	Obri_Uniques.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	21 April 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_uniques.h"


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
 * Class CObriUniques Constructor
 *
 *=========================================================================*/
CObriUniques::CObriUniques () {
  m_pEffects  = NULL;
  m_InputLine = 0;
 }
/*===========================================================================
 *		End of Class CObriUniques Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriUniques Method - void Destroy (void);
 *
 *=========================================================================*/
void CObriUniques::Destroy (void) {

	/* Clear the unique array */
  m_Uniques .Destroy();
  m_InputLine = 0;
 }
/*===========================================================================
 *		End of Class Method CObriUniques::Destroy()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriUniques Method - bool AddEffect (pNewUnique, pEffectName);
 *
 * Attempt to add the given effect to the given unique object. Returns
 * false on any error.
 *
 *=========================================================================*/
bool CObriUniques::AddEffect (obri_unique_t* pNewUnique, const SSCHAR* pEffectName) {

	/* Don't exceed the effect array size */
  if (pNewUnique->NumEffects >= OBRI_MAX_UNIQUEEFFECTS) {
    AddObUserError(OBERR_USER_MAXINDEX, "%5ld: Exceeded the maximum number of unique effects %d!", m_InputLine, OBRI_MAX_UNIQUEEFFECTS);
    return (false);
   }

  if (m_pEffects != NULL) {
    pNewUnique->pEffects[pNewUnique->NumEffects] = m_pEffects->FindEffect(pEffectName);

    if (pNewUnique->pEffects[pNewUnique->NumEffects] != NULL) 
      pNewUnique->NumEffects++;
    else
      AddObGeneralError("%5ld: Unknown effect '%s'!", m_InputLine, pEffectName);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriUniques::AddEffect()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriUniques Method - bool ReadUnique (File, pNewUnique);
 *
 * Protected class method which reads an unique section from the given file.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriUniques::ReadUnique (CObFile& File, obri_unique_t* pNewUnique) {
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
      Result = SetUniqueParam(pNewUnique, Variable, Value);
      if (!Result) return (false);
     }
    
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriUniques::ReadUnique()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriUniques Method - bool ReadUnique (File);
 *
 * Protected class method which reads an unique section from the given file.
 * Returns false on any error.
 *
 *=========================================================================*/
bool CObriUniques::ReadUnique (CObFile& File) {
  obri_unique_t* pNewUnique;

	/* Create and initialize the new unique */
  pNewUnique = new obri_unique_t;
  m_Uniques.Add(pNewUnique);

  DefaultObriUnique (*pNewUnique);
  
  return ReadUnique(File, pNewUnique);
 }
/*===========================================================================
 *		End of Class Method CObriUniques::ReadUnique()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriUniques Method - bool ReadUniqueFile (pFilename);
 *
 * Attempts to input unique data from the given file. Returns false on any error.
 *
 *=========================================================================*/
bool CObriUniques::ReadUniqueFile (const SSCHAR* pFilename) {
  CSString LineBuffer;
  CSString Variable;
  CSString Value;
  CObFile  File;
  bool	   Result;
  long     Filesize;
  
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

		/* Start of unique section */
    if (Variable.CompareNoCase("Unique") == 0) {
      Result = ReadUnique(File);
      if (!Result) return (false);
     }
   }

  SystemLog.Printf("Loaded %d uniques", m_Uniques.GetSize());
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriUniques::ReadUniqueFile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriUniques Method - bool SetUniqueParam (pNewUnique, pVariable, pValue);
 *
 * Sets the given unique object values from the given string inputs. Returns
 * false on any error. Protected class method.
 *
 *=========================================================================*/
bool CObriUniques::SetUniqueParam (obri_unique_t* pNewUnique, const SSCHAR* pVariable,
				   const SSCHAR* pValue) {

  if (stricmp(pVariable, "Name") == 0) {
    strnncpy(pNewUnique->Name, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "NameType") == 0) {
    if (stricmp(pValue, "Exact") == 0) 
      pNewUnique->NameType = OBRI_UNIQUE_NAMETYPE_EXACT;
    else if (stricmp(pValue, "Suffix") == 0) 
      pNewUnique->NameType = OBRI_UNIQUE_NAMETYPE_SUFFIX;
    else if (stricmp(pValue, "Prefix") == 0) 
      pNewUnique->NameType = OBRI_UNIQUE_NAMETYPE_PREFIX;
    else
      AddObGeneralError("%5ld: Unknown name type '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "Icon") == 0) {
    strnncpy(pNewUnique->Icon, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "Model") == 0) {
    strnncpy(pNewUnique->Model, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "ItemLevel") == 0) {
    pNewUnique->ItemLevel = atoi(pValue);
   }
  else if (stricmp(pVariable, "ItemMask") == 0) {
    pNewUnique->ItemMask = StringToObriItemMask(pValue);
   }
  else if (stricmp(pVariable, "Effect") == 0) {
    return AddEffect(pNewUnique, pValue);
   }
  else {
    AddObGeneralError("%5ld: Unknown unique parameter '%s'!", m_InputLine, pVariable);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriUniques::SetUniqueParam()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void DefaultObriUnique (UniqueItem);
 *
 *=========================================================================*/
void DefaultObriUnique (obri_unique_t& UniqueItem) {
  UniqueItem.ItemMask   = OBRI_ITEMTYPE_ALL;
  UniqueItem.ItemLevel  = 1;
  UniqueItem.NumEffects = 0;
  UniqueItem.Name[0]    = NULL_CHAR;
  UniqueItem.Model[0]   = NULL_CHAR;
  UniqueItem.Icon[0]    = NULL_CHAR;
  UniqueItem.NameType   = OBRI_UNIQUE_NAMETYPE_EXACT;
 }  
/*===========================================================================
 *		End of Function DefaultObriUnique()
 *=========================================================================*/

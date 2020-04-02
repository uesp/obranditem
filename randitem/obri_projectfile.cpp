/*===========================================================================
 *
 * File:	Obri_ProjectFile.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 21, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_projectfile.h"


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
 * Class CObriProjectFile Constructor
 *
 *=========================================================================*/
CObriProjectFile::CObriProjectFile () {
  DefaultObriOptions(m_Options);

  m_InputLine   = 0;
  m_NumEspFiles = 0;
 }
/*===========================================================================
 *		End of Class CObriProjectFile Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriProjectFile Method - void Destroy (void);
 *
 *=========================================================================*/
void CObriProjectFile::Destroy (void) {
  m_InputLine   = 0;
  m_NumEspFiles = 0;

  m_BaseItems.Empty();
  m_Effects.Empty();
  m_Uniques.Empty();
  m_LevelLists.Empty();

  DefaultObriOptions(m_Options);
 }
/*===========================================================================
 *		End of Class Method CObriProjectFile::Destroy()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriProjectFile Method - bool AddEspFile (pFilename);
 *
 *=========================================================================*/
bool CObriProjectFile::AddEspFile (const SSCHAR* pFilename) {

	/* Ensure we don't exceed the array limits */
  if (m_NumEspFiles >= OBRI_MAX_SCRIPTFILES) {
    AddObUserError(OBERR_USER_MAXINDEX, "Exceeded the maximum of %d ESP files!", m_NumEspFiles);
    return (false);
   }

  m_EspFiles[m_NumEspFiles] = pFilename;
  ++m_NumEspFiles;
  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriProjectFile::AddEspFile()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriProjectFile Method - bool Load (pFilename, ClearOptions);
 *
 * Attempt to load and parse a group file. Returns false on any error.
 *
 *=========================================================================*/
bool CObriProjectFile::Load (const SSCHAR* pFilename, const bool ClearOptions) {
  CSString LineBuffer;
  CSString Variable;
  CSString Value;
  CObFile  File;
  long     Filesize;
  bool	   Result;
   
	/* Attempt to open file for input */
  Result = File.Open(pFilename, "rt");
  if (!Result) return (false);
  Filesize = File.GetFileSize();

	/* Clear the current data */
  if (ClearOptions) {
    Destroy();
    DefaultObriOptions(m_Options);
   }

  	/* Input until end of file or end of effect */
  while (!File.IsEOF() && File.Tell() < Filesize) {

		/* Input a single line of text from the file */
    Result = File.ReadLine(LineBuffer);
    if (!Result) return (false);
    ++m_InputLine;

		/* Parse input, ignore comments, whitespace trim, etc... */
    Result = LineBuffer.SeperateVarValue(Variable, Value);
     
    if (Variable.CompareNoCase("options") == 0) {
      ReadOptionData(File);
     }
    else if (Result) {
      SetParameter(Variable, Value);
     }
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriProjectFile::Load()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriProjectFile Method - bool ReadOptionData (File);
 *
 * Reads an option data section from the given file.
 *
 *=========================================================================*/
bool CObriProjectFile::ReadOptionData (CObFile& File) {
  CSString LineBuffer;
  CSString Variable;
  CSString Value;
  bool	   Result;

  	/* Input until end of file or end of effect */
  while (!File.IsEOF()) {

		/* Input a single line of text from the file */
    Result = File.ReadLine(LineBuffer);
    if (!Result) return (false);
    ++m_InputLine;

		/* Parse input, ignore comments, whitespace trim, etc... */
    Result = LineBuffer.SeperateVarValue(Variable, Value);

    if (Variable.CompareNoCase("End") == 0) {
      return (true);
     }
    else if (Result) {
      SetOptionParameter(Variable, Value);
     }
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriProjectFile::ReadOptionData()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriProjectFile Method - bool Save (pFilename);
 *
 * Attempt to save the group data to the given file. Returns false on any error.
 *
 *=========================================================================*/
bool CObriProjectFile::Save (const SSCHAR* pFilename) {
  CObFile File;
  bool	  Result;
  dword   Index;
 
	/* Attempt to open file for output */
  Result = File.Open(pFilename, "wt");
  if (!Result) return (false);

  File.Printf("BaseItems = \"%s\"\n", m_BaseItems.c_str());
  File.Printf("Effects = \"%s\"\n", m_Effects.c_str());
  File.Printf("Uniques = \"%s\"\n", m_Uniques.c_str());
  File.Printf("LevelLists = \"%s\"\n", m_LevelLists.c_str());

  File.Printf("Options\n");
  File.Printf("\tOutputEsp = \"%s\"\n", m_Options.OutputEsp.c_str());
  File.Printf("\tOfString = \"%s\"\n", m_Options.OfString.c_str());
  File.Printf("\tIDPrefix = \"%s\"\n", m_Options.IDPrefix);
  File.Printf("\tExtendedID = \"%s\"\n", m_Options.ExtendedID.c_str());
  File.Printf("\tNameDelimiter = \"%s\"\n", m_Options.NameDelimiter.c_str());
  File.Printf("\tIDType = %s\n", ObriIDTypeToString(m_Options.IDType));
  File.Printf("\tMaxNameLength = %d\n", m_Options.MaxNameLength);
  File.Printf("\tHidePrefix2 = %s\n", BooleanToString(m_Options.HidePrefix2));
  File.Printf("\tLevelFactor = %g\n", m_Options.LevelFactor * 100.0f);
  File.Printf("\tForceMinLevel = %s\n", BooleanToString(m_Options.ForceMinLevel));
  File.Printf("\tItemMask = %s\n", ObriItemMaskToString(m_Options.ItemMask));
  File.Printf("\tMinLevel = %d\n", m_Options.MinLevel);
  File.Printf("\tMaxLevel = %d\n", m_Options.MaxLevel);
  File.Printf("\tNumItems = %d\n", m_Options.NumItems);
  File.Printf("\tMakeTotalItems = %s\n", BooleanToString(m_Options.MakeTotalItems));
  File.Printf("\tNumArmor = %d\n", m_Options.NumArmor);
  File.Printf("\tNumClothing = %d\n", m_Options.NumClothing);
  File.Printf("\tNumLights = %d\n", m_Options.NumLights);
  File.Printf("\tNumMisc = %d\n", m_Options.NumMisc);
  File.Printf("\tNumScrolls = %d\n", m_Options.NumScrolls);
  File.Printf("\tNumSigils = %d\n", m_Options.NumSigils);
  File.Printf("\tNumStaffs = %d\n", m_Options.NumStaffs);
  File.Printf("\tNumWeapons = %d\n", m_Options.NumWeapons);
  File.Printf("\tFirstItemFormID = 0x%08X\n", m_Options.FirstFormID1);
  File.Printf("\tFirstListFormID = 0x%08X\n", m_Options.FirstFormID2);
  File.Printf("\tRemoveNormalItems = %s\n", BooleanToString(m_Options.RemoveNormalItems));
  File.Printf("\tRemoveDuplicates = %s\n", BooleanToString(m_Options.RemoveDuplicates));
  File.Printf("\tCheckDuplicates = %s\n", BooleanToString(m_Options.CheckDuplicates));
  File.Printf("\tUseBell = %s\n", BooleanToString(m_Options.UseBell));
  File.Printf("\tRandomBell = %g\n", m_Options.RandomBell);
  File.Printf("\tPrefixChance = %g\n", m_Options.PrefixChance * 100.0f);
  File.Printf("\tSuffixChance = %g\n", m_Options.SuffixChance * 100.0f);
  File.Printf("\tUniqueChance = %g\n", m_Options.UniqueChance * 100.0f);
  File.Printf("\tMaterialChance = %g\n", m_Options.MaterialChance * 100.0f);
  File.Printf("\tCreateLevelLists = %s\n", BooleanToString(m_Options.CreateLevelLists));
  File.Printf("\tDistributeLists = %s\n", BooleanToString(m_Options.DistributeLists));
  File.Printf("\tCreateListParents = %s\n", BooleanToString(m_Options.CreateListParents));
  //File.Printf("\tCursedChance = %g\n", m_Options.CursedChance);
  File.Printf("End\n");

  for (Index = 0; Index < m_NumEspFiles; Index++) {
    File.Printf("ESPFile = \"%s\"\n", m_EspFiles[Index].c_str());
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriProjectFile::Save()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriProjectFile Method - bool SetOptionParameter (pVariable, pValue);
 *
 * Attempts to set an option value from the given variable/value pair.
 *
 *=========================================================================*/
bool CObriProjectFile::SetOptionParameter (const SSCHAR* pVariable, const SSCHAR* pValue) {
  bool Result;
  int  Value;

  if (stricmp(pVariable, "IDPrefix") == 0) {
    strnncpy(m_Options.IDPrefix, pValue, OBRI_MAX_NAMESIZE);
   }
  else if (stricmp(pVariable, "ExtendedID") == 0) {
    m_Options.ExtendedID = pValue;
   }
  else if (stricmp(pVariable, "NameDelimiter") == 0) {
    m_Options.NameDelimiter = pValue;
   }
  else if (stricmp(pVariable, "OutputEsp") == 0) {
    m_Options.OutputEsp = pValue;
   }
  else if (stricmp(pVariable, "OfString") == 0) {
    m_Options.OfString = pValue;
   }
  else if (stricmp(pVariable, "IDType") == 0) {
    Result = ObriIDTypeStringToValue(m_Options.IDType, pValue);
    if (!Result) return AddObGeneralError("%5ld: Unknown IDType value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "MaxNameLength") == 0) {
    Value = atoi(pValue);

    if (Value <= 0) {
      AddObGeneralError("%5ld: Invalid MaxNameLength value '%s'!", m_InputLine, pValue);
     }
    else {
      m_Options.MaxNameLength = Value;
     }
    }
  else if (stricmp(pVariable, "FirstItemFormID") == 0) {
    m_Options.FirstFormID1 = strtoul(pValue, NULL, 0);
    m_Options.FirstFormID1 &= 0x00FFFFFF;
    m_Options.FirstFormID1 |= 0x01000000;
   }
  else if (stricmp(pVariable, "FirstListFormID") == 0) {
    m_Options.FirstFormID2 = strtoul(pValue, NULL, 0);
    m_Options.FirstFormID2 &= 0x00FFFFFF;
    m_Options.FirstFormID2 |= 0x01000000;
   }
  else if (stricmp(pVariable, "MinLevel") == 0) {
    m_Options.MinLevel = atoi(pValue);
   }
  else if (stricmp(pVariable, "MaxLevel") == 0) {
    m_Options.MaxLevel = atoi(pValue);
   }
  else if (stricmp(pVariable, "NumItems") == 0) {
    m_Options.NumItems = atoi(pValue);
   }
  else if (stricmp(pVariable, "MakeTotalItems") == 0) {
    Result = StringToBoolean(m_Options.MakeTotalItems, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "NumArmor") == 0) {
    m_Options.NumArmor = atoi(pValue);
   }
  else if (stricmp(pVariable, "NumClothing") == 0) {
    m_Options.NumClothing = atoi(pValue);
   }
  else if (stricmp(pVariable, "NumLights") == 0) {
    m_Options.NumLights = atoi(pValue);
   }
  else if (stricmp(pVariable, "NumMisc") == 0) {
    m_Options.NumMisc = atoi(pValue);
   }
  else if (stricmp(pVariable, "NumScrolls") == 0) {
    m_Options.NumScrolls = atoi(pValue);
   }
  else if (stricmp(pVariable, "NumSigils") == 0) {
    m_Options.NumSigils = atoi(pValue);
   }
  else if (stricmp(pVariable, "NumStaffs") == 0) {
    m_Options.NumStaffs = atoi(pValue);
   }
  else if (stricmp(pVariable, "NumWeapons") == 0) {
    m_Options.NumWeapons = atoi(pValue);
   }
  else if (stricmp(pVariable, "RemoveNormalItems") == 0) {
    Result = StringToBoolean(m_Options.RemoveNormalItems, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "ItemMask") == 0) {
    m_Options.ItemMask = StringToObriItemMask(pValue);
   }
  else if (stricmp(pVariable, "RandomBell") == 0) {
    m_Options.RandomBell = (float) atof(pValue);
   }
  else if (stricmp(pVariable, "LevelFactor") == 0) {
    m_Options.LevelFactor = (float) atof(pValue) / 100.0f;
   }
  else if (stricmp(pVariable, "PrefixChance") == 0) {
    m_Options.PrefixChance = (float) atof(pValue) / 100.0f;
    if (m_Options.PrefixChance <= 0.01f) m_Options.PrefixChance *= 100.0f;	/* TODO: Temp fix in v0.35 */
   }
  else if (stricmp(pVariable, "SuffixChance") == 0) {
    m_Options.SuffixChance = (float) atof(pValue) / 100.0f;
    if (m_Options.SuffixChance <= 0.01f) m_Options.SuffixChance *= 100.0f;	/* TODO: Temp fix in v0.35 */
   }
  else if (stricmp(pVariable, "UniqueChance") == 0) {
    m_Options.UniqueChance = (float) atof(pValue) / 100.0f;
    if (m_Options.UniqueChance <= 0.01f) m_Options.UniqueChance *= 100.0f;	/* TODO: Temp fix in v0.35 */
   }
  else if (stricmp(pVariable, "MaterialChance") == 0) {
    m_Options.MaterialChance = (float) atof(pValue) / 100.0f;
   }
  else if (stricmp(pVariable, "CreateLevelLists") == 0) {
    Result = StringToBoolean(m_Options.CreateLevelLists, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "DistributeLists") == 0) {
    Result = StringToBoolean(m_Options.DistributeLists, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "CreateListParents") == 0) {
    Result = StringToBoolean(m_Options.CreateListParents, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "UseBell") == 0) {
    Result = StringToBoolean(m_Options.UseBell, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "ForceMinLevel") == 0) {
    Result = StringToBoolean(m_Options.ForceMinLevel, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "HidePrefix2") == 0) {
    Result = StringToBoolean(m_Options.HidePrefix2, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "RemoveDuplicates") == 0) {
    Result = StringToBoolean(m_Options.RemoveDuplicates, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else if (stricmp(pVariable, "CheckDuplicates") == 0) {
    Result = StringToBoolean(m_Options.CheckDuplicates, pValue);
    if (!Result) return AddObGeneralError("%5ld: Invalid boolean value '%s'!", m_InputLine, pValue);
   }
  else {
    AddObGeneralError("%5ld: Unknown option variable '%s'!", m_InputLine, pVariable);
    return (false);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriProjectFile::SetOptionParameter()
 *=========================================================================*/
 

/*===========================================================================
 *
 * Class CObriProjectFile Method - bool SetParameter (pVariable, pValue);
 *
 *=========================================================================*/
bool CObriProjectFile::SetParameter (const SSCHAR* pVariable, const SSCHAR* pValue) {

  if (stricmp(pVariable, "BaseItems") == 0) {
    m_BaseItems = pValue;
   }
  else if (stricmp(pVariable, "LevelLists") == 0) {
    m_LevelLists = pValue;
   }
  else if (stricmp(pVariable, "Effects") == 0) {
    m_Effects = pValue;
   }
  else if (stricmp(pVariable, "Uniques") == 0) {
    m_Uniques = pValue;
   }
  else if (stricmp(pVariable, "EspFile") == 0) {
    AddEspFile(pValue);
   }
  else {
    AddObGeneralError("%5ld: Unknown project parameter '%s'!", m_InputLine, pVariable);
    return (false);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObriProjectFile::SetParameter()
 *=========================================================================*/






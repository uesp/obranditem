/*===========================================================================
 *
 * File:	Obri_ProjectFile.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 21, 2006
 *
 * Defines the CObriFile class used for loading and saving session data 
 * and options used to create random items.
 *
 *=========================================================================*/
#ifndef __OBRI_PROJECTFILE_H
#define __OBRI_PROJECTFILE_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "obri_options.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

  #define OBRI_MAX_SCRIPTFILES 16

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriProjectFile Definition
 *
 * Holds random item session and option information.
 *
 *=========================================================================*/
class CObriProjectFile {

  /*---------- Begin Private Class Members ----------------------*/
private:
  CSString		m_BaseItems;
  CSString		m_Effects;
  CSString		m_Uniques;
  CSString		m_LevelLists;
  obri_options_t	m_Options;
  dword			m_InputLine;

  CSString		m_EspFiles[OBRI_MAX_SCRIPTFILES];
  dword			m_NumEspFiles;
  

  /*---------- Begin Protected Class Methods --------------------*/
protected:

	/* Set an option from a variable/value string */
  bool SetOptionParameter (const SSCHAR* pVariable, const SSCHAR* pValue);
  bool ReadOptionData     (CObFile& File);

  
  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Class Constructors/Destructors */
  CObriProjectFile();
  virtual ~CObriProjectFile() { Destroy(); }
  virtual void Destroy (void);

	/* Attempt to add an esp file to the group data */
  bool AddEspFile (const SSCHAR* pFilename);

  	/* Clear all merged esp files */
  void DeleteAllMergedEspFiles (void) { m_NumEspFiles = 0; }

	/* Get class members */
  const SSCHAR*   GetBaseItems   (void) { return (m_BaseItems); }
  const SSCHAR*   GetEffects     (void) { return (m_Effects); }
  const SSCHAR*   GetUniques     (void) { return (m_Uniques); }
  const SSCHAR*   GetLevelLists  (void) { return (m_LevelLists); }
  const SSCHAR*   GetOutputEsp   (void) { return (m_Options.OutputEsp); }
  obri_options_t& GetOptions     (void) { return (m_Options); }
  dword		  GetNumEspFiles (void) { return (m_NumEspFiles); }
  bool		  IsValidEspFile (const dword Index) { return (Index < m_NumEspFiles); }
  const SSCHAR*   GetEspFile     (const dword Index) { return (IsValidEspFile(Index) ? m_EspFiles[Index] : NULL); }
  bool		  HasBaseItems   (void) { return (!m_BaseItems.IsEmpty()); }
  bool		  HasEffects     (void) { return (!m_Effects.IsEmpty()); }
  bool		  HasUniques     (void) { return (!m_Uniques.IsEmpty()); }
  bool		  HasLevelLists  (void) { return (!m_LevelLists.IsEmpty()); }

	/* Attempt to load/save a group file */
  bool Load (const SSCHAR* pFilename, const bool ClearOptions = true);
  bool Save (const SSCHAR* pFilename);

	/* Sets a group parameter */
  bool SetParameter (const SSCHAR* pVariable, const SSCHAR* pValue);

	/* Set class members */
  void SetBaseItems  (const SSCHAR* pString) { m_BaseItems  = pString; }  
  void SetEffects    (const SSCHAR* pString) { m_Effects    = pString; }
  void SetLevelLists (const SSCHAR* pString) { m_LevelLists = pString; }
  void SetUniques    (const SSCHAR* pString) { m_Uniques    = pString; }
  void SetOutputEsp  (const SSCHAR* pString) { m_Options.OutputEsp = pString; }

 };
/*===========================================================================
 *		End of Class CObriProjectFile Definition
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Function Prototypes
 *
 *=========================================================================*/


/*===========================================================================
 *		End of Function Prototypes
 *=========================================================================*/


	
#endif
/*===========================================================================
 *		End of File Obri_ProjectFile.H
 *=========================================================================*/

/*===========================================================================
 *
 * File:	Obri_Generate.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 21, 2006
 *
 * Holds the main item generation routines.
 *
 *=========================================================================*/
#ifndef __OBRI_GENERATE_H
#define __OBRI_GENERATE_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "obri_baseitems.h"
  #include "obri_effects.h"
  #include "obri_uniques.h"
  #include "obri_projectfile.h"
  #include "obri_levellist.h"
  #include "obri_enchantmap.h"
  #include "obri_itemmap.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Number of items allowed in a levelled list */
  #define OBRI_MAXITEMS_LEVELLIST 100
 
	/* Number of levels to track the count of */
  #define OBRI_MAX_LEVELS 100

	/* Used in alternate item generation */
  #define OBRI_LEVEL_VARIANCE 2

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* Creation options */
  struct obri_generate_t {
	int		 CurrentItemLevel;
	int		 MinItemLevel;
	int		 MaxItemLevel;
	int		 CalcItemLevel;
	long		 CurrentEffectMask;
	float		 CurrentRatio;
	bool		 HasEnchantment;
	bool		 LastFailed;
	dword		 ItemValue;
	obri_item_t*	 pCurrentItem;
	obri_effect_t*	 pCurrentPrefix;
	obri_effect_t*	 pCurrentPrefix2;
	obri_effect_t*	 pCurrentSuffix;
	obri_unique_t*   pCurrentUnique;
   };

	/* Used for callback operations */
  struct obri_cbdata_t {
	dword	UserData;
	float	Completed;
   };

  typedef int (*FNOBRICALLBACK) (obri_cbdata_t Data);

  struct obri_callback_t {
	dword		UserData;
	FNOBRICALLBACK  Function;
   };

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriGenerate Definition
 *
 * Description
 *
 *=========================================================================*/
class CObriGenerate {

  /*---------- Begin Private Class Members ----------------------*/
private:
  CObriBaseItems	m_RandItems;		/* Main data classes */
  CObriEffects		m_RandEffects;
  CObriUniques		m_RandUniques;
  CObriProjectFile	m_ProjectFile;
  CObriLevelLists	m_LevelLists;

  CObriRefBaseItemArray	m_ValidItems;		/* Used when creating items */
  CObriRefEffectArray	m_ValidPrefixes;
  CObriRefEffectArray	m_ValidSuffixes;
  CObriRefUniqueArray	m_ValidUniques;
  obri_generate_t	m_GenerateData;

  CObriEnchantMap	m_EnchantMap;
  CObriItemMap		m_ItemMap;

  CSString		m_InitialPath;

  dword			m_NumNewItems;
  dword			m_NumEnchantments;
  dword			m_NumScripts;
  dword			m_ExportCount;
  dword			m_NumFailedItems;
  dword			m_NumDupEnchants;
  dword			m_NumDupItems;

  bool			m_HasMergedFile;
  
  CObRefRecordArray	m_NewRecords;		/* Newly created records */
  CObRefRecordArray	m_NewEnchantments;
  CObEspFile		m_EspFile;
  CObEspFile		m_SoundRecords;

  obri_callback_t	m_Callback;		/* Callback functions */

  dword			m_LevelCounts[OBRI_MAX_LEVELS + 1];
  dword			m_AverageLevelSum;


  /*---------- Begin Protected Class Methods --------------------*/
protected:

	/* Adds a single enchantment effect to the given record */
  void AddEnchantEffect (CObEffectItemRecord* pEnchant, obri_effect_t &Effect, obri_encheff_t& EnchantEffect);

	/* Adds an item to the level list */
  void AddLevelListItem  (CObRecord* pRecord);

	/* Allocates a new record according to the current settings */
  CObRecord* CreateNewRecord    (void);

	/* Returns true if the input item level is currently valid */
  bool CheckItemLevel       (const int Level);
  bool CheckItemLevel2      (obri_item_t* pItem);
  bool CheckEffectLevel     (const int Level);
  bool CheckEffectLevel2    (const int Level);
  bool CheckCursedLevel     (const int Level);
  bool CheckFinalItemLevel  (const int Level);
  bool CheckFinalItemLevel2 (const int Level);

	/* Checks the second prefix to ensure it is valid */
  bool CheckPrefix2 (void);

	/* Removes any suffixes that don't match the current prefix */
  void CheckValidSuffixes (void);

	/* Clear all the currently valid item arrays */
  void ClearValidItems (void);

	/* Compare two enchantments for equality */
  bool CompareEnchantments     (CObEnchRecord* pEnchant1, CObEnchRecord* pEnchant2);
  bool CompareEffects          (CObEnchRecord* pEnchant1, CObEfidSubrecord* pEffect1, CObEnchRecord* pEnchant2, CObEfidSubrecord* pEffect2);

  	/* Check if an item is "normal" */
  bool CheckNormalItem (void);
  
	/* Compute any magical effects for the current item */
  void ComputeCurrentEffects  (void);
  void ComputeCurrentEffects2 (void);
  
	/* Computes a new current item level */
  void ComputeItemLevel    (void);
  void ComputeItemLevel2   (void);
  void ComputeQualityLevel (void);
  int  ComputeItemLevel    (CObRecord* pItem);

	/* Create the given enchant for the record */
  void CreateEffect (CObRecord* pRecord, obri_effect_t* pEffect, CObEnchRecord** ppEnchant, const bool ForceUnique = false);

	/* Create/add specific effect types */
  void CreateEnchantment  (CObRecord* pRecord, obri_effect_t* pEffect, CObEnchRecord** ppEnchant, const bool ForceUnique = false);
  void CreateSigilEnchant (CObSgstRecord* pSigil, obri_effect_t* pEffect);
  void CreateArmor        (CObRecord* pRecord, obri_effect_t* pEffect);
  void CreateDamage       (CObRecord* pRecord, obri_effect_t* pEffect);
  void CreateEnchantPts   (CObRecord* pRecord, obri_effect_t* pEffect);
  void CreateWeight       (CObRecord* pRecord, obri_effect_t* pEffect);
  void CreateReach        (CObRecord* pRecord, obri_effect_t* pEffect);
  void CreateSpeed        (CObRecord* pRecord, obri_effect_t* pEffect);
  void CreateValue        (CObRecord* pRecord, obri_effect_t* pEffect);
  void CreateScript       (CObRecord* pRecord, obri_effect_t* pEffect);
  void CreateHealth       (CObRecord* pRecord, obri_effect_t* pEffect); 

	/* Main item creation function */
  bool CreateItem (const bool CheckDups);

	/* Export record data from an ESP/ESM file */
  bool ExportArmor      (CObFile& OutputFile, CObArmoRecord* pArmor);
  bool ExportClothing   (CObFile& OutputFile, CObClotRecord* pClothing);
  bool ExportMisc       (CObFile& OutputFile, CObMiscRecord* pMisc);
  bool ExportBook       (CObFile& OutputFile, CObBookRecord* pBook);
  bool ExportWeapon     (CObFile& OutputFile, CObWeapRecord* pWeapon);
  bool ExportBipedParts (CObFile& OutputFile, const dword Flags);
  bool ExportLevelList  (CObFile& OutputFile, CObEspFile& InputFile, CObLvliRecord* pLevelList);

	/* Get the item level of the current item */
  int GetCurrentItemLevel (void);

	/* Checks if a particular effect is negative or not */
  bool IsNegativeEffect (obri_effect_t* pEffect);

	/* Checks if the effect/item pair is valid */
  bool IsValidEffect (obri_effect_t* pEffect, obri_item_t* pItem);

	/* Makes a random item and stores it in the new array */
  bool MakeRandomItem  (void);
  bool MakeRandomItem2 (void);

	/* Make all items in the requested manner */
  bool MakeIndividualItems (void);
  bool MakeCountItems      (const dword ItemCount);

  	/* Make a new enchantment/spell object */
  void MakeEnchantment (CObRecord* pRecord, CObEnchRecord** ppEnchant);

	/* Merge the external ESP files */
  bool MergeEspFiles (void);

	/* Create base item strings  */
  const SSCHAR* MakeItemID         (void);
  const SSCHAR* MakeExtItemId      (void);
  const SSCHAR* MakeItemName       (void);
  const SSCHAR* MakeUniqueItemName (void);
  obformid_t    MakeFormID         (void);
  obformid_t    MakeListFormID     (void);

	/* Select a base item to use */
  void SelectBaseItem  (void);
  void SelectBaseItem2 (void);

	/* Select all valid items/effects/uniques based on the current settings */
  void SelectValidItems      (void);
  void SelectValidSuffixes   (void);
  void SelectValidPrefixes   (void);
  void SelectValidUniques    (void);
  void SelectValidSuffixes2  (void);
  void SelectValidPrefixes2  (void);
  void SelectValidUniques2   (void);
  void SelectValidSuffixes2a (void);
  void SelectValidPrefixes2a (void);
  void SelectValidUniques2a  (void);
  void SelectValidItems2     (void);

	/* Sets the new record parameters according to the current settings */
  void SetRecordItemParams (CObRecord*     pRecord);
  void SetRecordEffects    (CObRecord*     pRecord);

  void SetArmorParams      (CObArmoRecord* pArmor);
  void SetBookParams	   (CObBookRecord* pBook);
  void SetBipedParts       (CObRecord*     pRecord);
  void SetClothingParams   (CObClotRecord* pClothing);
  void SetLightParams      (CObLighRecord* pLight);
  void SetSigilParams	   (CObSgstRecord* pSigil);  
  void SetMiscParams	   (CObMiscRecord* pMisc);
  void SetWeaponParams     (CObWeapRecord* pWeapon);
  void SetItemValue        (CObRecord*     pRecord, const dword Value);
  

  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Class Constructors/Destructors */
  CObriGenerate();
  virtual ~CObriGenerate() { Destroy(); }
  virtual void Destroy (void);

  	/* Creates all items accorinding to the current settings */
  bool CreateItems     (const bool DoFullReset = true);
  bool CreateMoreItems (void);
  
	/* Delete all records in the new array */
  void DestroyNewRecords (void);

	/* Export base item data from an ESP/ESM file */
  bool ExportItemData   (const SSCHAR* pEsmFile, const SSCHAR* pOutputFile);
  bool ExportItemData   (CObEspFile&  InputFile, const SSCHAR* pOutputFile);
  bool ExportLevelLists (CObEspFile&  InputFile, const SSCHAR* pOutputFile);
  bool ExportSounds     (CObEspFile&  InputFile, const SSCHAR* pOutputFile);

	/* Find an item key object by its record */
  obri_itemkey_t* FindItemKey (CObIdRecord* pIdRecord);

	/* Get a subrecord in the record */
  CObSubrecord* FindSubrecord (const obrectype_t Name);

	/* Get class members */
  obri_options_t&	GetOptions         (void) { return (m_ProjectFile.GetOptions()); }
  CObRefRecordArray*    GetNewRecords      (void) { return (&m_NewRecords); }
  CObEspFile&		GetEspFile         (void) { return (m_EspFile); }
  CObriProjectFile&	GetProjectFile     (void) { return (m_ProjectFile); }
  dword			GetNumNewItems     (void) { return (m_NumNewItems); }
  dword			GetNumEnchantments (void) { return (m_NumEnchantments); }
  dword			GetNumLevelLists   (void) { return (m_LevelLists.GetCreationCount()); }
  dword			GetNumBaseItems    (void) { return (m_RandItems.GetNumItems()); }
  dword			GetNumSuffixes     (void) { return (m_RandEffects.GetNumSuffixes()); }
  dword			GetNumPrefixes     (void) { return (m_RandEffects.GetNumPrefixes()); }
  dword			GetNumUniques      (void) { return (m_RandUniques.GetNumUniques()); }

	/* Initialize the ESP file for use */
  void InitializeEspFile (void);

  	/* Initializes the set of default sound records */
  void InitializeSoundRecords (void);

	/* Attempt to load the data files */
  bool LoadDataFiles    (const bool ForceClear = true);
  bool LoadBaseItems    (const SSCHAR* pFilename);
  bool LoadEffects      (const SSCHAR* pFilename);
  bool LoadUniques      (const SSCHAR* pFilename);
  bool LoadLevelLists   (const SSCHAR* pFilename);
  bool LoadProjectFile  (const SSCHAR* pFilename);
  bool SaveProjectFile  (const SSCHAR* pFilename);
  bool MergeProjectFile (const SSCHAR* pFilename);

	/* Saves the current level counts to the given file in a CSV format */
  bool OutputLevelCounts (const SSCHAR* pFilename);

	/* Resets the generation settings */
  void Reset        (void);
  void PartialReset (void);

	/* Set the callback function used when creating items */
  void SetCallback (FNOBRICALLBACK Callback, dword UserData) { m_Callback.Function = Callback; m_Callback.UserData = UserData; }
	
 };
/*===========================================================================
 *		End of Class CObriGenerate Definition
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Function Prototypes
 *
 *=========================================================================*/

	/* Set default options */
  void DefaultObriGenerateOptions (obri_generate_t& Options);
   
/*===========================================================================
 *		End of Function Prototypes
 *=========================================================================*/



#endif
/*===========================================================================
 *		End of File Mwranditemgen.H
 *=========================================================================*/


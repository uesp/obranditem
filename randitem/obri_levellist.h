/*===========================================================================
 *
 * File:	Obri_levellist.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 4, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBRI_LEVELLIST_H
#define __OBRI_LEVELLIST_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "obri_defs.h"
  #include "windows/obrecordfilter.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Number of item masks allowed */
  #define OBRI_LLNUM_TYPES		8

	/* Masks for selecting items in list */
  #define OBRI_LLMASK_NONE		0
  #define OBRI_LLMASK_ITEMTYPES		1
  #define OBRI_LLMASK_WEAPONTYPES	2
  #define OBRI_LLMASK_ARMORTYPES	4
  #define OBRI_LLMASK_BIPEDPARTS	8
  #define OBRI_LLMASK_MINLEVEL		16
  #define OBRI_LLMASK_MAXLEVEL		32
  #define OBRI_LLMASK_ITEMNAME		64
  #define OBRI_LLMASK_CUSTOMTYPES	128
  #define OBRI_LLMASK_ITEMEFFECT	256

	/* Used in armor types */
  #define OBRI_LLARMOR_LIGHT	0
  #define OBRI_LLARMOR_HEAVY	1

	/* Item effect types */
  #define OBRI_ITEMEFFECT_NONE		1
  #define OBRI_ITEMEFFECT_PREFIX	2
  #define OBRI_ITEMEFFECT_SUFFIX	4
  #define OBRI_ITEMEFFECT_ONLYPREFIX	8
  #define OBRI_ITEMEFFECT_ONLYSUFFIX	16
  #define OBRI_ITEMEFFECT_ANY		32

	/* Initial ID counter for list editor IDs */
  #define OBRI_LLID_START 1

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* Forward class definition */
  class CObriLevelList;

  	/* Holds parent information */
  struct obri_listparent_t {
	CSString	Name;
	CObriLevelList*	pParent;

	obri_listparent_t() { 
		pParent = NULL; 
	 }
   };

	/* Array of parent information */
  typedef CObPtrArray<obri_listparent_t> CObriListParentArray;

	/* Holds item insertion information */
  struct obri_listinsert_t {
	CSString		EditorID;		/* Item to add */
	obformid_t		FormID;
	CObriListParentArray	Lists;			/* Lists to add item to */
	int			Count;			/* Number of times to insert item into the list */
	int			Level;

	obri_listinsert_t() { Count = 1; FormID = OB_FORMID_NULL; Level = 1; }

	bool SetParameter (const SSCHAR* Variable, const SSCHAR* Value);
   };

  	/* Array of level item pointer references */
  typedef CObRefPtrArray<CObLvliRecord> CObLvliRefArray;

	/* Array of list insertions */
  typedef CObPtrArray<obri_listinsert_t> CObriListInsertArray;

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriLevelList Definition
 *
 * Holds the definition need to create one levelled item list.
 *
 *=========================================================================*/
class CObriLevelList {

  /*---------- Begin Protected Class Members --------------------*/
protected:
  CObLvliRecord*	m_pLevelList;		/* The current level list record */
  CObEspFile*		m_pEspFile;		/* Reference to the destination plugin file */

  CObLvliRefArray	m_LevelLists;		/* References to created lists */

  CSString		m_Name;			/* Unique name prefix identifying the list */
  CObriListParentArray	m_Parents;		/* Link to parent list(s) */

  dword			m_IDCount;		/* Used when creating the level lists editor IDs */
  dword			m_CreationCount;	/* Number of lists created */
  dword			m_ItemCount;		/* Number of items added */

  bool			m_IsEmpty;		/* An empty list contains no item records */
  bool			m_ResetLevels;		/* Set all levels to 1 when adding items to the list */
  dword			m_MaxRecords;		/* Number of records allowed in the list */

  dword			m_ChanceNone;		/* Level list parameters */
  bool			m_CalculateEach;
  bool			m_CalculateAll;

  dword			m_ListMask;		/* Which types of masks are in effect? */

  dword			m_ItemTypes;				/* Items the list can contain */
  dword			m_WeaponTypes[OBRI_LLNUM_TYPES];	/* Weapon type the list can contain */
  dword			m_NumWeaponTypes;
  dword			m_ArmorTypes[OBRI_LLNUM_TYPES];		/* Armor type the list can contain */
  dword			m_NumArmorTypes;
  dword			m_BipedParts;				/* Biped parts the list can contain */
  dword			m_BipedCompare;				/* How the biped parts are compared */
  long			m_MinLevel;				/* Valid level range of items in list */
  long			m_MaxLevel;
  int			m_ItemEffectType;	/* Types of item effects the list can contain */

  CSString		m_NameFilter;				/* Used to filter based on the item name */

  CObriStringArray	m_CustomTypes;		/* Used to filter based on the item custom type */


  /*---------- Begin Protected Class Methods --------------------*/
protected:

	/* Adds a new parent to the list */
  bool AddParent (const SSCHAR* pValue);

	/* Helper check methods */
  bool CheckItemType    (CObRecord* pRecord);
  bool CheckArmorTypes  (CObRecord* pRecord);
  bool CheckWeaponTypes (CObRecord* pRecord);
  bool CheckItemMasks   (CObRecord* pRecord, const SSCHAR* pItemMask);
  bool CheckItemEffects (CObRecord* pRecord, const void* pPrefix, const void* pPrefix1, const void* pSuffix);

	/* Create a new list editor ID */
  const SSCHAR* MakeListEditorID (void);

	/* Helper parse methods */
  bool ParseArmorTypes  (const SSCHAR* pValue);
  bool ParseWeaponTypes (const SSCHAR* pValue);


  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Class Constructors/Destructors */
  CObriLevelList();
  virtual ~CObriLevelList() { Destroy(); }
  virtual void Destroy (void);

	/* Add a record to the level list */
  bool AddRecord    (CObRecord* pRecord);
  bool AddToParents (CObRecord* pRecord);
  bool AddRecord    (const obformid_t FormID, int Level, int Count = 1);

	/* Checks if the record can be added to the list */
  bool CheckRecord (CObRecord* pRecord, const SSCHAR* pCustomType, const void* pPrefix, const void* pPrefix1, const void* pSuffix);

	/* Ensure all lists have the same number of items in them */
  void DistributeLists (void);

	/* Get class members */
  const SSCHAR* GetName          (void) { return (m_Name); }
  dword         GetCreationCount (void) { return (m_CreationCount); }
  
  dword              GetNumParents      (void)              const { return (m_Parents.GetSize()); }
  bool               IsValidParentIndex (const dword Index) const { return (Index < m_Parents.GetSize()); }
  obri_listparent_t* GetParent          (const dword Index)       { return (m_Parents[Index]); }
  const SSCHAR*      GetParentName      (const dword Index);
  bool		     IsEmpty		(void)		    const { return (m_IsEmpty); }
  CObLvliRecord*     GetLevelList       (void)		          { return (m_pLevelList); }

	/* Check masks */
  bool IsMaskItemType    (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_ITEMTYPES); }
  bool IsMaskWeaponTypes (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_WEAPONTYPES); }
  bool IsMaskArmorTypes  (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_ARMORTYPES); }
  bool IsMaskBipedParts  (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_BIPEDPARTS); }
  bool IsMaskMinLevel    (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_MINLEVEL); }
  bool IsMaskMaxLevel    (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_MAXLEVEL); }
  bool IsMaskLevel       (void) { return IsMaskMinLevel() && IsMaskMaxLevel(); }
  bool IsMaskItemName    (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_ITEMNAME); }
  bool IsMaskItemMasks   (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_CUSTOMTYPES); }
  bool IsMaskItemEffects (void) { return CheckFlagBits(m_ListMask, OBRI_LLMASK_ITEMEFFECT); }

  	/* Creates a new level list record */
  bool MakeNewList (void);

	/* Used before a new set of items is generated */
  void Reset (void);
   
  	/* Sets a parameter from the given value-variable string pair */
  bool SetParameter (const SSCHAR* pVariable, const SSCHAR* pValue);

	/* Set class members */
  void SetEspFile (CObEspFile* pFile) { m_pEspFile = pFile; }

 };

	/* Pointer array of level lists */
  typedef CObPtrArray<CObriLevelList>    CObriLevelListArray;
  typedef CObRefPtrArray<CObriLevelList> CObriRefLevelListArray;
/*===========================================================================
 *		End of Class CObriLevelList Definition
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriLevelLists Definition
 *
 * Manages an array of lists.
 *
 *=========================================================================*/
class CObriLevelLists {

  /*---------- Begin Protected Class Members --------------------*/
protected:
  CObriLevelListArray	m_LevelLists;		/* Array of level list definitions */
  CObriListInsertArray	m_ListInserts;		/* Prefined items to insert into lists */

  CObEspFile*		m_pEspFile;		/* Reference to the destination plugin file */


  /*---------- Begin Protected Class Methods --------------------*/
protected:

  bool AddInsertItems (obri_listinsert_t* pListInsert);

	/* Resolve parent list pointers in new data */
  bool ResolveParents       (void);
  bool ResolveListParents   (CObriLevelList* pList);
  bool ResolveInsertParents (obri_listinsert_t* pListInsert);


  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Class Constructors/Destructors */
  CObriLevelLists();
  virtual ~CObriLevelLists() { Destroy(); }
  virtual void Destroy (void);

	/* Adds predefined insert items to the lists */
  bool AddInsertItems     (void);
  bool ResolveInsertItems (void);

  	/* Add a record to any matching lists */
  int AddRecord (CObRecord* pRecord, const SSCHAR* pCustomType, const void* pPrefix, const void* pPrefix1, const void* pSuffix);

	/* Ensure all parents are created */
  bool CreateParents (void);
  
	/* Ensure all lists have the same number of items in them */
  void DistributeLists (void);

	/* Find a list based on its name */
  CObriLevelList* Find (const SSCHAR* pString);

	/* Get class members */
  CObriLevelList* GetLevelList     (const dword Index) { return m_LevelLists[Index]; }
  dword           GetNumLevelLists (void)              { return (m_LevelLists.GetSize()); }
  dword           GetCreationCount (void);

	/* Load a level list data file */
  bool Load (const SSCHAR* pFilename);

  	/* Used before a new set of items is generated */
  void Reset (void);

  	/* Set class members */
  void SetEspFile (CObEspFile* pFile) { m_pEspFile = pFile; }

 };
/*===========================================================================
 *		End of Class CObriLevelLists Definition
 *=========================================================================*/


#endif
/*===========================================================================
 *		End of File Obri_levellist.H
 *=========================================================================*/

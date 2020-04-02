/*===========================================================================
 *
 * File:	Obri_options.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 21, 2006
 *
 * Defines options for the random item generation.
 *
 *=========================================================================*/
#ifndef __OBRI_OPTIONS_H
#define __OBRI_OPTIONS_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "obri_defs.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Default option values */
  #define OBRI_DEFAULT_PREFIXCHANCE	 0.25f
  #define OBRI_DEFAULT_SUFFIXCHANCE	 0.50f
  #define OBRI_DEFAULT_UNIQUECHANCE	 0.25f	
  #define OBRI_DEFAULT_CHANCENONE	 10.0f	
  #define OBRI_DEFAULT_RANDOMBELL	 50.0f
  #define OBRI_DEFAULT_USEBELL		 false
  #define OBRI_DEFAULT_ITEMTYPEMASK	 OBRI_ITEMTYPE_ALL
  #define OBRI_DEFAULT_IDTYPE		 OBRI_IDTYPE_LONG
  #define OBRI_DEFAULT_MINLEVEL		 1
  #define OBRI_DEFAULT_MAXLEVEL		 100
  #define OBRI_DEFAULT_NUMITEMS		 100
  #define OBRI_DEFAULT_CREATELEVELLISTS  true
  #define OBRI_DEFAULT_DISTRIBUTELISTS	 true
  #define OBRI_DEFAULT_MAXNAMELENGTH	 32
  #define OBRI_DEFAULT_REMOVENORMALITEMS false
  #define OBRI_DEFAULT_HIDEPREFIX2       true
  #define OBRI_DEFAULT_LEVELFACTOR	 0.1f
  #define OBRI_DEFAULT_MATERIALCHANCE	 0.75f
  #define OBRI_DEFAULT_OFSTRING		 " of "
  #define OBRI_DEFAULT_FIRSTFORMID1	 0x01001000
  #define OBRI_DEFAULT_FIRSTFORMID2	 0x01E00000

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* Basic data for a single group */	
  struct obri_options_t {

	CSString OutputEsp;				/* Output filename */
	CSString OfString;				/* Contains the localized " of " string value */

  	SSCHAR	 IDPrefix[OBRI_MAX_NAMESIZE + 1];	/* Prefix for EditorID generation */
	CSString ExtendedID;
	CSString NameDelimiter;				/* Used to prefix/suffix the overall name */
	int	 IDType;				/* Type of editor ID generation to use */
	int      MaxNameLength;				/* Maximum item name length in characters */
	bool	 HidePrefix2;				/* Don't show the prefix2 name */

	float    LevelFactor;				/* Used to adjust item levels */
	bool	 ForceMinLevel;

	int	 MinLevel;				/* Range of item levels to create */
	int	 MaxLevel;
	bool	 RemoveDuplicates;			/* Remove duplicated items */
	bool	 CheckDuplicates;
	bool	 RemoveNormalItems;			/* Don't generate unenchanted items */
	int	 ItemMask;				/* Types of items to create */

	dword	 NumItems;				/* Number of items to create */
	bool	 MakeTotalItems;			/* Determines the type of generation */
	dword	 NumArmor;
	dword	 NumClothing;
	dword	 NumLights;
	dword	 NumMisc;
	dword	 NumScrolls;
	dword	 NumSigils;
	dword	 NumStaffs;
	dword	 NumWeapons;

	dword    FirstFormID1;
	dword    FirstFormID2;
	
	bool	 UseBell;				/* Use a bell curve for random numbers */
	float	 RandomBell;				/* Ratio when using a bell (0-1) 0.5 = Normal distribution */
			
	float	 PrefixChance;				/* Chance for a prefix */
	float	 SuffixChance;				/* Chance for a suffix */
	float	 UniqueChance;				/* Chance for a unique item */
	float	 MaterialChance;			/* Chance of a material being including the item name */

	bool	 CreateLevelLists;			/* Are we creating any leveled lists? */
	bool	 DistributeLists;
	bool	 CreateListParents;			/* Create parents first */

   };

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Function Definitions
 *
 *=========================================================================*/

  void DefaultObriOptions (obri_options_t& Options);

/*===========================================================================
 *		End of Function Definitions
 *=========================================================================*/


#endif
/*===========================================================================
 *		End of File Obri_options.H
 *=========================================================================*/

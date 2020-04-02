/*===========================================================================
 *
 * File:	obri_defs.h
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 20, 2006
 *
 * Common definitions for ObRandItem
 *
 *=========================================================================*/
#ifndef __OBRIDEFS_H
#define __OBRIDEFS_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "modfile/obespfile.h"
  #include "common/obptrarray.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Various compile time options */
  #define OBRI_EFFECTLEVELCHECK		1
  #define OBRI_USEVALUEMULTIPLIER	0

	/* Maximum size of generated IDs */
  #define OBRI_MAXID_SIZE 32

	/* Where FormIDs start counting from */
  #define OBRI_FIRST_FORMID 0x01001000
	
	/* Types of IDs */
  #define OBRI_IDTYPE_LONG	0
  #define OBRI_IDTYPE_SHORT8	1
  #define OBRI_IDTYPE_SHORT6	2
  #define OBRI_IDTYPE_SHORT4	3
  #define OBRI_IDTYPE_SHORT	4
  #define OBRI_IDTYPE_EXT       5

  	/* Default file names */
  #define OBRI_DEFAULTFILE_ITEM		"baseitems.dat"
  #define OBRI_DEFAULTFILE_EFFECT	"effects.dat"
  #define OBRI_DEFAULTFILE_UNIQUE	"uniques.dat"
  #define OBRI_DEFAULTFILE_LEVELLIST	"levellists.dat"

	/* Special characters */
  #define OBRI_ITEMSEPCHAR   '|'
  #define OBRI_ITEMSEPSTRING "|"

	/* Maximum string name size (arbitrary) */
  #define OBRI_MAX_NAMESIZE	64
  #define OBRI_MAX_NICENAMESIZE 32

	/* Maximum filename size */
  #define OBRI_MAX_FILEESIZE	_MAX_PATH + 1

	/* Temporary buffer size */
  #define OBRI_MAX_BUFFERSIZE 511

  	/* Item types */
  #define OBRI_ITEMTYPE_WEAPON		1
  #define OBRI_ITEMTYPE_ARMOR		2
  #define OBRI_ITEMTYPE_CLOTHING	4
  #define OBRI_ITEMTYPE_APPAREL		6
  #define OBRI_ITEMTYPE_MISC		8
  #define OBRI_ITEMTYPE_SCROLL		16
  #define OBRI_ITEMTYPE_STAFF		32
  #define OBRI_ITEMTYPE_SIGIL		64
  #define OBRI_ITEMTYPE_LIGHT		128
  #define OBRI_ITEMTYPE_ALL		0xFFFF

	/* Effect masks */
  #define OBRI_EFFTYPE_NONE	   1
  #define OBRI_EFFTYPE_ENCHANT	   2
  #define OBRI_EFFTYPE_WEIGHT	   4
  #define OBRI_EFFTYPE_DAMAGE	   8
  #define OBRI_EFFTYPE_SCRIPT	   16
  #define OBRI_EFFTYPE_ENCHANTPTS  32
  #define OBRI_EFFTYPE_REACH	   64
  #define OBRI_EFFTYPE_SPEED       128
  #define OBRI_EFFTYPE_HEALTH      256
  #define OBRI_EFFTYPE_ARMOR       512
  #define OBRI_EFFTYPE_VALUE       1024
  #define OBRI_EFFTYPE_ALL         0xFFFFFFFF

	/* Enchantment masks */
  #define OBRI_ENCTYPE_STRIKES	   2
  #define OBRI_ENCTYPE_CONSTANT    4
  #define OBRI_ENCTYPE_USED	   8
  #define OBRI_ENCTYPE_ONCE	   16
  #define OBRI_ENCTYPE_TARGET      32
  #define OBRI_ENCTYPE_ALL         0xFFFFFFFF

	/* Number of decimal places to round values to */
  #define OBRI_ROUND_SPEED  0.01f
  #define OBRI_ROUND_WEIGHT 0.01f
  #define OBRI_ROUND_REACH  0.01f

	/* Used to check the valid base item level range */
  #define OBRI_MINLEVEL_FACTOR 0.5

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* String arrays */
  typedef CObPtrArray<CSString> CObriStringArray;

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Function Prototypes
 *
 *=========================================================================*/

	/* General random number generator */
  float MakeRandItemGaussian (const float FromValue, const float ToValue, const float PercentMean);
  float MakeRandItemNumber   (const float FromValue, const float ToValue);
  float MakeRandItemNumber   (const float FromValue, const float ToValue, const float BellValue);
  dword MakeRandItemIndex    (const dword MaxSize);
  float MakeRandItemRatio    (void);

	/* Splits values according to a ratio (0-1.0) */
  float SplitValue (const float FromValue, const float ToValue, const float Ratio);
  long  SplitValue (const long  FromValue, const long  ToValue, const float Ratio);

	/* Generate random item IDs */
  const SSCHAR* MakeRandomItemIdLong  (const SSCHAR* pPrefix);
  const SSCHAR* MakeNumberItemId8     (const SSCHAR* pPrefix);
  const SSCHAR* MakeNumberItemId6     (const SSCHAR* pPrefix);
  const SSCHAR* MakeNumberItemId4     (const SSCHAR* pPrefix);
  const SSCHAR* MakeNumberItemIdShort (const SSCHAR* pPrefix);

	/* String conversion methods */
  dword         StringToObriItemMask      (const SSCHAR* pString);
  int           StringToObriItemType      (const SSCHAR* pString);
  const SSCHAR* ObriItemTypeToString      (const dword ItemType);
  const SSCHAR* ObriItemMaskToString      (const dword ItemMask);
  bool          StringToObriEffectType    (int &Value, const SSCHAR* pString);
  bool          StringToObriEnchantType   (int &Value, const SSCHAR* pString);

  const SSCHAR* ObriIDTypeToString      (const int IDType);
  bool          ObriIDTypeToString      (const SSCHAR** ppString, const int IDType);
  bool          ObriIDTypeStringToValue (int& IDType, const SSCHAR* ppString);
  
  dword ObriGetNextFormID1   (CObEspFile* pEspFile = NULL);
  dword ObriGetNextFormID2   (CObEspFile* pEspFile = NULL);
  
  void ObriResetNextFormID1 (const dword FormID);
  void ObriResetNextFormID2 (const dword FormID); 

/*===========================================================================
 *		End of Function Prototypes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin External Definitions
 *
 *=========================================================================*/
  extern long l_ItemIDCount;
/*===========================================================================
 *		End of External Definitions
 *=========================================================================*/



#endif
/*===========================================================================
 *		End of File obri_defs.H
 *=========================================================================*/

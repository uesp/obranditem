/*===========================================================================
 *
 * File:	Obri_BaseItems.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	20 April 2006
 *
 * Defines the CObriBaseItems class for handling base item data.
 *
 *=========================================================================*/
#ifndef __OBRI_BASEITEMS_H
#define __OBRI_BASEITEMS_H


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

	
/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* Weapon specific data */
  typedef struct {
	long  Health;
	word  Damage;
	float Speed;
	float Reach;
	dword Type;
	bool  IgnoreResist;
	word  EnchantCharge;
   } obri_weapon_t;

	/* Armor specific data */
  typedef struct {
	dword Health;
	word  Armor;
	dword BipedParts;
	bool  HideAmulet;
	bool  HideRings;
	bool  HeavyArmor;
   } obri_armor_t;

   	/* Scroll specific data */
  typedef struct {
	word  Teaches;
	bool  IsScroll;
   } obri_scroll_t;

   	/* Clothing specific data */
  typedef struct {
	bool  HideAmulet;
	bool  HideRings;
	dword BipedParts;
   } obri_clothing_t;

      	/* Sigil stone specific data */
  typedef struct {
	byte Uses;
   } obri_sigil_t;

	/* Light specific data */
  typedef struct {
	obrgba_t	Color;	
	float		Falloff;
	float		FOV;
	float		Fade;
	long		Time;
	dword		Radius;
	int		Type;
	bool		Dynamic;
	bool		Negative;
	bool		DefaultOff;
	bool		SpotLight;
	bool		SpotShadow;
	bool		Carried;
	SSCHAR		Sound[OBRI_MAX_NAMESIZE+1];;
   } obri_light_t;

	/* Base item data */
  typedef struct {
  	SSCHAR Name[OBRI_MAX_NAMESIZE+1];	/* Base item name string */
	SSCHAR Material[OBRI_MAX_NAMESIZE+1];	/* Optional material of the item */
	SSCHAR CustomType[OBRI_MAX_NAMESIZE+1];	/* Used in level list definitions */
	dword  ItemType;			/* Item type (bitmask) */
	dword  BasePrice;			/* Base item price */
	float  Weight;				/* Base item weight */
	dword  EffectMask;			/* Types of valid effects for the item */	
	dword  EnchantMask;			/* Types of valid enchantments for the item */
	int    Frequency;			/* Used to create duplicates if greater than 1 */

	int   MinEffectLevel;			/* Valid effect levels to apply on item */
	int   MaxEffectLevel;


	union {		/* Specific item type data */
		obri_armor_t	Armor;
		obri_weapon_t	Weapon;
		obri_clothing_t Clothing;
		obri_scroll_t   Scroll;
		obri_sigil_t	Sigil;
		obri_light_t	Light;
	};

	long   ItemLevel;			/* Base item level (higher level items will be rarer) (0-100) */

	union {
		SSCHAR Model[OBRI_MAX_NAMESIZE+1];
		SSCHAR MBModel[OBRI_MAX_NAMESIZE+1];
	};

	union {
		SSCHAR Icon[OBRI_MAX_NAMESIZE+1];
		SSCHAR MIcon[OBRI_MAX_NAMESIZE+1];
	};

	SSCHAR FBModel[OBRI_MAX_NAMESIZE+1];	/* Additional models/icons for clothing/armor */
	SSCHAR MWModel[OBRI_MAX_NAMESIZE+1];
	SSCHAR FWModel[OBRI_MAX_NAMESIZE+1];
	SSCHAR FIcon[OBRI_MAX_NAMESIZE+1];
   } obri_item_t;

	/* Pointer array of items */
  typedef CObPtrArray<obri_item_t>    CObriBaseItemArray;
  typedef CObRefPtrArray<obri_item_t> CObriRefBaseItemArray;


/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriBaseItems Definition
 *
 * An array of base item records used for random item creation.
 *
 *=========================================================================*/
class CObriBaseItems {

  /*---------- Begin Private Class Members ----------------------*/
private:
  CObriBaseItemArray	m_BaseItems;	/* Array of item pointers */
  long			m_InputLine;


  /*---------- Begin Protected Class Methods --------------------*/
protected:

	/* Find a body part index */
  int FindBodyPart (const SSCHAR* pString);

	/* Read item from text file */
  bool ReadItem (CObFile& File, obri_item_t* pNewItem);
  bool ReadItem (CObFile& File);

	/* Parse an effect/enchant mask value string */
  dword ParseEffectMask    (const SSCHAR* pValue);
  dword ParseEnchantMask   (const SSCHAR* pValue);
  dword ParseBipedFlag     (const SSCHAR* pValue);

	/* Set item parameters from string values */
  bool SetItemParam      (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetArmorParam     (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetClothingParam  (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetLightParam     (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetMiscParam      (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetSigilParam     (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetScrollParam    (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetWeaponParam    (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetMaleBodyPart   (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetFemaleBodyPart (obri_item_t* pNewItem, const SSCHAR* pVariable, const SSCHAR* pValue);


  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Class Constructors/Destructors */
  CObriBaseItems();
  virtual ~CObriBaseItems() { Destroy(); }
  virtual void Destroy (void);

	/* Get class members */
  dword		GetNumItems (void)		{ return (m_BaseItems.GetSize()); }
  obri_item_t*  GetItem     (const dword Index)	{ return (m_BaseItems.GetAt(Index)); }

	/* Input items from the given filename */
  bool ReadItemFile (const SSCHAR* pFilename);

 };
/*===========================================================================
 *		End of Class CObriBaseItems Definition
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Function Prototypes
 *
 *=========================================================================*/

	/* Initialize an item structure */
  void DefaultObriBaseItem (obri_item_t& Item);
  void DefaultObriArmor    (obri_item_t& Item);
  void DefaultObriClothing (obri_item_t& Item);
  void DefaultObriLight    (obri_item_t& Item);
  void DefaultObriMisc     (obri_item_t& Item);
  void DefaultObriScroll   (obri_item_t& Item);
  void DefaultObriSigil    (obri_item_t& Item);
  void DefaultObriWeapon   (obri_item_t& Item);
  void DefaultObriStaff    (obri_item_t& Item);

/*===========================================================================
 *		End of Function Prototypes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin External Variable Definitions
 *
 *=========================================================================*/

/*===========================================================================
 *		End of External Variable Definitions
 *=========================================================================*/


#endif
/*===========================================================================
 *		End of File Obri_BaseItems.H
 *=========================================================================*/


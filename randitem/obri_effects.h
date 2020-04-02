/*===========================================================================
 *
 * File:	Obri_Effects.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	21 April 2006
 *
 * Defines the CObriEffects class for handling an array of prefix and 
 * suffix definitions.
 *
 *=========================================================================*/
#ifndef __OBRIEFFECTS_H
#define __OBRIEFFECTS_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "obri_baseitems.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Number of enchantment effects allowed per prefix/suffix */
  #define OBRI_MAX_ENCHEFFECTS 10

	/* Number of name masks allowed per effect */
  #define OBRI_MAX_EFFECT_NAMEMASKS 8

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* A single enchantment effect */
  typedef struct { 
  	const obrectype_t*	pEffectID;				/* The enchantment effect */
	short			ActorValue;				/* Actor value index for effects that need it */
	byte			RangeType;				/* Type of range: self, touch target */
	long			MinArea;				/* Min/max area */
	long			MaxArea;
	long			MinDuration;				/* Min/max durations */
	long			MaxDuration;
	long			MinMagnitude;				/* Min/max magnitudes */
	long			MaxMagnitude;

	SSCHAR			Script[OBRI_MAX_NAMESIZE+1];		/* Script ID */
	SSCHAR			EffectName[OBRI_MAX_NAMESIZE+1];	/* Custom effect name */
	int			School;					/* Spell school the effect uses */
	const obrectype_t*	pVisualEffectID;			/* Magic effect ID for visuals */
	bool			IsHostile;
   } obri_encheff_t;

	/* Effect enchantment data */
  typedef struct {
	int		 EnchantType;			/* Scroll, Apparel, Weapon, Staff */
	long		 MinCost;			/* Enchant cost range if not autocalc */
	long		 MaxCost;		
	long		 MinCharge;			/* Charge range if not autocalc */
	long		 MaxCharge;
	bool		 AutoCalc;
	obri_encheff_t   Effects[OBRI_MAX_ENCHEFFECTS];	/* Enchantment effects */
	dword		 NumEffects;
   } obri_enchant_t;

	/* Stores a single prefix/suffic effect */
  typedef struct {
  	int		 EffectType;			/* General effect type */
	SSCHAR		 Name[OBRI_MAX_NAMESIZE+1];	/* Prefix/suffix unique name string */
	SSCHAR		 NameID[OBRI_MAX_NAMESIZE+1];	/* Uniquely identifies the effect */
	SSCHAR		 Script[OBRI_MAX_NAMESIZE+1];	/* Script editor ID */
	long		 ItemLevel;			/* Level of effect (0-100) */
	bool		 Cursed;			/* Is the effect considered cursed? */
	long		 ItemMask;			/* Bitmask for which item types the effect is valid for */
	long		 MinQuality;			/* Quality range (-100,000 to 100,000) */
	long		 MaxQuality;
	long		 Multiplier;			/* General item value (-10 to 30) */
	float		 MinEffect;			/* Effect parameter ranges */
	float		 MaxEffect;

	CObriStringArray NameMasks;			/* Used to apply effects to items with matching names */
	CObriStringArray CustomTypes;			/* Used to apply effects to items with matching types */

	obri_enchant_t	 Enchantment;			/* Effect enchantment if valid */
   } obri_effect_t;

	/* Pointer array of effects */
  typedef CObPtrArray<obri_effect_t>    CObriEffectArray;
  typedef CObRefPtrArray<obri_effect_t> CObriRefEffectArray;

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriEffects Definition
 *
 * Holds an array of prefix and suffix effects used for randomly generating
 * Morrowind items.
 *
 *=========================================================================*/
class CObriEffects {

  /*---------- Begin Private Class Members ----------------------*/
private:
  CObriEffectArray	m_Prefixes;	/* Array of effects */
  CObriEffectArray	m_Suffixes;

  dword			m_InputLine;	/* Current line in input file when parsing */


  /*---------- Begin Protected Class Methods --------------------*/
protected:

	/* Input an effect record from the given file */
  bool ReadEffect	 (CObFile& File, obri_effect_t* pNewEffect);
  bool ReadPrefix	 (CObFile& File);
  bool ReadSuffix	 (CObFile& File);
  bool ReadEnchant	 (CObFile& File, obri_effect_t* pNewEffect);
  bool ReadEnchantEffect (CObFile& File, obri_effect_t* pNewEffect);
  bool ParseNameMask     (obri_effect_t* pNewEffect, const SSCHAR* pString);
  bool ParseCustomTypes  (obri_effect_t* pNewEffect, const SSCHAR* pString);

	/* Set an effect parameter from string values */
  bool SetEffectParam	     (obri_effect_t* pNewEffect, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetEnchantParam	     (obri_effect_t* pNewEffect, const SSCHAR* pVariable, const SSCHAR* pValue);
  bool SetEnchantEffectParam (obri_effect_t* pNewEffect, const SSCHAR* pVariable, const SSCHAR* pValue);


  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Class Constructors/Destructors */
  CObriEffects();
  virtual ~CObriEffects() { Destroy(); }
  virtual void Destroy (void);
	
	/* Find an effect by name */
  obri_effect_t* FindEffect (const SSCHAR* pName);

  	/* Get class members */
  dword		   GetNumPrefixes (void)		{ return (m_Prefixes.GetSize()); }
  obri_effect_t*   GetPrefix      (const int Index)	{ return (m_Prefixes.GetAt(Index)); }
  dword		   GetNumSuffixes (void)		{ return (m_Suffixes.GetSize()); }
  obri_effect_t*   GetSuffix      (const int Index)	{ return (m_Suffixes.GetAt(Index)); }

	/* Input effect data file */
  bool ReadEffectFile (const SSCHAR* pFilename);

 };
/*===========================================================================
 *		End of Class CObriEffects Definition
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Function Prototypes
 *
 *=========================================================================*/

	/* Initialize an effect or enchantments */
  void DefaultObriEffect	(obri_effect_t& Effect);
  void DefaultObriEnchant	(obri_enchant_t& Enchant);
  void DefaultObriEnchEffect	(obri_encheff_t& EnchEffect);

	/* String conversion methods */
  int   StringToObriEffectType (const SSCHAR* pString);
  bool  StringToObriEffectType (int& OutType, const SSCHAR* pString);

/*===========================================================================
 *		End of Function Prototypes
 *=========================================================================*/


#endif
/*===========================================================================
 *		End of File Obri_Effects.H
 *=========================================================================*/


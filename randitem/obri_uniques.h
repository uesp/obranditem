/*===========================================================================
 *
 * File:	Obri_Uniques.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	21 April 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBRI_UNIQUES_H
#define __OBRI_UNIQUES_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "obri_effects.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Number of effects allowed per unique */
  #define OBRI_MAX_UNIQUEEFFECTS 6

	/* Type of name generations */
  #define OBRI_UNIQUE_NAMETYPE_EXACT  0
  #define OBRI_UNIQUE_NAMETYPE_SUFFIX 1
  #define OBRI_UNIQUE_NAMETYPE_PREFIX 2 

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* The unique item data */
  typedef struct {
	char		Name[OBRI_MAX_NAMESIZE+1];	/* Unique item name/suffix/prefix */
	int		NameType;			/* Type of name generation */
	long		ItemMask;			/* Bitmask indicating which items are allowed */
	int		ItemLevel;			/* Basic level of unique item (higher levels will be rarer) */
	char		Model[OBRI_MAX_NAMESIZE+1];	/* Optional unique model/picture for the item */
	char		Icon[OBRI_MAX_NAMESIZE+1];

	obri_effect_t*	pEffects[OBRI_MAX_UNIQUEEFFECTS];	/* Effect data */
	dword		NumEffects;
   } obri_unique_t;

	/* Array of unique pointers */
  typedef CObPtrArray<obri_unique_t>    CObriUniqueArray;
  typedef CObRefPtrArray<obri_unique_t> CObriRefUniqueArray;

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriUniques Definition
 *
 * Description
 *
 *=========================================================================*/
class CObriUniques {

  /*---------- Begin Private Class Members ----------------------*/
private:
  CObriUniqueArray	m_Uniques;	/* Array of unique records */
  CObriEffects*		m_pEffects;	/* Reference to effects object */
  dword			m_InputLine;


  /*---------- Begin Protected Class Methods --------------------*/
protected:

	/* Attempt to add an effect to the unique item */
  bool AddEffect (obri_unique_t* pNewUnique, const SSCHAR* pEffectName);

	/* Input unique record data from a text file */
  bool ReadUnique (CObFile& File, obri_unique_t* pNewUnique);
  bool ReadUnique (CObFile& File);

  	/* Set unique parameters from string values */
  bool SetUniqueParam (obri_unique_t* pNewUnique, const SSCHAR* pVariable, const SSCHAR* pValue);


  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Class Constructors/Destructors */
  CObriUniques();
  virtual ~CObriUniques() { Destroy(); }
  virtual void Destroy (void);

  	/* Get class members */
  dword		 GetNumUniques (void)		 { return (m_Uniques.GetSize()); }
  obri_unique_t* GetUnique     (const int Index) { return (m_Uniques.GetAt(Index)); }

	/* Read unique data file */
  bool ReadUniqueFile (const SSCHAR* pFilename);

	/* Set class methods */
  void SetEffects (CObriEffects* pEffects) { m_pEffects = pEffects; }

 };
/*===========================================================================
 *		End of Class CObriUniques Definition
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Function Prototypes
 *
 *=========================================================================*/

	/* Initialize an unique structure */
  void DefaultObriUnique (obri_unique_t& UniqueItem);

/*===========================================================================
 *		End of Function Prototypes
 *=========================================================================*/



#endif
/*===========================================================================
 *		End of File Obri_Uniques.H
 *=========================================================================*/

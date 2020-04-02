/*===========================================================================
 *
 * File:	Obri_enchantmap.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 26, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBRI_ENCHANTMAP_H
#define __OBRI_ENCHANTMAP_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "modfile/records/obrecordmap.h"
  #include "obri_effects.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* Holds a key to a specific enchant map record */
  struct obri_enchantkey_t {
	CObriEffects* pEffect;
	float	      Ratio;
	int	      Level;
   };

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriEnchantMap Definition
 *
 * Holds a simple hash map of random item enchantments used to ensure no
 * duplicate enchantments are created.
 *
 *=========================================================================*/
class CObriEnchantMap : public CObBaseRecordMap<obri_enchantkey_t, CObEnchRecord, obri_enchantkey_t> {

  /*---------- Begin Protected Class Members --------------------*/
protected:


  /*---------- Begin Protected Class Methods --------------------*/
protected:

	/* Compare two string keys */
  bool CompareKeys (obri_enchantkey_t Key1, obri_enchantkey_t Key2);


  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Compute a hash from a string key */
  dword HashKey (obri_enchantkey_t Key);

 };
/*===========================================================================
 *		End of Class CObriEnchantMap Definition
 *=========================================================================*/


#endif
/*===========================================================================
 *		End of File Obri_enchantmap.H
 *=========================================================================*/

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
  #include "common/obsimplemap.h"
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
	obri_effect_t* pEffect;
	CObEnchRecord* pEnchant;
	float	       Ratio;
	int	       Level;

	operator dword (void) const { return (dword) (pEffect); }
   };

  bool operator == (const obri_enchantkey_t& Key1, const obri_enchantkey_t& Key2);

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
class CObriEnchantMap : public CObSimpleMap<obri_enchantkey_t> {

  /*---------- Begin Protected Class Members --------------------*/
protected:


  /*---------- Begin Protected Class Methods --------------------*/
protected:

	


  /*---------- Begin Public Class Methods -----------------------*/
public:

  void AddEnchantment (obri_effect_t* pEffect, const float Ratio, CObEnchRecord* pEnchant);

  CObEnchRecord* FindEnchantment (obri_effect_t* pEffect, const float Ratio);

 };
/*===========================================================================
 *		End of Class CObriEnchantMap Definition
 *=========================================================================*/


#endif
/*===========================================================================
 *		End of File Obri_enchantmap.H
 *=========================================================================*/

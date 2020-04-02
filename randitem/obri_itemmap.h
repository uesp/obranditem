/*===========================================================================
 *
 * File:	Obri_Itemmap.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 26, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBRI_ITEMMAP_H
#define __OBRI_ITEMMAP_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "common/obsimplemap.h"
  #include "obri_baseitems.h"
  #include "obri_effects.h"
  #include "obri_uniques.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* Holds a key to a specific item map record */
  struct obri_itemkey_t {
	obri_item_t*	pItem;
	obri_effect_t*	pPrefix1;
	obri_effect_t*	pPrefix2;
	obri_effect_t*	pSuffix;
	obri_unique_t*	pUnique;
	CObIdRecord*	pRecord;
	float		Ratio;
	int		Level;

	operator dword (void) const { return (dword) (pItem) + (dword) (pPrefix1) + (dword) (pPrefix2) + (dword) (pSuffix) + (dword) (pUnique); }
   };

  bool operator == (const obri_itemkey_t& Key1, const obri_itemkey_t& Key2);

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObriItemMap Definition
 *
 * Holds a simple hash map of random items used to ensure no
 * duplicate items are created.
 *
 *=========================================================================*/
class CObriItemMap : public CObSimpleMap<obri_itemkey_t> {

  /*---------- Begin Protected Class Members --------------------*/
protected:


  /*---------- Begin Protected Class Methods --------------------*/
protected:


  /*---------- Begin Public Class Methods -----------------------*/
public:

  void AddItem (obri_item_t* pItem, obri_effect_t* pPrefix1,  obri_effect_t* pPrefix2, obri_effect_t* pSuffix, obri_unique_t* pUnique, const float Ratio, const int Level,  CObRecord* pRecord);

  CObIdRecord* FindItem (obri_item_t* pItem, obri_effect_t* pPrefix1,  obri_effect_t* pPrefix2, obri_effect_t* pSuffix, obri_unique_t* pUnique, const float Ratio, const int Level, CObRecord* pRecord);

  obri_itemkey_t* FindItem (CObIdRecord* pIdRecord);

  	/* Hash a key value */
  virtual dword HashKey (const obri_itemkey_t Key);

 };
/*===========================================================================
 *		End of Class CObriItemMap Definition
 *=========================================================================*/


#endif
/*===========================================================================
 *		End of File Obri_ItemMap.H
 *=========================================================================*/

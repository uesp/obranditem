/*===========================================================================
 *
 * File:	Obri_enchantmap.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 26, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_enchantmap.h"
#include "math.h"



/*===========================================================================
 *
 * Function - bool operator == (Key1, Key2);
 *
 * Compares two enchant map key values.
 *
 *=========================================================================*/
bool operator == (const obri_enchantkey_t& Key1, const obri_enchantkey_t& Key2) {
  obri_encheff_t* pEffect;
  dword		  Index;
  float		  Value1;
  float		  Value2;

	/* Non-enchantments should not be called */
  if (Key1.pEffect->EffectType != OBRI_EFFTYPE_ENCHANT || Key2.pEffect->EffectType != OBRI_EFFTYPE_ENCHANT) return (false);

	/* Ignore enchantments that are not the same effect object */
  if (Key1.pEffect != Key2.pEffect) return (false);

	/* Basic test for close ratio values */
  if (fabs(Key1.Ratio - Key2.Ratio) < 0.01) return (true);
  //return (false);

	/* Test cost */
  Value1 = SplitValue((float)Key1.pEffect->Enchantment.MinCost, (float)Key1.pEffect->Enchantment.MaxCost, Key1.Ratio);
  Value2 = SplitValue((float)Key1.pEffect->Enchantment.MinCost, (float)Key1.pEffect->Enchantment.MaxCost, Key2.Ratio);
  if (fabs(Value2 - Value1) > 1) return (false);

  	/* Test charge */
  Value1 = SplitValue((float)Key1.pEffect->Enchantment.MinCharge, (float)Key1.pEffect->Enchantment.MaxCharge, Key1.Ratio);
  Value2 = SplitValue((float)Key1.pEffect->Enchantment.MinCharge, (float)Key1.pEffect->Enchantment.MaxCharge, Key2.Ratio);
  if (fabs(Value2 - Value1) > 1) return (false);

	/* Extended effect value test */
  for (Index = 0; Index < Key1.pEffect->Enchantment.NumEffects; ++Index) {
    pEffect = &Key1.pEffect->Enchantment.Effects[Index];
    
		/* Test duration */
    Value1 = SplitValue((float)pEffect->MinDuration, (float)pEffect->MaxDuration, Key1.Ratio);
    Value2 = SplitValue((float)pEffect->MinDuration, (float)pEffect->MaxDuration, Key2.Ratio);
    if (fabs(Value2 - Value1) > 1) return (false);

    		/* Test magnitude */
    Value1 = SplitValue((float)pEffect->MinMagnitude, (float)pEffect->MaxMagnitude, Key1.Ratio);
    Value2 = SplitValue((float)pEffect->MinMagnitude, (float)pEffect->MaxMagnitude, Key2.Ratio);
    if (fabs(Value2 - Value1) > 1) return (false);

    		/* Test area */
    Value1 = SplitValue((float)pEffect->MinArea, (float)pEffect->MaxArea, Key1.Ratio);
    Value2 = SplitValue((float)pEffect->MinArea, (float)pEffect->MaxArea, Key2.Ratio);
    if (fabs(Value2 - Value1) > 1) return (false);
   }

  return (true);
 }
/*===========================================================================
 *		End of Function operator ==()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEnchantMap Method - void AddEnchantment (pEffects, Ratio, pEnchant);
 *
 *=========================================================================*/
void CObriEnchantMap::AddEnchantment (obri_effect_t* pEffect, const float Ratio, CObEnchRecord* pEnchant) {
  obri_enchantkey_t  TempKey;

  TempKey.Level    = 0;
  TempKey.pEnchant = pEnchant;
  TempKey.pEffect  = pEffect;
  TempKey.Ratio    = Ratio;

  SetAt(TempKey);
 }
/*===========================================================================
 *		End of Class Method CObriEnchantMap::AddEnchantment()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriEnchantMap Method - CObEnchRecord* FindEnchantment (pEffect, Ratio);
 *
 *=========================================================================*/
CObEnchRecord* CObriEnchantMap::FindEnchantment (obri_effect_t* pEffect, const float Ratio) { 
  obri_enchantkey_t  TempKey;
  obri_enchantkey_t* pKey;
  bool		     Result;

  TempKey.Level    = 0;
  TempKey.pEnchant = NULL;
  TempKey.pEffect  = pEffect;
  TempKey.Ratio    = Ratio;

  Result = Lookup(TempKey, pKey);
  if (!Result) return (NULL);
  return (pKey->pEnchant);
 }
/*===========================================================================
 *		End of Class Method CObriEnchantMap::FindEnchantment()
 *=========================================================================*/
  
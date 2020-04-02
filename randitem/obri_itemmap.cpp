/*===========================================================================
 *
 * File:	Obri_Itemmap.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	May 26, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_itemmap.h"
#include "math.h"


/*===========================================================================
 *
 * Function - bool CompareEffects (pEnchant1, pEffect1, pEnchant2, pEffect2);
 *
 *=========================================================================*/
bool CompareEffects (CObEffectItemRecord* pEnchant1, CObEfidSubrecord* pEffect1, 
	             CObEffectItemRecord* pEnchant2, CObEfidSubrecord* pEffect2) {
  CObEfitSubrecord* pEfit1;
  CObEfitSubrecord* pEfit2;
  CObScitSubrecord* pScit1;
  CObScitSubrecord* pScit2;
  
	/* Ensure the effect types are the same */
  if (pEffect1->GetDataName() != pEffect2->GetDataName()) return (false);

	/* Get the effect data */
  pEfit1 = pEnchant1->GetEffectData(pEffect1);
  pEfit2 = pEnchant2->GetEffectData(pEffect2);
  if (pEfit1 == NULL || pEfit2 == NULL) return (false);

	/* Compare the effect data */
  if (pEfit1->GetArea()       != pEfit2->GetArea())       return (false);
  if (pEfit1->GetDuration()   != pEfit2->GetDuration())   return (false);
  if (pEfit1->GetMagnitude()  != pEfit2->GetMagnitude())  return (false);
  if (pEfit1->GetEffectType() != pEfit2->GetEffectType()) return (false);
  if (pEfit1->GetActorValue() != pEfit2->GetActorValue()) return (false);

	/* Compare script effects */
  if (pEffect1->GetDataName() == OB_MGEF_SEFF) {
    pScit1 = pEnchant1->GetScriptEffectData(pEffect1);
    pScit2 = pEnchant2->GetScriptEffectData(pEffect2);
    if (pScit1 == NULL || pScit2 == NULL) return (false);

    if (pScit1->IsHostile()       != pScit2->IsHostile())       return (false);
    if (pScit1->GetSchool()       != pScit2->GetSchool())       return (false);
    if (pScit1->GetScriptFormID() != pScit2->GetScriptFormID()) return (false);
    if (pScit1->GetVisualEffect() != pScit2->GetVisualEffect()) return (false);
   }

  return (true);
 }
/*===========================================================================
 *		End of Function CompareEffects()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareEffectItems (pEnchant1, pEnchant2);
 *
 *=========================================================================*/
bool CompareEffectItems (CObEffectItemRecord* pEnchant1, CObEffectItemRecord* pEnchant2) {
  CObEfidSubrecord* pEffect1;
  CObEfidSubrecord* pEffect2;
  bool              Result;
  int		    Pos1;
  int		    Pos2;

	/* Check effects */
  pEffect1 = pEnchant1->GetFirstEffect(Pos1);
  pEffect2 = pEnchant2->GetFirstEffect(Pos2);

  while (pEffect1 != NULL && pEffect2 != NULL) {
    Result = CompareEffects (pEnchant1, pEffect1, pEnchant2, pEffect2);
    if (!Result) return (false);

    pEffect1 = pEnchant1->GetNextEffect(Pos1);
    pEffect2 = pEnchant2->GetNextEffect(Pos2);
   }
    
	/* Ensure the enchantments have the same number of effects */
  if (pEffect1 == NULL && pEffect1 != NULL) return (false);
  if (pEffect1 != NULL && pEffect1 == NULL) return (false);
  return (true);
 }
/*===========================================================================
 *		End of Function CompareEffectItems()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareEnchantments (pEnchant1, pEnchant2);
 *
 *=========================================================================*/
bool CompareEnchantments (CObEnchRecord* pEnchant1, CObEnchRecord* pEnchant2) {

	/* Ignore invalid input */
  if (pEnchant1 == NULL || pEnchant2 == NULL) return (false);

	/* Check base enchantment properties */
  if (pEnchant1->IsAutoCalc() != pEnchant2->IsAutoCalc()) return (false);
  if (pEnchant1->GetCharge()  != pEnchant2->GetCharge())  return (false);
  if (pEnchant1->GetCost()    != pEnchant2->GetCost())    return (false);
  if (pEnchant1->GetType()    != pEnchant2->GetType())    return (false);

	/* Check effects */
  return CompareEffectItems(pEnchant1, pEnchant2);
 }
/*===========================================================================
 *		End of Function CompareEnchantments()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareArmors (pArmor1, pArmor2);
 *
 *=========================================================================*/
bool CompareArmors (CObArmoRecord* pArmor1, CObArmoRecord* pArmor2) {

  if (pArmor1 == NULL || pArmor2 == NULL) return (false);

  if (pArmor1->GetArmor()	   != pArmor2->GetArmor())	   return (false);
  if (pArmor1->GetEnchantment()    != pArmor2->GetEnchantment())   return (false);
  if (pArmor1->GetBipedFlags()	   != pArmor2->GetBipedFlags())    return (false);
  if (pArmor1->GetHealth()	   != pArmor2->GetHealth())        return (false);
  if (pArmor1->GetScript()	   != pArmor2->GetScript())        return (false);
  if (pArmor1->GetValue()	   != pArmor2->GetValue())         return (false);
  if (pArmor1->IsHeavyArmor()	   != pArmor2->IsHeavyArmor())     return (false);

  if (fabs(pArmor1->GetWeight() - pArmor2->GetWeight()) > 0.01) return (false);

  if (stricmp(pArmor1->GetModel(), pArmor2->GetModel()) != 0) return (false);

  return (true);
 }
/*===========================================================================
 *		End of Function CompareArmors()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareBooks (pBook1, pBook2);
 *
 *=========================================================================*/
bool CompareBooks (CObBookRecord* pBook1, CObBookRecord* pBook2) {

  if (pBook1 == NULL || pBook2 == NULL) return (false);

  if (pBook1->GetEnchantment()   != pBook2->GetEnchantment())   return (false);
  if (pBook1->GetTeaches()       != pBook2->GetTeaches())       return (false);
  if (pBook1->GetScript()	 != pBook2->GetScript())        return (false);
  if (pBook1->GetValue()	 != pBook2->GetValue())         return (false);
  if (pBook1->IsScroll()	 != pBook2->IsScroll())         return (false);

  if (fabs(pBook1->GetWeight() - pBook2->GetWeight()) > 0.01) return (false);

  if (stricmp(pBook1->GetModel(), pBook2->GetModel()) != 0) return (false);

  return (true);
 }
/*===========================================================================
 *		End of Function CompareBooks()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareClothing (pClothing1, pClothing2);
 *
 *=========================================================================*/
bool CompareClothing (CObClotRecord* pClothing1, CObClotRecord* pClothing2) {

  if (pClothing1 == NULL || pClothing2 == NULL) return (false);

  if (pClothing1->GetEnchantment()   != pClothing2->GetEnchantment())   return (false);
  if (pClothing1->GetBipedFlags()    != pClothing2->GetBipedFlags())    return (false);
  if (pClothing1->GetScript()	     != pClothing2->GetScript())        return (false);
  if (pClothing1->GetValue()	     != pClothing2->GetValue())         return (false);

  if (fabs(pClothing1->GetWeight() - pClothing2->GetWeight()) > 0.01) return (false);

  if (stricmp(pClothing1->GetModel(), pClothing2->GetModel()) != 0) return (false);

  return (true);
 }
/*===========================================================================
 *		End of Function CompareClothing()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareLights (pLight1, pLight2);
 *
 *=========================================================================*/
bool CompareLights (CObLighRecord* pLight1, CObLighRecord* pLight2) {

  if (pLight1 == NULL || pLight2 == NULL) return (false);

  if (pLight1->GetScript()    != pLight2->GetScript())    return (false);
  if (pLight1->GetValue()     != pLight2->GetValue())     return (false);
  if (pLight1->GetColor()     != pLight2->GetColor())     return (false);
  if (pLight1->GetRadius()    != pLight2->GetRadius())    return (false);
  if (pLight1->GetSound()     != pLight2->GetSound())     return (false);
  if (pLight1->GetTime()      != pLight2->GetTime())      return (false);
  if (pLight1->IsCarried()    != pLight2->IsCarried())    return (false);
  if (pLight1->IsDefaultOff() != pLight2->IsDefaultOff()) return (false);
  if (pLight1->IsDynamic()    != pLight2->IsDynamic())    return (false);
  if (pLight1->GetLightType() != pLight2->GetLightType()) return (false);

  if (fabs(pLight1->GetWeight()  - pLight2->GetWeight())  > 0.01) return (false);
  if (fabs(pLight1->GetFade()    - pLight2->GetFade())    > 0.01) return (false);
  if (fabs(pLight1->GetFalloff() - pLight2->GetFalloff()) > 0.01) return (false);
  if (fabs(pLight1->GetFOV()     - pLight2->GetFOV())     > 0.01) return (false);

  if (stricmp(pLight1->GetModel(), pLight2->GetModel()) != 0) return (false);

  return (true);
 }
/*===========================================================================
 *		End of Function CompareLights()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareMisc (pMisc1, pMisc2);
 *
 *=========================================================================*/
bool CompareMisc (CObMiscRecord* pMisc1, CObMiscRecord* pMisc2) {

  if (pMisc1 == NULL || pMisc2 == NULL) return (false);

  if (pMisc1->GetScript() != pMisc2->GetScript()) return (false);
  if (pMisc1->GetValue()  != pMisc2->GetValue())  return (false);

  if (fabs(pMisc1->GetWeight() - pMisc2->GetWeight()) > 0.01) return (false);

  if (stricmp(pMisc1->GetModel(), pMisc2->GetModel()) != 0) return (false);

  return (true);
 }
/*===========================================================================
 *		End of Function CompareMisc()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareSigils (pSigil1, pSigil2);
 *
 *=========================================================================*/
bool CompareSigils (CObSgstRecord* pSigil1, CObSgstRecord* pSigil2) {

  if (pSigil1 == NULL || pSigil2 == NULL) return (false);

  if (pSigil1->GetUses()    != pSigil2->GetUses())   return (false);
  if (pSigil1->GetScript()  != pSigil2->GetScript()) return (false);
  if (pSigil1->GetValue()   != pSigil2->GetValue())  return (false);

  if (fabs(pSigil1->GetWeight() - pSigil2->GetWeight()) > 0.01) return (false);

  if (stricmp(pSigil1->GetModel(), pSigil2->GetModel()) != 0) return (false);

  return CompareEffectItems(pSigil1, pSigil2);
 }
/*===========================================================================
 *		End of Function CompareSigils()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareWeapons (pWeapon1, pWeapon2);
 *
 *=========================================================================*/
bool CompareWeapons (CObWeapRecord* pWeapon1, CObWeapRecord* pWeapon2) {

  if (pWeapon1 == NULL || pWeapon2 == NULL) return (false);

  if (pWeapon1->GetDamage()	   != pWeapon2->GetDamage())	    return (false);
  if (pWeapon1->GetEnchantment()   != pWeapon2->GetEnchantment())   return (false);
  if (pWeapon1->GetEnchantPoints() != pWeapon2->GetEnchantPoints()) return (false);
  if (pWeapon1->GetHealth()	   != pWeapon2->GetHealth())        return (false);
  if (pWeapon1->GetScript()	   != pWeapon2->GetScript())        return (false);
  if (pWeapon1->GetType()	   != pWeapon2->GetType())          return (false);
  if (pWeapon1->GetValue()	   != pWeapon2->GetValue())         return (false);
  if (pWeapon1->IsIgnoreResist()   != pWeapon2->IsIgnoreResist())   return (false);

  if (fabs(pWeapon1->GetReach()  - pWeapon2->GetReach())  > 0.01) return (false);
  if (fabs(pWeapon1->GetSpeed()  - pWeapon2->GetSpeed())  > 0.01) return (false);
  if (fabs(pWeapon1->GetWeight() - pWeapon2->GetWeight()) > 0.01) return (false);

  if (stricmp(pWeapon1->GetModel(), pWeapon2->GetModel()) != 0) return (false);

  return (true);
 }
/*===========================================================================
 *		End of Function CompareWeapons()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool CompareRecords (pRecord1, pRecord2);
 *
 *=========================================================================*/
bool CompareRecords (CObIdRecord* pRecord1, CObIdRecord* pRecord2) {

  if (pRecord1->GetName() != pRecord2->GetName()) return (false);

  if (pRecord1->GetName() == OB_NAME_ARMO) {
    return CompareArmors(ObCastClass(CObArmoRecord, pRecord1), ObCastClass(CObArmoRecord, pRecord2));
   }
  else if (pRecord1->GetName() == OB_NAME_BOOK) {
    return CompareBooks(ObCastClass(CObBookRecord, pRecord1), ObCastClass(CObBookRecord, pRecord2));
   }
  else if (pRecord1->GetName() == OB_NAME_CLOT) {
    return CompareClothing(ObCastClass(CObClotRecord, pRecord1), ObCastClass(CObClotRecord, pRecord2));
   }
  else if (pRecord1->GetName() == OB_NAME_LIGH) {
    return CompareLights(ObCastClass(CObLighRecord, pRecord1), ObCastClass(CObLighRecord, pRecord2));
   }
  else if (pRecord1->GetName() == OB_NAME_MISC) {
    return CompareMisc(ObCastClass(CObMiscRecord, pRecord1), ObCastClass(CObMiscRecord, pRecord2));
   }
  else if (pRecord1->GetName() == OB_NAME_SGST) {
    return CompareSigils(ObCastClass(CObSgstRecord, pRecord1), ObCastClass(CObSgstRecord, pRecord2));
   }
  else if (pRecord1->GetName() == OB_NAME_WEAP) {
    return CompareWeapons(ObCastClass(CObWeapRecord, pRecord1), ObCastClass(CObWeapRecord, pRecord2));
   }

  return (false);
 }
/*===========================================================================
 *		End of Function CompareRecords()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool operator == (Key1, Key2);
 *
 * Compares two enchant map key values.
 *
 *=========================================================================*/
bool operator == (const obri_itemkey_t& Key1, const obri_itemkey_t& Key2) {
  float Effect1;
  float Effect2;

  if (Key1.pRecord != NULL && Key2.pRecord != NULL) return CompareRecords(Key1.pRecord, Key2.pRecord);

  if (Key1.pItem != Key2.pItem) return (false);

  if (!(Key1.pPrefix1 == Key2.pPrefix1 || Key1.pPrefix1 == Key2.pPrefix2)) return (false);
  if (!(Key1.pPrefix2 == Key2.pPrefix2 || Key1.pPrefix2 != Key2.pPrefix1)) return (false);
  if (Key1.pSuffix != Key2.pSuffix) return (false);
  if (Key1.pUnique != Key2.pUnique) return (false);

	/* Only need to check ratios for items with effects */
  if (Key1.pPrefix1 != NULL || Key1.pPrefix2 != NULL || Key1.pSuffix != NULL || Key1.pUnique != NULL) {
	
    if (Key1.pPrefix1 != NULL && Key2.pPrefix1 == Key1.pPrefix1 && Key1.pPrefix1->EffectType != OBRI_EFFTYPE_ENCHANT) { 
      Effect1 = SplitValue(Key1.pPrefix1->MinEffect, Key1.pPrefix1->MaxEffect, Key1.Ratio);
      Effect2 = SplitValue(Key2.pPrefix1->MinEffect, Key2.pPrefix1->MaxEffect, Key2.Ratio);
      if (fabs(Effect1 - Effect2) >= 1) return (false);
     }

    if (Key1.pPrefix1 != NULL && Key2.pPrefix2 == Key1.pPrefix1 && Key1.pPrefix1->EffectType != OBRI_EFFTYPE_ENCHANT) { 
      Effect1 = SplitValue(Key1.pPrefix1->MinEffect, Key1.pPrefix1->MaxEffect, Key1.Ratio);
      Effect2 = SplitValue(Key2.pPrefix2->MinEffect, Key2.pPrefix2->MaxEffect, Key2.Ratio);
      if (fabs(Effect1 - Effect2) >= 1) return (false);
     }

    if (Key1.pPrefix2 != NULL && Key2.pPrefix2 == Key1.pPrefix1 && Key1.pPrefix2->EffectType != OBRI_EFFTYPE_ENCHANT) { 
      Effect1 = SplitValue(Key1.pPrefix2->MinEffect, Key1.pPrefix2->MaxEffect, Key1.Ratio);
      Effect2 = SplitValue(Key2.pPrefix2->MinEffect, Key2.pPrefix2->MaxEffect, Key2.Ratio);
      if (fabs(Effect1 - Effect2) >= 1) return (false);
     }

    if (Key1.pPrefix2 != NULL && Key2.pPrefix1 == Key1.pPrefix1 && Key1.pPrefix2->EffectType != OBRI_EFFTYPE_ENCHANT) { 
      Effect1 = SplitValue(Key1.pPrefix2->MinEffect, Key1.pPrefix2->MaxEffect, Key1.Ratio);
      Effect2 = SplitValue(Key2.pPrefix1->MinEffect, Key2.pPrefix1->MaxEffect, Key2.Ratio);
      if (fabs(Effect1 - Effect2) >= 1) return (false);
     }

    if (Key1.pSuffix != NULL && Key2.pSuffix == Key1.pSuffix && Key1.pSuffix->EffectType != OBRI_EFFTYPE_ENCHANT) { 
      Effect1 = SplitValue(Key1.pSuffix->MinEffect, Key1.pSuffix->MaxEffect, Key1.Ratio);
      Effect2 = SplitValue(Key2.pSuffix->MinEffect, Key2.pSuffix->MaxEffect, Key2.Ratio);
      if (fabs(Effect1 - Effect2) >= 1) return (false);
     }

    if (fabs(Key1.Ratio - Key2.Ratio) >= 0.01) return (false);
   }
  
  return (true);
 }
/*===========================================================================
 *		End of Function operator ==()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemMap Method - void AddItem (pItem, Ratio, pEnchant);
 *
 *=========================================================================*/
void CObriItemMap::AddItem (obri_item_t* pItem, obri_effect_t* pPrefix1,  obri_effect_t* pPrefix2, obri_effect_t* pSuffix,
				obri_unique_t* pUnique, const float Ratio, const int Level, CObRecord* pRecord) {
  obri_itemkey_t  TempKey;

  TempKey.Level    = Level;
  TempKey.pRecord  = ObCastClass(CObIdRecord, pRecord);
  TempKey.pItem    = pItem;
  TempKey.pPrefix1 = pPrefix1;
  TempKey.pPrefix2 = pPrefix2;
  TempKey.pSuffix  = pSuffix;
  TempKey.pUnique  = pUnique;
  TempKey.Ratio    = Ratio;

  SetAt(TempKey);
 }
/*===========================================================================
 *		End of Class Method AddItem::AddEnchantment()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemMap Method - obri_itemkey_t* FindItem (pIdRecord);
 *
 *=========================================================================*/
obri_itemkey_t* CObriItemMap::FindItem (CObIdRecord* pIdRecord) {
  CObSimpleMapAssoc*	pAssoc;
  dword			Index;

  if (m_ppHashTable == NULL) return (NULL);

  for (Index = 0; Index < m_HashTableSize; ++Index) {

     for (pAssoc = m_ppHashTable[Index]; pAssoc != NULL; pAssoc = pAssoc->pNext) {
       if (pAssoc->Value.pRecord == pIdRecord) return (&pAssoc->Value);
      }
   }

  return (NULL);
 }
/*===========================================================================
 *		End of Class Method CObriItemMap::FindItem()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObriItemMap Method - CObIdRecord* FindItem (pItem, Ratio);
 *
 *=========================================================================*/
CObIdRecord* CObriItemMap::FindItem (obri_item_t* pItem, obri_effect_t* pPrefix1, obri_effect_t* pPrefix2, 
				obri_effect_t* pSuffix,	obri_unique_t* pUnique, const float Ratio,  
				const int Level, CObRecord* pRecord) { 
  obri_itemkey_t     TempKey;
  obri_itemkey_t*    pKey;
  bool		     Result;

  TempKey.Level    = Level;

  if (pRecord != NULL) {
    TempKey.pRecord  = ObCastClass(CObIdRecord, pRecord);
   }
  else {
    TempKey.pRecord = NULL;
   }

  TempKey.pItem    = pItem;
  TempKey.pPrefix1 = pPrefix1;
  TempKey.pPrefix2 = pPrefix2;
  TempKey.pSuffix  = pSuffix;
  TempKey.pUnique  = pUnique;
  TempKey.Ratio    = Ratio;

  Result = Lookup(TempKey, pKey);
  if (!Result) return (NULL);
  return (pKey->pRecord);
 }
/*===========================================================================
 *		End of Class Method CObriItemMap::FindItem()
 *=========================================================================*/
  

/*===========================================================================
 *
 * Class CObriItemMap Method - dword HashKey (Key);
 *
 *=========================================================================*/
dword CObriItemMap::HashKey (const obri_itemkey_t Key) {
  int Value = 0;
 
  if (Key.pRecord != NULL) {

    if (Key.pRecord->GetName() == OB_NAME_ARMO) {
      CObArmoRecord* pArmor = ObCastClass(CObArmoRecord, Key.pRecord);
      if (pArmor != NULL) Value = pArmor->GetValue();
     }
    else if (Key.pRecord->GetName() == OB_NAME_BOOK) {
      CObBookRecord* pBook = ObCastClass(CObBookRecord, Key.pRecord);
      if (pBook != NULL) Value = pBook->GetValue();
     }
    else if (Key.pRecord->GetName() == OB_NAME_CLOT) {
      CObClotRecord* pClothing = ObCastClass(CObClotRecord, Key.pRecord);
      if (pClothing != NULL) Value = pClothing->GetValue();
     }
    else if (Key.pRecord->GetName() == OB_NAME_LIGH) {
      CObLighRecord* pLight = ObCastClass(CObLighRecord, Key.pRecord);
      if (pLight != NULL) Value = pLight->GetValue();
     }
    else if (Key.pRecord->GetName() == OB_NAME_MISC) {
      CObMiscRecord* pMisc = ObCastClass(CObMiscRecord, Key.pRecord);
      if (pMisc != NULL) Value = pMisc->GetValue();
     }
    else if (Key.pRecord->GetName() == OB_NAME_SGST) {
      CObSgstRecord* pSigil = ObCastClass(CObSgstRecord, Key.pRecord);
      if (pSigil != NULL) Value = pSigil->GetValue();
     }
    else if (Key.pRecord->GetName() == OB_NAME_WEAP) {
      CObWeapRecord* pWeapon = ObCastClass(CObWeapRecord, Key.pRecord);
      if (pWeapon != NULL) Value = pWeapon->GetValue();
     }

    return (Value*100 + Key.pRecord->GetUserData() + (Key.pRecord->GetName().Value >> 8));
   }
  
  return (dword) Key;
 }
/*===========================================================================
 *		End of Class Method CObriItemMap::HashKey()
 *=========================================================================*/

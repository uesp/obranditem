/*===========================================================================
 *
 * File:	obri_defs.cpp
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 20, 2006
 *
 * Common definitions for ObRandItem
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_defs.h"
#include "windows.h"
#include "time.h"
#include "math.h"


/*===========================================================================
 *
 * Begin Local Definitions
 *
 *=========================================================================*/

	/* Used to generate a random item ID string */
  long 	l_ItemIDCount      = 1;
  DWORD l_ItemSerialNumber = 0x12345678;
  bool  l_HasSerialNumber  = false;

  dword l_NextFormID1 = OBRI_FIRST_FORMID;
  dword l_NextFormID2 = OBRI_FIRST_FORMID;

/*===========================================================================
 *		End of Local Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Value-Name Arrays
 *
 *=========================================================================*/
BEGIN_STRINGVALUE(l_ObriIDTypes)
	ADD_STRINGVALUE( OBRI_IDTYPE_LONG,	"Long")
	ADD_STRINGVALUE( OBRI_IDTYPE_SHORT,	"Short")
	ADD_STRINGVALUE( OBRI_IDTYPE_SHORT4,	"Short4")
	ADD_STRINGVALUE( OBRI_IDTYPE_SHORT6,	"Short6")
	ADD_STRINGVALUE( OBRI_IDTYPE_SHORT8,	"Short8")
	ADD_STRINGVALUE( OBRI_IDTYPE_EXT,	"Extended")
	ADD_STRINGVALUE(  -1,			NULL)
END_STRINGVALUE()
/*===========================================================================
 *		End of Value-Name Arrays
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Value Name Arrays
 *
 *=========================================================================*/
BEGIN_STRINGVALUE(l_ObriItemTypes)
	ADD_STRINGVALUE( OBRI_ITEMTYPE_ARMOR,		"Armor")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_CLOTHING,	"Clothing")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_APPAREL,		"Apparel")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_MISC,		"Misc")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_SCROLL,		"Scroll")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_WEAPON,		"Weapon")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_STAFF,		"Staff")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_SIGIL,		"Sigil")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_SIGIL,		"Sigil Stone")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_LIGHT,		"Light")
	ADD_STRINGVALUE( OBRI_ITEMTYPE_ALL,		"All" )
END_STRINGVALUE()
/*===========================================================================
 *		End of Value Name Arrays
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Effect Mask Value-String Arrays
 *
 *=========================================================================*/
BEGIN_STRINGVALUE(l_ObriEffectTypes)
	ADD_STRINGVALUE( OBRI_EFFTYPE_NONE,		"None")
	ADD_STRINGVALUE( OBRI_EFFTYPE_ENCHANT,		"Enchant")
	ADD_STRINGVALUE( OBRI_EFFTYPE_WEIGHT,		"Weight")
	ADD_STRINGVALUE( OBRI_EFFTYPE_DAMAGE,		"Damage")
	ADD_STRINGVALUE( OBRI_EFFTYPE_REACH,		"Reach")
	ADD_STRINGVALUE( OBRI_EFFTYPE_SPEED,		"Speed")
	ADD_STRINGVALUE( OBRI_EFFTYPE_HEALTH,		"Health")
	ADD_STRINGVALUE( OBRI_EFFTYPE_HEALTH,		"Condition")
	ADD_STRINGVALUE( OBRI_EFFTYPE_ARMOR,		"Armor")
	ADD_STRINGVALUE( OBRI_EFFTYPE_ARMOR,		"AC")
	ADD_STRINGVALUE( OBRI_EFFTYPE_ENCHANTPTS,	"EnchantCharge")
	ADD_STRINGVALUE( OBRI_EFFTYPE_ENCHANTPTS,	"EnchantPts")
	ADD_STRINGVALUE( OBRI_EFFTYPE_ENCHANTPTS,	"EnchantPoints")
	ADD_STRINGVALUE( OBRI_EFFTYPE_SCRIPT,		"Script")
	ADD_STRINGVALUE( OBRI_EFFTYPE_SCRIPT,		"Lore")
	ADD_STRINGVALUE( OBRI_EFFTYPE_VALUE,		"Value")
	ADD_STRINGVALUE( OBRI_EFFTYPE_ALL,		"All")
END_STRINGVALUE()
/*===========================================================================
 *		End of Value String Arrays
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Enchantment Mask Value-String Arrays
 *
 *=========================================================================*/
BEGIN_STRINGVALUE(l_ObriEnchantTypes)
	ADD_STRINGVALUE( OBRI_ENCTYPE_STRIKES,	"Cast Strikes")
	ADD_STRINGVALUE( OBRI_ENCTYPE_USED,	"Cast When Used")
	ADD_STRINGVALUE( OBRI_ENCTYPE_ONCE,	"Cast Once")
	ADD_STRINGVALUE( OBRI_ENCTYPE_TARGET,	"Cast at Target")
	ADD_STRINGVALUE( OBRI_ENCTYPE_CONSTANT,	"Constant Effect")
	ADD_STRINGVALUE( OBRI_ENCTYPE_ALL,	"All")
 END_STRINGVALUE()
/*===========================================================================
 *		End of Enchantment Mask Value-String Arrays
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void l_GetItemSerial (void);
 *
 * Makes the item serial number for the current session. Attempts to use
 * the user's hard drive serial number if available, otherwise the current
 * clock tick count.
 *
 *=========================================================================*/
void l_GetItemSerial (void) {
  BOOL Result;

	/* Attempt to get the drive c: serial number */
  Result = GetVolumeInformation("c:\\", NULL, 0, &l_ItemSerialNumber, NULL, NULL, NULL, 0);

	/* Generate a 'random' serial number if the previous call failed */  
  if (!Result) l_ItemSerialNumber = (DWORD) clock();
  l_HasSerialNumber = true;
 }
/*===========================================================================
 *		End of Function l_GetItemSerial()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - float MakeRandItemNumber (FromValue, ToValue, BellValue);
 *
 * Returns a random number in the given range ('From' can be smaller or larger
 * than 'To'). How the number is generated depends on the input flags.
 *
 * Default
 *	Random number with equal probability along the entire range.
 *
 * BellValue = Midpoint of range.
 *
 *=========================================================================*/
float MakeRandItemGaussian (const float FromValue, const float ToValue, const float PercentMean) {
  float x1, x2, w, y1, y2;
  float Range = (float) fabs(ToValue - FromValue);
  float RangeMean = Range * PercentMean / 100.0f;
  int Attempts = 0;

MakeRandItemGaussian_START:

  do {
    x1 = 2.0f * MakeRandItemRatio() - 1.0f;
    x2 = 2.0f * MakeRandItemRatio() - 1.0f;
    w = x1 * x1 + x2 * x2;
  } while ( w >= 1.0f );

  w = (float) sqrt( (-2.0f * log( w ) ) / w );
  y1 = x1 * w;
  y2 = x2 * w;

  y1 = y1 / 2.0f * Range + RangeMean;
  y2 = y2 / 2.0f * Range + RangeMean;

  if (y1 >= FromValue && y1 <= ToValue) {
    return (y1);
   }
  else if (y2 >= FromValue && y2 <= ToValue) {
    return (y2);
   }
  else {
    ++Attempts;
    if (Attempts < 10) goto MakeRandItemGaussian_START;
   }

  if (y1 < FromValue) return (FromValue);
  if (y1 >   ToValue) return (ToValue);
  return (y1);
 }


float MakeRandItemNumber (const float FromValue, const float ToValue, const float BellValue) {
  float Result;
  int   HalfResult;
  float FromValue1 = FromValue;
  float ToValue1   = ToValue;

	/* Determine which 'half' to use */
  HalfResult = (int) ((float) rand()/(float) RAND_MAX * 2);
  if (HalfResult == 0) ToValue1 = BellValue;
  if (HalfResult == 1) FromValue1 = BellValue;

  if (FromValue1 == ToValue1)
    Result = FromValue1;
  else if (FromValue1 > ToValue1) {
    Result = ((float) rand() / (float) RAND_MAX) * (FromValue1 - ToValue1) + ToValue1;
    if (Result > FromValue) Result = FromValue;
    if (Result <   ToValue) Result = ToValue;
   }
  else {
    Result = ((float) rand() / (float) RAND_MAX) * (ToValue1 - FromValue1) + FromValue1;
    if (Result < FromValue) Result = FromValue;
    if (Result >   ToValue) Result = ToValue;
   }

  return (Result);
 }


float MakeRandItemNumber (const float FromValue, const float ToValue) {
  float Result;

  if (FromValue == ToValue)
    Result = FromValue;
  else if (FromValue > ToValue) {
    Result = ((float) rand() / (float) RAND_MAX) * (FromValue - ToValue) + ToValue;
   }
  else {
    Result = ((float) rand() / (float) RAND_MAX) * (ToValue - FromValue) + FromValue;
   }

  return (Result);
 }


dword MakeRandItemIndex (const dword MaxSize) {
  return (dword) ((rand()/(float)(RAND_MAX + 1)) * MaxSize);
 }


float MakeRandItemRatio (void) {
  return (rand()/(float)RAND_MAX) * 1.0f;
 }
/*===========================================================================
 *		End of Function MakeRandItemNumber()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - const TCHAR* MakeRandomItemIdLong (pPrefix);
 *
 * Creates and returns a new random item ID string using a local static string.
 * The new ID is a digit string at most 32 bytes in length and will be a
 * unique ID for the current application. The input prefix string is added
 * to the beginning of the ID if it is not NULL (8 bytes at most).
 * 
 * The ID is composed of the following:
 *	bytes  0- 7: Current system time
 *	bytes  8-15: Drive c: serial number (or clock ticks)
 *	bytes 16-23: Incremented counter per application session
 *	bytes 24-31: Custom prefix string if present
 *
 *=========================================================================*/
const SSCHAR* MakeRandomItemIdLong (const SSCHAR* pPrefix) {
  static SSCHAR	s_Buffer[OBRI_MAXID_SIZE + 4];
  time_t	CurrentTime = time(NULL);
  
	/* Attempt to get a serial number if required */
  if (!l_HasSerialNumber) l_GetItemSerial();

	/* Make the ID string */
  if (pPrefix != NULL && *pPrefix != NULL_CHAR)
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%.8s%08lX%08lX%08lX", pPrefix, l_ItemIDCount, CurrentTime, l_ItemSerialNumber);
  else
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%lX%08lX%08lX", l_ItemIDCount, CurrentTime, l_ItemSerialNumber);

  l_ItemIDCount++;

  //SystemLog.Printf ("Item ID = '%s'", s_Buffer);
  return (s_Buffer);
 }
/*===========================================================================
 *		End of Class Method MakeRandomItemIdLong()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - const TCHAR* MakeNumberItemId8 (pPrefix);
 *
 * Makes an item ID of the format:
 *
 *	bytes 0- 7: Incremented counter per application session
 *	bytes 8-31: Custom prefix string
 *	 
 * The returned string is stored in a local static buffer.
 *
 *=========================================================================*/
const SSCHAR* MakeNumberItemId8 (const SSCHAR* pPrefix) {
  static SSCHAR	s_Buffer[OBRI_MAXID_SIZE + 4];

	/* Make the ID string */
  if (pPrefix != NULL && *pPrefix != NULL_CHAR)
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%.24s%08lX", pPrefix, l_ItemIDCount);
  else
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%08lX", l_ItemIDCount);

  l_ItemIDCount++;

  //SystemLog.Printf ("Item ID = '%s'", s_Buffer);
  return (s_Buffer);
 }
/*===========================================================================
 *		End of Function TCHAR* MakeNumberItemId8()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - const TCHAR* MakeNumberItemId6 (pPrefix);
 *
 * Makes an item ID of the format:
 *
 *	bytes  0-25: Custom prefix string
 *	bytes 26-31: Incremented counter per application session
 *	 
 * The returned string is stored in a local static buffer.
 *
 *=========================================================================*/
const SSCHAR* MakeNumberItemId6 (const SSCHAR* pPrefix) {
  static SSCHAR	s_Buffer[OBRI_MAXID_SIZE + 4];

	/* Make the ID string */
  if (pPrefix != NULL && *pPrefix != NULL_CHAR)
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%.26s%06lX", pPrefix, l_ItemIDCount);
  else
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%06lX", l_ItemIDCount);

  l_ItemIDCount++;

  //SystemLog.Printf ("Item ID = '%s'", s_Buffer);
  return (s_Buffer);
 }
/*===========================================================================
 *		End of Function TCHAR* MakeNumberItemId6()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - const TCHAR* MakeNumberItemId4 (pPrefix);
 *
 * Makes an item ID of the format:
 *
 *	bytes 0- 27: Custom prefix string
 *	bytes 28-31: Incremented counter per application session
 *	 
 * The returned string is stored in a local static buffer.
 *
 *=========================================================================*/
const SSCHAR* MakeNumberItemId4 (const SSCHAR* pPrefix) {
  static SSCHAR	s_Buffer[OBRI_MAXID_SIZE + 4];

	/* Make the ID string */
  if (pPrefix != NULL && *pPrefix != NULL_CHAR)
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%.28s%04lX", pPrefix, l_ItemIDCount);
  else
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%04lX", l_ItemIDCount);

  l_ItemIDCount++;

  //SystemLog.Printf ("Item ID = '%s'", s_Buffer);
  return (s_Buffer);
 }
/*===========================================================================
 *		End of Function TCHAR* MakeNumberItemId4()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - const TCHAR* MakeNumberItemIdShort (pPrefix);
 *
 * Makes an item ID of the format:
 *
 *	bytes 0- 23: Custom prefix string
 *	bytes 24-31: Incremented counter per application session
 *		The counter is not 0 padded.
 *	 
 * The returned string is stored in a local static buffer.
 *
 *=========================================================================*/
const SSCHAR* MakeNumberItemIdShort (const SSCHAR* pPrefix) {
  static SSCHAR	s_Buffer[OBRI_MAXID_SIZE + 4];

	/* Make the ID string */
  if (pPrefix != NULL && *pPrefix != NULL_CHAR)
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%.24s%lX", pPrefix, l_ItemIDCount);
  else
    SSSNPRINTF (s_Buffer, OBRI_MAXID_SIZE, "%lX", l_ItemIDCount);

  l_ItemIDCount++;

  //SystemLog.Printf ("Item ID = '%s'", s_Buffer);
  return (s_Buffer);
 }
/*===========================================================================
 *		End of Function TCHAR* MakeNumberItemIdShort()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - float SplitValue (FromValue, ToValue, Ratio);
 *
 * Returns a value from (FromValue to ToValue) according to the input ratio
 * (0.0 to 1.0).
 *
 *=========================================================================*/
float SplitValue (const float FromValue, const float ToValue, const float Ratio) {
  return (ToValue - FromValue) * Ratio + FromValue;
 }


long SplitValue (const long FromValue, const long ToValue, const float Ratio) {
  return (long) ((ToValue - FromValue) * Ratio + FromValue);
 }
/*===========================================================================
 *		End of Function SplitValue()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - long ShortStringToObriItemType (pString);
 *
 *=========================================================================*/
dword StringToObriItemMask (const SSCHAR* pString) {
  SSCHAR   Buffer[OBRI_MAX_BUFFERSIZE + 1];
  SSCHAR*  pParse;
  dword    Mask = 0;
  bool     Result;
  int      Value;
  

  if (pString == NULL) return (0);
  strnncpy(Buffer, pString, OBRI_MAX_BUFFERSIZE);

  pParse = strtok(Buffer, OBRI_ITEMSEPSTRING);

  while (pParse != NULL) {
    pParse = trim(pParse);

    Result = FindStringValue(Value, pParse, l_ObriItemTypes);

    if (Result) {
      Mask |= Value;
     }
    else {
      AddObGeneralError("Unknown item type '%s'!", pParse);
     }

    pParse = strtok(NULL, OBRI_ITEMSEPSTRING);
   }

  return (Mask);
 }


const SSCHAR* ObriItemMaskToString (const dword ItemMask) {
  static CSString s_Buffer;
  dword           Index;

  s_Buffer.Empty();

  for (Index = 0; l_ObriItemTypes[Index].pString != NULL; ++Index) {
    if ((l_ObriItemTypes[Index].Value & (int)ItemMask) == l_ObriItemTypes[Index].Value) {
      if (!s_Buffer.IsEmpty()) s_Buffer += " | ";
      s_Buffer += l_ObriItemTypes[Index].pString;
     }
   }

  return (s_Buffer);
 }
/*===========================================================================
 *		End of Function ShortStringToObriItemType()
 *=========================================================================*/

  
/*===========================================================================
 *
 * Function - int StringToObriItemType (pString);
 *
 * Converts the input string to an item mask and vice-versa.
 *
 *=========================================================================*/
int StringToObriItemType (const SSCHAR* pString) {
  return FindStringValue(pString, l_ObriItemTypes);
 }

const SSCHAR* ObriItemTypeToString (const long ItemMask) {
  return FindStringValue(ItemMask, l_ObriItemTypes);
 }
/*===========================================================================
 *		End of Function StringToObriItemType()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool StringToObriIDType (OutputValue, pString);
 *
 * Attempt to convert a string to an IDIype.
 *
 *=========================================================================*/
bool StringToObriIDType (int& OutputValue, const SSCHAR* pString) {
  return FindStringValue(OutputValue, pString, l_ObriIDTypes);
 }
/*===========================================================================
 *		End of Function StringToObriIDType()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool ObriIDTypeToString (OutputValue, pString);
 *
 * Attempt to convert a string to an IDIype.
 *
 *=========================================================================*/
bool ObriIDTypeToString (const SSCHAR** ppString, const int IDType) {
  return FindStringValue(ppString, IDType, l_ObriIDTypes);
 }

const SSCHAR* ObriIDTypeToString (const int IDType) {
  return FindStringValue(IDType, l_ObriIDTypes);
 }

bool ObriIDTypeStringToValue (int& IDType, const SSCHAR* ppString) {
  return FindStringValue(IDType, ppString, l_ObriIDTypes);
 }
/*===========================================================================
 *		End of Function ObriIDTypeToString()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool StringToObriEffectType (OutputValue, pString);
 *
 * Attempt to convert a string to an Effect type.
 *
 *=========================================================================*/
bool StringToObriEffectType (int& OutputValue, const SSCHAR* pString) {
  return FindStringValue(OutputValue, pString, l_ObriEffectTypes);
 }
/*===========================================================================
 *		End of Function StringToObriEffectType()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - bool StringToObriEnchantType (OutputValue, pString);
 *
 * Attempt to convert a string to an Enchant type.
 *
 *=========================================================================*/
bool StringToObriEnchantType (int& OutputValue, const SSCHAR* pString) {
  return FindStringValue(OutputValue, pString, l_ObriEnchantTypes);
 }
/*===========================================================================
 *		End of Function StringToObriEnchantType()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - dword ObriGetNextFormID1 (pEspFile);
 *
 * Gets the next available form ID. If the input file is not NULL it 
 * checks for a duplicate form ID.
 *
 *=========================================================================*/
dword ObriGetNextFormID1 (CObEspFile* pEspFile) {
  CObBaseRecord* pRecord = NULL;
  dword		 Count = 0;
  dword          ID;

  do {
    ID = l_NextFormID1;
    ++l_NextFormID1;
    ++Count;
    if (pEspFile != NULL) pRecord = pEspFile->FindFormID(ID);
  } while (pRecord != NULL); 

  return (ID);
 }


dword ObriGetNextFormID2 (CObEspFile* pEspFile) {
  CObBaseRecord* pRecord = NULL;
  dword		 Count = 0;
  dword          ID;

  do {
    ID = l_NextFormID2;
    ++l_NextFormID2;
    ++Count;
    if (pEspFile != NULL) pRecord = pEspFile->FindFormID(ID);
  } while (pRecord != NULL); 

  return (ID);
 }
/*===========================================================================
 *		End of Function ObriGetNextFormID()
 *=========================================================================*/


/*===========================================================================
 *
 * Function - void ObriResetNextFormID1 (void);
 *
 * Resets the form ID generation.
 *
 *=========================================================================*/
void ObriResetNextFormID1 (const dword FormID) {
  l_NextFormID1 = FormID;
 }

void ObriResetNextFormID2 (const dword FormID) {
  l_NextFormID2 = FormID;
 }
/*===========================================================================
 *		End of Function ObriResetNextFormID1()
 *=========================================================================*/

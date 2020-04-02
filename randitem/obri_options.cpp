/*===========================================================================
 *
 * File:	Obri_options.CPP
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 21, 2006
 *
 * Description
 *
 *=========================================================================*/

	/* Include Files */
#include "obri_options.h"


/*===========================================================================
 *
 * Function - void DefaultObriOptions (Options);
 *
 *=========================================================================*/
void DefaultObriOptions (obri_options_t& Options) {
  
  Options.PrefixChance	    = OBRI_DEFAULT_PREFIXCHANCE;
  Options.SuffixChance	    = OBRI_DEFAULT_SUFFIXCHANCE;
  Options.UniqueChance	    = OBRI_DEFAULT_UNIQUECHANCE;
  Options.MaterialChance    = OBRI_DEFAULT_MATERIALCHANCE;
  Options.RandomBell	    = OBRI_DEFAULT_RANDOMBELL;
  Options.UseBell	    = OBRI_DEFAULT_USEBELL;

  Options.ItemMask	    = OBRI_DEFAULT_ITEMTYPEMASK;
  Options.MinLevel	    = OBRI_DEFAULT_MINLEVEL;
  Options.MaxLevel	    = OBRI_DEFAULT_MAXLEVEL;
  Options.RemoveNormalItems = OBRI_DEFAULT_REMOVENORMALITEMS;
  Options.RemoveDuplicates  = false;
  Options.CheckDuplicates   = false;

  Options.NumItems	    = OBRI_DEFAULT_NUMITEMS;
  Options.MakeTotalItems    = true;
  Options.NumArmor	    = 0;
  Options.NumClothing	    = 0;
  Options.NumLights	    = 0;
  Options.NumMisc	    = 0;
  Options.NumScrolls	    = 0;
  Options.NumSigils	    = 0;
  Options.NumStaffs	    = 0;
  Options.NumWeapons	    = 0;

  Options.FirstFormID1      = OBRI_DEFAULT_FIRSTFORMID1;
  Options.FirstFormID2      = OBRI_DEFAULT_FIRSTFORMID2;
  
  Options.LevelFactor       = OBRI_DEFAULT_LEVELFACTOR;
  Options.ForceMinLevel     = false;

  Options.IDPrefix[0]	    = NULL_CHAR;
  Options.ExtendedID.Empty();
  Options.NameDelimiter.Empty();
  Options.IDType	    = OBRI_DEFAULT_IDTYPE;
  Options.MaxNameLength     = OBRI_DEFAULT_MAXNAMELENGTH;
  Options.HidePrefix2       = OBRI_DEFAULT_HIDEPREFIX2;

  Options.OutputEsp         = "testitems.esp";
  Options.OfString          = OBRI_DEFAULT_OFSTRING;

  Options.CreateLevelLists  = OBRI_DEFAULT_CREATELEVELLISTS;
  Options.DistributeLists   = OBRI_DEFAULT_DISTRIBUTELISTS;
  Options.CreateListParents = true;
 }
/*===========================================================================
 *		End of Function DefaultObriOptions()
 *=========================================================================*/


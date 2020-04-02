# Microsoft Developer Studio Project File - Name="obranditem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=obranditem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "obranditem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "obranditem.mak" CFG="obranditem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "obranditem - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "obranditem - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "obranditem - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../tes4lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "obranditem - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../tes4lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "obranditem - Win32 Release"
# Name "obranditem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tes4lib\common\obcallback.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\oberror.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\oberrorhandler.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obfile.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\oblogfile.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obmemfile.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obptrarray.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obutils.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\sstring.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\textutils.cpp
# End Source File
# End Group
# Begin Group "modfile"

# PROP Default_Filter ""
# Begin Group "subrecords"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obefitsubrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obenitsubrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obhedrsubrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obnamesubrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obscitsubrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obspitsubrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obstring1subrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obstringsubrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obsubreccont.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obsubrecord.cpp
# End Source File
# End Group
# Begin Group "records"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obammorecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obapparecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obarmorecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obbasereccont.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obbaserecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obbookrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obcellrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obclotrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obeffitemrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obenchrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obidrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obingrrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obitem1record.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obitem2record.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\oblighrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\oblvlirecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obmiscrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obreccont.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obrefrrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obscptrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obsgstrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obspelrecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obtes4record.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obweaprecord.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obwrldrecord.cpp
# End Source File
# End Group
# Begin Group "groups"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obgroup.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obgrupcont.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obtypegroup.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\tes4lib\modfile\obespfile.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\obfields.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obmoddefs.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obrecname.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecordfilter.cpp
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\obranditem.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\zlib\zdll.exp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\zlib\zdll.lib
# End Source File
# End Group
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ObArmoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObBookDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObClotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObEnchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObLighDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObLvliDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObMiscDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\obranditemView.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecdialog.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecordlistctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecordtreectrl.cpp
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecordvirtuallistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ObriErrorView.cpp
# End Source File
# Begin Source File

SOURCE=.\ObriGenerateView.cpp
# End Source File
# Begin Source File

SOURCE=.\ObriItemView.cpp
# End Source File
# Begin Source File

SOURCE=.\ObriOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObScptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObSgstDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\obsplitterwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ObWeapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\tabctrlsheet.cpp
# End Source File
# End Group
# Begin Group "RandItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\randitem\obri_baseitems.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_defs.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_effects.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_enchantmap.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_generate.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_itemmap.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_levellist.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_options.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_projectfile.cpp
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_uniques.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\clisttest.cpp
# End Source File
# Begin Source File

SOURCE=.\obranditem.cpp
# End Source File
# Begin Source File

SOURCE=.\obranditemDoc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "common."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tes4lib\common\obcallback.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\oberror.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\oberrorhandler.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obfile.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\oblogfile.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obmemfile.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obplatform.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obptrarray.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obsimplemap.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obtypes.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obutils.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\obversion.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\sstring.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\textutils.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\common\zlib\zlib.h
# End Source File
# End Group
# Begin Group "modfile."

# PROP Default_Filter ""
# Begin Group "subrecords."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obammodatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obappadatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obarmodatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obbookdatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obbytesubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obclotdatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obdwordsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obefitsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obenitsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obfloatsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obhedrsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obingrenitsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obint64subrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\oblighdatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\oblvlosubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obmiscdatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obnamesubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obrefrdatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obschrsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obscitsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obsgstdatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obspitsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obstring1subrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obstringsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obsubreccont.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obweapdatasubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obwordsubrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obxtelsubrecord.h
# End Source File
# End Group
# Begin Group "records."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obammorecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obapparecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obarmorecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obbasereccont.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obbaserecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obbookrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obcellrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obclotrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obeffitemrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obenchrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obidrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obingrrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obitem1record.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obitem2record.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\oblighrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\oblvlirecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obmiscrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obreccont.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obrecordmap.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obrefrrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obscptrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obsgstrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obspelrecord.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obtes4record.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obweaprecord.h
# End Source File
# End Group
# Begin Group "groups."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obblockgroup.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obformidgroup.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obgridgroup.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obgroup.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obgrupcont.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\groups\obtypegroup.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\tes4lib\modfile\obbasefile.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\obespfile.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\obfields.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obmoddefs.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\subrecords\obrecname.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\modfile\records\obrecordfield.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecordfilter.h
# End Source File
# End Group
# Begin Group "Misc."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Dialog."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ObArmoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObBookDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObClotDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObEnchDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObLighDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObLvliDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObMiscDlg.h
# End Source File
# Begin Source File

SOURCE=.\obranditemView.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecdialog.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecordlistctrl.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecordtreectrl.h
# End Source File
# Begin Source File

SOURCE=..\tes4lib\windows\obrecordvirtuallistctrl.h
# End Source File
# Begin Source File

SOURCE=.\ObriErrorView.h
# End Source File
# Begin Source File

SOURCE=.\ObriGenerateView.h
# End Source File
# Begin Source File

SOURCE=.\ObriItemView.h
# End Source File
# Begin Source File

SOURCE=.\ObriOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObScptDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObSgstDlg.h
# End Source File
# Begin Source File

SOURCE=.\obsplitterwnd.h
# End Source File
# Begin Source File

SOURCE=.\ObWeapDlg.h
# End Source File
# Begin Source File

SOURCE=.\tabctrlsheet.h
# End Source File
# End Group
# Begin Group "RandItem."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\randitem\obri_baseitems.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_defs.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_effects.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_enchantmap.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_generate.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_itemmap.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_levellist.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_options.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_projectfile.h
# End Source File
# Begin Source File

SOURCE=.\randitem\obri_uniques.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\clisttest.h
# End Source File
# Begin Source File

SOURCE=.\obranditem.h
# End Source File
# Begin Source File

SOURCE=.\obranditemDoc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\obranditem.ico
# End Source File
# Begin Source File

SOURCE=.\res\obranditem.rc2
# End Source File
# Begin Source File

SOURCE=.\res\obranditemDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\baseitems.dat
# End Source File
# Begin Source File

SOURCE=.\displayfilters.dat
# End Source File
# Begin Source File

SOURCE=.\Effects.dat
# End Source File
# Begin Source File

SOURCE=.\levelcounts.csv
# End Source File
# Begin Source File

SOURCE=.\levellists.dat
# End Source File
# Begin Source File

SOURCE=.\lights.dat
# End Source File
# Begin Source File

SOURCE=.\ObRandItem.log
# End Source File
# Begin Source File

SOURCE=.\obranditem.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\sigils.dat
# End Source File
# Begin Source File

SOURCE=.\test1.obri
# End Source File
# Begin Source File

SOURCE=.\weapons.dat
# End Source File
# End Target
# End Project

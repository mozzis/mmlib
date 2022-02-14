# Microsoft Developer Studio Project File - Name="OpenGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OpenGL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OpenGL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OpenGL.mak" CFG="OpenGL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OpenGL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OpenGL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OpenGL - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\obj\Release\OpenGL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Release/" /compile_only /nologo /libs:dll /warn:nofileopt /dll
# ADD F90 /include:"Release/" /compile_only /nologo /libs:dll /warn:nofileopt /dll
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib glu32.lib version.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\OpenGL32.def" /out:"../Release/OpenGL32.dll"

!ELSEIF  "$(CFG)" == "OpenGL - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "..\obj\Debug\OpenGL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Debug/" /compile_only /nologo /libs:dll /debug:full /optimize:0 /warn:nofileopt /dll
# ADD F90 /include:"Debug/" /compile_only /nologo /libs:dll /debug:full /optimize:0 /warn:nofileopt /dll
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /FR /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib version.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\OpenGL32d.def" /out:"../Debug/OpenGL32d.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OpenGL - Win32 Release"
# Name "OpenGL - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClientFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DdxDdvSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFilePrintOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOpenglInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGL.rc
# End Source File
# Begin Source File

SOURCE=.\OpenGL32.def
# PROP Exclude_From_Build 2
# End Source File
# Begin Source File

SOURCE=.\OpenGL32d.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GlDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\GlView.cpp
# End Source File
# Begin Source File

SOURCE=.\GlWinApp.cpp
# End Source File
# Begin Source File

SOURCE=.\Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\IntMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\MathAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertySheetAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\Scale.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterWndEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# Begin Source File

SOURCE=.\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Version.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClientFrm.h
# End Source File
# Begin Source File

SOURCE=.\DdxDdvSupport.h
# End Source File
# Begin Source File

SOURCE=.\DlgFilePrintOptions.h
# End Source File
# Begin Source File

SOURCE=.\DlgOpenglInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptions.h
# End Source File
# Begin Source File

SOURCE=.\OpenGL.h
# End Source File
# Begin Source File

SOURCE=.\GlDocument.h
# End Source File
# Begin Source File

SOURCE=.\GlView.h
# End Source File
# Begin Source File

SOURCE=.\GlWinApp.h
# End Source File
# Begin Source File

SOURCE=.\Helper.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\IntMatrix.h
# End Source File
# Begin Source File

SOURCE=.\MathAdd.h
# End Source File
# Begin Source File

SOURCE=.\mathglobals.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageAdjust.h
# End Source File
# Begin Source File

SOURCE=.\PropertySheetAdjust.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.hm
# End Source File
# Begin Source File

SOURCE=.\Scale.h
# End Source File
# Begin Source File

SOURCE=.\SplitterWndEx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringEx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\currotat.cur
# End Source File
# Begin Source File

SOURCE=.\res\OpenGL.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=".\Lies mich.txt"
# End Source File
# Begin Source File

SOURCE=".\Was ist neu.txt"
# End Source File
# End Target
# End Project

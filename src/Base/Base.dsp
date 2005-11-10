# Microsoft Developer Studio Project File - Name="Base" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Base - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Base.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Base.mak" CFG="Base - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Base - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "Base - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Base - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Base - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/Base", XPUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Base - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Gr /W3 /GX /O2 /Ob2 /I "$(SPRINGHEAD)/src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Base - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "$(SPRINGHEAD)/src" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "DEBUG" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"BaseD.lib"

!ELSEIF  "$(CFG)" == "Base - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Base___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "Base___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /O2 /Op /Ob2 /I "$(SPRINGHEAD)/src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"BaseMD.lib"

!ELSEIF  "$(CFG)" == "Base - Win32 MFC Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Base___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "Base___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /vd0 /GR- /GX /Zi /Ox /Ot /Og /Oi /Op /Ob2 /Gf /I "$(SPRINGHEAD)/src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Oa /Ow /Gy /Fr
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"BaseM.lib"

!ENDIF 

# Begin Target

# Name "Base - Win32 Release"
# Name "Base - Win32 Debug"
# Name "Base - Win32 MFC Debug"
# Name "Base - Win32 MFC Release"
# Begin Source File

SOURCE=.\Affine.cpp
# End Source File
# Begin Source File

SOURCE=.\Affine.h
# End Source File
# Begin Source File

SOURCE=.\Base.h
# End Source File
# Begin Source File

SOURCE=.\BaseDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseDebug.h
# End Source File
# Begin Source File

SOURCE=.\BaseTypeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTypeInfo.h
# End Source File
# Begin Source File

SOURCE=.\BaseUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseUtility.h
# End Source File
# Begin Source File

SOURCE=.\Combination.h
# End Source File
# Begin Source File

SOURCE=.\Env.h
# End Source File
# Begin Source File

SOURCE=.\KeyList.h
# End Source File
# Begin Source File

SOURCE=.\QuadProgram.h
# End Source File
# Begin Source File

SOURCE=.\RingBuf.h
# End Source File
# Begin Source File

SOURCE=.\StrbufBase.cpp
# End Source File
# Begin Source File

SOURCE=.\StrbufBase.h
# End Source File
# Begin Source File

SOURCE=.\TinyMat.h
# End Source File
# Begin Source File

SOURCE=.\TinyVec.h
# End Source File
# Begin Source File

SOURCE=.\TMatrix.h
# End Source File
# Begin Source File

SOURCE=.\TMatrixUtility.h
# End Source File
# Begin Source File

SOURCE=.\TQuaternion.h
# End Source File
# Begin Source File

SOURCE=.\TVector.h
# End Source File
# End Target
# End Project

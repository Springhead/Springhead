# Microsoft Developer Studio Project File - Name="Collision" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Collision - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Collision.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Collision.mak" CFG="Collision - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Collision - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "Collision - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Collision - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Collision - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/Samples/MFCD3DDyna", KNXAAAAA"
# PROP Scc_LocalPath "..\samples\mfcd3ddyna"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Collision - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /W3 /vd0 /GX /Ox /Ot /Oa /Og /Oi /Ob2 /Gf /I "$(SPRINGHEAD)/src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# SUBTRACT CPP /Ow /Os /Gy
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Collision.lib"

!ELSEIF  "$(CFG)" == "Collision - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /Ob2 /I "$(SPRINGHEAD)/src" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"CollisionD.lib"

!ELSEIF  "$(CFG)" == "Collision - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFC Debug"
# PROP BASE Intermediate_Dir "MFC Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Op /Ob0 /I "$(SPRINGHEAD)/src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"CollisionMD.lib"

!ELSEIF  "$(CFG)" == "Collision - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MFC Release"
# PROP BASE Intermediate_Dir "MFC Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /vd0 /GX /Zi /Ox /Ot /Og /Oi /Op /Ob2 /Gf /I "$(SPRINGHEAD)/src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Oa /Ow /Gy /Fr
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"CollisionM.lib"

!ENDIF 

# Begin Target

# Name "Collision - Win32 Release"
# Name "Collision - Win32 Debug"
# Name "Collision - Win32 MFC Debug"
# Name "Collision - Win32 MFC Release"
# Begin Source File

SOURCE=.\CDCollisionEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\CDCollisionEngine.h
# End Source File
# Begin Source File

SOURCE=.\CDConvex.cpp
# End Source File
# Begin Source File

SOURCE=.\CDConvex.h
# End Source File
# Begin Source File

SOURCE=.\CDDetectorImp.cpp
# End Source File
# Begin Source File

SOURCE=.\CDDetectorImp.h
# End Source File
# Begin Source File

SOURCE=.\CDFramePair.cpp
# End Source File
# Begin Source File

SOURCE=.\CDFramePair.h
# End Source File
# Begin Source File

SOURCE=.\CDGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\CDGeometry.h
# End Source File
# Begin Source File

SOURCE=.\CDGJK.cpp
# End Source File
# Begin Source File

SOURCE=.\CDMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\CDMesh.h
# End Source File
# Begin Source File

SOURCE=.\CDPhysicalMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\CDPhysicalMaterial.h
# End Source File
# Begin Source File

SOURCE=.\CDPolyhedron.cpp
# End Source File
# Begin Source File

SOURCE=.\CDPolyhedron.h
# End Source File
# Begin Source File

SOURCE=.\CDQuickHull2D.h
# End Source File
# Begin Source File

SOURCE=.\CDQuickHull2DImp.h
# End Source File
# Begin Source File

SOURCE=.\CDQuickHull3D.h
# End Source File
# Begin Source File

SOURCE=.\CDQuickHull3DImp.h
# End Source File
# Begin Source File

SOURCE=.\CDRegisterLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\CDSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\CDSphere.h
# End Source File
# Begin Source File

SOURCE=.\Collision.h
# End Source File
# End Target
# End Project

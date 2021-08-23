#  CMakeLists.txt for RunSwig_CSharp
#
#  OPTIONS:
#	TRAGET_TYPE:	DLL構成:    'ALL'(default) or 'PH'
#	PLAT:		マシン構成: 'x64'(default) or 'x86' 
#
#  DESCRIPTION:
#	NameManager, TargetManager を呼び出して環境を設定した後, swig を実行
#	して c++/c# のソースを生成する(Makefile.RunSwig_CSharp).
#
set(NAME_MANAGER_VERSION 1.1.0)		# 2021/07/29 バージョン管理開始

# ------------------------------------------------------------------------------
#  ターゲット名の設定
#
set(Target "RunSwig_CSharp")
message("** ${Target} **")

if(NOT DEFINED TARGET_TYPE)
    set(TARGET_TYPE ALL)
endif()
if(NOT DEFINED PLAT)
    set(PLAT x64)
endif()

# ------------------------------------------------------------------------------
#  パラメータの読み込み
#
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeSettings.txt")
    include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeSettings.txt")
endif()

# ------------------------------------------------------------------------------
#  Main target.
#
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(TO_NATIVE_PATH ${PROJECT_DIR} PROJECT_DIR)
file(TO_NATIVE_PATH ${TOPDIR} TOPDIR)

set(MAKE nmake /nologo)
set(Makefile ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.runSwig_CSharp)
set(MakeOpts TARGET=${TARGET_TYPE} PLAT=${PLAT} PROJECTDIR=${PROJECT_DIR}
						TOPDIR=${TOPDIR})
set(OutFiles)
file(TO_NATIVE_PATH ${Makefile} Makefile)

add_custom_target(${Target}
    BYPRODUCTS ${OutFiles}
    COMMAND echo ${MAKE} ${Makefile} ${MakeOpts}
    COMMAND ${MAKE} ${Makefile} ${MakeOpts}
    DEPENDS ${OutFiles}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/NameManager ${CMAKE_SOURCE_DIR}/NameManager)
add_subdirectory(${CMAKE_SOURCE_DIR}/TargetManager ${CMAKE_SOURCE_DIR}/TargetManager)
add_dependencies(${Target} NameManager TargetManager)

# end: CMakeLists.txt

# CMakeLists.txt for Install
#
set(INSTALL_VERSION 1.1.0)		# 2021/08/02 バージョン管理開始

# ------------------------------------------------------------------------------
#  Target definition.
#
set(Target Install)
message("** ${Target} **")

# ------------------------------------------------------------------------------
#  Main target.
#
set(Script ${NMAKE})
set(MakeFile Makefile.install)
set(ProjectDir ${SPRCSHARP}/Install)
set(Vers ${VS_VERSION})
set(Plat ${ARCHITECTURE})
set(Conf $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:Trace>:Trace>)
file(TO_NATIVE_PATH ${ProjectDir} ProjectDir)

add_custom_target(${Target} ALL
    COMMAND echo ${Script} /nologo -f ${MakeFile} PROJECTDIR=${ProjectDir} VERS=${Vers} PLAT=${Plat} CONF=${Conf}
    COMMAND ${Script} /nologo -f ${MakeFile} PROJECTDIR=${ProjectDir} VERS=${Vers} PLAT=${Plat} CONF=${Conf}
    BYPRODUCTS ${TargetFile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT [[  installing DLLs ... ]]
)

# end: CMakeLists.txt

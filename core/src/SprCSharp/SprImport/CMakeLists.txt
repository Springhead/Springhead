# CMakeLists.txt for SprImport
#
set(SPR_IMPORT_VERSION 1.1.0)		# 2021/08/18 バージョン管理開始

# ------------------------------------------------------------------------------
#  Set target name.
#
set(Target "SprImport")
message("** ${Target} **")

# ------------------------------------------------------------------------------
#  Output directory.
#
set(Vers ${VS_VERSION})
set(Plat ${ARCHITECTURE})
set(Conf $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:Trace>:Trace>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPRCSHARP}/${Vers}/bin/${Plat}/${Conf})

# ------------------------------------------------------------------------------
#  Source files
#
set(SRCS CSUtility.cs module.wrapper.cs wrapper.cs)
set_source_files_properties(module.wrapper.cs
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTIES GENERATED TRUE
)
foreach(_proj ${SPR_PROJS})
    set(_name CS${_proj}.cs)
    set(SRCS ${SRCS} ${_name})
    #  These files will be generated at runtime.
    set_source_files_properties(${_name}
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	PROPERTIES GENERATED TRUE
    )
endforeach()

# ------------------------------------------------------------------------------
#  Main target
#
add_library(${Target} SHARED
    ${SRCS}
)
message("++ SRCS ${SRCS}")

# end: CMakeLists.txt

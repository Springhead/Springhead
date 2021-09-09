# CMakeLists.txt for RunSwig
#	CMake variable "SPRINGHEAD_TOPDIR" must be set by the caller.
set(COLLISION_VERSION 1.1.2)		# 2021/09/09 bug fix

# target
set(Target "RunSwig")
message("** ${Target} **")

# Springhead top directory and build directory
if("${TOPDIR}" STREQUAL "")
    set(TOPDIR "${CMAKE_SOURCE_DIR}/../..")
endif()
set(SPR_TOP_DIR  "${TOPDIR}")
set(SPR_SRC_DIR  "${TOPDIR}/core/src")
get_filename_component(TOPDIR ${TOPDIR} ABSOLUTE)
get_filename_component(SPR_TOP_DIR ${SPR_TOP_DIR} ABSOLUTE)
get_filename_component(SPR_SRC_DIR ${SPR_SRC_DIR} ABSOLUTE)
get_filename_component(PRJ_BLD_DIR ${PROJECT_BINARY_DIR} ABSOLUTE)
file(TO_NATIVE_PATH "${TOPDIR}" TOPDIR)
file(TO_NATIVE_PATH "${SPR_TOP_DIR}" SPR_TOP_DIR)
file(TO_NATIVE_PATH "${SPR_SRC_DIR}" SPR_SRC_DIR)
file(TO_NATIVE_PATH "${PRJ_BLD_DIR}" PRJ_BLD_DIR)
#message(STATUS "${Target}: SPR_TOP_DIR: [${SPR_TOP_DIR}]")
#message(STATUS "${Target}: SPR_SRC_DIR: [${SPR_SRC_DIR}]")
#message(STATUS "${Target}: PRJ_BLD_DIR: [${PRJ_BLD_DIR}]")

# ------------------------------------------------------------------------------
#  Check if setup file exists.
#
set(SetupFileName ${SPR_SRC_DIR}/setup.conf)
if(EXISTS ${SetupFileName})
    message(STATUS "${SetupFileName} exists")
endif()

# RunSwig directory
set(RSSrcDir ${SPR_SRC_DIR}/RunSwig)
set(RSBldDir ${PRJ_BLD_DIR}/RunSwig)
file(TO_NATIVE_PATH "${RSSrcDir}" RSSrcDir)
file(TO_NATIVE_PATH "${RSBldDir}" RSBldDir)
#message(STATUS "${Target}: RSSrcDir: [${RSSrcDir}]")
#message(STATUS "${Target}: RSBldDir: [${RSBldDir}]")

if(Windows)
    set(Make "nmake /NOLOGO")
    set(Makefile Makefile.win)
elseif(Linux)
    set(Make make)
    set(Makefile Makefile.unix)
elseif(FreeBSD)
    set(Make gmake)
    set(Makefile Makefile.unix)
endif()

# RunSwig generate files
#	These files are removed when 'clean' target executed
#
set(_for_all @.i @Decl.hpp @Stub.cpp @Stub.makefile Spr@Decl.hpp swig_spr.log)
set(_for_FW  FWOldSpringheadDecl.hpp FWOldSpringheadStub.cpp)

# Swig does not apply to project 'Base'.
set(_projs ${SPR_PROJS})
list(REMOVE_ITEM _projs Base)
#
set(generated_files)
foreach(_dir ${_projs})
    foreach(_file ${_for_all})
	set(_tmp ${SPR_SRC_DIR}/${_dir}/${_file})
	string(REPLACE @ ${_dir} _tmp ${_tmp})
	list(APPEND generated_files ${_tmp})
    endforeach()
endforeach()
set(_dir Framework)
foreach(_file ${_for_FW})
    set(_tmp ${SPR_SRC_DIR}/${_dir}/${_file})
    list(APPEND generated_files ${_tmp})
endforeach()

foreach(_file ${generated_files})
    file(TO_NATIVE_PATH "${_file}" _file)
    #message("     generate: ${_file}")
endforeach()

# ------------------------------------------------------------------------------
# main target
#
if(Windows)
    add_custom_target(${Target}
	DEPENDS ${generated_files}
    )
    add_custom_command(
	OUTPUT ${generated_files}
	COMMAND IF EXIST "${SetupFileName}" (echo [ setup file exists ])
	COMMAND cmd /c ${Make} -f ${Makefile}
	WORKING_DIRECTORY ${RSSrcDir}
	COMMENT [[  generating files...]]
    )
elseif(unix)
    add_custom_target(${Target}
	DEPENDS ${generated_files}
    )
    add_custom_command(
	OUTPUT ${generated_files}
	COMMAND ${Make} -f ${RSSrcDir}/${Makefile}
	WORKING_DIRECTORY ${RSSrcDir}
	COMMENT [[  generating files...]]
    )
endif()

# end: CMakeLists.txt

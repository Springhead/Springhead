#  make.system.cmake

# ------------------------------------------------------------------------------
#  システムアーキテクチャを判定する
#
set(ARCH ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID})
set(PROC ${CMAKE_HOST_SYSTEM_PROCESSOR})
#message(STATUS ARCH: [${ARCH}])
#message(STATUS PROC: [${PROC}])

if("${ARCH}" STREQUAL "x64" OR "${ARCH}" STREQUAL "X64")
    set(ARCHITECTURE "x64")
elseif("${ARCH}" STREQUAL "x86" OR "${ARCH}" STREQUAL "X86")
    set(ARCHITECTURE "x86")
elseif("${PROC}" STREQUAL "x86_64" OR "${PROC}" STREQUAL "AMD64")
    set(ARCHITECTURE "x64")
endif()

# ------------------------------------------------------------------------------
#  実行OSを判定する
#	Cygwin, MacOS について判定条件を追加すること
#
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    set(Windows True)
elseif("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
    set(Linux True)
    set(unix True)
endif()
set(Cygwin  False)
set(MacOS   False)

# end: make.system.cmake

#  make.func.cmake

# ------------------------------------------------------------------------------
#  function split()
#	<str> を <sep> で分割してできた配列を <output> に設定する。
#
function(split str sep output)
	string(REPLACE "${sep}" ";" _tmp_list "${str}")
	set (${output} "${_tmp_list}" PARENT_SCOPE)
endfunction()

# ------------------------------------------------------------------------------
#  function join()
#	<array> の各要素を <glue> で結合してできた文字列を <output> に設定する。
#
function(join array glue output)
	string (REPLACE ";" "${glue}" _tmp_str "${array}")
	set (${output} "${_tmp_str}" PARENT_SCOPE)
endfunction()

# ------------------------------------------------------------------------------
#  function eval()
#	code		実行する(CMakeの)コード
#
function(eval code)
    set(path ${CMAKE_CURRENT_LIST_DIR}/.temp.cmake)
    file(WRITE ${path} "${code}")
    include(${path})
    file(REMOVE ${path})
endfunction()

# ------------------------------------------------------------------------------
#  リストに他のリストを結合する
#  macro append()
#  marco prepend()
#	list1		元のリスト
#	list2		結合するリスト
#
macro(append list1 list2)
    if(DEFINED ${list2})
        list(APPEND ${list1} ${${list2}})
    endif()
endmacro()

macro(prepend list1 list2)
    if(DEFINED ${list2})
        if(DEFINED ${list1})
            set(__tmp ${${list1}})
            set(${list1})
            list(APPEND ${list1} ${${list2}})
            list(APPEND ${list1} ${__tmp})
        else()
            set(${list1} ${${list2}})
        endif()
    endif()
endmacro()

# ------------------------------------------------------------------------------
#  環境変数が設定されていたらその値を、さもなければデフォルト値を返す。
#  function env()
#	return_var	戻り値を設定する変数名
#	environ_name	環境変数の名前
#	default_value	デフォルト値
#
function(envval return_var environ_name default_value)
    set(tmp $ENV{${environ_name}})
    if("${tmp}" STREQUAL "")
	set(${return_var} ${default_value} PARENT_SCOPE)
    else()
	set(${return_var} ${tmp} PARENT_SCOPE)
    endif()
endfunction()

# ------------------------------------------------------------------------------
#  環境変数の値に従って"-I"オプションを設定する。
#  function add_inc()
#	return_var	戻り値を設定する変数名
#	environ_name	環境変数の名前
#
function(add_inc return_var environ_name)
    set(tmp $ENV{${environ_name}})
    if("${tmp}" STREQUAL "")
	set(${return_var} "")
    else()
	set(${return_var} -I${tmp} PARENT_SCOPE)
    endif()
endfunction()

# ------------------------------------------------------------------------------
#  環境変数の値に従って"-L"オプションを設定する。
#  function add_inc()
#	return_var	戻り値を設定する変数名
#	environ_name	環境変数の名前
#
function(add_lib return_var environ_name)
    set(tmp $ENV{${environ_name}})
    if("${tmp}" STREQUAL "")
	set(${return_var} "")
    else()
	set(${return_var} -L${tmp} PARENT_SCOPE)
    endif()
endfunction()

# ------------------------------------------------------------------------------
#  【マクロ】ファイルをincludeする。
#  ${option}が定義されているならば、それが指すファイルをincludeする。
#  さもなければ、${file_path}, ${default_file}の順にファイルを探し、
#  最初に見つかったファイルをincludeする。
#  
#  macro include_file()
#	option_var	オプション変数名
#			  -D <option_var>=<file-absolute-path>
#	file_path	ディレクトリ名 (絶対パス)
#	default_file	デフォルトディレクトリ名 (絶対パス)
#	file_name	includeするファイル名	
#
macro(include_file option_var file_path default_file)
    if(DEFINED ${option_var})
	if(EXISTS ${${option_var}})
	    message(STATUS "option: ${option_var}=\"${${option_var}}\"")
	    if(IS_DIRECTORY ${${option_var}})
		message("Error: \"${${option_var}}\" is directory")
		return()
	    endif()
	    message(STATUS "including \"${${option_var}}\"")
	    include(${${option_var}})
	else()
	    message("Error: File \"${${option_var}}\" not exists!")
	    return()
	endif()
    else()
	if(EXISTS ${file_path})
	    message(STATUS "including \"${file_path}\"")
	    include(${file_path})
	elseif(EXISTS ${default_file})
	    message(STATUS "including \"${default_file}\"")
	    include(${default_file})
	else()
	    message("Error: File \"${file_name}\" not found")
	    return()
	endif()
    endif()
endmacro()

# ------------------------------------------------------------------------------
#  ${start_dir}で指定されたディレクトリに${file_name}のファイルがあるか調べる。
#  ファイルがない場合は一つ上位のディレクトリを調べるということをファイルが
#  見つかるまで繰り返す。
#  ファイルが見つかったときはそのファイルの絶対パスを、見つからなかったときは
#  "NotFound"を返す。
#
#  function finefile()
#	start_dir	検査を開始するディレクトリの絶対パス。
#	file_name	見つけるファイルの名称
#	output		戻り値を設定する変数名
#
function(findfile start_dir file_name output)
	#message("     findfile: ${file_name}")
	split(${start_dir} "/" DirList)
	while(true)
		join("${DirList}" "/" PathStr)
		#message("       ${PathStr}")
		if(EXISTS "${PathStr}/${file_name}")
			set(${output} "${PathStr}/${file_name}" PARENT_SCOPE)
			return()
		endif()
		list(LENGTH DirList ListLen)
		math(EXPR result "${ListLen} - 1")
		if(NOT ${result})
			break()
		endif()
		list(REMOVE_AT DirList ${result})
	endwhile()
	set(${output} "NotFound" PARENT_SCOPE)
endfunction()

# ------------------------------------------------------------------------------
#  ${cmake_build_dir}で示されたディレクトリからビルド(作業)ディレクトリ名を得る。
#  ビルドディレクトリの1段下(例えば"build/Foundation")から呼ぶこと － この場合は
#  build"を返す。
#
function(get_build_dir cmake_build_dir output)
	split("${cmake_build_dir}" "/" _tmp_dir_list)
	list(LENGTH _tmp_dir_list _tmp_list_len)
	math(EXPR result "${_tmp_list_len} - 1")
	list(GET _tmp_dir_list ${result} _tmp_build_dir)
	set(${output} ${_tmp_build_dir} PARENT_SCOPE)
endfunction()

# end: make.func.cmake

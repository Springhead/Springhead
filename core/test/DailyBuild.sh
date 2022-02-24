#! /bin/bash

# =============================================================================
#  SYNOPSIS
#	DailyBuild.sh [options] [test-repository] [result-repository]]
#
#	OPTIONS:
#	    --do-not-clone:	クローンを実行せず既存のレポジトリを使用する.
#	    --hook:		クローン後, "DailyBuildHook/hook.sh" が適用
#				される.
#	    --hook-only:	--hook を適用後, 処理を終了する.
#
#	ARGUMENTS:
#	    test-repository:	テストを実行するディレクトリ.
#				既存のディレクトリの場合その内容は破棄される.
#				デフォルトは "Springhead".
#	    result-repository:	テスト結果が置かれるディレクトリ.
#				デフォルトは "DailyBuildResult/Result"
#
#  DESCRIPTION
#	DailyBuild を実行を制御する (crontab に登録する).
#	  (1) test-repository に最新のコミットを clone する.
#	  (2) 指定があれば hook を適用する (ファイルのすげ替え).
#	  (3) テストを実行する.
#
#	Python が実行できること. このファイルはテストマシンの "~/Project" に
#	コピーし, そこから起動する.
#
#	起動パラメータの例
#	  DailyBuild.sh Springhead DailyBuildResult/Result
#
#	このファイル "DailyBuild.sh" 及び "hook.sh" は, DailyBuild を実行
#	しても自動的にはアップデートされない. 変更があったときは手動でコピー
#	すること.
#	
#  VERSION
#     Ver 1.0    2021/05/10 F.Kanehori	バッチファイルの再構築.
#     Ver 1.1	 2021/09/08 F.Kanehori	Add -S option to DailyBuild.py.
#     Ver 2.0	 2022/01/26 F.Kanehori	DailyBuild.py の機能を吸収.
# =============================================================================
PROG=`basename $0`
CWD=`pwd`
DEBUG=0

CLONE="yes"
HOOK="no"
HOOKONLY="no"
ARGS[1]="Springhead"			# default test repository
ARGS[2]="DailyBuildResult/Result"	# default result repository
OPTS=""		# "-c Release -p x64"
SETUP="no"
SETUPFILE="setup.conf"
DRYRUN="no"
VERBOSE=""

do_exec () {
	echo $*
	if [ "$DRYRUN" == "no" ]; then
		$*
	fi
}

function usage () {
	echo "DailyBuild [options] [test-repository [result-repository]]"
	echo
	echo "  options:"
	echo "    --do-not-clone: Do not clone source tree."
	echo "    --hook:         Apply hook script (\"DailyBuildHook/hook.sh\")."
	echo "    --hook-only:    Apply hook script then exit."
	echo "    -c conf:        Configurations (Debug | Release)."
	echo "    -p plat:        Platform (x86 | x64)."
	echo
	echo "  arguments:"
	echo "    test_repository:   test repository (default: Springhead)"
	echo "    result_repository: result repository (default: DailyBuildResult/Result)"
	echo
}

# ----------------------------------------------------------------------
#  Step 0
#	引数の解析
#
POS=1
for opt in "$@"; do
    case "${opt}" in
	'--do-not-clone')
		CLONE="no" && shift
		;;
	'--hook' )
		HOOK="yes" && shift
		;;
	'--hook-only' )
		HOOK="yes"
		HOOKONLY="yes" && shift
		;;
	'-s' )
		SETUPFILE=$2 && shift && shift
		;;
	'-S' )
		SETUP="yes" && shift
		;;
	'-D' )
		DRYRUN="yes" && shift
		;;
	'-v' )
		VERBOSE="1" && shift
		;;
	'-h' | '--help' )
		usage
		exit 0
		;;
	-* )
		echo "illegal option -- '$( echo $1 | sed 's/^-*//' )'" 1>&2
		exit 1
		;;
	* )
		if [ $POS -gt 2 ]; then
			echo "too many arguments"
			exit 1
		fi
		ARGS[${POS}]=$1 && POS=$((POS + 1)) && shift
		;;
    esac
done
START_DIRECTORY=`pwd`
TEST_REPOSITORY=${ARGS[1]}
RESULT_REPOSITORY=${ARGS[2]}

REMOTE_REPOSITORY="https://github.com/sprphys/Springhead"
HOOKFILE="DailyBuildHook/hook.sh"

echo "start directory:   [$START_DIRECTORY]"
echo "test repository:   [$TEST_REPOSITORY]"
echo "result repository: [$RESULT_REPOSITORY]"
if [ "$SETUP" == "yes" ]; then
	echo "setup file:        [$SETUPFILE]"
fi
echo "conf :  [$CONF]"
echo "plat :  [$PLAT]"
echo "clone:  [$CLONE]"
echo "hook:   [$HOOK]"
echo "dryrun: [$DRYRUN]"

if [ "$HOOK" != "no" ] && [ ! -e $HOOKFILE ]; then
	echo "--hook specified, but \"$HOOKFILE\" does not exist."
	echo "abort"
	exit 1
fi
if [ $DEBUG -ne 0 ]; then
	echo -n "OK [y/n]? "
	read x
	if [ "$x" != "y" ]; then
		echo "done"
		exit 0
	fi
fi

# ----------------------------------------------------------------------
#  Step 1
#	必要なツールの確認 (python が実行できること)
#
PYTHON=python

TOOLS=$PYTHON
ok="ok"
for tool in $TOOLS; do
	which $tool >/dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "$PROG: we need '$tool'"
		ok="no"
	fi
done
if [ "$ok" != "ok" ]; then
	echo "abort"
	exit 1
fi

# ----------------------------------------------------------------------
#  Step 2
#	Springhead を最新の状態にする.
#
if [ "$CLONE" == "yes" ]; then
	# 古いテストレポジトリがあったら削除して更の状態にする.
	if [ -d $TEST_REPOSITORY ]; then
		echo "$PROG: removing directory \"$TEST_REPOSITORY\""
		/bin/rm -rf $TEST_REPOSITORY
	fi
	echo "cloning $REMOTE_REPOSITORY"
	git clone $REMOTE_REPOSITORY $TEST_REPOSITORY
fi

# ----------------------------------------------------------------------
#  Step 3
#	Hook ファイルが存在したらそれをコピーする.
#
if [ "$HOOK" != "no" ]; then
	abs_dir=$(cd "$TEST_REPOSITORY" && pwd)
	echo "$PROG: calling hook \"$HOOKFILE\"."
	/bin/bash $HOOKFILE $abs_dir
fi
if [ "$HOOKONLY" == "yes" ]; then
	exit 0
fi

# ----------------------------------------------------------------------
#  Step 4
#	セットアップを実行する (-S オプション指定時のみ)
#
if [ "$SETUP" == "yes" ]; then
	echo "execute setup process (-S)"
	abs_dir=$(cd "$START_DIRECTORY/$TEST_REPOSITORY/core/src" && pwd)
	cd $abs_dir
	args="-R $TEST_REPOSITORY -f -s $SETUPFILE $VERBOSE"
	do_exec ./setup.sh $args
fi

# ----------------------------------------------------------------------
#  Step 5
#	テストを実行する
#
cd "$START_DIRECTORY/$TEST_REPOSITORY/core/test"
echo "test directory: \"`pwd`\""
do_exec $PYTHON TestMainGit.py $VERBOSE $TEST_REPOSITORY $RESULT_REPOSITORY
echo "rc: $?"
echo "done"

exit 0
#end: DailyBuild.sh

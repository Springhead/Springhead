#! /bin/bash
# =============================================================================
#  SYNOPSIS
#	hook.bat test-repository
#	    tset-repository	テストレポジトリ名. 絶対パスで与えること.
#
#  DESCRIPTION
#	DailyBuild.bat から呼び出され, dailybuild 実行時に使用するファイルを
#	すげ替える仕組みを提供する. この仕組を利用して github にコミットする
#	ことなくデバッグ操作を仕込むことを可能とする.
#
#	このファイルがある場所を <hook>, テストレポジトリを <test> とするとき
#	"<hook>/core" 以下にあるファイルを "<test>/core" 以下の対応する場所に
#	コピーする.
#
#	このファイルは, テストマシンの次のディレクトリにコピーして使用する.
#	     "~/Project/DailyBuildHook"
#	ただし, "~/Project" から呼び出すこと.
#
# ============================================================================
PROG=`basename $0`
CWD=`pwd`
echo "[$PROG] was called from $CWD."
DEBUG=0
HOOKDIR="DailyBuildHook"
HOOKLOG="$CWD/hook.log"

if [ "$1" == "" ]; then
	echo "Usage: $PROG test-repository"
	exit 1
fi
REPOSITORY=$1

if [ -e "$HOOKDIR/ignore" ]; then
	# このファイルが存在したら何もせずに抜ける
	echo "$PROG: nothing applied (\"ignore\" exits)"
	exit 0
fi

# core 以下にあるファイルをコピーする
#
cd $HOOKDIR
for file in `find core -name '*'`; do
	if [ -f $file ]; then
		echo -n "$PROG: applying \"$file\" ... "
		cp -f $file $REPOSITORY/$file >$HOOKLOG 2>&1
		if [ $? -eq 0 ]; then
			echo "OK"
		else
			echo "NG"
			cat $HOOKLOG
		fi
	fi
done
cd $CWD
echo done

exit 0
#end: hook.sh

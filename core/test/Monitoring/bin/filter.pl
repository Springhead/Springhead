#!/usr/bin/perl
BEGIN { push(@INC, 'bin'); }
# ==============================================================================
#  File:
#	filter.pl
#
#  Description:
#	mydiff.pl で並べ替えたログファイル2つについて、diff -c 形式に倣った
#       出力情報を作成する。
#
#  Version:
#	Ver 1.0  2013/02/06 F.Kanehori	Windows 移植初版 (全面改訂)
#	Ver 1.01 2015/05/25 F.Kanehori	コメント修正
#	Ver 1.02 2018/07/12 F.Kanehori	コメント修正
# ==============================================================================
use strict;
use warnings;
use Getopt::Long;
use base_lib;
use dailybuild_lib;

# 変数
#
my $script = leafname($0);
my $infile = "-";		# input file
my $outfile = "-";		# output file

# オプションの定義
#
my $help = 0;
my $verbose = 0;
my $debug = 0;
my %options = (
    'help'	=> \$help,
    'outfile=s'	=> \$outfile,
    'verbose'	=> \$verbose,
    'Debug=i'	=> \$debug
);

# コマンド引数の処理
#
GetOptions(%options);
usage() if $help;
verbose_set($verbose);
debug_push($debug);
if ($#ARGV == 0) { $infile = $ARGV[0] };
if ($#ARGV >  0) { usage() };

my $infilename = $infile eq "-" ? "STDIN" : $infile;
my $outfilename = $outfile eq "-" ? "STDOUT" : $outfile;
verbose("infile:  $infilename");
verbose("outfile: $outfilename");
show_options() if $debug > 0;

# ファイルを読み込む
#	%modules: モジュール名をキーとした %threads 情報
#	%threads: スレッド番号をキーとした入力データ行
#	@modules: モジュール名の配列 (出現順)
#	@lines:   入力データ行
#	※ モジュール開始パターンは '2' を使用
#
my ($modhash, $modname, $lines) = read_log($infile, $script, 2);
if (!defined($modhash) || !defined($modname) || !defined($lines)) {
    exit -1;
}
my %modules = %{$modhash};
my @modules = @{$modname};
my @lines = @{$lines};
object_dump(2, \%modules, "\%modules");
object_dump(2, \@modules, "\@modules");
object_dump(3, \@lines, "\@lines");

STDOUT->autoflush(1);

# 出力ファイルをオープンする
#
debug(1, "opening file \"%s\" for output\n", $outfile);
my $rc = open(OFH, '>'.$outfile);
if (!defined($rc)) {
    error($script, "Can't Open file \"$outfile\" for output");
    exit -1;
}

# モジュール単位で処理する
#
for (my $i = 0; $i < @modules; $i++) {

    my $module = $modules[$i];
    verbose("module: $module");
    my $buff = gather_lines(\%modules, $module);
    my @buff = @{$buff};

    # diffマークとスレッド番号を取り除いたものを作る
    #	$buff1 = ({'L'|'R'|'C'|'-'}, 配列要素番号, diffマーク以降の文字列);
    #	$buff2 = ({'L'|'R'|'C'|'-'}, 配列要素番号, スレッド番号以降の文字列);
    #
    my ($buff1, $buff2) = drop_thread_number(@buff);
    my @buff1 = @{$buff1};
    my @buff2 = @{$buff2};

    # 出力フラグを作成する
    #
    my @outflag;
    for (my $i = 0; $i < @buff; $i++) {
	$outflag[$i] = 1;
    }

    # 行の出現順序だけが異なるものは削除する
    #
    my %search = ( 'L' => 'R', 'R' => 'L', '!' => '!', '+' => 'L', '-' => 'R' );
    for (my $i = 0; $i < @buff; $i++) {
	my $j = $i * 3;
	my($mark, $indx, $line) = ($buff1[$j], $buff1[$j+1], $buff1[$j+2]);
	next if $outflag[$i] == 0;
	next if $mark eq '-';
	my $found = find($search{$mark}, $line, $indx, \@buff1);
	if ($found >= 0) {
	    $outflag[$i] = 0;
	    $outflag[$found] = 0;
	}
    }

    # スレッド番号を無視したとき行の出現順序だけが異なるものは削除する
    #
    for (my $i = 0; $i < @buff; $i++) {
	my $j = $i * 3;
	my($mark, $indx, $line) = ($buff2[$j], $buff2[$j+1], $buff2[$j+2]);
	next if $outflag[$i] == 0;
	next if $mark eq '-';
	my $found = find($search{$mark}, $line, $indx, \@buff2);
	if ($found >= 0) 	{
	    $outflag[$i] = 0;
	    $outflag[$found] = 0;
	}
    }

    # ファイルに書き出す
    #
    for (my $i = 0; $i < @buff; $i++) {
	printf OFH "%s\n", $buff[$i] if $outflag[$i] == 1;
    }
}
close OFH;

exit 0;

#-------------------------------------------------------------------------------
#  Subroutine:
#	(\@buff1, \@buff2) = drop_thread_number(@buff);
#
#	@buff		mydiff が出力した差分情報
#	@buff1		diffマークを取り除いた三つ組み情報
#			({'L'|'R'|'C'|'-'}, 配列要素番号, diffマーク以降の文字列);
#	@buff2		スレッド番号まで取り除いた三つ組み情報
#			({'L'|'R'|'C'|'-'}, 配列要素番号, スレッド番号以降の文字列);
#-------------------------------------------------------------------------------
sub drop_thread_number
{
    my @buff = @_;

    my $diff_mark = "<>!+-";
    my @buff1 = ();
    my @buff2 = ();
    for (my $i = 0; $i < @buff; $i++) {
	my $ch = substr($buff[$i], 0, 1);
	my $mark = ($ch eq "<" || $ch eq "-") ? "L" :
		   ($ch eq ">" || $ch eq "+") ? "R" :
		   ($ch eq "!") ? "C" : "-";
	my $len = length($buff[$i]);
	push @buff1, ($mark, $i, substr($buff[$i], ($len < 2) ? 0 : 2));
	my $pos = -1;
	$pos = index(substr($buff[$i], 2), ">") if $len > 2;
	push @buff2, ($mark, $i, substr($buff[$i], ($pos < 0) ? 0 : $pos+3));
    }
    dump_buff(4, @buff1);
    dump_buff(4, @buff2);
    return (\@buff1, \@buff2);
}
#-------------------------------------------------------------------------------
#  Subroutine:
#	$found = find($mark, $line, $indx, \@buff);
#
#	$mark		処理の対象とする行の diff マーク
#	$line		検索文字列
#	$indx		検索文字列の @buff での要素番号
#	@buff		drop_thread_number() が返した三つ組み情報配列
#	$found		>=0: 行番号(見つかったとき), -1: 見つからなかったとき
#-------------------------------------------------------------------------------
sub find
{
    my $mark = shift || assert(0, "argument missing [\$mark]");
    my $line = shift;	# || assert(0, "argument missing [\$line]");
    my $indx = shift || assert(0, "argument missing [\$indx]");
    my $buff = shift || assert(0, "argument missing [\$buff]");
    my @buff = @{$buff};

    debug(2, "find [$line]\n");
    return -1 unless $line;
    for (my $i = 0; $i < @buff; $i += 3) {
	my($tmark, $tindx, $tline) = ($buff[$i], $buff[$i+1], $buff[$i+2]);
	next if $tmark ne $mark;
	debug(3, "try [$tline]\n");
	next if $tindx == $indx;
	next if $tline ne $line;
	debug(2, "FOUND: at line %d: [$line]\n", $tindx);
	return $tindx if $line eq $tline;
    }
    return -1;
}

#-------------------------------------------------------------------------------
#  Subroutine:
#	dump_buff($level, @buff);
#
#	$level		表示デバッグレベル
#	@buff		drop_thread_number() が返した三つ組み情報配列
#-------------------------------------------------------------------------------
sub dump_buff
{
    my $level = shift || assert("argument missing [\$elvel]");
    my @buff = @_;

    return unless $debug >= $level;
    for (my $i = 0; $i < @buff; $i += 3) {
	my $str = sprintf "%s %3d [%s]\n", $buff[$i], $buff[$i+1], $buff[$i+2];
	debug($level, $str);
    }
}

# ======================
#  指定オプションの表示
# ======================
sub show_options
{
    debug(1, "ARGV: %d\n", $#ARGV+1);
    debug(1, "options:\n");
    debug(1, "  -o: $outfilename\n");
    debug(1, "  -h: $help\n");
    debug(1, "  -v: $verbose\n");
    debug(1, "  -D: $debug\n");
}

# ======================
#  使用方法の表示
# ======================
sub usage
{
    my $cmnd = "perl $script [options] [infile]";
    my @args = (
	{ "infile"	=> "input file name" }
    );
    my @opts = (
	{ "-o outfile"	=> "output file name" },
	{ "-h"		=> "show usage" },
	{ "-v"		=> "set verbose mode" },
	{ "-D level"	=> "debug level" }
    );
    show_usage($cmnd, \@args, \@opts);
    exit -1;
}
 
# end: filter.pl

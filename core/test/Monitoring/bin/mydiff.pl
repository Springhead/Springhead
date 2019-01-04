#!/usr/bin/perl
BEGIN { push(@INC, 'bin'); }
# ==============================================================================
#  File:
#	mydiff.pl
#
#  Description:
#	Visual Studio 10.0 (devenv) が出力したビルドログを比較して共通する行を
#	取り除く. 処理はモジュール単位に行なう.
#
#  Version:
#	Ver 1.0  2013/02/06 F.Kanehori	Windows 移植初版 (全面改訂)
#	Ver 1.1  2018/07/03 F.Kanehori	コメント修正.
#	Ver 1.11 2019/01/04 F.Kanehori	Bug fixed.
# ==============================================================================
#use strict;
use warnings;
use Getopt::Long;
use feature ':5.10';
use IO::Handle;
use base_lib;
use dailybuild_lib;

# 定数
#
my $mark_ins = ">";
my $mark_del = "<";
my $mark_chg = "!";
#my $mark_ins_c = "+";
#my $mark_del_c = "-";
#my $mark_chg_c = "!";
my $mark_spc = " ";
my $empty_mark = "_EMPTY_MARK_";
my %cmnd_tab = (
    $mark_ins	=> "a",
    $mark_del	=> "d",
    $mark_chg	=> "c"
);

# 変数
#
my $script = leafname($0);
my $outfile = "-";		# output file

# オプションの定義
#
my $cdiff = 0;
my $patt_id = 0;
my $help = 0;
my $verbose = 0;
my $debug = 0;
my %options = (
    'context'	=> \$cdiff,
    'outfile=s'	=> \$outfile,
    'patt_id=i'	=> \$patt_id,
    'help'	=> \$help,
    'verbose'	=> \$verbose,
    'Debug=i'	=> \$debug
);

# コマンド引数の処理
#
GetOptions(%options);
usage() if $help;
verbose_set($verbose);
debug_push($debug);
if ($#ARGV != 1) { usage() };
my $infile1 = shift || usage();
my $infile2 = shift || usage();

my $outfilename = $outfile eq "-" ? "STDOUT" : $outfile;
if ($verbose) {
    printf STDERR "infile1: %s\n", $infile1;
    printf STDERR "infile2: %s\n", $infile2;
    printf STDERR "outfile: %s\n", $outfilename;
}
show_options() if $debug > 0;

# ファイルを読み込む
#	%modhash: モジュール名をキーとした %threads 情報
#	%threads: スレッド番号をキーとした入力データ行
#	@modname: モジュール名の配列 (出現順)
#	@lines:   入力データ行
#
my($modhash1, $modname1, $lines1) = read_log($infile1, $script, $patt_id);
if (!defined($modhash1) || !defined($modname1) || !defined($lines1)) {
    exit -1;
}
my($modhash2, $modname2, $lines2) = read_log($infile2, $script, $patt_id);
if (!defined($modhash2) || !defined($modname2) || !defined($lines2)) {
    exit -1;
}
my %modules1= %{$modhash1};
my @modules1= @{$modname1};
my @lines1= @{$lines1};
my %modules2= %{$modhash2};
my @modules2= @{$modname2};
my @lines2= @{$lines2};

STDOUT->autoflush(1);

# ファイル1 側とファイル2 側のモジュール名の対応をとる.
#	同じモジュール名が @module1 と @module2 の同じ要素位置に来るように再配置する.
#	@module1 と @module2 の空いた要素（同名のモジュールが相手側にないもの）には
#	    “削除マーク”(@module1側) または“挿入マーク”(@module2側)
#	を記録しておく.
#
my($new_mod1, $new_mod2) = associate_modules(\@modules1, \@modules2, $script, $verbose, $debug);
@modules1 = @{$new_mod1};
@modules2 = @{$new_mod2};

# 出力ファイルをオープンする
#
verbose("opening file \"%s\" for output.", $outfile);
my $rc = open(FH, '>'.$outfile);
if (!defined($rc)) {
    error($script, "can't open file \"$outfile\" for output.");
    exit -1;
}

# 差分をとる
#	処理はモジュール単位で行なう
#
verbose("diff process started.");
my $lineno1 = 1;
my $lineno2 = 1;

for (my $i = 0; $i < @modules1; $i++) {
    debug(1, "START AT (%d,%d)\n", $lineno1, $lineno2);
    my $module1 = $modules1[$i];
    my $module2 = $modules2[$i];

    # どちらか一方のファイルにしかないモジュールの場合
    #
    if ($module1 eq mod_ins()) {
	verbose("inserted  [%s]", $module2);
	output_module_name($module2);
	my $buff = gather_lines(\%modules2, $module2);
	printf FH "%da%d,%d\n", $lineno1, $lineno2, $lineno2 + @{$buff} - 1;
	output(*FH, $buff, $mark_ins);
	$lineno2 += @{$buff};
    }
    elsif ($module2 eq mod_del()) {
	verbose("deleted   [%s]", $module1);
	output_module_name($module1);
	my $buff = gather_lines(\%modules1, $module1);
	printf FH "%d,%dd%d\n", $lineno1, $lineno1 + @{$buff} - 1, $lineno2;
	output(*FH, $buff, $mark_del);
	$lineno1 += @{$buff};
    }
    # 両方のファイルにあるモジュールの場合
    #
    else {
	verbose("take diff [%s]", $module1);
	assert($module1 eq $module2);
	output_module_name($module1);
	my($count1, $count2)
	    = diff(*FH, \%modules1, $module1, $lineno1, \%modules2, $module2, $lineno2);
	debug(1, "--> processed (%d,%d) lines\n", $count1, $count2);
	$lineno1 += $count1;
	$lineno2 += $count2;
    }
} 

# 出力ファイルをクローズする
#
close FH;

verbose("done.");
exit 0;

# ------------------------------------------------------------------------------
#  Subroutine:
#	($count1, $count2)
#	= diff($fh, \%modules1, $module1, $base1, \%modules2, $module2, $base2);
#
#	%modulesN	モジュール名と入力行を対応付けたハッシュテーブル
#	$moduleN	処理対象のモジュール名
#	$baseN		先頭行の行番号
#
#	$countN		%modulesN について処理が済んだ行数
# ------------------------------------------------------------------------------
sub diff
{
    my $fh	 = shift || assert(0, "argument missing [\$fh]");
    my $modules1 = shift || assert(0, "argument missing [\%modules1]");
    my $module1	 = shift || assert(0, "argument missing [\$module1]");
    my $base1	 = shift || assert(0, "argument missing [\$base1]");
    my $modules2 = shift || assert(0, "argument missing [\%modules2]");
    my $module2	 = shift || assert(0, "argument missing [\$module2]");
    my $base2	 = shift || assert(0, "argument missing [\$base2]");

    my $buff1 = gather_lines(\%modules1, $module1);
    my $buff2 = gather_lines(\%modules2, $module2);
    my @buff1 = @{$buff1};
    my @buff2 = @{$buff2};
    my $count1 = 0;
    my $count2 = 0;
    my @del_buff = ();		# $del_buff[$i] = \($lineno1, $lineno2, $line);
    my @ins_buff = ();		# $ins_buff[$i] = \($lineno1, $lineno2, $line);
    my $need_flush = 0;
    object_dump(2, \@buff1, "buff1");
    object_dump(2, \@buff2, "buff2");

    my $line1 = shift @buff1;
    my $line2 = shift @buff2;
    while (defined($line1) || defined($line2)) {
	debug(3, "--------\n");
	debug(3, "Line1: [%s]\n", $line1) if defined($line1);
	debug(3, "Line2: [%s]\n", $line2) if defined($line2);

	if (!defined($line1)) {
	    # @buff2 だけに行がある‐挿入された行
	    my $ins_buff = eat_up(\@buff2, $mark_ins, $base1+$count1, $base2+$count2);
	    $count2 += @{$ins_buff};
	    my $size1 = 0;
	    my $size2 = @{$ins_buff} + 0;
	    debug(1, "*1_I2* (%3d) [%d lines inserted]\n", $base2+$count2, $#{$ins_buff}+1);
	    flush($fh, $mark_ins, $size1, $size2, $dummy_buff, $ins_buff);
	    $need_flush = 0;
	    last;
	}
	elsif (!defined($line2)) {
	    # @buff1 だけに行がある‐削除された行
	    my $del_buff = eat_up(\@buff1, $mark_del, $base1+$count1, $base2+$count2);
	    $count1 += @{$del_buff};
	    my $size1 = @{$del_buff} + 0;
	    my $size2 = 0;
	    debug(1, "*1D_2* (%3d) [%d lines deleted]\n", $base1+$count1, $#{$del_buff}+1);
	    flush($fh, $mark_del, $size1, $size2, $del_buff, $dummy_buff);
	    $need_flush = 0;
	    last;
	}
	elsif ($line1 eq $line2) {
	    # 同じ行
	    my $size1 = @del_buff + 0;
	    my $size2 = @ins_buff + 0;
	    debug(1, "*same* (%3d) %s [%s]\n", $base1+$count1, $mark_spc, $line1);
	    if ($size1 >0 || $size2 > 0) {
		debug(2, "need flush: #(del_buff,ins_buff) = (%d,%d)\n", $size1, $size2);
	    }
	    flush($fh, $mark_chg, $size1, $size2, \@del_buff, \@ins_buff) if $need_flush;
	    $need_flush = 0;
	    $count1++;
	    $count2++;
	    $line1 = shift @buff1;
	    $line2 = shift @buff2;
	    @del_buff = ();
	    @ins_buff = ();
	}
	else {
	    my $line1_in_buff2 = is_in_buff(\@buff2, $line1);
	    my $line2_in_buff1 = is_in_buff(\@buff1, $line2);
	    debug(1, "line1 in buff2: [%s] [%s]\n", $line1_in_buff2 ? "Y" : "N", $line1);
	    debug(1, "line2 in buff1: [%s] [%s]\n", $line2_in_buff1 ? "Y" : "N", $line2);

	    if (!$line1_in_buff2 && $line2_in_buff1) {
		# $line1 が @buff2 になく, かつ $line2 が @buff1 にある － 削除された行
		while (defined($line1) && ($line1 ne $line2)) {
		    push @del_buff, triplet($base1+$count1, $base2+$count2, $line1);
		    debug(1, "*1d_2* (%3d) %s [%s]\n", $base1+$count1, $mark_del, $line1);
		    $count1++;
		    $line1 = shift @buff1;
		}
		$need_flush = 1;
	    }
	    elsif ($line1_in_buff2 && !$line2_in_buff1) {
		# $line1 が @buff2 にあり, かつ $line2 が @buff1 にない － 挿入された行
		while (defined($line2) && ($line2 ne $line1)) {
		    push @ins_buff, triplet($base1+$count1, $base2+$count2, $line2);
		    debug(1, "*1_i2* (%3d) %s [%s]\n", $base2+$count2, $mark_ins, $line2);
		    $count2++;
		    $line2 = shift @buff2;
		}
		$need_flush = 1;
	    }
	    else {
		# $line1 は削除された行で、$line2 は挿入された行
		push @del_buff, triplet($base1+$count1, $base2+$count2, $line1);
		push @ins_buff, triplet($base1+$count1, $base2+$count2, $line2);
		debug(1, "*1d__* (%3d) %s [%s]\n", $base1+$count1, $mark_del, $line1);
		debug(1, "*__i2* (%3d) %s [%s]\n", $base2+$count2, $mark_ins, $line2);
		$count1++;
		$count2++;
		$line1 = shift @buff1;
		$line2 = shift @buff2;
		$need_flush = 1;
	    }
	}
	#sleep(1);
    }
    my $size1 = @del_buff + 0;
    my $size2 = @ins_buff + 0;
    flush($fh, $mark_ins, $size1, $size2, \@del_buff, \@ins_buff);

    return ($count1, $count2);
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	\@outbuff = eat_up(\@inbuff, $mark, $base1, $base2);
#
#	@inbuff		入力データの配列
#	$mark		$mark_del か $mark_ins のいずれか
#	$base1		ファイル1 側に対応する先頭行番号
#	$base2		ファイル2 側に対応する先頭行番号
#
#	@outbuff	処理した三つ組みの配列
# ------------------------------------------------------------------------------
sub eat_up
{
    my $inbuff	= shift || assert(0, "argument missing [\@inbuff]");
    my $mark	= shift || assert(0, "argument missing [\$mark]");
    my $base1	= shift || assert(0, "argument missing [\$base1]");
    my $base2	= shift || assert(0, "argument missing [\$base2]");
    my @inbuff = @{$inbuff};

    my @outbuff = ();
    my $count = 0;
    my $line = shift @inbuff;
    while (defined($line)) {
	if ($mark eq $mark_del) {
	    push @outbuff, triplet($base1+$count, $base2, $line);
	    debug(1, "eat_up: DEL(%d) [%s]\n", $base1+$count, $line);
	}
	else {
	    push @outbuff, triplet($base1, $base2+$count, $line);
	    debug(1, "eat_up: INS(%d) [%s]\n", $base2+$count, $line);
	}
	$count++;
	$line = shift @inbuff;
    }
    return \@outbuff;
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	\@array3 = triplet($lineno1, $lineno2, $line);
#
#	$lineno1	ファイル1 側に対応する行番号
#	$lineno2	ファイル2 側に対応する行番号
#	$line		行データ
#
#	@array3		\($lineno1, $lineno2, $line)
# ------------------------------------------------------------------------------
sub triplet
{
    my $lineno1	= shift || assert(0, "argument missing [\$lineno1]");
    my $lineno2	= shift || assert(0, "argument missing [\$lineno2]");
    my $line	= shift || $empty_mark;		# undef としないため

    my @array3 = ();
    push @array3, $lineno1;
    push @array3, $lineno2;
    push @array3, $line;

    return \@array3;
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	flush($fh, $mark, $size1, $size2, \@buff1, \@buff2);
#
#	$fh		出力ファイルハンドル
#	$mark		行編集コマンドテーブルのキー
#	$sizeN		@buffN の要素数
#	@buffN		出力行データの配列
# ------------------------------------------------------------------------------
sub flush
{
    my $fh    = shift || assert(0, "argument missing [\*fh]");
    my $mark  = shift || assert(0, "argument missing [\$mark]");
    my $size1 = shift || 0;
    my $size2 = shift || 0;
    my $buff1 = shift;
    my $buff2 = shift;

    my @tmp1 = @{$buff1} if $buff1;
    my @tmp2 = @{$buff2} if $buff2;
    assert(($size1 == @tmp1), sprintf "buff1 size differ (%d)-(%d).", $size1, @tmp1+0);
    assert(($size2 == @tmp2), sprintf "buff2 size differ (%d)-(%d).", $size2, @tmp2+0);

    if ($size1 == 0 || $size2 == 0) {
	return if $size1 == 0 && $size2 == 0;
	flush1($fh, $mark_del, $buff1) unless $size1 == 0;
	flush1($fh, $mark_ins, $buff2) unless $size2 == 0;
	return;
    }
    my($min11, $max11, $min21, $max21, $lines1) = min_max($buff1);
    my($min12, $max12, $min22, $max22, $lines2) = min_max($buff2);
    printf $fh edit_command($min11, $max11, $min22, $max22, $cmnd_tab{$mark});
    my @lines1 = @{$lines1};
    my @lines2 = @{$lines2};
    output($fh, \@lines1, $mark_del);
    printf $fh "--\n";
    output($fh, \@lines2, $mark_ins);
    debug(1, "flush: DEL (%3d,%3d)-(%3d,%3d), INS (%3d,%3d)-(%3d,%3d)\n",
	    $min11, $max11, $min21, $max21, $min12, $max12, $min22, $max22);
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	flush1($fh, $mark, \@buff);
#
#	$fh		出力ファイルハンドル
#	$mark		行編集コマンド
#	@line		出力行データの配列
# ------------------------------------------------------------------------------
sub flush1
{
    my $fh   = shift || assert(0, "argument missing [\*fh]");
    my $mark = shift || assert(0, "argument missing [\$mark]");
    my $buff = shift || assert(0, "argument missing [\$buff]");
    return if @{$buff} == 0;

    my($min1, $max1, $min2, $max2, $lines) = min_max($buff);
    printf $fh edit_command($min1, $max1, $min2, $max2, $cmnd_tab{$mark});
    my @lines = @{$lines};
    output($fh, \@lines, $mark);
    debug(1, "flush: (%3d,%3d) - (%3d,%3d)\n", $min1, $max1, $min2, $max2);
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	($min1, $max1, $min2, $max2, \@lines) = min_max(\@buff);
#
#	@buff		三つ組みデータの配列
#
#	$min1,$max1	ファイル1 に対応する行番号の最小値と最大値
#	$min2,$max2	ファイル2 に対応する行番号の最小値と最大値
#	@lines		出力行データの配列
# ------------------------------------------------------------------------------
sub min_max
{
    my $buff = shift || assert(0, "argument missing [\$buff]");
    my @buff = @{$buff};

    my $min1 = 9999;
    my $max1 = 0;
    my $min2 = 9999;
    my $max2 = 0;
    my @lines = ();
    for (my $i = 0; $i < @buff; $i++) {
	my $element = $buff[$i];
	my($lineno1, $lineno2, $line) = @{$element};
	$min1 = $lineno1 if $lineno1 < $min1;
	$max1 = $lineno1 if $lineno1 > $max1;
	$min2 = $lineno2 if $lineno2 < $min2;
	$max2 = $lineno2 if $lineno2 > $max2;
	push @lines, $line;
    }

    return ($min1, $max1, $min2, $max2, \@lines);
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	$str = edit_command($min1, $max1, $min2, $max2, $cmnd);
#
#	$min1,$max1	ファイル1 に対応する行番号の最小値と最大値
#	$min2,$max2	ファイル2 に対応する行番号の最小値と最大値
#	$cmnd		編集コマンド
#
#	$str		編集コマンド（行番号指定を含む）
# ------------------------------------------------------------------------------
sub edit_command
{
    my $min1 = shift || assert(0, "argument missing [\$min1]");
    my $max1 = shift || assert(0, "argument missing [\$max1]");
    my $min2 = shift || assert(0, "argument missing [\$min2]");
    my $max2 = shift || assert(0, "argument missing [\$max2]");
    my $cmnd = shift || assert(0, "argument missing [\$cmnd]");

    my $fmt1 = ($min1 == $max1) ? "%d" : "%d,%d";
    my $fmt2 = ($min2 == $max2) ? "%d" : "%d,%d";
    my $str1 = sprintf $fmt1, $min1, $max1;
    my $str2 = sprintf $fmt2, $min2, $max2;

    return $str1.$cmnd.$str2."\n";
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	output_module_name($fh, $module);
#
#	$fh		出力ファイルハンドル
#	$module		モジュール名
# ------------------------------------------------------------------------------
sub output_module_name
{
    my $module = shift || assert(0, "argument missing [\$module]");

    return if $module eq "_top_level_";
    printf FH "\n[ $module ]\n";
    STDOUT->autoflush(1);
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	output($fh, \@buff, $mark);
#
#	$fh		出力ファイルハンドル
#	@buff		出力行データ
#	$mark		挿入・削除・置換表示文字
# ------------------------------------------------------------------------------
sub output
{
    my $fh   = shift || assert(0, "argument missing [\*fh]");
    my $buff = shift || assert(0, "argument missing [\$buff]");
    my $mark = shift || assert(0, "argument missing [\$mark]");
    my @buff = @{$buff};

    for (my $i = 0; $i < @buff; $i++) {
	$buff[$i] = "" if $buff[$i] eq $empty_mark;
	printf $fh "%s %s\n", $mark, $buff[$i];
    }
    STDOUT->autoflush(1);
}

# ----------------------
#  指定オプションの表示
# ----------------------
sub show_options
{
    debug(1, "ARGV: %d\n", $#ARGV+1);
    debug(1, "options:\n");
    debug(1, "  -c: $cdiff\n"),
    debug(1, "  -o: $outfilename\n");
    debug(1, "  -v: $verbose;\n");
    debug(1, "  -D: $debug;\n");
}

# ----------------------
#  使用方法の表示
# ----------------------
sub usage
{
    my $cmnd = "perl $script [options] infile-1 infile-2";
    my @args = ();
    my @opts = (
	{ "-o outfile"	=> "output file name (default is stdout)" },
	{ "-c"		=> "output like context diff(*NOT IMPLEMENTED*)" },
	{ "-p patt_id"	=> "module identifying pattern number" },
	{ "-h"		=> "show usage" },
    	{ "-v"		=> "set verbose mode" },
	{ "-D level"	=> "debug level"}
    );
    show_usage($cmnd, \@args, \@opts);
    exit -1;
}
 
# end: mydiff.pl

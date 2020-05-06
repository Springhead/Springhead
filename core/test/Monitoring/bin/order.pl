#!/usr/bin/perl
BEGIN { push(@INC, 'bin'); }
# ==============================================================================
#  File:
#	order.pl
#
#  Description:
#	Visual Studio 10.0 (devenv) が出力したログを, モジュール単位かつ
#	スレッド順に並べ直す.
#
#  Version:
#	Ver 1.0  2013/02/06 F.Kanehori	Windows 移植初版 (全面改訂)
# ==============================================================================
use strict;
use warnings;
use Getopt::Long;
use base_lib;
use dailybuild_lib;

# 変数
#
my $script = leafname($0);
my $logfile = "-";		# input file
my $outfile = "-";		# output file

# オプションの定義
#
my $patt_id = 0;
my $help = 0;
my $verbose = 0;
my $debug = 0;
my %options = (
    'help'	=> \$help,
    'outfile=s'	=> \$outfile,
    'patt_id=i'	=> \$patt_id,
    'verbose'	=> \$verbose,
    'Debug=i'	=> \$debug
);

# コマンド引数の処理
#
GetOptions(%options);
usage() if $help;
verbose_set($verbose);
debug_push($debug);
if ($#ARGV == 0) { $logfile = $ARGV[0] };
if ($#ARGV >  0) { usage() };

my $logfilename = $logfile eq "-" ? "STDIN"  : $logfile;
my $outfilename = $outfile eq "-" ? "STDOUT" : $outfile;
verbose("logfile: %s", $logfilename);
verbose("outfile: %s", $outfilename);
show_options() if $debug > 0;

# ファイルを読み込む
#	%modules: モジュール名をキーとした %threads 情報
#	%threads: スレッド番号をキーとした入力データ行
#	@modules: モジュール名の配列 (出現順)
#	@lines:   入力データ行
#
my ($modhash, $modname, $lines) = read_log($logfile, $script, $patt_id);
if (!defined($modhash) || !defined($modname) || !defined($lines)) {
    exit -1;
}
my %modules = %{$modhash};
my @modules = @{$modname};
my @lines = @{$lines};

# ファイルに書き出す
#
debug(1, "opening file \"%s\" for output\n", $outfile);
my $rc = open(OFH, '>'.$outfile);
if (!defined($rc)) {
    error($script, "Can't Open file \"$outfile\" for output");
    exit -1;
}

STDOUT->autoflush(1);

for (my $i = 0; $i < @modules; $i++) {
    my $m = $modules[$i];
    my %threads = %{$modules{$m}};
    foreach my $t (sort { $a <=> $b } keys %threads) {
	my @data = @{$threads{$t}};
	for (my $j = 0; $j < @data; $j++) {
	    printf OFH "%s\n", $data[$j];
	}
    }
}
close OFH;

exit 0;

# ======================
#  指定オプションの表示
# ======================
sub show_options
{
    debug(1, "ARGV: %d\n", $#ARGV+1);
    debug(1, "options:\n");
    debug(1, "  -o: $outfilename\n");
    debug(1, "  -p: $patt_id\n");
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
	{ "infile"	=> "input file name (default is stdin)" }
    );
    my @opts = (
	{ "-o outfile"	=> "output file name (default is stdout)" },
	{ "-p patt_id"	=> "module identifying pattern number" },
	{ "-h"		=> "show usage" },
	{ "-v"		=> "set verbose mode" },
	{ "-D level"	=> "debug level" }
    );
    show_usage($cmnd, \@args, \@opts);
    exit -1;
}
 
# end: order.pl

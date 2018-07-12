package base_lib;
# ==============================================================================
#  File:
#	base_lib.pm
#
#  Description:
#	Perl 開発で共通に使用するサブルーチンのライブラリ
#
#  Version:
#	Ver 1.0  2013/02/01 F.Kanehori	初版
#	Ver 1.01 2018/07/12 F.Kanehori	コメント修正
# ==============================================================================
use base 'Exporter';
@EXPORT = qw(assert verbose verbose_set show_usage
	     dirname leafname
	     warning error fatal panic
	     debug debug_push debug_pop object_dump);
@EXPORT_OK = qw();
use Carp;

# ------------------------------------------------------------------------------
#  Subroutine:
#	assert($expr);
#
#	$expr		アサート条件
# ------------------------------------------------------------------------------
sub assert { Carp::confess("assertion failed") unless shift; }

# ------------------------------------------------------------------------------
#  Subroutine:
#	verbose($text [,$stream]);
#	verbose_set($value);
#
#	$text		表示文字列
#	$stream		出力ストリーム
#	$value		1=on or 0=off
# ------------------------------------------------------------------------------
my $verbose = 0;
sub verbose
{
    return unless $verbose;
    my @text = @_;
    if (@text[0] ne STDOUT) {
	unshift @text, STDERR;
    }
    my $stream = shift @text;
    printf $stream @text;
    printf $stream "\n";

}
sub verbose_set { $verbose = shift; }

# ------------------------------------------------------------------------------
#  Subroutine:
#	show_usage($cmnd [,\@args [,\@opts]]);
#
#	$cmnd		コマンドライン
#	@args		コマンド引数
#	@opts		オプション引数
#			※ @args/@opts の各要素 = { "arg" => "description" }
# ------------------------------------------------------------------------------
sub show_usage
{
    my $cmnd = shift || assert("argument missing [\$cmnd]");
    my $args = shift;
    my $opts = shift;

    printf STDERR "Usage: $cmnd\n";
    if ($args) {
	my @args = @{$args};
	for (my $i = 0; $i < @args; $i++) {
	    my %elem = %{$args[$i]};
	    my @keys = keys %elem;
	    my $desc = add_padding("$keys[0]:", 16);
	    printf STDERR "       $desc\t$elem{$keys[0]}\n";
	}
    }
    if ($opts) {
	printf STDERR "       options:\n";
	my @opts = @{$opts};
	for (my $i = 0; $i < @opts; $i++) {
	    my %elem = %{$opts[$i]};
	    my @keys = keys %elem;
	    my $desc = add_padding("$keys[0]:", 7);
	    printf STDERR "         $desc\t$elem{$keys[0]}\n";
	}
    }
}
sub add_padding
{
    my $str = shift;
    my $wid = shift;

    return $str . ((length($str) < $wid) ? "\t" : "");
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	warning($prog, @text);
#	error($prog, @text);
#	fatal($prog, @text);
#	panic($prog, @text);
#
#	$prog		プログラム名
#	@text		表示文字列
# ------------------------------------------------------------------------------
sub warning { _error("Warning", 0, @_); }
sub error { _error("Error", 0, @_); }
sub fatal { _error("Fatal", -1, @_); }
sub panic {
    my $prog = shift;
    my $text = sprintf shift, @_;

    Carp::croak(sprintf "%sPanic: %s", ($prog ? "$prog: " : ""), $text);
}

sub _error
{
    my $kind = shift || assert(0, "argument missing [\$kind]");
    my $exit = shift || 0;
    my $prog = shift || "";
    my @text = @_;

    printf STDERR "$prog: " if $prog;
    printf STDERR "$kind: ";
    printf STDERR @text;
    printf STDERR "\n";
    exit $exit if $exit;
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	$name = dirname($path);
#
#	$path		パス名
#	$name		ディレクトリ名
# ------------------------------------------------------------------------------
sub dirname
{
    my $name = shift || "";
    $name =~ s/(.*)\\[_0-9a-zA-Z\.]*$/$1/;
    return $name;
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	$name = leafname($path);
#
#	$path		パス名
#	$name		リーフ名
# ------------------------------------------------------------------------------
sub leafname
{
    my $name = shift || "";
    $name =~ s/.*\\([_0-9a-zA-Z\.]*)$/$1/;
    return $name;
}

# ------------------------------------------------------------------------------
#  Subroutine:
#	debug($level, @text);
#	debug_push($value);
#	debug_pop();
#
#	$level		表示レベル
#	@text		出力フォーマット＆テキスト
#	$value		デバッグレベル
# ------------------------------------------------------------------------------
my @debug = ( 0 );
sub debug
{
    my $level = shift || 1;
    my @text  = @_;
    return unless $level <= $debug[@debug-1];

    my $indent = "";
    for (my $i = 0; $i < $level; $i++) { $indent .= "  "; }
    printf STDERR "%s", $indent;
    printf STDERR @_
}
sub debug_push { push @debug, shift; }
sub debug_pop  { pop @debug if @debug > 1; }

# ------------------------------------------------------------------------------
#  Subroutine:
#	object_dump($level, \$object [,$text]);
#
#	$object		ダンプするオブジェクトの参照 (\$, \@, \%, ..)
# ------------------------------------------------------------------------------
sub object_dump
{
    my $level  = shift || 0;
    my $object = shift;
    my $text   = shift || "";

    my $type = ref($object);
    if ($type eq ARRAY) {
	# オブジェクトは配列
	my @object = @{$object};
	debug($level, "%s:\n", $text);
	for (my $i = 0; $i < @object; $i++) {
	    debug($level, "  (%d) %s\n", $i+1, $object[$i]);
	}
    }
    elsif ($type eq HASH) {
	# オブジェクトはハッシュ
	my %object = %{$object};
	debug($level, "%s:\n", $text);
	foreach $k (sort keys %object) {
	    debug($level, "  %s => %s\n", $k, $object{$k});
	}
    }
    else {
	# オブジェクトはスカラー
	debug($level, "%s: %s\n", $text, $$object);
    }
}

1;
# end: base_lib.pm

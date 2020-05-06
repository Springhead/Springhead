<?php
//
//  1. Generate daily/samples build result table.
//	Table is sorted according to the number of successfully build modules
//	and revision number by discending order.
//
//  History:
//	2014/02/19 F.Kanehori	Line order of 'result.log' file has changed.
//	2013/10/28 F.Kanehori	Include today's (current version) information.
//	2013/06/26 F.Kanehori	Change default sort order to by revision.
//	2013/05/29 F.Kanehori	Adds sort order selection facility.
//	2013/05/02 F.Kanehori	Now both dailybuild and Samples available.
//	2013/04/01 F.Kanehori
//

function build_history_get_options()
{
	static $options = array(
		'cookie_usage'	=> 1,	// set 1 if cookies are available
		'delayed_jump'	=> 0,	// set 1 to debug page befor redirect
	);
	return $options;
}

function plugin_build_history_action()
{
	global $vars;

	$options = build_history_get_options();
	if ($options['cookie_usage'] == 0) {
		// Parameters are get from query string.
		//
		$base = isset($vars["base"]) ? $vars["base"] : "";
		$file = isset($vars["file"]) ? $vars["file"] : "";
		$type = isset($vars["type"]) ? $vars["type"] : "1";
		$span = isset($vars["span"]) ? $vars["span"] : "1";
		$unit = isset($vars["unit"]) ? $vars["unit"] : "month";
		$sort = isset($vars["sort"]) ? $vars["sort"] : "revision";
		$referer = isset($vars["referer"]) ? $vars["referer"] : "";
	}
	else {
		// Parameters are get from cookie.
		//
		$base = isset($_COOKIE["sbh_b"]) ? $_COOKIE["sbh_b"] : "";
		$file = isset($_COOKIE["sbh_f"]) ? $_COOKIE["sbh_f"] : "";
		$referer = isset($_COOKIE["sbh_r"]) ? $_COOKIE["sbh_r"] : "";

		// Rest parameters are get from query string.
		//
		$type = isset($vars["type"]) ? $vars["type"] : "1";
		$span = isset($vars["span"]) ? $vars["span"] : "1";
		$unit = isset($vars["unit"]) ? $vars["unit"] : "month";
		$sort = isset($vars["sort"]) ? $vars["sort"] : "revision";
	}


	if (!strcmp($base, "") || !strcmp($file, "")) {
		//
		$body = "Argument missing (\$base or \$file).";
		return array("msg" => $pagetitle, "body" => $body);
	}
	$base = urldecode($base);
	$file = urldecode($file);
	$referer = urldecode($referer);

	// <title>
	//
	$pagetitle = (($type == 1) ? "今朝" : "Samples") . "のビルドの履歴";

	// <body>
	//
	$text = "<div id='preview'><h2 id='content_1_0'>過去のビルドの履歴</h2></div>";

	// (1) display range selection link
	//
	$ot= "<tr><td>";
	$ct = "</td></tr>";
	$links = "<table border='0' cellspacing='4'>"
		. "<tr><td>"
		.	build_history_make_link($base, $file, $type, 1, "month", $sort, $referer, $span)
		.     "</td>"
		.     "<td width='50'>&nbsp;</td>"
		.     "<td>"
		.	build_history_make_link2($base, $file, $type, $span, $unit, "revision", $referer, $sort)
		.     "</td>"
		. "</tr>"
		. "<tr><td>"
		.	build_history_make_link($base, $file, $type, 3, "month", $sort, $referer, $span)
		.     "</td>"
		.     "<td></td>"
		.     "<td>"
		.	build_history_make_link2($base, $file, $type, $span, $unit, "succ", $referer, $sort)
		.     "</td>"
		. "</tr>"
		. "<tr><td>"
		.	build_history_make_link($base, $file, $type, 6, "month", $sort, $referer, $span)
		.     "</td>"
		.     "<td></td>"
		.     "<td></td>"
		. "</table>";

	// (2) history table
	//
	$table = "<hr>"
		. build_history_make_table($base, $file, $type, $span, $unit, $sort);

	// (3) reference link
	//
	$enc_referer = urlencode(htmlspecialchars($referer));
	$disp_referer = split("\?", $referer);
	$trailer = "<br>"
		. "<font size='-1'>関連ページ</font><br>&nbsp;&nbsp;"
		. "<a href='$referer'>$disp_referer[1]</a>";

	// Construct whole page.
	//
	$body = $text . $links . $table . $trailer;
	return array("msg" => "$pagetitle", "body" => $body);
}

function build_history_make_uri($base, $file, $type, $span, $unit, $sort, $referer)
{
	$options = build_history_get_options();

	// Make uri string from given arguments.
	//
	$site = "http://springhead.info/wiki/index.php";
	$cmnd = "?cmd=build_history";
	$opts = "";
	if ($options['cookie_usage'] == 0) {
		$opts = "&base=$base&file=$file&referer=$referer";
	}
	$opts .= "&type=$type&span=$span&unit=$unit&sort=$sort";
	return $site . $cmnd . $opts;
}

function build_history_make_link($base, $file, $type, $span, $unit, $sort, $referer, $curr_span)
{
	// Make link tag for time span in html.
	//
	$base = urlencode(htmlspecialchars($base));
	$file = urlencode(htmlspecialchars($file));
	$referer = urlencode(htmlspecialchars($referer));
	$uri = build_history_make_uri($base, $file, $type, $span, $unit, $sort, $referer);

	if ($span == $curr_span) {
		$link =  "<p><font color='gray'>過去 $span"
			. build_history_display_unit($unit)
			. " の履歴</font></p>";
	}
	else {
		$link =  "<p><a href='$uri'>過去 $span"
			. build_history_display_unit($unit)
			. " の履歴</a></p>";
	}

	return $link;
}

function build_history_make_link2($base, $file, $type, $span, $unit, $sort, $referer, $curr_sort)
{
	// Make link tag for sort order in html.
	//
	$base = urlencode(htmlspecialchars($base));
	$file = urlencode(htmlspecialchars($file));
	$referer = urlencode(htmlspecialchars($referer));
	$uri = build_history_make_uri($base, $file, $type, $span, $unit, $sort, $referer);

	$str = (!strcmp($sort, "succ")) ? "成功モジュール数" : "レビジョン";
	if (!strcmp($sort, $curr_sort)) {
		$link =  "<p><font color='gray'>" . $str . "順</font></p>";
	}
	else {
		$link =  "<p><a href='$uri'>" .$str . "順</a></p>";
	}

	return $link;
}

function build_history_display_unit($unit)
{
	// Make string displayed in link tag.
	//
	$unit_t = array(
		"month" => "ヶ月",
		"week"  => "週間",
		"day"   => "日"
	);
	return array_key_exists($unit, $unit_t) ? $unit_t[$unit] : $unit_t["month"];
}

function build_history_make_table($base, $file, $type, $span, $unit, $sort)
{
	// Make history table (in html).
	//
	//  $base:	Directory name where the file is.
	//  $file:	Name of hitory log file.
	//  $span:	Time span in $unit.
	//  $unit:	Time unit: "month", "week" or "day"

	// Set display date range.
	//
	$epoch = 6364;		// Epoch: when samples are added.
	$span_str = "-$span $unit";
	$limit = date("Y-m-d", strtotime($span_str));
				// Check until this date.

	// Generate table caption.
	//
	$pattern = '/-([0-9]+ )/';
	preg_match($pattern, "-$span $unit", $matches);
	$last = $matches[1];
	$disp_name_1 = (($type == 1) ? "今朝" : "Sanples ") . "のビルドの履歴";
	$disp_name_2 = " （"
		     . $last . build_history_display_unit($unit)
		     . "分）";

	$tag_1 = "<ul class='list1' style='padding-left:16px;margin-left:16px'><li>";
	$tag_2 = "</li></ul>";
	$caption = $tag_1 . $disp_name_1 . $disp_name_2 . $targ_2;
	$caption .= "<table height='5' border='0'><tr></tr></table>";

	// Read currunt version information.
	//
	$head = build_history_get_head_info();

	// Read content of history file.
	//
	$cmnd = "cat $base/$file | nkf -w";
	exec($cmnd, $histories);

	// Merge above two infomations.
	//
	for ($i = 0; $i < 4; $i++) {
		$lines[$i] = $head[$i];
	}
	for ($i = 0; $i < count($histories); $i++) {
		$lines[$i+4] = $histories[$i];
	}

	// Analyze the content.
	//
	$state = 0;
	for ($i = 0; $i < count($lines); $i++) {
		$line = $lines[$i];
		if (preg_match('/^--------/', $line)) {
			$state = 1;
		}
		else if ($state == 1 && preg_match('/^r[0-9]{4}/', $line)) {
			$pattern = '/^r([0-9]{4}).+\| ([0-9]{4}-[0-9]{2}-[0-9]{2}) /';
			preg_match($pattern, $line, $matches);
			$revision = $matches[1];
			$date = $matches[2];
			if (strcmp($date, $limit) <= 0 || $revision < $epoch) {
				break;
			}
			$state = 2;
		}
		else if ($state == 2 && preg_match('/^Autobuild done\./', $line)) {
			$history[$revision] = $date;
			$state = 0;
		}
	}

	// Get information of each revision.
	//   temporary data format:
	//	 0- 3	revision number
	//	 4-13	date of build [yyyy-mm-dd]
	//	14-16	number of modules successfully built
	//	17-19	number of modules having build error
	//	20-	success-module-names
	//
	foreach ($history as $revision => $date) {
		$tmp = build_history_get_one($revision, $date, $type);
		if (!strcmp($sort, "revision")) {
			$key = substr($tmp, 0, 4);
		}
		else {
			$key = substr($tmp, 14, 3) . substr($tmp, 0, 4);
		}
		$modules[$key] = $tmp;
	}
	krsort($modules);

	// Some table attributes.
	//
	$db_t = "border-style: none none dashed none;";
	$bd_m = "border-style: none none none none";
	$wd_e = "width: 1.6em;";		// left margin
	$wd_r = "width: 10%;";			// width: revision
	$wd_d = "width: 12%;";			// width: date
	$wd_s = "width: 10%;";			// width: #success
	$wd_f = "width: 10%;";			// width: #failure
	$wd_x = "width: 57%;";			// width: success module names
	$lo_f = "table-layout: fixed;";
	$ta_c = "text-align: center;";
	$ta_l = "text-align: left;";
	$ta_m = "vertical-align: middle;";
	$bc_h = "background: #98fb98;";		// for header row
	$bc_e = "background: #afeeee;";		// for even row
	$bc_o = "background: #f5f5dc;";		// for odd row
	$bold = "font-weight: bold;";

	$bc_e = "background: #e8e8e8;";		// for even row
	$bc_o = "background: #ffffff;";		// for odd row

	$style[0] = array(			// for even row
		"c" => "$ta_c $ta_m $bc_e",
		"l" => "$ta_l $ta_m $bc_e"
	);
	$style[1] = array(			// for odd row
		"c" => "$ta_c $ta_m $bc_o",
		"l" => "$ta_l $ta_m $bc_o"
	);

	// Generate table source code.
	//
	$kind = ($type == 1) ? "実行" : "ビルド";
	$table .= "<table width='90%' border='0' cellpadding='3' cellspacing='0'>"
		."<tr>"
		.  "<td style='$bd_t $wd_e'></td>"
		.  "<td>"
		.    "<table width='100%' border='1' rules='hidden' cellpadding='3' cellspacing='0'>"
		.      "<tr>"
		.        "<td style='$bd_m $lo_f $wd_r $ta_c $ta_m $bc_h $bold'>レビジョン</td>"
		.        "<td style='$bd_m $lo_f $wd_d $ta_c $ta_m $bc_h $bold'>日付</td>"
		.        "<td style='$bd_m $lo_f $wd_s $ta_c $ta_m $bc_h $bold'>ビルド<br>成功数</td>"
		.        "<td style='$bd_m $lo_f $wd_f $ta_c $ta_m $bc_h $bold'>ビルド<br>失敗数</td>"
		.        "<td style='$bd_m $lo_f $wd_x $ta_c $ta_m $bc_h $bold'>"
		.	   $kind . "<br>成功モジュール</td>"
		.      "</tr>";

	$sw = 1;
	foreach ($modules as $key => $val) {
		$mod_names = join("; ", build_history_make_array(substr($val, 20)));
		if (strcmp($mod_names, $mod_names_prev)) {
			$mod_names_prev = $mod_names;
			$val = substr($val, 0, 20) . $mod_names;
		}
		else {
			$val = substr($val, 0, 20) . "ditto";
		}
		$table .= build_history_edit_one($val, $style[$sw]);
		$sw = 1 - $sw;
	}

	$table .=    "</table>"
		.  "</td>"
		."</tr>"
		."</table>";

	return $caption . $table;
}

function build_history_get_head_info()
{
	$url = build_history_get_repository_url();
	$cmnd = "svn -r HEAD log $url | nkf -w";
	exec($cmnd, $output);

	return $output;
}

function build_history_get_one($revision, $date, $type)
{
	// Get the log of specified revision from svn.
	//
	$url = build_history_get_repository_url() . "/log/result.log";
	$cmnd = "svn cat -r $revision $url | nkf -w";
	exec($cmnd, $output);

	// Build result is ..
	//
	$b_succ_1 = 1;					// src/tests
	$b_fail_1 = ($revision <= 7031) ? 3 : 2;
	$r_succ_1 = ($revision <= 7031) ? 2 : 3;
	$r_fail_1 = 4;
	$b_succ_2 = 7;					// src/Samples
	$b_fail_2 = ($revision <= 7031) ? 9 : 8;
	$r_succ_2 = ($revision <= 7031) ? 8 : 9;
	$r_fail_2 = 10;

	// count
	$line_s = ($type == 1) ? $output[$b_succ_1] : $output[$b_succ_2];
	$line_f = ($type == 1) ? $output[$b_fail_1] : $output[$b_fail_2];
	preg_match('/.+\((.+)\)/', $line_s, $matches_s);
	preg_match('/.+\((.+)\)/', $line_f, $matches_f);
	$mods_s = split(",", $matches_s[1]);
	$mods_f = split(",", $matches_f[1]);
	$count_s = ($matches_s[0] == "") ? 0 : count($mods_s);
	$count_f = ($matches_f[0] == "") ? 0 : count($mods_f);
	// names
	$line_s = ($type == 1) ? $output[$r_succ_1] : $output[$b_succ_2];
	preg_match('/.+\((.+)\)/', $line_s, $matches_s);
	$mods_s = split(",", $matches_s[1]);
	$mod_names = implode(", ", $mods_s);

	//	 0- 3	revision number
	//	 4-13	date of build [yyyy-mm-dd]
	//	14-16	number of modules successfully built
	//	17-19	number of modules having build error
	//	20-	success-module-names
	
	$text = sprintf("%4s%10s%3s%3s%s",
			$revision, $date, $count_s, $count_f, $mod_names);
	return $text;
}

function build_history_get_repository_url()
{
	return "http://springhead.info/spr2/Springhead2/trunk/test";
}

function build_history_edit_one($data, $style)
{
	// Log editor for one revision.
	//
	$revision  = substr($data, 0, 4);
	$date      = substr($data, 4, 10);
	$count_s   = substr($data, 14, 3);
	$count_f   = substr($data, 17, 3);
	$mod_names = substr($data, 20);

	$style_c = $style["c"];
	$style_l = $style["l"];
	$row = "<tr>"
	     .   "<td style='$style_c'>$revision</td>"
	     .   "<td style='$style_c'>$date</td>"
	     .   "<td style='$style_c'>$count_s</td>"
	     .   "<td style='$style_c'>$count_f</td>"
	     .   "<td style='$style_l font-size: 80%'>"
	     .     build_history_make_inner_table($mod_names)
	     .   "</td>"
	     . "</tr>";

	return $row;
}

function build_history_make_array($data)
{
	// Convert string to array as follows.
	//	"lib1:mod1,lib2:mod2,…,libN:modN,…"
	//	==> array("lib1:mod11,mod12,…", …, "libN:modN1,modN2,…", …)
	//
	if ($data == "") {
		return array();
	}
	$i_ary = split(",", $data);
	$once = 1;
	for ($i = 0; $i < count($i_ary); $i++) {
		$t_ary = split(":", $i_ary[$i]);
		$t_libname = trim($t_ary[0]);
		$t_modname = trim($t_ary[1]);
		if (strcmp($t_libname, $libname)) {
			if ($once == 0) {
				$o_ary[] = "$libname:$modname";
			}
			$libname = $t_libname;
			$modname = $t_modname;
			$once = 0;
		}
		else {
			$modname .= ", $t_modname";
		}
	}
	$o_ary[] = "$t_libname:$modname";

	return $o_ary;
}

function build_history_make_inner_table($data)
{
	// Make inner table for 'build success modules'.
	//
	$style_t = "table-layout: fixed;";
	$style_i = "width: 2%";		// indent
	$style_b = "width: 55%";	// body

	$table = "<table border='0' style='$style_t' cellpadding='0' cellspacing='0'>";

	$items = split(";", $data);
	for ($i = 0; $i < count($items); $i++) {
		$item = split(":", $items[$i]);
		if (!strcmp($item[0], "ditto")) {
			$table .= "<tr>"
				.   "<td style='$style_i' colspan='2'>$item[0]</td>"
				. "</tr>";
		}
		else {
			$table .= "<tr>"
				.   "<td style='$style_i' colspan='2'>$item[0]:</td>"
				. "</tr>"
				. "<tr>"
				.   "<td style='$style_i'>&nbsp;</td>"
				.   "<td style='$style_b'>$item[1]</td>"
				. "</tr>";
		}
	}

	$table .= "</table>";

	return $table;
}

function plugin_build_history_convert()
{
	global $_SERVER;

	$options = build_history_get_options();
	if ($options['cookie_usage'] == 0) {
		// Set referer to the query, then redirect to the url given by the argument.
		//
		list($redirect_to) = func_get_args();
		$redirect_to = build_history_urlencode($redirect_to);
		$redirect_to .= "&referer=" . urlencode($_SERVER['HTTP_REFERER']);
	}
	else {
		// Set 'span' and 'unit' to the query and rest to the cookie,
		// then redirect to the url given by 'cmd'.
		//
		list($arg) = func_get_args();
		$args = split("\?", $arg);

		$queries = build_history_query_to_array($args[1]);
		$cmd  = isset($queries["cmd"]) ? $queries["cmd"] : "devel";
		$base = isset($queries["base"]) ? $queries["base"] : "";
		$file = isset($queries["file"]) ? $queries["file"] : "";
		$type = isset($queries["type"]) ? $queries["type"] : "1";
		$span = isset($queries["span"]) ? $queries["span"] : "1";
		$unit = isset($queries["unit"]) ? $queries["unit"] : "month";
		$sort = isset($queries["sort"]) ? $queries["sort"] : "succ";
		$referer = $_SERVER['HTTP_REFERER'];

		setcookie("sbh_b", $base);
		setcookie("sbh_f", $file);
		setcookie("sbh_r", $referer);

		$redirect_to = "$args[0]?cmd=$cmd";
		$redirect_to .= "&type=$type&span=$span&unit=$unit&sort=$sort";
	}

	// ------------------------------
	//  Hook for editting this page.
	// ------------------------------
	if ($options['delayed_jump'] == 0) {
		header("Location: $redirect_to");
		$result = "";
	}
	else {
		$msec = 5000;
		$result = <<<EOD
<script type='text/javascript'>
<!--
setTimeout("link()", $msec);
function link() {
location.href='$redirect_to';
}
-->
</script>
EOD;
	}
	return $result;
}

function build_history_urlencode($uri)
{
	// Urlencode the uri given by the argument.
	//
	$result = "";

	// -- scheme --
 	$part1 = split(":", $uri);
	if (count($part1) > 1) {
		$result = urlencode(htmlspecialchars($part1[0])) . ":";
		$part1[0] = $part1[1];
	}

	// -- path --
 	$part2 = split("\?", $part1[0]);
	$result .= build_history_urlencode_1($part2[0], "/", "");

	// -- query --
	if (count($part2) > 1) {
		$result .= "?" . build_history_urlencode_1($part2[1], "&", "=");
	}

	return $result;
}

function build_history_urlencode_1($str, $sep1, $sep2)
{
	$parts = split($sep1, $str);

	for ($i = 0; $i < count($parts); $i++) {
		if (strcmp($sep2, "") == 0) {
			$parts[$i] = urlencode(htmlspecialchars($parts[$i]));
		}
		else {
			$parts[$i] = build_history_urlencode_1($parts[$i], $sep2, "");
		}
	}

	return implode($sep1, $parts);
}

function build_history_query_to_array($query)
{
	$ary1 = split("&", $query);
	for ($i = 0; $i < count($ary1); $i++) {
		$ary2 = split("=", $ary1[$i]);
		if (isset($ary2[0]) && isset($ary2[1])) {
			$queries[$ary2[0]] = $ary2[1];
		}
	}

	return $queries;
}
?>

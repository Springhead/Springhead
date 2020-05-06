<?php
//
//  1. Generate dailybuild result table - block type.
//  2. Get revision number and the date to be compared with - inline type.
//
//  Result format:
//  1. result table:
//	+-------+---------------------+---------------------+
//	|       |       Success       |       Failure       |
//	+-------+---------------------+---------------------+
//	| Build | <test module names> | <test module names> |
//	+-------+---------------------+---------------------+
//	|  Run  | <test module names> | <test module names> |
//	+-------+---------------------+---------------------+
//
//  2. revision-date string:
//	"<revision> (<date>)"
//
//  Last modified:
//	2014/02/19 F.Kanehori
//
function plugin_dailybuild_result_convert()
{
	// Arguments are:
	//   $base:   Directory name where the file is.
	//   $file1:  Test report file name.
	//   $file2:  File name of dailybuild result log.
	//   $start:  Data extraction offset: 0 for 'tests', 4 for 'Samples'.
	//   $count:  Data extraction count:  4 for 'tests', 2 for 'Samples'.
	//
	if (func_num_args() != 5) {
		return 'bad request - NA';
	}
	list($base, $file1, $file2, $start, $count) = func_get_args();

	// Get date information.
	//
	$cmnd = "/usr/bin/head -3 $base/$file1";
	exec($cmnd, $output);
	$fields = split(" ", $output[2]);
	$df = $fields[3];
	$date = substr($df,1,4)."/".substr($df,6,2)."/".substr($df,8,2);

	// Read content of the result file.
	//
	$fname = "$base/$file2";
	if (($fh = fopen($fname, "r")) != TRUE) {
		return 'bad request - FO';
	}
	$content = fread($fh, filesize($fname));
	fclose($fh);

	// Analyze the content.
	//
	$lines = split('\)', $content);
		// Kludge - 2013-0912 F.Kanehori
		if (count($lines) == 6) {
			$lines[5] = "(";	// missing 'build failed' line!
			$lines[6] = "";
		}
		// End
	if ($lines == FALSE || (count($lines) != 9 && count($lines) != 7)) {
		$msg = sprintf("bad data - BC (count: %d)<br>", count($lines));
		return $msg;
	}
	$result = array();
	$result["B S"] = $result["B F"] = "";
	$result["R S"] = $result["R F"] = "";
	for ($i = $start, $j = 0; $i < $start + $count; $i++, $j++) {
		$line = htmlspecialchars($lines[$i], ENT_QUOTES);
		if ($count == 4) {
			$proc = ($j == 0 || $j == 1) ? "B" : "R";	// "Build" and "Run"
			$code = ($j == 0 || $j == 2) ? "S" : "F";	// "Success" and "Failure"
		}
		else {
			$proc = "B";					// "Build" only
			$code = ($j == 0) ? "S" : "F";			// "Success" and "Failure"
		}
		$ary1 = split('\(', $line);
		if (($ary1 == FALSE) || count($ary1) != 2) {
			$table = sprintf("line %d: invalid data", $i+1);
			return 'bad data - BD';
		}	
		$key = "$proc $code";
		$result[$key] = preg_replace("/,/", ", ", $ary1[1]);
	}
	// Rearange
	//	"lib1:mod1,lib2:mod2,…,libN:modN,…"
	//	==> array("lib1:mod11,mod12,…", …, "libN:modN1,modN2,…", …)
	//
	$bld_succ = dailybuild_result_make_array($result["B S"]);
	$bld_fail = dailybuild_result_make_array($result["B F"]);
	if ($count == 4) {
		$run_succ = dailybuild_result_make_array($result["R S"]);
		$run_fail = dailybuild_result_make_array($result["R F"]);
	}

	// String constants (in UTF8).
	//
	//$s_now  = mb_convert_encoding("現  在", "UTF-8", "EUC-JP");
	//$s_bld  = mb_convert_encoding("ビルド", "UTF-8", "EUC-JP");
	//$s_run  = mb_convert_encoding("実  行", "UTF-8", "EUC-JP");
	//$s_succ = mb_convert_encoding("成  功", "UTF-8", "EUC-JP");
	//$s_fail = mb_convert_encoding("失  敗", "UTF-8", "EUC-JP");
	$s_now  = "現  在";
	$s_bld  = "ビルド";
	$s_run  = "実  行";
	$s_succ = "成  功";
	$s_fail = "失  敗";

	// Some table attributes.
	//
	$bd_t = "border-style: none none none none;";
	$bd_m = "border-style: dashed none none none;";
	$bd_b = "border-style: dashed none none none;";
	$wd_e = "width: 1.6em;";		// left margin
	$wd_h = "width: 14%;";			// header
	$wd_s = "width: 43%;";			// success
	$wd_f = "width: 43%;";			// failure
	$ta_c = "text-align: center;";
	$ta_l = "text-align: left;";
	$ta_t = "vertical-align: top;";
	$ta_m = "vertical-align: middle;";
	$bc_n = "background: #ffffff;";		// none
	$bc_h = "background: #f0f0f0;";		// header
	$bc_s = "background: #f0fff0;";		// success
	$bc_f = "background: #f0f0ff;";		// failure
	$fc_n = "color: black;";		// normal
	$fc_w = "color: red;";			// warning
	$small   = "font-size: 75%;";
	$small_t = "font-size: 85%;";
	$bold = "font-weight: bold;";

	// Generate table source code.
	//
	$table = "<table width='80%' border='0' cellpadding='3' cellspacing='0'>"
		."<tr>"
		.  "<td style='$bd_t $wd_e'></td>"
		.  "<td>"
		.    "<table border='1' rules='hidden' cellpadding='3' cellspacing='0'>"
		.      "<tr>"
		.        "<td style='$bd_t $wd_h $ta_c $ta_m $bc_n $fc_n $small'>$date $s_now</td>"
		.        "<td style='$bd_t $wd_s $ta_c $ta_m $bc_h $fc_n $bold'>$s_succ</td>"
		.        "<td style='$bd_t $wd_f $ta_c $ta_m $bc_h $fc_n $bold'>$s_fail</td>"
		.      "</tr>";

	// ---- build ----
	$bd = ($count == 4) ? $bd_m : $bd_b;
	$table .=      "<tr>"
		.        "<td style='$bd $wd_h $ta_c $ta_m $fc_n $bc_h $bold'>$s_bld</td>"
		.        "<td style='$bd $wd_s $bc_s $ta_t'>"
		.	    dailybuild_result_make_table($bld_succ, $fc_n, $bc_s, $small_t)
		.        "</td>"
		.        "<td style='$bd $wd_f $bc_f $ta_t'>"
		.	    dailybuild_result_make_table($bld_fail, $fc_w, $bc_f, $small_t)
		.        "</td>"
		.      "</tr>";

	// ---- run ----
	if ($count == 4) {
		$bd = $bd_b;
		$table .=      "<tr>"
		.        "<td style='$bd $wd_h $ta_c $ta_m $fc_n $bc_h $bold'>$s_run</td>"
		.        "<td style='$bd $wd_s $bc_s $ta_t'>"
		.	    dailybuild_result_make_table($run_succ, $fc_n, $bc_s, $small_t)
		.        "</td>"
		.        "<td style='$bd $wd_f $bc_f $ta_t'>"
		.	    dailybuild_result_make_table($run_fail, $fc_w, $bc_f, $small_t)
		.        "</td>"
		.      "</tr>";
	}

	$table .=    "</table>"
		.  "</td>"
		."</tr>"
		."</table>";

	return $table;
}

function dailybuild_result_make_table($items, $fcolor, $bcolor, $font)
{
	// $items:	table contents
	// $fcolor:	text color
	// $bcolor:	background color
	// $font:	text font attribute

	$wid_i = "width: 3%;";	// indent
	$wid_b = "width: 43%;";	// body
	$align  = "text-align: left; vertical-align: top;";

	$style_i = "style='$wid_i $align $fcolor $bcolor $font'";
	$style_b = "style='$wid_b $align $bcolor $fcolor $font'";

	$table = "<table border='0' cellspacing='0' cellpadding='0'>";

	for ($i = 0; $i < count($items); $i++) {
		$item = split(':', $items[$i]);
		$table .= "<tr>"
			.   "<td $style_i colspan='2'>$item[0]:</td>"
			. "</tr>"
			. "<tr>"
			.   "<td $style_i>&nbsp;</td>"
			.   "<td $style_b>$item[1]</td>"
			. "</tr>";
	}

	$table .= "</table>";

	return $table;
}

function dailybuild_result_make_array($data)
{
	// $data:	result data

	$i_ary = split(",", $data);
	$o_ary = array();
	$libname = "";
	$modname = "";
	$t_libname = "";
	$once = 1;
	for ($i = 0; $i < count($i_ary); $i++) {
		if (! strpos($i_ary[$i], ":")) {
			continue;
		}
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
	if (strcmp($t_libname, "")) {
		$o_ary[] = "$t_libname:$modname";
	}

	return $o_ary;
}

function plugin_dailybuild_result_inline()
{
	// Arguments are:
	//   $base:  Directory name where the file is.
	//   $file:  Test report file name from which revision and date are taken. 
	//
	if (func_num_args() != 3) {
		return 'bad erquest - NA';
	}
	list($base, $file) = func_get_args();

	// Get revision and date information.
	//
	$cmnd = "/usr/bin/head -3 $base/$file";
	exec($cmnd, $output);
	$data = htmlspecialchars($output[1], ENT_QUOTES);
	$fields = split(" ", $data);
       	$rf = $fields[2];
       	$df = $fields[3];
       	$revision = substr($rf,1);
       	$date = substr($df,1,4)."/".substr($df,6,2)."/".substr($df,8,2);

	return "$revision ($date)";
}
?>

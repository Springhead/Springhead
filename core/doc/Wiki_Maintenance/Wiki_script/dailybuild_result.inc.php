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
//	2022/03/03 F.Kanehori	Change to new style description.
//	2020/04/16 F.Kanehori	Add success/failure count to the table.
//	2018/04/16 F.Kanehori	Correspond to new php version.
//	2018/03/15 F.Kanehori
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
		return 'bad request - NA ('.func_num_args().')';
	}
	list($base, $file1, $file2, $start, $count) = func_get_args();

	// Get date information.
	//
	$cmnd = "/usr/bin/head -3 $base/$file1";
	exec($cmnd, $output);
	$fields = explode(" ", $output[2]);
	$df = $fields[3];
	$date = substr($df,0,4)."/".substr($df,5,2)."/".substr($df,7,2);

	// Read content of the result file.
	//
	$fname = "$base/$file2";
	if (($fh = fopen($fname, "r")) != TRUE) {
		return 'bad request - FO ("'.$file2.'")';

	}
	$content = fread($fh, filesize($fname));
	fclose($fh);
	$content = mb_convert_encoding($content, 'SJIS');

	// Analyze the content.
	//
	$lines = explode(")", $content);
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
		$line = htmlspecialchars($lines[$i], ENT_QUOTES, "cp932");
		if ($count == 4) {
			$proc = ($j == 0 || $j == 1) ? "B" : "R";	// "Build" and "Run"
			$code = ($j == 0 || $j == 2) ? "S" : "F";	// "Success" and "Failure"
		}
		else {
			$proc = "B";					// "Build" only
			$code = ($j == 0) ? "S" : "F";			// "Success" and "Failure"
		}
		$ary1 = explode("(", $line);
		if (($ary1 == FALSE) || count($ary1) != 2) {
			$msg = sprintf("bad data - BD (line %d)", $i+1);
			return $msg;
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

	// Count up
	$rcount = array();
	$rcount["B S"] = count_elements($bld_succ);
	$rcount["B F"] = count_elements($bld_fail);
	$rcount["R S"] = count_elements($run_succ);
	$rcount["R F"] = count_elements($run_fail);

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
	// border
	$bd_n = "border: 0;";
	$bd_s = "border: solid 1px;";
	$bd_d = "border: 1px; border-style: dashed none none none;";
	$collapse = "border-collapse: collapse;";

	// padding
	$padd = "padding: 5px;";

	// width
	$wd_l = "width: 1.6em;";		// left margin
	$wd_h = "width: 14%;";			// header
	$wd_s = "width: 43%;";			// success
	$wd_f = "width: 43%;";			// failure

	// text alignment
	$ta_c = "text-align: center;";
	$ta_t = "vertical-align: top;";
	$ta_m = "vertical-align: middle;";

	// background color
	$bc_n = "background: #ffffff;";		// none
	$bc_h = "background: #f0f0f0;";		// header
	$bc_s = "background: #f0fff0;";		// success
	$bc_f = "background: #f0f0ff;";		// failure

	// font
	$fc_n = "color: black;";		// normal
	$fc_w = "color: red;";			// warning
	$small   = "font-size: 75%;";
	$small_t = "font-size: 85%;";
	$bold = "font-weight: bold;";
	$norm = "font-weight: normal;";

	//
	$style_header_date = "style='$bd_n $wd_h $ta_c $ta_m $bc_n $fc_n $small'";	// header: date
	$style_header_succ = "style='$bd_n $wd_s $ta_c $ta_m $bc_h $fc_n $bold'";	// header: succ
	$style_header_fail = "style='$bd_n $wd_f $ta_c $ta_m $bc_h $fc_n $bold'";	// header: fail

	$style_result_numb = "style='$bd_d $wd_h $ta_c $ta_m $fc_n $bc_h $bold'";
	$style_result_succ = "style='$bd_d $wd_s $bc_s $ta_t $padd'";
	$style_result_fail = "style='$bd_d $wd_f $bc_f $ta_t $padd'";

	// Generate table source code.
	//
	$table = "<table style='width: 95%; border: 0;'>"
		."<tr>"
		.  "<td style='$bd_n $wd_l'></td>"
		.  "<td>"
		.    "<table style='$bd_s $collapse;'>"
		.      "<tr>"
		.        "<td $style_header_date>$date $s_now</td>"
		.        "<td $style_header_succ>$s_succ</td>"
		.        "<td $style_header_fail>$s_fail</td>"
		.      "</tr>";

	// ---- build ----
	$bld_msg = "$s_bld<br><br><span style='$small $norm'>"
		 . "$s_succ ".$rcount["B S"]."<br>$s_fail ".$rcount["B F"]
		 . "</span>";
	$table .=      "<tr>"
		.        "<td $style_result_numb>$bld_msg</td>"
		.        "<td $style_result_succ>".dailybuild_result_make_table($bld_succ, $fc_n, $bc_s, $small_t)."</td>"
		.        "<td $style_result_fail>".dailybuild_result_make_table($bld_fail, $fc_w, $bc_f, $small_t)."</td>"
		.      "</tr>";

	// ---- run ----
	if ($count == 4) {
		$run_msg = "$s_run<br><br><span style='$small $norm'>"
			 . "$s_succ ".$rcount["R S"]."<br>$s_fail ".$rcount["R F"]
			 . "</span>";
		$table .=      "<tr>"
		.        "<td $style_result_numb>$run_msg</td>"
		.        "<td $style_result_succ>".dailybuild_result_make_table($run_succ, $fc_n, $bc_s, $small_t)."</td>"
		.        "<td $style_result_fail>".dailybuild_result_make_table($run_fail, $fc_w, $bc_f, $small_t)."</td>"
		.      "</tr>";
	}

	$table .=    "</table>"
		.  "</td>"
		."</tr>"
		."</table>";

	return $table;
}

function count_elements($items) {
	// $items:	table contents

	$nelems = 0;
	foreach ($items as $item) {
		$modules = explode(":", $item);
		$elems = explode(",", $modules[1]);
		$nelems += count($elems);
	}
	return $nelems;
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

	$table = "<table style='line-height: 80%'>";

	for ($i = 0; $i < count($items); $i++) {
		$item = explode(":", $items[$i]);
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

	$i_ary = explode(",", $data);
	$o_ary = array();
	$libname = "";
	$modname = "";
	$t_libname = "";
	$once = 1;
	for ($i = 0; $i < count($i_ary); $i++) {
		if (! strpos($i_ary[$i], ":")) {
			continue;
		}
		$t_ary = explode(":", $i_ary[$i]);
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
	$fields = explode(" ", $data);
       	$rf = $fields[3];
       	$df = $fields[4];
       	$revision = $rf;
       	$date = substr($df,1,4)."/".substr($df,6,2)."/".substr($df,8,2);

	return "$revision ($date)";
}
?>

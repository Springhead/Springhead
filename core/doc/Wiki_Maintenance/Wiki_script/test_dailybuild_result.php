<?php

$script = "local";
$datdir = "local";

if ($script == "plugin") {
	$plugin = '/home/WWW/docroots/springhead/wiki/plugin';
}
else {
	$plugin = '.';
}
include($plugin.'/dailybuild_result.inc.php');

if ($datdir == "daily_build") {
	$base  = '/export/home/WWW/docroots/springhead/daily_build';
	$file1 = 'Test.report';
	$file2 = 'result.log';
}
else {
	$base  = '.';
	$file1 = './TestData/Test.report';
	$file2 = './TestData/result.log';
}

$dummy = "";
printf("case 1:\n");
printf("%s\n", plugin_dailybuild_result_inline($base, $file1, $dummy));
printf("\n");

printf("case 2:\n");
printf("%s\n", plugin_dailybuild_result_convert($base, $file1, $file2, 0, 4));

printf("case 3:\n");
printf("%s\n", plugin_dailybuild_result_convert($base, $file1, $file2, 4, 2));

?>

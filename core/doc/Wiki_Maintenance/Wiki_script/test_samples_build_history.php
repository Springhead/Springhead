<?php
	$uri = "http://root/med/leaf/file.xyz?k1=v1&k2=v2";
	$uri = "h+ttp://ro+ot/m+ed/leaf/file.x+yz?k1+=+v1&+k2=v2+";
	$uri = "/root/med/leaf/file.xyz";
	$uri = "/r+oot/me+d/leaf/f+ile.x+yz";
	$uri = "http://springhead.info/wiki/index.php?devel";
$uri = urldecode($uri);

	printf("[%s]\n", samples_build_history_urlencode($uri));

function samples_build_history_urlencode($uri)
{
	$result = "";

	// -- scheme --
 	$part1 = split(":", $uri);
	if (count($part1) > 1) {
		$result = urlencode(htmlspecialchars($part1[0])) . ":";
		$part1[0] = $part1[1];
	}

	// -- path --
 	$part2 = split("\?", $part1[0]);
	$result .= samples_build_history_urlencode_1($part2[0], "/", "");

	// -- query --
	if (count($part2) > 1) {
		$result .= "?" . samples_build_history_urlencode_1($part2[1], "&", "=");
	}

	return $result;
}

function samples_build_history_urlencode_1($str, $sep1, $sep2)
{
	$parts = split($sep1, $str);

	for ($i = 0; $i < count($parts); $i++) {
		if (strcmp($sep2, "") == 0) {
			$parts[$i] = urlencode(htmlspecialchars($parts[$i]));
		}
		else {
			$parts[$i] = samples_build_history_urlencode_1($parts[$i], $sep2, "");
		}
	}

	return implode($sep1, $parts);
}
?>

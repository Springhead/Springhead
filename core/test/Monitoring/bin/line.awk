#!/usr/bin/awak -f
BEGIN {
    if (line == 0) line = 1
}
/.*/ {
    lcount++
    if (lcount == line) {
	print $0
	exit
    }
}

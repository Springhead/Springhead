#!/usr/bin/awak -f
BEGIN {
    if (block == 0) block = 1
    if (field == 0) field = 1
    if (start == 0) start = 1
    if (sep1 == "") sep1 = " "
}
/.*/ {
    if (sep2 == "") {
	split($0, fields, sep1)
    } else {
	split($0, blocks, sep1)
	if (sep2 == "") sep2 = " "
	split(blocks[block], fields, sep2)
    }
    data = fields[field]
    print data
}

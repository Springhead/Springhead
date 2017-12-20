#!/usr/bin/awak -f
BEGIN {
    if (pat == 0) pat = 1
    if (pat == "rev") pat = 1
    if (pat == "head") pat = 2
}
/^r[0-9]+/	{ if (pat == 1) print }
/^[0-9a-z]+/	{ if (pat == 1) print }
/^HEAD/		{ if (pat == 2) print }


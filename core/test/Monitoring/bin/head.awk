#!/usr/bin/awak -f
/.*/ { if (i++ < lines) print $0 }

#!/bin/sh -f

if [ $# -lt 2 ]; then
    echo "Usage: $0 outlib proj1 proj2 ..."
    exit 1
fi

outlib=$1; shift
projs=$*

members=
addcmnd=
for proj in ${projs}
do
    members="${members} ${proj}/lib${proj}.a"
    addcmnd="${addcmnd}addlib ${proj}/lib${proj}.a\\n"
done

echo step 1 ... creating thin archive
/bin/rm -f ${outlib}
ar cqT ${outlib} ${members}

echo step 2 ... creating static library
echo -n "create ${outlib}\\n${addcmnd}\\nsave\\nend" | ar -M

echo step 3 ... ranlib
ranlib ${outlib}
#ar t ${outlib}

exit 0

#!/bin/sh -f

if [ $# -lt 3 ]; then
    echo "Usage: $0 SHARED outlib proj1 proj2 ..."
    echo "       $0 STATIC outdir proj1 proj2 ..."
    exit 1
fi

libtype=$1; shift
outlib=$1; shift
projs=$*
if [ $libtype = "SHARED" ]; then
    suffix=so
else
    suffix=a
fi

#if [ 1 -eq 1 ]; then
if [ $libtype = "SHARED" ]; then
    #
    # Simply copy archive files to output directory.
    #
    suffix=so
    outdir = ${outlib}
    script="import os.path; print(os.path.relpath('$outlib', '$cwd'))"
    for proj in ${projs}
    do
	echo copying ${proj}/lib${proj}.${suffix} to `python -c "${script}"`
	cp ${proj}/lib${proj}.${suffix} ${outlib}
    done
else
    #
    # if combine archive files to one file
    #
    suffix=a
    members=
    addcmnd=
    for proj in ${projs}
    do
	members="${members} ${proj}/lib${proj}.${suffix}"
	addcmnd="${addcmnd}addlib ${proj}/lib${proj}.${suffix}\\n"
    done

    echo step 1 ... creating thin archive
    /bin/rm -f ${outlib}
    ar cqT ${outlib} ${members}

    echo step 2 ... creating \"`basename ${outlib}`\"
    echo -n "create ${outlib}\\n${addcmnd}\\nsave\\nend" | ar -M

    echo step 3 ... ranlib \"`basename ${outlib}`\"
    ranlib ${outlib}
    #ar t ${outlib}
fi

exit 0

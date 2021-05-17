#!/bin/bash -f

if [ $# -lt 4 ]; then
    echo "Usage: $0 SHARED builddir outdir libname proj1 proj2 ..."
    echo "       $0 STATIC builddir outdir libname proj1 proj2 ..."
    exit 1
fi

libtype=$1; shift
builddir=$1; shift
outdir=$1; shift
libname=$1; shift
projs=$*
#echo "libtype: ${libtype}"
#echo "outdir:  ${outdir}"
#echo "libname: ${libname}"
#echo "projs:   ${projs}"

function fun() { 
    # object files
    objs=`/bin/ls $1 | grep '\.o'`
    # directories
    files=`/bin/ls $1`
    tmpobjs=
    for f in $files; do
        if [ -d $1/$f ]; then 
            tmpobjs=`fun $1/$f`
            for tmpf in $tmpobjs; do
                objs="$objs $f/$tmpf"
            done
        fi      
    done    
    echo $objs
    return 0
}

if [ ${libtype} = "STATIC" ]; then
    #
    # step 1-3:  combine archive files to one file
    #
    /bin/rm -f ${outdir}/${libname}.a

    echo step 1 ... creating static library
    for proj in ${projs}
    do
	pushd ${builddir}/${proj}/CMakeFiles/${proj}.dir > /dev/null
	objs=`fun .`
	for obj in $objs;
	do
	    ar qvs ${outdir}/${libname}.a $objs
	done
	popd > /dev/null
    done

    echo step 2 ... ranlib \"${libname}.a\"
    ranlib ${outdir}/${libname}.a
    #ar t ${libname}.a
fi

if [ $libtype = "SHARED" ]; then
    #
    # step 1-3: generate shared library
    #
    members=
    for proj in ${projs}
    do
	members="${members}${proj}/lib${proj}.a "
    done

    echo step 1 ... creating shared library
    tmpout=${outdir}/${libname}.tmp.so
    gcc -shared -o ${tmpout} -Wl,--whole-archive ${members} \
			     -Wl,--no-whole-archive
    stat=$?
    if [ ${stat} -eq 0 ]; then
	mv ${tmpout} ${outdir}/${libname}.so
    else
	echo "failed (status ${stat})"
    fi
fi

exit 0

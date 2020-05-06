# -*- coding: sjis -*-

#
# 同ディレクトリの.hファイルの中にPyAPI_FUNCがあった場合
# __PYDECLを挿入する(Add_decl.hにて #define __PYDECL __cdecl)
#

# <!!> object.hの一部関数（richcmpfunc等）には__PYDECLがつかないので手動で挿入すること

import os
import re
if __name__ == "__main__":
    files = os.listdir( os.getcwd() )

    for file in files:
        regex = re.compile(".*\.h$")
        res = regex.match(file)
        if res == None :
            continue
        elif re.match("Add_decl\.h",file) != None:
            continue
        else:
            filename = res.group()
            print("'" + filename + "' を変更中")
            
            fp = open(file,"r+")

            if fp != None:
                lines = fp.readlines()
                if re.match("^#include \"Add_decl.h\"\n",lines[0]) != None:
                    print("\n")
                    continue
                

                output = []
                output.append( "#include \"Add_decl.h\"\n" )

                regex = re.compile("[ \t]*(PyAPI_FUNC\(.*?\))(.*\(.*)")

                for line in lines:
                    if regex.match(line) and re.match(".* __PYDECL .*",line) == None :
                        output.append( regex.sub(r"\1 __PYDECL \2",line) )
                    else :
                        output.append( line )
                
                fp.seek(0)
                for oline in output:
                    fp.write(oline)
                    print(".")
                fp.close()
            
            print("\n'" + filename + "'の変更終了")
                
            
    


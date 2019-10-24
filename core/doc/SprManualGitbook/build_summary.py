from pathlib import Path
import re

with open("SUMMARY.md", mode='w') as ofp:

    ofp.write("# Summary\n\n")
    ofp.write("* [Introduction](README.md)\n\n")

    contents = Path("contents")
    for file in sorted(list(contents.glob("*.md"))):
        with open(file) as ifp:
            maintitle = ifp.readline().rstrip("\n").lstrip('#').lstrip(' ')
            ofp.write("* [" + maintitle + "](" + str(file) + ")" + "\n")
            for line in ifp:
                line = line.rstrip('\n')
                match = re.match("##[^#]", line)
                if match:
                    title = line.lstrip('#').lstrip(' ')
                    ofp.write("  * [" + title + "](" + str(file) + "#" + title.lower().replace(' ', '-') + ")" + "\n")

            

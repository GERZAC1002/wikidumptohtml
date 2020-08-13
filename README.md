# wikidumptohtml
Crude wikipedia dump to html converter that is not ready yet<br />
Compile: gcc wikidumptohtml.c -o wikidumptohtml<br />
How to use:<br />
wikidumptohtml <wikidumpfile.xml-*><br />
Outputs the html files in the current directory.
# ISSUES
~~- if files already exist the program breaks after 1021 pages~~ FunFact: single program can not open more than 1024 files per default on Arch Linux
- it does not create tables/references/hyperlinks
- it creates only basic html files
- it is far away from being usable for most people

# NOTE
This is a hobby project!!!

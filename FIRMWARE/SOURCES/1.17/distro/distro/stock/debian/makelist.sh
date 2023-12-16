#!/bin/sh
echo "<HTML>"
echo "<TITLE>File listing</TITLE>"
echo "<BODY>"
echo "<UL>"
for i in *; do
if [ "$i" != "makelist.sh" ]; then
if [ "$i" != "index.html" ]; then
echo "<LI><A HREF=\"$i\">$i</A></LI>"
fi
fi
done
echo "</UL>"
echo "</BODY>"
echo "</HTML>"

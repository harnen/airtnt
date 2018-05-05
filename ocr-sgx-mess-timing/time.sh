#!/bin/bash
echo "Bash version: "$BASH_VERSION


FILE="timings/ocr-imsize.dat"
CHECK_MARK="\033[0;32m\xE2\x9C\x94\033[0m"
END=$1
for (( c=0; c<$END; c++ ))
do
	./app >> $FILE"."$c
	echo -ne "\\r[$c/$END] Timing OCR for various image sizes..."
	sleep 0.1
done

echo -en "\033[1K"
echo -e "\\r[$c/$END] ${CHECK_MARK}"
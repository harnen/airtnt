#!/bin/bash

for file in ocr_log*.dat.[0-9]
do 
    ./parse_data.sh $file > parsed_$file
done

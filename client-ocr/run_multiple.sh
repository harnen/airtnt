#!/bin/bash

#	source /opt/intel/sgxsdk/environment
    
    for ((i=0; i<10; i++))
    do
        echo Running size  repetition $i
        ./run.sh > "ocr_log.dat.${i}"
    done

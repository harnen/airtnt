#!/bin/bash


for size in 10 25 50
do
    ssh ubuntu@ec2-18-130-51-114.eu-west-2.compute.amazonaws.com "cd ~/airtnt/server; sed -i -e 's/unsigned int size = .*/unsigned int size = $size;/g' service_provider.cpp; make"
    
    for ((i=0; i<10; i++))
    do
        echo Running size $size repetition $i
        ./run.sh > "log_size=${size}.dat.${i}"
    done
done

#!/bin/bash

start=100
end=10000
step=100
for ((i=start; i<end; i+=step))
do
    echo Now running $i >&2
    echo iteration:$i limit:$end
    ssh ubuntu@ec2-18-130-51-114.eu-west-2.compute.amazonaws.com "killall server; cd ~/airtnt/server; LD_LIBRARY_PATH=./sample_libcrypto ./server 8000 ${i} ${end}" &
    sleep 2 
    LD_LIBRARY_PATH=./sample_libcrypto  ./app        
done

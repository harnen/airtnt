#!/bin/bash

while read line
do
#    echo $line
    echo $line | grep "iteration:" &> /dev/null
    if [ $? -eq 0 ]
    then   
        ITER=`echo $line | cut -d ':' -f 2 | cut -d ' ' -f 1`
        LIMIT=`echo $line | cut -d ':' -f 3`
#        echo Got iter: $ITER limit: $LIMIT
        echo -en "$ITER\t"
    fi 
    echo $line | grep "Time connected" &> /dev/null
    if [ $? -eq 0 ]
    then   
        T_ALL=`echo $line | cut -d ']' -f 4 | cut -d ',' -f 1`
#        echo Got T_ALL: $T_ALL
        echo -en "$T_ALL\t"
    fi
    echo $line | grep "Time spent in the enclave" &> /dev/null
    if [ $? -eq 0 ]
    then   
        T_ENC=`echo $line | cut -d ' ' -f 6`
#        echo Got T_ENC: $T_ENC
        echo  "$T_ENC"
    fi
    
done < $1

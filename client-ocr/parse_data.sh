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
    fi 
    echo $line | grep "Time connected" &> /dev/null
    if [ $? -eq 0 ]
    then   
        T_ALL=`echo $line | cut -d ']' -f 4 | cut -d ',' -f 1`
#        echo Got T_ALL: $T_ALL
    fi
    echo $line | grep "Bytes received:" &> /dev/null
    if [ $? -eq 0 ]
    then   
        BYTES_RECV=`echo $line | cut -d ':' -f 2`
#        echo Got BYTES_RECV $BYTES_RECV
    fi
    
    echo $line | grep "Bytes sent:" &> /dev/null
    if [ $? -eq 0 ]
    then   
        BYTES_SENT=`echo $line | cut -d ':' -f 2`
#        echo Got BYTES_SENT $BYTES_SENT
    fi
    
    echo $line | grep "Time spent in the enclave" &> /dev/null
    if [ $? -eq 0 ]
    then   
        T_ENC=`echo $line | cut -d ' ' -f 6`
#        echo Got T_ENC: $T_ENC
        echo -en "$ITER\t"
        echo -en "$T_ALL\t"
        echo -en "$T_ENC\t"
        echo -en "$BYTES_SENT\t"
        echo -en "$BYTES_RECV\t"
        echo ""
    fi
    
done < $1

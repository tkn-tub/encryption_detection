#!/bin/bash                                                                                                                                                                                                 

#datasetPath="/mnt/d/DATA_SET/CHOSEN/test_file_types"
datasetPath="/root/DATA/CHOSEN_FOR_EXTRACTING/"
fileTypes=("au" "txt" "mp3" "pdf" "wav" "png" "xls" "csv" "webm" "mat" "zip" "jpg" "mp4")
declare -A GetIndex
GetIndex=([au]=0 [txt]=1 [mp3]=2 [pdf]=3 [wav]=4 [png]=5 [xls]=6 [csv]=7 [webm]=8 [mat]=9 [zip]=10 [jpg]=11 [mp4]=12)



#pip3 install pyftpdlib
run_ftp_servers (){

    for dir in ${fileTypes[*]}; do
            Index=${GetIndex[$dir]}
            # echo $Index
            serverPort=`expr $1 + ${GetIndex[$dir]}`
            echo "$dir"
            echo $serverPort
            echo "$datasetPath/$dir"
            cd "$datasetPath/$dir"
            python3 -m pyftpdlib -p $serverPort &

	done

}


run_ftp_servers "2000"

# sleap before shutting down the servers
sleep 20000

servers_pid=$(ps -e | pgrep python3)
for i in $servers_pid; do kill -9 $i; done
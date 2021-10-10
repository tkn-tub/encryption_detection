#!/bin/bash                                                                                                                                                                                                 

datasetPath="/root/DATA/CHOSEN_FOR_EXTRACTING/"
fileTypes=("au" "txt" "mp3" "pdf" "wav" "png" "xls" "csv" "webm" "mat" "zip" "jpg" "mp4")
declare -A GetIndex
GetIndex=([au]=0 [txt]=1 [mp3]=2 [pdf]=3 [wav]=4 [png]=5 [xls]=6 [csv]=7 [webm]=8 [mat]=9 [zip]=10 [jpg]=11 [mp4]=12)


run_https_servers (){

    for dir in ${fileTypes[*]}; do
            Index=${GetIndex[$dir]}
            # echo $Index
            serverPort=`expr $1 + ${GetIndex[$dir]}`
            echo "$dir"
            echo $serverPort
            echo "$datasetPath/$dir"
            cd "$datasetPath/$dir"
            # Run the https server from httpsServer.py script, Full path is recommended
            python3 ./httpsServer.py $serverPort &

	done

}

run_https_servers "4000"

# sleap before shutting down the servers
sleep 20000

servers_pid=$(ps -e | pgrep python3)
for i in $servers_pid; do kill -9 $i; done
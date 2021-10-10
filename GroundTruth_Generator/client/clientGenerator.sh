#!/bin/bash                                                                                                                                                                                                 

datasetPath="/root/DATA/CHOSEN_FOR_EXTRACTING/"
pcapPath="/root/pcapFiles/"
protocols=("HTTP" "FTP" "TELNET" "HTTPS" "SFTP" "SSH")
fileTypes=("au" "txt" "mp3" "pdf" "wav" "png" "xls" "csv" "webm" "mat" "zip" "jpg" "mp4")
protocols=("HTTP" "FTP" "TELNET" "HTTPS" "SFTP" "SSH")
fileTypes=("au" "txt" "mp3" "pdf" "wav" "png" "xls" "csv" "webm" "mat" "zip" "jpg" "mp4")
declare -A GetIndex
GetIndex=([au]=0 [txt]=1 [mp3]=2 [pdf]=3 [wav]=4 [png]=5 [xls]=6 [csv]=7 [webm]=8 [mat]=9 [zip]=10 [jpg]=11 [mp4]=12)

# Define the maximum number of flows (files) need to be generated which are labeled according to protocol and file type.
max=20000
counter=0
#this function extracts the file names from a directory and stores in an array
#it uses the datasetPath variable which is the parent directory that contains subDir of file types
#it takes the subdirectory (files type) as parameter, returns the file names
extract_array_files() {
    local array
    i=0
    while read line
    do
        array[ $i ]="$line"
        (( i++ ))
    done < <(ls "$datasetPath"/$1)
    echo "${array[*]}"

}
# save file names for each type in an array
for i in ${fileTypes[*]}; do
    Array=($(extract_array_files "$i"))
    if [ "$i" == "au" ]; then auArray=($(extract_array_files "au")); fi
    if [ "$i" == "txt" ]; then txtArray=($(extract_array_files "txt")); fi
    if [ "$i" == "mp3" ]; then mp3Array=($(extract_array_files "mp3")); fi
    if [ "$i" == "pdf" ]; then pdfArray=($(extract_array_files "pdf")); fi
    if [ "$i" == "wav" ]; then wavArray=($(extract_array_files "wav")); fi
    if [ "$i" == "png" ]; then pngArray=($(extract_array_files "png")); fi
    if [ "$i" == "xls" ]; then xlsArray=($(extract_array_files "xls")); fi
    if [ "$i" == "csv" ]; then csvArray=($(extract_array_files "csv")); fi
    if [ "$i" == "webm" ]; then webmArray=($(extract_array_files "webm")); fi
    if [ "$i" == "mat" ]; then matArray=($(extract_array_files "mat")); fi
    if [ "$i" == "zip" ]; then zipArray=($(extract_array_files "zip")); fi
    if [ "$i" == "jpg" ]; then jpgArray=($(extract_array_files "jpg")); fi
    if [ "$i" == "mp4" ]; then mp4Array=($(extract_array_files "mp4")); fi
done


#select protocol randomly from protocols array
select_protocol (){
    

    local selectedProtocol=${protocols[$RANDOM % ${#protocols[@]} ]}

    # Write to Shell
    echo "$selectedProtocol"
}
#select directory randomly from fileTypes array (which contains files from same type)
select_directory_of_fileType (){
    

    local selectedFileType=${fileTypes[$RANDOM % ${#fileTypes[@]} ]}
    echo "$selectedFileType"
    # Write to Shell
    # echo $selectedFileType
}
select_file_type (){
    

    #local selectedFile=${fileTypes[$RANDOM % ${#fileTypes[@]} ]}
    if [ "$1" == "au" ]; then local selectedFile=${auArray[$RANDOM % ${#auArray[@]} ]}; fi
    if [ "$1" == "txt" ]; then local selectedFile=${txtArray[$RANDOM % ${#txtArray[@]} ]}; fi
    if [ "$1" == "mp3" ]; then local selectedFile=${mp3Array[$RANDOM % ${#mp3Array[@]} ]}; fi
    if [ "$1" == "pdf" ]; then local selectedFile=${pdfArray[$RANDOM % ${#pdfArray[@]} ]}; fi
    if [ "$1" == "wav" ]; then local selectedFile=${wavArray[$RANDOM % ${#wavArray[@]} ]}; fi
    if [ "$1" == "png" ]; then local selectedFile=${pngArray[$RANDOM % ${#pngArray[@]} ]}; fi
    if [ "$1" == "xls" ]; then local selectedFile=${xlsArray[$RANDOM % ${#xlsArray[@]} ]}; fi
    if [ "$1" == "csv" ]; then local selectedFile=${csvArray[$RANDOM % ${#csvArray[@]} ]}; fi
    if [ "$1" == "webm" ]; then local selectedFile=${webmArray[$RANDOM % ${#webmArray[@]} ]}; fi
    if [ "$1" == "mat" ]; then local selectedFile=${matArray[$RANDOM % ${#matArray[@]} ]}; fi
    if [ "$1" == "zip" ]; then local selectedFile=${zipArray[$RANDOM % ${#zipArray[@]} ]}; fi
    if [ "$1" == "jpg" ]; then local selectedFile=${jpgArray[$RANDOM % ${#jpgArray[@]} ]}; fi
    if [ "$1" == "mp4" ]; then local selectedFile=${mp4Array[$RANDOM % ${#mp4Array[@]} ]}; fi
    echo "$selectedFile"
    # Write to Shell
    # echo $selectedFileType
}

#get the absolute path of the selected file randomly
get_file_path (){
    # DIR=$(select_directory_of_fileType)
    # echo $DIR
    file=$(select_file_type "$DIR")
    # echo $file
    local FILEPATH="$datasetPath/$1/$file"
    echo "$FILEPATH"
}
http_ (){
    echo "I am HTTP :)"
    #echo the selected file type
    DIR=$(select_directory_of_fileType)
    # echo $DIR
    # FILE=$(get_file_path "$DIR")
    file=$(select_file_type "$DIR")
    FILEPATH="$datasetPath/$DIR/$file"
    # echo "THIS IS THE FILE PATH"; echo $FILEPATH
    # filePath=$(get_file_path)

    portBase=8000
    Index=${GetIndex[$DIR]}
    # echo $Index
    serverPort=`expr $portBase + ${GetIndex[$DIR]}`
    echo $serverPort
    echo $file
    # cd 
    echo ${FILEPATH#$datasetPath}
    wget http://138.68.92.16:"$serverPort/$file" -P /root/download/http

}
ftp_ (){

    echo "I am FTP :)"
    #echo the selected file type
    DIR=$(select_directory_of_fileType)
    # echo $DIR
    # FILE=$(get_file_path "$DIR")
    file=$(select_file_type "$DIR")
    FILEPATH="$datasetPath/$DIR/$file"
    # echo "THIS IS THE FILE PATH"; echo $FILEPATH
    # filePath=$(get_file_path)

    portBase=2000
    Index=${GetIndex[$DIR]}
    # echo $Index
    serverPort=`expr $portBase + ${GetIndex[$DIR]}`
    echo $serverPort
    echo $file
    # cd 
    # echo ${FILEPATH#$datasetPath}
    wget ftp://134.122.80.82:"$serverPort/$file" -P /root/download/ftp

}
recurse_in_dierctories (){
    count=1
	for pathName in "$1"/*; do
		if [ -d "$pathName" ]; then
			echo "Directory:	${pathName#./} .."
            ftp_download_files "${pathName#./}"
            let count++
		fi
	done

}
telnet_ (){
    echo "I am telnet here:)"
    DIR=$(select_directory_of_fileType)
    file=$(select_file_type "$DIR")
    FILEPATH="$datasetPath/$DIR/$file"
    echo "THIS IS THE FILE PATH"; echo $FILEPATH

    portBase=6000
    Index=${GetIndex[$DIR]}
    # echo $Index
    serverPort=`expr $portBase + ${GetIndex[$DIR]}`
    echo $serverPort
    echo $file
    #client listen
    nc -nlvvp $serverPort -q 1 >  "/root/download/nctcp/$file"  &
    nc -w 1 138.68.65.189 $serverPort < $FILEPATH
    servers_pid=$(ps -e | pgrep nc)
    for i in $servers_pid; do kill -2 $i; done
}


#CERTIFICATE
#openssl req -new -x509 -keyout localhost.pem -out localhost.pem -days 365 -nodes
https_ () {
    echo "I am HTTPS )"
    DIR=$(select_directory_of_fileType)
    file=$(select_file_type "$DIR")
    FILEPATH="$datasetPath/$DIR/$file"
    echo "THIS IS THE FILE PATH"; echo $FILEPATH

    portBase=4000
    Index=${GetIndex[$DIR]}
    # echo $Index
    serverPort=`expr $portBase + ${GetIndex[$DIR]}`
    echo $serverPort
    echo $file
    # -P path to save file in
    wget --no-check-certificate https://138.197.177.67:"$serverPort/$file" -P /root/download/https
    # wget --no-check-certificate "https://138.68.92.16:4443/test.txt"
}
ssh_ () {
    echo "I am SSH )"
    DIR=$(select_directory_of_fileType)
    file=$(select_file_type "$DIR")
    FILEPATH="$datasetPath/$DIR/$file"
    echo "THIS IS THE FILE PATH"; echo $FILEPATH

    portBase=3000
    Index=${GetIndex[$DIR]}
    # echo $Index
    serverPort=`expr $portBase + ${GetIndex[$DIR]}`
    echo $serverPort
    echo $file
    if [ $serverPort -lt "3007" ]; then
    scp -i /root/id_rsa -P $serverPort root@138.68.65.189:$FILEPATH /root/download/ssh/
    else 
    scp -i /root/id_rsa -P $serverPort root@138.68.92.16:"/root/DATA/CHOSEN_FOR_EXTRACTING/$DIR/$file" /root/download/ssh/
    fi
}
sftp_ () {
    echo "I am SFTP )"
    DIR=$(select_directory_of_fileType)
    file=$(select_file_type "$DIR")
    FILEPATH="$datasetPath/$DIR/$file"
    echo "THIS IS THE FILE PATH"; echo $FILEPATH

    portBase=7000
    Index=${GetIndex[$DIR]}
    # echo $Index
    serverPort=`expr $portBase + ${GetIndex[$DIR]}`
    echo $serverPort
    echo $file
    if [ $serverPort -lt "7007" ]; then
    sftp -i /root/id_rsa -P $serverPort root@134.122.80.82:"/root/DATA/CHOSEN_FOR_EXTRACTING/$DIR/$file" /root/download/sftp
    else 
    sftp -i /root/id_rsa -P $serverPort root@138.197.177.67:"/root/DATA/CHOSEN_FOR_EXTRACTING/$DIR/$file" /root/download/sftp
    fi

}

sudo tcpdump -s0 -i any "( portrange 6000 -6013 or portrange 8000 -8013 or portrange 2000 -2013 or portrange 4000 -4013 or portrange 3000 -3013 or portrange 7000 -7013) " -nvv -w ./pcapFiles / capturedTraffic . pcap



# For each iteration of this loop a protocol is selected randomly and a file is downloaded from the corresponding server.
while [[ "$counter" -lt "$max" ]]
do
    #echo the selected protocol
    PROTO=$(select_protocol)
    echo $PROTO
    if [ "$PROTO" == "SFTP" ]; then sftp_ ; fi
    if [ "$PROTO" == "HTTP" ]; then http_ ; fi
    if [ "$PROTO" == "FTP" ]; then ftp_ ; fi
    if [ "$PROTO" == "SSH" ]; then ssh_ ; fi
    if [ "$PROTO" == "TELNET" ]; then telnet_ ; fi
    if [ "$PROTO" == "HTTPS" ]; then https_ ; fi


    let counter++
done


pid=$(ps -e | pgrep tcpdump)  
echo $pid  
sleep 60
kill -2 $pid

servers_pid =$(ps -e | pgrep nc); for i in $servers_pid ; do kill -9 $i; done ; ps -e
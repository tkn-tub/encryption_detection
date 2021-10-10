# Dataset
A variety of file types with different sizes are downloaded from a publicly available machine learning dataset website [Kaggle](https://www.kaggle.com/datasets). These file types are ("au" "txt" "mp3" "pdf" "wav" "png" "xls" "csv" "webm" "mat" "zip" "jpg" "mp4"). Using different data-set is possible, but it is recommended to have all the listed file types included to avoid any errors. There are multiple operations must be done on the private data-set to be ready:

    * Organizing the files in directories that named with the files type and include all files from the same type in one directory, i.e. png directory contains all.png files. The following simple script can do this task `GroundTruth_Generator/DataSetPreprocesssing`.
    * To avoid having too large PCAP file, this command can be run to exclude the files that have size more than 3M.
```
toremove=$(find . -size +3M);for i in $toremove; do rm $i; echo $i; done
```
    * As experienced, the files that contain spaces in their names can not be downloaded by some protocols such as HTTP. To avoid that, the following command can be run in the parent directory (of the file type directories) to examine all files and then replace the spaces in file names with underscores `_`.
```
for file in */**; do mv "$file" `echo $file | tr ' ' '_'` ; done
```
# Servers Setup
On each server, multiple installations are needed according to the supporting protocols.

- SCP and NETCAT: 138.68.65.189, SHH port numbers [3000, 3007]
- SCP and HTTP server: 138.68.92.16, SHH port numbers [3008, 3013]
- SFTP and FTP server : 134.122.80.82 SSH, port numbers [7000, 7007]
- SFTP and HTTPS server: 138.197.177.67, SSH port numbers [7008, 7013]

+ On all servers, OpenSSH implementation must be installed for SCP and SFTP protocols. For that, run this command.
```
sudo apt-get install openssh-server
```
+ For each server, a number of ports for SCP [3000, 3013] and SFTP [7000, 7013] must be enabled. The maximum number of ports can be enabled is eight ports. For enabling the ports, the configuration file in the path `/etc/ssh/sshd_config` is modified, see [here](https://askubuntu.com/questions/826765/opening-two-ports-at-once-in-ssh). Below, an example on how to add the port numbers on server for SCP, on another server the rest of the ports can be added. As experienced, having more than eight ports in the file enabled, will make the only port available is the default port 22.
```
Include /etc/ssh/sshd_config.d/*.conf #The ports added below this line
Port 22
Port 3001
Port 3002
Port 3003
Port 3004
Port 3005
Port 3006
```
+ Restart the SSH service by executing the following command.
```
sudo systemctl restart ssh.service
```
+ HTTP servers (port numbers [8000, 8013]) are created with the script `http.sh` using the built-in python3 library `http.server`, which runs 13 http servers on each file type directory.
+ For NETCAT (port numbers [6000, 6013]), the tool can be installed on the server with the following command. [See here](https://zoomadmin.com/HowToInstall/UbuntuPackage/netcat).
```
sudo apt-get install -y netcat
```
+ On FTP server (port numbers [2000, 2013]), the Python3 `pyftpdlib` library can be installed on the server with the following command. see [here](https://zoomadmin.com/HowToInstall/UbuntuPackage/python-pyftpdlib). Then the script `ftp.sh` runs 13 ftp servers on each file type directory.
```
sudo apt-get install -y python-pyftpdlib
```
+ HTTPS servers (port numbers [4000, 4013])are created with script `https.sh`(on each file type directory), which uses the Python3 script `httpsServer.py`, see [here](https://gist.github.com/stephenbradshaw/a2b72b5b58c93ca74b54f7747f18a481), for creating each server using the Python3 built-in libraries `http.server` and `ssl`. However, an SSL certificate needs to be created beforehand. With the following command, a basic SSL certificate using openssl can be created on the HTTPS server.
```
openssl req -new -x509 -keyout CreatedCertificate.pem -out server.pem -days 365 -nodes
```
+ After generating the ground truth, shutting down the servers running with Python3 or NC commands can be simply done with the following commands on the corresponding servers:
```
servers_pid=$(ps -e | pgrep python3); for i in $servers_pid; do kill -9 $i; done; ps -e
servers_pid=$(ps -e | pgrep nc); for i in $servers_pid; do kill -9 $i; done; ps -e
```

# Client
In practice the client is run on the server which runs `NETCAT`.
+ The traffic is captured and saved with tcpdump. Filtering the traffic is accordingto the labeled port numbers as shown in the following command
```
sudo tcpdump -s0 -i any "(portrange 6000-6013 or portrange 8000-8013 or portrange 2000-2013 or portrange 4000-4013 or portrange 3000-3013 or portrange 7000-7013)"  -nvv -w capturedTraffic.pcap
```
+ Before running the generator, all servers for all protocols must be run, then after making the `clientGenerator.sh` script executable, run this command:
```
sudo ./clientGenerator.sh
```
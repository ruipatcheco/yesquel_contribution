# G04_yesquel
G04_yesquel is a project for Large Scales Systems Engineering




To RUN a DEMO, follow this guide:

#create docker volume esle-vol
sudo docker volume create esle-vol

#inspect docker volume to discover mountpoint
sudo docker volume inspect esle-vol

#cd into that mountpoint
cd /moountpoint 

#clone this git repository to the mountpoint
git clone https://github.com/escuda/G04_yesquel 

#build the image
docker build –t yesquel:latest .

#execute 2 images
sudo docker run -it -v esle-vol:/data yesquel
sudo docker run -it -v esle-vol:/data yesquel

##### DO THIS STEP FOR ONE IMAGE ONLY #####
#inside the images, run the server
cd /data/yesquel/src
rm config.txt
nano config.txt
copy-paste the following, replacing 172.17.0.2 for docker machine 1 IP and replacing 172.17.0.3 for docker machine 2 IP:

###############################################################################
# Sample of Yesquel configuration for one server running locally

nservers       2               # number of storage servers
stripe_method  0               # method to stripe coids. Only 0 supported.
stripe_parm    0               # parameter to method to sprite. Use 0.
prefer_ip      "0.0.0.0"       # preferred IP prefix. If a server has many IPs,
                               #   it will try to pick an IP with this prefix.
prefer_ip_mask "0.0.0.0"       # enabled bits in preferred IP prefix

server 0 host "172.17.0.2" port 11223  # declaration of first server. Host
                                      # can be an IP or a DNS name. Many servers
                                      # can be on one host by assigning them
                                      # different ports (in this case,
                                      # the command line to start a server
                                      # needs to indicate a port).
server 1 host "172.17.0.3" port 11224                                      

# Configuration for each storage server

host "172.17.0.2" port 11223 { # must match the declaration above
  logfile "/tmp/d1.log"       # where to store the transaction log (if enabled)
  storedir "/tmp/d1store"     # where objects are stored (if disk enabled)
}  

host "172.17.0.3" port 11224 { # must match the declaration above
  logfile "/tmp/d2.log"       # where to store the transaction log (if enabled)
  storedir "/tmp/d2store"     # where objects are stored (if disk enabled)
}  
###############################################################################

#run the server on machine 1
cd /data/yesquel/src
./storageserver -s 11223

#run the server on machine 1
cd /data/yesquel/src
./storageserver -s 11224

#run a third docker machine and connect the servers
sudo docker run -it -v esle-vol:/data yesquel
cd /data/yesquel/src
./callserver splitter

#now both servers are ready to operate
#from your host, run a simple client that operates with the yesquel storage cluster 
cd /mountpoint/yesquel/src

#fill the database with data
./fill-db

# this executes 1000 repetitions of workload #2 with 1 client 
./test-sql-simple 1000 1

#you can also run 20 multiple concurrent clients, like so:
./test-sql-simple 1000 20 



DEMO FINISHED (hopefully) SUCCESSFULLY!





extra, if you'd like to compile any of the code from yesquel, you'll need the following compillation gimmicks 

	-added #include <unistd.h> to some .h files as errors appear :D
	sudo apt install libmysqlclient-dev
	sudo apt install mysql-server                                                     
	git clone git://github.com/lloyd/yajl and install afterwards
	sudo apt install libncurses5-dev  
	sudo apt install redis

# OS_Project 2

Synchronous Virtual Device 

Problem descriptions can be found here: http://rswiki.csie.org/dokuwiki/courses:107_2:project_2

This is the source code of operating system project2.

./master_device : the device module for master server
./slave_device  : the device module for slave client
./ksocket: the device module including the funtions used for kernel socket
./data   : input/output data
./user_program : the user program "master" and "slave"


To use it, please:
1.change to super user
2.execute "./compile.sh" to compile codes and install modules
3.follow the input instrutions in the spec, 
i.e.
./master ../data/file1_in mmap
./slave ../data/file1_out fcntl 127.0.0.1

Make sure that you are under the path "./user_program" when you execute user programs.
Though the execution order of user program "master" and "slave" does not matter,
it is suggested to execute "master" first to get more precise transmission time.

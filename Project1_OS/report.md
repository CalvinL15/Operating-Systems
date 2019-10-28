# OS2019

Project 1 - Process Scheduling

# Design

We calculate the priority of the processes in each case with providing 4 policies, and the Linux system call "sched_setscheduler" is used to set the priority and change the processing order accordingly. Qsort is used to sort the processes according to the ready time of each process. For further details, refer to process.c and scheduler.c 

# Results 
  The input files can be found in this website: http://rswiki.csie.org/dokuwiki/_media/courses:107_2:os_pj1_test.zip
  
  [Kernel and Stdout Output](https://github.com/fslo1709/Project1_OS/blob/master/Result.txt)

# Comparison

實際運行的狀況會受到CPU BOOST或是切換process的overhead所影響，所以執行時間會和理論時間有細微的差距。

# Role and Contributions

B06902100 - wrote the main.c, the scheduling function (inside scheduler.c), helping in debugging process, helping in writing the report

B06902102 - finished writing scheduler.c, helping in debugging process, checking the results of some of the test cases.

B06902131 - finished writing process.c and get_time_ns(system call), testing the code and working on the report.

B06902135 - finished the kernel-result part, testing the code, running the test_set and working on the report.

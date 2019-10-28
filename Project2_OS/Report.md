# 1. Programming Design
In this project, We are using mmap system call to implement an I/O and compare it with the general file I/O. The code principally work like this: pass the file descriptor of the file into mmap as the starting address, and pass the file size as the mapping length. We access the file directly using virtual addresses.

Below are the descriptions for how the master part and slave part work:
  ## master
Since the master can know the file size in advance, the entire file then can be directly mapped to the virtual addresses, so that the master device can directly read the content of the files through the mapping address. With this method, we can minimize the buffer size required for the file I/O. However, it should be noted that setting up a mmap itself has overhead.

  ## slave
For the slave side, since it does not know the file size beforehand, the mapping starts with a given size, usually with an integer multiple of the page size. If it's full, immediately open a new one and we keep doing this until all of the files are finished. Each time we want to open a new mapping address, it's necessary to first write a NULL character in the corresponding offset position of the file. After the file transfer is completed, the extra offset truncate is dropped. We do this to avoid a bus error that would occur if we immediately map the open file (which is originally empty).     

# 2. Results 
  ## file1
  https://github.com/fslo1709/OS_project2/blob/master/file1.png
  ## file2
  https://github.com/fslo1709/OS_project2/blob/master/file2.png
  ## file3
  https://github.com/fslo1709/OS_project2/blob/master/file3.png
  ## file4 
  https://github.com/fslo1709/OS_project2/blob/master/file4.png
 
  #
  
These files are the files provided in http://rswiki.csie.org/dokuwiki/courses:107_2:project_2 (sample code for 4.14.25).

# 3. Performance Comparison between File I/O and Memory-mapped I/O
Based from the results that we get, we can see that file I/O takes less transmission time than memory-mapped I/O when we are dealing with a small enough data. This result holds for both master and slave side. However, for large enough inputs, memory-mapped I/O will eventually outperform file I/O. Theoretically, as the file grows larger, the difference in performance between memory-mapped I/O and file I/O would also grow bigger too (with memory-mapped I/O being the quicker one).

Why?

When we are dealing with a large enough file, memory-mapped I/O is more efficient becasuse there's less copying compared to usual I/O: mmap doesn't require a copy of the file data from kernel to user-space. In addition, memory mapping can simplify the operations by letting the code treat the entire file content as accessible. 

However, there are some cases when mmap is slower. Page faulting is quite resource-expensive and slow and that is how the mapping get populated. Hence, when we are dealing with data that is processed almost at once, that is, when the data is not in the memory, the usual file I/O would take less time.

# 4. Team Members and Contributions
B06902100 (劉益瑋) - wrote the report, helped in completion of master.c and slave.c

B06902102 (羅費南) - wrote master.c and slave.c, debugging and completion of kernel files



# basic-os
Basic operating system that simulates process life cycles, CPU scheduling, and memory allocation. Created for 
my final project in **CSCI 340 - Operating Systems.**  


## Compile my program using the command:
	make main

## Execute the generated main.exe program using the command:
	./main


## User Commands:
| Command        | Description  | 
| :---------------- |:-------------| 
| A | Create a new process.  |
|Q | Give currently running process +1 time quantum.  |
|t | Currently running process inside the CPU terminates.  |
|d *number file_name* | The process that currently uses the CPU requests the hard disk number # for a READ or WRITE to file_name|
|D *number* | Hard disk number # has finished the work for one process.|
|m *address* | Process currently using the CPU requests a memory operation for the logical address. |
|S r | Shows what process is currently using the CPU and what processes are waiting in the ready-queue.  |
|S i | Shows what processes are currently using the hard disks and what processes are waiting to use them. |
|S m | Shows the state of memory. For each used frame displays the process number that occupies it and the page number stored in it.|

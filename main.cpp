//Kevin Espinola
//CSCI 340 Section 03 Home Project
//
//
//Please READ the README.txt!

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <list>
#include "functions.h" //Custom functions and PCB class used to simulate OS.
#include "pagetable.h" //Pagetable functions used to simulate a Pagetable.

using namespace std;

//checks to see if user input is an integer, and if any spaces are found
//in the user input string, outputs an error
bool numberCheck(const string& s)
{
	for(int i=0; i<s.length(); i++)
	{
		if( !(s[i] >= '0' && s[i] <= '9' || s[i] == ' ') || s[i] == ' ')
		{
			cout << "Error in user input.\n";
			return false;
		}
	}
	return true;
}

int main()
{
	//Initialize User determined variables for the simulation.
	int pageSize = -1; //Page/frame size.
	int totalRAM = -1; //Total RAM size.
	int maxRAM = 4000000000;
	int totalHDD = -1; //Total number of hard disks.
	int PID = 1;      //Process ID number, DO NOT REUSE terminated PID #s!!

	vector <PCB> priority0; //RR scheduling, single time quantum.
	vector <PCB> priority1; //RR scheduling, double time quantum.
	vector <PCB> priority2; //FCFS scheduling.
	vector <PCB> CPU;		//CPU, queue size of one.

	string input; //User input for simulation settings.
	
	cout << endl << "How much RAM memory(in bytes) is there on the simulated computer?\n";
	while( (totalRAM > maxRAM) && (totalRAM < 0) )
	{
		cout << "Valid inputs are positive integers up to 4000000000(4 Billion).\n";
		getline(cin, input);
		if(numberCheck(input))
		{
			totalRAM = atoi(input.c_str());
		}
		else
		{
			//continue scanning user input for valid integers
		}
	}

	cout << endl << "What is the size of a page/frame on the simulated computer?\n";
	while( pageSize < 0)
	{
		cout << "The enumeration of pages starts at 0.\n";
		getline(cin, input);
		if(numberCheck(input))
		{
			pageSize = atoi(input.c_str());
		}
		else
		{
			//continue scanning user input for valid integers
		}
	}

	cout << endl << "How many hard disks does the simulated computer have?\n";
	while( totalHDD < 0)
	{
		cout << "The enumeration of hard disks starts at 0.\n";
		getline(cin, input);
		if(numberCheck(input))
		{
			totalHDD = atoi(input.c_str());
		}
		else
		{
			//continue scanning user input for valid integers
		}
	}
	vector <vector<PCB> > hdd(totalHDD);	//list of HDD queues.
	int numOfPages = totalRAM/pageSize; //i dont even really need this its used in the next cout block.

	Pagetable frametable; //main frame/page table used for memory stuff.
	frametable.setTable(numOfPages, pageSize);


	cout << endl
		 << "The simulation has started with the following settings:" << endl
		 << "Total Memory(RAM): " << totalRAM << endl 
		 << "Maximum Page Size: " << pageSize << endl
		 << "Number of pages: "   << numOfPages << endl
		 << "Hard disks on PC:  " << totalHDD << endl << endl;


	//Main loop of the project, only exit is through ctrl+c or by crashing my program..
	while(true)
	{
		getline(cin, input); //read user input.

		//"A" input means that a new process has been created. 
		if(input == "A")
		{
			//Creation of new process with priority 0, PID == current PID, and PAGE 0
			PCB process;
			process.setPID(PID);
			process.setPriority(0);
			process.setPage(0);

			frametable.updateTable(process); // Throw it into the table.

			cout << "Process: " << PID << " has entered the ready-queue.\n";

			//increment PID counter.
			PID++;

			//Process is allowed into the CPU if it is empty or it preempts the currently 
			//running process and sends it back to its respective priority queue.
			if( CPU.size() == 0)
			{
				CPU.push_back(process);
			}
			else if( CPU.size() > 0)
			{
				//case when both priorities are equal
				//send them to their respective priority# Queue.
				if(process.getPriority() == CPU[0].getPriority())
				{
					if(process.getPriority() == 0)
					{
						priority0.push_back(process);
					}
					else if(process.getPriority() == 1)
					{
						priority1.push_back(process);
					}
					else if(process.getPriority() == 2)
					{
						priority2.push_back(process);
					}
				}
				//case when priority 0 process preempts priority 1 or priority 2 process.
				else if(process.getPriority() == 0 && ((CPU[0].getPriority() == 1) || (CPU[0].getPriority() == 2))  )
				{
					//priority 0 preempts priority 1
					//priority 1 goes to head of its queue
					if((CPU[0].getPriority() == 1))
					{
						PCB preemptedProcess = CPU.back();
						CPU.pop_back();
						priority0.push_back(process);
						priority1.insert(priority1.begin(), preemptedProcess);
					}
					//priority 0 preempts priority 2
					//priority 2 goes to head of its queue
					if((CPU[0].getPriority() == 2))
					{
						PCB preemptedProcess = CPU.back();
						CPU.pop_back();
						priority0.push_back(process);
						priority2.insert(priority2.begin(), preemptedProcess);
					}
				}
			}
		}

		//"Q" input means the currently running process has spent a time quantum using the CPU.
		else if(input == "Q")
		{
			//Q shouldn't be allowed to run if the CPU is empty, if the conditions are 
			//met for proper Q execution, then we increment the time quantum.
			if( CPU.size() == 0)
			{
				cout << "The CPU is empty.\n";
			}
			else if( CPU.size() > 0)
			{
				cout << "The currently running process has spent a time quantum using the CPU.\n";
				//One quantum of time has passed by
				//update the quantum count of the process inside the CPU.
				PCB temp = CPU.back();
				CPU.pop_back();
				temp.incrQuantum();


				//handling of process demotion.
				if(temp.getPriority() == 1)
				{
					priority1.push_back(temp);
				}
				else if(temp.getPriority() == 2) //FCFS method
				{
					if( (priority2.size() > 0) && (priority0.size() == 0) && (priority1.size() == 0) )
					{
						CPU.push_back(temp);
					}
					else
					{
						priority2.push_back(temp);
					}
				}
				else if(temp.getPriority() == 0)
				{
					priority0.push_back(temp);
				}
			}
		}

		//"t" input means the process that is currently using the CPU terminates. 
		//It leaves the system immediately.
		else if(input == "t")
		{
			//Check to see if CPU is empty, if the CPU is empty and t is allowed
			//to execute, a crash occurs.
			if( CPU.size() == 0)
			{
				cout << "No processes are currently running inside the CPU.\n";
			}
			else if( CPU.max_size() > 0)
			{
				//terminate whatever is in the CPU.
				PCB terminatedProcess = CPU.back();
				CPU.pop_back();
				frametable.releaseMemory(terminatedProcess); //call function to release process memory from RAM.
				cout << "Process: ";
				terminatedProcess.displayProcess();
				cout << " that was running inside the CPU has been terminated.\n";
			}
		}

		//"d number file_name" input means the process that currently uses the CPU requests the hard disk number #.
		//it wants to read or write to file_name.
		else if(input[0] == 'd')
		{
			//parsing for d command
			//tokens[0] == d
			//tokens[1] == requested hard disk number '#'
			//tokens[2] == "file_name"
			stringstream ss(input);
			string token;
			vector <string> tokens;
			while (getline(ss, token, ' '))
			{
				tokens.push_back(token);
			}
			//Only three arguments should be entered for this command
			if(tokens.size() != 3)
			{
				cout << "Invalid user input in command 'd number file_name'.\n";
			}
			//Is the middle argument a number?
			else if(numberCheck(tokens[1]))
			{
				int requestedHdd = atoi(tokens[1].c_str());

				if(totalHDD == 0)
				{
					cout << "There are no hard disks on this PC.\n";
				}
				else if(requestedHdd >= totalHDD)
				{
					cout << "The requested hard disk does not exist, enumeration begins at 0.\n";
				}
				else if(CPU.size() == 0)
				{
					cout << "The CPU is empty.\n";
				}
				else
				{
					//send the process to the requested HDD QUEUE.
					PCB temp = CPU.back();
					temp.setFilename(tokens[2]);

					cout << "Process ";
					temp.displayProcess();
					cout << " has joined HDD Queue " << requestedHdd << " to perform memory operations on file '"
						 << tokens[2] << "'\n";
					CPU.pop_back();
					hdd[requestedHdd].push_back(temp);
				}
			}
		}

		//"D number" input means the hard disk number # has finished the work for one process.
		else if(input[0] == 'D')
		{
			//parsing for D command
			//items[0] == D
			//items[1] == requested hard disk number#
			stringstream sss(input);
			string item;
			vector <string> items;
			while (getline(sss, item, ' '))
			{
				items.push_back(item);
			}
			if(items.size() != 2)
			{
				cout << "Invalid user input in command 'D number'.\n";
			}
			else if(numberCheck(items[1]))
			{
				int targetHdd = atoi(items[1].c_str());
				if(totalHDD == 0)
				{
					cout << "There are no hard disks on this PC.\n";
				}
				else if(targetHdd >= totalHDD)
				{
					cout << "The requested hard disk does not exist, enumeration begins at 0.\n";
				}
				else if(hdd[targetHdd].size() == 0)
				{
					cout << "Hard disk #" << targetHdd << " is empty.\n";
				}
				else
				{
					//Erase the process from the QUEUE since it's done with the process
					//Send the process back to the CPU if it's empty, if it's not then
					//do some spaghetti code to figure out where it should go

					PCB temp = hdd[targetHdd][0];
					hdd[targetHdd].erase(hdd[targetHdd].begin());

					
					cout << "HDD #" << targetHdd << " has finished the work for process: ";
					temp.displayProcess();
					cout << endl;

					if(CPU.size() == 0)
					{
						CPU.push_back(temp);
					}
					else if( CPU.size() > 0)//some preemption handled here.
					{
						//case when both priorities are equal
						if(temp.getPriority() == CPU[0].getPriority())
						{
							if(temp.getPriority() == 0)
							{
								priority0.push_back(temp);
							}
							else if(temp.getPriority() == 1)
							{
								priority1.push_back(temp);
							}
							else if(temp.getPriority() == 2)
							{
								priority2.push_back(temp);
							}
						}
						//case when priority 0 process preempts priority 1 or priority 2 process.
						else if(temp.getPriority() == 0 && ((CPU[0].getPriority() == 1) || (CPU[0].getPriority() == 2))  )
						{
							//priority 0 preempts priority 1
							//priority 1 goes to head of its queue
							if((CPU[0].getPriority() == 1))
							{
								PCB preemptedProcess = CPU.back();
								CPU.pop_back();
								priority0.push_back(temp);

								priority1.insert(priority1.begin(), preemptedProcess);
							}
							//priority 0 preempts priority 2
							//priority 2 goes to head of its queue
							if((CPU[0].getPriority() == 2))
							{
								PCB preemptedProcess = CPU.back();
								CPU.pop_back();
								priority0.push_back(temp);

								priority2.insert(priority2.begin(), preemptedProcess);
							}
						}
						//case when priority 1 process returns to the ready queue and has to to deal with
						//either a priority 0 or priority 2 process in the CPU
						else if(temp.getPriority() == 1 && ((CPU[0].getPriority() == 0) || (CPU[0].getPriority() == 2))  )
						{
							//if CPU process is higher prio.
							if((CPU[0].getPriority() == 0))
							{
								priority1.push_back(temp);
							}
							//if CPU process is lower prio
							else if((CPU[0].getPriority() == 2))
							{
								PCB preemptedProcess = CPU.back();
								CPU.pop_back();
								CPU.push_back(temp);

								priority2.insert(priority2.begin(), preemptedProcess);
							}
						}
						//case when priority 2 process returns to the ready queue.
						//priority 2 can't preempt anything since it is the lowest level priority so it just
						//goes to the back of its queue.
						else if(temp.getPriority() == 2 && ((CPU[0].getPriority() == 0) || (CPU[0].getPriority() == 1))  )
						{
							priority2.push_back(temp);
						}
					}
				}
			}
		}

		//"m address" input means the process that is currently using the CPU requests a memory 
		//operation for the logical address.
		else if(input[0] == 'm')
		{
			//parsing for m command
			//tickets[0] == m
			//tickets[1] == requested memory address
			stringstream sss(input);
			string ticket;
			vector <string> tickets;
			while (getline(sss, ticket, ' '))
			{
				tickets.push_back(ticket);
			}
			if(tickets.size() != 2)
			{
				cout << "Invalid user input in command 'm address'.\n";
			}
			else if(numberCheck(tickets[1]))
			{
				int targetAddress = atoi(tickets[1].c_str());
				if(targetAddress > totalRAM-1)
				{
					cout << "Target address '" << targetAddress << "' is out of range.\n";
				}
				else
				{
					if(CPU.size() == 0)
					{
						cout << "No processes are currently running inside the CPU.\n";
					}
					else
					{
						//perform memory request.
						PCB temp = CPU.back();
						frametable.updateTable(temp, targetAddress);
					}
				}
			}
		}

		//"S r" shows what process is currently using the CPU and what processes are waiting in the ready-queue.
		else if(input == "S r")
		{
			//I should redo this if I have time..
			cout << "CPU: ";
			if( CPU.size() == 0)
			{
				cout << "Empty.";
			}
			else if( CPU.max_size() > 0)
			{
				PCB temp = CPU.back();
				temp.displayProcess();
				cout << " arrived from level " << temp.getPriority() << " queue.";
			}

			cout << endl << "Priority 0 Queue: ";
			for(int i=0; i<priority0.size(); i++)
			{
				PCB temp = priority0.at(i);
				cout << "<-";
				temp.displayProcess();
			}
			cout << endl;

			cout << "Priority 1 Queue: ";
			for(int i=0; i<priority1.size(); i++)
			{
				PCB temp = priority1.at(i);
				cout << "<-";
				temp.displayProcess();
			}
			cout << endl;

			cout << "Priority 2 Queue: ";
			for(int i=0; i<priority2.size(); i++)
			{
				PCB temp = priority2.at(i);
				cout << "<-";
				temp.displayProcess();
			}
			cout << endl;

		}

		//"S i" shows what processes are currently using the hard disks and what processes are waiting to use them.
		//for each busy hard disk shows the process that uses it and shows its I/O-queue. Also displays file_name
		//for each process.
		else if(input == "S i")
		{
			for(int i=0; i<totalHDD; i++)
			{
				cout << "HDD " << i << ": ";
				for(int j=0; j<hdd[i].size(); j++)
				{
					if(j == 0)
					{
						hdd[i][j].displayProcess();
						hdd[i][j].showFilename();
						cout << "\nHDD " << i << " Queue: ";
					}
					else
					{
						cout << "<-";
						hdd[i][j].displayProcess();
						hdd[i][j].showFilename();
					}
				}
				cout << endl;
			}
		}

		//"S m" shows the state of memory. For each used frame displays the process number that occupies it and 
		//the page number stored in it.
		else if(input == "S m")
		{
			frametable.printTable();
		}
		else
		{
			cout << "Error in user input: " << input << endl
				 << "Please check your input carefully and try again." << endl;
		}

		//The CPU is empty and a new process from the queue should take over now.
		while(CPU.size() == 0)
		{
			PCB nextProcess;
			if(priority0.size() != 0)
			{
				nextProcess = priority0[0];
				//frametable.updateTable(nextProcess, nextProcess.getPage());
				priority0.erase(priority0.begin());
				CPU.push_back(nextProcess);
			}
			else if(priority1.size() != 0)
			{
				nextProcess = priority1[0];
				//frametable.updateTable(nextProcess, nextProcess.getPage());
				priority1.erase(priority1.begin());
				CPU.push_back(nextProcess);
			}
			else if(priority2.size() != 0)
			{
				nextProcess = priority2[0];
				//frametable.updateTable(nextProcess, nextProcess.getPage());
				priority2.erase(priority2.begin());
				CPU.push_back(nextProcess);
			}
			break;
		}

		/*
		
		Mostly unused preemption system I was trying to implement but since I used vectors instead
		of std::dequeues this started to get really tricky so I'm just leaving it here for myself
		for inspiration..

		if(CPU.size() > 0)
		{
			PCB check = CPU.back();
			//if level 0 queue is full and a lower priority processor is 
			if( (priority0.size() > 0 ) && check.getPriority() == 1 )
			{
				PCB temp = check;
				CPU.pop_back();
				priority1.push_back(temp);
			}
			else if( (priority0.size() > 0 ) && check.getPriority() == 2 )
			{
				PCB temp = check;
				CPU.pop_back();
				priority2.push_back(temp);
			}
		}
		*/
		cout << endl;
	}

	return 0;
}
//Kevin Espinola
//CSCI 340 Section 03 Home Project
//
//
//Please READ the README.txt!

#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "functions.h"

using namespace std;

class Pagetable
{
public:
	Pagetable():PID(1), pageNum(0){};

	//Standard setter functions.
	void setPage(int p);
	void setPID(int inP);
	void setTimeStamp(int ts);
	//a little more complicated
	void setTable(int fMax, int fSize);

	//Standard getter functions.
	int getPage();
	int getPID();
	int getTimeStamp();

	void plusTime(); //increment timeStamp.

	//both of the next functions implement least recently used by tracking
	//the smallest timeStamp value encountered.
	void updateTable(PCB process); //lazy implementation of updateTable

	//lazy implementation of updateTable that also takes a target address
	//used in the 'm Address' command
	void updateTable(PCB process, int m);

	//used in "t" command, searches the generated realTable and deletes all matching entries
	//associated with passed process.
	void releaseMemory(PCB process);

	//prints the current table, used in "S m"
	void printTable();

private:
	int PID;
	int pageNum;
	int pageSize;
	vector <Pagetable> realTable; //im not particulary proud of this implementation but it works?
	int maxFrames;
	int timeStamp = 1;

};

void Pagetable::releaseMemory(PCB process)
{
	for(int i=0; i<maxFrames; i++)
	{
		//search for matching process values and deletes them.
		if(realTable[i].getPID() == process.getPID())
		{
			realTable[i].setPID(-1);
			realTable[i].setPage(-1);
			realTable[i].setTimeStamp(-1);
		}
	}
}

void Pagetable::plusTime()
{
	timeStamp++;
}

void Pagetable::updateTable(PCB process, int m)
{
	PCB temp = process;

	//simple maths for finding the target page associated with
	//the requested memory address associated with memory address 'm'
	int targetPage = m/pageSize;
	temp.setPage(targetPage);

	int lru = 0; //tracks smallest value encountered.
	for(int i=0; i<maxFrames; i++)
	{
		//iterate through frame table
		//really lazy way of finding the least recently used value.
		if(realTable[i].getPage() == -1)
		{
			realTable[i].setPID(temp.getPID());
			realTable[i].setPage(temp.getPage());
			realTable[i].setTimeStamp(timeStamp);
			timeStamp++;
			return;
		}
		else if(realTable[i].getPID() == process.getPID())
		{
			if(realTable[i].getPage() == targetPage)
			{
				//at this point in the search a match is 100% valid
				realTable[i].setPID(temp.getPID());
				realTable[i].setPage(temp.getPage());
				realTable[i].setTimeStamp(timeStamp);
				timeStamp++;
				return;
			}
			else if(realTable[lru].getTimeStamp() > realTable[i].getTimeStamp())
			{
				//no matches found so keep track of the least used value
				lru = i;
			}
		}
		else if(realTable[lru].getTimeStamp() > realTable[i].getTimeStamp())
		{
			//no matches found so keep track of the least used value
			lru = i;
		}
	}
	realTable[lru].setPID(temp.getPID());
	realTable[lru].setPage(temp.getPage());
	realTable[lru].setTimeStamp(timeStamp);
	timeStamp++;
	return;
}

void Pagetable::updateTable(PCB process)
{
	PCB temp = process;

	int lru = 0;
	for(int i=0; i<maxFrames; i++)
	{
		//iterate through frame table
		//really lazy way of finding the least recently used value.
		if(realTable[i].getPage() == -1)
		{
			realTable[i].setPID(temp.getPID());
			realTable[i].setPage(temp.getPage());
			realTable[i].setTimeStamp(timeStamp);
			timeStamp++;
			return;
		}
		else if(realTable[lru].getTimeStamp() > realTable[i].getTimeStamp())
		{
			//no matches found so keep track of the least used value
			lru = i;
		}
	}
	realTable[lru].setPID(temp.getPID());
	realTable[lru].setPage(temp.getPage());
	realTable[lru].setTimeStamp(timeStamp);
	timeStamp++;
	return;
}

void Pagetable::printTable()
{
	for(int i=0; i<realTable.size(); i++)
	{
		cout << "Frame: |" << i 
			 << "| Page: |" << realTable[i].getPage() 
			 << "| PID: |" << realTable[i].getPID()
			 << "| Timestamp: |" << realTable[i].getTimeStamp() 
			 << "| " << endl;
	}
	
}

void Pagetable::setTable(int fMax, int fSize)
{
	//builds the frametable given a maximum RAM size
	//and a minimum page size.
	pageSize = fSize;
	maxFrames = fMax;
	for(int i=0; i<maxFrames; i++)
	{
		//set all values to -1
		Pagetable frame;
		frame.setPID(-1);
		frame.setPage(-1);
		frame.setTimeStamp(-1);

		realTable.push_back(frame);
	}
}

void Pagetable::setPage(int p)
{
	pageNum = p;
}

void Pagetable::setPID(int inP)
{
	PID = inP;
}

void Pagetable::setTimeStamp(int ts)
{
	timeStamp = ts;
}

int Pagetable::getPage()
{
	return pageNum;
}

int Pagetable::getPID()
{
	return PID;
}

int Pagetable::getTimeStamp()
{
	return timeStamp;
}


#endif //PAGETABLE_H
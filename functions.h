//Kevin Espinola
//CSCI 340 Section 03 Home Project
//
//
//Please READ the README.txt!

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

using namespace std;

class PCB
{
public:
	PCB():PID(0), priority(0), filename(""){};	//Default empty constructor.
	//PCB(int PID, int priority); i forgot why I put this here.

	//Standard setter functions.
	void setPID(int PID);
	void setPriority(int priority); 
	void setFilename(string filename);
	void setPage(int pag);

	//Standard getter functions
	int getPage();
	int getPriority();
	int getPID();
	void getQuantum();

	//functions used for printing to user.
	void showFilename();
	void displayProcess();



	int incrQuantum();	//Increment time quantum of the process by 1.

private:
	int PID; //internal process PID
	int priority; //track priority
	int timeQuantum = 0;
	string filename;

	int page; //used for comparisons in pagetable searching.

};


void PCB::setPID(int a)
{
	PID = a;
}

void PCB::setPriority(int a)
{
	priority = a;
}

void PCB::setPage(int pag)
{
	page = pag;
}

void PCB::setFilename(string s)
{
	filename = s;
}

int PCB::getPriority()
{
	return priority;
}

int PCB::getPID()
{
	return PID;
}

int PCB::getPage()
{
	return page;
}

int PCB::incrQuantum()
{
	timeQuantum++;
	if(timeQuantum == 1)
	{
		priority = 1;
	}
	else if(timeQuantum > 1)
	{
		priority = 2;
	}
}

void PCB::getQuantum()
{
	cout << timeQuantum << endl;
}

void PCB::showFilename()
{
	cout << "{filename: " << filename << "} ";
}

void PCB::displayProcess()
{
	cout << "[" << PID << "]";
}


#endif //FUNCTIONS_H
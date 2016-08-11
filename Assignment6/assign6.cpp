#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<iomanip>
#include<vector>
#include<set>
#include "Frame.h"
/*
NAME: CODY HAMMOND
Z-ID: Z1615158
ASSIGNMENT 6
*/

//function prototypes
void printStatus(bool,std::vector<int>&,int);
void printHeader(unsigned);
void FIFO(std::ifstream&, std::vector<Frame>&);
void LRU(std::ifstream&, std::vector<Frame>&); 

//global variables
typedef std::vector<Frame>::iterator iterator;
iterator iter;
unsigned frames, pages;
int pageNum;
unsigned counter = 0;

int main()
{	
	//ifstream object to open file
	std::ifstream file("data6.txt");

	if (!file.is_open())//check to see if the file has opened
	{
		std::cout << "File did not open. Terminating program.";
		exit(-1);
	}

	char mode=' ';//inititalize variable
	while (mode != '?')//loop while the delimiter has not been encountered
	{
		file >> mode;//read in the mode
		
		if (mode == 'F')//if mode is F
		{
			
			std::vector<Frame>TLB;//create vector of frames
			FIFO(file,TLB);//call FIFO method
			Frame::FrameCount = -1;//set frame count
			std::cout << "\n";
		}
		else if (mode == 'L')//if mode is L
		{
			std::vector<Frame>TLB;//create vector of frames
			LRU(file, TLB);//call LRU method
			Frame::FrameCount = -1;//set frame count
			std::cout << "\n";
		}
		else
			break;//break 
	}
	file.close();
}

//Function to print the header of the report accepts an integer argument to determine header title 
void printHeader(unsigned choice)
{
	counter = 0;//set counter to 0 for steps
	if (choice == 0)// if 0 dispay FIFO header
		std::cout << "\nFIFO IMPLEMENTATION\n";
	else//otherwise display LRU header
		std::cout << "\nLRU IMPLEMENTATION\n";
	//display more information
	std::cout << "\nSTEP" << "   " << "PAGE" << "    ";

	//display frame headers
	for (unsigned i = 0; i < frames; i++)
	{
		std::cout << "FRAME " << i + 1 << "  ";
	}
	std::cout << "PAGE FAULT";//display page fault header
}

//Function to print the contents of the vector and page fault result.
//function accepts a bool argument to determine if a fault has occured and a vector reference
void printStatus(bool fault,std::vector<Frame>& frames,int num)
{
	std::string f;//string 
	

	std::cout << "\n";
	
	//display step number and page numer
	std::cout << std::setw(3) << counter <<"     " << pageNum << "    ";
	//display values of each frame
	for (unsigned i = 0; i < frames.size(); i++)
	{
		if (frames[i].value != -1)
			std::cout<<"    "<<frames[i].value<<"    ";
		else
			std::cout << "    " << "     ";
	}
	//display page fault
	if (fault)//if true
		std::cout << "    " << "NO";
	else
		std::cout << "    " << "YES, in frame " << num; //if false

	counter++;//increment step
	
}


//This function holds the LRU logic,accepts a reference to a ifstream object and a vector object
void LRU(std::ifstream& file, std::vector<Frame>& TLB)
{
	file >> pages;//read amount of pages
	file >> frames;//read amount of frames
	TLB.resize(frames);//resize vector accordingly
	std::set<int>history;//  a set used to keep track of the lowest counter used to determine which frame was least recently used
	printHeader(1);//call function and pass 1 for LRU header

	bool flag = false;//set flag to false
	
	//for loop to read
	for (unsigned j = 0; j < 20; j++)
	{
		file >> pageNum;//read in page number
		if (pageNum == -1)//if page number equal to -1 skip iteration of loop
			continue;

		//for loop used to search frames the page number
		for (unsigned i = 0; i<TLB.size(); i++)
		{
			if (TLB[i].value == pageNum)//if found
			{
				TLB[i].counter = j;//set counter to loop counter
				flag = true;//set flag to true
				printStatus(flag, TLB,i);//print contents
				break;
			}
		}
		

		if (!flag)//if page number was not found
		{
			//search the vector for frame counter equal to -1 and initialize frame
			for (unsigned i = 0; i < TLB.size(); i++)
			{
				if (TLB[i].counter == -1)
				{
					TLB[i].counter = j;//set counter to loop counter
					TLB[i].value = pageNum;//set frame value to page number
					flag = true;//set flag to true

					printStatus(false, TLB, i);//print contents
					break;
				}
			}

			//if flag is false
			if (flag == false)
			{
				//put all frame counters into set
				for (unsigned i = 0; i < TLB.size(); i++)
					history.insert(TLB[i].counter);

				//search vector of counter equalling to the smallest number in set
				for (unsigned i = 0; i < TLB.size(); i++)
				{
					if (TLB[i].counter == *history.begin())// if found
					{
						TLB[i].counter = j;//set frame counter to loop counter
						TLB[i].value = pageNum;//set value to page number

						printStatus(flag, TLB, i);//print contents
						break;
					}
				}
				history.clear();//clear set history
			}
		}
		
		flag = false;//set flag to false
		
	}

}

//Function containing the FIFO logic,accepts a reference to an ifstream object and a vector
void FIFO(std::ifstream& file, std::vector<Frame>& TLB)
{
	//read in pages and frames
	file >> pages;
	file >> frames;
	//print FIFO header
	printHeader(0);
	
	TLB.resize(frames);//resize vector accordingly
	bool flag = false;//set flag to false

	//These two variables are used to loop through vector for FIFO implementation
	int curr = -1;
	int next = 0;

	//read in rest of file
	for (unsigned i = 0; i < 20; i++)
	{
		file >> pageNum;//read page number
		if (pageNum == -1)//if page number equal to -1 skip this iteration
			continue;

		//loop through vector to find a match with the page number
		for (iter = TLB.begin(); iter != TLB.end(); iter++)
		{
			if (pageNum == iter->value)
			{
				flag = true;//if found set flag to true
				break;
			}
		}

		if (flag == false)//if page not found
		{
			while(true)
			{
				curr = (curr + 1) % TLB.size();//curr  and next variable is made to loop through the range 0 - TLB.size();
				next = (next + 1) % TLB.size();
				if (TLB[curr].counter <= TLB[next].counter)//if the current frame is less than or equal to the next frame, replace the current frame
				{
					TLB[curr].value = pageNum;//assign page num to value
					TLB[curr].counter=i;//assign i to counter;
					printStatus(flag, TLB,curr);//print contents
					break;
				}
			}
		}
		
		flag = false;//set flag to false
		
		
	}
	
}
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iomanip>

using namespace std;

#define TABLESIZE 100
#define KEYSIZE 4
#define EMPTYKEY "----"
#define DATAFILE "P4DATA.TXT"



//Define the structure for use in the hash table
struct HashStruct
	{
		char key [5];
		int data;
	};

//Three Hash functions
	//Number 1 : Truncation ("BC") % TABLESIZE
	int Hash_1(char*key)
	{
		int hashVal;

		hashVal = ((key[1] * key[2]) % TABLESIZE);
		//cout<<"Truncation hash compelted."<<endl;
		//cout<<hashVal;


		return hashVal;
	};


	//Number 2 : Folding ("AB" + "CD") % TABLESIZE
	int Hash_2(char *key)
	{
		int hashVal;

		hashVal = ((key[0] * key[1]) + (key[2] * key[3])) % TABLESIZE;
		//cout<<"Folding hash completed."<<endl;
		//cout<<hashVal;

		return hashVal;
	};
	
	//Number 3 : Middle Squaring ("B+C"*"B+C")% TABLESIZE
	int Hash_3(char*key)
	{
		int hashVal;

		hashVal = ((key[1] + key[2])*(key[1] + key[2])) % TABLESIZE;
		//cout<<"Middle Squaring hash completed."<<endl;
		//cout<<hashVal;

		return hashVal;
	};

	//Three Double Hash function
	//Number 1 : Linear Probing
	int ProbeDec_1(char*key)
	{
		return 1;
	};

	//Number 2 : Double Hash 
	int ProbeDec_2(char*key)
	{
		int dHashVal;

		dHashVal =10*((key[0] + key[1]) /TABLESIZE); 

		return dHashVal;
	};

	//Number 3
	int ProbeDec_3(char*key)
	{
		int dHashVal;

		dHashVal = (int)((key[0]) + key[1] / TABLESIZE);

		return dHashVal;
	};

//===================
	/* --- Snippet 5: Hash Insert function testNum is a number from 0 through 8 indicating
				which test is being run. This can be calculated from hashNum and dHashNum in 
				snippet 2, for example: 
				testNum = (hashNum * 3) + dHashNum. ---*/

int HashInsert(HashStruct T[], char*key, int data, int hNum, int dhNum)
{
		int testNum = (hNum * 3) + dhNum;
		int colCount = 0; 
		int hashIndex, probeDec;

		switch(testNum)
		{
			case 0: //Hash function 1 -- Double has 1 (linear probing)
				hashIndex = Hash_1(key);
				probeDec = ProbeDec_1(key); //Function just returns 1
				break;
			case 1: //Hash function 1 -- Double hash 2
				hashIndex = Hash_1(key);
				probeDec = ProbeDec_2(key);
				break;
			case 2: //Hash function 1 -- Double hase 3
				hashIndex = Hash_1(key);
				probeDec = ProbeDec_3(key);
				break;
			case 3: //Hash function 2 -- Double hash 1 (linear probing)
				hashIndex = Hash_2(key);
				probeDec = ProbeDec_1(key); //Fucntion just returns 1
				break;
			case 4: //Hash function 2 -- Double hash 2
				hashIndex = Hash_2(key);
				probeDec = ProbeDec_2(key);
				break;
			case 5: //Hash function 2 -- Double hash 3
				hashIndex = Hash_2(key);
				probeDec = ProbeDec_3(key);
				break;
			case 6: //Hash function 3 -- Double hash 1 (linear probing)
				hashIndex = Hash_3(key);
				probeDec = ProbeDec_1(key); //Function just returns 1
				break;
			case 7: //Hash function 3 -- Double hash 2
				hashIndex = Hash_3(key);
				probeDec = ProbeDec_2(key);
				break;
			case 8: //Hash function 3 -- Double hash 3
				hashIndex = Hash_3(key);
				probeDec = ProbeDec_3(key);
				break;
		}

		//Find a place to insert into the table
		while(strcmp(T[hashIndex].key, EMPTYKEY)!=0)
		{
			colCount++;
			hashIndex -= probeDec; //Decrementing was chosen you could also choose to
			if(hashIndex <0) //increment and wrap back to the beginning of the table
				hashIndex = hashIndex + TABLESIZE;
		}
		
		strcpy(T[hashIndex].key, key);
		T[hashIndex].data = data;
		return colCount;
}

	

	

//=====================
/* --- Snippet 4: Initialize table function --- */
void InitTable (HashStruct hashT[], int TableSize)
{
	int i;

	for (i=0; i<TableSize; i++)
	{
		strcpy(hashT[i].key, EMPTYKEY);
		hashT[i].data = 0;
	}
}

int main (void)
{
	//==================
	/* --- Snippet 1: Partial list of variables defined in main() --- */

	int hashNum, dHashNum, count; 
	ifstream *inFile;
	HashStruct T[100];
	char line[64];
	char key[5];
	int data;

	//=================
	/* --- Snippet 2: The following code can be used to perform the 9 tests. Use hashNum
					to increment a loop for each of 3 hash function and dHashNum to increment a nestd
					loop for each of 3 double hash functions. --- */


	//For each of three hash functions
	for(hashNum = 0; hashNum <3; hashNum++)
	{
		//For each of three double has functions
		for(dHashNum = 0; dHashNum < 3; dHashNum ++)
		{
			InitTable( T, TABLESIZE); //Call function to initialize table to empty
			inFile = new ifstream();
			inFile -> open(DATAFILE, ifstream::in); //Open data file for this test
			if(!inFile -> is_open())
			{
				cout<<"Unable to open data file. Program terminating\n"<<endl;
				return 0;
			}

			count = 0; //Initialize collision counter

			for(int i = 0; i < 50; i++)
			{
				inFile->getline(line, 64, '\n');
				sscanf(line, "%s %d", key, &data);
				count += HashInsert(T, key, data, hashNum, dHashNum);
				//Note: The HashInsert() function uses the indices of hashNum and
				//dHashNum to know which hash function and double hase function to call
				//in this test.
			}
			inFile->close(); //close the text file
			delete inFile;

				//======================
			/* --- Snippet 3: This code shows how to create a diagram of the results of a single test
			using one hash function and one double hash function. The resulting diagram looks simimlar to the 
			sample below with '|' representing a slot where a key hashed or doulbe hashed to and '-' representing 
			an empty slot.
			||-||||---|||-|-|-||||||||-----||-|-||---||||--|-||||...etc. ---*/
			cout<< "Testing hash function " << hashNum << " using double hash " << dHashNum<< ".\n" <<endl;
			cout<< "Total collisions = " <<count << ".\n";
			//Show the form of the array
			for (int i = 0; i < 100; i++)
				if(strcmp(T[i].key, EMPTYKEY)) //stcmp gives a non-zero (true) result if Key is  NOT the EMPTYKEY
					cout << "|"; //Indicate an entry at this location
				else 
					cout << "-"; //Indicate no entry at this location.
				cout << "\n\n";
				
		}
	}
		

	
	system ("pause");
	return 0;
	
}



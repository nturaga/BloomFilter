/*
 * Bloom-Test.cpp
 *
 *  Created on: Jul 6, 2014
 *      Author: niteshturaga
 */

#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#include "bloom_filter.hpp"

#define datafile "/Users/niteshturaga/Documents/BenLangmeadResearch/BloomFilter/data/chrI_1_kmerized.fa"

// Function Prototypes
void GetNextLine(ifstream& inFile, char *line, int lineLen);

int main(int argc, char **argv)
{

	// Define bloom Filter parameters
	bloom_parameters parameters;
	// How many elements roughly do we expect to insert?
	parameters.projected_element_count = 10000;
	// Maximum tolerable false positive probability? (0,1)
	parameters.false_positive_probability = 0.001; // 1 in 10000
	// Simple randomizer (optional)
	parameters.random_seed = 0xA5A5A5A5;
	if (!parameters) {
		std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
		return 1;
	}
	parameters.compute_optimal_parameters();
	//Instantiate Bloom Filter
	bloom_filter filter(parameters);

	ifstream inFile;      // Input file stream
	char str[128];        // Character array to hold lines read from the file

	cout << "Opening file...\n";
	// Open the file for reading,
	inFile.open(datafile, ifstream::in);

	//	TODO : Remove later
	// checking to make sure it was successfully opened
	if(!inFile.is_open())
	{
		cout << "Unable to open file" << inFile << "\nProgram terminating...\n";
		return 0;
	}

	// File was successfully opened so call the C++ function to read
	//  all lines from the file.
	cout << "Printing lines in file...\n\n";
	while(true)
	{
		GetNextLine(inFile, str, strlen(str));
		if(inFile.good()) {
			//			cout << str << "\n";    // Print the line
			filter.insert(str);
			//			cout <<"Inserted into filter"<<endl;
		}
		else{
			break;
		}// terminate the loop
	} // end eternal while() loop
	//	inFile.close();                        // Close the data file
	cout << "\n\ndone...\n\n";


	// Query Bloom Filter
	// Query the existence of kmers in BF
	cout << "Check existence in BF"<<endl;
	std::string str_list[] = { "gcctaagcctaagcctaagc","cctaagcctaagcctaagcc","ctaagcctaagcctaagcct"};

	for (std::size_t i = 0; i < 3; ++i)
	{
		if (filter.contains(str_list[i]))
		{
			std::cout << "BF contains: " << str_list[i] << std::endl;
		}
	}

	return 0;

}



/**************************************************************/
/* GetNextLine() - Wrapper function                           */
/*                                                            */
/* Purpose: Read a line from a data file.                     */
/* Args: inFile -- istream pointer                            */
/*       line -- Character array to place the line in.        */
/*       lineLen -- Length of the character array line.       */
/* Returns: void                                              */
/**************************************************************/
void GetNextLine(ifstream& inFile, char *line, int lineLen)
{
	int done = false;
	while(!done)
	{
		inFile.getline(line, 128);
		if(inFile.good())    // If a line was successfully read
		{
			if(strlen(line) == 0)  // Skip any blank lines
				continue;
			else if(line[0] == '#')  // Skip any comment lines
				continue;
			else done = true;    // Got a valid data line so return with this line
		}
		else
		{
			strcpy(line, "");
			return;
		}
	}
}


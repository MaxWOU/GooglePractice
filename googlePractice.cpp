// googlePractice.cpp : this is just a program to solve a youtube "google question" problem.
// it basically solves the following problem:

/*
	In any given list of Block objects
	ea. block will have a few locations (always the same)
	find the BEST block with each location, in units, summed up
	to be the closest from your current location.

	This means, if block 3 has a school and gym but no store
	the algorithm will search for the nearest school.
	if it finds a school 3 blocks away the total miles will be
	3 and it will be the best location (so long as there isn't a
	better one.)

	one better location might be a block with all 3 locations OR
	a block with 2 locations and the other one being just 1 or 2
	miles away.

	Link for the problem: youtu.be.com/rw4s4M3hFfs
	Titled: "Google coding interview with a normal software engineer."
*/

#include <vector>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

/*
	a block can hold 1-N locations. True meaning the location exists here
	and false meaning it's baron.

	My program aims to have triple false that's 0 0 0 MORE OFTEN than
	the already reduced chances of a 1 1 1.

*/
class Block {
public:
	bool gym = false; //some random location of interest
	bool school = false; //random location
	bool store = false;

	Block() : gym(rand() % 2), school(rand() % 2), store(rand() % 2)
	{
		if (rand() % 3 != 2)
		{
			if (gym && school && store) // more likely than not we will have a rich block
			{//this just reduces the chances of us getting a block with all 3 locations

				gym = rand() % 2;
				school = rand() % 2;
				store = rand() % 2;
			}
		}
		else { // 1/3rd of the time we will have a baron block
			gym = 0;
			school = 0;
			store = 0;
		}
	}
};

/*
total complexity of this function is
O(n^2) at worst; and O(n) at best
where if we have 10 blocks
we would've checked nearly
100 blocks after finishing.
*/
int returnBestBlock(vector<Block> l)
{
	int length = l.size(); // size of our region

	int gym_check, school_check, store_check; // semi-local-semi-global gym BOTH-WAYS distance counters

	gym_check = school_check = store_check = l.size() + 1; //both-way counter, if > than size+1, not found yet

	int last_best_block = -1; //our last best block is subject to change, but only changes if
	//we find a smaller total miles from our house number.

	//these bools determine whether or not I found a location or not.
	bool f_school, f_gym, f_store;
	f_school = f_gym = f_store = false;
	//--------

	//total miles, the less the better.
	int total_miles = length * 3; //total miles is the amount of miles all the stores combined will be from our house.
	//-------


	int current_total = length * 3;// our current total will always start x times the length
	//where x is the number of locations we're favoring.

	for (int i = 0; i < length; i++) // complexity of this outer loop is N
	{
		int length_from_source = 0; // length from source is a local length checker
		//which is responsable for labeling the closest location to any given block
		//which we are checking. 

		//the first j will be our LEFT side of the list, checking everything on the left side
		//of the current block being checked.
		for (int j = i; j > 0; j--) // complexity of this inner loop is N with N*3 checks
		{
			//if our length from source is smaller than the last recorded gym distance
			//and a gym is found at this location, we're gonna lock it in.
			if (l.at(j).gym && length_from_source < gym_check)
			{
				gym_check = length_from_source;
				f_gym = true; //marking that we found a gym.
			}

			//same as here. 
			if (l.at(j).school && length_from_source < school_check)
			{
				school_check = length_from_source;
				f_school = true; // if this is false or any other f_location(s), 
				//we will handel that with a few messages
			}

			if (l.at(j).store && length_from_source < store_check)
			{
				store_check = length_from_source;
				f_store = true;
			}


			length_from_source++; // length from source increments inside this inner loop.
			//if this didn't increment we wouldn't be able to tell whether or not
			//a location is closer or not from our current block i
		}

		length_from_source = 0; // resetting length from source for our RIGHT side of 
		//list check

		//this is to check all the blocks on the RIGHT side of our current block
		for (int j = i; j < length; j++)
		{

			if (l.at(j).gym && length_from_source < gym_check)
			{
				gym_check = length_from_source;
				f_gym = true;
			}


			if (l.at(j).school && length_from_source < school_check)
			{
				school_check = length_from_source;
				f_school = true;
			}


			if (l.at(j).store && length_from_source < store_check)
			{
				store_check = length_from_source;
				f_store = true;
			}

			length_from_source++;
		}

		//setting our current total to see if we can later get a smaller total.
		current_total = store_check + school_check + gym_check;


		//if our current total is smaller than our last recorded total, change it.
		if (current_total < total_miles)
		{
			total_miles = current_total; // record a new smallest total
			last_best_block = i;//record the block we were on during the smallest total
		}

		//reset our distances so we can get clean numbers for our next block checks
		gym_check = school_check = store_check = l.size() + 1;
	}

	//visual helper to see what's going on in each block
	for (int i = 0; i < length; i++)
	{
		cout << "=====Block: " << i << ":" << "====" << endl;
		cout << "\tgym: " << l.at(i).gym << endl;
		cout << "\tschool: " << l.at(i).school << endl;
		cout << "\tstore: " << l.at(i).store << endl << endl << endl;
	}


	if (!f_store || !f_school || !f_gym) // if any of the locations weren't found
	{
		cout << "It appears that one of your priority locations doesn't exist." << endl;
		cout << "Please look for another region to find your block. Thank you." << endl;

		total_miles = 0; // reset total miles since before we counted our place holders


		if (!f_store)
			cout << "Missing location: STORE" << endl;
		total_miles += f_store;

		if (!f_gym)
			cout << "Missing location: GYM" << endl;
		total_miles += f_gym;

		if (!f_school)
			cout << "Missing location: SCHOOL" << endl;
		total_miles += f_school;
	}


	cout << total_miles << " driving miles total." << endl;


	return last_best_block;
}



int main()
{
	vector<Block> blocks;
	srand(time(0)); //randomize block location popularity

	for (int i = 0; i < 10; i++)
	{
		Block b = Block();
		blocks.push_back(b); // pushing a number of blocks into our vector
	}



	cout << returnBestBlock(blocks) << " is the best block!" << endl;

	return 0;
}



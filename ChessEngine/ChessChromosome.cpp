/*
 *  ChessChromosome.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-02-27.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ChessChromosome.h"
#include "assert.h"
#include <string>
#include <math.h>
#include "ChessBoard.h"
#include <fstream>

using namespace std;

void Chromosome::LoadChromosome()
{
	char filename[50];
	sprintf(&filename[0], "Chromosome%i.chr", ChromosomeNumber);
	
	char nextNum[10];
	ifstream myfile;
	
	myfile.open(filename, ifstream::in);
	
	if (myfile.fail() )
	{
		//failed to open
		ChessAssert();
	}
	
	for (int i = 0; i < NUM_EVALNODES; i++)
	{
		if (myfile.good())
		{
			int index = 0;
			int decimalPlace = 0;
			char nextChar = 'b';
			while (nextChar != '\n' && nextChar != '\0' && nextChar != ' ')
			{
				myfile.get(nextChar);
				nextNum[index] = nextChar;
				if (nextChar == '.')
					decimalPlace = index;
				index++;
			}
			float tally = 0;
			for (int currIndex = 0 ; currIndex < index-1; currIndex++)
			{
				if (nextChar == '.')
					continue;
				if (decimalPlace != 0 && i > decimalPlace)
				{
					tally += (nextNum[currIndex]-'0') * pow(10, decimalPlace - currIndex);
				}	
				else 
				{
					tally += (nextNum[currIndex]-'0') * pow(10, (decimalPlace == 0) ? (index - 2 - currIndex) : (decimalPlace - 2 - currIndex));	
				}
			
			}
			printf("Tally %f\n", tally);
			Weightings[i] = tally;
		}
		else 
		{
			//Bad file
			ChessAssert();
		}
	}
	
	myfile.close();
	
	bIsLoaded = true;
}

void Chromosome::SaveChromosome()
{
	
	char filename[50];
	sprintf(&filename[0], "Chromosome%i.chr", ChromosomeNumber);
	
	ofstream myfile;
	
	myfile.open(filename, ofstream::out);
	
	for (int i = 0; i < NUM_EVALNODES; i++)
	{
		myfile << Weightings[i] <<endl;
	}
	
	myfile.close();
}

Chromosome* Chromosome::SinglePointCrossoverWithChromosome(Chromosome* Mutator)
{
	//srand( (unsigned)time(0) ); srand should have been called already
	Chromosome* ret = new Chromosome(10); // NEED NUM HERE
	for (int i = 0; i < NUM_EVALNODES; i++)
	{
		//In single point crossover, we select a random point
		//Before the random point, take from A, take rest from B
		int random_integer = (rand() %10); //32 bit integer
		int mask = 1;
		for (int j = 0 ; j < random_integer; j++)
		{
			mask = mask<<1;
			mask |= 1;
		}
		int newWeighting = (Weightings[i]&mask) | (Mutator->GetWeighting(i)& ~mask);
		ret->SetWeighting(i, newWeighting);
	}
	return ret;
}

Chromosome* Chromosome::MultiplePointCrossoverWithChromosome(Chromosome* Mutator)
{
	//srand( (unsigned)time(0) ); srand should have been called already
	Chromosome* ret = new Chromosome(10); // NEED NUM HERE
	for (int i = 0; i < NUM_EVALNODES; i++)
	{
		//In single point crossover, we select a random point
		//Before the random point, take from A, take rest from B
		int random_integer1 = (rand() %10); //32 bit integer
		int random_integer2 = (rand() %10);
		while (random_integer1 == random_integer2)
			random_integer2 = (rand() % 10);

		int mask = 1;
		//if (random_integer1 > random_integer2)
		{
			for (int j = 0 ; j < abs(random_integer1-random_integer2); j++)
			{
				mask = mask<<1;
				mask |= 1;
			}
			int nextShift = ((random_integer2 > random_integer1) ? random_integer1 : random_integer2);
			mask = mask << nextShift;
		}
		
		int newWeighting = (Weightings[i]&mask) | (Mutator->GetWeighting(i)& ~mask);
		ret->SetWeighting(i, newWeighting);
	}
	return ret;
}

Chromosome* Chromosome::UniformPointCrossoverWithChromosome(Chromosome* Mutator)
{
	Chromosome* ret = new Chromosome(10);
	for (int i = 0 ;i < NUM_EVALNODES; i++)
	{
		int Weighting = 0;
		int currMutatorWeighting = Mutator->GetWeighting(i);
		for (int j = 0; j < 32; j++) //32 bits in an int
		{
			int mask = 1 << j;
			bool bOne = (rand() &1);
			if (bOne)
			{
				Weighting |= mask & Weightings[i];
			}
			else {
				Weighting |= mask & currMutatorWeighting;
			}
		}
		ret->SetWeighting(i, Weighting);
	}
	return ret;
}

Chromosome* Chromosome::ArithmeticCrossoverWithChromosome(Chromosome* Mutator)
{
	//srand( (unsigned)time(0) );	 srand should have been called already
	bool bAnd = (rand() &1);
	
	//0 doesn't really matter, updating weights anyway
	Chromosome* ret = new Chromosome(0);
	
	if (bAnd)
	{
		for (int i = 0; i < NUM_EVALNODES; i++)
		{
			int newWeighting = (Weightings[i] & Mutator->GetWeighting(i));
			ret->SetWeighting(i, newWeighting);
		}
	}
	else {
		for (int i = 0; i < NUM_EVALNODES; i++)
		{
			int newWeighting = (Weightings[i] | Mutator->GetWeighting(i));
			ret->SetWeighting(i, newWeighting);
		}
	}
	return ret;
}

void Chromosome::BitStringMutation(int WeightingIndex)
{
	//srand( (unsigned)time(0) );	 srand should have been called already
	int topBit;
	//for (int i = 0; i < NUM_EVALNODES; i++)
	{
		topBit = 0;
		int mask = 1;
		int tempWeighting = Weightings[WeightingIndex];
		for (int j = 0; j < 32; j ++)
		{
			if(mask & tempWeighting)
			{
				topBit = j;
			}
			mask = mask << 1;
		}
		int random_bit;
		if (topBit != 0)
			random_bit = rand() % topBit;
		else {
			random_bit = 1;
		}

		int newBit = (~((1 << random_bit) & tempWeighting));
		int newWeighting = (tempWeighting & (~ (1 << random_bit) )) | newBit;
		//Nothing bigger than 50
		newWeighting = newWeighting % 50;
		//Nothing negative
		if (newWeighting < 0)
			newWeighting = 0;
		SetWeighting(WeightingIndex, newWeighting);
	}
}

void Chromosome::FlipBitMutation(int WeightingIndex)
{
	//No negative numbers
	int NewWeighting = ~ (Weightings[WeightingIndex] | 0x80000000);
	NewWeighting = NewWeighting % 50; //nothing bigger than 50
	if (NewWeighting < 0)
		NewWeighting = 0;
	SetWeighting(WeightingIndex, NewWeighting);
}

void Chromosome::BoundaryMutation(int WeightingIndex)
{
	//srand( (unsigned)time(0) );	srand should have been called already
	bool bMax = (rand() &1);
	if (bMax)
	{
		//Max is 50
		SetWeighting(WeightingIndex, 50);
	}
	else {
		//No negative numbers
		SetWeighting(WeightingIndex, 0);
	}

}

void Chromosome::OrderMutation(int WeightingIndex)
{
	//randomly swap two chromosomes
	 //srand( (unsigned)time(0) ); srand should have been called already
	int swapWeighting = (rand() % NUM_EVALNODES);
	int tempWeighting = Weightings[WeightingIndex];
	Weightings[WeightingIndex] = Weightings[swapWeighting];
	Weightings[swapWeighting] = tempWeighting;
}




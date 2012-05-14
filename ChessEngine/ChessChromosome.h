/*
 *  ChessChromosome.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-02-27.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef CHESSCHROMOSOME_H
#define CHESSCHROMOSOME_H
#include "Constants.h"
class Chromosome
{
public:
	void Delete() 
	{
		//No pointers or memory allocated here
	}
	Chromosome(int NumChromo) 
	{ 
		ChromosomeNumber = NumChromo; 
		for (int i = 0; i < NUM_EVALNODES; i++)
		{
			Weightings[i] = 0;
		}
		GamesPlayed = 0;
		WonGames = 0;
		FitnessLevel = 1;
		bIsLoaded = false;
	}
	/*Fitness Level*/
	
	Chromosome(Chromosome const* const copyOsome)
	{
		for (int i = 0; i < NUM_EVALNODES; i++)
		{
			Weightings[i] = copyOsome->GetWeighting(i);
		}
		GamesPlayed = 0;
		WonGames = 0;
		FitnessLevel = 1;
		bIsLoaded = false;
	}
	
	int GetFitnessLevel() { return FitnessLevel; }
	
	void UpdateFitnessLevel(bool bWin)
	{
		GamesPlayed++;
		if (bWin)
		{
			WonGames++;
			//FitnessLevel = (int)((float)WonGames / GamesPlayed * 100);
			FitnessLevel++;
		}
	}
	
	void UpdateFitnessLevelAfterTie()
	{
		FitnessLevel += 0.5;
	}
	
	/*Initialization*/
	
	void LoadChromosome();
	
	void SaveChromosome();
	
	void SetChromosomeNumber(int _ChromosomeNumber)
	{
		ChromosomeNumber = _ChromosomeNumber;
	}
	
	int GetChromosomeNumber() { return ChromosomeNumber; }
	
	//void UpdateConstantsPage();
	
	bool IsLoaded() { return bIsLoaded; }
	
	/*Crossover Algorithms*/
public:
	Chromosome* Crossover(ECrossoverType crossover, Chromosome* parent2)
	{
		switch (crossover)
		{
			case SinglePointMutate:
				return SinglePointCrossoverWithChromosome(parent2);
				break;
			case MultiplePointMutate:
				return MultiplePointCrossoverWithChromosome(parent2);
				break;
			case UniformPointMutate:
				return UniformPointCrossoverWithChromosome(parent2);
				break;
			case ArithmeticInversionMutate:
			case ArithmeticMutate:
				return ArithmeticCrossoverWithChromosome(parent2);
				break;
			default:
				//Invalid
				ChessAssert();
				break;
		}
		return NULL;
	}
	
	void Mutate(EMutateType mutator, int mutatorIndex)
	{
		switch (mutator)
		{
			case OrderMutate:
				OrderMutation(mutatorIndex);
				break;
			case BoundaryMutate:
				BoundaryMutation(mutatorIndex);
				break;
			case FlipBitMutate:
				FlipBitMutation(mutatorIndex);
				break;
			case BitStringMutate:
				BitStringMutation(mutatorIndex);
				break;
			default:
				//Invalid
				ChessAssert();
				break;
		}
	}
private:
	Chromosome* SinglePointCrossoverWithChromosome(Chromosome* Mutator);
	
	Chromosome* MultiplePointCrossoverWithChromosome(Chromosome* Mutator);
	
	Chromosome* UniformPointCrossoverWithChromosome(Chromosome* Mutator);
	
	Chromosome* ArithmeticCrossoverWithChromosome(Chromosome* Mutator);
	
	void ArithmeticInversionMutateWithChromosome(Chromosome* Mutator);
	
	void OrderMutation(int WeightingIndex);
	
	void BoundaryMutation(int WeightingIndex);
	
	void FlipBitMutation(int WeightingIndex);
	
	void BitStringMutation(int WeightingIndex);
	
private:
	double FitnessLevel;
	int GamesPlayed;
	int WonGames;
	int ChromosomeNumber;
	bool bIsLoaded;
	
	int Weightings[NUM_EVALNODES];
	
public:
	
	int GetWeighting(int index) const { assert(index < NUM_EVALNODES); return Weightings[index]; }
	void SetWeighting(int index, int newWeighting) { assert (index < NUM_EVALNODES) ; Weightings[index] = newWeighting; }
	
private:

	int PawnMobilityWeight;
	int KnightMobilityWeight;
	int BishopMobilityWeight;
	int RookMobilityWeight;
	int QueenMobilityWeight;
	int KingMobilityWeight;
	
	int PawnDefendingWeight;
	int KnightDefendingWeight;
	int BishopDefendingWeight;
	int RookDefendingWeight;
	int QueenDefendingWeight;
	int KingDefendingWeight;

	int PawnUnderAttackWeight ;
	int KnightUnderAttackWeight;
	int BishopUnderAttackWeight ;
	int RookUnderAttackWeight;
	int QueenUnderAttackWeight;
	int KingUnderAttackWeight ;

	int CenterWeighting ;
	int OpponentSquareWeighting ;
	int CastleScoreWeighting ;
	int DoublePawnWeight ;
	int IsolatedPawnWeight ;
	int OpenFileRookWeighting;
	int CheckMateWeighting;
};
#endif
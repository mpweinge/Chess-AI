/*
 *  Tournament.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-02-27.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tournament.h"
#include <fstream>
#include "Constants.h"
#include <stdio.h>
#include <iostream>

Match* Tournament::CreateMatch(Chromosome* Chromo1, Chromosome* Chromo2, int TourneyPly, int TourneyMaxMoves)
{
	//Ensure they are loaded
	//Chromo1->LoadChromosome();
	//Chromo2->LoadChromosome();
	
	assert(Chromo1->IsLoaded() );
	assert(Chromo2->IsLoaded() );

	//Set the ply of the match
	//Set the move limit of the match
	Match* currMatch = new Match(Chromo1, Chromo2, TourneyPly, TourneyMaxMoves);
	return currMatch;
}

void Tournament::PlayMatch(Match* currMatch)
{
	//Reset the chess board
	//ChessBoard::GetInstance()->ResetBoard();
	
	currMatch->PlayMatch();
	if (currMatch->IsTie())
	{
		currMatch->GetCompetitor1()->UpdateFitnessLevelAfterTie();
		currMatch->GetCompetitor2()->UpdateFitnessLevelAfterTie();
	}
	else {
		currMatch->GetWinner()->UpdateFitnessLevel(true);
		Chromosome* Loser = currMatch->GetLoser();
		Loser->UpdateFitnessLevel(false);	
	}
	
	/*FitnessSum = 0;
	for (int i = 0; i < InactiveChromosomes.size(); i++)
	{
		FitnessSum += InactiveChromosomes[i]->GetFitnessLevel();
	}*/
	/*int removeIndex;
	removeIndex = -1;
	for( int i = 0; i < ActiveChromosomes.size(); i++)
	{
		//FitnessSum += ActiveChromosomes[i]->GetFitnessLevel();
		if (ActiveChromosomes[i] == Loser)
		{
			removeIndex = i;
		}
	}
	assert(removeIndex != -1);
	ActiveChromosomes.erase(ActiveChromosomes.begin() + removeIndex);
	InactiveChromosomes.push_back(Loser);*/
}

void Match::PlayMatch()
{
	MoveTree WhiteTree(PLY, true, Competitor1);
	MoveTree BlackTree(PLY, false, Competitor2);
	int i;
	bool bTie = false;
	
	std::vector<ChessBoard*> PastBoards;
	PastBoards.reserve(100);
	
	int RepeatBoards = 0;
	
	for (i = 0; i <MoveLimit; i++)
	{
		WhiteTree.GenerateTree();
		BlackTree.PruneBranches();
		
		for (int k = PastBoards.size() - 1; k >= 0; k--)
		{
			if (ChessBoard::GetInstance()->IsEqual(PastBoards[k]))
			{
				RepeatBoards++;
				if (RepeatBoards > 1)
				{
					bTie = true;
					i = MoveLimit;
					break;
				}
			}
		}
		RepeatBoards = 0;
		if (bTie)
			break;
		else 
		{
			PastBoards.push_back(ChessBoard::GetInstance()->Copy() );
		}

		
		if (BlackPlayer::GetInstance()->IsInCheck())
		{
			if (BlackPlayer::GetInstance()->CheckForCheckMate() )
			{
				Winner = Competitor1;
				Loser = Competitor2;
				break;
			}
		}
		else if (BlackPlayer::GetInstance()->CheckForStalemate() )
		{
			i = MoveLimit;
			bTie = true;
			break;
		}
		
		ChessLog::GetInstance()->printLog();
		ChessBoard::GetInstance()->PrintBoard();
		
		BlackTree.GenerateTree();
		WhiteTree.PruneBranches();
		
		for (int k = 0; k < PastBoards.size(); k++)
		{
			if (ChessBoard::GetInstance()->IsEqual(PastBoards[k]))
			{
				RepeatBoards++;
				if (RepeatBoards > 1)
				{
					bTie = true;
					i = MoveLimit;
					break;
				}
			}
		}
		RepeatBoards = 0;
		if (bTie)
			break;
		else 
		{
			PastBoards.push_back(ChessBoard::GetInstance()->Copy() );
		}
		
		if (WhitePlayer::GetInstance()->IsInCheck() )
		{
			if (WhitePlayer::GetInstance()->CheckForCheckMate() )
			{
				Winner = Competitor2;
				Loser = Competitor1;
				break;
			}
		}
		else if (WhitePlayer::GetInstance()->CheckForStalemate() )
		{
			i = MoveLimit;
			bTie = true;
			break;
		}
		
		ChessLog::GetInstance()->printLog();
		ChessBoard::GetInstance()->PrintBoard();
	}
	printf("Game over, final position below: Tie? %c\n", bTie? 'y' : 'n');
	fflush(stdout);
	ChessLog::GetInstance()->printLog();
	ChessBoard::GetInstance()->PrintBoard();
	
	for (int i = 0; i < PastBoards.size(); i++)
	{
		PastBoards[i]->Erase();
		delete(PastBoards[i]);
	}
	PastBoards.clear();
	
	if (!bTie)
	{
		int k;
		k = 0;
		k++;
	}
	
	if (i == MoveLimit)
	{
		int WhitePieceScore = ChessBoard::GetInstance()->CalculatePieceScore(true);
		int BlackPieceScore = ChessBoard::GetInstance()->CalculatePieceScore(false);
		int ChromoOneScore = ChessBoard::GetInstance()->CalculateScore(Competitor1);
		int ChromoTwoScore = ChessBoard::GetInstance()->CalculateScore(Competitor2);
		
		bool bWhiteWin = false;
		
		
		if ( WhitePieceScore > BlackPieceScore && ChromoTwoScore > 0)
		{
			//White wins
			bWhiteWin = true;
		}
		else if (BlackPieceScore > WhitePieceScore && ChromoOneScore < 0 )
		{
			//Black wins
		}
		else if (BlackPieceScore > WhitePieceScore)
		{
			if (ChromoTwoScore < 5) //losing by a bit
			{
				//Black Wins
			}
			else {
				//Black has a piece advantage, but thinks it is losing
				bTie = true;
			}

		}
		else if (WhitePieceScore > BlackPieceScore)
		{
			if (ChromoOneScore > -5 )
			{
				//White wins
				bWhiteWin = true;
			}
			else {
				//White has a piece advantage, but thinks it is losing
				bTie = true;
			}

		}
		else 
		{
			//(WhitePieceScore == BlackPieceScore);
			//Nothing else to do but go off piece score
			/*if (ChromoOneScore > 0 && ChromoTwoScore > 0)
			{
				//White wins
				bWhiteWin = true;
			}
			else if (ChromoOneScore < 0 && ChromoTwoScore < 0)
			{
				//Black wins
			}
			else if ( abs(ChromoOneScore) > abs(ChromoTwoScore))
			{
				//White wins
				bWhiteWin = true;
			}
			else if ( abs(ChromoTwoScore) > abs(ChromoOneScore) )
			{
				//Black wins
			}
			else {
				//Tie
				//Advance white
				bWhiteWin = true;
			}*/
			bTie = true;
		}
		if (bTie)
		{
			m_bTie = true;
		}
		else if (bWhiteWin)
		{
			Winner = Competitor1;
			Loser = Competitor2;
		}
		else 
		{
			Loser = Competitor1;
			Winner = Competitor2;
		}

	}
}

Chromosome* Tournament::RouletteWheelSelection(int random_number)
{
	//if (FitnessSum == -1)
	{
		FitnessSum = 0;
		for( int i = 0; i < ActiveChromosomes.size(); i++)
		{
			FitnessSum += ActiveChromosomes[i]->GetFitnessLevel();
		}
		/*for (int i = 0; i < InactiveChromosomes.size(); i++)
		{
			FitnessSum += InactiveChromosomes[i]->GetFitnessLevel();
		}*/
	}
	
	int random_integer = random_number % FitnessSum;
	
	int currSum = 0; 
	int i;
	for (i = 0; i < ActiveChromosomes.size(); i++)
	{
		if (currSum > random_integer)
			break;
		currSum += ActiveChromosomes[i]->GetFitnessLevel();
	}
	
	return (i == ActiveChromosomes.size() ) ? ActiveChromosomes[ActiveChromosomes.size() - 1] : ActiveChromosomes[i];
}

void Tournament::PlayTournament()
{
	char filename[50];
	
	std::ofstream myfile;
	
	ActiveChromosomes.clear();
	InactiveChromosomes.clear();
	
	double WeightingAverages[NUM_WEIGHTINGS];
	
	//Load in dem chromo's, set up matches
	
	for (int i = 0; i < NumTourneyGames; i++)
	{
		ActiveChromosomes.push_back(new Chromosome(i + RoundNumber*100));
		ActiveChromosomes[i]->LoadChromosome(true);
	}
	
	int LocalSum = 0;
	int InnerLoopTally = ActiveChromosomes.size();
	for (int i = 0; i < NUM_WEIGHTINGS; i++)
	{
		sprintf(&filename[0], "Factor%i.num", i);
		myfile.open(filename, std::ios::app);
		for (int j = 0; j < InnerLoopTally; j++)
		{
			LocalSum+= ActiveChromosomes[j]->GetWeighting(i);
		}
		WeightingAverages[i] = LocalSum / ActiveChromosomes.size();
		myfile<<WeightingAverages[i]<<std::endl;
		myfile.close();
		LocalSum = 0;
	}
	
	if (RoundNumber == TournamentLevels)
	{
		for (int i = ActiveChromosomes.size() - 1; i >= 0; i--)
		{
			delete(ActiveChromosomes[i]);
		}
		ActiveChromosomes.clear();
		return;
	}
	
	for (int i = 0; i < NumTourneyGames; i++)
	{
		for (int j = 0; j < NumTourneyGames; j++)
		{
			if (i == j)
				continue;
			//Populate number of chromosomes (equal to num of tourney games for now)
			//if ( i&1 == 1)
			{
				BaseMatches.push_back(new Match(ActiveChromosomes[i], ActiveChromosomes[j], TourneyPly, TourneyMaxMoves));
			}
		}
	}
	
	int subRound = 0;
	sprintf(&filename[0], "Round%i_Games.chr", RoundNumber);
	myfile.open(filename, std::ofstream::out);
	//Loop until last match ... is this gonna work lol?
	//while ( BaseMatches.size() != 1 )
	{
	//	subRound++;
		//Play all the matches in the round
		for (int i = (BaseMatches.size()-1); i >= 0; i--)
		{
			
			//Play the matches in reverse order 
			PlayMatch(BaseMatches[i]);
			if (BaseMatches[i]->IsTie() )
			{
				myfile << "Round: " << subRound << "Game: " << i << "Tie between: " << BaseMatches[i]->GetCompetitor1()->GetChromosomeNumber() 
				<< "and " << BaseMatches[i]->GetCompetitor2()->GetChromosomeNumber()  << std::endl;
			}
			else {
				int winner = BaseMatches[i]->GetWinner()->GetChromosomeNumber();
				int loser = BaseMatches[i]->GetLoser()->GetChromosomeNumber();
				
				myfile << "Round: " << subRound << "Game: " << i << "Winner: " << winner 
				<< "Loser: " << loser << std::endl;
			}
			delete(BaseMatches[i]);
			BaseMatches.erase(BaseMatches.begin() + i);
			ChessBoard::GetInstance()->ResetBoard();
			//ChessLog::GetInstance()->ResetLog();
		}
		assert(BaseMatches.size() == 0);
		/*for (int i = 0; i < ActiveChromosomes.size(); i++)
		{
			//Populate number of chromosomes (equal to num of tourney games for now)
			if ( i&1 == 1)
			{
				BaseMatches.push_back(new Match(ActiveChromosomes[i], ActiveChromosomes[i-1], TourneyPly, TourneyMaxMoves));
			}
		}*/
	}
	
	//Last match
	/*PlayMatch(BaseMatches[BaseMatches.size()-1]);
	int winner = BaseMatches[BaseMatches.size()-1]->GetWinner()->GetChromosomeNumber();
	int loser = BaseMatches[BaseMatches.size()-1]->GetLoser()->GetChromosomeNumber();
	if (BaseMatches[i]->IsTie() )
	{
		myfile << "Round: " << subRound << "Game: " << i << "Tie between: " << BaseMatches[i]->GetCompetitor1()->GetChromosomeNumber() 
		<< "and " << BaseMatches[i]->GetCompetitor2()->GetChromosomeNumber()  << std::endl;
	}
	else {
		int winner = BaseMatches[i]->GetWinner()->GetChromosomeNumber();
		int loser = BaseMatches[i]->GetLoser()->GetChromosomeNumber();
		
		myfile << "Round: " << subRound << "Game: " << i << "Winner: " << winner 
		<< "Loser: " << loser << std::endl;
	}
	delete (BaseMatches[BaseMatches.size() - 1]);*/
	
	myfile.close();
	
	sprintf(&filename[0], "Round%i_Mutations.chr", RoundNumber);
	
	myfile.open(filename, std::ofstream::out);
	
	ChessBoard::GetInstance()->ResetBoard();
	//ChessLog::GetInstance()->ResetLog();
	ChessBoard::GetInstance()->PrintBoard();
	//Special update for winner of tournament? Maybe Later
	//The one and only srand
	const float CrossoverProbability = .5f;
	const float MutationProbability = .1f;
	srand( (unsigned)time(0) );	
	//For the next level of the tournament, cross the chromosomes
	for (int i = 0; i < NumTourneyGames; i++)
	{
		Chromosome* parent1 = RouletteWheelSelection(rand());
		
		float CrossoverProb = (float)rand()/(float)RAND_MAX;
		
		Chromosome* newChromo = NULL; 
		
		int parent1number = parent1->GetChromosomeNumber();
	
		if (CrossoverProb > CrossoverProbability )
		{
		
			Chromosome* parent2 = RouletteWheelSelection(rand());
			int parent2number = parent2->GetChromosomeNumber();
			//Mate the two parents
			int random_number = rand();
			int crossover = (random_number % NUM_CROSSOVER_METHODS);
			
			float MutatorProb = (float)rand()/(float)RAND_MAX;
			
			newChromo = parent1->Crossover(ECrossoverType(crossover), parent2);
			
			if (MutatorProb > MutationProbability)
			{
				int random_number2 = rand();
				int mutator = (random_number2 % NUM_MUTATION_METHODS); //mutate one weighting
				int random_number3 = rand();
				int mutatorIndex = ( random_number3  % NUM_WEIGHTINGS);
			
				
				
			
				newChromo->Mutate(EMutateType(mutator), mutatorIndex);
			
				myfile << "New Chromosome: " << 100*(RoundNumber+1)+i <<"Parent 1: " << parent1number
				<< "Parent 2: " << parent2number <<" ParentCrossover: " << Crossover_Type[crossover] 
				<< "Mutation " << Mutate_Types[mutator] << "Index " << mutatorIndex << std::endl;
			}
			else 
			{
				myfile << "New Chromosome: " << 100*(RoundNumber+1)+i <<"Parent 1: " << parent1number
				<< "Parent 2: " << parent2number <<" ParentCrossover: " << Crossover_Type[crossover] 
				<< "No Mutation"<<std::endl;
			}

			
		
		}
		else {
			newChromo = new Chromosome(parent1);
			
			myfile << "New Chromosome: " << 100*(RoundNumber+1)+i <<"One Parent: " << parent1number
			<<"No Crossover "
			<<"No Mutation"<<std::endl;
		}

		newChromo->SetChromosomeNumber((RoundNumber+1)*100+i);
		newChromo->SaveChromosome();
		delete(newChromo);
		newChromo = NULL;
	}
	
	for (int i = (ActiveChromosomes.size() - 1); i >= 0; i--)
	{
		delete(ActiveChromosomes[i]);
	}
	ActiveChromosomes.clear();
	myfile.close();
	RoundNumber++;
	BaseMatches.clear();
	PlayTournament();
}

Chromosome* Match::GetWinner()
{
	assert(Winner);
	assert(Winner != Loser);
	return Winner;
}

Chromosome* Match::GetLoser()
{
	assert(Loser);
	assert(Winner != Loser);
	return Loser;
}
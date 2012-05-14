/*
 *  Tournament.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-02-27.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ChessChromosome.h"
#include "ChessBoard.h"
#include "MoveTree.h"
#include "Constants.h"

class Match
{
public:
	Match(Chromosome *_Competitor1, Chromosome* _Competitor2, int _FixedPly, int _MoveLimit) :
	Competitor1(_Competitor1), Competitor2(_Competitor2), FixedPly(_FixedPly), MoveLimit(_MoveLimit) { m_bTie = false;} 
	
	Chromosome* GetCompetitor1() { return Competitor1; }
	Chromosome* GetCompetitor2() { return Competitor2; }
	
	int GetFixedPly() { return FixedPly; }
	int GetMoveLimit() { return MoveLimit; }
	Chromosome* GetWinner();
	Chromosome* GetLoser();
	
	bool IsTie() { return m_bTie; }
	
	void PlayMatch();
	
private:
	Chromosome* Competitor1;
	Chromosome* Competitor2;
	
	Chromosome* Winner;
	Chromosome* Loser;
	
	int FixedPly;
	
	bool m_bTie;
	
	int MoveLimit; 
};

class Tournament
{
public:
	Tournament(int _TourneyPly, int _TourneyMaxMoves, int _NumTourneyGames, int _TournamentLevels) :
	TourneyPly(_TourneyPly), TourneyMaxMoves(_TourneyMaxMoves), NumTourneyGames(_NumTourneyGames), TournamentLevels(_TournamentLevels)
	{ FitnessSum = -1;RoundNumber = 1;}
	
	void AddBaseMatch(Match* baseMatch);
	
	static Match* CreateMatch(Chromosome* Chromo1, Chromosome* Chromo2, int TourneyPly, int TourneyMaxMoves);
	void PlayMatch(Match* nextMatch);
	
	void PlayTournament();
	
private:
	
	void UpdateFitnesses();
	
	std::vector<Match*> BaseMatches;
	
	std::vector<Chromosome*> ActiveChromosomes;
	std::vector<Chromosome*> InactiveChromosomes;
	
	int FitnessSum;
	
	void ChromosomeSex();
	
	Chromosome* RouletteWheelSelection(int random_number);
	
	int TourneyPly;
	
	int TournamentLevels;
	
	int NumTourneyGames;
	int TourneyMaxMoves;	
	int RoundNumber;
};
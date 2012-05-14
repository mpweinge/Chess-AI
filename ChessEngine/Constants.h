/*
 *  Constants.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAXPAWNMOVES 4
#define MAXKNIGHTMOVES 8
#define MAXBISHOPMOVES 13
#define MAXROOKMOVES 14
#define MAXQUEENMOVES 27
#define MAXKINGMOVES 8

#define SUPPRESS_OUTPUT

#include <assert.h>
//#include "ChessLog.h"
//#include "ChessBoard.h"
#include <stdlib.h>

const char * const Crossover_Type[] = {
	"Single Point Mutate",
	"Multiple Point Mutate", 
	"Uniform Point Mutate",
	"Arithmetic Mutate",
	"Arithmetic Inversion Mutate"
};

const char * const Mutate_Types[] = {"Order Mutate", 
	"Boundary Mutate", 
	"Flip Bit Mutate", 
	"Bit String Mutate" 
};

const int PawnScore = 100;
const int KnightScore = 300;
const int BishopScore = 300;
const int RookScore = 500;
const int QueenScore = 900;
const int KingScore = 1000000;

enum ENeuralType
{
	KnightAdvantageType = 0,
	KnightMobilityType,
	KnightUnderAttackType,
	BishopAdvantageType,
	BishopUnderAttackType,
	BishopMobilityType,
	MinorPieceAdvantageType,
	MinorPieceMobilityAdvantageType,
	MinorPieceUnderAttackAdvantageType,
	RookAdvantageType,
	RookUnderAttackType,
	RookMobilityType,
	QueenAdvantageType,
	QueenUnderAttackType,
	QueenMobilityType,
	RookOnSeventhRowType,
	BatteryAttackType,
	KingInCheckType,
	KingMobilityType,
	MajorPieceAdvantageType,
	MajorPieceMobilityAdvantageType,
	MajorPieceUnderAttackAdvantageType,
	CastleType,
	CenterAttackType,
	OpponentSquareAttackType,
	PawnAdvantageType,
	CenterPawnType,
	DoublePawnType,
	IsolatedPawnType,
};

#define NUM_NEURAL_NODES (30)

/*const int PawnMobilityWeight = 1;
const int KnightMobilityWeight = 1;
const int BishopMobilityWeight = 1;
const int RookMobilityWeight = 1;
const int QueenMobilityWeight = 1;
const int KingMobilityWeight = 1;

const int PawnDefendingWeight = 1;
const int KnightDefendingWeight = 1;
const int BishopDefendingWeight = 1;
const int RookDefendingWeight = 1;
const int QueenDefendingWeight = 1;
const int KingDefendingWeight = 1;

const int PawnUnderAttackWeight = 1;
const int KnightUnderAttackWeight = 1;
const int BishopUnderAttackWeight = 1;
const int RookUnderAttackWeight = 1;
const int QueenUnderAttackWeight = 1;
const int KingUnderAttackWeight = 1;

const int CenterWeighting = 1;
const int OpponentSquareWeighting = 1;
const int CastleScoreWeighting = 1;
const int DoublePawnWeight = 1;
const int IsolatedPawnWeight = 1;
const int OpenFileRookWeighting = 1;
const int CheckMateWeighting = 1;*/
#define NUM_EVALNODES 20

enum EPromotionPiece
{
	queen = 'Q',
	bishop = 'B',
	knight = 'N',
	rook = 'R',
};
const int NUM_CROSSOVER_METHODS = 5;

enum ECrossoverType
{
	SinglePointMutate = 0,
	MultiplePointMutate,
	UniformPointMutate,
	ArithmeticMutate,
	ArithmeticInversionMutate,
};

const int NUM_MUTATION_METHODS = 4;

enum EMutateType
{
	OrderMutate = 0,
	BoundaryMutate,
	FlipBitMutate,
	BitStringMutate,
};

const int NUM_WEIGHTINGS = 20;

enum EWeightingType
{
	CenterAttackWeighting = 0,
	OpponentSquareWeighting,
	CastleScoreWeighting,
	DoublePawnWeight,
	IsolatedPawnWeight,
	OpenFileRookWeighting,
	CheckMateWeighting,
	CenterPawnWeight,
	BishopPair,
	RookOnSeventhRow,
	RookBatteryType,
	CastleMissed,
	UnMovedKnightBishop,
	PassedPawns,
	RookSupportedPassPawn,
	CheckWeight,
	KnightMobilityWeight,
	QueenMobilityWeight,
	BishopMobilityWeight,
	RookMobilityWeight,
	/* For old stuff */
	PawnMobilityWeight = -100,
	KingMobilityWeight,
	PawnDefendingWeight,
	KnightDefendingWeight,
	BishopDefendingWeight,
	RookDefendingWeight,
	QueenDefendingWeight,
	KingDefendingWeight,
	PawnUnderAttackWeight,
	KnightUnderAttackWeight,
	BishopUnderAttackWeight,
	RookUnderAttackWeight,
	QueenUnderAttackWeight,
	KingUnderAttackWeight,
};

class Move
{
public:
	Move() {row = 0; col = 0;}
	Move(int _row, int _col) { row = _row; col = _col; }
	~Move() { }
	int row;
	int col;
	virtual char getPromotionPiece() const { 
		assert(0); return ' ';
	}
	virtual bool IsPromotionMove() const { return false; }
	virtual bool IsCastleMove() const { return false; }
	virtual bool IsKingSide() const { assert(0); return false; }
	virtual bool IsEnPassantMove() const { return false; }
};

const int NUM_PIECES = 16;

const int MAX_LOG_SIZE = 250;

const int PLY = 4;

const int NUM_THREADS = 1;

//#define USE_THREADS


//#define PRINT_MEMORY_PAWN

//#define PRINT_MEMORY_MOVES

void ChessAssert();

#endif
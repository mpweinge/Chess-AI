/*
 *  NeuralInput.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-03-29.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INPUTARGS_H
#define INPUTARGS_H
#include "Constants.h"
#include <vector>
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
#include "Rook.h"

class Input_Args
{
public:
	virtual void Delete() = 0;
};

//class Bishop_

class Bishop_Args : public Input_Args
{
public:
	Bishop_Args() { }
	std::vector<Bishop*> WhiteBishops;
	std::vector<Bishop*> BlackBishops;
	void Delete() { }
};

class Rook_Args : public Input_Args
{
public:
	Rook_Args() { }
	std::vector<Rook*> WhiteRooks;
	std::vector<Rook*> BlackRooks;
	void Delete() { }
};

class Queen_Args : public Input_Args
{
public:
	Queen_Args() { }
	std::vector<Queen*> WhiteQueens;
	std::vector<Queen*> BlackQueens;
	void Delete() { }
};

class Knight_Args : public Input_Args
{
public:
	Knight_Args() { }
	std::vector<Knight*> WhiteKnights;
	std::vector<Knight*> BlackKnights;
	void Delete() { }
};

class King_Args : public Input_Args
{
public:
	King_Args() { }
	King* WhiteKings;
	King* BlackKings;
	void Delete() { }
};

class Pawn_Args : public Input_Args
{
public:
	Pawn_Args() { }
	std::vector<Pawn*> WhitePawns;
	std::vector<Pawn*> BlackPawns;
	void Delete() { }
};

class MinorPieces_Args : public Input_Args
{
public:
	MinorPieces_Args(Knight_Args* _KnightArgs, Bishop_Args* _BishopArgs)
	: KnightArgs(_KnightArgs), BishopArgs(_BishopArgs) { }
	Bishop_Args* BishopArgs;
	Knight_Args* KnightArgs;
	void Delete() { }
};

class MajorPieces_Args : public Input_Args
{
public:
	MajorPieces_Args(Rook_Args* _RookArgs, Queen_Args* _QueenArgs, King_Args* _KingArgs)
	: RookArgs(_RookArgs), QueenArgs(_QueenArgs), KingArgs(_KingArgs) { }
	Rook_Args* RookArgs;
	Queen_Args* QueenArgs;
	King_Args* KingArgs;
	void Delete() { }
};
#endif
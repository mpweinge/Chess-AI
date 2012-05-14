/*
 *  Queen.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-30.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef QUEEN_H
#define QUEEN_H

#include "ChessPiece.h"
#include "ChessBoard.h"

class ChessBoard;

class Queen : public ChessPiece
{
public:
	~Queen() 
	{ 
		EvalMoves.clear();
	}
	explicit Queen(const int& startingX, const int & startingY, const bool& _isWhite, ChessBoard* _Board);
private:
	Queen(const Queen& original);
	Queen& operator=( const Queen& right_hand_side );
public:
	
	void GetLocation(ChessSquare& Move2) const {Move2 = currLoc; }
	ChessSquare GetLocation() const { return currLoc; }
	void SetCol(const int& col) { currLoc.col = col; }
	void SetRow(const int& row) { currLoc.row = row; }
	
	int GetRow() const { return currLoc.row; }
	int GetCol() const { return currLoc.col; }
	
	int GetScore() const { return QueenScore; }
	double GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax);
	double GetDefendingScore(const Chromosome* calcChromo);
	double GetPieceUnderAttackScore(const Chromosome* calcChromo);
	
	bool HasLineToKing(ChessSquare& KingLoc) 
	{
		if( (currLoc.row - KingLoc.row) == 0)
			return true;
		if( (currLoc.col - KingLoc.col) == 0)
			return true;
		return (abs(currLoc.row - KingLoc.row) == abs(currLoc.col - KingLoc.col));
	}
	
	bool LookForCheck(bool test) const;
	bool IsAttackingSquare(const ChessSquare& Square) const;
	
	void GetLegalMoves(std::vector<ChessMove>& PotMoves);
	std::vector<ChessMove>& GetEvalLegalMoves();
	bool HasLegalMove();
	bool MakeMove(const ChessSquare& Move3);
	bool isWhite() const { return bIsWhite; }
	bool isBlockingCheck() const { return bIsBlockingCheck; }
	void setBlockingCheck(bool newState) { bIsBlockingCheck = newState; }
	bool IsInKingArray()  { return bIsInKingArray; }
	void SetIsInKingArray(const bool InArray) { bIsInKingArray = InArray; }
	char Name() const { return 'Q'; }
	ChessBoard* GetBoardPointer() const { return ChessBoardInstance; }
	void SetBoardPointer(ChessBoard* _Ptr) { ChessBoardInstance = _Ptr; }
	
	Queen* Copy() 
	{ 
		Queen* ret = new Queen(currLoc.row, currLoc.col, bIsWhite, ChessBoardInstance); 
#ifdef PRINT_MEMORY
		printf("Created queen at:%i\n", ret);
		fflush(stdout);
#endif
		return ret;
	}

private:
	std::vector<ChessMove> EvalMoves;
	bool CheckForCapture(const ChessSquare& MoveTo);
	ChessBoard* ChessBoardInstance;
	ChessSquare currLoc;
	bool bIsWhite;
	bool bIsBlockingCheck;
	bool bIsInKingArray;
};

#endif
/*
 *  Knight.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-26.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Constants.h"
#include "assert.h"

class ChessBoard;

class Knight : public ChessPiece
{
public:
	~Knight() { EvalMoves.clear();}
	explicit Knight(const int& startingX, const int & startingY, const bool _isWhite, ChessBoard* _Instance);
private:
	Knight(const Knight& original);
	Knight& operator=( const Knight& right_hand_side );
public:
	void GetLocation(ChessSquare& Move2) const;
	ChessSquare GetLocation() const { return currLoc; }
	void SetCol(const int& col) { currLoc.col = col; }
	void SetRow(const int& row) { currLoc.row = row; }
	bool isLegalMove(ChessSquare& MoveTo) const;
	void GetLegalMoves(std::vector<ChessMove>& PotMoves);
	std::vector<ChessMove>& GetEvalLegalMoves();
	bool MakeMove(const ChessSquare& Move3);
	bool isWhite() const { return bIsWhite; }
	bool HasLegalMove();
	bool HasLineToKing(ChessSquare& KingLoc) 
	{ 
		if( abs(currLoc.row - KingLoc.row) == 2)
		{
			if (abs(currLoc.col - KingLoc.col) == 1)
				return true;
			return false;
		}
		if( abs(currLoc.row - KingLoc.row) == 1)
		{
			if (abs(currLoc.col - KingLoc.col) == 2)
				return true;
			return false;
		}
		return false;
	}
	
	int GetRow() const { return currLoc.row; }
	int GetCol() const { return currLoc.col; }
	
	int GetScore() const { return KnightScore; }
	double GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax);
	double GetDefendingScore(const Chromosome* calcChromo);
	double GetPieceUnderAttackScore(const Chromosome* calcChromo);
	
	bool IsInKingArray()  { return bIsInKingArray; }
	void SetIsInKingArray(const bool InArray) { bIsInKingArray = InArray; }
		
	bool isBlockingCheck() const { return bIsBlockingCheck; }
	void setBlockingCheck(bool newState) { bIsBlockingCheck = newState; }
	bool LookForCheck(bool test) const;
	bool IsAttackingSquare(const ChessSquare& Square) const;
	char Name() const { return 'N'; }
	
	ChessBoard* GetBoardPointer() const { return ChessBoardInstance; }
	void SetBoardPointer(ChessBoard* _Ptr) { ChessBoardInstance = _Ptr; }
	
	ChessPiece* Copy() 
	{ 
		ChessPiece* ret = new Knight(currLoc.row, currLoc.col, bIsWhite, ChessBoardInstance); 
		
#ifdef PRINT_MEMORY
		printf("Created knight at:%i\n", ret);
		fflush(stdout);
#endif
		
		return ret;
	}
private:
	std::vector<ChessMove> EvalMoves;
	bool CheckForCapture(const ChessSquare& MoveTo);
	ChessSquare currLoc;
	ChessBoard* ChessBoardInstance;
	bool bIsWhite;
	bool bIsBlockingCheck;
	bool bIsInKingArray;
};

#endif
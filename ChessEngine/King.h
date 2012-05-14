/*
 *  King.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-30.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef KING_H
#define KING_H

#include "ChessPiece.h"
#include "ChessBoard.h"

class ChessBoard;

class King : public ChessPiece
{
public:
	~King() { EvalMoves.clear();}
	explicit King(const int& startingX, const int & startingY, const bool& _isWhite, ChessBoard* _Instance);
private:
	King(const King& original);
	King& operator=( const King& right_hand_side );
public:
	
	void GetLocation(Move& Move2) const { Move2 = currLoc; }
	Move GetLocation() const { return currLoc; }
	void SetCol(const int& col) { currLoc.col = col; }
	void SetRow(const int& row) { currLoc.row = row; }
	
	int GetRow() const { return currLoc.row; }
	int GetCol() const { return currLoc.col; }
	
	int GetScore() const { return KingScore; }
	double GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax);
	double GetDefendingScore(const Chromosome* calcChromo);
	double GetPieceUnderAttackScore(const Chromosome* calcChromo);
	
	bool LookForCheck(bool test) const { return false; }
	
	void SetHasCastled(bool _bHasCastled) { bHasCastled = _bHasCastled; }
	bool HasCastled() { return bHasCastled; }
	
	bool bHasMoved() { return (bMoved != 0); }
	void SetHasMoved(const bool bHasMoved) { bMoved += bHasMoved? 1 : -1; }
	bool IsAttackingSquare(const Move& Square) const;
	
	bool HasLineToKing(Move& KingLoc) 
	{
		return false;
	}
	
	bool HasLegalMove();
	
	bool isBlockingCheck() const { return false; }
	void setBlockingCheck(bool newState) {; }
	void GetLegalMoves(std::vector<ChessMove>& PotMoves);
	std::vector<ChessMove>& GetEvalLegalMoves();
	bool MakeMove(const Move& Move3);
	bool isWhite() const { return bIsWhite; }
	bool IsInKingArray()  { return bIsInKingArray; }
	void SetIsInKingArray(const bool InArray) { bIsInKingArray = InArray; }
	char Name() const { return 'K'; }
	
	bool CastleMissed() { 
		if (bHasCastled)
			return false;
		else 
		{
			if (bMoved > 0)
				return true;
		}
		return false;
	}
	ChessBoard* GetBoardPointer() const { return ChessBoardInstance; }
	void SetBoardPointer(ChessBoard* _Ptr) { ChessBoardInstance = _Ptr; }
	
	King* Copy() 
	{ 
		King* ret = new King(currLoc.row, currLoc.col, bIsWhite, ChessBoardInstance); 
#ifdef PRINT_MEMORY
		printf("Created king at:%i\n", ret);
		fflush(stdout);
#endif
		return ret;
	}

private:
	std::vector<ChessMove> EvalMoves;
	bool CheckForCapture(const Move& MoveTo);
	ChessBoard* ChessBoardInstance;
	Move currLoc;
	bool bIsWhite;
	bool bHasCastled;
	int bMoved;
	bool bIsInKingArray;
};

#endif
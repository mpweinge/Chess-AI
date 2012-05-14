/*
 *  Rook.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-29.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ROOK_H
#define ROOK_H

#include "ChessPiece.h"
#include "ChessBoard.h"

class ChessBoard;

class Rook : public ChessPiece
{
public:
	~Rook() { EvalMoves.clear();}
	explicit Rook(const int& startingX, const int & startingY, const bool _isWhite, ChessBoard* _Instance);
private:
	Rook(const Rook& original);
	Rook& operator=( const Rook& right_hand_side );
public:
	void GetLocation(Move& Move2) const { Move2 = currLoc; }
	Move GetLocation() const { return currLoc; }
	void SetCol(const int& col) { currLoc.col = col; }
	void SetRow(const int& row) { currLoc.row = row; }
	
	int GetRow() const { return currLoc.row; }
	int GetCol() const { return currLoc.col; }
	
	int GetScore() const { return RookScore; }
	double GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax);
	double GetDefendingScore(const Chromosome* calcChromo);
	double GetPieceUnderAttackScore(const Chromosome* calcChromo);
	
	bool HasLineToKing(Move& KingLoc) 
	{
		if( (currLoc.row - KingLoc.row) == 0)
			return true;
		if( (currLoc.col - KingLoc.col) == 0)
			return true;
		return false;
	}
	
	bool bHasMoved() { return (bMoved != 0); }
	void SetHasMoved(const bool bHasMoved) { bMoved += bHasMoved? 1 : -1; }
	
	void GetLegalMoves(std::vector<ChessMove>& PotMoves);
	std::vector<ChessMove>& GetEvalLegalMoves();
	bool HasLegalMove();
	bool MakeMove(const Move& Move3);
	bool isWhite() const { return bIsWhite; }
	bool isBlockingCheck() const { return bIsBlockingCheck; }
	void setBlockingCheck(bool newState) { bIsBlockingCheck = newState; }
	bool LookForCheck(bool test) const;
	bool IsAttackingSquare(const Move& Square) const;
	char Name() const { return 'R'; }
	
	bool IsInKingArray()  { return bIsInKingArray; }
	void SetIsInKingArray(const bool InArray) { bIsInKingArray = InArray; }
	ChessBoard* GetBoardPointer() const { return ChessBoardInstance; }
	void SetBoardPointer(ChessBoard* _Ptr) { ChessBoardInstance = _Ptr; }
	
	ChessPiece* Copy() 
	{ 
		ChessPiece* ret = new Rook(currLoc.row, currLoc.col, bIsWhite, ChessBoardInstance); 
#ifdef PRINT_MEMORY
		printf("Created rook at:%i\n", ret);
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
	int bMoved;
	bool bIsBlockingCheck;
	bool bIsInKingArray;
};

#endif
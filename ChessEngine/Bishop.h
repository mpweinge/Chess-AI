/*
 *  Bishop.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-26.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BISHOP_H
#define BISHOP_H

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "assert.h"

class ChessBoard;
class ChessSquare;

class Bishop : public ChessPiece
{
public:
	~Bishop() { EvalMoves.clear();}
	explicit Bishop(const int& startingX, const int & startingY, const bool _isWhite, ChessBoard* _Instance);
private:
	Bishop(const Bishop& original);
	
	Bishop& operator=( const Bishop& right_hand_side );
public:
	
	void GetLocation(ChessSquare& Move2) const;
	ChessSquare GetLocation() const { return currLoc; }
	void SetCol(const int& col) { currLoc.col = col; }
	void SetRow(const int& row) { currLoc.row = row; }
	
	bool isLegalMove(ChessSquare& Move2) const;
	bool isBlockingCheck() const { return bIsBlockingCheck; }
	void setBlockingCheck(bool newState) { bIsBlockingCheck = newState; }
	void GetLegalMoves(std::vector<ChessMove>& PotMoves);
	std::vector<ChessMove>& GetEvalLegalMoves();
	bool HasLegalMove();
	bool MakeMove(const ChessSquare& Move3);
	bool isWhite() const { return bIsWhite; }
	bool HasLineToKing(ChessSquare& KingLoc) 
	{
		return (abs(currLoc.row - KingLoc.row) == abs(currLoc.col - KingLoc.col));
	}
	
	int GetRow() const { return currLoc.row; }
	int GetCol() const { return currLoc.col; }
	
	int GetScore() const { return BishopScore; }
	double GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax);
	double GetDefendingScore(const Chromosome* calcChromo);
	double GetPieceUnderAttackScore(const Chromosome* calcChromo);
	
	bool IsInKingArray()  { return bIsInKingArray; }
	void SetIsInKingArray(const bool InArray) { bIsInKingArray = InArray; }
	
	bool LookForCheck(bool test) const;
	bool IsAttackingSquare(const ChessSquare& Square) const;
	
	char Name() const { return 'B'; }
	ChessBoard* GetBoardPointer() const { return ChessBoardInstance; }
	void SetBoardPointer(ChessBoard* _Ptr) { ChessBoardInstance = _Ptr; }
	
	ChessPiece* Copy() { return new Bishop(currLoc.row, currLoc.col, bIsWhite, ChessBoardInstance); }

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
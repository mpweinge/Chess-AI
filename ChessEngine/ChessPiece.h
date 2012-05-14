/*
 *  ChessPiece.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-26.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "Constants.h"
#include "ChessMove.h"
#include <vector>
#include <string>
#include "ChessChromosome.h"

#include <typeinfo>

class ChessBoard;

char pieceToText(const ChessPiece* piece);
void pieceToText(const ChessPiece* piece, char& outText);
char rowToLetter(int row);
bool checkPuttingInCheck(const Move& Move3, const Move& startingLoc, bool bWhiteMove, ChessBoard* _Instance);

class ChessPiece
{
public:
	explicit ChessPiece(){}
	virtual ~ChessPiece()
	{
		//printf("Deleting Piece");
		//fflush(stdout);
	}
private:
	ChessPiece(const ChessPiece& original) { assert(0); }
public:
	virtual void GetLocation(Move& MoveTo) const = 0;
	virtual int GetRow() const = 0;
	virtual int GetCol() const = 0;
	virtual Move GetLocation() const = 0;
	virtual void SetRow(const int& row) = 0;
	virtual void SetCol(const int& col) = 0;
	virtual void GetLegalMoves(std::vector<ChessMove>& PotMoves) = 0;
	virtual std::vector<ChessMove>& GetEvalLegalMoves() = 0;
	virtual bool MakeMove(const Move& MoveTo) = 0;
	virtual bool isWhite() const = 0;
	virtual char Name() const = 0;
	virtual bool isBlockingCheck() const = 0;
	virtual void setBlockingCheck(bool newState) = 0;
	virtual bool LookForCheck(bool test) const = 0;
	virtual bool HasLegalMove() = 0;
	virtual bool IsAttackingSquare(const Move& Square) const= 0;
	virtual int GetScore() const = 0;
	virtual double GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax) = 0;
	virtual double GetDefendingScore(const Chromosome* calcChromo) = 0;
	virtual double GetPieceUnderAttackScore(const Chromosome* calcChromo) = 0;
	virtual bool HasLineToKing(Move& KingLoc) = 0;
	virtual bool IsInKingArray() = 0;
	virtual void SetIsInKingArray(const bool InArray) = 0;
	virtual ChessPiece* Copy() = 0;
	
	virtual ChessBoard* GetBoardPointer() const = 0;
	
	void SetIndex(int _index) { index = _index; }
	int GetIndex() { return index; }
	
	bool isEqual(const ChessPiece* comparisonPiece) const
	{
		if (GetRow() != comparisonPiece->GetRow() )
			return false;
		if (GetCol() != comparisonPiece->GetCol() )
			return false;
		if (pieceToText(this) != pieceToText(comparisonPiece) )
			return false;
		if (isWhite() != comparisonPiece->isWhite() )
			return false;
		return true;
	}
	
	bool IsAttackingSquare(const int row, const int col) { const Move temp(row,col); return IsAttackingSquare(temp); }
	static bool ValidCheckStateMove(const Move& Move3, const ChessPiece* instance);
	static bool ValidCheckStateMove(const int row, const int col, const ChessPiece* instance)
	{
		const Move temp(row, col); return ValidCheckStateMove(temp, instance);
	}
	
	virtual void SetBoardPointer(ChessBoard* _Ptr) = 0;
	
private:
	virtual bool CheckForCapture(const Move& MoveTo) = 0;	
	int index;
};




#endif


/*
 *  Pawn.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-30.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"
#include "ChessBoard.h"

class ChessBoard;

class Pawn : public ChessPiece
{
public:
	~Pawn() 
	{ 
#ifdef PRINT_MEMORY_PAWN
		printf("Deleting pawn at:%i\n", this);
		fflush(stdout);
#endif
		EvalMoves.clear();
	}
	explicit Pawn(const int& startingX, const int & startingY, const bool& _isWhite, ChessBoard* _Instance);
private:
	Pawn(const Pawn& original);
	Pawn& operator=( const Pawn& right_hand_side );
public:
	
	void GetLocation(ChessSquare& Move2) const {Move2 = currLoc; }
	ChessSquare GetLocation() const { return currLoc; }
	
	void SetCol(const int& col) { currLoc.col = col; }
	void SetRow(const int& row) { currLoc.row = row; }
	
	int GetRow() const { return currLoc.row; }
	int GetCol() const { return currLoc.col; }
	
	int GetScore() const { return PawnScore; }
	double GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax);
	double GetDefendingScore(const Chromosome* calcChromo);
	double GetPieceUnderAttackScore(const Chromosome* calcChromo);
	
	void GetLegalMoves(std::vector<ChessMove>& PotMoves);
	std::vector<ChessMove>& GetEvalLegalMoves();
	bool HasLegalMove();
	
	bool MakeMove(const ChessSquare& Move3);
	bool isWhite() const { return bIsWhite; }
	bool isBlockingCheck() const { return bIsBlockingCheck; }
	void setBlockingCheck(bool newState) { bIsBlockingCheck = newState; }
	bool LookForCheck(bool test) const;
	bool IsAttackingSquare(const ChessSquare& Square) const;
	char Name() const { return 'P'; }
	
	bool IsInKingArray()  { return bIsInKingArray; }
	void SetIsInKingArray(const bool InArray) { bIsInKingArray = InArray; }
	bool HasLineToKing(ChessSquare& KingLoc) 
	{
		return IsAttackingSquare(KingLoc);
	}
	ChessBoard* GetBoardPointer() const { return ChessBoardInstance; }
	void SetBoardPointer(ChessBoard* _Ptr) { ChessBoardInstance = _Ptr; }
	
	ChessPiece* Copy() 
	{ 
		ChessPiece* ret = new Pawn(currLoc.row, currLoc.col, bIsWhite, ChessBoardInstance);
		
#ifdef PRINT_MEMORY_PAWN
		printf("Created pawn at:%i\n", ret);
		fflush(stdout);
#endif
		
		return ret; 
	}

private:
	std::vector<ChessMove> EvalMoves;
	bool CheckForCapture(const ChessSquare& MoveTo);
	bool CheckForEnPassant(const ChessSquare& MoveTo, ChessPiece* &_captPiece);
	ChessSquare currLoc;
	ChessBoard* ChessBoardInstance;
	bool bIsWhite;
	bool bIsBlockingCheck;
	bool bIsInKingArray;
};

#endif
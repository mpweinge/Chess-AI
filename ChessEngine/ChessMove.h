/*
 *  ChessMove.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHESSMOVE_H
#define CHESSMOVE_H

#include "assert.h"
#include "Constants.h"
#include <stdio.h>

class ChessPiece;
class ChessBoard;

class ChessMove
{	
public:
	explicit ChessMove(ChessPiece* piece, const int startingRow, const int startingCol, const int row, const int col, const bool isCapture, const bool OpponentInCheck, ChessBoard* _ChessBoardInstance);
	explicit ChessMove(ChessPiece* piece, const int startingRow, const int startingCol, const int row, const int col, const bool isCapture, ChessBoard* _ChessBoardInstance);
	explicit ChessMove(bool bKingSide, ChessBoard* _ChessBoardInstance) 
	{ 
		bIsCastle = true; 
		bIsKingSideCastle = bKingSide; 
		bNullMove = false; 
		bCheckSet = false; 
		bIsPromotion = false; 
		ChessBoardInstance = _ChessBoardInstance;
	    capturedPiece = NULL;
	}
	explicit ChessMove(ChessBoard* _ChessBoardInstance) 
	{ 
		bNullMove = true; 
		bCheckSet = false; 
		bIsPromotion = false; 
		ChessBoardInstance = _ChessBoardInstance;
		capturedPiece = NULL;
	}
	explicit ChessMove() 
	{ 
		bNullMove = true; 
		bCheckSet = false; 
		bIsPromotion = false; 
		ChessBoardInstance = NULL; 
		capturedPiece = NULL;
	}
	
	~ChessMove() { }
	
	//ChessMove(const ChessMove& original) {  }
	//ChessMove& operator=( const ChessMove& right_hand_side );
private:
	
public:
	bool isCastle() const { return bIsCastle; }
	bool isKingSideCastle() const { return bIsKingSideCastle; }
	bool isLegalMove();
	int makeMove();
	void SetPromotion(const bool _isPromotion); 
	bool isPromotion() const { return bIsPromotion; }
	bool isCapture() const { return bIsCapture; }
	int getRow() const { return row; }
	int getCol() const { return col; }
	int getStartingRow() const { return startingRow; }
	int getStartingCol() const { return startingCol; }
	bool IsEnPassant() const { return bIsEnPassant; }
	ChessPiece* getCapturedPiece() const { return capturedPiece; }
	ChessPiece* getPiece() const { return piece; }
	
	void setPiece(ChessPiece* _piece) { piece = _piece; }
	
	void setCapturedPiece(ChessPiece* _captpiece) { capturedPiece = _captpiece; }
	
	void SetPromotionChar(const char _promChar) { cPromotionChar = _promChar; }
	bool IsNullMove() const { return bNullMove; }
	bool IsCheckMove() const { 
		if(!bCheckSet) 
		{
			printf("FIND ME AND FIX ME LATER!!!!");
 			return false;
		}
		
		return OpponentInCheck; }
	
	void SetCheckMove(bool bCheck) { bCheckSet = true; OpponentInCheck = bCheck; }
	
	char GetPromotionChar() const { return cPromotionChar; }
	bool isEqual(const ChessMove& comparisonMove) const;
	
	void Delete()
	{
		//Can't delete piece because they exist. Just want to delete the pointers
	}
	
	void SetChessBoard(ChessBoard* _Instance) 
	{ 
		ChessBoardInstance = _Instance; 
	}
	
	ChessMove* Copy()
	{
		ChessMove* ret = new ChessMove();
		ret->startingRow = startingRow;
		ret->startingCol = startingCol;
		ret->row = row;
		ret->col = col;
		ret->bIsCapture = bIsCapture;
		ret->bIsCastle = bIsCastle;
		ret->bIsKingSideCastle = bIsKingSideCastle;
		ret->bIsEnPassant = bIsEnPassant;
		ret->cPromotionChar = cPromotionChar;
		ret->bCheckSet = bCheckSet;
		ret->bNullMove = bNullMove;
		ret->bIsPromotion = bIsPromotion;
		ret->OpponentInCheck = OpponentInCheck;
		ret->bLegalMove = bLegalMove;
		
		ret->piece = piece;
		ret->capturedPiece = capturedPiece;
		
#ifdef PRINT_MEMORY_MOVES
		printf("Created move at: %i\n", ret);
		fflush(stdout);
#endif
		
		return ret;
	}
	
private:
	int addMoveToLog(ChessPiece* piece, int x, int y);
	bool bLegalMove;
	bool bIsPromotion;
	bool OpponentInCheck;
	
	bool bCheckSet;
		
	bool bNullMove;
	
	ChessPiece* piece;
	ChessPiece* capturedPiece;
	
	ChessBoard* ChessBoardInstance;
	
	int startingRow;
	int startingCol;
	int row; 
	int col;
	bool bIsCapture;
	bool bIsCastle;
	bool bIsKingSideCastle;
	bool bIsEnPassant;
	char cPromotionChar;
};
#endif
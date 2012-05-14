/*
 *  ChessMove.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ChessMove.h"
#include "ChessLog.h"

ChessMove::ChessMove(ChessPiece* _piece, const int _startingRow, const int _startingCol, const int _row, const int _col, const bool _isCapture, const bool _OpponentInCheck, ChessBoard* _ChessBoardInstance)
{
	piece = _piece;
	startingRow = _startingRow;
	startingCol = _startingCol;
	row = _row;
	col = _col;
	bIsCapture = _isCapture;
	
	bIsEnPassant = false;
	
	ChessBoardInstance = _ChessBoardInstance;
	
	if (bIsCapture)
	{
		capturedPiece = ChessBoardInstance->GetPiece(row, col);
		if (capturedPiece == NULL)
		{
			bIsEnPassant = true;
			capturedPiece = ChessBoardInstance->GetPiece(row, startingCol);
		}

	}
	else {
		capturedPiece = NULL;
	}
	
	bIsCastle = false;
	bNullMove = false;
	bIsPromotion = false;
	
	OpponentInCheck = _OpponentInCheck;
	bCheckSet = true;

}

ChessMove::ChessMove(ChessPiece* _piece, const int _startingRow, const int _startingCol, const int _row, const int _col, const bool _isCapture, ChessBoard* _ChessBoardInstance)
{
	piece = _piece;
	startingRow = _startingRow;
	startingCol = _startingCol;
	row = _row;
	col = _col;
	bIsCapture = _isCapture;
	
	bIsEnPassant = false;
	bIsPromotion = false;
	
	ChessBoardInstance = _ChessBoardInstance;
	
	if (bIsCapture)
	{
		capturedPiece = ChessBoardInstance->GetPiece(ChessSquare(row, col));
		if (capturedPiece == NULL)
		{
			bIsEnPassant = true;
			capturedPiece = ChessBoardInstance->GetPiece(ChessSquare(row, startingCol));
		}
		
	}
	else {
		capturedPiece = NULL;
	}
	
	bIsCastle = false;
	bNullMove = false;
	bIsPromotion = false;

	bCheckSet = false;
	
}

int ChessMove::makeMove()
{
	//Add to log
	//Add to history
	ChessBoardInstance->GetLog()->addMoveToLog(this);
	//this->piece->SetRow(row); Circular Dependency
	//this->piece->SetCol(col);
	
	;
	return 0;
}

bool ChessMove::isLegalMove()
{
	//Allow this to be done by individual piece
	ChessAssert();
	return true;
}
void ChessMove::SetPromotion(const bool _isPromotion)
{ 
	bIsPromotion = _isPromotion; 
	//bIsPromotion = false;
	//int k;
}

int ChessMove::addMoveToLog(ChessPiece* piece, int x, int y)
{
	;
	ChessAssert();
	return -1;
}

bool ChessMove::isEqual(const ChessMove& comparisonMove) const
{
	if (IsNullMove() )
		ChessAssert();
		
		if (bIsCastle)
		{
			if( !(comparisonMove.isCastle() ) )
				return false;
			if ( bIsKingSideCastle == comparisonMove.isKingSideCastle() )
				return true;
			return false;
		}
		else if (bIsCapture)
		{
			if( !(comparisonMove.isCapture() ) )
				return false;
			if( !(capturedPiece->isEqual(comparisonMove.getCapturedPiece() ) ) )
				return false;
			
			if( !(piece->isEqual(comparisonMove.getPiece() ) ) )
				return false;
			
			return true;
		}
		else if (bIsEnPassant)
		{
			if( !(comparisonMove.IsEnPassant() ) )
				return false;
			
			if( !(capturedPiece->isEqual(comparisonMove.getCapturedPiece() ) ) )
				return false;
			
			if( !(piece->isEqual(comparisonMove.getPiece() ) ) )
				return false;
			
			return true;
		}
		else if (bIsPromotion)
		{
			if( !(comparisonMove.isPromotion() ) )
				return false;
			
			if( !(piece->isEqual(comparisonMove.getPiece() ) ) )
				return false;
			
			if (cPromotionChar != comparisonMove.GetPromotionChar() )
				return false;
			
			if (row != comparisonMove.getRow() )
				return false;
			
			if (col != comparisonMove.getCol() )
				return false;
			
			return true;
		}
		else 
		{
			if (comparisonMove.isPromotion()
				|| comparisonMove.isCastle()
				|| comparisonMove.IsNullMove()
				|| comparisonMove.IsEnPassant()
				|| comparisonMove.isCapture()
				)
				return false;
			
			if( !(piece->isEqual(comparisonMove.getPiece() ) ) )
				return false;
			
			if (row != comparisonMove.getRow() )
				return false;
			
			if (col != comparisonMove.getCol() )
				return false;
			
			return true;
		}
	
	
	
}

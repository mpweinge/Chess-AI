/*
 *  King.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-30.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "King.h"

std::vector<ChessMove>& King::GetEvalLegalMoves()
{
	return EvalMoves;
}

bool King::MakeMove(const Move& Move2)
{	
	if ( !(abs(Move2.row-currLoc.row) <= 1) || !(abs(Move2.col-currLoc.col) <= 1) )
		return false;
	
	if (ChessBoardInstance->bHasPiece(Move2.row, Move2.col))
	{
		bool isWhite = ChessBoardInstance->GetPiece(Move2.row, Move2.col)->isWhite();
		if ((isWhite && bIsWhite) || (!isWhite && !bIsWhite))
			return false;
		else {
			return CheckForCapture(Move2);
		}
	}
	
	ChessBoardInstance->MovePiece(this, Move2.row, Move2.col);
	bMoved = true;
	return true;
	
}

bool King::CheckForCapture(const Move& MoveTo)
{
	//ChessAssert();
	/*if (bIsWhite)
	{
		if ( WhitePlayer::GetInstance()->IsInCheck() )
		{
			Move checkLoc = WhitePlayer::GetInstance()->GetCheckingPieceLocation();
			if( (checkLoc.row == MoveTo.row) && (checkLoc.col == MoveTo.col) )
			{
				//assert(0);
				ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
				LookForCheck(false);
				bMoved = true;
				return true;
			}
			return false;
		}
	}
	else
	{
		if ( BlackPlayer::GetInstance()->IsInCheck() )
		{
			Move checkLoc = BlackPlayer::GetInstance()->GetCheckingPieceLocation();
			if( (checkLoc.row == MoveTo.row) && (checkLoc.col == MoveTo.col) )
			{
				//assert(0);
				ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
				LookForCheck(false);
				bMoved = true;
				return true;
			}
			return false;
		}
	}
	 */
	if (ChessPiece::ValidCheckStateMove(MoveTo.row, MoveTo.col, this) )
	{
		//We already checked for capturing
		ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
		//LookForCheck(false);
		return true;
	}
	return false;
	//We already checked for capturing
	//ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
	//bMoved = true;
	//return true;
}

King::King(const int& startingX, const int& startingY, const bool& _isWhite, ChessBoard* _Instance)
{
	currLoc = Move(startingX, startingY);
	bIsWhite = _isWhite;
	bMoved = 0;
	bIsInKingArray = false;
	bHasCastled = false;
	ChessBoardInstance = _Instance;
}

bool King::IsAttackingSquare(const Move& Square) const
{ 
	if( (abs(Square.col - currLoc.col) <= 1) && ( abs(Square.row - currLoc.row) <= 1) )
		return true;
	return false;
}

double King::GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax)
{
	EvalMoves.clear();
	
	if (bReturnMax)
	{
		return calcChromo->GetWeighting(KingMobilityWeight);
	}
	
	GetLegalMoves(EvalMoves);
	int mobilityScore = (EvalMoves.size() / MAXKINGMOVES) * calcChromo->GetWeighting(KingMobilityWeight);

	return mobilityScore;
}

double King::GetDefendingScore(const Chromosome* calcChromo)
{
	Player* currPlayer = NULL;
	if (bIsWhite)
		currPlayer = ChessBoardInstance->GetWhitePlayer();
	else {
		currPlayer = ChessBoardInstance->GetBlackPlayer();
	}
	int numDefendingPieces = 0;
	for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
	{
		if (currPlayer->GetPiece(i)->IsAttackingSquare(currLoc.row, currLoc.col) )
			numDefendingPieces++;
	}
	return numDefendingPieces * calcChromo->GetWeighting(KingDefendingWeight);
}

double King::GetPieceUnderAttackScore(const Chromosome* calcChromo)
{
	Player* opponentPlayer = NULL;
	if (bIsWhite)
		opponentPlayer = ChessBoardInstance->GetBlackPlayer();
	else {
		opponentPlayer = ChessBoardInstance->GetWhitePlayer();
	}
	int numAttackingPieces = 0;
	for (int i = 0; i < opponentPlayer->GetNumPiecesLeft(); i++)
	{
		if (opponentPlayer->GetPiece(i)->IsAttackingSquare(currLoc.row, currLoc.col) )
			numAttackingPieces++;
	}
	return numAttackingPieces * calcChromo->GetWeighting(KingUnderAttackWeight);
}

bool King::HasLegalMove()
{
	std::vector<ChessMove> potMoves;
	GetLegalMoves(potMoves);
	if (potMoves.size() > 0 )
	{
		potMoves.clear();
		return true;
	}
	potMoves.clear();
	return false;
}

void King::GetLegalMoves(std::vector<ChessMove>& PotMoves)
{
	Player* player;
	if (bIsWhite)
	{
		if (currLoc.col == 0 && currLoc.row == 4)
		{
			player = ChessBoardInstance->GetWhitePlayer();
			if (player->CanCastle(true, ChessBoardInstance) )
				PotMoves.push_back( ChessMove(true, ChessBoardInstance) );
			if (player->CanCastle(false, ChessBoardInstance) )
				PotMoves.push_back( ChessMove(false, ChessBoardInstance) );
		}
	}
	else 
	{
		if (currLoc.col == 7 && currLoc.row == 4)
		{
			player = ChessBoardInstance->GetBlackPlayer();
			if (player->CanCastle(true, ChessBoardInstance) )
				PotMoves.push_back( ChessMove(true, ChessBoardInstance) );
			if (player->CanCastle(false, ChessBoardInstance) )
				PotMoves.push_back( ChessMove(false, ChessBoardInstance) );
		}
	}
	
	//ChessPiece* checkingPiece = NULL;
	
	/*if ( player->IsInCheck() )
	{
		Move checkLoc = player->GetCheckingPieceLocation();
		if( (abs(checkLoc.row - currLoc.row) < 2) && (abs(checkLoc.col - currLoc.col)<2) )
		{
			if (ChessPiece::ValidCheckStateMove(checkLoc.row, checkLoc.col, this))
				PotMoves.push_back( Move(checkLoc.row, checkLoc.col) );
		}
		
		checkingPiece = player->GetCheckingPiece();
	}*/
	//int col = currLoc.col;
	Move testMove = Move(currLoc.row, currLoc.col+1);
	ChessPiece* testPiece = ChessBoardInstance->GetPiece(testMove.row, testMove.col);
	if (testPiece)
	{
		if (testPiece->isWhite() != bIsWhite)
		{
			if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, true, ChessBoardInstance) );
		}
	}
	else 
	{
		if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
			PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, false, ChessBoardInstance) );
	}
	
	testMove = Move(currLoc.row+1, currLoc.col);
	testPiece = ChessBoardInstance->GetPiece(testMove.row, testMove.col);
	if (testPiece)
	{
		if (testPiece->isWhite() != bIsWhite)
		{
			if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, true, ChessBoardInstance) );
		}
	}
	else 
	{
		if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
			PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, false, ChessBoardInstance) );
	}
	
	testMove = Move(currLoc.row-1, currLoc.col);
	testPiece = ChessBoardInstance->GetPiece(testMove.row, testMove.col);
	if (testPiece)
	{
		if (testPiece->isWhite() != bIsWhite)
		{
			if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, true, ChessBoardInstance) );
		}
	}
	else 
	{
		if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
			PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, false, ChessBoardInstance) );
	}
	
	testMove = Move(currLoc.row-1, currLoc.col-1);
	testPiece = ChessBoardInstance->GetPiece(testMove.row, testMove.col);
	if (testPiece)
	{
		if (testPiece->isWhite() != bIsWhite)
		{
			if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, true, ChessBoardInstance) );
		}
	}
	else 
	{
		if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
			PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, false, ChessBoardInstance) );
	}
	
	testMove = Move(currLoc.row, currLoc.col-1);
	testPiece = ChessBoardInstance->GetPiece(testMove.row, testMove.col);
	if (testPiece)
	{
		if (testPiece->isWhite() != bIsWhite)
		{
			if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, true, ChessBoardInstance) );
		}
	}
	else 
	{
		if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
			PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, false, ChessBoardInstance) );
	}
	
	testMove = Move(currLoc.row+1, currLoc.col-1);
	testPiece = ChessBoardInstance->GetPiece(testMove.row, testMove.col);
	if (testPiece)
	{
		if (testPiece->isWhite() != bIsWhite)
		{
			if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, true, ChessBoardInstance) );
		}
	}
	else 
	{
		if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
			PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, false, ChessBoardInstance) );
	}
	
	testMove = Move(currLoc.row-1, currLoc.col+1);
	testPiece = ChessBoardInstance->GetPiece(testMove.row, testMove.col);
	if (testPiece)
	{
		if (testPiece->isWhite() != bIsWhite)
		{
			if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, true, ChessBoardInstance) );
		}
	}
	else 
	{
		if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
			PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, false, ChessBoardInstance) );
	}
	
	testMove = Move(currLoc.row+1, currLoc.col+1);
	testPiece = ChessBoardInstance->GetPiece(testMove.row, testMove.col);
	if (testPiece)
	{
		if (testPiece->isWhite() != bIsWhite)
		{
			if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, true, ChessBoardInstance) );
		}
	}
	else 
	{
		if (ChessPiece::ValidCheckStateMove(testMove.row, testMove.col, this) )
			PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, testMove.row, testMove.col, false, ChessBoardInstance) );
	}
}

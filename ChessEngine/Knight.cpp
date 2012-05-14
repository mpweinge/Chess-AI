/*
 *  Knight.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-26.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Knight.h"

std::vector<ChessMove>& Knight::GetEvalLegalMoves()
{
	return EvalMoves;
}

bool Knight::MakeMove(const Move& Move3)
{
	if (Move3.row < 0 || Move3.col < 0 || Move3.row > 7 || Move3.col > 7)
		return false;
	
	if (ChessBoardInstance->bHasPiece(Move3.row, Move3.col))
	{
		bool isWhite = ChessBoardInstance->GetPiece(Move3.row, Move3.col)->isWhite();
		if ((isWhite && bIsWhite) || (!isWhite && !bIsWhite))
			return false;
		else {
			if (ChessPiece::ValidCheckStateMove(Move3, this))
				return CheckForCapture(Move3);
			else {
				return false;
			}
		}
	}
			
	if ( (abs(Move3.row - currLoc.row) == 2 && abs(Move3.col-currLoc.col) == 1) ||
		(abs(Move3.row - currLoc.row) == 1 && abs(Move3.col-currLoc.col) == 2) )
	{
		assert(Move3.row >= 0 && Move3.col >= 0 && Move3.row < 8 && Move3.col < 8 );
		
		if (ChessPiece::ValidCheckStateMove(Move3, this))
		{
			ChessBoardInstance->MovePiece(this, Move3.row, Move3.col);
			LookForCheck(false);
			return true;
		}
		else {
			return false;
		}
	}
	
	return false;
}

bool Knight::LookForCheck(bool test) const
{
	//Get King Location
	Move KingLoc = ChessBoardInstance->GetKingLocation(bIsWhite);
	if( IsAttackingSquare(KingLoc) )
	{
		if (!test)
		{
			printf("Check!");
			ChessBoardInstance->PutPlayerInCheck(bIsWhite, this);
		}
		return true;
	}
	return false;
}

bool Knight::IsAttackingSquare(const Move& Square) const
{
	if( ((abs(Square.row - currLoc.row) == 2) && (abs(Square.col - currLoc.col) == 1))
	   || ((abs(Square.col - currLoc.col) == 2) && (abs(Square.row - currLoc.row) == 1)) )
	{
		return true;
	}
	return false;
}

bool Knight::CheckForCapture(const Move& MoveTo)
{
	//ChessAssert();
	/*if (bIsWhite)
	{
		if ( WhitePlayer::GetInstance()->IsInCheck() )
		{
			Move checkLoc = WhitePlayer::GetInstance()->GetCheckingPieceLocation();
			if( (checkLoc.row == MoveTo.row) && (checkLoc.col == MoveTo.col) )
			{
				ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
				LookForCheck(false);
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
				ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
				LookForCheck(false);
				return true;
			}
			return false;
		}
	}*/
	if (ChessPiece::ValidCheckStateMove(MoveTo.row, MoveTo.col, this) )
	{
		//We already checked for capturing
		ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
		//LookForCheck(false);
		return true;
	}
	return false;
}

Knight::Knight(const int& startingX, const int & startingY, const bool _isWhite, ChessBoard* _Instance)
{
	currLoc = Move(startingX, startingY);
	bIsWhite = _isWhite;
	bIsBlockingCheck = false;
	bIsInKingArray = false;
	ChessBoardInstance = _Instance;
}

void Knight::GetLocation(Move& Move2) const
{
	Move2 = currLoc;
}

double Knight::GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax)
{
	EvalMoves.clear();
	
	if (bReturnMax)
	{
		return calcChromo->GetWeighting(KnightMobilityWeight);
	}
	
	GetLegalMoves(EvalMoves);
	
	int MobilityScore = (EvalMoves.size() / MAXKNIGHTMOVES) * calcChromo->GetWeighting(KnightMobilityWeight);

	return MobilityScore;
}

double Knight::GetDefendingScore(const Chromosome* calcChromo)
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
	return numDefendingPieces * calcChromo->GetWeighting(KnightDefendingWeight);
}

double Knight::GetPieceUnderAttackScore(const Chromosome* calcChromo)
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
	return numAttackingPieces * calcChromo->GetWeighting(KnightUnderAttackWeight);
}

bool Knight::HasLegalMove()
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

void Knight::GetLegalMoves(std::vector<ChessMove>& PotMoves)
{
	//Knights move two up and one across
	//8 possible locations from any given position
	
	Player* player;
	if (bIsWhite)
	{
		player = ChessBoardInstance->GetWhitePlayer();
	}
	else {
		player = ChessBoardInstance->GetBlackPlayer();
	}


	if ( isBlockingCheck() )
		return;
	
	/*if ( player->IsInCheck() )
	{
		Move checkLoc = player->GetCheckingPieceLocation();
		if(
			( abs( checkLoc.row - currLoc.row ) == 1 )&&( abs(checkLoc.col - currLoc.col) == 2 ) ||
			( abs( checkLoc.row - currLoc.row ) == 2 )&&( abs(checkLoc.col - currLoc.col) == 1 )
		   )
		{
			PotMoves.push_back( Move(checkLoc.row, checkLoc.col) );
			return;
		}
		return;
	}*/
	
	ChessPiece * currPiece = ChessBoardInstance->GetPiece(currLoc.row+2, currLoc.col+1);
	Move currMove = Move(currLoc.row + 2, currLoc.col + 1);
	if( (currMove.row <= 7) && (currMove.col <= 7) && (currMove.row >= 0) && (currMove.col >= 0))
	{
		if (currPiece)
		{
			if (currPiece->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, true, ChessBoardInstance));
			}
		}
		else
		{
			if (ChessPiece::ValidCheckStateMove(currLoc.row+2, currLoc.col+1, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, false, ChessBoardInstance));
		}
	}
	
	currPiece = ChessBoardInstance->GetPiece(currLoc.row+1, currLoc.col+2);
	currMove = Move(currLoc.row + 1, currLoc.col + 2);
	if( (currMove.row <= 7) && (currMove.col <= 7) && (currMove.row >= 0) && (currMove.col >= 0))
	{
		if (currPiece)
		{
			if (currPiece->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, true, ChessBoardInstance));
			}
		}
		else
		{
			if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, false, ChessBoardInstance));
		}
	}
	
	currPiece = ChessBoardInstance->GetPiece(currLoc.row+1, currLoc.col-2);
	currMove = Move(currLoc.row + 1, currLoc.col - 2);
	if( (currMove.row <= 7) && (currMove.col <= 7) && (currMove.row >= 0) && (currMove.col >= 0))
	{
		if (currPiece)
		{
			if (currPiece->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, true, ChessBoardInstance));
			}
		}
		else
		{
			if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, false, ChessBoardInstance));
		}
	}
	
	currPiece = ChessBoardInstance->GetPiece(currLoc.row+2, currLoc.col-1);
	currMove = Move(currLoc.row + 2, currLoc.col -1);
	if( (currMove.row <= 7) && (currMove.col <= 7) && (currMove.row >= 0) && (currMove.col >= 0))
	{
		if (currPiece)
		{
			if (currPiece->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, true, ChessBoardInstance));
			}
		}
		else
		{
			if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, false, ChessBoardInstance));
		}
	}
	
	currPiece = ChessBoardInstance->GetPiece(currLoc.row-1, currLoc.col+2);
	currMove = Move(currLoc.row - 1, currLoc.col + 2);
	if( (currMove.row <= 7) && (currMove.col <= 7) && (currMove.row >= 0) && (currMove.col >= 0))
	{
		if (currPiece)
		{
			if (currPiece->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, true, ChessBoardInstance));
			}
		}
		else
		{
			if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, false, ChessBoardInstance));
		}
	}
	
	currPiece = ChessBoardInstance->GetPiece(currLoc.row-2, currLoc.col+1);
	currMove = Move(currLoc.row - 2, currLoc.col + 1);
	if( (currMove.row <= 7) && (currMove.col <= 7) && (currMove.row >= 0) && (currMove.col >= 0))
	{
		if (currPiece)
		{
			if (currPiece->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, true, ChessBoardInstance));
			}
		}
		else
		{
			if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, false, ChessBoardInstance));
		}
	}
		
	currPiece = ChessBoardInstance->GetPiece(currLoc.row-1, currLoc.col-2);
	currMove = Move(currLoc.row - 1, currLoc.col - 2);
	if( (currMove.row <= 7) && (currMove.col <= 7) && (currMove.row >= 0) && (currMove.col >= 0))
	{
		if (currPiece)
		{
			if (currPiece->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, true, ChessBoardInstance));
			}
		}
		else
		{
			if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, false, ChessBoardInstance));
		}
	}
		
	currPiece = ChessBoardInstance->GetPiece(currLoc.row-2, currLoc.col-1);
	currMove = Move(currLoc.row - 2, currLoc.col - 1);
	if( (currMove.row <= 7) && (currMove.col <= 7) && (currMove.row >= 0) && (currMove.col >= 0))
	{
		if (currPiece)
		{
			if (currPiece->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, true, ChessBoardInstance));
			}
		}
		else
		{
			if (ChessPiece::ValidCheckStateMove(currMove.row, currMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currMove.row, currMove.col, false, ChessBoardInstance));
		}
	}
													  													  
}

						   
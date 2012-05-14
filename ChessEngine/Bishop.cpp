/*
 *  Bishop.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-26.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Bishop.h"

std::vector<ChessMove>& Bishop::GetEvalLegalMoves()
{
	return EvalMoves;
}

bool Bishop::MakeMove(const ChessSquare& Move2)
{
	
	//Check if there is a piece along the diagonal
	
	if ( !( abs(Move2.row - currLoc.row) == abs(Move2.col - currLoc.col) ) )
	{
		return false;
	}
	
	int diffx = Move2.row - currLoc.row;
	int diffy = Move2.col - currLoc.col;
	
	for (int i = 1; i < abs(Move2.row - currLoc.row); i++)
	{
		if (ChessBoardInstance->bHasPiece( (diffx > 0) ? currLoc.row+i: currLoc.row - i , 
											 (diffy > 0) ? currLoc.col+i : currLoc.col - i) )
			return false;
	}
	
	if (ChessBoardInstance->bHasPiece(Move2.row, Move2.col))
	{
		bool isWhite = ChessBoardInstance->GetPiece(Move2.row, Move2.col)->isWhite();
		if ((isWhite && bIsWhite) || (!isWhite && !bIsWhite))
			return false;
		else {
			if (ChessPiece::ValidCheckStateMove(Move2, this))
				return CheckForCapture(Move2);
			else {
				return false;
			}
		}
	}
	
	if (ChessPiece::ValidCheckStateMove(Move2, this))
	{
		ChessBoardInstance->MovePiece(this, Move2.row, Move2.col);
		LookForCheck(false);
		return true;
	}
	else {
		return false;
	}
}

bool Bishop::CheckForCapture(const ChessSquare& MoveTo)
{
	//ChessAssert();
	/*if (bIsWhite)
	{
		if ( WhitePlayer::GetInstance()->IsInCheck() )
		{
			ChessSquare checkLoc = WhitePlayer::GetInstance()->GetCheckingPieceLocation();
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
			ChessSquare checkLoc = BlackPlayer::GetInstance()->GetCheckingPieceLocation();
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
	//We already checked for capturing
	//ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
	//LookForCheck(false);
	//return true;
}

Bishop::Bishop(const int& startingX, const int& startingY, const bool _isWhite, ChessBoard* _Instance)
{
	currLoc = ChessSquare(startingX, startingY);
	bIsWhite = _isWhite;
	bIsBlockingCheck = false;
	bIsInKingArray = false;
	ChessBoardInstance = _Instance;
}

bool Bishop::LookForCheck(bool test) const
{
	ChessSquare KingLoc = ChessBoardInstance->GetKingLocation(bIsWhite);

	if ( IsAttackingSquare(KingLoc)	)
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

bool Bishop::IsAttackingSquare(const ChessSquare& Square) const
{
	int diffx = Square.row - currLoc.row;
	int diffy = Square.col - currLoc.col;
	if ( abs(diffx) != abs(diffy) )
		return false;
	for (int i = 1; i < abs(diffx); i++)
	{
		if (ChessBoardInstance->bHasPiece(diffx > 0 ? currLoc.row + i:currLoc.row-i, 
												 diffy > 0 ? currLoc.col + i: currLoc.col - i))
			return false;
	}
	return true;
}

void Bishop::GetLocation(ChessSquare& Move2) const
{
	Move2 = currLoc;
}

double Bishop::GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax)
{
	EvalMoves.clear();
	
	if (bReturnMax)
	{
		return calcChromo->GetWeighting(BishopMobilityWeight);
	}
	
	GetLegalMoves(EvalMoves);
	int mobilityscore = (EvalMoves.size() / MAXBISHOPMOVES) * calcChromo->GetWeighting(BishopMobilityWeight);

	return mobilityscore;
}

double Bishop::GetDefendingScore(const Chromosome* calcChromo)
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
	return numDefendingPieces * calcChromo->GetWeighting(BishopDefendingWeight);
}

double Bishop::GetPieceUnderAttackScore(const Chromosome* calcChromo)
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
	return numAttackingPieces * calcChromo->GetWeighting(BishopUnderAttackWeight);
}

bool Bishop::HasLegalMove()
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

void Bishop::GetLegalMoves(std::vector<ChessMove>& PotMoves)
{
	//Bishops move in diagonals until the first piece
	
	Player* player;
	if (bIsWhite)
	{
		player = ChessBoardInstance->GetWhitePlayer();
	}
	else {
		player = ChessBoardInstance->GetBlackPlayer();
	}
	
	/*if ( isBlockingCheck() )
	{
		ChessSquare checkLoc = player->GetCheckingPieceLocation();
		int diffx = currLoc.row - checkLoc.row;
		int diffy = currLoc.col - checkLoc.col;
		
		if (abs(diffx) == abs(diffy) )
		{
			for (int i = 1; i < abs(diffx); i++)
			{
				if( ChessBoard::GetInstance()->bHasPiece( diffx > 0 ? currLoc.row + i : currLoc.row - i, 
														 diffy > 0 ? currLoc.col + i : currLoc.col - i ) )
					return;
				else {
					PotMoves.push_back(ChessSquare( diffx > 0 ? currLoc.row + i : currLoc.row - i, 
												diffy > 0 ? currLoc.col + i : currLoc.col - i ) );
				}

			}
			PotMoves.push_back(ChessSquare(checkLoc.row, checkLoc.col)) ;
		}
		return;
	}*/
	
	/*if ( player->IsInCheck() )
	{		
		ChessSquare checkLoc = player->GetCheckingPieceLocation();
		ChessSquare kingLoc = player->GetKingLocation();
		char p = pieceToText(player->GetCheckingPiece());
		
		if( ( p != 'N') && (p != 'P') )
		{
			int xdiff = kingLoc.row - checkLoc.row;
			int ydiff = kingLoc.col - checkLoc.col;
			
			if (abs(xdiff) == abs(ydiff))
			{
				for (int i = 1; i < abs(xdiff); i++)
				{
					const ChessSquare temp(xdiff < 0 ? kingLoc.row + i : kingLoc.row - i, 
									ydiff < 0 ? kingLoc.col + i : kingLoc.col - i);
					if (IsAttackingSquare(temp) )
						PotMoves.push_back(ChessSquare(temp.row, temp.col) );
				}
			}
			else if (xdiff == 0)
			{
				for (int i = 1; i < abs(ydiff); i++)
				{
					const ChessSquare temp(kingLoc.row, 
									ydiff < 0 ? kingLoc.col + i : kingLoc.col - i);
					if (IsAttackingSquare(temp) )
						PotMoves.push_back(ChessSquare(temp.row, temp.col) );
				}
			}
			else
			{
				for (int i = 1; i < abs(xdiff); i++)
				{
					const ChessSquare temp(xdiff < 0 ? kingLoc.row + i: kingLoc.row - i, 
									kingLoc.col);
					if (IsAttackingSquare(temp) )
						PotMoves.push_back(ChessSquare(temp.row, temp.col) );
				}
			}

		}
		
		
		
		int diffx = currLoc.row - checkLoc.row;
		int diffy = currLoc.col - checkLoc.col;
		
		if (abs(diffx) == abs(diffy) )
		{
			for (int i = 1; i < diffx; i++)
			{
				if( ChessBoard::GetInstance()->bHasPiece( diffx > 0 ? currLoc.row + i : currLoc.row - i, 
														 diffy > 0 ? currLoc.col + i : currLoc.col - i ) )
				   return;
			}
			PotMoves.push_back(ChessSquare(checkLoc.row, checkLoc.col)) ;
		}
		return;
	}*/
	
	//int limOne = currLoc.row;

	//int limTwo = currLoc.col;
	
	ChessSquare tempMove = currLoc;
	
	for (int i = 0; i < 8; i++)
	{
		tempMove.row--;
		tempMove.col--;
		
		if (tempMove.row < 0 || tempMove.col < 0 || tempMove.row > 7 || tempMove.col > 7)
			break;
		
		if (! (ChessBoardInstance->bHasPiece(tempMove.row, tempMove.col)))
		{
			if (ChessPiece::ValidCheckStateMove(tempMove.row, tempMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, tempMove.row, tempMove.col, false, ChessBoardInstance));
		}
		else {
			ChessPiece* piece = ChessBoardInstance->GetPiece(tempMove.row, tempMove.col);
			if( piece && (piece->isWhite() != bIsWhite ) )
			{
				if (ChessPiece::ValidCheckStateMove(tempMove.row, tempMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, tempMove.row, tempMove.col, true, ChessBoardInstance));
			}
			break;
		}

	}
	tempMove = currLoc;
	
	for (int i = 0; i < 8; i++)
	{
		tempMove.row++;
		tempMove.col++;
		
		if (tempMove.row < 0 || tempMove.col < 0 || tempMove.row > 7 || tempMove.col > 7)
			break;
		
		if (! (ChessBoardInstance->bHasPiece(tempMove.row, tempMove.col)))
		{
			if (ChessPiece::ValidCheckStateMove(tempMove.row, tempMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, tempMove.row, tempMove.col, false, ChessBoardInstance));
		}
		else {
			ChessPiece* piece = ChessBoardInstance->GetPiece(tempMove.row, tempMove.col);
			if( piece && (piece->isWhite() != bIsWhite ) )
			{
				if (ChessPiece::ValidCheckStateMove(tempMove.row, tempMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, tempMove.row, tempMove.col, true, ChessBoardInstance));
			}
			break;
		}
	}
	tempMove = currLoc;
	
	for (int i = 0; i < 8; i++)
	{
		tempMove.row++;
		tempMove.col--;
		
		if (tempMove.row < 0 || tempMove.col < 0 || tempMove.row > 7 || tempMove.col > 7)
			break;
		
		if (! (ChessBoardInstance->bHasPiece(tempMove.row, tempMove.col)))
		{
			if (ChessPiece::ValidCheckStateMove(tempMove.row, tempMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, tempMove.row, tempMove.col, false, ChessBoardInstance));
		}
		else {
			ChessPiece* piece = ChessBoardInstance->GetPiece(tempMove.row, tempMove.col);
			if( piece && (piece->isWhite() != bIsWhite ) )
			{
				if (ChessPiece::ValidCheckStateMove(tempMove.row, tempMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, tempMove.row, tempMove.col, true, ChessBoardInstance));
			}
			break;
		}
		
	}
	tempMove = currLoc;
	
	for (int i = 0; i < 8; i++)
	{
		tempMove.row--;
		tempMove.col++;
		
		if (tempMove.row < 0 || tempMove.col < 0 || tempMove.row > 7 || tempMove.col > 7)
			break;
		
		if (! (ChessBoardInstance->bHasPiece(tempMove.row, tempMove.col)))
		{
			if (ChessPiece::ValidCheckStateMove(tempMove.row, tempMove.col, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, tempMove.row, tempMove.col, false, ChessBoardInstance));
		}
		else {
			ChessPiece* piece = ChessBoardInstance->GetPiece(tempMove.row, tempMove.col);
			if( piece && (piece->isWhite() != bIsWhite ) )
			{
				if (ChessPiece::ValidCheckStateMove(tempMove.row, tempMove.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, tempMove.row, tempMove.col, true, ChessBoardInstance));
			}
			break;
		}
		
	}
}
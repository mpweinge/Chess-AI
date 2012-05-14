/*
 *  Rook.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-29.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Rook.h"

std::vector<ChessMove>& Rook::GetEvalLegalMoves()
{
	return EvalMoves;
}

bool Rook::MakeMove(const Move& Move2)
{
	
	if ( (Move2.row != currLoc.row) && (Move2.col != currLoc.col) )
		return false;
	
	int diffx = Move2.row - currLoc.row;
	int diffy = Move2.col - currLoc.col;
	
	for (int i = 1; i < (abs(diffx) > abs(diffy) ? abs(diffx) : abs(diffy)); i++)
	{
		if (diffx != 0)
		{
			if (ChessBoardInstance->bHasPiece( (diffx>0) ? (currLoc.row + i) : (currLoc.row - i),
													 currLoc.col) )
				return false;
		}
		if (diffy != 0)
		{
			if (ChessBoardInstance->bHasPiece(currLoc.row, 
													 (diffy > 0) ? (currLoc.col + i): (currLoc.col - i)) )
				return false;
		}
			
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
		bMoved = true;
		return true;
	}
	else {
		return false;
	}
}

bool Rook::CheckForCapture(const Move& MoveTo) 
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
				ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
				LookForCheck(false);
				bMoved = true;
				return true;
			}
			return false;
		}
	}
	//We already checked for capturing
	ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
	LookForCheck(false);
	bMoved = true;*/
	
	if (ChessPiece::ValidCheckStateMove(MoveTo.row, MoveTo.col, this) )
	{
		//We already checked for capturing
		ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
		//LookForCheck(false);
		return true;
	}
	return false;
	//return true;
}

bool Rook::LookForCheck(bool test) const
{
	Move KingLoc = ChessBoardInstance->GetKingLocation(bIsWhite);
	
	if (IsAttackingSquare(KingLoc) )
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

bool Rook::IsAttackingSquare(const Move& Square) const
{
	int diffx = Square.row - currLoc.row;
	int diffy = Square.col - currLoc.col;
	if ( ( diffx != 0 ) && (diffy != 0) )
		return false;
	
	int lim = (diffx == 0)? abs(diffy) : abs(diffx);
	
	for (int i = 1; i < lim; i++)
	{
		if (diffx == 0)
		{
			if (ChessBoardInstance->bHasPiece(currLoc.row, 
													 diffy > 0 ? currLoc.col + i: currLoc.col - i))
				return false;
		}
		if (diffy == 0)
		{
			if (ChessBoardInstance->bHasPiece(diffx > 0 ? currLoc.row+i : currLoc.row - i, 
													 currLoc.col))
				return false;
		}
	}
	return true;
}

Rook::Rook(const int& startingX, const int& startingY, const bool _isWhite, ChessBoard* _Instance)
{
	currLoc = Move(startingX, startingY);
	bIsWhite = _isWhite;
	bIsBlockingCheck = false;
	bMoved = 0;
	bIsInKingArray = false;
	ChessBoardInstance = _Instance;
}

double Rook::GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax)
{
	EvalMoves.clear();
	
	if (bReturnMax)
	{
		return calcChromo->GetWeighting(RookMobilityWeight);
	}
	
	GetLegalMoves(EvalMoves);
	int mobilityScore = (EvalMoves.size() / MAXROOKMOVES) * calcChromo->GetWeighting(RookMobilityWeight);

	return mobilityScore;
}

double Rook::GetDefendingScore(const Chromosome* calcChromo)
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
	return numDefendingPieces * calcChromo->GetWeighting(RookDefendingWeight);
}

double Rook::GetPieceUnderAttackScore(const Chromosome* calcChromo)
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
	return numAttackingPieces * calcChromo->GetWeighting(RookUnderAttackWeight);
}

bool Rook::HasLegalMove()
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

void Rook::GetLegalMoves(std::vector<ChessMove>& PotMoves)
{
	Player* player;
	if (bIsWhite)
	{
		player = ChessBoardInstance->GetWhitePlayer();
	}
	else {
		player = ChessBoardInstance->GetBlackPlayer();
	}
	
	if ( isBlockingCheck()   )
	{
		/*Move checkLoc = player->GetCheckingPieceLocation();
		int diffx = currLoc.row - checkLoc.row;
		int diffy = currLoc.col - checkLoc.col;
		
		if( (diffx == 0) || (diffy == 0) )
		{
			if (diffx == 0)
			{
				for (int i = 1; i <= abs(diffy); i++)
				{
					if (ChessBoard::GetInstance()->bHasPiece( currLoc.row, diffy > 0 ? currLoc.col + i : currLoc.col - i ))
						return;
					else 
					{
						PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row, diffy > 0 ? currLoc.col + i : currLoc.col - i, true));
					}

				}
			}
			else
			{
				for (int i = 1; i <= abs(diffx); i++)
				{
					if (ChessBoard::GetInstance()->bHasPiece( diffx > 0 ? currLoc.row+i : currLoc.row - i, currLoc.col ))
						return;
					else {
						PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row, diffx > 0 ? currLoc.row + i: currLoc.row - i, currLoc.col, true));
					}
					
				}
			}
										   
		}*/
		return;
	}
	
	/*if (player->IsInCheck() )
	{
		Move checkLoc = player->GetCheckingPieceLocation();
		Move kingLoc = player->GetKingLocation();
		char p = pieceToText(player->GetCheckingPiece());
		
		if( ( p != 'N') && (p != 'P') )
		{
			int xdiff = kingLoc.row - checkLoc.row;
			int ydiff = kingLoc.col - checkLoc.col;
			
			if (abs(xdiff) == abs(ydiff))
			{
				for (int i = 1; i < abs(xdiff); i++)
				{
					const Move temp(xdiff < 0 ? kingLoc.row + i : kingLoc.row - i, 
									ydiff < 0 ? kingLoc.col + i : kingLoc.col - i);
					if (IsAttackingSquare(temp) )
						PotMoves.push_back(Move(temp.row, temp.col) );
				}
			}
			else if (xdiff == 0)
			{
				for (int i = 1; i < abs(ydiff); i++)
				{
					const Move temp(kingLoc.row, 
									ydiff < 0 ? kingLoc.col + i : kingLoc.col - i);
					if (IsAttackingSquare(temp) )
						PotMoves.push_back(Move(temp.row, temp.col) );
				}
			}
			else
			{
				for (int i = 1; i < abs(xdiff); i++)
				{
					const Move temp(xdiff < 0 ? kingLoc.row + i: kingLoc.row - i, 
									kingLoc.col);
					if (IsAttackingSquare(temp) )
						PotMoves.push_back(Move(temp.row, temp.col) );
				}
			}
			
		}
		
		int diffx = currLoc.row - checkLoc.row;
		int diffy = currLoc.col - checkLoc.col;
		
		if( (diffx == 0) || (diffy == 0) )
		{
			if (diffx == 0)
			{
				for (int i = 1; i < abs(diffy); i++)
				{
					if (ChessBoard::GetInstance()->bHasPiece( currLoc.row, diffy > 0 ? currLoc.col + i : currLoc.col - i ));
						return;
				}
				PotMoves.push_back(Move(checkLoc.row, checkLoc.col)) ;
			}
			else
			{
				for (int i = 1; i < abs(diffx); i++)
				{
					if (ChessBoard::GetInstance()->bHasPiece( diffx > 0 ? currLoc.row+i : currLoc.row - i, currLoc.col ));
						return;
				}
				PotMoves.push_back(Move(checkLoc.row, checkLoc.col)) ;
			}
						
		}
		return;
	}*/
	
	int diffx = 7 - currLoc.row;
	int diffy = 7 - currLoc.col;
	
	for (int i = 1; i <= diffx; i++)
	{
		if ( !(ChessBoardInstance->bHasPiece( currLoc.row + i,currLoc.col) ) )
		{
			if (ChessPiece::ValidCheckStateMove(currLoc.row+i, currLoc.col, this))
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row+i, currLoc.col, false, ChessBoardInstance) );
		}
		else {
			if (ChessBoardInstance->GetPiece(currLoc.row+i, currLoc.col)->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currLoc.row+i, currLoc.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row+i, currLoc.col, true, ChessBoardInstance) );
			}
			break;
		}

	}
	
	for (int i = 1; i <= diffy; i++)
	{

		if ( !(ChessBoardInstance->bHasPiece( currLoc.row,currLoc.col+i) ) )
		{
			if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+i, this))
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row, currLoc.col+i, false, ChessBoardInstance) );
		}
		else {
			if (ChessBoardInstance->GetPiece(currLoc.row, currLoc.col+i)->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+i, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row, currLoc.col+i, true, ChessBoardInstance) );
			}
			break;
		}

	}
	
	for (int i = 1; i <= 7 - diffx; i++)
	{
		if ( !(ChessBoardInstance->bHasPiece( currLoc.row-i,currLoc.col) ) )
		{
			if (ChessPiece::ValidCheckStateMove(currLoc.row-i, currLoc.col, this))
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row-i, currLoc.col, false, ChessBoardInstance) );
		}
		else {
			if (ChessBoardInstance->GetPiece(currLoc.row-i, currLoc.col)->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currLoc.row-i, currLoc.col, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row-i, currLoc.col, true, ChessBoardInstance) );
			}
			break;
		}

	}
	
	for (int i = 1; i <= 7 - diffy; i++)
	{
		
		if ( !(ChessBoardInstance->bHasPiece( currLoc.row,currLoc.col-i) ) )
		{
			if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col-i, this))
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row, currLoc.col-i, false, ChessBoardInstance) );
		}
		else {
			if (ChessBoardInstance->GetPiece(currLoc.row, currLoc.col-i)->isWhite() != bIsWhite)
			{
				if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col-i, this) )
					PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row, currLoc.col-i, true, ChessBoardInstance) );
			}
			break;
		}

	}							   
							   
}
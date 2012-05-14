/*
 *  Queen.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-30.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Queen.h"

std::vector<ChessMove>& Queen::GetEvalLegalMoves()
{
	return EvalMoves;
}

bool Queen::MakeMove(const Move& Move2)
{	
	int diffx = Move2.row - currLoc.row;
	int diffy = Move2.col - currLoc.col;
	
	if ( (abs(diffx) != abs(diffy)) &&  (diffx != 0) && (diffy != 0) )
		return false;
	
	int lim = (abs(diffx) > abs(diffy)) ? abs(diffx) : abs(diffy);
	
	for (int i = 1; i < lim; i++)
	{
		if (diffx != 0)
		{
			if (diffy != 0)
			{
					if (ChessBoardInstance->bHasPiece( (diffx > 0) ? currLoc.row+i: currLoc.row - i , 
															 (diffy > 0) ? currLoc.col+i : currLoc.col - i) )
						return false;
			}
			else {
				if (ChessBoardInstance->bHasPiece( (diffx > 0) ? currLoc.row+i: currLoc.row - i , 
														 currLoc.col ) )
					return false;
			}

		}
		else
		{
			if (ChessBoardInstance->bHasPiece( currLoc.row, 
													 (diffy > 0) ? currLoc.col + i : currLoc.col - i) )
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
		return true;
	}
	else {
		return false;
	}
	
}

bool Queen::CheckForCapture(const Move& MoveTo)
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
	}
	//We already checked for capturing
	ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
	LookForCheck(false);*/
	
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

Queen::Queen(const int& startingX, const int& startingY, const bool& _isWhite, ChessBoard* _Instance)
{
	currLoc = Move(startingX, startingY);
	bIsWhite = _isWhite;
	bIsBlockingCheck = false;
	bIsInKingArray = false;
	ChessBoardInstance = _Instance;
}

bool Queen::LookForCheck(bool test) const
{
	Move KingLoc = ChessBoardInstance->GetKingLocation(bIsWhite);
	
	if ( IsAttackingSquare (KingLoc) )
	{
		if (!test)
		{
			printf("Check");
			ChessBoardInstance->PutPlayerInCheck(bIsWhite, this);
		}
		return true;
	}
	return false;
}

bool Queen::IsAttackingSquare(const Move& Square) const
{
	int diffx = Square.row - currLoc.row;
	int diffy = Square.col - currLoc.col;
	if( ( abs(diffx) != abs(diffy) ) )
	{
	    if((diffx != 0) && (diffy != 0)) 
			return false;
	}
	
	if ( abs(diffx) == abs(diffy) )
	{
		for (int i = 1; i < abs(diffx); i++)
		{
			if (ChessBoardInstance->bHasPiece(diffx > 0 ? currLoc.row + i:currLoc.row-i, 
													 diffy>0 ? currLoc.col + i: currLoc.col - i))
				return false;
		}
	}
	else 
	{
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
	}
	
	return true;
}

double Queen::GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax)
{
	EvalMoves.clear();
	
	if (bReturnMax)
	{
		return calcChromo->GetWeighting(QueenMobilityWeight);
	}
	
	GetLegalMoves(EvalMoves);
	int mobScore = (EvalMoves.size() / MAXQUEENMOVES) * calcChromo->GetWeighting(QueenMobilityWeight);

	return mobScore;
}

double Queen::GetDefendingScore(const Chromosome* calcChromo)
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
	return numDefendingPieces * calcChromo->GetWeighting(QueenDefendingWeight);
}

double Queen::GetPieceUnderAttackScore(const Chromosome* calcChromo)
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
	return numAttackingPieces * calcChromo->GetWeighting(QueenUnderAttackWeight);
}

bool Queen::HasLegalMove()
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

void Queen::GetLegalMoves(std::vector<ChessMove>& PotMoves)
{
	
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
		Move checkLoc = player->GetCheckingPieceLocation();
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
					PotMoves.push_back(Move( diffx > 0 ? currLoc.row + i : currLoc.row - i, 
												diffy > 0 ? currLoc.col + i : currLoc.col - i ) );
				}
				
			}
			PotMoves.push_back(Move(checkLoc.row, checkLoc.col)) ;
		}
		else if (diffx == 0)
		{
			for (int i = 1; i < abs(diffy); i++)
			{
				if (ChessBoard::GetInstance()->bHasPiece( currLoc.row, diffy > 0 ? currLoc.col + i : currLoc.col - i ))
					return;
				else 
				{
					PotMoves.push_back(Move( currLoc.row, 
												diffy > 0 ? currLoc.col + i : currLoc.col - i ) );
				}
				
			}
		}
		else if (diffy == 0)
		{
			for (int i = 1; i < abs(diffx); i++)
			{
				if (ChessBoard::GetInstance()->bHasPiece( diffx > 0 ? currLoc.row+i : currLoc.row - i, currLoc.col ))
					return;
				else {
					PotMoves.push_back(Move( diffx > 0 ? currLoc.row + i: currLoc.row - i, 
												currLoc.col ) );
				}
				
			}
		}
		return;
	}*/
	
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
		if (abs(diffx) == abs(diffy) )
		{
			for (int i = 1; i < abs(diffx); i++)
			{
				if( ChessBoard::GetInstance()->bHasPiece( diffx > 0 ? currLoc.row + i : currLoc.row - i, 
														 diffy > 0 ? currLoc.col + i : currLoc.col - i ) )
					return;
			}
			PotMoves.push_back(Move(checkLoc.row, checkLoc.col)) ;
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
	
	//Bishops move in diagonals until the first piece
	
	//int limOne = currLoc.row;
	
	//int limTwo = currLoc.col;
	
	Move tempMove = currLoc;
	
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
}
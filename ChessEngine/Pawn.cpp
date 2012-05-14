/*
 *  Pawn.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-30.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Pawn.h"
#include "ChessMove.h"
#include "math.h"

std::vector<ChessMove>& Pawn::GetEvalLegalMoves()
{
	return EvalMoves;
}

bool Pawn::MakeMove(const Move& Move2)
{	

	if ( (bIsWhite) && (Move2.col == currLoc.col + 1) && (abs(Move2.row - currLoc.row) == 1) )
	{
		if (ChessPiece::ValidCheckStateMove(Move2, this))
			return CheckForCapture(Move2);
		else {
			return false;
		}

	}
	
	if ( (!bIsWhite) && (Move2.col == currLoc.col - 1) && (abs(Move2.row - currLoc.row) == 1) )
	{
		if (ChessPiece::ValidCheckStateMove(Move2, this))
			return CheckForCapture(Move2);
		else {
			return false;
		}
	}
	
	if (ChessBoardInstance->bHasPiece(Move2.row, Move2.col))
		return false;
	
	if ( (Move2.row != currLoc.row) )
		return false;
	
	if (( bIsWhite && (Move2.col == currLoc.col+2) && (currLoc.col == 1)) )
	{
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
	
	if ((!bIsWhite && (Move2.col == currLoc.col-2) && (currLoc.col == 6)) )
	{
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
		
	if (
		(Move2.col == currLoc.col + 1) && bIsWhite && (Move2.row == currLoc.row)
		)
	{
		if (ChessPiece::ValidCheckStateMove(Move2, this))
		{
			
			if (Move2.col == 7)
			{
				ChessBoardInstance->PromotePawn(const_cast<Pawn*>(this), &Move2, 'Q');
			}
			else {
				ChessBoardInstance->MovePiece(this, Move2.row, Move2.col);
			}

			LookForCheck(false);
			return true;
		}
		else {
			return false;
		}
	}
	
	if (
		(Move2.col == currLoc.col - 1) && !bIsWhite && (Move2.row == currLoc.row)
		)
	{
		if (ChessPiece::ValidCheckStateMove(Move2, this))
		{
			if (Move2.col == 0)
			{
				ChessBoardInstance->PromotePawn(const_cast<Pawn*>(this), &Move2, 'Q');
			}
			else {
				ChessBoardInstance->MovePiece(this, Move2.row, Move2.col);
			}

			LookForCheck(false);
			return true;
		}
		else {
			return false;
		}
	}
	
	return false;
	
}

bool Pawn::CheckForCapture(const Move& MoveTo)
{
	//ChessAssert();
	/*ChessPiece* captPiece = NULL;*/
	ChessPiece* captPiece = NULL;
	if (CheckForEnPassant(MoveTo, captPiece))
	{
		ChessBoard::GetInstance()->EnPassantCapture(this, MoveTo, captPiece->GetLocation());
		//LookForCheck(false);
		return true;
	}
	/*
	ChessPiece* piece = ChessBoard::GetInstance()->GetPiece(MoveTo.row, MoveTo.col);
	if ( piece && (bIsWhite != piece->isWhite() ) )
	{
		if (bIsWhite)
		{
			if ( WhitePlayer::GetInstance()->IsInCheck() )
			{
				Move checkLoc = WhitePlayer::GetInstance()->GetCheckingPieceLocation();
				if( (checkLoc.row == MoveTo.row) && (checkLoc.col == MoveTo.col) )
				{
					if (MoveTo.col == 7)
					{
						ChessBoard::GetInstance()->PromotePawn(this, &MoveTo, 'Q');
					}
					else {
						ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
					}

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
					if (MoveTo.col == 7)
					{
						ChessBoard::GetInstance()->PromotePawn(const_cast<Pawn*>(this), &MoveTo, 'Q');
					}
					else {
						ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
					}

					LookForCheck(false);
					return true;
				}
				return false;
			}
		}
		//Time to capture
		
		if (MoveTo.col == 7)
		{
			ChessBoard::GetInstance()->PromotePawn(const_cast<Pawn*>(this), &MoveTo, 'Q');
		}
		else {
			ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
		}

		LookForCheck(false);
		return true;
	}*/
	/*if (CheckForEnPassant(MoveTo, captPiece))
	{
		//ChessBoard::GetInstance()->EnPassantCapture(this, MoveTo, );
		//LookForCheck(false);
		return true;
	}*/
	//Check if there is a piece
	ChessPiece* CaptPiece = ChessBoardInstance->GetPiece(MoveTo);
	if (CaptPiece == NULL)
		return false;
	if (ChessPiece::ValidCheckStateMove(MoveTo.row, MoveTo.col, this) )
	{
		//We already checked for capturing
		if (MoveTo.col == 0 && !bIsWhite)
		{
			ChessBoardInstance->PromotePawn(const_cast<Pawn*>(this), &MoveTo, 'Q');
			return true;
		}
		else if (MoveTo.col == 7 && bIsWhite)
		{
			ChessBoardInstance->PromotePawn(const_cast<Pawn*>(this), &MoveTo, 'Q');
			return true;
		}
		ChessBoard::GetInstance()->MakeCapture(this, MoveTo);
		//LookForCheck(false);
		return true;
	}
	//return false;
	return false;
}

bool Pawn::CheckForEnPassant(const Move& MoveTo, ChessPiece* &_captPiece)
{
	//check the location
	if( (bIsWhite && (currLoc.col == 4) ) || (!bIsWhite && (currLoc.col == 3) ) )
	{
		//Check the last move
		ChessMove* lastMove = ChessBoardInstance->GetLog()->getLastMove();
		if (lastMove->isCastle() )
			return false;
		if (lastMove->isPromotion() )
			return false;
		if (lastMove->isCapture() )
			return false;
		if (bIsWhite)
		{
			Move captPiece = Move(lastMove->getRow(), lastMove->getCol());
			if (
				(lastMove->getStartingCol() == 6) && (lastMove->getCol() == 4)
				&& (lastMove->getRow() == MoveTo.row)
				&& (pieceToText(lastMove->getPiece()) == 'P')
				)
			{
				/*if (WhitePlayer::GetInstance()->IsInCheck() )
				{
					Move checkLoc = BlackPlayer::GetInstance()->GetCheckingPieceLocation();
					if( (checkLoc.row == lastMove->getRow()) && (checkLoc.col == lastMove->getCol() ) )
					{
						//ChessBoard::GetInstance()->EnPassantCapture(this, MoveTo, *captPiece);
						_captPiece = ChessBoard::GetInstance()->GetPiece(captPiece);
						return true;
					}
					else {
						//Discovered check, ya asshole
						//ChessBoard::GetInstance()->EnPassantCapture(this, MoveTo, *captPiece);
						return false;
					}

				}*/
				//ChessBoard::GetInstance()->EnPassantCapture(this, MoveTo, *captPiece);
				_captPiece = ChessBoardInstance->GetPiece(captPiece);
				ChessBoardInstance->RemovePiece(captPiece.row, captPiece.col);
				if (ChessPiece::ValidCheckStateMove(MoveTo.row, MoveTo.col, this) )
				{
					ChessBoardInstance->AddPiece(_captPiece);
					return true;
				}
				ChessBoardInstance->AddPiece(_captPiece);
				return false;
			}
		}
		else 
		{
			Move captPiece = Move(lastMove->getRow(), lastMove->getCol());
			if (
				(lastMove->getStartingCol() == 1) && (lastMove->getCol() == 3)
				&& (lastMove->getRow() == MoveTo.row)
				&& (pieceToText(lastMove->getPiece()) == 'P')
				)
			{
				/*if (BlackPlayer::GetInstance()->IsInCheck() )
				{
					Move checkLoc = WhitePlayer::GetInstance()->GetCheckingPieceLocation();
					if( (checkLoc.row == lastMove->getRow()) && (checkLoc.col == lastMove->getCol() ) )
					{
						//ChessBoard::GetInstance()->EnPassantCapture(this, MoveTo, *captPiece);
						_captPiece = ChessBoard::GetInstance()->GetPiece(captPiece);
						return true;
					}
					else {
						//Discovered check, ya asshole
						//ChessBoard::GetInstance()->EnPassantCapture(this, MoveTo, *captPiece);
						return false;
					}
					
				}*/
				//ChessBoard::GetInstance()->EnPassantCapture(this, MoveTo, *captPiece);
				_captPiece = ChessBoardInstance->GetPiece(captPiece);
				ChessBoardInstance->RemovePiece(captPiece.row, captPiece.col);
				if (ChessPiece::ValidCheckStateMove(MoveTo.row, MoveTo.col, this) )
				{
					ChessBoardInstance->AddPiece(_captPiece);
					return true;
				}
				ChessBoardInstance->AddPiece(_captPiece);
				return false;
			}
		}

	}
	return false;
}

bool Pawn::LookForCheck(bool test) const
{
	Move KingLoc = ChessBoardInstance->GetKingLocation(bIsWhite);
	if( IsAttackingSquare(KingLoc) )
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

bool Pawn::IsAttackingSquare(const Move& Square) const
{
	int diffx = abs(Square.row - currLoc.row);
	if( diffx == 1)
	{
		if (bIsWhite)
		{
			if (Square.col == (currLoc.col+1))
				return true;
		}
		else {
			if (Square.col == (currLoc.col-1))
				return true;
		}
	}
	return false;
}

Pawn::Pawn(const int& startingX, const int& startingY, const bool& _isWhite, ChessBoard* _Instance)
{
	currLoc = Move(startingX, startingY);
	bIsWhite = _isWhite;
	bIsBlockingCheck = false;
	bIsInKingArray = false;
	ChessBoardInstance = _Instance;
}

double Pawn::GetMobilityScore(const Chromosome* calcChromo, bool bReturnMax)
{
	EvalMoves.clear();
	
	if (bReturnMax)
	{
		return calcChromo->GetWeighting(PawnMobilityWeight);
	}
	
	GetLegalMoves(EvalMoves);
	int mobilityScore = (EvalMoves.size() / MAXPAWNMOVES) * calcChromo->GetWeighting(PawnMobilityWeight);

	return mobilityScore;
}

double Pawn::GetDefendingScore(const Chromosome* calcChromo)
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
	return numDefendingPieces * calcChromo->GetWeighting(PawnDefendingWeight);
}

double Pawn::GetPieceUnderAttackScore(const Chromosome* calcChromo)
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
	return numAttackingPieces * calcChromo->GetWeighting(PawnUnderAttackWeight);
}

bool Pawn::HasLegalMove()
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

void Pawn::GetLegalMoves(std::vector<ChessMove>& PotMoves)
{
	Player* player;
	Move EnPassant1(currLoc.row-1, currLoc.col);
	Move EnPassant2(currLoc.row+1, currLoc.col);
	int incr;
	if (bIsWhite)
	{
		player = ChessBoardInstance->GetWhitePlayer();
		incr = 1;
		EnPassant1.col = currLoc.col + 1;
		EnPassant2.col = currLoc.col + 1;
	}
	else {
		player = ChessBoardInstance->GetBlackPlayer();
		incr = -1;
		EnPassant1.col = currLoc.col - 1;
		EnPassant2.col = currLoc.col - 1;
	}
	ChessPiece* _captPiece;
	if ( CheckForEnPassant(EnPassant1, _captPiece) )
	{
		PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, EnPassant1.row, EnPassant1.col, true, ChessBoardInstance));
	}
	if ( CheckForEnPassant(EnPassant2, _captPiece) )
	{
		PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, EnPassant2.row, EnPassant2.col, true, ChessBoardInstance));
		
	}
	
	/*if ( isBlockingCheck() )
	{
		Move checkLoc = player->GetCheckingPieceLocation();
		
		if (checkLoc.row == currLoc.row )
		{
			if(( (checkLoc.col - currLoc.col) > 1 ) && !ChessBoard::GetInstance()->bHasPiece(currLoc.row, currLoc.col+incr) 
			   && ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+incr, this)
			   )
				PotMoves.push_back(Move(currLoc.row, currLoc.col+incr) );
			else if(( (checkLoc.col - currLoc.col) < 0) && !ChessBoard::GetInstance()->bHasPiece(currLoc.row, currLoc.col+incr) 
					&& ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+incr, this)
					)
				PotMoves.push_back(Move(currLoc.row, currLoc.col+incr) );
		}
		return;
	}
	else if (player->IsInCheck() )
	{
		Move checkLoc = player->GetCheckingPieceLocation();
		Move kingLoc = player->GetKingLocation();
		char p = pieceToText(player->GetCheckingPiece());
		
		if( ( p != 'N') && (p != 'P') )
		{
			
			if (!ChessBoard::GetInstance()->bHasPiece(currLoc.row, currLoc.col+incr))
			{
				if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+incr, this) )
					PotMoves.push_back(Move(currLoc.row, currLoc.col+incr));
				if (!ChessBoard::GetInstance()->bHasPiece(currLoc.row, currLoc.col+2*incr))
				{
					if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+2*incr, this) 
						&&( ( bIsWhite && (currLoc.col == 1)) || (!bIsWhite && (currLoc.col == 7)))
						&& !(ChessBoard::GetInstance()->bHasPiece( currLoc.row,currLoc.col + incr) )
						)
						PotMoves.push_back(Move(currLoc.row, currLoc.col+2*incr));
				}
			}
			ChessPiece *leftPiece = ChessBoard::GetInstance()->GetPiece(currLoc.row-1, currLoc.col+incr);
			ChessPiece *rightPiece = ChessBoard::GetInstance()->GetPiece(currLoc.row+1, currLoc.col+incr);
			if (ChessPiece::ValidCheckStateMove(currLoc.row+1, currLoc.col+incr, this) )
			{
				if ( rightPiece && (bIsWhite != rightPiece->isWhite() ) )
				{
					if( (bIsWhite && (currLoc.col == 6) ) || (!bIsWhite) && (currLoc.col == 1) )
					{
						PotMoves.push_back(new PromotionMove(currLoc.row+1, currLoc.col+incr, queen));
						PotMoves.push_back(new PromotionMove(currLoc.row+1, currLoc.col+incr, knight));	
						PotMoves.push_back(new PromotionMove(currLoc.row+1, currLoc.col+incr, rook));
						PotMoves.push_back(new PromotionMove(currLoc.row+1, currLoc.col+incr, bishop));
					}
					else
						PotMoves.push_back(Move(currLoc.row+1, currLoc.col+incr) );
				}
				else {
				}

			}
			if (ChessPiece::ValidCheckStateMove(currLoc.row-1, currLoc.col+incr, this) )
			{
				if ( leftPiece && (bIsWhite != leftPiece->isWhite() ) )
				{
					if( (bIsWhite && (currLoc.col == 6) ) || (!bIsWhite) && (currLoc.col == 1) )
					{
						PotMoves.push_back(new PromotionMove(currLoc.row-1, currLoc.col+incr, queen));
						PotMoves.push_back(new PromotionMove(currLoc.row-1, currLoc.col+incr, knight));	
						PotMoves.push_back(new PromotionMove(currLoc.row-1, currLoc.col+incr, rook));
						PotMoves.push_back(new PromotionMove(currLoc.row-1, currLoc.col+incr, bishop));
					}
					else
						PotMoves.push_back(Move(currLoc.row-1, currLoc.col+incr) );
				}
				else {
				}
				
			}
				

		}
		else {
			if (bIsWhite)
			{
				if ((checkLoc.col == currLoc.col + 1) && abs((checkLoc.row - currLoc.row) == 1) )
				{
					if (ChessPiece::ValidCheckStateMove(checkLoc.row, currLoc.col+1, this) )
						PotMoves.push_back(Move(checkLoc.row, currLoc.col+1) );
				}
			}
			else {
				if ((checkLoc.col == currLoc.col - 1) && abs((checkLoc.row - currLoc.row) == 1) )
				{
					if (ChessPiece::ValidCheckStateMove(checkLoc.row, currLoc.col-1, this) )
						PotMoves.push_back(Move(checkLoc.row, currLoc.col-1) );
				}
			}

		}

		return;
	}*/
	
	if ( !(ChessBoardInstance->bHasPiece( currLoc.row,currLoc.col + incr) ) )
	{
		if( (bIsWhite && (currLoc.col == 6) ) || (!bIsWhite) && (currLoc.col == 1) )
		{
			if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+incr, this) )
			{
				PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row, currLoc.col+incr, false, ChessBoardInstance));
				PotMoves[PotMoves.size()-1].SetPromotion(true);
				PotMoves[PotMoves.size()-1].SetPromotionChar('Q');
				
				PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row, currLoc.col+incr, false, ChessBoardInstance));	
				PotMoves[PotMoves.size()-1].SetPromotion(true);
				PotMoves[PotMoves.size()-1].SetPromotionChar('N');
				
				PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row, currLoc.col+incr, false, ChessBoardInstance));
				PotMoves[PotMoves.size()-1].SetPromotion(true);
				PotMoves[PotMoves.size()-1].SetPromotionChar('R');
				
				PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row, currLoc.col+incr, false, ChessBoardInstance));
				PotMoves[PotMoves.size()-1].SetPromotion(true);
				PotMoves[PotMoves.size()-1].SetPromotionChar('B');
			}
		}
		else 
		{
			if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+incr, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row, currLoc.col+incr, false, ChessBoardInstance) );
		}
		
		if ( 
			( ( bIsWhite && (currLoc.col == 1)) || ( !bIsWhite && (currLoc.col == 6)))
			&& !(ChessBoardInstance->bHasPiece( currLoc.row,currLoc.col + 2*incr) )
			)
		{
			if (ChessPiece::ValidCheckStateMove(currLoc.row, currLoc.col+2*incr, this) )
				PotMoves.push_back(ChessMove(this, currLoc.row, currLoc.col, currLoc.row, currLoc.col+2*incr, false, ChessBoardInstance) );
		}
	}
	
	ChessPiece *leftPiece = ChessBoardInstance->GetPiece(currLoc.row-1, currLoc.col+incr);
	ChessPiece *rightPiece = ChessBoardInstance->GetPiece(currLoc.row+1, currLoc.col+incr);
	if ( leftPiece && (bIsWhite != leftPiece->isWhite() ) 
		&& ChessPiece::ValidCheckStateMove(currLoc.row-1, currLoc.col+incr, this)
		)
	{
		if( (bIsWhite && (currLoc.col == 6) ) || (!bIsWhite) && (currLoc.col == 1) )
		{
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row-1, currLoc.col+incr, true, ChessBoardInstance));
			PotMoves[PotMoves.size()-1].SetPromotion(true);
			PotMoves[PotMoves.size()-1].SetPromotionChar('Q');
			
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row-1, currLoc.col+incr, true, ChessBoardInstance));	
			PotMoves[PotMoves.size()-1].SetPromotion(true);
			PotMoves[PotMoves.size()-1].SetPromotionChar('N');
			
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row-1, currLoc.col+incr, true, ChessBoardInstance));
			PotMoves[PotMoves.size()-1].SetPromotion(true);
			PotMoves[PotMoves.size()-1].SetPromotionChar('R');
			
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row-1, currLoc.col+incr, true, ChessBoardInstance));
			PotMoves[PotMoves.size()-1].SetPromotion(true);
			PotMoves[PotMoves.size()-1].SetPromotionChar('B');
		}
		else
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row-1, currLoc.col+incr, true, ChessBoardInstance));
	}
	if ( rightPiece && (bIsWhite != rightPiece->isWhite() ) 
		&& ChessPiece::ValidCheckStateMove(currLoc.row+1, currLoc.col+incr, this)
		)
	{
		if( (bIsWhite && (currLoc.col == 6) ) || (!bIsWhite) && (currLoc.col == 1) )
		{
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row+1, currLoc.col+incr, true, ChessBoardInstance));
			PotMoves[PotMoves.size()-1].SetPromotion(true);
			PotMoves[PotMoves.size()-1].SetPromotionChar('Q');
			
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row+1, currLoc.col+incr, true, ChessBoardInstance));	
			PotMoves[PotMoves.size()-1].SetPromotion(true);
			PotMoves[PotMoves.size()-1].SetPromotionChar('N');
			
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row+1, currLoc.col+incr, true, ChessBoardInstance));
			PotMoves[PotMoves.size()-1].SetPromotion(true);
			PotMoves[PotMoves.size()-1].SetPromotionChar('R');
			
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row+1, currLoc.col+incr, true, ChessBoardInstance));
			PotMoves[PotMoves.size()-1].SetPromotion(true);
			PotMoves[PotMoves.size()-1].SetPromotionChar('B');
		}
		else
			PotMoves.push_back(ChessMove(this,currLoc.row, currLoc.col, currLoc.row+1, currLoc.col+incr, true, ChessBoardInstance));
	}
						   
}
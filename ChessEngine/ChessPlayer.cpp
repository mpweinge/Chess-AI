/*
 *  ChessPlayer.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-20.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ChessPlayer.h"

WhitePlayer* m_WhiteInstance;
BlackPlayer* m_BlackInstance;

WhitePlayer::WhitePlayer(bool bInit)
{
	if (bInit)
		InitWhitePlayer();
}

BlackPlayer::BlackPlayer(bool bInit)
{
	if (bInit)
		InitBlackPlayer();
}

WhitePlayer * WhitePlayer::GetInstance()
{
	if (m_WhiteInstance == NULL)
		m_WhiteInstance = new WhitePlayer(true);
	return m_WhiteInstance;
}

BlackPlayer * BlackPlayer::GetInstance()
{
	if (m_BlackInstance == NULL)
		m_BlackInstance = new BlackPlayer(true);
	return m_BlackInstance;
}

void WhitePlayer::EnterCheckState()
{
	bIsInCheck = true;
}

bool WhitePlayer::CheckForStalemate() const
{
	for (int i = 0; i < GetNumPiecesLeft(); i++)
	{
		if (GetPiece(i)->HasLegalMove() )
			return false;
	}
	return true;
}

bool BlackPlayer::CheckForStalemate() const
{
	for (int i = 0; i < GetNumPiecesLeft(); i++)
	{
		if (GetPiece(i)->HasLegalMove() )
			return false;
	}
	return true;
}

bool WhitePlayer::CheckForCheckMate()
{
	for (int i = 0; i < GetNumPiecesLeft(); i++)
	{
		if (GetPiece(i)->HasLegalMove() )
			return false;
	}
	//ChessBoard::GetInstance()->PrintBoard();
	bIsCheckmated = true;
	return true;
}

bool BlackPlayer::CheckForCheckMate()
{
	for (int i = 0; i < GetNumPiecesLeft(); i++)
	{
		if (GetPiece(i)->HasLegalMove() )
			return false;
	}
	//ChessBoard::GetInstance()->PrintBoard();
 	bIsCheckmated = true;
	return true;
}

void BlackPlayer::EnterCheckState()
{
	bIsInCheck = true;
}

ChessPiece* WhitePlayer::GetPiece(int index) const
{
	assert( (index > -1) && (index < remainingPieces.size()) );
	return remainingPieces[index];
}

ChessPiece* BlackPlayer::GetPiece(int index) const
{
	assert( (index > -1) && (index < remainingPieces.size()) );
	return remainingPieces[index];
}

void WhitePlayer::RemovePiece(ChessPiece* piece)
{
	for (int i = 0 ; i < remainingPieces.size(); i++)
	{
		if (remainingPieces[i]->isEqual (piece) )
		{
			capturedPieces.push_back(remainingPieces[i]);
			if (remainingPieces[i]->IsInKingArray() )
			{
				RemoveKingAttacking(remainingPieces[i]);
			}
			remainingPieces[i]->SetIsInKingArray(false);
			remainingPieces[i]->SetIndex(i);
			remainingPieces.erase(remainingPieces.begin()+i);
			break;
		}
		if (i == remainingPieces.size()-1)
			ChessAssert();
	}
}

void BlackPlayer::RemovePiece(ChessPiece* piece)
{
	for (int i = 0 ; i < remainingPieces.size(); i++)
	{
		if (remainingPieces[i]->isEqual (piece) )
		{
			capturedPieces.push_back(remainingPieces[i]);
			if (remainingPieces[i]->IsInKingArray() )
			{
				RemoveKingAttacking(remainingPieces[i]);
			}
			remainingPieces[i]->SetIsInKingArray(false);
			remainingPieces[i]->SetIndex(i);
			remainingPieces.erase(remainingPieces.begin()+i);
			break;
		}
		if (i == remainingPieces.size()-1)
			ChessAssert();
	}
}

Move WhitePlayer::GetKingLocation()
{
	char text;
	int i;
	for (i = 0; i < remainingPieces.size(); i++)
	{
		pieceToText(remainingPieces[i], text);
		if (text == 'K')
			break;
	}
	if(i == remainingPieces.size() )
		ChessAssert();
	return remainingPieces[i]->GetLocation();
}

Move BlackPlayer::GetKingLocation()
{
	char text;
	int i;
	for (i = 0; i < remainingPieces.size(); i++)
	{
		pieceToText(remainingPieces[i], text);
		if (text == 'K')
			break;
	}
	if(i == remainingPieces.size() )
		ChessAssert();
	return remainingPieces[i]->GetLocation();
}

void WhitePlayer::PawnPromotion(ChessPiece* pawn, ChessPiece* newPiece)
{
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		if (remainingPieces[i] ->isEqual( pawn) )
		{
			remainingPieces.erase(remainingPieces.begin() + i);
			if (pawn->IsInKingArray() )
			{
				RemoveKingAttacking(pawn);
			}
			//pawn->Delete();
			promotedPiece.push_back(pawn);
			break;
		}
		if (i == remainingPieces.size() - 1)
		{
			ChessAssert();
		}
	}
	remainingPieces.push_back(newPiece);
	//pieceToText(newPiece);
	//ChessBoard::GetInstance()->RemovePiece(currLoc.row, currLoc.col);
	ChessBoardInstance->AddPiece(newPiece);
}

void BlackPlayer::PawnPromotion( ChessPiece* pawn,  ChessPiece* newPiece)
{
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		if (remainingPieces[i] ->isEqual( pawn) )
		{
			remainingPieces.erase(remainingPieces.begin() + i);
			//pawn->Delete();
			if (pawn->IsInKingArray() )
			{
				RemoveKingAttacking(pawn);
			}
			promotedPiece.push_back(pawn);
			break;
		}
		if (i == remainingPieces.size() - 1)
		{
			ChessAssert();
		}
	}
	remainingPieces.push_back(newPiece);
	//pieceToText(newPiece);
	//ChessBoard::GetInstance()->RemovePiece(currLoc.row, currLoc.col);
	ChessBoardInstance->AddPiece(newPiece);
}

void WhitePlayer::UndoPawnPromotion(int row, Move promotionSquare, ChessPiece* PromotionPiece)
{
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		if( (remainingPieces[i]->isEqual(PromotionPiece) ) )
		{
			remainingPieces.erase(remainingPieces.begin()+i);
			//promotedPiece->Delete();
			for (int j = KingAttackingPieces.size() -1; j >= 0; j--)
			{
				if (PromotionPiece -> isEqual(KingAttackingPieces[j]))
					KingAttackingPieces.erase(KingAttackingPieces.begin() + j);
			}
			delete (PromotionPiece);
			PromotionPiece = NULL;
			break;
		}
		if (i == remainingPieces.size()-1)
			ChessAssert();
	}
	
	if (promotedPiece.size() < 1)
		ChessAssert();
	ChessPiece* LastPromotedPiece = promotedPiece.back();
	
	remainingPieces.push_back(LastPromotedPiece);
	LastPromotedPiece->SetRow(row);
	LastPromotedPiece->SetCol(6);
	//pieceToText(newPawn);
	ChessBoardInstance->AddPiece(LastPromotedPiece);
	promotedPiece.pop_back();
}

void BlackPlayer::UndoPawnPromotion(int row, Move promotionSquare, ChessPiece* PromotionPiece)
{
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		if( (remainingPieces[i]->isEqual(PromotionPiece) ) )
		{
			remainingPieces.erase(remainingPieces.begin()+i);
			//promotedPiece->Delete();
			for (int j = KingAttackingPieces.size() -1; j >= 0; j--)
			{
				if (PromotionPiece -> isEqual(KingAttackingPieces[j]))
					KingAttackingPieces.erase(KingAttackingPieces.begin() + j);
			}
			delete (PromotionPiece);
			PromotionPiece = NULL;
			break;
		}
		if (i == remainingPieces.size()-1)
			ChessAssert();
	}
	
	if (promotedPiece.size() < 1)
		ChessAssert();
	ChessPiece* LastPromotedPiece = promotedPiece.back();
	
	remainingPieces.push_back(LastPromotedPiece);
	LastPromotedPiece->SetRow(row);
	LastPromotedPiece->SetCol(1);
	//pieceToText(newPawn);
	ChessBoardInstance->AddPiece(LastPromotedPiece);
	promotedPiece.pop_back();
}

ChessPiece* WhitePlayer::FindCapturedPiece(const int row, const int col, ChessPiece* capturedPiece)
{
	if (!capturedPiece)
		ChessAssert();
	for (int i = capturedPieces.size() -1 ; i >= 0 ; i--)
	{
		if( capturedPieces[i]->isEqual (capturedPiece) )
		{
			return capturedPieces[i];
		}
	}
	ChessAssert();
	return NULL;
}

ChessPiece* BlackPlayer::FindCapturedPiece(const int row, const int col, ChessPiece* capturedPiece)
{
	if (!capturedPiece)
		ChessAssert();
	for (int i = capturedPieces.size() -1 ; i >= 0 ; i--)
	{
		if( capturedPieces[i]->isEqual (capturedPiece) )
		{
			return capturedPieces[i];
		}
	}
	ChessAssert();
	return NULL;
}

void BlackPlayer::RebirthCapturedPiece(ChessPiece* capturedPiece)
{
	for (int i = (capturedPieces.size()-1) ; i >=0 ; i--)
	{
		if(capturedPieces[i]->isEqual(capturedPiece) )
		{
			capturedPieces.erase(capturedPieces.begin()+i);
			break;
		}
		if (i == 0)
			ChessAssert();
	}
	
	remainingPieces.insert(remainingPieces.begin() + capturedPiece->GetIndex(), capturedPiece);
	//ChessBoard::GetInstance()->AddPiece(capturedPiece);
}

void WhitePlayer::RebirthCapturedPiece(ChessPiece* capturedPiece)
{
	for (int i = (capturedPieces.size()-1) ; i >=0 ; i--)
	{
		if(capturedPieces[i]->isEqual( capturedPiece) )
		{
			capturedPieces.erase(capturedPieces.begin()+i);
			break;
		}
		if (i == 0)
			ChessAssert();
	}
	
	remainingPieces.insert(remainingPieces.begin() + capturedPiece->GetIndex(), capturedPiece);
	//ChessBoard::GetInstance()->AddPiece(capturedPiece);
}

const bool WhitePlayer::CanCastle(bool bKingSide, ChessBoard* _Instance)
{
	return Player::CanCastle(true, bKingSide, _Instance);
}

const bool BlackPlayer::CanCastle(bool bKingSide, ChessBoard* _Instance)
{
	return Player::CanCastle(false, bKingSide, _Instance);
}

const bool Player::CanCastle(const bool bIsWhite, const bool bKingSide, ChessBoard* ChessBoardInstance)
{
	Player* currPlayer;
	int col;
	if (bIsWhite)
	{
		currPlayer = ChessBoardInstance->GetWhitePlayer();
		col = 0;
	}
	else {
		currPlayer = ChessBoardInstance->GetBlackPlayer();
		col = 7;
	}

	if (currPlayer->IsInCheck() )
	{
		#ifndef SUPPRESS_OUTPUT
		printf("Can't castle in check");
#endif
		return false;
	}

	int bAbleCastle;
	bAbleCastle = 0;
	ChessPiece* _King;
	ChessPiece* _Rook;

//if (bWhiteMove)
//	{
	for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
	{
		char name = pieceToText(currPlayer->GetPiece(i));
		if (name == 'K')
		{
			if ( (dynamic_cast<King*>(currPlayer->GetPiece(i)))->bHasMoved() )
			{
				#ifndef SUPPRESS_OUTPUT
				printf("King has moved!");
#endif
				//bAbleCastle = false;
				return false;
			}
			else {
				_King = currPlayer->GetPiece(i);
				bAbleCastle++;
			}
		
		}
		else if( (name == 'R') )
		{
			if ( !bKingSide )
			{
				//If the second rook has moved
				Move currLoc = currPlayer->GetPiece(i)->GetLocation();
				if (currLoc.row == 0 && currLoc.col == col)
				{
					if( (dynamic_cast<Rook*>(currPlayer->GetPiece(i)))->bHasMoved() )
					{
						#ifndef SUPPRESS_OUTPUT
						printf("Rook has moved!");
#endif
						//bAbleCastle = false;
						return false;
					}
					else {
						_Rook = currPlayer->GetPiece(i);
						bAbleCastle++;
					}
				
				}
			}
			else 
			{
				Move currLoc = currPlayer->GetPiece(i)->GetLocation();
				if (currLoc.row == 7 && currLoc.col == col)
				{
					if( (dynamic_cast<Rook*>(currPlayer->GetPiece(i)))->bHasMoved() )
					{
#ifndef SUPPRESS_OUTPUT
						printf("Rook has moved!");
#endif
						//bAbleCastle = false;
						return false;
					}
					else {
						_Rook = currPlayer->GetPiece(i);
						bAbleCastle++;
					}
					
				}
				
			}
		
		// }
		}
	}

	Player* opponent;
	if (bIsWhite)
		opponent = ChessBoardInstance->GetBlackPlayer();
	else {
		opponent = ChessBoardInstance->GetWhitePlayer();
	}

	Move test(1,col);

	for (int i = 0; i < opponent->GetNumPiecesLeft(); i++)
	{
		if( !bKingSide )
		{
			test.row = 1;
			test.col= col;
			if (opponent->GetPiece(i)->IsAttackingSquare(test ) )
			{
				bAbleCastle = 0;
#ifndef SUPPRESS_OUTPUT
				printf("Can't castle through check");
#endif
				return false;
			}
			test.row = 2;
			test.col= col;
			if (opponent->GetPiece(i)->IsAttackingSquare(test ) )
			{
				bAbleCastle = 0;
				#ifndef SUPPRESS_OUTPUT
				printf("Can't castle through check");
#endif
				return false;		
			}
			test.row = 3;
			test.col= col;
			if (opponent->GetPiece(i)->IsAttackingSquare(test ) )
			{
				bAbleCastle = 0;
				#ifndef SUPPRESS_OUTPUT
				printf("Can't castle through check");
#endif
				return false;
			}	
		}
		else 
		{
			test.row = 5;
			test.col= col;
			if (opponent->GetPiece(i)->IsAttackingSquare(test ) )
			{
				bAbleCastle = 0;
				#ifndef SUPPRESS_OUTPUT
				printf("Can't castle through check");
#endif
				return false;
			}
			test.row = 6;
			test.col= col;
			if (opponent->GetPiece(i)->IsAttackingSquare(test ) )
			{
				bAbleCastle = 0;
				#ifndef SUPPRESS_OUTPUT
				printf("Can't castle through check");
#endif
				return false;
			}
		}
	
	}

	if (bAbleCastle == 2)
	{
		//printf("Able to Castle");
	
		if ( !bKingSide )
		{
			if ( ChessBoardInstance->bHasPiece(1, col) ||
				ChessBoardInstance->bHasPiece(2, col) ||
				ChessBoardInstance->bHasPiece(3, col) )
			{
#ifndef SUPPRESS_OUTPUT
				printf("Piece in the way");
#endif
				return false;
			}
			else 
			{
				#ifndef SUPPRESS_OUTPUT
				printf("Castlang");
#endif
				return true;
			}
			
		}
		else 
		{
			if ( 
				(ChessBoardInstance->bHasPiece(5, col)) ||
				(ChessBoardInstance->bHasPiece(6, col)) 
				)
			{
#ifndef SUPPRESS_OUTPUT
				printf("Piece in the way");
#endif
				return false;
			}
			else {
#ifndef SUPPRESS_OUTPUT
				printf("Castlang");
#endif
				return true;
			}
		}
	}
	return false;
}

void WhitePlayer::ResetPlayer()
{
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		delete remainingPieces[i];
	}
	remainingPieces.clear();
	for (int i = 0; i < capturedPieces.size(); i++)
	{
		delete capturedPieces[i];
	}
	capturedPieces.clear();
	KingAttackingPieces.clear();
	if (checkingPiece)
		checkingPiece = NULL;
	if (promotedPiece.size() > 0)
	{
		for (int i = 0; i < promotedPiece.size(); i++)
		{
			delete promotedPiece[i];
		}
		promotedPiece.clear();
	}
	
	InitWhitePlayer();
}

void BlackPlayer::ResetPlayer()
{
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		delete remainingPieces[i];
	}
	remainingPieces.clear();
	for (int i = 0; i < capturedPieces.size(); i++)
	{
		delete capturedPieces[i];
	}
	capturedPieces.clear();
	KingAttackingPieces.clear();
	if (checkingPiece)
	{
		checkingPiece = NULL;
	}
	if (promotedPiece.size() > 0)
	{
		for (int i = 0; i < promotedPiece.size(); i++)
		{
			delete promotedPiece[i];
		}
	}
	promotedPiece.clear();
	InitBlackPlayer();
}

void WhitePlayer::AddToKingAttacking(ChessPiece* piece)
{
	if (piece->IsInKingArray() )
		return;
	piece->SetIsInKingArray(true);
	KingAttackingPieces.push_back(piece);
	//pieceToText(piece);
	//printf("-- Added piece %c to White King array at location %c%i --", pieceToText(piece), rowToLetter(piece->GetRow()), piece->GetCol()+1);
	//fflush(stdout);
}
void WhitePlayer::RemoveKingAttacking(ChessPiece* piece)
{
	for (int i = 0 ; i < KingAttackingPieces.size(); i++)
	{
		if (KingAttackingPieces[i]->isEqual (piece) )
		{
			KingAttackingPieces.erase(KingAttackingPieces.begin()+i);
			piece->SetIsInKingArray(false);
			//pieceToText(piece);
			//printf("-- Removing piece %c from White King array at location %c%i --", pieceToText(piece), rowToLetter(piece->GetRow()), piece->GetCol()+1);
			//fflush(stdout);
			break;
		}
		if (i == KingAttackingPieces.size()-1)
			ChessAssert();
	}
}

void BlackPlayer::AddToKingAttacking(ChessPiece* piece)
{
	if (piece->IsInKingArray() )
		return;
	piece->SetIsInKingArray(true);
	KingAttackingPieces.push_back(piece);
	//printf("-- Added piece %c to Black King array at location %c%i --", pieceToText(piece), rowToLetter(piece->GetRow()), piece->GetCol()+1);
	//fflush(stdout);
}
void BlackPlayer::RemoveKingAttacking(ChessPiece* piece)
{
	for (int i = 0 ; i < KingAttackingPieces.size(); i++)
	{
		if (KingAttackingPieces[i]->isEqual (piece) )
		{
			KingAttackingPieces.erase(KingAttackingPieces.begin()+i);
			piece->SetIsInKingArray(false);
			//printf("-- Removing piece %c from White King array at location %c%i --", pieceToText(piece), rowToLetter(piece->GetRow()), piece->GetCol()+1);
			//fflush(stdout);
			break;
		}
		if (i == KingAttackingPieces.size()-1)
			ChessAssert();
	}
}

void WhitePlayer::InitWhitePlayer()
{
	assert(remainingPieces.size() == 0);
	remainingPieces.push_back(new Rook(0, 0, true, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Knight(1, 0, true, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Bishop(2, 0, true, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Queen(3, 0, true, ChessBoard::GetInstance()));
	remainingPieces.push_back(new King(4, 0, true, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Bishop(5, 0, true, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Knight(6, 0, true, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Rook(7, 0, true, ChessBoard::GetInstance()));
	
	for (int i = 8; i < 16; i++)
		remainingPieces.push_back(new Pawn(i-8, 1, true, ChessBoard::GetInstance()));
	
	for (int i = 0 ; i < 16; i++)
	{
		remainingPieces[i]->SetIndex(i);
		remainingPieces[i]->SetIsInKingArray(false);
		ChessBoard::GetInstance()->AddPieceNoLineOfSight(remainingPieces[i]);
	}
	
	assert (capturedPieces.size() == 0);
	assert (KingAttackingPieces.size() == 0);
	
	bIsInCheck = false;
	bIsCheckmated = false;
	promotedPiece.reserve(2);
	bHasCastled = false;
	checkingPiece = NULL;
	
	ChessBoard::GetInstance()->SetWhitePlayer(this);
	ChessBoardInstance = ChessBoard::GetInstance();
}

void BlackPlayer::InitBlackPlayer()
{
	assert(remainingPieces.size() == 0);
	remainingPieces.push_back(new Rook(0, 7, false, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Knight(1, 7, false, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Bishop(2, 7, false, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Queen(3, 7, false, ChessBoard::GetInstance()));
	remainingPieces.push_back(new King(4, 7, false, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Bishop(5, 7, false, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Knight(6, 7, false, ChessBoard::GetInstance()));
	remainingPieces.push_back(new Rook(7, 7, false, ChessBoard::GetInstance()));
	
	for (int i = 8; i < 16; i++)
		remainingPieces.push_back(new Pawn(i-8, 6, false, ChessBoard::GetInstance()));
	
	for (int i = 0 ; i < 16; i++)
	{
		remainingPieces[i]->SetIndex(i);
		remainingPieces[i]->SetIsInKingArray(false);
		ChessBoard::GetInstance()->AddPieceNoLineOfSight(remainingPieces[i]);
	}
	
	assert (capturedPieces.size() == 0);
	assert (KingAttackingPieces.size() == 0);
	
	bIsInCheck = false;
	bIsCheckmated = false;
	bHasCastled = false;
	promotedPiece.reserve(2);
	checkingPiece = NULL;
	
	ChessBoard::GetInstance()->SetBlackPlayer(this);
	
	ChessBoardInstance = ChessBoard::GetInstance();
}

WhitePlayer* WhitePlayer::Copy()
{
	WhitePlayer* newPlayer = new WhitePlayer(false);
	
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		ChessPiece* copyPiece = remainingPieces[i]->Copy();
		newPlayer->AddRemainPiece( copyPiece );
		if (remainingPieces[i]->IsInKingArray() )
		{
			newPlayer->AddKingPiece(copyPiece);
		}
	}
	for (int i = 0; i < promotedPiece.size(); i++)
	{
		newPlayer->AddPromoPiece(promotedPiece[i]->Copy());
	}
	for (int i = 0; i < capturedPieces.size(); i++)
	{
		newPlayer->AddCaptPiece(capturedPieces[i]->Copy() );
	}
	assert(KingAttackingPieces.size() == newPlayer->GetNumAttackingKingPieces());
	return newPlayer;
}

BlackPlayer* BlackPlayer::Copy()
{
	BlackPlayer* newPlayer = new BlackPlayer(false);
	
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		ChessPiece* copyPiece = remainingPieces[i]->Copy();
		newPlayer->AddRemainPiece( copyPiece );
		if (remainingPieces[i]->IsInKingArray() )
		{
			newPlayer->AddKingPiece(copyPiece);
		}
	}
	for (int i = 0; i < promotedPiece.size(); i++)
	{
		newPlayer->AddPromoPiece(promotedPiece[i]->Copy());
	}
	for (int i = 0; i < capturedPieces.size(); i++)
	{
		newPlayer->AddCaptPiece(capturedPieces[i]->Copy() );
	}
	assert(KingAttackingPieces.size() == newPlayer->GetNumAttackingKingPieces());
	return newPlayer;
}

void WhitePlayer::AddPiecesToBoard()
{
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		ChessBoardInstance->AddPiece(remainingPieces[i]);
	}
}

void BlackPlayer::AddPiecesToBoard()
{
	for (int i = 0; i < remainingPieces.size(); i++)
	{
		ChessBoardInstance->AddPiece(remainingPieces[i]);
	}
}

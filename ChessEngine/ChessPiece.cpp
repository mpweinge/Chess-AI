/*
 *  ChessPiece.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-26.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ChessPiece.h"
#include "ChessPlayer.h"

static char Row[] = 
{
	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
};

char rowToLetter(int row)
{
	return Row[row];
}

char pieceToText(const ChessPiece* piece)
{
	char T;
	pieceToText(piece, T);
	return T;
}

void pieceToText(const ChessPiece* piece, char& outText)
{
	//char[6] = typeid(*piece).name();
	
	if ( piece  == NULL)
	{
		outText = ' ';
		return;
	}
	outText = piece->Name();
	
}

bool ChessPiece::ValidCheckStateMove(const Move& Move3, const ChessPiece* instance)
{
	Move startingLoc = instance->GetLocation();
	if( (Move3.row > 7) || (Move3.col < 0) || (startingLoc.row > 7) || (startingLoc.col < 0) ||
	   (Move3.row < 0) || (Move3.col > 7) || (startingLoc.row < 0) || (startingLoc.col > 7) )
		return false;
	
	return !checkPuttingInCheck(Move3, startingLoc, instance->isWhite(), instance->GetBoardPointer());
	
	/*if (instance->isWhite())
	{
		if (!(WhitePlayer::GetInstance()->IsInCheck()))
		{
			//if (!(instance->isBlockingCheck() ) )
			{
				
				if ( checkPuttingInCheck(Move3, instance->GetLocation(), instance->isWhite()) )
				{
					printf("Putting you in check sir");
					return false;
				}
				return true;
			}
		}
		
		ChessPiece* checkPiece = WhitePlayer::GetInstance()->GetCheckingPiece();
		//Cant block a knight or a pawn
		char Piece = pieceToText(checkPiece);
		assert(instance);
		char PieceInstance = pieceToText(const_cast<ChessPiece*>(instance));
		if( (Piece == 'N' || Piece == 'P') && (PieceInstance != 'K') )
			return false;
		
		Move checkLoc = checkPiece->GetLocation();
		Move kingLoc = WhitePlayer::GetInstance()->GetKingLocation();
		
		int diffx = kingLoc.row - checkLoc.row;
		int diffy = kingLoc.col - checkLoc.col;
		
		if ( abs(diffx) == abs(diffy) )
		{
			//bishop or queen
			//Draw a path from piece to king
			
			int xsigndiff = diffx > 0 ? 1 : -1;
			int ysigndiff = diffy > 0 ? 1 : -1;
			
			for (int i = 1; i < abs(diffx); i++)
			{
				if( ((checkLoc.row + i*xsigndiff) == Move3.row) && ((checkLoc.col + i*ysigndiff) == Move3.col))
				{
					const_cast<ChessPiece*>(instance)->setBlockingCheck(true);
					//WhitePlayer::GetInstance()->ClearCheckState();
					return true;
				}
			}
			return false;
		}
		
		else if (diffx == 0)
		{
			//Queen or Rook
			int iterdiff;
			diffy > 0 ? iterdiff = 1 : iterdiff = -1;
			for (int i = 1; i<abs(diffy); i++)
			{
				if( ((checkLoc.row) == Move3.row) && ((checkLoc.col + i*iterdiff) == Move3.col))
				{
					const_cast<ChessPiece*>(instance)->setBlockingCheck(true);
					//WhitePlayer::GetInstance()->ClearCheckState();
					return true;
				}
			}
			return false;
		}
		else {
			int iterdiff;
			diffx > 0 ? iterdiff = 1 : iterdiff = -1;
			assert(diffy == 0);
			//Queen or Rook
			for (int i = 1; i<abs(diffx); i++)
			{
				
				if( ((checkLoc.row+i*iterdiff) == Move3.row) && ((checkLoc.col) == Move3.col))
				{
					const_cast<ChessPiece*>(instance)->setBlockingCheck(true);
					//WhitePlayer::GetInstance()->ClearCheckState();
					return true;
				}
			}
			return false;
		}
	}
	else
	{
		if (!(BlackPlayer::GetInstance()->IsInCheck()))
		{
		//	if (!(instance->isBlockingCheck() ) )
			{
				if ( checkPuttingInCheck(Move3, instance->GetLocation(), instance->isWhite()) )
				{
					printf("Putting you in check sir");
					return false;
				}
				return true;
			}
		}
		ChessPiece* checkPiece = BlackPlayer::GetInstance()->GetCheckingPiece();
		//Cant block a knight or a pawn
		char Piece = pieceToText(checkPiece);
		assert(instance);
		char PieceInstance = pieceToText(const_cast<ChessPiece*>(instance));
		if( (Piece == 'N' || Piece == 'P') && (PieceInstance != 'K') )
			return false;
		
		Move checkLoc = checkPiece->GetLocation();
		Move kingLoc = BlackPlayer::GetInstance()->GetKingLocation();
		
		int diffx = kingLoc.row - checkLoc.row;
		int diffy = kingLoc.col - checkLoc.col;
		
		if ( abs(diffx) == abs(diffy) )
		{
			//Bishop or queen
			//Draw a path
			
			int xsigndiff = diffx > 0 ? 1 : -1;
			int ysigndiff = diffy > 0 ? 1 : -1;
			
			for (int i = 1; i < abs(diffx); i++)
			{
				if( ((checkLoc.row + i*xsigndiff) == Move3.row) && ((checkLoc.col + i*ysigndiff) == Move3.col))
				{
					const_cast<ChessPiece*>(instance)->setBlockingCheck(true);
					//BlackPlayer::GetInstance()->ClearCheckState();
					return true;
				}
			}
			return false;
		}
		
		else if (diffx == 0)
		{
			//Queen or Rook
			int iterdiff;
			diffy > 0 ? iterdiff = 1 : iterdiff = -1;
			for (int i = 1; i<abs(diffy); i++)
			{
				if( ((checkLoc.row) == Move3.row) && ((checkLoc.col + i*iterdiff) == Move3.col))
				{
					const_cast<ChessPiece*>(instance)->setBlockingCheck(true);
					//BlackPlayer::GetInstance()->ClearCheckState();
					return true;
				}
			}
			return false;
		}
		else {
			int iterdiff;
			diffx > 0 ? iterdiff = 1 : iterdiff = -1;
			assert(diffy == 0);
			//Queen or Rook
			for (int i = 1; i<abs(diffx); i++)
			{
				
				if( ((checkLoc.row+i*iterdiff) == Move3.row) && ((checkLoc.col) == Move3.col))
				{
					const_cast<ChessPiece*>(instance)->setBlockingCheck(true);
					//BlackPlayer::GetInstance()->ClearCheckState();
					return true;
				}
			}
			return false;
		}
	}*/
}

bool checkPuttingInCheck(const Move& Move3, const Move& startingLoc, bool bWhiteMove, ChessBoard* ChessBoardInstance)
{
	if( (Move3.row > 7) || (Move3.col < 0) || (startingLoc.row > 7) || (startingLoc.col < 0) ||
		(Move3.row < 0) || (Move3.col > 7) || (startingLoc.row < 0) || (startingLoc.col > 7) )
		return false;
	Player* opponentPlayer;
	//Just as a last resort double check, for pins and the such
	if (bWhiteMove )
	{
		opponentPlayer = ChessBoardInstance->GetBlackPlayer();
	}
	else {
		opponentPlayer = ChessBoardInstance->GetWhitePlayer();
	}

	ChessPiece* currPiece = ChessBoardInstance->GetPiece(startingLoc.row,startingLoc.col);
	assert(currPiece->GetBoardPointer() == ChessBoardInstance);
	if (!currPiece)
	{
		ChessBoardInstance->PrintBoard();
		ChessAssert();
	}
	//in case of capture
	ChessPiece* captPiece = ChessBoardInstance->GetPiece(Move3.row, Move3.col);
	
	char currPieceText = pieceToText(currPiece);
	if (currPieceText == 'K')
	{
		ChessBoardInstance->RemovePiece(startingLoc.row, startingLoc.col);
		currPiece->SetRow(Move3.row);
		currPiece->SetCol(Move3.col);
		if (captPiece)
		{
			ChessBoardInstance->RemovePiece(Move3.row, Move3.col);
		}
		ChessBoardInstance->AddPiece(currPiece);
		
		 //Need this here for kings because don't know who is attacking their new square
		//opponentPlayer->ClearAttackingKingPieces();
		Move KingLoc = Move3;
		bool bKingInCheck = false;
		for (int i = 0; i < opponentPlayer->GetNumPiecesLeft(); i++)
		{
			if (captPiece && opponentPlayer->GetPiece(i)->isEqual(captPiece) )
				continue;
			/*if (opponentPlayer->GetPiece(i)->HasLineToKing(KingLoc)
				)
			{
				opponentPlayer->AddToKingAttacking(opponentPlayer->GetPiece(i));
			}*/
			if (opponentPlayer->GetPiece(i)->IsAttackingSquare(Move3.row, Move3.col))
			{
				bKingInCheck = true;
				break;
			}
		}
		ChessBoardInstance->RemovePiece(Move3.row, Move3.col);
		currPiece->SetRow(startingLoc.row);
		currPiece->SetCol(startingLoc.col);
		ChessBoardInstance->AddPiece(currPiece);
		
		if (captPiece)
			ChessBoardInstance->AddPiece(captPiece);
		
		if (bKingInCheck)
		{
			return true;
		}
		return false;
		/*int LoopLimit = opponentPlayer->GetNumAttackingKingPieces();
		for (int i = 0; i < LoopLimit; i++)
		{
			ChessPiece* currAttackingPiece = opponentPlayer->GetAttackingKingPiece(i);
			if (captPiece && currAttackingPiece->isEqual(captPiece) )
				continue;
			if (currAttackingPiece->IsAttackingSquare(Move3.row, Move3.col))
			{
				ChessBoard::GetInstance()->RemovePiece(Move3.row, Move3.col);
				currPiece->SetRow(startingLoc.row);
				currPiece->SetCol(startingLoc.col);
				ChessBoard::GetInstance()->AddPiece(currPiece);
				
				if (captPiece)
					ChessBoard::GetInstance()->AddPiece(captPiece);
				return true;
			}
		}*/
			 
		ChessBoardInstance->RemovePiece(Move3.row, Move3.col);
		currPiece->SetRow(startingLoc.row);
		currPiece->SetCol(startingLoc.col);
		ChessBoardInstance->AddPiece(currPiece);
		
		if (captPiece)
			ChessBoardInstance->AddPiece(captPiece);
		return false;
	}
	
	ChessBoardInstance->RemovePiece(startingLoc.row, startingLoc.col);
	currPiece->SetRow(Move3.row);
	currPiece->SetCol(Move3.col);
	if (captPiece)
	{
		ChessBoardInstance->RemovePiece(Move3.row, Move3.col);
	}
	ChessBoardInstance->AddPiece(currPiece);
	
	/* OLD CODE
	 --M.Weingert April 1, 2012
	 
	 for (int i = 0; i < opponentPlayer->GetNumPiecesLeft(); i++)
	{

		//char name = pieceToText(opponentPlayer->GetPiece(i));
		if( (captPiece != opponentPlayer->GetPiece(i)) )
		{
						
			if ( opponentPlayer->GetPiece(i)->LookForCheck(true) )
			{
				
				ChessBoard::GetInstance()->RemovePiece(Move3.row, Move3.col);
				currPiece->SetRow(startingLoc.row);
				currPiece->SetCol(startingLoc.col);
				ChessBoard::GetInstance()->AddPiece(currPiece);
				
				if (captPiece)
					ChessBoard::GetInstance()->AddPiece(captPiece);
				return true;
			}
			else {
			}

		}
	}*/
	int LoopLimit = opponentPlayer->GetNumAttackingKingPieces();
	for (int i = 0; i < LoopLimit; i++)
	{
		const ChessPiece* currAttackingPiece = opponentPlayer->GetAttackingKingPiece(i);
		if (captPiece && currAttackingPiece->isEqual(captPiece) )
			continue;
		if ( currAttackingPiece->LookForCheck(true) )
		{
			ChessBoardInstance->RemovePiece(Move3.row, Move3.col);
			currPiece->SetRow(startingLoc.row);
			currPiece->SetCol(startingLoc.col);
			ChessBoardInstance->AddPiece(currPiece);
			
			if (captPiece)
				ChessBoardInstance->AddPiece(captPiece);
			return true;
		}
	}
	
	ChessBoardInstance->RemovePiece(Move3.row, Move3.col);
	currPiece->SetRow(startingLoc.row);
	currPiece->SetCol(startingLoc.col);
	ChessBoardInstance->AddPiece(currPiece);
	
	if (captPiece)
		ChessBoardInstance->AddPiece(captPiece);
	return false;
	
}
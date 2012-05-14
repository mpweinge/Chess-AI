/*
 *  ChessBoard.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-20.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ChessBoard.h"
#include <iostream>

static ChessBoard* m_Instance;

ChessBoard::ChessBoard()
{
	bWhiteMove = true;
	bBlackMove = false;
	
	_WhitePlayer = NULL;
	_BlackPlayer = NULL;
}

ChessBoard::~ChessBoard()
{
	assert(!_WhitePlayer);
	assert(!_BlackPlayer);
	assert(!_ChessLogInstance);
}
void ChessBoard::Erase()
{
	_WhitePlayer->Erase();
	delete (_WhitePlayer);
	_WhitePlayer = NULL;
	_BlackPlayer->Erase();
	delete (_BlackPlayer);
	_BlackPlayer = NULL;
	_ChessLogInstance->Erase();
	delete (_ChessLogInstance);
	_ChessLogInstance = NULL;
}

ChessBoard* ChessBoard::Copy()
{
	ChessBoard* ret = new ChessBoard();
	
#ifdef PRINT_MEMORY
	printf("Created chess board at:%i\n", ret);
	fflush(stdout);
#endif
	
	ret->SetWhitePlayer( WhitePlayer::GetInstance()->Copy() );
	ret->SetBlackPlayer( BlackPlayer::GetInstance()->Copy() );
	
	
	
	ChessLog* LogInstance = ChessLog::GetInstance()->Copy();
	
	ret->SetChessLog(LogInstance);
	
	LogInstance->SetChessBoard(ret);
	
	ret->GetWhitePlayer()->SetPieceBoardPointer(ret);
	
	//printf("Thread white player");
	//fflush(stdout);
	//ret->GetWhitePlayer()->PrintPlayer();
	
	ret->GetBlackPlayer()->SetPieceBoardPointer(ret);
	
	//ret->GetBlackPlayer()->PrintPlayer();
	
	ret->ClearBoard();
	
	ret->GetWhitePlayer()->AddPiecesToBoard();
	ret->GetBlackPlayer()->AddPiecesToBoard();
	
	//ret->PrintBoard();
	
	ret->bWhiteMove = bWhiteMove;
	
	fflush(stdout);
	
	return ret;
}

void ChessBoard::Copy(const ChessBoard* ReferenceBoard)
{
	for (int i = 0; i< 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Board[i][j] = ReferenceBoard->GetPiece(i, j);
		}
	}
}

bool ChessBoard::IsEqual(ChessBoard const * const ReferenceBoard) const
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Board[i][j])
			{
				const ChessPiece* testPiece = ReferenceBoard->GetPiece(i,j);
				if (testPiece)
				{
					if (Board[i][j]->isEqual(testPiece) )
					{
					}
					else 
					{
						return false;
					}

				}
				else
					return false;
			}
			else {
				const ChessPiece* testPiece = ReferenceBoard->GetPiece(i,j);
				if (testPiece)
					return false;
			}

		}
	}
	return true;
}

void ChessBoard::ResetBoard()
{
	bWhiteMove = true;
	bBlackMove = false;
	//Just to make sure
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//if (Board[i][j])
			{
				Board[i][j] = NULL;
			}
		}
	}
	_WhitePlayer->ResetPlayer(); 
	_BlackPlayer->ResetPlayer();
	_ChessLogInstance->ResetLog();
}

ChessBoard* ChessBoard::GetInstance()
{
	if(m_Instance == NULL)
	{
		m_Instance = new ChessBoard();
		m_Instance->SetChessLog(ChessLog::GetInstance());
	}
	return m_Instance;
}

void ChessBoard::PrintBoard()
{
	char Text;
	for (int i = 7 ; i >= 0; i--)
	{
		printf("\n -----------------------------------------\n%i|", i+1);
		for (int j = 0; j < 8; j++)
		{
			pieceToText(Board[j][i], Text);
			if (Board[j][i])
				printf(" %c%c |", Text, Board[j][i]->isWhite() ? 'w' : 'b');
			else {
				printf(" %c  |", Text);
			}

		}
	}
	printf("\n -----------------------------------------\n");
	printf("   a    b    c    d    e    f    g    h\n");
}

void ChessBoard::PutPlayerInCheck(bool isWhite, const ChessPiece* checkingPiece)
{
	if (isWhite)
	{
		_BlackPlayer->EnterCheckState();
		if (_BlackPlayer->CheckForCheckMate() )
		{
			printf("Checkmate");
		}
	}
	else {
		_WhitePlayer->EnterCheckState();
		if (_WhitePlayer->CheckForCheckMate() )
		{
			printf("Checkmate");
		}
	}

}

int ChessBoard::Castle(ChessPiece* _King, ChessPiece* _Rook, bool bKingSide, int col)
{
	
	ChessMove* castleMove = new ChessMove(bKingSide, this);
	
#ifdef PRINT_MEMORY_MOVES
	printf("Created move at: %i\n", castleMove);
	fflush(stdout);
#endif
	
	ChessSquare kingLoc;
	_King->GetLocation(kingLoc);
	
	ChessSquare rookLoc;
	_Rook->GetLocation(rookLoc);
	
	if (bKingSide)
	{
		Board[6][col] = _King;
		_King->SetCol(col);
		_King->SetRow(6);
		Board[5][col] = _Rook;
		_Rook->SetCol(col);
		_Rook->SetRow(5);
		Board[7][col] = NULL;
		Board[4][col] = NULL;
		(dynamic_cast<King*>(_King))->SetHasMoved(true);
		(dynamic_cast<King*>(_King))->SetHasCastled(true);
		(dynamic_cast<Rook*>(_Rook))->SetHasMoved(true);
	}
	else 
	{
		Board[2][col] = _King;
		_King->SetCol(col);
		_King->SetRow(2);
		Board[3][col] = _Rook;
		_Rook->SetCol(col);
		_Rook->SetRow(3);
		Board[0][col] = NULL;
		Board[4][col] = NULL;
		(dynamic_cast<King*>(_King))->SetHasMoved(true);
		(dynamic_cast<King*>(_King))->SetHasCastled(true);
		(dynamic_cast<Rook*>(_Rook))->SetHasMoved(true);
	}

	
	//Need to have a custom move
	castleMove->makeMove();
	
	
	if (bWhiteMove)
		_WhitePlayer->Castle();
	else {
		_BlackPlayer->Castle();
	}

		
#ifndef SUPPRESS_OUTPUT
	_ChessLogInstance->printLog();
	PrintBoard();
#endif
	
	ResolveCheck(bWhiteMove);	
	
	bWhiteMove = !bWhiteMove;
	bBlackMove = !bBlackMove;
	
	//bWhiteMove
	return 0;
}

int ChessBoard::MovePiece(ChessPiece* piece, int row, int col)
{
	
	if (pieceToText(piece) == 'K') 
		(dynamic_cast<King*>(piece))->SetHasMoved(true);
	else if (pieceToText(piece) == 'R')
		(dynamic_cast<Rook*>(piece))->SetHasMoved(true);
	assert(piece->GetBoardPointer() == this);
	ChessSquare pieceLoc;
	piece->GetLocation(pieceLoc);
	if (pieceLoc.row == row)
	{
		if(pieceLoc.col == col)
		{
			printf("Thread stuff");
			_WhitePlayer->PrintPlayer();
			this->PrintBoard();
			
			printf("Instance stuff");
			WhitePlayer::GetInstance()->PrintPlayer();
			ChessBoard::GetInstance()->PrintBoard();
			ChessAssert();
		}
	}
	
	ChessMove* newMove = new ChessMove(piece, pieceLoc.row, pieceLoc.col,row, col, false, this);
	
#ifdef PRINT_MEMORY_MOVES
	printf("Created move at: %i\n", newMove);
	fflush(stdout);
#endif
	
	Board[row][col] = piece;
	
	
	Board[pieceLoc.row][pieceLoc.col] = NULL;
	piece->SetCol(col);
	piece->SetRow(row);
	newMove->makeMove();
	
	
#ifndef SUPPRESS_OUTPUT
	_ChessLogInstance->printLog();
	PrintBoard();
#endif
	
	ResolveCheck(bWhiteMove);
	
	if (bWhiteMove)
	{
		if (_BlackPlayer->IsInCheck() )
			newMove->SetCheckMove(true);
		else {
			newMove->SetCheckMove(false);
		}

	}
	else {
		if (_WhitePlayer->IsInCheck() )
			newMove->SetCheckMove(true);
		else {
			newMove->SetCheckMove(false);
		}
	}
	
	
	

	bWhiteMove = !bWhiteMove;
	bBlackMove = !bBlackMove;
	
	return 0;
}

void ChessBoard::ResolveCheck(bool bWhiteMove)
{
	if (bWhiteMove)
	{
		if (_WhitePlayer->IsInCheck() )
			_WhitePlayer->ClearCheckState();
		
		Player* currPlayer = _WhitePlayer;
		Player* opponentPlayer = _BlackPlayer;
		
		for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
		{
			if (CheckForCheck(currPlayer->GetPiece(i)) )
			{
				opponentPlayer->EnterCheckState();
				//printf("Checking Piece: %c", pieceToText(currPlayer->GetPiece(i)));
				//fflush(stdout);
				currPlayer->AddToKingAttacking(currPlayer->GetPiece(i));
				break;
			}
		}
	}
	else {
		if (_BlackPlayer->IsInCheck() )
			_BlackPlayer->ClearCheckState();
		
		Player* currPlayer = _BlackPlayer;
		Player* opponentPlayer = _WhitePlayer;
		
		
		for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
		{
			if (CheckForCheck(currPlayer->GetPiece(i)) )
			{
				opponentPlayer->EnterCheckState();
				//printf("Checking Piece: %c", pieceToText(currPlayer->GetPiece(i)));
				//fflush(stdout);
				currPlayer->AddToKingAttacking(currPlayer->GetPiece(i));
				break;
			}
		}
	}
}

void ChessBoard::UndoMove()
{
	Player* currPlayer;
	Player* opponentPlayer;
	bool bKingMoved = false;
	int col;
	if (bWhiteMove)
	{
		currPlayer = _BlackPlayer;
		opponentPlayer = _WhitePlayer;
		col = 7;
	}
	else {
		currPlayer = _WhitePlayer;
		opponentPlayer = _BlackPlayer;
		col = 0;
	}

	ChessMove* moveToUndo = _ChessLogInstance->getLastMove();
	if ( moveToUndo->isCastle() )
	{
		if (moveToUndo->isKingSideCastle())
		{
			ChessPiece* king = GetPiece(6, col); 
			if(pieceToText(king) != 'K')
				ChessAssert();
			ChessPiece* rook = GetPiece(5, col);
			if(pieceToText(rook) != 'R')
				ChessAssert();
			rook->SetRow(7);
			king->SetRow(4);
			
			Board[4][col] = king;
			Board[6][col] = NULL;
			Board[5][col] = NULL;
			Board[7][col] = rook;
			
			(dynamic_cast<King*>(king))->SetHasMoved(false);
			(dynamic_cast<King*>(king))->SetHasCastled(false);
			(dynamic_cast<Rook*>(rook))->SetHasMoved(false);
			ResolveLineOfSight(rook);
		}
		else {
			ChessPiece* king = GetPiece(2, col); 
			if(pieceToText(king) != 'K')
				ChessAssert();
			ChessPiece* rook = GetPiece(3, col);
			if(pieceToText(rook) != 'R')
				ChessAssert();
			rook->SetRow(0);
			king->SetRow(4);
			
			Board[4][col] = king;
			Board[3][col] = NULL;
			Board[2][col] = NULL;
			Board[0][col] = rook;
			(dynamic_cast<King*>(king))->SetHasMoved(false);
			(dynamic_cast<King*>(king))->SetHasCastled(false);
			(dynamic_cast<Rook*>(rook))->SetHasMoved(false);
			ResolveLineOfSight(rook);
		}
		bKingMoved = true;

	}
	//NEED TO CHECK FOR PAWN PROMOTION HERE
	else if ( moveToUndo->isPromotion() )
	{
		ChessPiece* newPromoPiece = GetPiece(moveToUndo->getRow(), moveToUndo->getCol());
		ChessSquare PromotionSquare = ChessSquare(moveToUndo->getRow(), moveToUndo->getCol());
		currPlayer->UndoPawnPromotion(moveToUndo->getStartingRow(), PromotionSquare, newPromoPiece);
		Board[moveToUndo->getRow()][moveToUndo->getCol()] = NULL;
		if (PromotionSquare.row != moveToUndo->getStartingRow())
		{
			ChessPiece* capturedPiece = opponentPlayer->FindCapturedPiece(moveToUndo->getRow(), moveToUndo->getCol(), moveToUndo->getCapturedPiece());
			if(!capturedPiece)
				ChessAssert();
			ChessSquare capturedPieceLoc = capturedPiece->GetLocation();
			if (Board[capturedPieceLoc.row][capturedPieceLoc.col] != NULL )
				ChessAssert();
			opponentPlayer->RebirthCapturedPiece(capturedPiece);
			AddPiece(capturedPiece);
		}
		//printf("Undid pawn promotion below: \n");
		//ChessBoard::GetInstance()->PrintBoard();
	}
	else if ( moveToUndo->IsEnPassant() )
	{
		ChessPiece* capturedPiece = opponentPlayer->FindCapturedPiece(moveToUndo->getRow(), moveToUndo->getStartingCol(), moveToUndo->getCapturedPiece());
		if(!capturedPiece)
			ChessAssert();
		ChessPiece* movedPiece = GetPiece(moveToUndo->getRow(), moveToUndo->getCol() );
		
		Board[moveToUndo->getRow()][moveToUndo->getCol()] = NULL;
		Board[moveToUndo->getRow()][moveToUndo->getStartingCol()] = capturedPiece;
		Board[moveToUndo->getStartingRow()][moveToUndo->getStartingCol()] = movedPiece;
		
		movedPiece->SetRow(moveToUndo->getStartingRow());
		movedPiece->SetCol(moveToUndo->getStartingCol());
		
		//AddPiece(capturedPiece);
		ResolveLineOfSight(capturedPiece);
		opponentPlayer->RebirthCapturedPiece(capturedPiece);
		ResolveLineOfSight(movedPiece);
	}
	else if ( moveToUndo->isCapture() )
	{
		ChessPiece* capturedPiece = moveToUndo->getCapturedPiece();
		if (!capturedPiece)
			ChessAssert();
		ChessPiece* movedPiece = GetPiece(moveToUndo->getRow(), moveToUndo->getCol() );
		
		Board[moveToUndo->getRow()][moveToUndo->getCol()] = capturedPiece;
		Board[moveToUndo->getStartingRow()][moveToUndo->getStartingCol()] = movedPiece;
		
		movedPiece->SetRow(moveToUndo->getStartingRow());
		movedPiece->SetCol(moveToUndo->getStartingCol());
		
		if (pieceToText(movedPiece) == 'K')
		{
			(dynamic_cast<King*>(movedPiece))->SetHasMoved(false);
			bKingMoved = true;
		}
		else if (pieceToText(movedPiece) == 'R')
			(dynamic_cast<Rook*>(movedPiece))->SetHasMoved(false);
		
		ResolveLineOfSight(capturedPiece);
		opponentPlayer->RebirthCapturedPiece(capturedPiece);
		ResolveLineOfSight(movedPiece);
	}
	else
	{
		ChessPiece* movedPiece = GetPiece(moveToUndo->getRow(), moveToUndo->getCol() );
		Board[moveToUndo->getRow()][moveToUndo->getCol()] = NULL;
		Board[moveToUndo->getStartingRow()][moveToUndo->getStartingCol()] = movedPiece;
		movedPiece->SetRow(moveToUndo->getStartingRow());
		movedPiece->SetCol(moveToUndo->getStartingCol());
		
		if (pieceToText(movedPiece) == 'K')
		{
			(dynamic_cast<King*>(movedPiece))->SetHasMoved(false);
			bKingMoved = true;
		}
		else if (pieceToText(movedPiece) == 'R')
			(dynamic_cast<Rook*>(movedPiece))->SetHasMoved(false);
		ResolveLineOfSight(movedPiece);
	}
	
	if (bKingMoved)
	{
		currPlayer->ClearCheckState();
		opponentPlayer->ClearAttackingKingPieces();
		
		ChessSquare kingLoc = currPlayer->GetKingLocation();
		for (int i = 0; i < opponentPlayer->GetNumPiecesLeft(); i++)
		{
			if (CheckForCheck(opponentPlayer->GetPiece(i)) )
			{
				currPlayer->EnterCheckState();
				//printf("Checking Piece: %c", pieceToText(currPlayer->GetAttackingKingPiece(i)));
				//fflush(stdout);
				opponentPlayer->AddToKingAttacking(opponentPlayer->GetPiece(i));
			}
			else if (opponentPlayer->GetPiece(i)->HasLineToKing(kingLoc))
				opponentPlayer->AddToKingAttacking(opponentPlayer->GetPiece(i));
		}
	}
	else 
	{
		for (int i = 0; i < opponentPlayer->GetNumAttackingKingPieces(); i++)
		{
			if (CheckForCheck(opponentPlayer->GetAttackingKingPiece(i)) )
			{
				currPlayer->EnterCheckState();
				//printf("Checking Piece: %c", pieceToText(opponentPlayer->GetAttackingKingPiece(i)));
				//fflush(stdout);
				break;
			}
			if (i == (opponentPlayer->GetNumAttackingKingPieces() - 1))
				currPlayer->ClearCheckState();
		}
	}

	
	for (int i = 0; i < currPlayer->GetNumAttackingKingPieces(); i++)
	{
		if (CheckForCheck(currPlayer->GetAttackingKingPiece(i)) )
		{
			opponentPlayer->EnterCheckState();
			//printf("Checking Piece: %c", pieceToText(currPlayer->GetAttackingKingPiece(i)));
			//fflush(stdout);
			break;
		}
		if (i == (currPlayer->GetNumAttackingKingPieces() - 1))
			opponentPlayer->ClearCheckState();
	}
	
	_ChessLogInstance->removeLastItemFromLog();
	
	bWhiteMove = !bWhiteMove;
	bBlackMove = !bBlackMove;
	//REMOVE BELOW WHEN WORKING
	ChessSquare currPlayerKing = currPlayer->GetKingLocation();
	/*for (int i = 0; i < opponentPlayer->GetNumPiecesLeft(); i++)
	{
		if (opponentPlayer->GetPiece(i)->HasLineToKing(currPlayerKing) )
		{
			if (!(opponentPlayer->GetPiece(i)->IsInKingArray()))
			{
				ChessPiece* forTestPiece = opponentPlayer->GetPiece(i);
				forTestPiece->HasLineToKing(currPlayerKing);
				ChessAssert();
			}
		}
		else {
			if (opponentPlayer->GetPiece(i)->IsInKingArray())
				ChessAssert();
		}

	}
	ChessSquare opponentPlayerKing = opponentPlayer->GetKingLocation();
	for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
	{
		if (currPlayer->GetPiece(i)->HasLineToKing(opponentPlayerKing) )
		{
			if (!(currPlayer->GetPiece(i)->IsInKingArray()))
				ChessAssert();
		}
		else {
			if (currPlayer->GetPiece(i)->IsInKingArray())
				ChessAssert();
		}
		
	}*/
}

ChessPiece* ChessBoard::GetPiece(int row, int col) const
{
	if( (row < 0) || (col < 0) || (row > 7) || (col > 7) )
		return NULL;
	if( Board[row][col]==NULL)
		return NULL;
	else 
		return Board[row][col];
}

bool ChessBoard::bHasPiece(int row, int col)
{
	if( (row < 0) || (col < 0) || (row > 7) || (col > 7) )
		return true; 
	if ( Board[row][col]==NULL )
		return false;
	else {
		return true;
	}
}

void ChessBoard::MakeCapture(ChessPiece* piece, const ChessSquare& MoveTo)
{
	ChessPiece* captPiece = GetPiece(MoveTo.row, MoveTo.col);
	if (!captPiece)
		ChessAssert();
	
	if (pieceToText(piece) == 'K')
		(dynamic_cast<King*>(piece))->SetHasMoved(true);
	else if (pieceToText(piece) == 'R')
		(dynamic_cast<Rook*>(piece))->SetHasMoved(true);
	
	ChessSquare pieceLoc;
	piece->GetLocation(pieceLoc);
	
	ChessMove* newMove = new ChessMove(piece, pieceLoc.row, pieceLoc.col, MoveTo.row, MoveTo.col, true, this);
	
#ifdef PRINT_MEMORY_MOVES
	printf("Created move at: %i\n", newMove);
	fflush(stdout);
#endif
	
	bWhiteMove ? _BlackPlayer->RemovePiece(captPiece) :
	_WhitePlayer->RemovePiece(captPiece);
	
	Board[MoveTo.row][MoveTo.col] = NULL;
	Board[MoveTo.row][MoveTo.col] = piece;
	
	Board[pieceLoc.row][pieceLoc.col] = NULL;

	piece->SetCol(MoveTo.col);
	piece->SetRow(MoveTo.row);
	newMove->makeMove();
		
#ifndef SUPPRESS_OUTPUT
	_ChessLogInstance->printLog();
	PrintBoard();
#endif
	
	ResolveCheck(bWhiteMove);
	
	if (bWhiteMove)
	{
		if (_BlackPlayer->IsInCheck() )
			newMove->SetCheckMove(true);
		else {
			newMove->SetCheckMove(false);
		}

	}
	else {
		if (_WhitePlayer->IsInCheck() )
			newMove->SetCheckMove(true);
		else
			newMove->SetCheckMove(false);
	}
	
	
	
	bWhiteMove = !bWhiteMove;
	bBlackMove = !bBlackMove;
}

void ChessBoard::EnPassantCapture(ChessPiece* piece, const ChessSquare& MoveTo, const ChessSquare& CaptPieceLoc)
{
	ChessPiece* captPiece = GetPiece(CaptPieceLoc.row, CaptPieceLoc.col);
	if(!captPiece)
		ChessAssert();
	
	ChessSquare pieceLoc;
	piece->GetLocation(pieceLoc);
	
	bool bOpponentInCheck = false;
	
	if (bWhiteMove)
	{
		if (_BlackPlayer->IsInCheck() )
			bOpponentInCheck = true;
	}
	else {
		if (_WhitePlayer->IsInCheck() )
			bOpponentInCheck = true;
	}
	
	ChessMove* newMove = new ChessMove(piece, pieceLoc.row, pieceLoc.col, MoveTo.row, MoveTo.col, true, bOpponentInCheck, this);
	
#ifdef PRINT_MEMORY_MOVES
	printf("Created move at: %i\n", newMove);
	fflush(stdout);
#endif
	
	bWhiteMove ? _BlackPlayer->RemovePiece(GetPiece(CaptPieceLoc.row, CaptPieceLoc.col)) :
	_WhitePlayer->RemovePiece(GetPiece(CaptPieceLoc.row, CaptPieceLoc.col));
	
	Board[MoveTo.row][MoveTo.col] = NULL;
	Board[MoveTo.row][MoveTo.col] = piece;
	
	Board[pieceLoc.row][pieceLoc.col] = NULL;
	Board[CaptPieceLoc.row][CaptPieceLoc.col] = NULL;
	
	piece->SetCol(MoveTo.col);
	piece->SetRow(MoveTo.row);
	newMove->makeMove();
	
#ifdef SUPPRESS_OUTPUT
	_ChessLogInstance->printLog();
	PrintBoard();
#endif
	
	ResolveCheck(bWhiteMove);
	
	if (bWhiteMove)
	{
		if (_BlackPlayer->IsInCheck() )
			newMove->SetCheckMove(true);
		else {
			newMove->SetCheckMove(false);
		}
		
	}
	else {
		if (_WhitePlayer->IsInCheck() )
			newMove->SetCheckMove(true);
		else {
			newMove->SetCheckMove(false);
		}
	}
	
	bWhiteMove = !bWhiteMove;
	bBlackMove = !bBlackMove;
}

bool ChessBoard::CheckForCheck(const ChessPiece* piece)
{
	//assert(0);
	return piece->LookForCheck(true);
}

ChessSquare ChessBoard::GetKingLocation(bool isWhite)
{
	if (!(isWhite))
	{
		return _WhitePlayer->GetKingLocation();
	}
	else {
		return _BlackPlayer->GetKingLocation();
	}

}

int ChessBoard::AddPieceNoLineOfSight(ChessPiece* piece)
{
	ChessSquare currSquare;
	
	piece->GetLocation(currSquare);
	
	if (Board[currSquare.row][currSquare.col] == NULL)
	{
		Board[currSquare.row][currSquare.col] = piece;
		return 0;
	}
	else 
	{
		return -1;
	}
}

int ChessBoard::AddPiece(ChessPiece* piece)
{
	ChessSquare currSquare;
	
	piece->GetLocation(currSquare);
	
	if (Board[currSquare.row][currSquare.col] == NULL)
	{
		Board[currSquare.row][currSquare.col] = piece;
		ResolveLineOfSight(piece);
		return 0;
	}
	else 
	{
		ChessAssert();
		return -1;
	}
}

void ChessBoard::PromotePawn(Pawn* pawn, const ChessSquare* MoveTo, char PromotionPiece)
{
	ChessSquare pieceLoc;
	pawn->GetLocation(pieceLoc);
	
	ChessMove* newMove = NULL;
	if ((MoveTo->row) != (pawn->GetLocation().row ))
	{
		if (!GetPiece(MoveTo->row, MoveTo->col))
			ChessAssert();
			
		bWhiteMove ? _BlackPlayer->RemovePiece(GetPiece(MoveTo->row, MoveTo->col)) :
		_WhitePlayer->RemovePiece(GetPiece(MoveTo->row, MoveTo->col));
		
		//NEED TO CHANGE THIS
		newMove = new ChessMove(pawn, pieceLoc.row, pieceLoc.col,MoveTo->row, MoveTo->col, true, this);
		
#ifdef PRINT_MEMORY_MOVES
		printf("Created move at: %i\n", newMove);
		fflush(stdout);
#endif
		
		Board[MoveTo->row][MoveTo->col] = NULL;
		
	}
	else 
	{
		newMove = new ChessMove(pawn, pieceLoc.row, pieceLoc.col,MoveTo->row, MoveTo->col, false, this);
		
#ifdef PRINT_MEMORY_MOVES
		printf("Created move at: %i\n", newMove);
		fflush(stdout);
#endif
	}
	
	newMove->SetPromotion(true);
	
	Board[pieceLoc.row][pieceLoc.col] = NULL;
	
	if(pieceToText(pawn) != 'P')
		ChessAssert();
	
	ChessPiece* newPiece;
	Player* currPlayer;
	
	ChessSquare currLoc = pawn->GetLocation();
	
	if ( pawn->isWhite() )
	{
		if(pawn->GetLocation().col != 6)
			ChessAssert();
		currPlayer = _WhitePlayer;
	}
	else {
		if(pawn->GetLocation().col != 1)
			ChessAssert();
		currPlayer = _BlackPlayer;
	}
	
	//char* c = new char;
	
	//bool bValidChar;
	
	//bValidChar = false;
	//
	//printf("What piece would you like to turn the pawn into? Type N, R, B, or Q");
	
	//while (! (bValidChar) )
	{
		//std::cin >> c;
		
		switch(PromotionPiece)
		{
			case 'N':
				newPiece = new Knight(MoveTo->row, MoveTo->col, pawn->isWhite(), this);
				newPiece->SetIsInKingArray(false);
				break;
			case 'n':
				newPiece = new Knight(MoveTo->row, MoveTo->col, pawn->isWhite(), this);
				newPiece->SetIsInKingArray(false);
				break;
			case 'R':
				newPiece = new Rook(MoveTo->row, MoveTo->col, pawn->isWhite(), this);
				newPiece->SetIsInKingArray(false);
				break;
			case 'r':
				newPiece = new Rook(MoveTo->row, MoveTo->col, pawn->isWhite(), this);
				newPiece->SetIsInKingArray(false);
				break;
			case 'B':
				newPiece = new Bishop(MoveTo->row, MoveTo->col, pawn->isWhite(), this);
				newPiece->SetIsInKingArray(false);
				break;
			case 'b':
				newPiece = new Bishop(MoveTo->row, MoveTo->col, pawn->isWhite(), this);
				newPiece->SetIsInKingArray(false);
				break;
			case 'Q':
				newPiece = new Queen(MoveTo->row, MoveTo->col, pawn->isWhite(), this);
				newPiece->SetIsInKingArray(false);
				break;
			case 'q':
				newPiece = new Queen(MoveTo->row, MoveTo->col, pawn->isWhite(), this);
				newPiece->SetIsInKingArray(false);
				//bValidChar = true;
				break;
			default:
				ChessAssert();
				printf("Please enter in N, R, B, or Q");
				break;
		}
	}
	
	newMove->makeMove();
	newMove->SetPromotionChar(PromotionPiece);
	
	if (bWhiteMove)
	{
		if (_WhitePlayer->IsInCheck() )
			_WhitePlayer->ClearCheckState();
	}
	else {
		if (_BlackPlayer->IsInCheck() )
			_BlackPlayer->ClearCheckState();
	}
	
	ResolveCheck(bWhiteMove);
	
	if (bWhiteMove)
	{
		if (_BlackPlayer->IsInCheck() )
			newMove->SetCheckMove(true);
		else {
			newMove->SetCheckMove(false);
		}
		
	}
	else {
		if (_WhitePlayer->IsInCheck() )
			newMove->SetCheckMove(true);
		else {
			newMove->SetCheckMove(false);
		}
	}
	
	currPlayer->PawnPromotion(pawn, newPiece);
	
	bWhiteMove = !bWhiteMove;
	bBlackMove = !bBlackMove;
	
	
#ifndef SUPPRESS_OUTPUT
	_ChessLogInstance->printLog();
	PrintBoard();
#endif
	ResolveLineOfSight(newPiece);
	//printf("Promotion Below: \n");
	//ChessBoard::GetInstance()->PrintBoard();
}

int ChessBoard::CalculateScore(const Chromosome* calcChromo)
{
	int score = 0;
	BlackPlayer* blackPlayer = _BlackPlayer;
	WhitePlayer* whitePlayer = _WhitePlayer;
	
	std::vector<const ChessPiece*> WhiteRooks;
	WhiteRooks.reserve(10);
	
	std::vector<const ChessPiece*> BlackRooks;
	BlackRooks.reserve(10);
	
	std::vector<const ChessPiece*> WhiteKnights;
	WhiteKnights.reserve(10);
	
	std::vector<const ChessPiece*> BlackKnights;
	BlackKnights.reserve(10);
	
	std::vector<const ChessPiece*> WhiteBishops;
	WhiteBishops.reserve(10);
	
	std::vector<const ChessPiece*> BlackBishops;
	BlackBishops.reserve(10);
	
	std::vector<const ChessPiece*> WhitePawns[8];
	
	std::vector<const ChessPiece*> BlackPawns[8];
	
	ChessPiece* WhiteKing;
	ChessPiece* BlackKing;
	
	ChessSquare WhiteKingLoc = GetKingLocation(false);
	ChessSquare BlackKingLoc = GetKingLocation(true);
	
	for (int i = 0; i < whitePlayer->GetNumPiecesLeft(); i++)
	{
		ChessPiece* currPiece = whitePlayer->GetPiece(i);
		switch (pieceToText(currPiece) )
		{
			case 'R':
				WhiteRooks.push_back(currPiece);
				if (currPiece->IsAttackingSquare(BlackKingLoc.row, BlackKingLoc.col))
					score += currPiece->GetMobilityScore(calcChromo, false);
				else
					score += currPiece->GetMobilityScore(calcChromo, true);
				break;
			case 'B':
				WhiteBishops.push_back(currPiece);
				if (currPiece->IsAttackingSquare(BlackKingLoc.row, BlackKingLoc.col))
					score += currPiece->GetMobilityScore(calcChromo, false);
				else
					score += currPiece->GetMobilityScore(calcChromo, true);
				break;
			case 'P':
				WhitePawns[currPiece->GetRow()].push_back(currPiece);
				break;
			case 'K':
				WhiteKing = currPiece;
				break;
			case 'N':
				WhiteKnights.push_back(currPiece);
				if (currPiece->IsAttackingSquare(BlackKingLoc.row, BlackKingLoc.col))
					score += currPiece->GetMobilityScore(calcChromo, false);
				else
					score += currPiece->GetMobilityScore(calcChromo, true);
				break;
			default:
				//Don't care about queens
				if (currPiece->IsAttackingSquare(BlackKingLoc.row, BlackKingLoc.col))
					score += currPiece->GetMobilityScore(calcChromo, false);
				else
					score += currPiece->GetMobilityScore(calcChromo, true);
				break;
		}
	}
	
	for (int i = 0; i < blackPlayer->GetNumPiecesLeft(); i++)
	{
		ChessPiece* currPiece = blackPlayer->GetPiece(i);
		switch (pieceToText(currPiece) )
		{
			case 'R':
				BlackRooks.push_back(currPiece);
				if (currPiece->IsAttackingSquare(WhiteKingLoc.row, WhiteKingLoc.col))
					score -= currPiece->GetMobilityScore(calcChromo, false);
				else
					score -= currPiece->GetMobilityScore(calcChromo, true);
				break;
			case 'B':
				BlackBishops.push_back(currPiece);
				if (currPiece->IsAttackingSquare(WhiteKingLoc.row, WhiteKingLoc.col))
					score -= currPiece->GetMobilityScore(calcChromo, false);
				else
					score -= currPiece->GetMobilityScore(calcChromo, true);
				break;
			case 'P':
				BlackPawns[currPiece->GetRow()].push_back(currPiece);
				break;
			case 'K':
				BlackKing = currPiece;
				break;
			case 'N':
				BlackKnights.push_back(currPiece);
				if (currPiece->IsAttackingSquare(WhiteKingLoc.row, WhiteKingLoc.col))
					score -= currPiece->GetMobilityScore(calcChromo, false);
				else
					score -= currPiece->GetMobilityScore(calcChromo, true);
				break;
			default:
				//Don't care about queens
				if (currPiece->IsAttackingSquare(WhiteKingLoc.row, WhiteKingLoc.col))
					score -= currPiece->GetMobilityScore(calcChromo, false);
				else
					score -= currPiece->GetMobilityScore(calcChromo, true);
				break;
		}
	}

	for (int i = blackPlayer->GetNumPiecesLeft() - 1; i >= 0; i--)
	{
		ChessPiece * currPiece = blackPlayer->GetPiece(i);
		score -= currPiece->GetScore();
		//currPiece = NULL;
		
		/*{
			if (currPiece->IsAttackingSquare(WhiteKing->GetRow(), WhiteKing->GetCol() ) )
			{
				//Player shouldn't capture king	
				score -= currPiece->GetMobilityScore(calcChromo, true);
			}
			else {
				score -= currPiece->GetMobilityScore(calcChromo, false);
			}

		}
		score -= currPiece->GetDefendingScore(calcChromo);
		score += currPiece->GetPieceUnderAttackScore(calcChromo);*/
	}
	
	for (int i = whitePlayer->GetNumPiecesLeft() -1 ; i >= 0; i--)
	{
		ChessPiece * currPiece = whitePlayer->GetPiece(i);
		score += currPiece->GetScore();
		//currPiece = NULL;
		
		/*{
			if (currPiece->IsAttackingSquare(BlackKing->GetRow(), BlackKing->GetCol() ) )
			{
				//Player shouldn't capture king
				score += currPiece->GetMobilityScore(calcChromo, true);
			}
			else {
				score += currPiece->GetMobilityScore(calcChromo, false);
			}
			
			
		}

		score += currPiece->GetDefendingScore(calcChromo);
		score -= currPiece->GetPieceUnderAttackScore(calcChromo);*/
	}
	
	if (WhiteBishops.size() > 1)
	{
		score += calcChromo->GetWeighting(BishopPair);
	}
	if (BlackBishops.size() > 1)
	{
		score -= calcChromo->GetWeighting(BishopPair);
	}
	
	for (int i = 0; i < WhiteRooks.size(); i++)
	{
		if (WhiteRooks[i]->GetCol() == 6)
		{
			score += calcChromo->GetWeighting(RookOnSeventhRow);
			break;
		}
	}
	for (int i = 0; i < BlackRooks.size(); i++)
	{
		if (BlackRooks[i]->GetCol() == 1)
		{
			score -= calcChromo->GetWeighting(RookOnSeventhRow);
			break;
		}
	}
	
	score += RookBattery(WhiteRooks) * calcChromo->GetWeighting(RookBatteryType);
	score -= RookBattery(BlackRooks) * calcChromo->GetWeighting(RookBatteryType);
	
	score -= dynamic_cast<King*>(WhiteKing)->CastleMissed() * calcChromo->GetWeighting(CastleMissed);
	score += dynamic_cast<King*>(BlackKing)->CastleMissed() * calcChromo->GetWeighting(CastleMissed);
	
	score -= UnMovedWhiteKnightBishop(WhiteBishops, WhiteKnights) * calcChromo->GetWeighting(UnMovedKnightBishop);
	score += UnMovedBlackKnightBishop(BlackBishops, BlackKnights) * calcChromo->GetWeighting(UnMovedKnightBishop);
	
	score += calcChromo->GetWeighting(CenterAttackWeighting) * CenterAttackScore();
	score += calcChromo->GetWeighting(OpponentSquareWeighting) * OpponentSquareScore();
	score += calcChromo->GetWeighting(CastleScoreWeighting) * CastleScore();
	
	score += PawnStructureScore(WhitePawns, BlackPawns, WhiteRooks, calcChromo);
	score -= PawnStructureScore(BlackPawns, WhitePawns, BlackRooks, calcChromo);
	
	score += calcChromo->GetWeighting(OpenFileRookWeighting) * OpenFileRookScore();
	//const int CheckmateWeight = 100000;
	//score += CheckmateWeight * CheckMateScore();
	
	if (score < -1000000000 || score > 0x7FFFFFFF)
		ChessAssert();
	
	WhiteRooks.clear();
	BlackRooks.clear();
	WhiteKnights.clear();
	BlackKnights.clear();
	WhiteBishops.clear();
	BlackBishops.clear();
	for (int i =0 ; i < 8; i++)
	{
		WhitePawns[i].clear();
		BlackPawns[i].clear();
	}
	
	return score;
}

int ChessBoard::RookBattery(const std::vector<const ChessPiece*>& _Rooks)
{
	int RookBattery = 0;
	if (_Rooks.size() < 2)
		return 0;
	//for (int i = 0; i < _Rooks.size(); i++)
	{
		
		ChessSquare CurrLoc = _Rooks[0]->GetLocation();
		for (int i = 1; i < _Rooks.size(); i++)
		{
			if (_Rooks[i]->IsAttackingSquare(CurrLoc) )
				RookBattery++;
		}
	}
	return RookBattery;
}

int ChessBoard::UnMovedWhiteKnightBishop(const std::vector<const ChessPiece*>& WhiteBishops, const std::vector<const ChessPiece*>& WhiteKnights)
{
	int UnmovedPieces = 0;
	for (int i = 0; i < WhiteBishops.size(); i++)
	{
		if (WhiteBishops[i]->GetRow() == 0)
			UnmovedPieces++;
	}
	
	for (int i = 0; i < WhiteKnights.size(); i++)
	{
		if (WhiteKnights[i]->GetRow() == 0)
			UnmovedPieces++;	
	}
	return UnmovedPieces;
}

int ChessBoard::UnMovedBlackKnightBishop(const std::vector<const ChessPiece*>& BlackBishops, const std::vector<const ChessPiece*>& BlackKnights)
{
	int UnmovedPieces = 0;
	for (int i = 0; i < BlackBishops.size(); i++)
	{
		if (BlackBishops[i]->GetRow() == 7)
			UnmovedPieces++;
	}
	
	for (int i = 0; i < BlackKnights.size(); i++)
	{
		if (BlackKnights[i]->GetRow() == 7)
			UnmovedPieces++;	
	}
	return UnmovedPieces;
}

int ChessBoard::CalculatePieceScore(bool bWhite)
{
	int score = 0;
	
	Player* currPlayer;
	
	if (bWhite)
		currPlayer = _WhitePlayer;
	else 
	{
		currPlayer = _BlackPlayer;
	}

	for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
	{
		score+= currPlayer->GetPiece(i)->GetScore();
	}
	return score;
}

void QuickSort(	 std::vector<const ChessPiece*>& PawnLocations, int bottomrange, int toprange);

int ChessBoard::PawnStructureScore( const std::vector<const ChessPiece*> PlayerPawns[8], 
								   const std::vector<const ChessPiece*> OpponentPawns[8], 
								   const std::vector<const ChessPiece*>& PlayerRooks,
								   const Chromosome* calcChromo)
{
	//Check for isolated pawns
	
	//Sort white pawns by row
	//QuickSort(PlayerPawns, 0, PlayerPawns.size());

	//Check for isolated pawns
	//Check for double pawns
	int doublePawns = 0; 
	int isolatedPawns = 0;
	int centerPawns = 0;
	int passedPawns = 0;
	int LocalRookSupportedPassPawn = 0;
	
	bool bFoundPassPawn = false;
	for (int i = 0; i < 8; i++)
	{
		
		//Found a double pawn
		if (PlayerPawns[i].size() > 1)
		{
			doublePawns++;
		}
		
		//Found a pawn
		if (PlayerPawns[i].size() > 0)
		{
			if (i > 0 )
			{
				if (PlayerPawns[i-1].size() == 0)
				{
					if (i < 7)
					{
						if (PlayerPawns[i+1].size() == 0)
						{
							isolatedPawns++;
						}
						
					}
					else 
					{
						isolatedPawns++;
					}
				}
				if (OpponentPawns[i-1].size() == 0)
				{
					if (i < 7)
					{
						if (OpponentPawns[i+1].size() == 0)
						{
							passedPawns++;
							bFoundPassPawn = true;
						}
					}
					else {
						passedPawns++;
						bFoundPassPawn = true;
					}

				}
			}
			else 
			{
				if (PlayerPawns[i+1].size() == 0)
				{
					isolatedPawns++;
				}
				if (OpponentPawns[i+1].size() == 0)
				{
					passedPawns++;
					bFoundPassPawn = true;
				}
			}

			if (i ==4 | i == 5)
			{
				for (int j = 0; j < PlayerPawns[i].size(); j++ )
				{
					int currCol = PlayerPawns[i][j]->GetCol();
					if((currCol == 4 ) || (currCol == 5))
						centerPawns ++;
				}
			}
			
			if (bFoundPassPawn)
			{
				if (PlayerPawns[i].size() == 1)
				{
					for (int j = 0; j < PlayerRooks.size(); j++)
					{
						if (PlayerRooks[j]->GetRow() == i)
						{
							if (PlayerRooks[j]->GetCol() < PlayerPawns[i][0]->GetCol() )
							{
								if (PlayerRooks[j]->IsAttackingSquare(PlayerPawns[i][0]->GetLocation() ) )
									LocalRookSupportedPassPawn++;
							}
						}
					}
				}
			}
			bFoundPassPawn = false;
		}
	}
	
	return (-doublePawns * calcChromo->GetWeighting(DoublePawnWeight) + 
			-isolatedPawns * calcChromo->GetWeighting(IsolatedPawnWeight) + 
			centerPawns * calcChromo->GetWeighting(CenterPawnWeight) +
			passedPawns * calcChromo->GetWeighting(PassedPawns) + 
			LocalRookSupportedPassPawn * calcChromo->GetWeighting(RookSupportedPassPawn)
			);
}

void QuickSort(	std::vector<const ChessPiece*>& PawnLocations, int bottomrange, int toprange)
{
	if(( (toprange - bottomrange) == 1)||( (toprange - bottomrange) == 0))
	{
		//already sorted
		return;
	}
	
	const ChessPiece* pivot = PawnLocations[ (bottomrange + toprange) / 2 ];
	int pivotposition = (bottomrange + toprange) / 2;
	
	for (int i = bottomrange; i < pivotposition; i++)
	{
		if ( PawnLocations[i]->GetRow() > pivot->GetRow() )
		{
			const ChessPiece* tempMove = PawnLocations[i];
			PawnLocations.erase(PawnLocations.begin() + i);
			PawnLocations.insert(PawnLocations.begin() + pivotposition+1, tempMove);
			pivotposition--;
		}
	}
	
	for (int i = pivotposition; i < toprange; i++)
	{
		if ( PawnLocations[i]->GetRow() < pivot->GetRow() )
		{
			const ChessPiece* tempMove = PawnLocations[i];
			PawnLocations.erase(PawnLocations.begin() + i);
			PawnLocations.insert(PawnLocations.begin() + pivotposition-1, tempMove);
			pivotposition++;
		}
	}
	
	QuickSort(PawnLocations, bottomrange, pivotposition);
	QuickSort(PawnLocations, pivotposition, toprange);

}

int ChessBoard::OpenFileRookScore()
{
	Player* whitePlayer = _WhitePlayer;
	Player* blackPlayer = _BlackPlayer;
	
	int NumOpenFileRooks = 0;
	int NumSemiOpenFileRooks = 0;
	
	for (int i = 0; i < whitePlayer->GetNumPiecesLeft(); i++)
	{
		if (pieceToText(whitePlayer->GetPiece(i)) == 'R')
		{
			const std::vector<ChessMove> & PotMoves = whitePlayer->GetPiece(i)->GetEvalLegalMoves();
	
			int row = whitePlayer->GetPiece(i)->GetLocation().row;
			int numRowAttacks = 0;
			for (int i = 0; i < PotMoves.size(); i++)
			{
				if (PotMoves[i].getRow() == row)
					numRowAttacks++;
			}
			if (numRowAttacks > 4)
				NumSemiOpenFileRooks++;
			if (numRowAttacks == 7)
				NumOpenFileRooks++;
		}
		
	}
	for (int i = 0; i < blackPlayer->GetNumPiecesLeft(); i++)
	{
		if (pieceToText(blackPlayer->GetPiece(i)) == 'R')
		{
			const std::vector<ChessMove> & PotMoves = blackPlayer->GetPiece(i)->GetEvalLegalMoves();
			
			int row = blackPlayer->GetPiece(i)->GetLocation().row;
			int numRowAttacks = 0;
			for (int i = 0; i < PotMoves.size(); i++)
			{
				if (PotMoves[i].getRow() == row)
					numRowAttacks++;
			}
			if (numRowAttacks > 4)
				NumSemiOpenFileRooks--;
			if (numRowAttacks == 7)
				NumOpenFileRooks--;
		}
		
	}
	return NumOpenFileRooks;

}

int ChessBoard::CheckMateScore()
{
	if (_WhitePlayer->IsCheckMated() )
		return 1;
	else if (_BlackPlayer->IsCheckMated() )
		return -1;
	else
		return 0;
}

int ChessBoard::CastleScore()
{
	if (_WhitePlayer->HasCastled())
	{
		if (_BlackPlayer->HasCastled() )
			return 0;
		return 1;
	}
	else {
		if (_BlackPlayer->HasCastled() )
			return -1;
		return 0;
	}

}

int ChessBoard::OpponentSquareScore()
{
	std::vector<ChessSquare*>PotMoves;
	
	BlackPlayer* blackPlayer = _BlackPlayer;
	WhitePlayer* whitePlayer = _WhitePlayer;
	
	int attackdiff = 0;
	
	for (int i = 0; i < blackPlayer->GetNumPiecesLeft(); i++)
	{
		const std::vector<ChessMove> & PotMoves = blackPlayer->GetPiece(i)->GetEvalLegalMoves();
		for (int j = 0; j < PotMoves.size(); j++)
		{
			if (PotMoves[j].getCol() < 4)
				attackdiff--;
		}
	}
	for (int i = 0; i < whitePlayer->GetNumPiecesLeft(); i++)
	{
		const std::vector<ChessMove> & PotMoves = whitePlayer->GetPiece(i)->GetEvalLegalMoves();
		for (int j = 0; j < PotMoves.size(); j++)
		{
			if (PotMoves[j].getCol() > 3)
				attackdiff++;
		}

	}
	
	return attackdiff;
}

int ChessBoard::CenterAttackScore()
{
	BlackPlayer* blackPlayer = _BlackPlayer;
	WhitePlayer* whitePlayer = _WhitePlayer;
	
	int attackdiff = 0;
	
	for (int i = 0; i < blackPlayer->GetNumPiecesLeft(); i++)
	{
		if (blackPlayer->GetPiece(i)->IsAttackingSquare(3,3))
			attackdiff--;
		if (blackPlayer->GetPiece(i)->IsAttackingSquare(3,4))
			attackdiff--;
		if (blackPlayer->GetPiece(i)->IsAttackingSquare(4,3))
			attackdiff--;
		if (blackPlayer->GetPiece(i)->IsAttackingSquare(4,4))
			attackdiff--;
	}
	for (int i = 0; i < whitePlayer->GetNumPiecesLeft(); i++)
	{
		if (whitePlayer->GetPiece(i)->IsAttackingSquare(3,3))
			attackdiff++;
		if (whitePlayer->GetPiece(i)->IsAttackingSquare(3,4))
			attackdiff++;
		if (whitePlayer->GetPiece(i)->IsAttackingSquare(4,3))
			attackdiff++;
		if (whitePlayer->GetPiece(i)->IsAttackingSquare(4,4))
			attackdiff++;
	}
	return attackdiff;
}

int ChessBoard::MakeMove(const ChessMove& move)
{ 
	if (move.isCastle() )
	{
		ChessPiece* _king;
		ChessPiece* _rook;
		if (bWhiteMove)
			_king = GetPiece(_WhitePlayer->GetKingLocation());
			else {
				_king = GetPiece(_BlackPlayer->GetKingLocation());
			}
		
		
		int col;
		if (bWhiteMove)
			col = 0;
			else {
				col = 7;
			}
		
		if (move.isKingSideCastle() )
		{
			_rook = GetPiece(7, col);
			Castle(_king, _rook, true, col);
		}
		else {
			_rook = GetPiece(0, col);
			Castle(_king, _rook, false, col);
		}
		
	}
	else if (move.isPromotion() )
	{
		ChessPiece* _pawn = GetPiece(move.getStartingRow(), move.getStartingCol());
		ChessSquare* newMove = new ChessSquare(move.getRow(), move.getCol() );
		PromotePawn(dynamic_cast<Pawn*>(_pawn),
					newMove ,
					move.GetPromotionChar()
					);
		//ChessBoard::GetInstance()->PrintBoard();
		delete newMove;
		
	}
	else if (move.IsEnPassant())
	{
		EnPassantCapture(move.getPiece(), ChessSquare(move.getRow(), move.getCol()), move.getCapturedPiece()->GetLocation()); // to code this later
	}
	else if (move.isCapture())
	{
		MakeCapture(move.getPiece(), ChessSquare(move.getRow(), move.getCol()));
	}
	else
		MovePiece(move.getPiece(), move.getRow(), move.getCol());
	
	if (move.isPromotion())
	{
		ResolveLineOfSight(Board[move.getRow()][move.getCol()]);
	}
	else if (!move.isCastle())
	{
		ResolveLineOfSight(move.getPiece());
	}
	else
	{
		ResolveCastleLineOfSight(!bWhiteMove);
	}
	//move->Delete();
	//delete(move);
	
	return 0;
}

void ChessBoard::ResolveCastleLineOfSight(bool bIsWhite)
{
	Player* opponentPlayer;
	Player* currPlayer;
	if (bIsWhite)
	{
		opponentPlayer = _BlackPlayer;
		currPlayer = _WhitePlayer;
	}
	else
	{
		opponentPlayer = _WhitePlayer;
		currPlayer = _BlackPlayer;
	}
	
	ChessSquare OpponentKingLoc = opponentPlayer->GetKingLocation();
	for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
	{
		ChessPiece* currPiece = currPlayer->GetPiece(i);
		if (pieceToText(currPiece) == 'R')
		{
			if (currPiece->HasLineToKing(OpponentKingLoc) )
			{
				currPlayer->AddToKingAttacking(currPiece);
			}
		}
	}
	opponentPlayer->ClearAttackingKingPieces();
	ChessSquare KingLoc = currPlayer->GetKingLocation();
	for (int i = 0; i < opponentPlayer->GetNumPiecesLeft(); i++)
	{
		ChessPiece* currPiece = opponentPlayer->GetPiece(i);
		
		if (currPiece->HasLineToKing( KingLoc) )
			opponentPlayer->AddToKingAttacking(currPiece);
	}
				
	
}

void ChessBoard::ResolveLineOfSight(ChessPiece* movedPiece)
{
	Player* opponentPlayer;
	Player* currPlayer;
	if (movedPiece->isWhite())
	{
		opponentPlayer = _BlackPlayer;
		currPlayer = _WhitePlayer;
	}
	else
	{
		opponentPlayer = _WhitePlayer;
		currPlayer = _BlackPlayer;
	}
	//Will be handled here now lulz
	if (pieceToText(movedPiece) == 'K')
	{
		opponentPlayer->ClearAttackingKingPieces();
		ChessSquare KingLoc = currPlayer->GetKingLocation();
		for (int i = 0; i < opponentPlayer->GetNumPiecesLeft(); i++)
		{
			ChessPiece* currPiece = opponentPlayer->GetPiece(i);
			if (currPiece->HasLineToKing(KingLoc) )
				opponentPlayer->AddToKingAttacking(currPiece);
		}
		return;
	}
	
	ChessSquare KingLoc = opponentPlayer->GetKingLocation();
	
	if (!movedPiece->IsInKingArray() )
	{
		if (movedPiece->HasLineToKing(KingLoc) )
		{
			currPlayer->AddToKingAttacking(movedPiece);
		}
	}
	else 
	{
		if (movedPiece->HasLineToKing(KingLoc) )
		{
			//currPlayer->AddToKingAttacking(movedPiece);
		}
		else
			currPlayer->RemoveKingAttacking(movedPiece);
	}
	
}

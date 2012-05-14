/*
 *  ChessBoard.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-20.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Constants.h"
#include <vector>
#include "ChessMove.h"
#include "ChessPiece.h"
#include "Constants.h"
#include "ChessLog.h"
#include "ChessPlayer.h"
#include <stdio.h>
#include <stdlib.h>
#include "Pawn.h"
#include "ChessChromosome.h"

//#include "Tournament.h"

class WhitePlayer;
class BlackPlayer;
class Player;
class Pawn;
class ChessLog;

class ChessBoard
{
private:
	ChessBoard();
public:
	~ChessBoard();
	void SetWhitePlayer(WhitePlayer* _Player) { _WhitePlayer = _Player; }
	void SetBlackPlayer(BlackPlayer* _Player) { _BlackPlayer = _Player; }
	bool IsWhiteMove(){ return bWhiteMove; }
	static ChessBoard* GetInstance();
	WhitePlayer* GetWhitePlayer() { return _WhitePlayer; }
	BlackPlayer* GetBlackPlayer() { return _BlackPlayer; }
	bool isWhiteMove() {return bWhiteMove; }
	int MovePiece(ChessPiece* piece, int row, int col); // do error checking in here?
	int MakeMove(const ChessMove& move);
	void PromotePawn(Pawn* pawn, const ChessSquare* MoveTo, char PromotionPawn);
	ChessPiece* GetPiece(ChessSquare Location) const { return GetPiece(Location.row, Location.col); }
	ChessPiece* GetPiece(int row, int col) const;
	bool bHasPiece(int row, int col);
	int KingCastle();
	int QueenCastle();
	int AddPiece(ChessPiece* piece);
	int AddPieceNoLineOfSight(ChessPiece* piece);
	void RemovePiece(int row, int col) { Board[row][col] = NULL; }
	void PrintBoard();
	int Castle(ChessPiece* _King, ChessPiece* _Rook, bool bKingSide, int col);
	void MakeCapture(ChessPiece* piece, const ChessSquare& MoveTo);
	ChessSquare GetKingLocation(bool isWhite);
	void EnPassantCapture(ChessPiece* piece, const ChessSquare& MoveTo, const ChessSquare& CaptPieceLoc);
	void PutPlayerInCheck(bool isWhite, const ChessPiece* checkingPiece);
	void ResolveLineOfSight(ChessPiece* movedPiece);
	void ResolveCastleLineOfSight(bool bIsWhite);
	
	//AI Helper
	void UndoMove();
	
	//AI Methods
	int CalculateScore(const Chromosome* calcChromo);
	
	int CalculatePieceScore(bool bWhite);
	
	bool IsEqual(ChessBoard const * const ReferenceBoard) const;
	
	
	void ResetBoard();
	
	int UnMovedWhiteKnightBishop(const std::vector<const ChessPiece*>& WhiteBishops, const std::vector<const ChessPiece*>& WhiteKnights);
	int UnMovedBlackKnightBishop(const std::vector<const ChessPiece*>& WhiteBishops, const std::vector<const ChessPiece*>& WhiteKnights);
	
	int RookBattery(const std::vector<const ChessPiece*>& _Rooks);
	
	void SetChessLog(ChessLog * _LogInstance) { _ChessLogInstance = _LogInstance;}
	ChessLog* GetLog() const { return _ChessLogInstance;}
	
	void Erase();
	
private:
	int CenterAttackScore();
	int OpponentSquareScore();
	int CastleScore();
	int PawnStructureScore( const std::vector<const ChessPiece*> PlayerPawns[8], 
							const std::vector<const ChessPiece*> OpponentPawns[8], 
						   const std::vector<const ChessPiece*>& WhiteRooks,
						   const Chromosome* calcChromo);
	int OpenFileRookScore();
	int CheckMateScore();
	
private:
	//void Capture(ChessSquare& MoveTo);
	void RemovePiece();
	bool bInCheckWhite;
	bool bInCheckBlack;
	int Promote();
	bool CheckForCheck(const ChessPiece* piece);
	void ResolveCheck(bool bWhiteMove);
	
	bool bWhiteMove;
	bool bBlackMove;
				   
	
	
	WhitePlayer* _WhitePlayer;
	BlackPlayer* _BlackPlayer;
	ChessLog* _ChessLogInstance;
	//used as a dynamic stack
	//std::vector<ChessMove *> Moves;
	ChessPiece* Board[8][8];
	
	void ClearBoard()
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				Board[i][j] = NULL;
			}
		}
	}
	
public:
	void Copy(const ChessBoard* ReferenceBoard);
	ChessBoard* Copy();
};

#endif
/*
 *  ChessPlayer.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-20.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHESSPLAYER_H
#define CHESSPLAYER_H

#include "ChessMove.h"
#include "Constants.h"
#include "ChessPiece.h"
#include "ChessBoard.h"

#include <vector>

#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "King.h"

class WhitePlayer;
class BlackPlayer;
class Player;

class Player
{
public:
	//virtual ChessMove MakeAMove() = 0;
	virtual ~Player() { }
	virtual void RemovePiece(ChessPiece* piece) = 0;
	virtual ChessPiece* GetPiece(int index) const = 0;
	virtual int GetNumPiecesLeft() const = 0;
	virtual ChessSquare GetKingLocation() = 0;
	virtual void EnterCheckState() = 0;
	virtual void ClearCheckState() = 0;
	virtual bool IsInCheck() = 0;
	virtual void PawnPromotion(ChessPiece* pawn, ChessPiece* newPiece) = 0;
	//virtual ChessPiece* GetCheckingPiece() = 0;
	//virtual ChessSquare GetCheckingPieceLocation() = 0;
	virtual bool CheckForCheckMate() = 0;
	virtual ChessPiece* FindCapturedPiece(const int row, const int col, ChessPiece* capturedPiece) = 0;
	virtual void RebirthCapturedPiece(ChessPiece* capturedPiece) = 0;
	virtual void UndoPawnPromotion(int row, ChessSquare promotionSquare, ChessPiece* PromotedPiece) = 0;
	virtual const bool HasCastled() const = 0;
	static const bool CanCastle(const bool bIsWhite, const bool bKingSide, ChessBoard* _BoardInstance);
	virtual const bool CanCastle(bool bKingSide, ChessBoard* _BoardInstance) = 0;
	virtual void AddToKingAttacking(ChessPiece* piece) = 0;
	virtual void RemoveKingAttacking(ChessPiece* piece) = 0;
	virtual const int GetNumAttackingKingPieces() const = 0;
	virtual ChessPiece const * GetAttackingKingPiece(int index) const = 0;
	virtual void ClearAttackingKingPieces() = 0;
	virtual bool CheckForStalemate() const = 0;
	virtual void Erase() = 0;
};

class WhitePlayer : public Player
{
private:
	WhitePlayer(bool bInit);
public:
	~WhitePlayer() { assert( remainingPieces.size() == 0); }
	ChessMove MakeAMove();
	static WhitePlayer* GetInstance();
	void RemovePiece(ChessPiece* piece);
	ChessPiece* GetPiece(int index) const;
	int GetNumPiecesLeft() const { return remainingPieces.size(); }
	ChessSquare GetKingLocation();
	void EnterCheckState();
	void ClearCheckState()
	{ 
		bIsInCheck = false;
		bIsCheckmated = false;
	}
	void PawnPromotion(ChessPiece* pawn, ChessPiece* newPiece);
	bool IsInCheck() { return bIsInCheck; }
	bool IsCheckMated() { return bIsCheckmated; }
	bool CheckForCheckMate();
	//ChessPiece* GetCheckingPiece() { return checkingPiece; }
	//ChessSquare GetCheckingPieceLocation() { return checkingPiece->GetLocation(); }
	ChessPiece* FindCapturedPiece(const int row, const int col, ChessPiece* capturedPiece);
	void RebirthCapturedPiece(ChessPiece* capturedPiece);
	void UndoPawnPromotion(int row, ChessSquare promotionSquare, ChessPiece* PromotedPiece);
	const bool HasCastled() const { return bHasCastled; }
	void Castle() { bHasCastled = true; }
	const bool CanCastle(bool bKingSide, ChessBoard* _BoardInstance);
	void ResetPlayer();
	void AddToKingAttacking(ChessPiece* piece);
	void RemoveKingAttacking(ChessPiece* piece);
	
	ChessPiece* FindPiece(ChessPiece* ReferencePiece) // For Multithreading
	{
		for (int i = 0; i < remainingPieces.size(); i++)
		{
			if (ReferencePiece->isEqual(remainingPieces[i]) )
				return remainingPieces[i];
		}
		PrintPlayer();
		printf("Piece: %c, Location %c%i", pieceToText(ReferencePiece), rowToLetter(ReferencePiece->GetRow()), ReferencePiece->GetCol()+1);
		ChessAssert();
		return NULL;
	}
	
	bool CheckForStalemate() const ;
	
	const int GetNumAttackingKingPieces() const { return KingAttackingPieces.size(); }
	const ChessPiece* GetAttackingKingPiece(int index)  const{ return KingAttackingPieces[index]; }
	void ClearAttackingKingPieces() 
	{ 
		for (int i = 0; i < KingAttackingPieces.size(); i++)
		{
			KingAttackingPieces[i]->SetIsInKingArray(false);
		}
		KingAttackingPieces.clear(); 
	}
	void InitWhitePlayer();
	WhitePlayer* Copy();
	
	void AddPiecesToBoard();
	
	void SetPieceBoardPointer(ChessBoard* _BoardPtr)
	{
		for (int i = 0; i < promotedPiece.size(); i++)
		{
			promotedPiece[i]->SetBoardPointer(_BoardPtr);
		}
		for (int i = 0; i < capturedPieces.size(); i++)
		{
			capturedPieces[i]->SetBoardPointer(_BoardPtr);
		}
		for (int i = 0; i < KingAttackingPieces.size(); i++)
		{
			KingAttackingPieces[i]->SetBoardPointer(_BoardPtr);
		}
		for (int i = 0; i < remainingPieces.size(); i++)
		{
			remainingPieces[i]->SetBoardPointer(_BoardPtr);
		}
		ChessBoardInstance = _BoardPtr;
	}
	
	void PrintPlayer()
	{
		printf("White: ");
		for (int i = 0; i < remainingPieces.size(); i++)
		{
			ChessSquare PieceLoc = remainingPieces[i]->GetLocation();
			printf("Piece %c Location: %c%i ", pieceToText(remainingPieces[i]), rowToLetter(PieceLoc.row), PieceLoc.col+1);
			remainingPieces[i]->isWhite() ? printf("White") : printf("Black") ;
		}
	}
	
	void Erase()
	{
		for (int i = (promotedPiece.size() - 1); i >= 0; i--)
		{
			delete (promotedPiece[i]);
		}
		promotedPiece.clear();
		for (int i = (remainingPieces.size() - 1); i >= 0; i--)
		{
			delete (remainingPieces[i]);
		} 
		remainingPieces.clear();
		for (int i = (capturedPieces.size() - 1); i >= 0; i--)
		{
			delete (capturedPieces[i]);
		} 
		capturedPieces.clear();
		KingAttackingPieces.clear();
	}
	
private:
	bool bHasCastled;
	bool bIsInCheck;
	bool bIsCheckmated;
	ChessPiece* checkingPiece;
	std::vector<ChessPiece*> promotedPiece;
	void AddPromoPiece(ChessPiece* _Piece) { promotedPiece.push_back(_Piece); }
	std::vector<ChessPiece*> remainingPieces;
	void AddRemainPiece(ChessPiece* _Piece) { remainingPieces.push_back(_Piece); }
	std::vector<ChessPiece*> capturedPieces;
	void AddCaptPiece(ChessPiece* _Piece) { capturedPieces.push_back(_Piece); }
	std::vector<ChessPiece*> KingAttackingPieces;
	void AddKingPiece(ChessPiece* _Piece) { KingAttackingPieces.push_back(_Piece); }
	
	ChessBoard* ChessBoardInstance;
};

class BlackPlayer : public Player
{
private:
	BlackPlayer(bool bInit);
public:
	~BlackPlayer() { assert( remainingPieces.size() == 0); }
	ChessMove MakeAMove();
	static BlackPlayer* GetInstance();
	void RemovePiece(ChessPiece* piece);
	ChessPiece* GetPiece(int index) const;
	int GetNumPiecesLeft() const { return remainingPieces.size(); }
	ChessSquare GetKingLocation();
	void EnterCheckState();
	void ClearCheckState()
	{ 
		bIsInCheck = false; 
		bIsCheckmated = false;
	}
	bool IsInCheck() { return bIsInCheck; }
	bool IsCheckMated() { return bIsCheckmated; }
	bool CheckForCheckMate();
	//ChessPiece* GetCheckingPiece() { return checkingPiece; }
	void PawnPromotion(ChessPiece* pawn, ChessPiece* newPiece);
	//ChessSquare GetCheckingPieceLocation() { return checkingPiece->GetLocation(); }
	ChessPiece* FindCapturedPiece(const int row, const int col, ChessPiece* capturedPiece);
	void RebirthCapturedPiece(ChessPiece* capturedPiece);
	void UndoPawnPromotion(int row, ChessSquare promotionSquare, ChessPiece* PromotedPiece);
	const bool HasCastled() const { return bHasCastled; }
	void Castle() { bHasCastled = true; }
	const bool CanCastle(bool bKingSide, ChessBoard* _BoardInstance);
	void ResetPlayer();
	void AddToKingAttacking(ChessPiece* piece);
	void RemoveKingAttacking(ChessPiece* piece);
	
	bool CheckForStalemate() const ;
	
	const int GetNumAttackingKingPieces() const { return KingAttackingPieces.size(); }
	const ChessPiece* GetAttackingKingPiece(int index) const { return KingAttackingPieces[index]; }
	void ClearAttackingKingPieces() 
	{ 
		for (int i = 0; i < KingAttackingPieces.size(); i++)
		{
			KingAttackingPieces[i]->SetIsInKingArray(false);
		}
		KingAttackingPieces.clear(); 
	}
	void InitBlackPlayer();
	BlackPlayer* Copy();
	
	
	ChessPiece* FindPiece(ChessPiece* ReferencePiece) // For Multithreading
	{
		for (int i = 0; i < remainingPieces.size(); i++)
		{
			if (ReferencePiece->isEqual(remainingPieces[i]) )
				return remainingPieces[i];
		}
		PrintPlayer();
		printf("Piece: %c, Location %c%i", pieceToText(ReferencePiece), rowToLetter(ReferencePiece->GetRow()), ReferencePiece->GetCol()+1);
		ChessAssert();
		return NULL;
	}
	
	void AddPiecesToBoard();
	
	void SetPieceBoardPointer(ChessBoard* _BoardPtr)
	{
		for (int i = 0; i < promotedPiece.size(); i++)
		{
			promotedPiece[i]->SetBoardPointer(_BoardPtr);
		}
		for (int i = 0; i < capturedPieces.size(); i++)
		{
			capturedPieces[i]->SetBoardPointer(_BoardPtr);
		}
		for (int i = 0; i < KingAttackingPieces.size(); i++)
		{
			KingAttackingPieces[i]->SetBoardPointer(_BoardPtr);
		}
		for (int i = 0; i < remainingPieces.size(); i++)
		{
			remainingPieces[i]->SetBoardPointer(_BoardPtr);
		}
		ChessBoardInstance = _BoardPtr;
	}
	
	void PrintPlayer()
	{
		printf("Black:  ");
		for (int i = 0; i < remainingPieces.size(); i++)
		{
			ChessSquare PieceLoc = remainingPieces[i]->GetLocation();
			printf("Piece %c Location: %c%i\n", pieceToText(remainingPieces[i]), rowToLetter(PieceLoc.row), PieceLoc.col+1);		
		}
	}
	
	void Erase()
	{
		for (int i = (promotedPiece.size() - 1); i >= 0; i--)
		{
			delete (promotedPiece[i]);
		}
		promotedPiece.clear();
		for (int i = (remainingPieces.size() - 1); i >= 0; i--)
		{
			delete (remainingPieces[i]);
		} 
		remainingPieces.clear();
		for (int i = (capturedPieces.size() - 1); i >= 0; i--)
		{
			delete (capturedPieces[i]);
		} 
		capturedPieces.clear();
		
		KingAttackingPieces.clear();
	}
	
private:
	bool bHasCastled;
	bool bIsInCheck;
	bool bIsCheckmated;
	ChessPiece* checkingPiece;
	std::vector<ChessPiece*> promotedPiece;
	void AddPromoPiece(ChessPiece* _Piece) { promotedPiece.push_back(_Piece); }
	std::vector<ChessPiece*> remainingPieces;
	void AddRemainPiece(ChessPiece* _Piece) { remainingPieces.push_back(_Piece); }
	std::vector<ChessPiece*> capturedPieces;
	void AddCaptPiece(ChessPiece* _Piece) { capturedPieces.push_back(_Piece); }
	std::vector<ChessPiece*> KingAttackingPieces;
	void AddKingPiece(ChessPiece* _Piece) { KingAttackingPieces.push_back(_Piece); }
	ChessBoard* ChessBoardInstance;
};

#endif
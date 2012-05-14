/*
 *  NeuralRoot.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-07.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "NeuralRoot.h"

static NeuralRoot* m_RootInstance;

NeuralRoot* NeuralRoot::GetInstance()
{
	if (m_RootInstance == NULL)
	{
		m_RootInstance = new NeuralRoot();
	}
	return m_RootInstance;
}

NeuralRoot::NeuralRoot()
{
	m_WhiteInstance = WhitePlayer::GetInstance();
	m_BlackInstance = BlackPlayer::GetInstance();
	
	m_PawnArgs = new Pawn_Args();
	m_KnightArgs = new Knight_Args();
	m_BishopArgs = new Bishop_Args();
	m_RookArgs = new Rook_Args();
	m_QueenArgs = new Queen_Args();
	m_KingArgs = new King_Args();
	m_MinorPiecesArgs = new MinorPieces_Args(m_KnightArgs, m_BishopArgs);
	m_MajorPiecesArgs = new MajorPieces_Args(m_RookArgs, m_QueenArgs, m_KingArgs);
}

void NeuralRoot::TransmitToChildren()
{
	/*for (int i = Children.size() - 1; i >= 0; i--)
	 {
	 //Children already have pointers to the necessary structures
	 //Just signal them to update
	 Children[i]->Update();
	 }*/
}

void NeuralRoot::SetupNode()
{
	PawnChildren.push_back(new PawnAdvantage() );
	PawnChildren.push_back(new CenterPawn() );
	PawnChildren.push_back(new DoublePawn() );
	PawnChildren.push_back(new IsolatedPawn() );
	/*
	MinorPiecesChildren.push_back(new MinorPieceRoot() );
	
	MajorPiecesChildren.push_back(new MajorPieceRoot() );*/
	
	RookChildren.push_back(new RookMobility() );
	RookChildren.push_back(new RookAdvantage() );
	RookChildren.push_back(new RookUnderAttack() );
	
	KnightChildren.push_back(new KnightMobility() );
	KnightChildren.push_back(new KnightAdvantage() );
	KnightChildren.push_back(new KnightUnderAttack() );
	
	BishopChildren.push_back(new BishopMobility() );
	BishopChildren.push_back(new BishopAdvantage() );
	BishopChildren.push_back(new BishopUnderAttack() );
	
	QueenChildren.push_back(new QueenMobility() );
	QueenChildren.push_back(new QueenAdvantage() );
	QueenChildren.push_back(new QueenUnderAttack() );
	
	KingChildren.push_back(new KingMobility() );
	KingChildren.push_back(new KingInCheck() );
	
	BoardChildren.push_back(new Castle() );
	BoardChildren.push_back(new CenterAttack() );
	BoardChildren.push_back(new OpponentSquareAttack() );
}

void NeuralRoot::UpdateTree()
{
	ChessPiece* currPiece;
	char c;
	
	//Erase current args
	m_PawnArgs->Delete();
	m_KnightArgs->Delete();
	m_BishopArgs->Delete();
	m_RookArgs->Delete();
	m_QueenArgs->Delete();
	m_KingArgs->Delete();
	
	for (int i = 0; i < m_WhiteInstance->GetNumPiecesLeft(); i++)
	{
		currPiece = m_WhiteInstance->GetPiece(i);
		c = pieceToText(currPiece);
		switch (c) 
		{
			case 'P':
				m_PawnArgs->WhitePawns.push_back(dynamic_cast<Pawn*>(currPiece)); 
				break;
			case 'N':
				m_KnightArgs->WhiteKnights.push_back(dynamic_cast<Knight*>(currPiece)); 
				break;
			case 'B':
				m_BishopArgs->WhiteBishops.push_back(dynamic_cast<Bishop*>(currPiece)); 
				break;
			case 'R':
				m_RookArgs->WhiteRooks.push_back(dynamic_cast<Rook*>(currPiece)); 
				break;
			case 'Q':
				m_QueenArgs->WhiteQueens.push_back(dynamic_cast<Queen*>(currPiece)); 
				break;
			case 'K':
				m_KingArgs->WhiteKings = dynamic_cast<King*>(currPiece); 
				break;
			default:
				ChessAssert();
				break;
		}
	}
	
	for (int i = 0; i < m_BlackInstance->GetNumPiecesLeft(); i++)
	{
		currPiece = m_BlackInstance->GetPiece(i);
		c = pieceToText(currPiece);
		switch (c) 
		{
			case 'P':
				m_PawnArgs->BlackPawns.push_back(dynamic_cast<Pawn*>(currPiece)); 
				break;
			case 'N':
				m_KnightArgs->BlackKnights.push_back(dynamic_cast<Knight*>(currPiece)); 
				break;
			case 'B':
				m_BishopArgs->BlackBishops.push_back(dynamic_cast<Bishop*>(currPiece)); 
				break;
			case 'R':
				m_RookArgs->BlackRooks.push_back(dynamic_cast<Rook*>(currPiece)); 
				break;
			case 'Q':
				m_QueenArgs->BlackQueens.push_back(dynamic_cast<Queen*>(currPiece)); 
				break;
			case 'K':
				m_KingArgs->BlackKings = dynamic_cast<King*>(currPiece); 
				break;
			default:
				ChessAssert();
				break;
		}
	}
}

int NeuralRoot::GetBoardScore()
{
	for (int i = 0; i < PawnChildren.size(); i++)
	{
		PawnChildren[i]->Update(m_PawnArgs);
		PawnChildren[i]->TransmitToChildren();
	}
	for (int i = 0 ; i < KnightChildren.size(); i++)
	{
		KnightChildren[i]->Update(m_KnightArgs);
		KnightChildren[i]->TransmitToChildren();
	}
	for (int i = 0 ; i < BishopChildren.size(); i++)
	{
		BishopChildren[i]->Update(m_BishopArgs);
		BishopChildren[i]->TransmitToChildren();
	}
	
	for (int i = 0; i < RookChildren.size(); i++)
	{
		RookChildren[i]->Update(m_RookArgs);
		RookChildren[i]->TransmitToChildren();
	}
	
	for (int i = 0; i < QueenChildren.size(); i++)
	{
		QueenChildren[i]->Update(m_QueenArgs);
		QueenChildren[i]->TransmitToChildren();
	}
	
	for (int i = 0; i < KingChildren.size(); i++)
	{
		KingChildren[i]->Update(m_KingArgs);
		KingChildren[i]->TransmitToChildren();
	}
	
	return NeuralOutput::GetInstance()->GetScore();
}
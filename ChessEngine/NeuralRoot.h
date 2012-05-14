/*
 *  NeuralRoot.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-07.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NEURALROOT_H
#define NEURALROOT_H

#include "NeuralNode.h"
#include <vector>
#include "NeuralInput.h"
#include "ChessPlayer.h"
#include "MinorPieceNodes.h"
#include "MajorPieceNodes.h"
#include "BoardNodes.h"
#include "PawnNodes.h"

class NeuralRoot : public NeuralNode
{
	//Children are globals, 
	//Splits to minor and major
public:
	//Singleton tree
	static NeuralRoot* GetInstance();
	virtual void SetWeight(int NewWeight) { };
protected:
	bool IsActivated() { return true; }
	void TransmitToChildren();
	NeuralRoot();
	void SetupNode(); //Add the childrens, etc.
	int GetBoardScore();
public:
	void UpdateTree();
private:
	WhitePlayer* m_WhiteInstance;
	BlackPlayer* m_BlackInstance;
	Pawn_Args* m_PawnArgs;
	Knight_Args* m_KnightArgs;
	Bishop_Args* m_BishopArgs;
	Rook_Args* m_RookArgs;
	Queen_Args* m_QueenArgs;
	King_Args* m_KingArgs;
	MinorPieces_Args* m_MinorPiecesArgs;
	MajorPieces_Args* m_MajorPiecesArgs;
	std::vector<NeuralNode*> PawnChildren;
	
	std::vector<NeuralNode*> MinorPiecesChildren;
	std::vector<NeuralNode*> MajorPiecesChildren;
	
	std::vector<NeuralNode*> KnightChildren;
	std::vector<NeuralNode*> BishopChildren;
	std::vector<NeuralNode*> RookChildren;
	std::vector<NeuralNode*> QueenChildren;
	std::vector<NeuralNode*> KingChildren;
	
	std::vector<NeuralNode*> BoardChildren;
};

#endif
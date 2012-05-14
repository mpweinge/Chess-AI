/*
 *  MoveTree.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-02-12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MOVETREE_H
#define MOVETREE_H

#include "MoveNode.h"
#include "ChessMove.h"

class TreeLevel
{
public:
	TreeLevel(bool isAlpha) : bIsAlpha(isAlpha), Beta(-1000000), Alpha(0x7FFFFFFF) {}
	bool isMin() { return bIsAlpha; }
	bool isMax() { return !bIsAlpha; }
	void SetMin(int _Alpha) { Alpha = _Alpha; }
	void SetMax(int _Beta) { Beta = _Beta; }
	int GetMin(){ return Alpha; }
	void SetBestMove(MoveNode* _bestMove) { bestMove = _bestMove; }
	MoveNode* GetBestMove() { return bestMove; }
	int GetMax(){ return Beta; }
private:
	MoveNode* bestMove;
	bool bIsAlpha;
	int Alpha;
	int Beta;
};

class MoveTree;

struct ThreadStruct
{
	int Ply;
	MoveNode* CurrentNode;
	bool bIsWhite;
	MoveTree* Tree;
	ChessBoard* ChessBoardInstance;
};

class MoveTree
{
private:
	int FixedPly;
	bool bIsWhite;
	MoveNode* root;
	Chromosome* attachedChromosome;
public:
	MoveTree(int Ply, bool bIsWhite, Chromosome* _AttachedChromosome);// { FixedPly = Ply; } 
	~MoveTree() { root->Erase(); delete (root); }
private:
	void AddNode(MoveNode* square, MoveNode* parent);
public:
	void FillAlphaBetaNode(int Ply, MoveNode* Parent, bool bIsWhite, WhitePlayer* _WhitePlayer, BlackPlayer* _BlackPlayer, ChessBoard* ChessBoardInstance);
private:
	TreeLevel* GetLevelFromDepth(int depth);
	MoveNode* MoveToMoveNode(const ChessMove& currMove, MoveNode* Parent, bool bIsMinLevel, ChessBoard* _ChessBoardInstance);

public:
	void MakeMove(ChessMove* squre);
	void GenerateTree(); // get possible moves
	ChessMove* GetHighestScoreMove();
	void PrintTree();
	void PruneBranches();
	//Prune tree
private:
	void RemoveSubTree(ChessMove* parent);
	std::vector<TreeLevel*> TreeLevels;
	void ClearTree();
	void FillNode(int Ply, MoveNode* Parent, bool bIsWhite, WhitePlayer* _WhitePlayer, BlackPlayer* _BlackPlayer, ChessBoard* _ChessBoardInstance);
	int FindScore(MoveNode* Parent, bool bMax);
	void UpdateDepth(MoveNode* Parent, int depth);
	MoveNode* BestMove;
	
	pthread_t TreeThreads[NUM_THREADS];
	
	int m_iCurrentRunningThreads;
};

#endif
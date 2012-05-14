/*
 *  MoveNode.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-02-12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MOVENODE_H
#define MOVENODE_H

#include "ChessMove.h"
#include "ChessBoard.h"

class MoveNode
{
public: 
	MoveNode(const ChessMove& GuessMove, 
			 MoveNode* _parent, 
			 bool _bEvaluate, 
			 bool _MinLevel, 
			 Chromosome* _calcChromo, 
			 ChessBoard* _ChessBoardInstance);
	
	~MoveNode() {assert(Children.size() == 0); }
	
	MoveNode* Copy()
	{
		MoveNode* ret = new MoveNode(move, parent, bEvaluate, bMinLevel, calcChromo, ChessBoardInstance);
#ifdef PRINT_MEMORY
		printf("Created move node at:%i\n", ret);
		fflush(stdout);
#endif
		ret->depth = depth;
		return ret;
	}
	
	int GetBoardScore() { return BoardScore; }
	void SetBoardScore(int _BoardScore) { /*assert(!bLeafNode);*/ BoardScore = _BoardScore; }
	void AddChild(MoveNode* node) 
	{ 
		Children.push_back(node); 
	}
	MoveNode* GetChild(int i) 
	{ 
		return Children[i]; 
	}
	void MakeMove() { ChessBoardInstance->MakeMove(move); }
	void UndoMove() { ChessBoardInstance->UndoMove(); }
	bool IsLeafNode() { return bLeafNode; }
	inline void removeChild(int index) { Children.erase(Children.begin() + index); }
	bool IsNullMove() { return move.IsNullMove(); }
	void CalculateScore() 
		{ 
			//if (move != NULL)
			{
				ChessBoardInstance->MakeMove(move); 
				BoardScore = ChessBoardInstance->CalculateScore(calcChromo);
#ifndef SUPPRESS_OUTPUT
				printf("Score: %i", BoardScore);
#endif
				ChessBoardInstance->UndoMove();
			}
			//bLeafNode = true;
		}
	int GetNumChildren() { return Children.size(); }
	MoveNode* getParent() { return parent; }
	bool IsEqual(MoveNode* comparisonNode)
	{
		return move.isEqual(comparisonNode->GetMove() );
	}
	void Erase() 
	{ 
		if (Children.size() == 0)
		{
			DeleteAllMemory();
		}
		else {
			for (int i = Children.size() - 1; i >= 0 ; i--)
			{
				Children[i]->Erase();
				delete(Children[i]);
				removeChild(i);
			}
			DeleteAllMemory();
		}
	}
	void PrintNode();
	ChessMove& GetMove() { return move; }
	int depth;
	MoveNode* GetParent() { return parent; }
	
	void SwitchMinLevel() {bMinLevel = !bMinLevel; }
	
	void DeleteAllMemory();
	
	bool IsMaxLevel() { return !bMinLevel; }
	bool IsMinLevel() { return bMinLevel; }
	int GetMinScore() { assert (!bMinLevel); return iMinScore; }
	void SetMinScore(int _MinScore) { iMinScore = _MinScore; }
	void SetMaxScore(int _MaxScore) { iMaxScore = _MaxScore; }
	int GetMaxScore() { assert (bMinLevel); return iMaxScore; }
	void SetBestMove(MoveNode * node) { bestNode = node; }
	MoveNode* GetBestMove() { 
		if (!bestNode)
			ChessAssert(); 
		return bestNode; }
	
	void SetChessBoardPtr(ChessBoard* _Ptr) 
	{ 
		ChessBoardInstance = _Ptr;
		move.SetChessBoard(_Ptr);
	}
	
private:
	MoveNode* bestNode;
	Chromosome* calcChromo;
	bool bLeafNode;
	bool bMinLevel;
	int iMinScore;
	int iMaxScore;
	bool bEvaluate;
	int BoardScore;
	ChessMove move;
	std::vector<MoveNode*> Children;
	ChessBoard* ChessBoardInstance;
	MoveNode* parent;
public:
	MoveNode* UnThreadedCopy;
	
};

#endif
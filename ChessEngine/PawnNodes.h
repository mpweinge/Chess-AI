/*
 *  PawnNodes.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PAWNNODES_H
#define PAWNNODES_H
#include "NeuralNode.h"
#include <vector>
#include "Constants.h"
#include "ChessBoard.h"
#include "NeuralInput.h"

class PawnAdvantage : public NeuralNode
{
public:
	PawnAdvantage();
	void Update(Pawn_Args* currPawns);
protected:
	void SetupNode() { }
	bool IsActivated() { return bActivated; }
	void TransmitToChildren()
	{
		assert(bActivated);
		for (int i = 0; i < Children.size(); i++)
		{
			(dynamic_cast<NeuralOutput*>(Children[i]))->AddToScore(Weight);
		}
	}
public:
	void SetWeight(int NewWeight) { Weight = NewWeight; }
private:
	bool bActivated;
	const static ENeuralType NodeType = PawnAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class CenterPawn : public NeuralNode
{
public:
	CenterPawn();
	void Update(Pawn_Args* currPawns);
protected:
	void SetupNode() { }
	bool IsActivated() { return bActivated; }
	void TransmitToChildren()
	{
		assert(bActivated);
		for (int i = 0; i < Children.size(); i++)
		{
			(dynamic_cast<NeuralOutput*>(Children[i]))->AddToScore(Weight);
		}
	}
public:
	void SetWeight(int NewWeight) { Weight = NewWeight; }
private:
	bool bActivated;
	const static enum ENeuralType NodeType = CenterPawnType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class DoublePawn : public NeuralNode
{
public:
	DoublePawn();
	void Update(Pawn_Args* currPawns);
protected:
	void SetupNode() { }
	bool IsActivated() { return bActivated; }
	void TransmitToChildren()
	{
		assert(bActivated);
		for (int i = 0; i < Children.size(); i++)
		{
			(dynamic_cast<NeuralOutput*>(Children[i]))->AddToScore(Weight);
		}
	}
public:
	void SetWeight(int NewWeight) { Weight = NewWeight; }
private:
	bool bActivated;
	const static ENeuralType NodeType = DoublePawnType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class IsolatedPawn : public NeuralNode
{
public:
	IsolatedPawn();
	void Update(Pawn_Args* currPawns);
protected:
	void SetupNode() { }
	bool IsActivated() { return bActivated; }
	void TransmitToChildren()
	{
		assert(bActivated);
		for (int i = 0; i < Children.size(); i++)
		{
			(dynamic_cast<NeuralOutput*>(Children[i]))->AddToScore(Weight);
		}
	}
public:
	void SetWeight(int NewWeight) { Weight = NewWeight; }
private:
	bool bActivated;
	const static enum ENeuralType NodeType = IsolatedPawnType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

#endif
/*
 *  BoardNodes.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BOARDNODES_H
#define BOARDNODES_H
#include "NeuralNode.h"
#include "Constants.h"
#include <vector>
#include "NeuralInput.h"

class Castle : public NeuralNode
{
public:
	Castle();
	
protected:
	void SetupNode() { Children.push_back(NeuralOutput::GetInstance() ); }
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
	const static ENeuralType NodeType = CastleType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class CenterAttack : public NeuralNode
{
public:
	CenterAttack();
	
protected:
	void SetupNode() { Children.push_back(NeuralOutput::GetInstance() ); }
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
	const static ENeuralType NodeType = CenterAttackType;
	bool bActivated;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class OpponentSquareAttack : public NeuralNode
{
public:
	OpponentSquareAttack();
	
protected:
	void SetupNode() { Children.push_back(NeuralOutput::GetInstance() ); }
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
	const static ENeuralType NodeType = OpponentSquareAttackType;
	std::vector<NeuralNode*> Children;
	int Weight;
};
#endif

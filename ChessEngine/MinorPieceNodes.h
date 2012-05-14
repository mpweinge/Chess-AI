/*
 *  MinorPieceNodes.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MINORPIECES_H
#define MINORPIECES_H

#include "NeuralNode.h"
#include "Constants.h"
#include <vector>
#include "NeuralInput.h"

class KnightAdvantage : public NeuralNode
{
public:
	KnightAdvantage();
	void Update(const Knight_Args* currKnights)
	{
		if (currKnights->WhiteKnights.size() > 
			currKnights->BlackKnights.size() )
			bActivated = true;
		else {
			bActivated = false;
		}
	}
	
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
	static const ENeuralType NodeType = KnightAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class KnightMobility : public NeuralNode
{
public:
	KnightMobility();
	void Update(const Knight_Args* currKnights)
	{
		int WhiteMobility = 0;
		int BlackMobility = 0;
		for (int i = 0; i < currKnights->WhiteKnights.size(); i++)
		{
			WhiteMobility += currKnights->WhiteKnights[i]->GetMobilityScore(NULL, false);
		}
		for (int i = 0; i < currKnights->BlackKnights.size(); i++)
		{
			BlackMobility += currKnights->BlackKnights[i]->GetMobilityScore(NULL, false);
		}
		bActivated = WhiteMobility > BlackMobility ? true : false;
	}

protected:
	void SetupNode(){ Children.push_back(NeuralOutput::GetInstance() ); }
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
	static const ENeuralType NodeType = KnightMobilityType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class KnightUnderAttack : public NeuralNode
{
public:
	KnightUnderAttack();
	void Update(const Knight_Args* currKnights)
	{
		int WhiteUnderAttack = 0;
		int BlackUnderAttack = 0;
		for (int i = 0; i < currKnights->WhiteKnights.size(); i++)
		{
			WhiteUnderAttack += currKnights->WhiteKnights[i]->GetPieceUnderAttackScore(NULL);
		}
		for (int i = 0; i < currKnights->BlackKnights.size(); i++)
		{
			BlackUnderAttack += currKnights->BlackKnights[i]->GetPieceUnderAttackScore(NULL);
		}
		bActivated = WhiteUnderAttack > BlackUnderAttack ? true : false;
	}
protected:
	void SetupNode(){ Children.push_back(NeuralOutput::GetInstance() ); }
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
	static const ENeuralType NodeType = KnightUnderAttackType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class BishopAdvantage : public NeuralNode
{
public:
	BishopAdvantage();
	void Update(const Bishop_Args* currBishops)
	{
		if (currBishops->WhiteBishops.size() > 
			currBishops->BlackBishops.size() )
			bActivated = true;
		else {
			bActivated = false;
		}
	}
protected:
	void SetupNode(){ Children.push_back(NeuralOutput::GetInstance() ); }
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
	static const ENeuralType NodeType = BishopAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class BishopUnderAttack : public NeuralNode
{
public:
	BishopUnderAttack();
	void Update(const Bishop_Args* currBishops)
	{
		int WhiteUnderAttack = 0;
		int BlackUnderAttack = 0;
		for (int i = 0; i < currBishops->WhiteBishops.size(); i++)
		{
			WhiteUnderAttack += currBishops->WhiteBishops[i]->GetPieceUnderAttackScore(NULL);
		}
		for (int i = 0; i < currBishops->BlackBishops.size(); i++)
		{
			BlackUnderAttack += currBishops->BlackBishops[i]->GetPieceUnderAttackScore(NULL);
		}
		bActivated = WhiteUnderAttack > BlackUnderAttack ? true : false;
	}
protected:
	void SetupNode(){ Children.push_back(NeuralOutput::GetInstance() ); }
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
	static const ENeuralType NodeType = BishopUnderAttackType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class BishopMobility : public NeuralNode
{
public:
	BishopMobility();
	void Update(const Bishop_Args* currBishops)
	{
		int WhiteMobility = 0;
		int BlackMobility = 0;
		for (int i = 0; i < currBishops->WhiteBishops.size(); i++)
		{
			WhiteMobility += currBishops->WhiteBishops[i]->GetMobilityScore(NULL, false);
		}
		for (int i = 0; i < currBishops->BlackBishops.size(); i++)
		{
			BlackMobility += currBishops->BlackBishops[i]->GetMobilityScore(NULL, false);
		}
		bActivated = WhiteMobility > BlackMobility ? true : false;
	}
	
protected:
	void SetupNode()
	{ 
		Children.push_back(NeuralOutput::GetInstance() ); 
	}
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
	static const ENeuralType NodeType = BishopMobilityType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class MinorPieceAdvantage : public NeuralNode
{
public:
	MinorPieceAdvantage();
	
protected:
	void SetupNode() 
	{ 
		Children.push_back(NeuralOutput::GetInstance() ); 
	}
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
	static const ENeuralType NodeType = MinorPieceAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class MinorPieceMobilityAdvantage : public NeuralNode
{
public:
	MinorPieceMobilityAdvantage();
	
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
	static const ENeuralType NodeType = MinorPieceMobilityAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class MinorPieceUnderAttackAdvantage : public NeuralNode
{
public:
	MinorPieceUnderAttackAdvantage();
	
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
	static const ENeuralType NodeType = MinorPieceUnderAttackAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class MinorPieceRoot : public NeuralNode
{
public:
	MinorPieceRoot() { }
protected:
	void SetupNode()
	{
		Children.push_back(new KnightAdvantage() );
		Children.push_back(new KnightMobility() );
		Children.push_back(new KnightUnderAttack() );
		
		Children.push_back(new BishopAdvantage() );
		Children.push_back(new BishopMobility() );
		Children.push_back(new BishopUnderAttack() );
	}
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
	std::vector<NeuralNode*> Children;
	int Weight;
};

#endif

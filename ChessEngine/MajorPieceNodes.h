/*
 *  MajorPieceNodes.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MAJORPIECENODE_H
#define MAJORPIECENODE_H

#include "NeuralNode.h"
#include "Constants.h"
#include <vector>
#include "NeuralInput.h"

class RookAdvantage : public NeuralNode
{
public:
	RookAdvantage();
	void Update(Rook_Args* currRooks)
	{
		if (currRooks->WhiteRooks.size() > 
			currRooks->BlackRooks.size() )
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
	static const ENeuralType NodeType = RookAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class RookUnderAttack : public NeuralNode
{
public:
	RookUnderAttack();
	void Update(Rook_Args* currRooks)
	{
		int WhiteUnderAttack = 0;
		int BlackUnderAttack = 0;
		for (int i = 0; i < currRooks->WhiteRooks.size(); i++)
		{
			WhiteUnderAttack += currRooks->WhiteRooks[i]->GetPieceUnderAttackScore(NULL);
		}
		for (int i = 0; i < currRooks->BlackRooks.size(); i++)
		{
			BlackUnderAttack += currRooks->BlackRooks[i]->GetPieceUnderAttackScore(NULL);
		}
		bActivated = WhiteUnderAttack > BlackUnderAttack ? true : false;
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
	static const ENeuralType NodeType = RookUnderAttackType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class RookMobility : public NeuralNode
{
public:
	RookMobility();
	void Update(Rook_Args* currRooks)
	{
		int WhiteMobility = 0;
		int BlackMobility = 0;
		for (int i = 0; i < currRooks->WhiteRooks.size(); i++)
		{
			WhiteMobility += currRooks->WhiteRooks[i]->GetMobilityScore(NULL, false);
		}
		for (int i = 0; i < currRooks->BlackRooks.size(); i++)
		{
			BlackMobility += currRooks->BlackRooks[i]->GetMobilityScore(NULL, false);
		}
		bActivated = WhiteMobility > BlackMobility ? true : false;
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
	static const ENeuralType NodeType = RookMobilityType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class QueenAdvantage : public NeuralNode
{
public:
	QueenAdvantage();
	void Update(Queen_Args* currQueens)
	{
		if (currQueens->WhiteQueens.size() > 
			currQueens->BlackQueens.size() )
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
	static const ENeuralType NodeType = QueenAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class QueenUnderAttack : public NeuralNode
{
public:
	QueenUnderAttack();
	void Update(Queen_Args* currQueens)
	{
		int WhiteUnderAttack = 0;
		int BlackUnderAttack = 0;
		for (int i = 0; i < currQueens->WhiteQueens.size(); i++)
		{
			WhiteUnderAttack += currQueens->WhiteQueens[i]->GetPieceUnderAttackScore(NULL);
		}
		for (int i = 0; i < currQueens->BlackQueens.size(); i++)
		{
			BlackUnderAttack += currQueens->BlackQueens[i]->GetPieceUnderAttackScore(NULL);
		}
		bActivated = WhiteUnderAttack > BlackUnderAttack ? true : false;
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
	static const ENeuralType NodeType = QueenUnderAttackType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class QueenMobility : public NeuralNode
{
public:
	QueenMobility();
	void Update(Queen_Args* currQueens)
	{
		int WhiteMobility = 0;
		int BlackMobility = 0;
		for (int i = 0; i < currQueens->WhiteQueens.size(); i++)
		{
			WhiteMobility += currQueens->WhiteQueens[i]->GetMobilityScore(NULL, false);
		}
		for (int i = 0; i < currQueens->BlackQueens.size(); i++)
		{
			BlackMobility += currQueens->BlackQueens[i]->GetMobilityScore(NULL, false);
		}
		bActivated = WhiteMobility > BlackMobility ? true : false;
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
	static const ENeuralType NodeType = QueenMobilityType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class RookOnSeventhRow : public NeuralNode
{
public:
	RookOnSeventhRow();
	void Update(Rook_Args* currRooks);
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
	static const ENeuralType NodeType = RookOnSeventhRowType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class BatteryAttack : public NeuralNode
{
public:
	BatteryAttack();
	void Update(Rook_Args* currRooks);
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
	static const ENeuralType NodeType = BatteryAttackType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class KingInCheck : public NeuralNode
{
public:
	KingInCheck();
	void Update(King_Args* currKings);
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
	static const ENeuralType NodeType = KingInCheckType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class KingMobility : public NeuralNode
{
public:
	KingMobility();
	void Update(King_Args* currKings)
	{
		int WhiteMobility = 0;
		int BlackMobility = 0;
		//for (int i = 0; i < currKings->WhiteKings.size(); i++)
		{
			WhiteMobility += currKings->WhiteKings->GetMobilityScore(NULL, false);
		}
		//for (int i = 0; i < currKings->BlackKings.size(); i++)
		{
			BlackMobility += currKings->BlackKings->GetMobilityScore(NULL, false);
		}
		bActivated = WhiteMobility > BlackMobility ? true : false;
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
	static const ENeuralType NodeType = KingMobilityType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class MajorPieceAdvantage : public NeuralNode
{
public:
	MajorPieceAdvantage();
	
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
	static const ENeuralType NodeType = MajorPieceAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class MajorPieceMobilityAdvantage : public NeuralNode
{
public:
	MajorPieceMobilityAdvantage();
	
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
	static const ENeuralType NodeType = MajorPieceMobilityAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class MajorPieceUnderAttackAdvantage : public NeuralNode
{
public:
	MajorPieceUnderAttackAdvantage();
	
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
	static const ENeuralType NodeType = MajorPieceUnderAttackAdvantageType;
	std::vector<NeuralNode*> Children;
	int Weight;
};

class MajorPieceRoot : public NeuralNode
{
public:
	MajorPieceRoot() { }
protected:
	void SetupNode()
	{
		Children.push_back(new RookAdvantage() );
		Children.push_back(new RookUnderAttack() );
		Children.push_back(new RookMobility() );
		Children.push_back(new QueenAdvantage() );
		Children.push_back(new QueenMobility() );
		Children.push_back(new QueenUnderAttack() );
		//Children.push_back(new RookOnSeventhRow() );
		Children.push_back(new BatteryAttack() );
		Children.push_back(new KingInCheck() );
		Children.push_back(new KingMobility() );
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
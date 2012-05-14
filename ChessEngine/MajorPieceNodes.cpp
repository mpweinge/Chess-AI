/*
 *  MajorPieceNodes.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "MajorPieceNodes.h"

RookAdvantage::RookAdvantage()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

RookUnderAttack::RookUnderAttack()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

RookMobility::RookMobility()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

QueenAdvantage::QueenAdvantage()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

QueenUnderAttack::QueenUnderAttack()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

QueenMobility::QueenMobility()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

RookOnSeventhRow::RookOnSeventhRow()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

BatteryAttack::BatteryAttack()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

KingInCheck::KingInCheck()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

KingMobility::KingMobility()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}


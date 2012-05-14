/*
 *  MinorPieceNodes.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "MinorPieceNodes.h"

KnightAdvantage::KnightAdvantage()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

KnightMobility::KnightMobility()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

KnightUnderAttack::KnightUnderAttack()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

BishopAdvantage::BishopAdvantage()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

BishopUnderAttack::BishopUnderAttack()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

BishopMobility::BishopMobility()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

MinorPieceAdvantage::MinorPieceAdvantage()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

MinorPieceMobilityAdvantage::MinorPieceMobilityAdvantage()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

MinorPieceUnderAttackAdvantage::MinorPieceUnderAttackAdvantage()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}


/*
 *  PawnNodes.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "PawnNodes.h"

PawnAdvantage::PawnAdvantage()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

CenterPawn::CenterPawn()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

DoublePawn::DoublePawn()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

IsolatedPawn::IsolatedPawn()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}
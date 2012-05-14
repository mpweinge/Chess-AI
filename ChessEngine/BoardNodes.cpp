/*
 *  BoardNodes.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-04-05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "BoardNodes.h"

Castle::Castle()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

CenterAttack::CenterAttack()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}

OpponentSquareAttack::OpponentSquareAttack()
{ 
	Weight = NeuralNodeWeightingTable::GetInstance()->GetWeighting(NodeType);
	bActivated = false;
}
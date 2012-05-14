/*
 *  NeuralNode.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-03-29.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "NeuralNode.h"

static NeuralNodeWeightingTable* m_TableInstance;

NeuralNodeWeightingTable* NeuralNodeWeightingTable::GetInstance()
{
	if (m_TableInstance == NULL)
	{
		m_TableInstance = new NeuralNodeWeightingTable();
	}
	return m_TableInstance;
}

static NeuralOutput* m_OutputInstance;

NeuralOutput* NeuralOutput::GetInstance()
{
	if (m_OutputInstance == NULL)
	{
		m_OutputInstance = new NeuralOutput();
	}
	return m_OutputInstance;
}




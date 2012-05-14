/*
 *  NeuralNode.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-03-29.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#ifndef NEURALNODE_H
#define NEURALNODE_H
//The structure for a neural node
#include "Constants.h"
#include "NeuralInput.h"

class NeuralNode
{
	//No parent pointer, forward transmit only
	//virtual void ActivationFunction( *std::vector<Input_Args>);
public:
	virtual void Update(Input_Args* args) { }
	//Setup  aka adding children
	virtual void SetupNode() { };
	//Activation function
	//virtual bool IsActivated() = 0;
	//If activated, transmit to children
	virtual void TransmitToChildren() { ChessAssert();}
public:
	//Weight aka how good is this
	//virtual void SetWeight(int NewWeight) = 0;
};

class NeuralNodeWeightingTable
{
public:
	static NeuralNodeWeightingTable* GetInstance();
	void SetWeighting(ENeuralType index, double newWeighting)
	{
		WEIGHTINGS[(int)index] = newWeighting;
	}
	double GetWeighting(ENeuralType index) { return WEIGHTINGS[(int)index]; }
private:
	double WEIGHTINGS[NUM_NEURAL_NODES];
};

class NeuralOutput : public NeuralNode
{
protected:
	void SetupNode() { }
	bool IsActivated() { return false;}
	void TransmitToChildren() { }
public:
	NeuralOutput() { }
	static NeuralOutput* GetInstance();
	void ResetScore() { score = 0; }
	void AddToScore(int Addition) { score += Addition; }
	int GetScore() { return score; }
public:
	void SetWeight(int NewWeight) { }
private:
	int score;
};
#endif


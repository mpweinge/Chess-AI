/*
 *  MoveNode.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-02-12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "MoveNode.h"

MoveNode::MoveNode(const ChessMove& GuessMove, MoveNode* _parent, bool _bEvaluate, bool _MinLevel, Chromosome* _calcChromo, ChessBoard* _ChessBoardInstance)
{ 
	//move = ChessMove(_ChessBoardInstance);
	move = GuessMove; 
	BoardScore = -1; 
	parent = _parent; 
	bEvaluate = _bEvaluate;
	if(!_calcChromo)
		ChessAssert();
	calcChromo = _calcChromo;
	if (bEvaluate)
		CalculateScore(); 
	bLeafNode = false;
	bMinLevel = _MinLevel;
	bestNode = NULL;
	iMinScore = 0x7FFFFFFF;
	iMaxScore = -1000000;
	ChessBoardInstance = _ChessBoardInstance;
	UnThreadedCopy = NULL;
}

void MoveNode::DeleteAllMemory()
{
	//calcChromo->Delete();
	//delete(calcChromo);
	calcChromo = NULL;
	
	assert(Children.size() == 0);
	//delete(Children);
	//Best node will delete itself
	bestNode = NULL;
	//Parent will delete itself
	parent = NULL;
}

void MoveNode::PrintNode()
{
	for (int i = 1; i < depth; i++)
		printf("\t");
	
	if (Children.size() == 0)
	{
		
		if (move.isPromotion() )
		{
			printf("ChessSquare: %c%c%i-%c%i%c, Score:%i\n", pieceToText(move.getPiece()), rowToLetter(move.getStartingRow()), move.getStartingCol()+1,
				   rowToLetter(move.getRow()), move.getCol()+1, move.GetPromotionChar(),
				   BoardScore);
		}
		else if (move.isCastle() )
		{
			if (move.isKingSideCastle() )
				printf("ChessSquare: O-O, Score:%i\n", BoardScore);
			else {
				printf("ChessSquare: O-O-O, Score:%i\n", BoardScore);
			}
			
		}
		else if (move.isCapture() )
		{
			printf("ChessSquare: %c%c%ix%c%c%i, Score:%i\n", pieceToText(move.getPiece()), rowToLetter(move.getStartingRow()), move.getStartingCol()+1, 
				   pieceToText(move.getCapturedPiece()), rowToLetter(move.getRow()), move.getCol()+1, BoardScore);
		}
		else
		{
			printf("ChessSquare: %c%c%i-%c%i, Score:%i\n", pieceToText(move.getPiece()), rowToLetter(move.getStartingRow()), move.getStartingCol()+1,
				   rowToLetter(move.getRow()), move.getCol()+1, BoardScore);
		}
		fflush(stdout);
		//if (!bLeafNode)
		//{
		//	printf("ChessSquare: %c%c%i-%c%i\n", pieceToText(move->getPiece()), rowToLetter(move->getStartingRow()), move->getStartingCol()+1, 
		//		   rowToLetter(move->getRow()), move->getCol()+1);
		//}
		//else {
		//	printf("ChessSquare: %c%c%i-%c%i Score:%i\n", pieceToText(move->getPiece()), rowToLetter(move->getStartingRow()), move->getStartingCol()+1, 
		//		   rowToLetter(move->getRow()), move->getCol()+1, BoardScore);

		//}

	}
	else {
		if (move.isPromotion() )
		{
			printf("ChessSquare: %c%c%i-%c%i%c, Score:%i\n", pieceToText(move.getPiece()), rowToLetter(move.getStartingRow()), move.getStartingCol()+1,
				   rowToLetter(move.getRow()), move.getCol()+1, move.GetPromotionChar(),
				   BoardScore);
		}
		else if (move.isCastle() )
		{
			if (move.isKingSideCastle() )
				printf("ChessSquare: O-O, Score:%i\n", BoardScore);
			else {
				printf("ChessSquare: O-O-O, Score:%i\n", BoardScore);
			}
			
		}
		else if (move.isCapture() )
		{
			printf("ChessSquare: P%c%ix%c%i, Score:%i\n", rowToLetter(move.getStartingRow()), move.getStartingCol()+1, rowToLetter(move.getRow()), move.getCol()+1,
				   BoardScore);
		}
		else
		{
			printf("ChessSquare: %c%c%i-%c%i, Score:%i\n", pieceToText(move.getPiece()), rowToLetter(move.getStartingRow()), move.getStartingCol()+1,
				   rowToLetter(move.getRow()), move.getCol()+1, BoardScore);
		}
		
		for (int i = 0; i < Children.size(); i++)
		{
			Children[i]->PrintNode();
		}
	}

}
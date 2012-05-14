/*
 *  ChessLog.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ChessLog.h"

static ChessLog* m_Instance;

ChessLog::ChessLog()
{
	MoveNum = 0;
}

ChessLog* ChessLog::Copy()
{
	ChessLog* returnLog = new ChessLog();
	for (int i = 0; i < MoveNum; i++)
	{
		returnLog->addMoveToLog(Log[i]->Copy());
	}
	return returnLog;
}

ChessLog* ChessLog::GetInstance()
{
	if (m_Instance == NULL)
		m_Instance = new ChessLog();
	return m_Instance;
}

void ChessLog::SetChessBoard(ChessBoard* Instance)
{
	for (int i = 0; i < MoveNum; i++)
	{
		Log[i]->SetChessBoard(Instance);
	}
}

void ChessLog::printLog()
{
	char outText;
	
	printf("White | Black\n1:");
	
	for (int i = 0; i < MoveNum; i++)
	{
		if (Log[i]->isCastle())
		{
			if ( Log[i]->isKingSideCastle() )
			{
				printf("O-O   |");
			}
			else {
				printf("O-O-O |");
			}
			if (i&1 == 1)
				printf("\n%i:", (i/2+2));
			continue;
		}
		
		
		pieceToText(Log[i]->getPiece(), outText);
		
		char CheckMove = ' ';
		
		if( Log[i]->IsCheckMove() )
		{
			CheckMove = '+';
		}
		
		if ( !(Log[i]->isCapture() ) )
		{
			printf("%c%c%i-%c%i%c", outText, rowToLetter(Log[i]->getStartingRow()), Log[i]->getStartingCol()+1,
				   rowToLetter(Log[i]->getRow()), Log[i]->getCol()+1, CheckMove);
			if (Log[i]->isPromotion())
			{
				printf("%c|", Log[i]->GetPromotionChar() );
			}
			else {
				printf("|");
			}

		}
		else {
			char outText2;
			pieceToText(Log[i]->getCapturedPiece() , outText2);
			printf("%c%c%ix%c%c%i%c", outText, rowToLetter(Log[i]->getStartingRow()), Log[i]->getStartingCol()+1,
				   outText2, rowToLetter(Log[i]->getRow()), Log[i]->getCol()+1, CheckMove);
			if (Log[i]->isPromotion())
			{
				printf("%c|", Log[i]->GetPromotionChar() );
			}
			else {
				printf("|");
			}
		}
		
		if (i&1 == 1)
			printf("\n%i:", (i/2+2));

	}
}

void ChessLog::addMoveToLog(ChessMove* move)
{
	Log[MoveNum] = move;
	MoveNum++;
	;
}

void ChessLog::ResetLog()
{
	for (int i = 0; i < MoveNum; i++)
	{
		Log[i]->Delete();
		delete Log[i];
	}
	MoveNum = 0;
}



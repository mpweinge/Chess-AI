/*
 *  ChessLog.h
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-01-25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHESSLOG_H
#define CHESSLOG_H

#include "ChessMove.h"
#include "ChessPlayer.h"
#include "Constants.h"

class ChessLog
{
private:
	ChessLog();
	ChessMove* Log[MAX_LOG_SIZE];
	int MoveNum;
public:
	~ChessLog() { assert(Log[0] == NULL); }
	void Erase()
	{
		for (int i = 0; i < MoveNum; i++)
		{
			Log[i]->Delete(); 
#ifdef PRINT_MEMORY_MOVES
			printf("Deleting move at: %i\n", Log[i]);
			fflush(stdout);
#endif
			delete Log[i];
			Log[i] = NULL;
		}
	}
	static ChessLog* GetInstance();
	ChessLog* getLog();
	void printLog();
	void addMoveToLog(ChessMove* move);
	void removeLastItemFromLog() 
	{ 
		Log[MoveNum-1]->Delete(); 
#ifdef PRINT_MEMORY_MOVES
		printf("Deleting move at: %i\n", Log[MoveNum-1]);
		fflush(stdout);
#endif
		delete Log[MoveNum-1]; 
		Log[MoveNum-1] = NULL;
		MoveNum--; 
	}
	ChessMove* getIndexedItem(int index) 
	{ 
		if( index >= MoveNum)
			return NULL;
		return Log[index];
	}
	int GetNumItemsInLog() { return MoveNum; }
	ChessMove* getLastMove() { return Log[MoveNum-1]; }
	void ResetLog();
	ChessLog* Copy();
	void SetChessBoard(ChessBoard* Instance);
};

#endif

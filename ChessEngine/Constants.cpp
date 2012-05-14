/*
 *  Constants.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-03-28.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#include "Constants.h"

#include "ChessBoard.h"
#include "ChessLog.h"
#include <execinfo.h>

void ChessAssert()
{
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = 0; i < frames; ++i) {
		printf("%s\n", strs[i]);
	}
	free(strs);
	ChessBoard::GetInstance()->PrintBoard();
	ChessLog::GetInstance()->printLog();
	fflush(stdout);
	assert(0);
}
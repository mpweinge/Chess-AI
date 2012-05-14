/*
 *  ObjectiveCWrapper.h
 *  ChessUI
 *
 *  Created by Michael Weingert on 12-05-06.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef MYOBJCWRAPPER_H
#define MYOBJCWRAPPER_H 1

void ObjCHighightSquare(int row, int col);

void ObjCMovePiece(int StartingRow, int StartingCol, int row, int col);

void UpdateBoard();

#endif
//
//  DrawingQueues.m
//  ChessUI
//
//  Created by Michael Weingert on 12-05-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "DrawingQueues.h"


@implementation DrawingQueues

@end

@implementation GLChessPiece
@synthesize _PieceChar;
@synthesize row;
@synthesize col;
@synthesize bIsWhite;
-(id)initWithParameters:(int)_row:(int)_col:(char)__PieceChar:(bool)_bIsWhite
{
	_PieceChar = __PieceChar;
	row = _row;
	col = _col;
	bIsWhite = _bIsWhite;
	[super init];
	return self;
}

-(id)init
{
	row = -1;
	col = -1;
	[super init];
	return self;
}
@end

@implementation PieceQueue

static PieceQueue* m_QueueInstance = nil;
-(id)init
{
	numPieces = 0;
	
	[super init];
	return self;
}

+ (PieceQueue *) getInstance
{
	if (m_QueueInstance == nil)
		m_QueueInstance = [[self alloc] init];
	
	return m_QueueInstance;
}

-(id)retain
{
	return self;
}

-(NSUInteger) retainCount
{
	return NSUIntegerMax;
}

-(void)release
{
	//do nothing
}

-(id)autorelease
{
	return self;
}

-(void)AddPieceToQueue:(GLChessPiece *)newPiece
{
	LocalPieceQueue[numPieces] = newPiece;
	numPieces++;
	if (numPieces > 35)
		assert(0);
	
	//printf("Adding piece to queue");
	//fflush(stdout);
}
-(void)ResetQueue
{
	for (int i = 0; i < numPieces; i++)
	{
		[LocalPieceQueue[i] release];
	}
	numPieces = 0;
}

-(GLChessPiece*)GetPiece:(int)index
{
	return LocalPieceQueue[index];
}

-(int)GetNumPieces
{
	return numPieces;
}
@end

//
//  DrawingQueues.h
//  ChessUI
//
//  Created by Michael Weingert on 12-05-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface DrawingQueues : NSObject {

}

@end

@interface GLChessPiece : NSObject
{
	char _PieceChar;
	int row;
	int col;
	bool bIsWhite;
}

@property (readonly) char _PieceChar;
@property (readonly) int row;
@property (readonly) int col;
@property (readonly) bool bIsWhite;
-(id)init;
-(id)initWithParameters:(int)_row:(int)_col:(char) __PieceChar:(bool)_bIsWhite;
@end

@interface PieceQueue : NSObject
{
	GLChessPiece* LocalPieceQueue[35];
	int numPieces;
}
-(id)init;
+(PieceQueue*)getInstance;
-(void)AddPieceToQueue: (GLChessPiece*)newPiece;
-(GLChessPiece*)GetPiece:(int)index;
-(void)ResetQueue;
-(int)GetNumPieces;
-(id)retain;

-(NSUInteger) retainCount;

-(void)release;

-(id)autorelease;

@end
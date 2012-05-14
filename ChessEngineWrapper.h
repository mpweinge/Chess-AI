//
//  ChessEngineWrapper.h
//  
//
//  Created by Michael Weingert on 12-05-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "DrawingQueues.h"
#import "ObjectiveCWrapper.h"
#import "MyOpenGLView.h"

// declare c++ implementation for .mm (Obj-C++) files
#ifdef __cplusplus
class CImplement;
#endif

// declare c++ implementation for .m (Obj-C) files
#ifdef __OBJC__
#ifndef __cplusplus
typedef void CImplement;
#endif
#endif

@interface ChessEngineWrapper : NSObject <NSApplicationDelegate>{
	CImplement* mImplement;
	NSTextFieldCell* LocalMoveList;
	MyOpenGLView* OpenGLView;
	NSButtonCell* GenerateMoveButton;
	NSButtonCell* PlayMatchButton;
	NSButtonCell* PlayAgainstCPUButton;
	bool PlayAgainstCPU;
}

struct CocoaMatchStruct
{
	CImplement* mImplement;
};

//@property (assign) IBOutlet NSMatrix* SudokuWindow2;

+(ChessEngineWrapper*)getInstance;
- (id)init;
- (void)GetLegalLocations:(int)x: (int)y;
- (void)TestLegalMove: (int)x: (int) y;
- (void)UpdateBoard;
- (void)GenerateTree;
- (void)SetMoveList: (NSTextFieldCell*) _MoveList;
- (void)UpdateMoveList;
- (void)PlayMatch;
- (void)ResetBoard;
- (void)SetOpenGLView : (MyOpenGLView*) _OpenGLView;
- (void)UpdateGL;
- (void)SelectorMethod;
- (void)SetButtons: (NSButtonCell*)_GenerateMoveButton : (NSButtonCell*)_PlayMatchButton : (NSButtonCell*) _PlayAgainstCPUButton;
- (void)UpdateButtons;
- (void)SetPlayAgainstCPU: (bool)_Play;

@end


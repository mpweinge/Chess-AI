//
//  ChessUIAppDelegate.m
//  ChessUI
//
//  Created by Michael Weingert on 12-01-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "ChessUIAppDelegate.h"
#import "ChessEngineWrapper.h"

@implementation ChessUIAppDelegate

@synthesize window, NewGameButton, MoveList, openGLView, AIGameButton, ResetBoard, PlayAgainstCPUButton;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application 
	[NewGameButton setTitle:@"Make AI Move"];
	[NewGameButton setAction: @selector(myButtonClick:)];
	[AIGameButton setTitle:@"Play Match"];
	[AIGameButton setAction: @selector(AIGameButtonClick:)];
	[ResetBoard setTitle:@"Reset Board"];
	[ResetBoard setAction: @selector(ResetBoardButtonClick:)];
	[PlayAgainstCPUButton setTitle:@"Play CPU?"];
	[PlayAgainstCPUButton setAction: @selector(PlayAgainstCPUButtonClick:)];
	//[NewGameButton addTarget:self action:@selector(myButtonClick:) 
	//		forControlEvents:(UIControlEvents)UIControlEventTouchDown];
	[MoveList setStringValue: @"Welcome to Mikey's Chess Game :)"];
	[MoveList setEnabled:FALSE];
	[MoveList setEditable:FALSE];
	[[ChessEngineWrapper getInstance]SetMoveList:MoveList];
	[[ChessEngineWrapper getInstance]UpdateMoveList];
	[[ChessEngineWrapper getInstance]SetOpenGLView:openGLView];
	[[ChessEngineWrapper getInstance]SetButtons:NewGameButton : AIGameButton : PlayAgainstCPUButton];
}


-(IBAction) myButtonClick: (id)sender 
{
	[sender setEnabled:NO];
	[[ChessEngineWrapper getInstance]GenerateTree];
	[openGLView setNeedsDisplay:true];
}

-(IBAction) AIGameButtonClick: (id)sender
{
	[sender setEnabled: NO];
	//[[ChessEngineWrapper getInstance]ResetBoard];
	[[ChessEngineWrapper getInstance]PlayMatch];
	[openGLView setNeedsDisplay:true];
	//[sender setEnabled:YES];
}

-(IBAction) ResetBoardButtonClick: (id)sender
{
	[[ChessEngineWrapper getInstance]ResetBoard];
	[[PieceQueue getInstance]ResetQueue];
	[[ChessEngineWrapper getInstance]UpdateBoard];
	[openGLView setNeedsDisplay:true];
}

-(IBAction) PlayAgainstCPUButtonClick: (id)sender
{
	[[ChessEngineWrapper getInstance]SetPlayAgainstCPU: true];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}
@end

//
//  ChessUIAppDelegate.h
//  ChessUI
//
//  Created by Michael Weingert on 12-01-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MyOpenGLView.h"

@interface ChessUIAppDelegate : NSObject <NSApplicationDelegate> {
    IBOutlet NSWindow *window;
	IBOutlet NSButtonCell *NewGameButton;
	IBOutlet NSButtonCell *AIGameButton;
	IBOutlet NSButtonCell *ResetBoard;
	IBOutlet NSButtonCell *PlayAgainstCPUButton;
	IBOutlet NSTextFieldCell *MoveList;
	IBOutlet MyOpenGLView* openGLView;
}
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;

@property (assign) IBOutlet NSWindow *window;

@property (assign) IBOutlet NSButtonCell *NewGameButton;
-(IBAction) myButtonClick: (id)sender;

@property (assign) IBOutlet NSButtonCell* AIGameButton;

@property (assign) IBOutlet NSTextFieldCell *MoveList;

@property (assign) IBOutlet MyOpenGLView* openGLView;

@property (assign) IBOutlet NSButtonCell* ResetBoard;

@property (assign) IBOutlet NSButtonCell* PlayAgainstCPUButton;


@end

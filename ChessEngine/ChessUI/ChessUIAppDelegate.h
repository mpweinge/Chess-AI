//
//  ChessUIAppDelegate.h
//  ChessUI
//
//  Created by Michael Weingert on 12-01-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ChessUIAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end

//
//  MyOpenGLView.h
//  ChessUI
//
//  Created by Michael Weingert on 12-01-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CommonInclude.h"
#import "ObjectiveCWrapper.h"
#import "DrawingQueues.h"

typedef struct {
    recVec viewPos; // View position
    recVec viewDir; // View direction vector
    recVec viewUp; // View up direction
    recVec rotPoint; // Point to rotate about
    GLdouble aperture; // pContextInfo->camera aperture
    GLint viewWidth, viewHeight; // current window/screen height and width
} recCamera;

@interface SquareDrawQueue : NSObject
{
	int DrawQueue[64][2];
	int numDrawItems;
}
+(SquareDrawQueue*)getInstance;
-(id)init;
-(void)AddItemToQueue : (int)row: (int)col;
-(void)PopBack;
-(int)GetNumItems;
-(int)GetRow: (int) index;
-(int)GetCol: (int) index;
-(void)Clear;

@end

@interface MyOpenGLView : NSOpenGLView {
	
    // spin
    GLfloat rRot [3];
    GLfloat rVel [3];
    GLfloat rAccel [3];
    
    // camera handling
    recCamera camera;
    GLfloat worldRotation [4];
    GLfloat objectRotation [4];
    GLfloat shapeSize;
}
-(void) drawRect: (NSRect) bounds;

//Update methods
- (void) prepareOpenGL;
//- (void) update;        // moved or resized

- (BOOL) acceptsFirstResponder;
- (BOOL) becomeFirstResponder;
- (BOOL) resignFirstResponder;

//Initialization
- (id) initWithFrame: (NSRect) frameRect;

/*Event Handlers*/
- (void) mouseDown:(NSEvent *)theEvent;
- (void) rightMouseDown:(NSEvent *)theEvent;
- (void) otherMouseDown:(NSEvent *)theEvent;
- (void) mouseUp:(NSEvent *)theEvent;
- (void) rightMouseUp:(NSEvent *)theEvent;
- (void) otherMouseUp:(NSEvent *)theEvent;
- (void) mouseDragged:(NSEvent *)theEvent;
- (void) scrollWheel:(NSEvent *)theEvent;
- (void) rightMouseDragged:(NSEvent *)theEvent;
- (void) otherMouseDragged:(NSEvent *)theEvent;

+ (NSOpenGLPixelFormat*) basicPixelFormat;

- (void) updateProjection;
- (void) updateModelView;
- (void) resizeGL;
- (void) resetCamera;
- (void) mousePan: (NSPoint) location;
- (void) mouseDolly: (NSPoint) location;

@end

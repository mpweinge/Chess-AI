//
//  MyOpenGLView.m
//  ChessUI
//
//  Created by Michael Weingert on 12-01-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MyOpenGLView.h"
#include "Trackball.h"
#include "3DPieces.h"
#include "Collision.h"
#import "ChessEngineWrapper.h"


@implementation SquareDrawQueue

static SquareDrawQueue* m_QueueInstance;
-(id)init
{
	numDrawItems = 0;
	
	[super init];
	return self;
}

+ (SquareDrawQueue *) getInstance
{
	if (m_QueueInstance == NULL)
		m_QueueInstance = [[self alloc] init];
	
	return m_QueueInstance;
}

-(void)AddItemToQueue: (int)row : (int)col
{
	DrawQueue[numDrawItems][0] = row;
	DrawQueue[numDrawItems][1] = col;
	numDrawItems++;
}

-(void)PopBack
{
	numDrawItems--;
}

-(void)Clear
{
	numDrawItems = 0;
}

-(int)GetNumItems
{
	return numDrawItems;
}

-(int)GetRow: (int) index
{
	assert(index < numDrawItems);
	return DrawQueue[index][0];
}
-(int)GetCol: (int) index
{
	assert(index < numDrawItems);
	return DrawQueue[index][1];
}
@end

@implementation MyOpenGLView

MyOpenGLView * gTrackingViewInfo = NULL;

GLboolean gDolly = GL_FALSE;
GLboolean gPan = GL_FALSE;
GLboolean gTrackball = GL_FALSE;

GLfloat gTrackBallRotation [4] = {0.0f, 0.0f, 0.0f, 0.0f};
GLint gDollyPanStartPoint[2] = {0, 0};
recVec gOrigin = {0.0, 0.0, 0.0};

recVec gRayCastLocation = {0.0, 0.0, 0.0};

GLfloat cube_vertices [8][3] = {
	{1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
	{1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };

GLfloat cube_vertex_colors [8][3] = {
	{1.0, 1.0, 1.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
	{1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0} };

GLint num_faces = 6;

short cube_faces [6][4] = {
	{3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };

void drawRook2D(double x, double y);
void drawKnight2D(double x, double y);
void drawBishop2D(double x, double y);
void drawQueen2D(double x, double y);
void drawPawn2D(double x, double y);
void drawKing2D(double x, double y);

recVec SphereDrawQueue[10];

// set initial OpenGL state (current context is set)
// called after context is created
- (void) prepareOpenGL
{
    //long swapInt = 1;
	
   // [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; // set to vbl sync
	
    // init GL stuff here
    glEnable(GL_DEPTH_TEST);
	
    glShadeModel(GL_SMOOTH);    
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);
    glPolygonOffset (1.0f, 1.0f);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	recVec tempVector = {-1, -1, -1};
	SphereDrawQueue[0] = tempVector;
    [self resetCamera];
    shapeSize = 7.0f; // max radius of of objects
	
    // init fonts for use with strings
    //NSFont * font =[NSFont fontWithName:@"Helvetica" size:12.0];
   // stanStringAttrib = [[NSMutableDictionary dictionary] retain];
    //[stanStringAttrib setObject:font forKey:NSFontAttributeName];
    //[stanStringAttrib setObject:[NSColor whiteColor] forKey:NSForegroundColorAttributeName];
    //[font release];
    
    // ensure strings are created
    //
	
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

// ---------------------------------

- (BOOL)becomeFirstResponder
{
	return  YES;
}

// ---------------------------------

- (BOOL)resignFirstResponder
{
	return YES;
}
// pixel format definition
+ (NSOpenGLPixelFormat*) basicPixelFormat
{
    NSOpenGLPixelFormatAttribute attributes [] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFADoubleBuffer,    // double buffered
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)16, // 16 bit depth buffer
        (NSOpenGLPixelFormatAttribute)nil
    };
    return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

-(id) initWithFrame: (NSRect) frameRect
{
    NSOpenGLPixelFormat * pf = [MyOpenGLView basicPixelFormat];
	
    self = [super initWithFrame: frameRect pixelFormat: pf];
    return self;
}


//Trackball, or normal click
- (void)mouseDown:(NSEvent *)theEvent // trackball
{
    if ([theEvent modifierFlags] & NSControlKeyMask) // send to pan
        [self rightMouseDown:theEvent];
    else if ([theEvent modifierFlags] & NSAlternateKeyMask) // send to dolly
        [self otherMouseDown:theEvent];
    else {
        NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
        location.y = camera.viewHeight - location.y;
        gDolly = GL_FALSE; // no dolly
        gPan = GL_FALSE; // no pan
        gTrackball = GL_TRUE;
        startTrackball (location.x, location.y, 0, 0, camera.viewWidth, camera.viewHeight);
        gTrackingViewInfo = self;
		recVec nearVector;
		recVec collisionVector = CastRay(location.x, location.y, camera.viewPos, &nearVector);
		SphereDrawQueue[0].x = collisionVector.x;
		SphereDrawQueue[0].y = collisionVector.y;
		SphereDrawQueue[0].z = collisionVector.z;
		SphereDrawQueue[1].x = nearVector.x;
		SphereDrawQueue[1].y = nearVector.y;
		SphereDrawQueue[1].z = nearVector.z;
		
		recVec nearPoint = SphereDrawQueue[1];
		float scalingFactor = -nearPoint.z / SphereDrawQueue[0].z;
		//drawSphere(nearPoint.x + SphereDrawQueue[0].x*scalingFactor, nearPoint.y + SphereDrawQueue[0].y*scalingFactor, nearPoint.z + SphereDrawQueue[0].z*scalingFactor, 1.0, 10, 10);
		
		double xPos = (nearPoint.x + SphereDrawQueue[0].x*scalingFactor);
		double yPos = (nearPoint.y + SphereDrawQueue[0].y*scalingFactor);
		
		[[SquareDrawQueue getInstance]Clear];
		
		//Round up to nearest int
		xPos = xPos / 3.0 + 4;
		yPos = yPos / 3.0;
		
		int intXPos = (int)xPos;
		int intYPos = (int)yPos;
		
		if ((xPos - intXPos) > 0.5)
			intXPos++;
		else if ((xPos - intXPos) < -0.5)
			intXPos--;
		
		if ((yPos - intYPos) > 0.5)
			intYPos++;
		else if ((yPos - intYPos) < -0.5)
			intXPos--;
		
		if (intXPos >= 0 && intXPos < 8 && intYPos >= 0 && intYPos < 8)
		{
			
			[[ChessEngineWrapper getInstance] GetLegalLocations : 
														 intXPos: intYPos];
			
			[[ChessEngineWrapper getInstance] TestLegalMove :
													 intXPos: intYPos];
		
			[[PieceQueue getInstance]ResetQueue];
			[[ChessEngineWrapper getInstance]UpdateBoard];
			[[ChessEngineWrapper getInstance]UpdateMoveList];
		}
				
		[self drawRect: [self bounds] ];
    }
}

//Pan
- (void)rightMouseDown:(NSEvent *)theEvent // pan
{
    NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    location.y = camera.viewHeight - location.y;
    if (gTrackball) { // if we are currently tracking, end trackball
        if (gTrackBallRotation[0] != 0.0)
            addToRotationTrackball (gTrackBallRotation, worldRotation);
        gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
    }
    gDolly = GL_FALSE; // no dolly
    gPan = GL_TRUE; 
    gTrackball = GL_FALSE; // no trackball
    gDollyPanStartPoint[0] = location.x;
    gDollyPanStartPoint[1] = location.y;
    gTrackingViewInfo = self;
}

- (void)otherMouseDown:(NSEvent *)theEvent //dolly
{
    NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    location.y = camera.viewHeight - location.y;
    if (gTrackball) { // if we are currently tracking, end trackball
        if (gTrackBallRotation[0] != 0.0)
            addToRotationTrackball (gTrackBallRotation, worldRotation);
        gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
    }
    gDolly = GL_TRUE;
    gPan = GL_FALSE; // no pan
    gTrackball = GL_FALSE; // no trackball
    gDollyPanStartPoint[0] = location.x;
    gDollyPanStartPoint[1] = location.y;
    gTrackingViewInfo = self;
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    location.y = camera.viewHeight - location.y;
    if (gTrackball) {
        rollToTrackball (location.x, location.y, gTrackBallRotation);
        [self setNeedsDisplay: YES];
    } else if (gDolly) {
        [self mouseDolly: location];
        [self updateProjection];  // update projection matrix (not normally done on draw)
        [self setNeedsDisplay: YES];
    } else if (gPan) {
        [self mousePan: location];
        [self setNeedsDisplay: YES];
    }
}

- (void)scrollWheel:(NSEvent *)theEvent
{
    float wheelDelta = [theEvent deltaX] +[theEvent deltaY] + [theEvent deltaZ];
    if (wheelDelta)
    {
        GLfloat deltaAperture = wheelDelta * -camera.aperture / 200.0f;
        camera.aperture += deltaAperture;
        if (camera.aperture < 0.1) // do not let aperture <= 0.1
            camera.aperture = 0.1;
        if (camera.aperture > 179.9) // do not let aperture >= 180
            camera.aperture = 179.9;
        [self updateProjection]; // update projection matrix
        [self setNeedsDisplay: YES];
    }
}

- (void) updateProjection
{
    GLdouble ratio, radians, wd2;
    GLdouble left, right, top, bottom, near, far;
	
    [[self openGLContext] makeCurrentContext];
	
    // set projection
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    near = -camera.viewPos.z - shapeSize * 0.5;
    if (near < 0.00001)
        near = 0.00001;
    far = -camera.viewPos.z + shapeSize * 0.5;
    if (far < 1.0)
        far = 1.0;
    radians = 0.0174532925 * camera.aperture / 2; // half aperture degrees to radians 
    wd2 = near * tan(radians);
    ratio = camera.viewWidth / (float) camera.viewHeight;
    if (ratio >= 1.0) {
        left  = -ratio * wd2;
        right = ratio * wd2;
        top = wd2;
        bottom = -wd2;  
    } else {
        left  = -wd2;
        right = wd2;
        top = wd2 / ratio;
        bottom = -wd2 / ratio;  
    }
	//glOrtho (-10, 0, -20, 20, 6, 50);
    glFrustum (left, right, bottom, top, 1, 9999);
	//glFrustum (left, right, bottom, top, near, far);
	glMatrixMode(GL_MODELVIEW);
    //[self updateCameraString];
}

// updates the contexts model view matrix for object and camera moves
- (void) updateModelView
{
    [[self openGLContext] makeCurrentContext];
    
    // move view
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (camera.viewPos.x, camera.viewPos.y, camera.viewPos.z,
               camera.viewPos.x + camera.viewDir.x,
               camera.viewPos.y + camera.viewDir.y,
               camera.viewPos.z + camera.viewDir.z,
               camera.viewUp.x, camera.viewUp.y ,camera.viewUp.z);
	
    // if we have trackball rotation to map (this IS the test I want as it can be explicitly 0.0f)
    //if ((gTrackingViewInfo == self) && gTrackBallRotation[0] != 0.0f) 
	if (gTrackBallRotation[0] != 0.0f)
        glRotatef (gTrackBallRotation[0], gTrackBallRotation[1], gTrackBallRotation[2], gTrackBallRotation[3]);
    else {
    }
    // accumlated world rotation via trackball
    glRotatef (worldRotation[0], worldRotation[1], worldRotation[2], worldRotation[3]);
    // object itself rotating applied after camera rotation
    glRotatef (objectRotation[0], objectRotation[1], objectRotation[2], objectRotation[3]);
    rRot[0] = 0.0f; // reset animation rotations (do in all cases to prevent rotating while moving with trackball)
    rRot[1] = 0.0f;
    rRot[2] = 0.0f;
    //[self updateCameraString];
}

// move camera in z axis
-(void)mouseDolly: (NSPoint) location
{
    GLfloat dolly = (gDollyPanStartPoint[1] -location.y) * -camera.viewPos.z / 300.0f;
    camera.viewPos.z += dolly;
    if (camera.viewPos.z == 0.0) // do not let z = 0.0
        camera.viewPos.z = 0.0001;
    gDollyPanStartPoint[0] = location.x;
    gDollyPanStartPoint[1] = location.y;
}

// ---------------------------------

// move camera in x/y plane
- (void)mousePan: (NSPoint) location
{
    GLfloat panX = (gDollyPanStartPoint[0] - location.x) / (900.0f / -camera.viewPos.z);
    GLfloat panY = (gDollyPanStartPoint[1] - location.y) / (900.0f / -camera.viewPos.z);
    camera.viewPos.x -= panX;
    camera.viewPos.y -= panY;
    gDollyPanStartPoint[0] = location.x;
    gDollyPanStartPoint[1] = location.y;
}

static void drawCube (GLfloat fSize)
{
	//return;
    long f, i;
    if (1) {
        glColor3f (1.0, 0.5, 0.0);
        glBegin (GL_QUADS);
        for (f = 0; f < num_faces; f++)
            for (i = 0; i < 4; i++) {
                glColor3f (cube_vertex_colors[cube_faces[f][i]][0], cube_vertex_colors[cube_faces[f][i]][1], cube_vertex_colors[cube_faces[f][i]][2]);
                glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
            }
        glEnd ();
    }
    if (1) {
        glColor3f (0.0, 0.0, 0.0);
        for (f = 0; f < num_faces; f++) {
            glBegin (GL_LINE_LOOP);
			for (i = 0; i < 4; i++)
				glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
            glEnd ();
        }
    }
}

static void drawAnObject()
{
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 12, 0, 12,-1,1);
	glMatrixMode(GL_MODELVIEW);*/
	//GLint nViewport[4];
	//glGetIntegerv(GL_VIEWPORT, nViewport);
	//int ret = glGetError();
	
	double ScalingFactor = 1.5;
	//glHint(GL_CLIP_VOLUME_CLIPPING_HINT_EXT,GL_FASTEST);
	//glBegin(GL_QUADS);
	for (int it = 0; it < 8; it++)
	{
		int yOffset =  it*3;
		//const int num_squares = 64;
		for (int l = -2; l < 2; l++)
		{
			int xOffset = l * 6 + (it%2)*3;
			//int color = (l&1) ? 200 : 0;
			//int color = 200;
			//glColor3f(color, color, color);
			glColor3f(200.0f, 200.0f, 200.0f);
			glBegin(GL_QUADS);
			for (int f = 0; f < num_faces; f++)
			{
				for (int i = 0; i < 4; i++) 
				{
					float xCoord = cube_vertices[cube_faces[f][i]][0] * ScalingFactor;
					float yCoord = cube_vertices[cube_faces[f][i]][1] * ScalingFactor;
					float zCoord = cube_vertices[cube_faces[f][i]][2] * ScalingFactor*0.1;
					//glColor3f (cube_vertex_colors[cube_faces[f][i]][0], cube_vertex_colors[cube_faces[f][i]][1], cube_vertex_colors[cube_faces[f][i]][2]);
					glVertex3f(xCoord + xOffset, yCoord +yOffset, zCoord);
				}
			}
			glEnd();
			glColor3f (0.0, 0.0, 0.0);
			for (int f = 0; f < num_faces; f++) {
				glBegin (GL_LINE_LOOP);
				for (int i = 0; i < 4; i++)
					glVertex3f((cube_vertices[cube_faces[f][i]][0] * ScalingFactor) + xOffset, 
							   cube_vertices[cube_faces[f][i]][1] * ScalingFactor+yOffset, 
							   cube_vertices[cube_faces[f][i]][2] * ScalingFactor*0.1);
				glEnd ();
			}
		}
	}
	ScalingFactor *= 2;
	/*for (int i = 0; i < 8; i++)
	{
		drawPawn3D(i*ScalingFactor-12, 1*ScalingFactor, 0, 0);
	}*/
	//glLineWidth(5);
	for (int i = [[PieceQueue getInstance]GetNumPieces]-1; i >= 0; i--)
	{
		GLChessPiece* currPiece = [[PieceQueue getInstance]GetPiece:i];
		if (currPiece.bIsWhite )
		{
			glColor3f(160/185.33, 82/185.33, 45/185.33); // brown
		}
		else {
			glColor3f(160/290.21, 32/290.21, 240/290.21); // purple
		}

		switch(currPiece._PieceChar)
		{
			case 'R':
				drawRook3D(currPiece.row * ScalingFactor - 12, currPiece.col*ScalingFactor, 0, 2);
				break;
			case 'B':
				drawBishop3D(currPiece.row * ScalingFactor - 12, currPiece.col*ScalingFactor, 0, 2);
				break;
			case 'P':
				drawPawn3D(currPiece.row * ScalingFactor - 12, currPiece.col*ScalingFactor, 0, 2);
				break;
			case 'K':
				drawKing3D(currPiece.row * ScalingFactor - 12, currPiece.col*ScalingFactor, 0, 2);
				break;
			case 'N':
				drawKnight3D(currPiece.row * ScalingFactor - 12, currPiece.col*ScalingFactor, 0, 2);
				break;
			case 'Q':
				drawQueen3D(currPiece.row * ScalingFactor - 12, currPiece.col*ScalingFactor, 0, 2);
				break;
		}
	}
	/*drawRook3D(0-12, 0, 0, 2);
	drawKnight3D(1*ScalingFactor-12, 0, 0, 2);
	drawBishop3D(2*ScalingFactor-12, 0, 0, 2);
	drawQueen3D(3*ScalingFactor-12, 0, 0, 2);
	drawKing3D(4*ScalingFactor-12, 0, 0, 2);
	drawBishop3D(5*ScalingFactor-12, 0, 0, 2);
	drawKnight3D(6*ScalingFactor-12, 0, 0, 2);
	drawRook3D(7*ScalingFactor-12, 0, 0, 2);*/
	//glLineWidth(1);
	//glEnd ();
		//Draw the grid
		/*for (int i = 1; i < 5;i++)
		{
			for (int j = 1; j < 5; j++)
			{
				//2D
				glColor3f(200, 200, 200);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2), 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*i*2, 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				
				//glColor3f(200, 200, 200);
				glVertex3f(ScalingFactor*(2*j-2), ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-2), 0);
				glVertex3f(ScalingFactor*(2*j-2), ScalingFactor*(i*2-2), 0);
				
				glColor3f(0, 0, 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-2), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-2), 0);
				
				//glColor3f(0, 0, 0);
				glVertex3f(ScalingFactor*(2*j-2), ScalingFactor*(i*2-0), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-0), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-2), ScalingFactor*(i*2-1), 0);
				
				//--- End of 2D
				glColor3f(200, 200, 200);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2), 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*i*2, 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2), 2);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*i*2, 2);
				
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-1), 2);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 2);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2), 2);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*i*2, 2);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-1), 2);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 2);
				
			}
		}*/
	//}
	//glEnd();
	return;
	
	glColor3f(139/139,69/139.0,19/139.0);
	
	drawRook2D(0,0);
	drawKnight2D(1*ScalingFactor,0);
	drawBishop2D(2*ScalingFactor,0);
	drawQueen2D(3*ScalingFactor,0);
	glColor3f(139/139,69/139.0,19/139.0);
	drawKing2D(4*ScalingFactor,0);
	drawBishop2D(5*ScalingFactor,0);
	drawKnight2D(6*ScalingFactor,0);
	drawRook2D(7*ScalingFactor,0);
	
	for (int i = 0; i < 8; i++)
	{
		drawPawn2D(i*ScalingFactor,1*ScalingFactor);
	}
	
	
	
}

-(void) drawRect:(NSRect) bounds
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	[self resizeGL];
	[self updateModelView];
	[self updateProjection];
	
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex3f(gRayCastLocation.x, gRayCastLocation.y, gRayCastLocation.z);
	glVertex3f(gRayCastLocation.x * 2, gRayCastLocation.y * 2, gRayCastLocation.z * 2);
	glEnd();
	glLineWidth(1);
	
	//drawCube(1.5);
	drawAnObject();
	
	/*if (SphereDrawQueue[0].z != -1 )
	{
		glColor3f(255, 255, 0);
		//drawSphere(SphereDrawQueue[0].x, SphereDrawQueue[0].y, 0.0, 1.0, 10, 10);
		glLineWidth(10);
		glBegin(GL_LINES);
		glVertex3f(0.f, 0.f, -10.f);
		recVec nearPoint = SphereDrawQueue[1];
		glVertex3f(nearPoint.x, nearPoint.y, nearPoint.z);
		for (int i = 0; i < 10; i++)
		{
			glVertex3f(nearPoint.x + SphereDrawQueue[0].x*i*5, nearPoint.y + SphereDrawQueue[0].y*i*5, nearPoint.z + SphereDrawQueue[0].z*i*5 );
			//if (SphereDrawQueue[0].z*i*5 -10> 0)
			//	drawSphere(SphereDrawQueue[0].x*i*5, SphereDrawQueue[0].y*i*5, SphereDrawQueue[0].z*i*5-10, 1.0, 10, 10);
		}
		glEnd();
		recVec nearPoint = SphereDrawQueue[1];
		float scalingFactor = -nearPoint.z / SphereDrawQueue[0].z;
		drawSphere(nearPoint.x + SphereDrawQueue[0].x*scalingFactor, nearPoint.y + SphereDrawQueue[0].y*scalingFactor, nearPoint.z + SphereDrawQueue[0].z*scalingFactor, 1.0, 10, 10);
		
		glColor3f(255, 0, 0);
	}*/
	double scale = 3.0;
	glColor3f(255, 255, 0);
	for (int i = [[SquareDrawQueue getInstance ]GetNumItems] - 1; i >=0; i--)
	{
		int xOffset = ([[SquareDrawQueue getInstance ]GetRow: i] - 4 ) * scale;
		//printf("Row: %i Actual Draw: %i", [[SquareDrawQueue getInstance ]GetRow: i], xOffset);
		drawSphere( //xOffset > 0 ? xOffset + scale / 2 : xOffset - scale / 2, 
				   xOffset,
				   ([[SquareDrawQueue getInstance ]GetCol: i] * scale),
				   1, 
				   0.5,
				   10,
				   10);
	}
	glColor3f(255, 0, 0);
	/*glBegin(GL_LINES);
	for (int i = 0; i < 10; i++)
	{
		glVertex3f(0, 0, -10 + i);
		glVertex3f(0, 0, -10 + (i+1));
	}
	glEnd();*/
	
	glLineWidth(1);
	
	glFlush();
}

void drawKing(double x, double y)
{
	float PI = 3.1415926535;
	x+=0.5;
	
	glBegin(GL_TRIANGLES);
	{
		//Base triangle of king
		glVertex3f(0.0+x, 0.2+y, 0.0);
		glVertex3f(0.5+x, 0.2+y, 0.0);
		glVertex3f(0.25+x, 0.9+y, 0.0);
		
		//Middle circle
		for (int i = 0; i < 18; i++)
		{
			glVertex3f( 0.25+x, 0.9+y, 0.0);
			glVertex3f( 0.25 +x + .2 * cos(20*i * PI / 180), y + 0.9 + .2 * sin(20*i * PI / 180), 0.0);
			//glVertex3f(0.5 + 0.4*cos((4.5*i)*180/PI),0.6+0.4*sin((4.5*i)*180/PI), 0.0);	
			glVertex3f( 0.25 +x + .2 * cos((i+1) * 20 * PI/180), y + 0.9 + .2 * sin((i+1) * 20*PI/180), 0.0);
		}
		
	}
	glEnd();
	
	glBegin(GL_QUADS);
	{
		glVertex3f(x+0.22,y+1.0,0);
		glVertex3f(x+0.28,y+1.0,0);
		glVertex3f(x+0.28,y+1.25,0);
		glVertex3f(x+0.22,y+1.25,0);
		
		glVertex3f(x+0.15,y+1.15,0);
		glVertex3f(x+0.35,y+1.15,0);
		glVertex3f(x+0.35,y+1.20,0);
		glVertex3f(x+0.15,y+1.20,0);
	}
	glEnd();
}

void drawPawn(double x, double y)
{
	float PI = 3.1415926535;
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < 18; i++)
		{
			glVertex3f( x+0.72, y+0.8, 0.0);
			glVertex3f( x+0.72 + .23 * cos(20*i * PI / 180), y+0.8 + .23 * sin(20*i * PI / 180), 0.0);	
			glVertex3f( x+0.72 + .23 * cos((i+1) * 20 * PI/180), y+0.8 + .23 * sin((i+1) * 20*PI/180), 0.0);
			
			glVertex3f( x+0.72, y+1.1, 0.0);
			glVertex3f( x+0.72 + .1 * cos(20*i * PI / 180), y+1.1 + .1 * sin(20*i * PI / 180), 0.0);	
			glVertex3f( x+0.72 + .1 * cos((i+1) * 20 * PI/180), y+1.1 + .1 * sin((i+1) * 20*PI/180), 0.0);
			
			glVertex3f( x+0.72, y+0.2, 0.0);
			glVertex3f( x+0.72 + .4 * cos(10*i * PI / 180), y+0.2 + .4 * sin(10*i * PI / 180), 0.0);	
			glVertex3f( x+0.72 + .4 * cos((i+1) * 10 * PI/180), y+0.2 + .4 * sin((i+1) * 10*PI/180), 0.0);
		}
	}
	glEnd();
	
	glBegin(GL_QUADS);
	{
		glVertex3f(x+0.32, y+0.1, 0);
		glVertex3f(x+0.32, y+0.2, 0);
		glVertex3f(x+1.12, y+0.2, 0);
		glVertex3f(x+1.12, y+0.1, 0);
	}
	glEnd();
}

void drawQueen(double x, double y)
{
	float PI = 3.1415926535;
	
	x += 0.5;
	
	glBegin(GL_TRIANGLES);
	{
		//Base triangle of queen
		glVertex3f(x+0.0, y+0.2, 0.0);
		glVertex3f(x+0.5, y+0.2, 0.0);
		glVertex3f(x+0.25, y+0.8, 0.0);
		
		
		
		//Middle circle
		for (int i = 0; i < 18; i++)
		{
			glVertex3f( x+0.25, y+0.8, 0.0);
			glVertex3f( x+0.25 + .2 * cos(20*i * PI / 180), y+0.9 + .2 * sin(20*i * PI / 180), 0.0);
			//glVertex3f(0.5 + 0.4*cos((4.5*i)*180/PI),0.6+0.4*sin((4.5*i)*180/PI), 0.0);	
			glVertex3f( x+0.25 + .2 * cos((i+1) * 20 * PI/180), y+0.9 + .2 * sin((i+1) * 20*PI/180), 0.0);
		}
		
	}
	glEnd();
	
	glLineWidth(6);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0x0F0F);
	
	glBegin(GL_LINE_STRIP);
	{
		for (int i = 0; i <36; i++)
		{
			//glVertex3f( 0.25, 0.6, 0.0);
			glVertex3f( x+0.25 + .3 * cos(10*i * PI / 180), y+0.9 + .3 * sin(10*i * PI / 180), 0.0);
			//glVertex3f(0.5 + 0.4*cos((4.5*i)*180/PI),0.6+0.4*sin((4.5*i)*180/PI), 0.0);	
			glVertex3f( x+0.25 + .3 * cos((i+1) * 10 * PI/180), y+0.9 + .3 * sin((i+1) * 10*PI/180), 0.0);
		}
	}
	glEnd();
	
	glColor3f(255.0f,255.0f,255.0f);
	glBegin(GL_QUADS);
	{
		glVertex3f(x+0.23, y+0.3, 0.0);
		glVertex3f(x+0.23, y+0.7, 0.0);
		glVertex3f(x+0.27, y+0.7, 0.0);
		glVertex3f(x+0.27, y+0.3, 0.0);
		
		glVertex3f(x+0.18, y+0.60, 0.0);
		glVertex3f(x+0.32, y+0.60, 0.0);
		glVertex3f(x+0.32, y+0.65, 0.0);
		glVertex3f(x+0.18, y+0.65, 0.0);
	}
	glEnd();
}

void drawBishop(double x, double y)
{
	glBegin(GL_TRIANGLES);
	{
	//Base	
	glVertex3f( x+0.15, y+0.2, 0.0);
	glVertex3f( x+1.15, y+0.2, 0.0);
	glVertex3f( x+1.15, y+0.3, 0.0);
	
	glVertex3f( x+0.15, y+0.2, 0.0);
	glVertex3f( x+0.15, y+0.3, 0.0);
	glVertex3f( x+1.15, y+0.3, 0.0);
	
	//Middle
	glVertex3f( x+0.55, y+0.3, 0.0);
	glVertex3f( x+0.75, y+0.4, 0.0);
	glVertex3f( x+0.55, y+0.4, 0.0);
	
	glVertex3f( x+0.55, y+0.3, 0.0);
	glVertex3f( x+0.75, y+0.4, 0.0);
	glVertex3f( x+0.75, y+0.3, 0.0);
	
	//Corners
	glVertex3f( x+0.35, y+0.4, 0.0);
	glVertex3f( x+0.45, y+0.6, 0.0);
	glVertex3f( x+0.55, y+0.4, 0.0);
	
	glVertex3f( x+0.75, y+0.4, 0.0);
	glVertex3f( x+0.85, y+0.6, 0.0);
	glVertex3f( x+0.95, y+0.4, 0.0);
	
	glVertex3f(x+0.45, y+0.6, 0.0);
	glVertex3f(x+0.45, y+0.4, 0.0);
	glVertex3f(x+0.95, y+0.4, 0.0);
	
	glVertex3f(x+0.45, y+0.6, 0.0);
	glVertex3f(x+0.85, y+0.6, 0.0);
	glVertex3f(x+0.85, y+0.4, 0.0);
	
	double PI = 3.1415926535;
	
	//Middle circle
	for (int i = -10; i < 7; i++)
	{
		glVertex3f( x+0.65, y+0.9, 0.0);
		glVertex3f( x+0.65 + sqrt(.13) * cos(20*i * PI / 180), y+0.9 + sqrt(.13) * sin(20*i * PI / 180), 0.0);
		//glVertex3f(0.5 + 0.4*cos((4.5*i)*180/PI),0.6+0.4*sin((4.5*i)*180/PI), 0.0);	
		glVertex3f( x+0.65 + sqrt(.13) * cos((i+1) * 20 * PI/180), y+0.9 + sqrt(.13) * sin((i+1) * 20*PI/180), 0.0);
	}
	
	//top cross
	glVertex3f(x+0.62, y+1.2, 0.0);
	glVertex3f(x+0.68, y+1.4, 0.0);
	glVertex3f(x+0.62, y+1.4, 0.0);
	
	glVertex3f(x+0.68, y+1.4, 0.0);
	glVertex3f(x+0.68, y+1.2, 0.0);
	glVertex3f(x+0.62, y+1.2, 0.0);
	}
	glEnd();
}

void drawKnight(double x, double y)
{
	glBegin(GL_TRIANGLES);
	{
	//Base
	glVertex3f( x+0.35, y+0.2, 0.0);
	glVertex3f( x+1.15, y+0.2, 0.0);
	glVertex3f( x+1.15, y+0.3, 0.0);
	
	glVertex3f( x+0.35, y+0.2, 0.0);
	glVertex3f( x+0.35, y+0.3, 0.0);
	glVertex3f( x+1.15, y+0.3, 0.0);
	
	//Middle Arch
	glVertex3f( x+0.55, y+0.3, 0.0);
	glVertex3f( x+0.65, y+0.3, 0.0);
	glVertex3f( x+0.65, y+0.8, 0.0);
	
	glVertex3f( x+1.05, y+0.3, 0.0);
	glVertex3f( x+0.65, y+0.3, 0.0);
	glVertex3f( x+1.05, y+0.8, 0.0);

	glVertex3f( x+0.65, y+0.3, 0.0);
	glVertex3f( x+0.65, y+0.8, 0.0);
	glVertex3f( x+1.05, y+0.8, 0.0);
	
	glVertex3f( x+1.05, y+0.7, 0.0);
	glVertex3f( x+1.05, y+0.3, 0.0);
	glVertex3f( x+1.08, y+0.7, 0.0);
	
	glVertex3f( x+1.05, y+0.7, 0.0);
	glVertex3f( x+1.05, y+0.8, 0.0);
	glVertex3f( x+1.08, y+0.7, 0.0);
	
	//Nose
	
	glVertex3f( x+0.25, y+0.7, 0.0);
	glVertex3f( x+0.15, y+0.75, 0.0);
	glVertex3f( x+0.65, y+0.8, 0.0);
	
	glVertex3f( x+0.15, y+0.75, 0.0);
	glVertex3f( x+0.65, y+0.8, 0.0);
	glVertex3f( x+0.35, y+1.0, 0.0);
	
	glVertex3f( x+0.35, y+1.0, 0.0);
	glVertex3f( x+0.45, y+1.1, 0.0);
	glVertex3f( x+0.25, y+1.1, 0.0);
	
	glVertex3f( x+0.35, y+1.0, 0.0);
	glVertex3f( x+0.45, y+1.1, 0.0);
	glVertex3f( x+0.65, y+0.8, 0.0);
	
	glVertex3f( x+0.45, y+1.1, 0.0);
	glVertex3f( x+0.65, y+0.8, 0.0);
	glVertex3f( x+0.55, y+1.2, 0.0);
	
	glVertex3f( x+0.45, y+1.1, 0.0);
	glVertex3f( x+0.5, y+1.3, 0.0);
	glVertex3f( x+0.55, y+1.2, 0.0);
	
	float PI = 3.1415926535;
	
	//Arch from head center of 0.5,0.6 radius of 0.3
	for (int i = 0; i < 9; i++)
	{
		glVertex3f(x+0.65,y+0.8, 0.0);
		glVertex3f(x+0.65 + 0.4*cos((4.5*i)*180/PI),y+0.8+0.4*sin((4.5*i)*180/PI), 0.0);	
		glVertex3f(x+0.65 + 0.4*cos((4.5*(i+1))*180/PI),y+0.8+0.4*sin((4.5*(i+1))*180/PI), 0.0);
	}
	}
	glEnd();
}

void drawRook(double x, double y)
{
	glBegin(GL_TRIANGLES);
	{
	//Base of the rook
	glVertex3f( x+0.30, y+0.2, 0.0);
	glVertex3f( x+0.30, y+0.3, 0.0);
	glVertex3f( x+1.20, y+0.2, 0.0);
	
	glVertex3f( x+0.30, y+0.3, 0.0);
	glVertex3f( x+1.20, y+0.3, 0.0);
	glVertex3f( x+1.20, y+0.2, 0.0);
	
	//next level up
	
	//Corners
	glVertex3f( x+0.35, y+0.3, 0.0);
	glVertex3f( x+0.45, y+0.5, 0.0);
	glVertex3f( x+0.45, y+0.3, 0.0);
	
	glVertex3f( x+1.15, y+0.3, 0.0);
	glVertex3f( x+1.05, y+0.5, 0.0);
	glVertex3f( x+1.05, y+0.3, 0.0);
	
	//Base accross
	
	glVertex3f( x+0.45, y+0.3, 0.0);
	glVertex3f( x+1.05, y+0.3, 0.0);
	glVertex3f( x+1.05, y+0.5, 0.0);
	
	glVertex3f( x+0.45, y+0.3, 0.0);
	glVertex3f( x+0.45, y+0.5, 0.0);
	glVertex3f( x+1.05, y+0.5, 0.0);
	
	//Middle section
	glVertex3f( x+0.45, y+0.5, 0.0);
	glVertex3f( x+0.45, y+0.8, 0.0);
	glVertex3f( x+1.05, y+0.8, 0.0);
	
	glVertex3f( x+0.45, y+0.5, 0.0);
	glVertex3f( x+1.05, y+0.5, 0.0);
	glVertex3f( x+1.05, y+0.8, 0.0);
	
	//Top Corner
	
	glVertex3f( x+0.45, y+0.8, 0.0);
	glVertex3f( x+0.35, y+1.05, 0.0);
	glVertex3f( x+0.45, y+1.05, 0.0);
	
	glVertex3f( x+1.05, y+0.8, 0.0);
	glVertex3f( x+1.15, y+1.05, 0.0);
	glVertex3f( x+1.05, y+1.05, 0.0);
	
	//top middle
	glVertex3f( x+0.45, y+0.8, 0.0);
	glVertex3f( x+1.05, y+0.8, 0.0);
	glVertex3f( x+1.05, y+1.05, 0.0);
	
	glVertex3f( x+0.45, y+0.8, 0.0);
	glVertex3f( x+0.45, y+1.05, 0.0);
	glVertex3f( x+1.05, y+1.05, 0.0);
	
	//Top square things
	glVertex3f( x+0.35, y+1.2, 0.0);
	glVertex3f( x+0.5, y+1.2, 0.0);
	glVertex3f( x+0.5, y+1.05, 0.0);
	
	glVertex3f( x+0.35, y+1.2, 0.0);
	glVertex3f( x+0.35, y+1.05, 0.0);
	glVertex3f( x+0.5, y+1.05, 0.0);
	
	glVertex3f( x+0.65, y+1.2, 0.0);
	glVertex3f( x+0.85, y+1.2, 0.0);
	glVertex3f( x+0.85, y+1.05, 0.0);
	
	glVertex3f( x+0.65, y+1.2, 0.0);
	glVertex3f( x+0.65, y+1.05, 0.0);
	glVertex3f( x+0.85, y+1.05, 0.0);
	
	glVertex3f( x+1.0, y+1.2, 0.0);
	glVertex3f( x+1.15, y+1.2, 0.0);
	glVertex3f( x+1.15, y+1.05, 0.0);
	
	glVertex3f( x+1.0, y+1.2, 0.0);
	glVertex3f( x+1.0, y+1.05, 0.0);
	glVertex3f( x+1.15, y+1.05, 0.0);
	}
	glEnd();
}
- (void) resetCamera
{
	camera.aperture = 40;
	camera.rotPoint = gOrigin;
	
	camera.viewPos.x = 0.0;
	camera.viewPos.y = 0.0;
	camera.viewPos.z = -10.0;
	camera.viewDir.x = -camera.viewPos.x; 
	camera.viewDir.y = -camera.viewPos.y; 
	camera.viewDir.z = -camera.viewPos.z;
	
	camera.viewUp.x = 0;  
	camera.viewUp.y = 1; 
	camera.viewUp.z = 0;
}

- (void) resizeGL
{
    NSRect rectView = [self bounds];
    
    // ensure camera knows size changed
    if ((camera.viewHeight != rectView.size.height) ||
        (camera.viewWidth != rectView.size.width)) {
        camera.viewHeight = rectView.size.height;
        camera.viewWidth = rectView.size.width;
        
        glViewport (-2, -2, camera.viewWidth, camera.viewHeight);
		//glViewport (-20, 20, camera.viewWidth*2, camera.viewHeight*2);
        [self updateProjection];  // update projection matrix
        //[self updateInfoString];
    }
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
    [self mouseDragged: theEvent];
}

// ---------------------------------

- (void)otherMouseDragged:(NSEvent *)theEvent
{
    [self mouseDragged: theEvent];
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
    [self mouseUp:theEvent];
}

// ---------------------------------

- (void)otherMouseUp:(NSEvent *)theEvent
{
    [self mouseUp:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
    if (gDolly) { // end dolly
        gDolly = GL_FALSE;
    } else if (gPan) { // end pan
        gPan = GL_FALSE;
    } else if (gTrackball) { // end trackball
        gTrackball = GL_FALSE;
        if (gTrackBallRotation[0] != 0.0)
            addToRotationTrackball (gTrackBallRotation, worldRotation);
        gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
    } 
	[self updateModelView];
	[self updateProjection];
    gTrackingViewInfo = NULL;
}
@end
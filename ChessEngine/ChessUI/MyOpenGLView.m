//
//  MyOpenGLView.m
//  ChessUI
//
//  Created by Michael Weingert on 12-01-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "MyOpenGLView.h"
#include <OpenGL/gl.h>


@implementation MyOpenGLView

void drawRook(double x, double y);
void drawKnight(double x, double y);
void drawBishop(double x, double y);
void drawQueen(double x, double y);
void drawPawn(double x, double y);
void drawKing(double x, double y);

static void drawAnObject()
{
	glColor3f(1.0f, 0.85f, 0.35f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 12, 0, 12,-1,1);
	glMatrixMode(GL_MODELVIEW);
	//GLint nViewport[4];
	//glGetIntegerv(GL_VIEWPORT, nViewport);
	int ret = glGetError();
	
	double ScalingFactor = 1.5;
	
	glBegin(GL_QUADS);
	{
		//Draw the grid
		for (int i = 1; i < 5;i++)
		{
			for (int j = 1; j < 5; j++)
			{
				glColor3f(200, 200, 200);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2), 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*i*2, 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				
				glColor3f(200, 200, 200);
				glVertex3f(ScalingFactor*(2*j-2), ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-2), 0);
				glVertex3f(ScalingFactor*(2*j-2), ScalingFactor*(i*2-2), 0);
				
				glColor3f(0, 0, 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*2*j, ScalingFactor*(i*2-2), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-2), 0);
				
				glColor3f(0, 0, 0);
				glVertex3f(ScalingFactor*(2*j-2), ScalingFactor*(i*2-0), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-0), 0);
				glVertex3f(ScalingFactor*(2*j-1), ScalingFactor*(i*2-1), 0);
				glVertex3f(ScalingFactor*(2*j-2), ScalingFactor*(i*2-1), 0);
			}
		}
	}
	glEnd();
	
	glColor3f(139/139,69/139.0,19/139.0);
	
	drawRook(0,0);
	drawKnight(1*ScalingFactor,0);
	drawBishop(2*ScalingFactor,0);
	drawQueen(3*ScalingFactor,0);
	glColor3f(139/139,69/139.0,19/139.0);
	drawKing(4*ScalingFactor,0);
	drawBishop(5*ScalingFactor,0);
	drawKnight(6*ScalingFactor,0);
	drawRook(7*ScalingFactor,0);
	
	for (int i = 0; i < 8; i++)
	{
		drawPawn(i*ScalingFactor,1*ScalingFactor);
	}
	
	
	
}

-(void) drawRect:(NSRect) bounds
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawAnObject();
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

@end

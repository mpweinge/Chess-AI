/*
 *  3DPieces.c
 *  ChessUI
 *
 *  Created by Michael Weingert on 12-03-19.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "3DPieces.h"
#include "assert.h"
#include <math.h>

const double PI = 3.1415926;

void drawRook3D(double x, double y, double z, double scalingFactor)
{
	drawCylinder(x, y, z, .6, .1, 18);
	drawCylinder(x, y, z+.1, .5, .2, 18);
	drawCylinder(x, y, z+.3, .4, .1, 18);
	drawCylinder(x, y, z+.4, .4, .4, 18);
	
	drawCylinder(x, y, z+.8, .5, .1, 18);
	drawCylinder(x, y, z+.9, .6, .1, 18);
	
	const int NumTowers = 12;
	const int AngleIncrement = 360 / NumTowers;
	for (int i = 0 ; i < NumTowers; i+=2)
	{
		drawDonut(AngleIncrement*i, AngleIncrement*(i+1), 9, .7, .5, x, y, 1.0, 1.1);
	}
}

void drawKing3D(double x, double y, double z, double scalingFactor)
{
	glBegin(GL_QUADS);
	{
		glVertex3f(x-(0.2)*scalingFactor, y, z*scalingFactor);
		glVertex3f(x-(0.2)*scalingFactor, y, (z+1.0)*scalingFactor);
		glVertex3f(x-(0.2)*scalingFactor, y-0.5, (z+1.0)*scalingFactor);
		glVertex3f(x-(0.2)*scalingFactor, y-0.5, z*scalingFactor);
		
		glVertex3f(x-(0.2)*scalingFactor, y, (z+1.0)*scalingFactor);
		glVertex3f((x), y, (z+1.0)*scalingFactor);
		glVertex3f((x), y-0.5, (z+1.0)*scalingFactor);
		glVertex3f(x-(0.2)*scalingFactor, y-0.5, (z+1.0)*scalingFactor);
		
		
		glVertex3f(x-(0.2)*scalingFactor, y, (z)*scalingFactor);
		glVertex3f((x), y, (z)*scalingFactor);
		glVertex3f((x), y-0.5, (z)*scalingFactor);
		glVertex3f(x-(0.2)*scalingFactor, y-0.5, (z)*scalingFactor);
		
		glVertex3f((x), y, (z)*scalingFactor);
		glVertex3f((x), y, (z+1.0)*scalingFactor);
		glVertex3f((x), y-0.5, (z+1.0)*scalingFactor);
		glVertex3f((x), y-0.5, (z)*scalingFactor);
		
		glVertex3f((x), y, (z+0.4)*scalingFactor);
		glVertex3f((x) + .6*scalingFactor, y, (z + 1)*scalingFactor);
		glVertex3f((x) + .6*scalingFactor, y-0.5, (z + 1)*scalingFactor);
		glVertex3f((x), y-0.5, (z+0.4)*scalingFactor);
		
		glVertex3f((x), y, (z+0.6)*scalingFactor);
		glVertex3f((x) + .6*scalingFactor, y, (z)*scalingFactor);
		glVertex3f((x) + .6*scalingFactor, y-0.5, (z)*scalingFactor);
		glVertex3f((x), y-0.5, (z+0.6)*scalingFactor);
		
		glVertex3f((x), y, (z+0.6)*scalingFactor);
		glVertex3f((x) + .4*scalingFactor, y, (z+1)*scalingFactor);
		glVertex3f((x) + .4*scalingFactor, y-0.5, (z+1)*scalingFactor);
		glVertex3f((x), y-0.5, (z+0.6)*scalingFactor);
		
		glVertex3f((x), y, (z+0.4)*scalingFactor);
		glVertex3f(x+(.4)*scalingFactor, y, (z)*scalingFactor);
		glVertex3f(x+(.4)*scalingFactor, y-0.5, (z)*scalingFactor);
		glVertex3f((x), y-0.5, (z+0.4)*scalingFactor);
		
		glVertex3f((x), y, (z+0.5)*scalingFactor);
		glVertex3f(x+(.4)*scalingFactor, y, (z+0.9)*scalingFactor);
		glVertex3f(x+(.4)*scalingFactor, y-0.5, (z+0.9)*scalingFactor);
		glVertex3f((x), y-0.5, (z+0.5)*scalingFactor);
		
		glVertex3f((x), y, (z+0.5)*scalingFactor);
		glVertex3f(x+(.4)*scalingFactor, y, (z+0.1)*scalingFactor);
		glVertex3f(x+(.4)*scalingFactor, y-0.5, (z+0.1)*scalingFactor);
		glVertex3f((x), y-0.5, (z+0.5)*scalingFactor);
		
		glVertex3f(x + (.4)*scalingFactor, y, (z)*scalingFactor);
		glVertex3f(x + (.6)*scalingFactor, y, (z)*scalingFactor);
		glVertex3f(x + (.6)*scalingFactor, y-0.5, (z)*scalingFactor);
		glVertex3f(x + (.4)*scalingFactor, y-0.5, (z)*scalingFactor);
		
		glVertex3f(x + (.4)*scalingFactor, y, (z + 1)*scalingFactor);
		glVertex3f(x + (.6)*scalingFactor, y, (z + 1)*scalingFactor);
		glVertex3f(x + (.6)*scalingFactor, y-0.5,( z + 1)*scalingFactor);
		glVertex3f(x + (.4)*scalingFactor, y-0.5, (z + 1)*scalingFactor);
	}
	glEnd();
}

void drawBishop3D(double x, double y, double z, double scalingFactor)
{
	
	glBegin(GL_QUADS);
	{
		glVertex3f(x+(0.25)*scalingFactor, y, (z)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y, (z)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y-0.5, (z)*scalingFactor);
		glVertex3f(x+(0.25)*scalingFactor, y-0.5, (z)*scalingFactor);
		
		glVertex3f(x+(0.25)*scalingFactor, y, (z+0.3)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y, (z+0.3)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y-0.5, (z+0.3)*scalingFactor);
		glVertex3f(x+(0.25)*scalingFactor, y-0.5, (z+0.3)*scalingFactor);
		
		glVertex3f(x+(0.25)*scalingFactor, y, (z+0.7)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y, (z+0.7)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y-0.5, (z+0.7)*scalingFactor);
		glVertex3f(x+(0.25)*scalingFactor, y-0.5, (z+0.7)*scalingFactor);
		
		glVertex3f(x+(0.25)*scalingFactor, y, (z+1.0)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y, (z+1.0)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y-0.5, (z+1.0)*scalingFactor);
		glVertex3f(x+(0.25)*scalingFactor, y-0.5, (z+1.0)*scalingFactor);
		
		glVertex3f(x-(0.5)*scalingFactor, y, (z+1.0)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y, (z)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y-0.5, (z)*scalingFactor);
		glVertex3f(x-(0.5)*scalingFactor, y-0.5, (z+1.0)*scalingFactor);
		
		glVertex3f(x-(0.3)*scalingFactor, y, (z+1.0)*scalingFactor);
		glVertex3f(x-(0.3)*scalingFactor, y, (z)*scalingFactor);
		glVertex3f(x-(0.3)*scalingFactor, y-0.5, (z)*scalingFactor);
		glVertex3f(x-(0.3)*scalingFactor, y-0.5, (z+1.0)*scalingFactor);
		
		for (int i = 0; i < 9; i++)
		{
			glVertex3f( x + (.25 + .25 * sin ( 20 * i * PI / 180) ) *scalingFactor, y, (z + .75 + .25 * cos(20 * i * PI / 180) )*scalingFactor);
			glVertex3f( x + (.25 + .25 * sin ( 20 * (i + 1) * PI / 180) )*scalingFactor, y, (z + .75 + .25 * cos(20 * (i+1) * PI / 180) )*scalingFactor);
			glVertex3f( x + (.25 + .25 * sin ( 20 * (i + 1) * PI / 180) )*scalingFactor, y-0.5, (z + .75 + .25 * cos(20 * (i+1) * PI / 180) )*scalingFactor);
			glVertex3f( x + (.25 + .25 * sin ( 20 * i * PI / 180) )*scalingFactor, y-0.5, (z + .75 + .25 * cos(20 * i * PI / 180) )*scalingFactor);
		}
		for (int i = 0; i < 9; i++)
		{
			glVertex3f( x + (.25 + .25 * sin ( 20 * i * PI / 180) )*scalingFactor, y, (z + .25 + .25 * cos(20 * i * PI / 180) )*scalingFactor);
			glVertex3f( x + (.25 + .25 * sin ( 20 * (i + 1) * PI / 180) )*scalingFactor, y, (z + .25 + .25 * cos(20 * (i+1) * PI / 180))*scalingFactor );
			glVertex3f( x + (.25 + .25 * sin ( 20 * (i + 1) * PI / 180) )*scalingFactor, y-0.5, (z + .25 + .25 * cos(20 * (i+1) * PI / 180)  )*scalingFactor);
			glVertex3f( x + (.25 + .25 * sin ( 20 * i * PI / 180) )*scalingFactor, y-0.5, (z + .25 + .25 * cos(20 * i * PI / 180) )*scalingFactor);
		}
	}
	glEnd();
	
	//Center of circle start at x + 0.35, z + 0.85
	/*drawCylinder(x, y, z, .8, .1, 18);
	drawCylinder(x, y, z+.1, .2, .2, 18);
	drawPartialCone(0, 360, 18, .6, .2, x, y, z+.3, z+.6);
	
	drawCustomBishopHead(5, 75, x, y, z+.6, z+1.2, .5);*/
	
	//void drawCustomBishopHead(int startingAngle,
	//						  int endingAngle,
	//						  float centerX,
	//						  float centerY,
	//						  float startZ,
	//						  float endZ,
	//						  float radius)
}

void drawQueen3D(double x, double y, double z, double scalingFactor)
{
	glBegin(GL_QUADS);
	for (int i = 0; i < 18; i++)
	{
		glVertex3f( x + (.5 * sin(i * 20 * PI / 180 ))*scalingFactor, y, (z + .5 + .5 * cos (i * 20 * PI / 180 ) )*scalingFactor);
		glVertex3f( x + (.5 * sin( (i+1) * 20 * PI / 180 ))*scalingFactor, y, (z + .5 + .5 * cos ( (i+1) * 20 * PI / 180 ) )*scalingFactor);
		glVertex3f( x + (.5 * sin( (i+1) * 20 * PI / 180 ))*scalingFactor, y-0.5, (z + .5 + .5 * cos ( (i+1) * 20 * PI / 180 ) )*scalingFactor);
		glVertex3f( x + (.5 * sin(i * 20 * PI / 180 ))*scalingFactor, y-0.5, (z + .5 + .5 * cos (i * 20 * PI / 180 ) )*scalingFactor);		
		
		glVertex3f( x + (.4 * sin(i * 20 * PI / 180 ))*scalingFactor, y, (z + .5 + .4 * cos (i * 20 * PI / 180 ) )*scalingFactor);
		glVertex3f( x + (.4 * sin( (i+1) * 20 * PI / 180 ))*scalingFactor, y, (z + .5 + .4 * cos ( (i+1) * 20 * PI / 180 ) )*scalingFactor);
		glVertex3f( x + (.4 * sin( (i+1) * 20 * PI / 180 ))*scalingFactor, y-0.5, (z + .5 + .4 * cos ( (i+1) * 20 * PI / 180 ) )*scalingFactor);
		glVertex3f( x + (.4 * sin(i * 20 * PI / 180 ))*scalingFactor, y-0.5, (z + .5 + .4 * cos (i * 20 * PI / 180 ) )*scalingFactor);		
	}
	
	glVertex3f( x + (.5)*scalingFactor, y, (z + .1)*scalingFactor);
	glVertex3f( x + (.4)*scalingFactor, y, (z)*scalingFactor);
	glVertex3f( x + (.4)*scalingFactor, y-0.5, (z)*scalingFactor);
	glVertex3f( x + (.5)*scalingFactor, y-0.5, (z + .1)*scalingFactor);
	
	glVertex3f( x + (.2)*scalingFactor, y, (z + .4)*scalingFactor);
	glVertex3f( x + (.1)*scalingFactor, y, (z + .3)*scalingFactor);
	glVertex3f( x + (.1)*scalingFactor, y-0.5, (z + .3)*scalingFactor);
	glVertex3f( x + (.2)*scalingFactor, y-0.5, (z + .4)*scalingFactor);	
	
	glVertex3f( x + (.5)*scalingFactor, y, (z + .1)*scalingFactor);
	glVertex3f( x + (.2)*scalingFactor, y, (z + .4)*scalingFactor);
	glVertex3f( x + (.2)*scalingFactor, y-0.5, (z + .4)*scalingFactor);
	glVertex3f( x + (.5)*scalingFactor, y-0.5, (z + .1)*scalingFactor);
	
	glVertex3f( x + (.4)*scalingFactor, y, (z)*scalingFactor);
	glVertex3f( x + (.1)*scalingFactor, y, (z + .3)*scalingFactor);
	glVertex3f( x + (.1)*scalingFactor, y-0.5, (z + .3)*scalingFactor);
	glVertex3f( x + (.4)*scalingFactor, y-0.5, (z)*scalingFactor);
			   
	glEnd();
}

void drawPawn3D(double x, double y, double z, double scalingFactor)
{
	//Square
	//assert(x == y);
	//glColor3f(255, 0, 0);
	//glBegin(GL_POLYGON);
	{
		//for (int ring = 0; ring < 9; ring++)
		{
			//Sphere one
			//9 'rings'
			//int radius = .23 * cos(ring / 9 * PI / 2);
			
			drawSphere(x, y, z+1.0, .23, 10, 10);
			drawSphere(x, y, z+1.3, .1, 10, 10);
			drawSphere(x, y, z+.4, .4, 10, 10);
		}
	}
	//glEnd();

	drawCylinder(x, y, z, 0.6, 0.2, 9);
}
void drawKnight3D(double x, double y, double z, double scalingfactor)
{
	glBegin(GL_QUADS);
	{
		glVertex3f((x), y, (z)*scalingfactor);
		glVertex3f((x), y, (z+1.0)*scalingfactor);
		glVertex3f((x), y-0.5, (z+1.0)*scalingfactor);
		glVertex3f((x), y-0.5, (z)*scalingfactor);
	
		glVertex3f((x), y, (z+1.0)*scalingfactor);
		glVertex3f(x+(.5)*scalingfactor, y, (z)*scalingfactor);
		glVertex3f(x+(.5)*scalingfactor, y-0.5, (z)*scalingfactor);
		glVertex3f((x), y-0.5, (z+1.0)*scalingfactor);
		
		glVertex3f(x+(0.5)*scalingfactor, y, (z)*scalingfactor);
		glVertex3f(x+(0.5)*scalingfactor, y, (z+1.0)*scalingfactor);
		glVertex3f(x+(0.5)*scalingfactor, y-0.5, (z+1.0)*scalingfactor);
		glVertex3f(x+(0.5)*scalingfactor, y-0.5, (z)*scalingfactor);
		
		glVertex3f(x+(.1)*scalingfactor, y, (z+1.0)*scalingfactor);
		glVertex3f((x), y, (z+1.0)*scalingfactor);
		glVertex3f((x), y-0.5, (z+1.0)*scalingfactor);
		glVertex3f(x+(.1)*scalingfactor, y-0.5, (z+1.0)*scalingfactor);
		
		glVertex3f(x + (.1)*scalingfactor, y, (z)*scalingfactor);
		glVertex3f(x + (.1)*scalingfactor,y, (z+1.0)*scalingfactor);
		glVertex3f(x + (.1)*scalingfactor,y-0.5, (z+1.0)*scalingfactor);
		glVertex3f(x + (.1)*scalingfactor, y-0.5, (z)*scalingfactor);
		
		glVertex3f(x + (.1)*scalingfactor, y, (z+1.0)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y, (z)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y-0.5, (z)*scalingfactor);
		glVertex3f(x + (.1)*scalingfactor, y-0.5, (z+1.0)*scalingfactor);
		
		glVertex3f(x + (.6)*scalingfactor, y, (z+1.0)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y, (z)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y-0.5, (z)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y-0.5, (z+1.0)*scalingfactor);
		
		glVertex3f(x + (.5)*scalingfactor, y, (z+1.0)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y, (z + 1.0)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y-0.5, (z + 1.0)*scalingfactor);
		glVertex3f(x + (.5)*scalingfactor, y-0.5, (z+1.0)*scalingfactor);
		
		glVertex3f(x + (.5)*scalingfactor, y, (z)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y, (z)*scalingfactor);
		glVertex3f(x + (.6)*scalingfactor, y-0.5, (z)*scalingfactor);
		glVertex3f(x + (.5)*scalingfactor, y-0.5, (z)*scalingfactor);
		
		glVertex3f((x), y, z);
		glVertex3f(x + (.1)*scalingfactor, y, z);
		glVertex3f(x + (.1)*scalingfactor, y-0.5, z);
		glVertex3f((x), y-0.5, z);
		
	}
	glEnd();
}
						 
void drawCustomBishopHead(int startingAngle,
						  int endingAngle,
						  float centerX,
						  float centerY,
						  float startZ,
						  float endZ,
						  float radius)
{
	//Draw a line from center to radius 
	glBegin(GL_QUADS);
	{
		glVertex3f(centerX, centerY+radius, (startZ+endZ)/2);
		glVertex3f(centerX, centerY-radius, (startZ+endZ)/2);
		glVertex3f(centerX+radius, centerY-radius*cos(startingAngle*PI/180), (startZ+endZ)/2+radius*sin(startingAngle*PI/180));
		glVertex3f(centerX+radius, centerY+radius*cos(startingAngle*PI/180), (startZ+endZ)/2+radius*sin(startingAngle*PI/180));
		
		glVertex3f(centerX, centerY+radius, (startZ+endZ)/2);
		glVertex3f(centerX, centerY-radius, (startZ+endZ)/2);
		glVertex3f(centerX+radius, centerY-radius*cos(endingAngle*PI/180), (startZ+endZ)/2+radius*sin(endingAngle*PI/180));
		glVertex3f(centerX+radius, centerY+radius*cos(endingAngle*PI/180), (startZ+endZ)/2+radius*sin(endingAngle*PI/180));
	}
	glEnd();
	//int numDivisions = 18;
	const int angleIncrement = (360 - (endingAngle - startingAngle)) / 29;
	const float centerZ = (startZ + endZ) / 2;
	glColor3f(0, 255, 0);
	
	for (int i = 0; i < 29; i++)
	{
		//volatile float yCoord = centerY + radius * cos(i*angleIncrement * PI / 180);
		//volatile float zCoord = centerZ + radius * sin(i*angleIncrement * PI / 180);
		//volatile float xCoord = centerX;
		//glVertex3f(xCoord, yCoord, zCoord);
		
		//volatile float xCoord = centerX + radius * cos((endingAngle + i*angleIncrement) * PI / 180);
		//volatile float yCoord = centerY;
		volatile float zCoord = centerZ + radius * sin((endingAngle + i*angleIncrement) * PI / 180);
		//glVertex3f(xCoord, yCoord, zCoord);
		
		float topRadius = radius * cos((endingAngle + i*angleIncrement) * PI / 180);
		
		float stoppingInnerAngle;
		if (zCoord < centerZ)
		{
			stoppingInnerAngle = 90;
		}
		else {
			float xDist = zCoord / tan(endingAngle);
			stoppingInnerAngle = acos(xDist / radius);
		}
		int outerAngle = 360 - 2 * (90 - stoppingInnerAngle);
		int topAngleIncrement = outerAngle / 10;
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < 10; j++)
		{
			glVertex3f(centerX + topRadius * cos(j * topAngleIncrement * PI / 180), 
					   centerY + topRadius * sin(j * topAngleIncrement * PI / 180),
					   zCoord);
		}
		glEnd();
	}
	
	glColor3f(255, 0, 0);
}

void drawPartialCone(int startingAngle,
					 int endAngle,
					 int numDivisions,
					 float outerRadius,
					 float innerRadius,
					 float centerX,
					 float centerY,
					 float startingZ,
					 float endZ)
{
	glBegin(GL_QUADS);
	{
		int angleIncrement = 360 / numDivisions;
		for (int i = 0; i < numDivisions; i++)
		{
			glVertex3f(centerX + outerRadius * cos(angleIncrement * i), centerY + outerRadius * sin(angleIncrement * i), startingZ);
			glVertex3f(centerX + innerRadius * cos(angleIncrement * i), centerY + innerRadius * sin(angleIncrement * i), endZ);
			glVertex3f(centerX + innerRadius * cos(angleIncrement * (i+1)), centerY + innerRadius * sin(angleIncrement * (i+1)), endZ);
			glVertex3f(centerX + outerRadius * cos(angleIncrement * (i+1)), centerY + outerRadius * sin(angleIncrement * (i+1)), startingZ);
		}
	}
	glEnd();
}

void drawDonut(int StartingAngle, 
			   int EndAngle, 
			   int NumDivisions, 
			   double outerRadius, 
			   double innerRadius, 
			   double centerX, 
			   double centerY, 
			   double startZ, 
			   double endZ)
{
	int AngleIncrement = (EndAngle - StartingAngle) / NumDivisions;
	
	glBegin(GL_QUADS);
	for (int i = StartingAngle ; i < EndAngle; i+= AngleIncrement )
	{
		glVertex3f(centerX + outerRadius * cos(i * PI / 180), centerY + outerRadius * sin(i * PI / 180), startZ);
		glVertex3f(centerX + outerRadius * cos((i+ AngleIncrement ) * PI / 180), centerY + outerRadius * sin((i+ AngleIncrement ) * PI / 180), startZ);
		glVertex3f(centerX + outerRadius * cos((i+ AngleIncrement ) * PI / 180), centerY + outerRadius * sin((i+ AngleIncrement ) * PI / 180), endZ);
		glVertex3f(centerX + outerRadius * cos(i * PI / 180), centerY + outerRadius * sin(i * PI / 180), endZ);
		
		glVertex3f(centerX + outerRadius * cos(i * PI / 180), centerY + outerRadius * sin(i * PI / 180), endZ);
		glVertex3f(centerX + outerRadius * cos((i+ AngleIncrement ) * PI / 180), centerY + outerRadius * sin((i+ AngleIncrement ) * PI / 180), endZ);
		glVertex3f(centerX + innerRadius * cos((i+ AngleIncrement ) * PI / 180), centerY + innerRadius * sin((i+ AngleIncrement ) * PI / 180), endZ);
		glVertex3f(centerX + innerRadius * cos(i * PI / 180), centerY + innerRadius * sin(i * PI / 180), endZ);
		
		glVertex3f(centerX + innerRadius * cos(i * PI / 180), centerY + innerRadius * sin(i * PI / 180), startZ);
		glVertex3f(centerX + innerRadius * cos((i+ AngleIncrement ) * PI / 180), centerY + innerRadius * sin((i+ AngleIncrement ) * PI / 180), startZ);
		glVertex3f(centerX + innerRadius * cos((i+ AngleIncrement ) * PI / 180), centerY + innerRadius * sin((i+ AngleIncrement ) * PI / 180), endZ);
		glVertex3f(centerX + innerRadius * cos(i * PI / 180), centerY + innerRadius * sin(i * PI / 180), endZ);
		
		glVertex3f(centerX + outerRadius * cos(i * PI / 180), centerY + outerRadius * sin(i * PI / 180), startZ);
		glVertex3f(centerX + outerRadius * cos((i+ AngleIncrement ) * PI / 180), centerY + outerRadius * sin((i+ AngleIncrement ) * PI / 180), startZ);
		glVertex3f(centerX + innerRadius * cos((i+ AngleIncrement ) * PI / 180), centerY + innerRadius * sin((i+ AngleIncrement ) * PI / 180), startZ);
		glVertex3f(centerX + innerRadius * cos(i * PI / 180), centerY + innerRadius * sin(i * PI / 180), startZ);
		
	}
	glEnd();
}

void drawCylinder(double startX, double startY, double startZ, double radius, double height, int numDivisions)
{
	glBegin(GL_TRIANGLES);
	{
		//Rectangular prism
		int angleIncrease = 360 / numDivisions;
		for (int i = 0; i < numDivisions; i++)
		{
			glVertex3f(startX+ radius * cos(angleIncrease*i * PI / 180), startY+ radius * sin(angleIncrease*i * PI / 180), startZ);
			glVertex3f(startX+ radius * cos(angleIncrease*i * PI / 180), startY+ radius * sin(angleIncrease*i * PI / 180), startZ+height);
			glVertex3f(startX+ radius * cos(angleIncrease*(i+1) * PI / 180), startY+ radius * sin(angleIncrease*(i+1) * PI / 180), startZ);
			
			glVertex3f(startX+ radius * cos(angleIncrease*i * PI / 180), startY+ radius * sin(angleIncrease*i * PI / 180), startZ+height);
			glVertex3f(startX+ radius * cos(angleIncrease*(i+1) * PI / 180), startY+ radius * sin(angleIncrease*(i+1) * PI / 180), startZ);
			glVertex3f(startX+ radius * cos(angleIncrease*(i+1) * PI / 180), startY+ radius * sin(angleIncrease*(i+1) * PI / 180), startZ+height);
			
			glVertex3f(startX+ radius * cos(angleIncrease*i * PI / 180), startY+ radius * sin(angleIncrease*i * PI / 180), startZ+height);
			glVertex3f(startX, startY, startZ+height);
			glVertex3f(startX+ radius * cos(angleIncrease*(i+1) * PI / 180), startY+ radius * sin(angleIncrease*(i+1) * PI / 180), startZ+height);
		}
		
	}
	glEnd();
}

void drawSphere(double startX, double startY, double startZ, double radius, int lats, int longs)
{
	int i, j;
	for(i = 0; i <= lats; i++) 
	{
		double lat0 = PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = radius * sin(lat0);
		double zr0 =  radius * cos(lat0);
		
		double lat1 = PI * (-0.5 + (double) i / lats);
		double z1 = radius * sin(lat1);
		double zr1 = radius * cos(lat1);
		
		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) 
		{
			double lng = 2 * PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);
			
			//No lighting ... yet
			//glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0 + startX, y * zr0 + startY, z0 + startZ);
			//glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1 + startX, y * zr1 + startY, z1 + startZ);
		}
		glEnd();
	}
}
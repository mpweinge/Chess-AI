/*
 *  3DPieces.h
 *  ChessUI
 *
 *  Created by Michael Weingert on 12-03-19.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef THREEDPIECES_H
#define THREEDPIECES_H

#include "CommonInclude.h"

void drawRook3D(double x, double y, double z, double scalingFactor);
void drawKnight3D(double x, double y, double z, double scalingFactor);
void drawBishop3D(double x, double y, double z, double scalingFactor);
void drawQueen3D(double x, double y, double z, double scalingFactor);
void drawPawn3D(double x, double y, double z, double ScalingFactor);
void drawKing3D(double x, double y, double z, double scalingfactor);

void drawCylinder(double startX, double startY, double startZ, double radius, double height, int numDivisions);
void drawSphere(double startX, double startY, double startZ, double radius, int lats, int longs);
void drawDonut(int StartingAngle, 
			   int EndAngle, 
			   int NumDivisions, 
			   double outerRadius, 
			   double innerRadius, 
			   double centerX, 
			   double centerY, 
			   double startZ, 
			   double endZ);

void drawPartialCone(int startingAngle,
					 int endAngle,
					 int numDivisions,
					 float outerRadius,
					 float innerRadius,
					 float centerX,
					 float centerY,
					 float startingZ,
					 float endZ);

void drawCustomBishopHead(int startingAngle,
						  int endingAngle,
						  float centerX,
						  float centerY,
						  float startZ,
						  float endZ,
						  float radius);

#endif
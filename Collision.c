/*
 *  Collision.c
 *  ChessUI
 *
 *  Created by Michael Weingert on 12-03-22.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Collision.h"

#include "3DPieces.h"

recVec CastRay(float x, float y, recVec CameraPosition, recVec* nearPosition)
{
	const float nearZPos = 0.0f;
	const float farZPos = 1.0f;
	
	GLdouble modelViewMatrix[16];
	GLdouble projectionMatrix[16];
	GLint viewportMatrix[4];
    
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetIntegerv(GL_VIEWPORT, viewportMatrix);
	
	y = (float)viewportMatrix[3] - y;
	
	GLdouble nearPointX, nearPointY, nearPointZ;
	GLdouble farPointX, farPointY, farPointZ;
	
	GLfloat winZ;
	
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	recVec rayVector;
	gluUnProject(x,y,winZ,modelViewMatrix, projectionMatrix, viewportMatrix, &nearPointX,  &nearPointY, &nearPointZ);
	/*rayVector.x = x;
	rayVector.y = y;
	rayVector.z = winZ;
	return rayVector;*/
	
	gluUnProject(x,y,nearZPos,modelViewMatrix, projectionMatrix, viewportMatrix, &nearPointX,  &nearPointY, &nearPointZ);
	gluUnProject(x,y,farZPos,modelViewMatrix, projectionMatrix, viewportMatrix, &farPointX,  &farPointY, &farPointZ);
	
	nearPosition->x = nearPointX;
	nearPosition->y = nearPointY;
	nearPosition->z = nearPointZ;
	//float rayMagnitude = sqrt(pow(fx, 2) + pow(fy, 2) + pow(fz, 2));
    
	
	
	rayVector.x = farPointX - nearPointX;
	rayVector.y = farPointY - nearPointY;
	rayVector.z = farPointZ - nearPointZ;
	
	float rayLength = sqrtf(pow(rayVector.x, 2) + pow(rayVector.y, 2) + pow(rayVector.z, 2));
	
	//normalizing ray vector
	rayVector.x /= rayLength;
	rayVector.y /= rayLength;
	rayVector.z /= rayLength;
	
	return rayVector;
	
    /*recVec ray = {fx - CameraPosition.x, fy - CameraPosition.y, fz - CameraPosition.z};
    //[ray normalize];
    
    //T = [planeNormal.(pointOnPlane - rayOrigin)]/planeNormal.rayDirection;
    //pointInPlane = rayOrigin + (rayDirection * T);
    
    float dot1, dot2;
    
    recVec pointOnPlane = {0, 0, 0};
    recVec rayOrigin = CameraPosition;
    recVec planeNormal = {0, 0, -1};
    recVec rayDirection = {ray.x / rayMagnitude, ray.y / rayMagnitude, ray.z / rayMagnitude };
    
    recVec p1 = {pointOnPlane.x - rayOrigin.x, pointOnPlane.y - rayOrigin.y, pointOnPlane.z - rayOrigin.z};
    
    dot1 = (planeNormal.x * p1.x) + (planeNormal.y * p1.y) + (planeNormal.z * p1.z);
    dot2 = (planeNormal.x * rayDirection.x) + (planeNormal.y * rayDirection.y) + (planeNormal.z * rayDirection.z);
    
    float t = dot1/dot2;
    
    rayDirection.x *= t;
	rayDirection.y *= t;
	rayDirection.z *= t;
    
    recVec pointInPlane = {CameraPosition.x +rayDirection.x, CameraPosition.y +rayDirection.y, CameraPosition.z +rayDirection.z};
	
	recVec outPosition = {pointInPlane.x, pointInPlane.y, 0.0};
	return outPosition;*/
	/*glColor3f(255, 255, 0);
	drawSphere(fx, fy, 0.0, 1, 10, 10);
	outPosition.x = fx;
	outPosition.y = fy;
	outPosition.z = 0;
	
	glColor3f(255, 0, 0);*/
}
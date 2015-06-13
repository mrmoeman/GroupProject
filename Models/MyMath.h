#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

XMMATRIX RotateToFace(XMFLOAT3 Point, XMFLOAT3 Face);
	
float RotateYToFace(XMFLOAT3 Point, XMFLOAT3 Face);

float CalculateLength(XMFLOAT3 PointA, XMFLOAT3 PointB);

float TwoDimensionalLength(XMFLOAT2 PointA, XMFLOAT2 PointB);

XMFLOAT3 DirectionFrom(XMFLOAT3 PointA, XMFLOAT3 PointB);

bool AreParralel(XMFLOAT3 LineA, XMFLOAT3 LineB);

bool IsPointInCube(XMFLOAT3 Point, XMFLOAT3 CubeCenter, float Width);

float TrinagleArea(XMFLOAT3 VA, XMFLOAT3 VB, XMFLOAT3 VC);

bool IsPointInTriangle(XMFLOAT3 TriPointA, XMFLOAT3 TriPointB, XMFLOAT3 TriPointC, XMFLOAT3 Point);

XMFLOAT3 VectorBetween(XMFLOAT3 PointA, XMFLOAT3 PointB);

bool IsPointInCylinder(XMFLOAT3 Point, XMFLOAT3 CylinderBase, float Height, float Width);

float DegreesToRadians(float AngleInDegrees);

float RadiansToDegrees(float AngleInRadians);

XMFLOAT3 PointWhereLineIntersectWithPlane(XMFLOAT3 LineOrigin, XMFLOAT3 LineVector, XMFLOAT3 PlanePoint, XMFLOAT3 PlaneNormal);

XMFLOAT3 CalculateTrianlgeNormal(XMFLOAT3 TriPointA,XMFLOAT3 TriPointB, XMFLOAT3 TriPointC);

XMFLOAT2 AngleToHeadingVector(float AngleInRadians);

XMFLOAT3 NormaliseMe(XMFLOAT3 Vector);

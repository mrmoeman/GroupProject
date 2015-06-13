#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include "MyMath.h"

XMMATRIX RotateToFace(XMFLOAT3 Point, XMFLOAT3 Face){

	float deltaY = Face.z - Point.z;
	float deltaX = Face.x - Point.x;

	float Y_Rot = atan2(deltaX, deltaY);


	deltaY = Face.y - Point.y;
	deltaX = hypot(Face.x - Point.x, Face.z - Point.z);

	float X_Rot = atan2(deltaX, deltaY);

	return XMMatrixRotationRollPitchYaw(X_Rot, Y_Rot,0);

}
//returns a matrix which will rotate pointA to point towards face.

float RotateYToFace(XMFLOAT3 Point, XMFLOAT3 Face){
	float deltaY = Face.z - Point.z;
	float deltaX = Face.x - Point.x;

	return atan2(deltaX, deltaY);
}

float CalculateLength(XMFLOAT3 PointA, XMFLOAT3 PointB){

	float HypXZ = hypot(PointA.x - PointB.x, PointA.z - PointB.z);

	return hypot(HypXZ, PointA.y - PointB.y);

}
//returns the length between PointA and PointB

float TwoDimensionalLength(XMFLOAT2 PointA, XMFLOAT2 PointB){

	return hypot(PointA.x - PointB.x, PointA.y - PointB.y);

}
//returns the length between PointA and PointB

XMFLOAT3 DirectionFrom(XMFLOAT3 PointA, XMFLOAT3 PointB){

	XMFLOAT3 Between;
	Between.x = PointA.x - PointB.x;
	Between.y = PointA.y - PointB.y;
	Between.z = PointA.z - PointB.z;

	return NormaliseMe(Between);

}
//returns a direction vector between PointA and PointB

bool AreParralel(XMFLOAT3 LineA, XMFLOAT3 LineB){

	if (LineA.x / LineB.x == LineA.y/LineB.y && LineA.z/LineB.z == LineA.y/LineB.y){
			return true;
		}
		else return false;

}
//returns whether two vectors are parralel

bool IsPointInCube(XMFLOAT3 Point, XMFLOAT3 CubeCenter, float Width){

	if(Point.x > CubeCenter.x - Width && Point.x < CubeCenter.x + Width){
			if(Point.y > CubeCenter.y - Width && Point.y < CubeCenter.y + Width){
				if(Point.z > CubeCenter.z - Width && Point.z < CubeCenter.z + Width){
					return true;
				}
				else{
					return false;
				}
			}
			else{
			return false;
			}
		}
	else{
		return false;
	}

}
//retruns whether a point in inside a cube

float TrinagleArea(XMFLOAT3 TriPointA, XMFLOAT3 TriPointB, XMFLOAT3 TriPointC){
		
		double AB = CalculateLength(TriPointA, TriPointB);
		double AC = CalculateLength(TriPointA, TriPointC);
		double BC = CalculateLength(TriPointB, TriPointC);
		
		double P = (AB + AC + BC)/2;
		
		double area =  sqrt(P*(P-AB)*(P-BC)*(P-AC)); 

		return area;
	}
//returns the area of a triange

bool IsPointInTriangle(XMFLOAT3 TriPointA, XMFLOAT3 TriPointB, XMFLOAT3 TriPointC, XMFLOAT3 Point){
		
		double ABC = TrinagleArea(TriPointA, TriPointB, TriPointC);
		double ABP = TrinagleArea(TriPointA, TriPointB, Point);
		double APC = TrinagleArea(TriPointA, Point, TriPointC);
		double PBC = TrinagleArea(Point, TriPointB, TriPointC);
		
		if ((ABP + APC + PBC) < ABC){
			return true;
		}
		else{
			return false;
		}
	}
//returns wther a point is inside a flat triangle

XMFLOAT3 VectorBetween(XMFLOAT3 PointA, XMFLOAT3 PointB){
	return  XMFLOAT3(PointA.x - PointB.x, PointA.y - PointB.y, PointA.z - PointB.z);
}
//returns the vector between two points

bool IsPointInCylinder(XMFLOAT3 Point, XMFLOAT3 CylinderBase, float Height, float Width){
		if(TwoDimensionalLength(XMFLOAT2(Point.x, Point.z), XMFLOAT2(CylinderBase.x, CylinderBase.z)) <= Width){
			if(Point.y >= CylinderBase.y && Point.y <= CylinderBase.y + Height){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
//returns whether a point is inside a cylinder

float DegreesToRadians(float AngleInDegrees){

	return AngleInDegrees * 180/XM_PI;

}
//returns an angle in radians

float RadiansToDegrees(float AngleInRadians){

	return AngleInRadians * XM_PI/180;

}
//returns an angle in degrees

XMFLOAT3 PointWhereLineIntersectWithPlane(XMFLOAT3 LineOrigin, XMFLOAT3 LineVector, XMFLOAT3 PlanePoint, XMFLOAT3 PlaneNormal){
		
		double PlaneNP = PlaneNormal.x * PlanePoint.x + PlaneNormal.y * PlanePoint.y +PlaneNormal.z * PlanePoint.z;
		double PlaneNLineP = PlaneNormal.x * LineOrigin.x + PlaneNormal.y * LineOrigin.y + PlaneNormal.z * LineOrigin.z;
		double PlaneNLineV = PlaneNormal.x * LineVector.x + PlaneNormal.y * LineVector.y + PlaneNormal.z * LineVector.z;
		double LineMagnitude = (PlaneNP - PlaneNLineP)/PlaneNLineV;
		
		XMFLOAT3 IntersectPoint;
		IntersectPoint.x = (float) (LineOrigin.x + LineVector.x * LineMagnitude);
		IntersectPoint.y = (float) (LineOrigin.y + LineVector.y * LineMagnitude);
		IntersectPoint.z = (float) (LineOrigin.z + LineVector.z * LineMagnitude);
		
		return IntersectPoint;
	}
//given a line and a point on that line this will return the point at which said line intersects with a plane with point planepoint and a normal of PlaneNormal

XMFLOAT3 CalculateTrianlgeNormal(XMFLOAT3 TriPointA,XMFLOAT3 TriPointB, XMFLOAT3 TriPointC){
		
		XMFLOAT3 U;
		XMFLOAT3 V;
		U.x = TriPointA.x - TriPointB.x;
		U.y = TriPointA.y - TriPointB.y;
		U.z = TriPointA.z - TriPointB.z;

		V.x = TriPointA.x - TriPointC.x;
		V.y = TriPointA.y - TriPointC.y;
		V.z = TriPointA.z - TriPointC.z;
		
		XMFLOAT3 Normal;
		Normal.x = U.y * V.z - U.z * V.y;
		Normal.y = U.z * V.x - U.x * V.z;
		Normal.z = U.x * V.y - U.y * V.x;
		
		return Normal;
	}
//calculates the normal of a triangle

XMFLOAT2 AngleToHeadingVector(float AngleInRadians){
		float XHeading = (float) cos(AngleInRadians);
		float YHeading = (float) sin(AngleInRadians);
		return XMFLOAT2(XHeading,YHeading);
	}
//returns a 2D heading vector from an angle in radians

XMFLOAT3 NormaliseMe(XMFLOAT3 Vector){

	float length = sqrt(Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z);
	
	return XMFLOAT3 (Vector.x/length, Vector.y/length, Vector.z/length);

}
//normalises a XMFLOAT3

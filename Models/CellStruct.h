///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//					CU6005 Prototype Development                                 //
// Student Names: Andrew L. Braid, Christopher C. McAuliffe, Tyanah K. O. Graham //
// Student ID's: 12032769, 12054673, 12023366									 //
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

#pragma once

// INCLUDES //
#include <math.h>
// END: INCLUDES //

#define GRID_SIZE 40 // Sets a grid size. Each point is 100...

/* CELL STRUCT:
 * This header file degines the structure of data
 * in each cell and is used primarily for path finding. Each cell
 * is required to have data such as ID, cumulative cost, heuristic, it's 
 * parent cell, and manhattan distance.
 */
struct CellStruct
{
	public:
		int xPos;
		int zPos;
		int cellID;
		CellStruct* parent;
		float cumulativeCost;
		float heuristic;

		CellStruct(): parent(0) 
		{
		
		};

		CellStruct(int x, int z, CellStruct*_parent = 0) : xPos(x), zPos(z), parent(_parent), cellID(z * GRID_SIZE + x), cumulativeCost(0), heuristic(0)
		{

		};

		float evaluate()
		{
			return cumulativeCost + heuristic; 
		};

		float ManhattanDistance(CellStruct* nodeEnd)
		{
			float a = (float)(fabs((float)(this->xPos - nodeEnd->xPos)));
			float b = (float)(fabs((float)(this->zPos - nodeEnd->zPos)));
			
			int z;

			if(a <= b)
			{
				z = a;
			}
			else 
			{
				z = b;
			}

			return 1 * (a + b) + (1.414 - 2 * 1) * z;
		}
};

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
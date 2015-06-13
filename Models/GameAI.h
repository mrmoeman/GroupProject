///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//					CU6005 Prototype Development                                 //
// Student Names: Andrew L. Braid, Christopher C. McAuliffe, Tyanah K. O. Graham //
// Student ID's: 12032769, 12054673, 12023366									 //
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

#pragma once

// INCLUDES //
#include "CellStruct.h"
#include <vector>

#include "Dx11DemoBase.h"
#include "DisplayObject.h"
#include "LevelArray.h"
#include <xnamath.h>
// END: INCLUDES //

/* 
 * This struct contains all the data
 * needed for the A.I. agents to make
 * descisions such as relaying the grid
 * positions and checking the goal states.
 */
struct brainWaves
{
	int rangedX;
	int rangedZ;
	int findRangedX;
	int findRangedZ;
	int gridX;
	int gridZ;
	int nextGridX;
	int nextGridZ;
	int prevGridX;
	int prevGridZ;
	int leaderGridX;
	int leaderGridZ;
	int previousIndex;
	int findLeaderX;
	int findLeaderZ;
	int attackPlayerX;
	int attackPlayerZ;
	int attackID[8];
	int relativeGridX;
	int relativeGridZ;
	int relativeLeaderGridX;
	int relativeLeaderGridZ;
	int followRelLeaderGridX;
	int followRelLeaderGridZ;

	float rotX;
	float rotZ;
	float rotY;
	float manDistPlayer;
	float relativePosX;
	float relativePosZ;

	bool firstIteration;
	bool idle;
	bool initialisedStartGoal;
	bool foundGoal;
};

/* GAMEAI:
 * This class contains the artificial 'brain' behind
 * all the A.I. controlled enemy agents. 
 */
class GameAI
{
	public: 
		GameAI(void);
		~GameAI(void);
		
		void FindPath(int goalX, int goalZ, brainWaves* objectProperties);
		
		/* LISTS */
		void ClearOpenList()
		{
			openList.clear();
		}
		void ClearVisitedList()
		{
			visitedList.clear();
		}
		void ClearPathToGoal()
		{
			pathToGoal.clear();
		}
		/* END: LISTS */

		// ENEMY STATES //
		enum myState 
		{
			ATTACK,
			LEADER,
			SWARM,
			RANGED,
			FLEE,
			STEAL,
			IDLE,
			DEAD
		};

		myState AIState;	// Creates an instance of the AI enum states...
		// END: ENEMY STATES //

		int localArray[60][60];
	
	private:
		void SetStartAndGoal(CellStruct start, CellStruct goal);
		void PathOpened(int x, int z, float newCost, CellStruct* parent, brainWaves* objectProperties);
		void ContinuePath(brainWaves* objectProperties);
		
		CellStruct* GetNextCell();
		CellStruct* startCell;
		CellStruct* goalCell;
		CellStruct* currentCell;
		CellStruct* getPath;
		CellStruct start;
		CellStruct goal;
		CellStruct* newChild;
		CellStruct path[100];
		CellStruct prevPath[100];
		CellStruct cells[100][100];
		CellStruct* nextCell;


		float newEvaluation;
		float bestEvaluation;

		int bestHeuristic;
		int cellIndex;
		int _cellID;
		
		std::vector<CellStruct*> openList;
		std::vector<CellStruct*> visitedList;
		std::vector<XMFLOAT3*> pathToGoal;
};

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
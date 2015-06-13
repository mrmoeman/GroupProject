///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//					CU6005 Prototype Development                                 //
// Student Names: Andrew L. Braid, Christopher C. McAuliffe, Tyanah K. O. Graham //
// Student ID's: 12032769, 12054673, 12023366									 //
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

// INCLUDES //
#include "GameAI.h"
// END: INCLUDES //

GameAI::GameAI(void)
{
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			localArray[i][j] = 0;

		}

	}
}

GameAI::~GameAI(void)
{

}

/* FIND PATH:
 * Sets the start and goal grid positions and clears
 * any existing data from previous paths.
 */
void GameAI::FindPath(int goalX, int goalZ, brainWaves* objectProperties)
{
	/*for (int i = 20; i < 40; i++)
	{
		if (i +  objectProperties->gridX < GRID_SIZE)
		{
			for (int j = 0; j < 40; j++)
			{
				if (j +  objectProperties->gridZ < GRID_SIZE)
				{
					localArray[i][j] = levelArray[objectProperties->gridX + i][objectProperties->gridZ + j];
				}
				else localArray[i][j] = 1;

			}
		}
		else
			for (int j = 0; j < 40; j++)
			{
				localArray[i][j] = 1;
			}

	}

	for (int i = 19; i > -1; i--)
	{
		if (i +  objectProperties->gridX > 0)
		{
			for (int j = 39; j > -1; j--)
			{
				if (j +  objectProperties->gridZ > 0)
				{
					localArray[i][j] = levelArray[objectProperties->gridX + i][objectProperties->gridZ + j];
				}
				else localArray[i][j] = 1;
			}
		}
		else
			for (int j = 39; j > -1; j--)
			{
				localArray[i][j] = 1;
			}

	}*/
	/*int a = -20;
	int b = -20;
	for (int i = objectProperties->relativeGridX - 20; i < objectProperties->relativeGridX + 20; i++)
	{
		if (a +  objectProperties->gridX < widthIndex && a +  objectProperties->gridX >= 0)
		{
			for (int j =  objectProperties->relativeGridZ - 20; j < objectProperties->relativeGridZ + 20; j++)
			{
				if (b +  objectProperties->gridZ < heightIndex && b +  objectProperties->gridZ >= 0)
				{
					if(i >= 0 && j >= 0)
					localArray[i][j] = levelArray[objectProperties->gridX + a][objectProperties->gridZ + b];
				}
				else 
				{	
					if(i >= 0 && j >= 0)
					localArray[i][j] = 1;
				}
				b++;
			}
		}
		else
		{
			for (int j =  objectProperties->relativeGridZ - 20; j < objectProperties->relativeGridZ + 20; j++)
			{
				if(i >= 0 && j >= 0)
				localArray[i][j] = 1;
			}
		}
			a++;
			b = -20;
	}*/

	goalX = (int)(goalX / 100);
	goalZ = (int)(goalZ / 100);

	// Clears the vectors of their current data //
	if(!objectProperties->initialisedStartGoal)
	{
		objectProperties->foundGoal = false;

		for (int i = 0; i < openList.size(); i++)
		{
			delete openList[i];
		}
		openList.clear();

		for (int i = 0; i < visitedList.size(); i++)
		{
			delete visitedList[i];
		}
		visitedList.clear();

		for (int i = 0; i < pathToGoal.size(); i++)
		{
			delete pathToGoal[i];
		}
		pathToGoal.clear();
		
		start.xPos = objectProperties->relativeGridX;
		start.zPos = objectProperties->relativeGridZ;	
		
		goal.xPos = goalX;
		goal.zPos = goalZ;

		SetStartAndGoal(start, goal);
		objectProperties->initialisedStartGoal = true;
	}

	// Continues the path //
	if(objectProperties->initialisedStartGoal)
	{
		ContinuePath(objectProperties);
	}

}

/* SET START AND GOAL:
 * Assisgns Id's to the starting goal cells 
 * in order to begin the pathfinding.
 */
void GameAI::SetStartAndGoal(CellStruct start, CellStruct goal)
{
	startCell = new CellStruct(start.xPos, start.zPos, NULL);
	goalCell = new CellStruct(goal.xPos, goal.zPos, &goal);

	startCell->cumulativeCost = 0;
	startCell->heuristic = startCell->ManhattanDistance(goalCell);
	startCell->parent = 0;

	openList.push_back(startCell);
}

/* GET NEXT CELL:
 * Assigns a new Id to the next
 * cell in the path and connects it to the previous
 * parent cell.
 */
CellStruct* GameAI::GetNextCell()
{
	// bestEvaluation = 999999.0f;

	cellIndex = -1;
	nextCell = NULL;
	bestHeuristic = 999;

	for (int i = 0; i < openList.size(); i++)
	{
		/*if(openList[i]->evaluate() < bestEvaluation)
		{
			bestEvaluation = openList[i]->evaluate();
			cellIndex = i;
		}*/

		if(openList[i]->heuristic < bestHeuristic)
		{
			bestHeuristic = openList[i]->heuristic;
			cellIndex = i;
		}

	}

	if(cellIndex >= 0)
	{
		nextCell = openList[cellIndex];
		visitedList.push_back(nextCell);
		openList.erase(openList.begin() + cellIndex);
	}

	return nextCell;
}

/* PATH OPENED:
 * Checks for the next best cell in the
 * surrounding area, based on the heuristic calculated by
 * Cheby Shev distance. It also checks the occupancy of the cells,
 * in case there is another object or enemy.
 */
void GameAI::PathOpened(int x, int z, float newCost, CellStruct* parent, brainWaves* objectProperties)
{
	if(x < 0 || z < 0 || x > 40 || z > 40)
	{
		return;
	}

	/*
	if(levelArray[x][z] == 1 || levelArray[x+1][z] == 1 || levelArray[x][z+1] == 1 || levelArray[x-1][z] == 1
	|| levelArray[x][z-1] == 1 || levelArray[x+1][z+1] == 1 || levelArray[x-1][z+1] == 1 || levelArray[x+1][z-1] == 1 
	|| levelArray[x-1][z-1] == 1)*/
	
	// Checks if the space is occupied //
	/*if(localArray[x][z] == 1)
	{ 
		return;
	}*/

	/*if(levelArray[objectProperties->gridX - start.xPos + x][objectProperties->gridZ - start.zPos + z] != 0)
	{
		return;
	}*/

	_cellID = z * GRID_SIZE + x;

	for(int i = 0; i < visitedList.size(); i++)
	{
		if(_cellID == visitedList[i]->cellID)
		{
			return;
		}
	}

	newChild = new CellStruct(x, z, parent);
	newChild->cumulativeCost = newCost;
	newChild->heuristic = parent->ManhattanDistance(goalCell);

	for(int i = 0; i < openList.size(); i++)
	{
		if(_cellID == openList[i]->cellID)
		{
			newEvaluation = newChild->cumulativeCost + newCost + openList[i]->heuristic;
			if(openList[i]->evaluate() > newEvaluation)
			{
				openList[i]->cumulativeCost = newChild->cumulativeCost + newCost;
				openList[i]->parent = newChild;
			}
			else
			{
				return;
			}
			
		}
	}

	openList.push_back(newChild);
}

/* CONTINUE PATH:
 * This is a loop which drives the path finding. It assigns
 * cell Id's by calling getNextCell and sends off neighbouring cells
 * to be evaluated by PathOpened. Once an optimal path is found, it will
 * assign each cell in the path to the path array and change the brainWave
 * nextCell value to move the enemy to the next path.
 */
void GameAI::ContinuePath(brainWaves* objectProperties)
{
	if(openList.empty())
	{
		return;
	}

	currentCell = GetNextCell();

	if(currentCell->cellID == goalCell->cellID)
	{
		goalCell->parent = currentCell->parent;
		
		int i = 0;

		for(getPath = goalCell; getPath != NULL; getPath = getPath->parent)
		{
			// pathToGoal.push_back(new XMFLOAT3(getPath->xPos, 0, getPath->zPos));

			path[i].xPos = getPath->xPos;
			path[i].zPos = getPath->zPos;

			i++;
		}

		// objectProperties->nextGridX = pathToGoal[pathToGoal.size() - 2]->x;
		// objectProperties->nextGridZ = pathToGoal[pathToGoal.size() - 2]->z;

		objectProperties->nextGridX = path[i-2].xPos;
		objectProperties->nextGridZ = path[i-2].zPos;

		objectProperties->initialisedStartGoal = false;
		objectProperties->foundGoal = true;
		return;		
	}
	
	else
	{
		//Right
		PathOpened(currentCell->xPos + 1, currentCell->zPos, currentCell->cumulativeCost + 1, currentCell, objectProperties);
		//Left
		PathOpened(currentCell->xPos - 1, currentCell->zPos, currentCell->cumulativeCost + 1, currentCell, objectProperties);
		//Above
		PathOpened(currentCell->xPos, currentCell->zPos + 1, currentCell->cumulativeCost + 1, currentCell, objectProperties);
		//Below
		PathOpened(currentCell->xPos, currentCell->zPos - 1, currentCell->cumulativeCost + 1, currentCell, objectProperties);
		//Up Right
		PathOpened(currentCell->xPos + 1, currentCell->zPos + 1, currentCell->cumulativeCost + 1.414, currentCell, objectProperties);
		//Up Left
		PathOpened(currentCell->xPos - 1, currentCell->zPos + 1, currentCell->cumulativeCost + 1.414, currentCell, objectProperties);
		//Down Left
		PathOpened(currentCell->xPos - 1, currentCell->zPos - 1, currentCell->cumulativeCost + 1.414, currentCell, objectProperties);
		//Down Right
		PathOpened(currentCell->xPos + 1, currentCell->zPos - 1, currentCell->cumulativeCost + 1.414, currentCell, objectProperties);

		for(int i = 0; i < openList.size(); i++)
		{
			if(currentCell->cellID == openList[i]->cellID)
			{
				openList.erase(openList.begin() + i);
			}
		}

		if(!objectProperties->foundGoal) ContinuePath(objectProperties);
	}

}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
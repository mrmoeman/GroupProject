///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//					CU6005 Prototype Development                                 //
// Student Names: Andrew L. Braid, Christopher C. McAuliffe, Tyanah K. O. Graham //
// Student ID's: 12032769, 12054673, 12023366									 //
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

// INCLUDES //
#include "SmartGameObject.h"
#include <dinput.h>
// END: INCLUDES //

/* INITIALISER:
 * Initialises all the data variables of
 * the brainFreeze struct for use.
 */
SmartGameObject::SmartGameObject(void)
{
	brain = new GameAI();
	brain->AIState = brain->SWARM;

	brainFreeze.initialisedStartGoal = false;
	brainFreeze.foundGoal = false;
	brainFreeze.gridX = abs(floor(getX() / 100));
	brainFreeze.gridZ = abs(floor(getZ() / 100));
	brainFreeze.prevGridX = abs(floor(getX() / 100));
	brainFreeze.prevGridZ = abs(floor(getZ() / 100));
	brainFreeze.leaderGridX = 1;
	brainFreeze.leaderGridZ = 1;
	brainFreeze.firstIteration = false;

	// Loops through the attack array and sets the values to 0 //
	for(int attackI = 0; attackI < 8; attackI++)
	{
		brainFreeze.attackID[attackI] = 0;
	}
}

SmartGameObject::~SmartGameObject(void)
{
	//int ++ [] = {++, ++, ++, ++, ++, ++, ++, ++, ++, ++, ++, ++, ++};
}

/* RETURN MANHATTAN DISTANCE:
 * Returns the distance between the player and the enemy
 * objects.
 */
void SmartGameObject::returnManhattanDistance(XMFLOAT3 _position)
{
	float distance;
	int playerX = abs(floor(_position.x / 100));
	int playerZ = abs(floor(_position.z / 100));

	int dx = playerX - brainFreeze.gridX;
	int dz = playerZ - brainFreeze.gridZ;
	distance = sqrtf(dx*dx + dz*dz);

	brainFreeze.manDistPlayer = distance;
}

/* RETURN MANHATTAN DISTANCE:
 * Returns the distance between the enemy and a specific point or
 * object.
 */
float SmartGameObject::returnManhattanDistance(int x, int z)
{
	float distance;

	int dx = x - brainFreeze.gridX;
	int dz = z - brainFreeze.gridZ;
	distance = sqrtf(dx*dx + dz*dz);

	return distance;
}

/* RETURN MANHATTAN DISTANCE:
 * Returns the distance between the player and a specific object.
 */
float SmartGameObject::returnManhattanDistance(int x, int z, XMFLOAT3 _playerPos)
{
	float distance;
	int playerX = abs(floor(_playerPos.x / 100));
	int playerZ = abs(floor(_playerPos.z / 100));

	int dx = x - playerX;
	int dz = z - playerZ;
	distance = sqrtf(dx*dx + dz*dz);

	return distance;
}

/* UPDATE GRID POSITION:
 * Updates the position for all AI Agents of the LEADER.
 * It also assigns cells occupied by the AI agents as occupied.
 * Also aids the agents in finding an unoccupied state when in different
 * behaviours such as SWARM or ATTACK. 
 */
void SmartGameObject::updateGridPosition(XMFLOAT3 _playerPos)
{
	// Sets the leader grid positions //
	if(brain->AIState == brain->LEADER)
	{
		brainFreeze.leaderGridX = abs(floor(getX() / 100));
		brainFreeze.leaderGridZ = abs(floor(getZ() / 100));
		brainFreeze.relativeLeaderGridX = brainFreeze.relativeGridX;
		brainFreeze.relativeLeaderGridZ = brainFreeze.relativeGridZ;
	}

	/*if(brainFreeze.prevGridX > brainFreeze.gridX || brainFreeze.prevGridX < brainFreeze.gridX || 
		brainFreeze.prevGridZ > brainFreeze.gridZ || brainFreeze.prevGridZ < brainFreeze.gridZ )
	{
		
		levelArray[brainFreeze.prevGridX][brainFreeze.prevGridZ] = 0;
		levelArray[brainFreeze.gridX][brainFreeze.gridZ] = 1;
		brainFreeze.prevGridZ = brainFreeze.gridZ;
		brainFreeze.prevGridX = brainFreeze.gridX;
	}*/

	/*
	float smallestMan = 999999.0f;
	int ID = -1;

	// Modifies the data for the swarming enemies //
	if(brain->AIState == brain->SWARM)
	{
		if (returnManhattanDistance(brainFreeze.leaderGridX + 1, brainFreeze.leaderGridZ) < smallestMan
			&& levelArray[brainFreeze.leaderGridX + 1][brainFreeze.leaderGridZ] == 0)
		{
			smallestMan = returnManhattanDistance(brainFreeze.leaderGridX + 1, brainFreeze.leaderGridZ);
			ID = 1;
		}

		if (returnManhattanDistance(brainFreeze.leaderGridX, brainFreeze.leaderGridZ + 1) < smallestMan
			&& levelArray[brainFreeze.leaderGridX][brainFreeze.leaderGridZ + 1] == 0)
		{
			smallestMan = returnManhattanDistance(brainFreeze.leaderGridX, brainFreeze.leaderGridZ + 1);
			ID = 2;
		}

		if (returnManhattanDistance(brainFreeze.leaderGridX - 1, brainFreeze.leaderGridZ) < smallestMan
			&& levelArray[brainFreeze.leaderGridX - 1][brainFreeze.leaderGridZ] == 0)
		{
			smallestMan = returnManhattanDistance(brainFreeze.leaderGridX - 1, brainFreeze.leaderGridZ);
			ID = 3;
		}

		if (returnManhattanDistance(brainFreeze.leaderGridX, brainFreeze.leaderGridZ - 1) < smallestMan
			&& levelArray[brainFreeze.leaderGridX][brainFreeze.leaderGridZ - 1] == 0)
		{
			smallestMan = returnManhattanDistance(brainFreeze.leaderGridX, brainFreeze.leaderGridZ - 1);
			ID = 4;
		}

		if (returnManhattanDistance(brainFreeze.leaderGridX + 1, brainFreeze.leaderGridZ + 1) < smallestMan
			&& levelArray[brainFreeze.leaderGridX + 1][brainFreeze.leaderGridZ + 1] == 0)
		{
			smallestMan = returnManhattanDistance(brainFreeze.leaderGridX + 1, brainFreeze.leaderGridZ + 1);
			ID = 5;
		}

		if (returnManhattanDistance(brainFreeze.leaderGridX - 1, brainFreeze.leaderGridZ - 1) < smallestMan
			&& levelArray[brainFreeze.leaderGridX - 1][brainFreeze.leaderGridZ - 1] == 0)
		{
			smallestMan = returnManhattanDistance(brainFreeze.leaderGridX - 1, brainFreeze.leaderGridZ - 1);
			ID = 6;
		}

		if (returnManhattanDistance(brainFreeze.leaderGridX - 1, brainFreeze.leaderGridZ + 1) < smallestMan
			&& levelArray[brainFreeze.leaderGridX - 1][brainFreeze.leaderGridZ + 1] == 0)
		{
			smallestMan = returnManhattanDistance(brainFreeze.leaderGridX - 1, brainFreeze.leaderGridZ + 1);
			ID = 7;
		}

		if (returnManhattanDistance(brainFreeze.leaderGridX + 1, brainFreeze.leaderGridZ - 1) < smallestMan
			&& levelArray[brainFreeze.leaderGridX + 1][brainFreeze.leaderGridZ - 1] == 0)
		{
			smallestMan = returnManhattanDistance(brainFreeze.leaderGridX + 1, brainFreeze.leaderGridZ - 1);
			ID = 8;
		}

		// Moves the swarm toward the leader //
		switch (ID)
		{
			case 1:
				brainFreeze.findLeaderX = brainFreeze.leaderGridX + 1;
				brainFreeze.findLeaderZ = brainFreeze.leaderGridZ;
				break;
			case 2:
				brainFreeze.findLeaderX = brainFreeze.leaderGridX;
				brainFreeze.findLeaderZ = brainFreeze.leaderGridZ + 1;
				break;
			case 3:
				brainFreeze.findLeaderX = brainFreeze.leaderGridX - 1;
				brainFreeze.findLeaderZ = brainFreeze.leaderGridZ;
				break;
			case 4:
				brainFreeze.findLeaderX = brainFreeze.leaderGridX;
				brainFreeze.findLeaderZ = brainFreeze.leaderGridZ - 1;
				break;
			case 5:
				brainFreeze.findLeaderX = brainFreeze.leaderGridX + 1;
				brainFreeze.findLeaderZ = brainFreeze.leaderGridZ + 1;
				break;
			case 6:
				brainFreeze.findLeaderX = brainFreeze.leaderGridX - 1;
				brainFreeze.findLeaderZ = brainFreeze.leaderGridZ - 1;
				break;
			case 7:
				brainFreeze.findLeaderX = brainFreeze.leaderGridX - 1;
				brainFreeze.findLeaderZ = brainFreeze.leaderGridZ + 1;
				break;
			case 8:
				brainFreeze.findLeaderX = brainFreeze.leaderGridX + 1;
				brainFreeze.findLeaderZ = brainFreeze.leaderGridZ - 1;
				break;
			default:
				break;
		}

	}

	int playerGridX = abs(floor(_playerPos.x / 100));
	int playerGridZ = abs(floor(_playerPos.z / 100));

	// Modifies the data for the ATTACK enemies. (This occurs when close to player) //
	// Currently we attempted to set different attack ID's to prevent clipping issures
	// however, we have been unable to solve this problem //
	if(brain->AIState == brain->ATTACK)
	{
		if (returnManhattanDistance(playerGridX + 1, playerGridZ) < smallestMan
			&& levelArray[playerGridX + 1][playerGridZ] == 0
			&& brainFreeze.attackID[0] == 0)// || brainFreeze.attackID[0] == 2)
		{
			smallestMan = returnManhattanDistance(playerGridX + 1, playerGridZ);
			ID = 1;
			//brainFreeze.attackID[0] = 2;
		}

		if (returnManhattanDistance(playerGridX, playerGridZ + 1) < smallestMan
			&& levelArray[playerGridX][playerGridZ + 1] == 0
			&& brainFreeze.attackID[1] == 0)// || brainFreeze.attackID[1] == 2)
		{
			smallestMan = returnManhattanDistance(playerGridX, playerGridZ + 1);
			ID = 2;
			//brainFreeze.attackID[1] = 2;
		}

		if (returnManhattanDistance(playerGridX - 1, playerGridZ) < smallestMan
			&& levelArray[playerGridX - 1][playerGridZ] == 0
			&& brainFreeze.attackID[2] == 0)// || brainFreeze.attackID[2] == 2)
		{
			smallestMan = returnManhattanDistance(playerGridX - 1, playerGridZ);
			ID = 3;
			//brainFreeze.attackID[2] = 2;
		}

		if (returnManhattanDistance(playerGridX, playerGridZ - 1) < smallestMan
			&& levelArray[playerGridX][playerGridZ - 1] == 0
			&& brainFreeze.attackID[3] == 0)// || brainFreeze.attackID[3] == 2)
		{
			smallestMan = returnManhattanDistance(playerGridX, playerGridZ - 1);
			ID = 4;
			//brainFreeze.attackID[3] = 2;
		}

		if (returnManhattanDistance(playerGridX + 1, playerGridZ + 1) < smallestMan
			&& levelArray[playerGridX + 1][playerGridZ + 1] == 0
			&& brainFreeze.attackID[4] == 0)// || brainFreeze.attackID[4] == 2)
		{
			smallestMan = returnManhattanDistance(playerGridX + 1, playerGridZ + 1);
			ID = 5;
			//brainFreeze.attackID[4] = 2;
		}

		if (returnManhattanDistance(playerGridX - 1, playerGridZ - 1) < smallestMan
			&& levelArray[playerGridX - 1][playerGridZ - 1] == 0
			&& brainFreeze.attackID[5] == 0)// || brainFreeze.attackID[5] == 2)
		{
			smallestMan = returnManhattanDistance(playerGridX - 1, playerGridZ - 1);
			ID = 6;
			//brainFreeze.attackID[5] = 2;
		}

		if (returnManhattanDistance(playerGridX - 1, playerGridZ + 1) < smallestMan
			&& levelArray[playerGridX - 1][playerGridZ + 1] == 0
			&& brainFreeze.attackID[6] == 0)// || brainFreeze.attackID[6] == 2)
		{
			smallestMan = returnManhattanDistance(playerGridX - 1, playerGridZ + 1);
			ID = 7;
			//brainFreeze.attackID[6] = 2;
		}

		if (returnManhattanDistance(playerGridX + 1, playerGridZ - 1) < smallestMan
			&& levelArray[playerGridX + 1][playerGridZ - 1] == 0
			&& brainFreeze.attackID[7] == 0)// || brainFreeze.attackID[7] == 2)
		{
			smallestMan = returnManhattanDistance(playerGridX + 1, playerGridZ - 1);
			ID = 8;
			//brainFreeze.attackID[7] = 2;
		}

		// Moves the enemies toward the player //
		switch (ID)
		{
			case 1:
				brainFreeze.attackPlayerX = playerGridX + 1;
				brainFreeze.attackPlayerZ = playerGridZ;
				break;
			case 2:
				brainFreeze.attackPlayerX = playerGridX;
				brainFreeze.attackPlayerZ = playerGridZ + 1;
				break;
			case 3:
				brainFreeze.attackPlayerX = playerGridX - 1;
				brainFreeze.attackPlayerZ = playerGridZ;
				break;
			case 4:
				brainFreeze.attackPlayerX = playerGridX;
				brainFreeze.attackPlayerZ = playerGridZ - 1;
				break;
			case 5:
				brainFreeze.attackPlayerX = playerGridX + 1;
				brainFreeze.attackPlayerZ = playerGridZ + 1;
				break;
			case 6:
				brainFreeze.attackPlayerX = playerGridX - 1;
				brainFreeze.attackPlayerZ = playerGridZ - 1;
				break;
			case 7:
				brainFreeze.attackPlayerX = playerGridX - 1;
				brainFreeze.attackPlayerZ = playerGridZ + 1;
				break;
			case 8:
				brainFreeze.attackPlayerX = playerGridX + 1;
				brainFreeze.attackPlayerZ = playerGridZ - 1;
				break;
			default:
				break;
		}


	}

	int largestManhatten = -1;

	// Modifies the data for RANGED enemies //
	if(brain->AIState == brain->RANGED)
	{
		if (returnManhattanDistance(brainFreeze.rangedX + 1, brainFreeze.rangedZ, _playerPos) > largestManhatten
			&& levelArray[brainFreeze.rangedX  + 1][brainFreeze.rangedZ] == 0)
		{
			largestManhatten = returnManhattanDistance(brainFreeze.rangedX  + 1, brainFreeze.rangedZ, _playerPos);
			ID = 1;
		}

		if (returnManhattanDistance(brainFreeze.rangedX , brainFreeze.rangedZ + 1, _playerPos) > largestManhatten
			&& levelArray[brainFreeze.rangedX ][brainFreeze.rangedZ + 1] == 0)
		{
			largestManhatten = returnManhattanDistance(brainFreeze.rangedX , brainFreeze.rangedZ + 1, _playerPos);
			ID = 2;
		}

		if (returnManhattanDistance(brainFreeze.rangedX  - 1, brainFreeze.rangedZ, _playerPos) > largestManhatten
			&& levelArray[brainFreeze.rangedX  - 1][brainFreeze.rangedZ] == 0)
		{
			largestManhatten = returnManhattanDistance(brainFreeze.rangedX  - 1, brainFreeze.rangedZ, _playerPos);
			ID = 3;
		}

		if (returnManhattanDistance(brainFreeze.rangedX , brainFreeze.rangedZ - 1, _playerPos) > largestManhatten
			&& levelArray[brainFreeze.rangedX ][brainFreeze.rangedZ - 1] == 0)
		{
			largestManhatten = returnManhattanDistance(brainFreeze.rangedX , brainFreeze.rangedZ - 1, _playerPos);
			ID = 4;
		}

		if (returnManhattanDistance(brainFreeze.rangedX  + 1, brainFreeze.rangedZ + 1, _playerPos) > largestManhatten
			&& levelArray[brainFreeze.rangedX  + 1][brainFreeze.rangedZ + 1] == 0)
		{
			largestManhatten = returnManhattanDistance(brainFreeze.rangedX  + 1, brainFreeze.rangedZ + 1, _playerPos);
			ID = 5;
		}

		if (returnManhattanDistance(brainFreeze.rangedX  - 1, brainFreeze.rangedZ - 1, _playerPos)  > largestManhatten
			&& levelArray[brainFreeze.rangedX  - 1][brainFreeze.rangedZ - 1] == 0)
		{
			largestManhatten = returnManhattanDistance(brainFreeze.rangedX  - 1, brainFreeze.rangedZ - 1, _playerPos);
			ID = 6;
		}

		if (returnManhattanDistance(brainFreeze.rangedX  - 1, brainFreeze.rangedZ + 1, _playerPos) > largestManhatten
			&& levelArray[brainFreeze.rangedX  - 1][brainFreeze.rangedZ + 1] == 0)
		{
			largestManhatten = returnManhattanDistance(brainFreeze.rangedX  - 1, brainFreeze.rangedZ + 1, _playerPos);
			ID = 7;
		}

		if (returnManhattanDistance(brainFreeze.rangedX  + 1, brainFreeze.rangedZ - 1, _playerPos) > largestManhatten
			&& levelArray[brainFreeze.rangedX  + 1][brainFreeze.rangedZ - 1] == 0)
		{
			largestManhatten = returnManhattanDistance(brainFreeze.rangedX  + 1, brainFreeze.rangedZ - 1, _playerPos);
			ID = 8;
		}

		// Moves the enemy behind an object. (The furthest point from the player) //
		switch (ID)
		{
			case 1:
				brainFreeze.findRangedX = brainFreeze.rangedX + 1;
				brainFreeze.findRangedZ = brainFreeze.rangedZ;
				break;
			case 2:
				brainFreeze.findRangedX =  brainFreeze.rangedX;
				brainFreeze.findRangedZ = brainFreeze.rangedZ + 1;
				break;
			case 3:
				brainFreeze.findRangedX =  brainFreeze.rangedX - 1;
				brainFreeze.findRangedZ = brainFreeze.rangedZ;
				break;
			case 4:
				brainFreeze.findRangedX =  brainFreeze.rangedX;
				brainFreeze.findRangedZ = brainFreeze.rangedZ - 1;
				break;
			case 5:
				brainFreeze.findRangedX =  brainFreeze.rangedX + 1;
				brainFreeze.findRangedZ = brainFreeze.rangedZ + 1;
				break;
			case 6:
				brainFreeze.findRangedX =  brainFreeze.rangedX - 1;
				brainFreeze.findRangedZ = brainFreeze.rangedZ - 1;
				break;
			case 7:
				brainFreeze.findRangedX =  brainFreeze.rangedX - 1;
				brainFreeze.findRangedZ = brainFreeze.rangedZ + 1;
				break;
			case 8:
				brainFreeze.findRangedX =  brainFreeze.rangedX + 1;
				brainFreeze.findRangedZ = brainFreeze.rangedZ - 1;
				break;
			default:
				break;
		}

	}*/
}

/* UPDATE:
 * Provides the update protocols to the enemy agents, based
 * on their current states.
 */
void SmartGameObject::update(XMFLOAT3 _position, float dt)
{
	if(dt > 0.1)
	{
		dt = 0.01;
	}
	
	int playerX = (int)(_position.x / 100);
	int playerZ = (int)(_position.z / 100);

	int playerDiffX = _position.x - 2000;
	int playerDiffZ = _position.z - 2000;
	brainFreeze.relativePosX = getX() - playerDiffX;
	brainFreeze.relativePosZ = getZ() - playerDiffZ;
	brainFreeze.relativeGridX = (int)brainFreeze.relativePosX / 100;
	brainFreeze.relativeGridZ = (int)brainFreeze.relativePosZ / 100;

	brainFreeze.gridX = (int)(getX() / 100);
	brainFreeze.gridZ = (int)(getZ() / 100);

	updateGridPosition(_position);

	if (levelArray[playerX][playerZ] == 0)
	{
		brainFreeze.initialisedStartGoal = false;
		brainFreeze.foundGoal = false;
	}
	
	// STATE MACHINE //
	switch(brain->AIState)
	{
		case(brain->DEAD):
			speed = 0.0;
			break;
		case(brain->IDLE):
			speed = 0.0;
			break;
		case(brain->SWARM):
			speed = 100.0 * dt;

			brain->FindPath(brainFreeze.leaderGridX * 100, brainFreeze.leaderGridZ * 100, &brainFreeze);
			
			if(brainFreeze.nextGridX > brainFreeze.relativeGridX)
			{
				MovingPos(speed + getX(), 0, getZ());
			}
			else if (brainFreeze.nextGridX < brainFreeze.relativeGridX)
			{
				MovingPos(getX() - speed, 0, getZ());
			}
			
			if(brainFreeze.nextGridZ > brainFreeze.relativeGridZ)
			{
				MovingPos(getX(), 0, speed + getZ());
			}
			else if (brainFreeze.nextGridZ < brainFreeze.relativeGridZ)
			{
				MovingPos(getX(), 0, getZ() - speed);
			}

			brainFreeze.idle = false; // Used to stop the AI from functioning...

			//MovingPos(brainFreeze.testX, 0, brainFreeze.testZ);
			//Pos.z = brainFreeze.testZ;
			//Rot.x = brainFreeze.rotX;
			//Rot.z = brainFreeze.rotZ;
			//Rot.y = brainFreeze.rotY;
			//brainFreeze.nextGridPos = brain->NextPathPos(Pos);
			//brainFreeze.initialisedStartGoal = false;
			//brainFreeze.foundGoal = false;
			//brain->FindPath(2000, 2000, &brainFreeze);

			break;
		case(brain->FLEE):
			speed = 300.0 * dt;

			brainFreeze.initialisedStartGoal = false;
			brainFreeze.foundGoal = false;

			brain->FindPath(getX() - 10, getZ() - 10, &brainFreeze);

			if(brainFreeze.nextGridX > brainFreeze.gridX)
			{
				MovingPos(speed + newX, 0, getZ());
			}
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			{
				MovingPos(getX(), 0, speed + newZ);
			}
			if(brainFreeze.nextGridX < brainFreeze.gridX)
			{
				MovingPos(newX - speed, 0, getZ());
			}
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			{
				MovingPos(getX(), 0, newZ - speed);
			}

			brainFreeze.idle = false; // Used to stop the AI from functioning...
			
			//Pos.z = brainFreeze.testZ;
			//Rot.x = brainFreeze.rotX;
			//Rot.z = brainFreeze.rotZ;
			//Rot.y = brainFreeze.rotY;
			//brainFreeze.nextGridPos = brain->NextPathPos(Pos);

			break;
		case(brain->LEADER):
			speed = 100.0 * dt;

			if (returnManhattanDistance(playerX, playerZ) < 20.0f)
			{
				brain->FindPath(2000, 2000, &brainFreeze);
			}
			
			if(brainFreeze.nextGridX > brainFreeze.relativeGridX)
			{
				MovingPos(speed + getX(), 0, getZ());
			}
			else if (brainFreeze.nextGridX < brainFreeze.relativeGridX)
			{
				MovingPos(getX() - speed, 0, getZ());
			}
			
			if(brainFreeze.nextGridZ > brainFreeze.relativeGridZ)
			{
				MovingPos(getX(), 0, speed + getZ());
			}
			else if (brainFreeze.nextGridZ < brainFreeze.relativeGridZ)
			{
				MovingPos(getX(), 0, getZ() - speed);
			}

			brainFreeze.idle = false; // Used to stop the AI from functioning...

			//else MovingPos(getX(), 0, getZ());
			//MovingPos(brainFreeze.testX, 0, brainFreeze.testZ);
			//Pos.z = brainFreeze.testZ;
			//Rot.x = brainFreeze.rotX;
			//Rot.z = brainFreeze.rotZ;
			//Rot.y = brainFreeze.rotY;
			//brainFreeze.nextGridPos = brain->NextPathPos(Pos);
			//brainFreeze.initialisedStartGoal = false;
			//brainFreeze.foundGoal = false;
			//brain->FindPath(2000, 2000, &brainFreeze);

			break;
		case(brain->ATTACK):
			speed = 100.0 * dt;

			//brain->FindPath(brainFreeze.attackPlayerX * 100, brainFreeze.attackPlayerZ * 100, &brainFreeze);
			brain->FindPath(2000, 2000, &brainFreeze);
			
			if(brainFreeze.nextGridX > brainFreeze.relativeGridX)
			{
				MovingPos(speed + getX(), 0, getZ());
			}
			else if (brainFreeze.nextGridX < brainFreeze.relativeGridX)
			{
				MovingPos(getX() - speed, 0, getZ());
			}

			if(brainFreeze.nextGridZ > brainFreeze.relativeGridZ)
			{
				MovingPos(getX(), 0, speed + getZ());
			}
			else if (brainFreeze.nextGridZ < brainFreeze.relativeGridZ)
			{
				MovingPos(getX(), 0, getZ() - speed);
			}

			brainFreeze.idle = false; // Used to stop the AI from functioning...

			//MovingPos(brainFreeze.testX, 0, brainFreeze.testZ);
			//Pos.z = brainFreeze.testZ;
			//Rot.x = brainFreeze.rotX;
			//Rot.z = brainFreeze.rotZ;
			//Rot.y = brainFreeze.rotY;
			//brainFreeze.nextGridPos = brain->NextPathPos(Pos);
			//brainFreeze.initialisedStartGoal = false;
			//brainFreeze.foundGoal = false;
			//brain->FindPath(2000, 2000, &brainFreeze);

			break;
		case(brain->STEAL):
			speed = 300.0 * dt;

			brainFreeze.initialisedStartGoal = false;
			brainFreeze.foundGoal = false;

			brain->FindPath(getX() - 10, getZ() - 10, &brainFreeze);

			if(brainFreeze.nextGridX > brainFreeze.gridX)
			{
				MovingPos(speed + newX, 0, getZ());
			}
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			{
				MovingPos(getX(), 0, speed + newZ);
			}
			if(brainFreeze.nextGridX < brainFreeze.gridX)
			{
				MovingPos(newX - speed, 0, getZ());
			}
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			{
				MovingPos(getX(), 0, newZ - speed);
			}

			brainFreeze.idle = false; // Used to stop the AI from functioning...

			//Pos.z = brainFreeze.testZ;
			//Rot.x = brainFreeze.rotX;
			//Rot.z = brainFreeze.rotZ;
			//Rot.y = brainFreeze.rotY;
			//brainFreeze.nextGridPos = brain->NextPathPos(Pos);
			
			break;
		//case(brain->RANGED):
			/*speed = 1.0;

			brainFreeze.gridX = (int)(getX() / 100);
			brainFreeze.gridZ = (int)(getZ() / 100);

			brain->FindPath(brainFreeze.findRangedX * 100, brainFreeze.findRangedZ * 100, &brainFreeze);
			
			if(brainFreeze.nextGridX > brainFreeze.gridX)
			{
				MovingPos(speed + getX(), 0, getZ());
			}
			else if (brainFreeze.nextGridX < brainFreeze.gridX)
			{
				MovingPos(getX() - speed, 0, getZ());
			}
			
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			{
				MovingPos(getX(), 0, speed + getZ());
			}
			else if (brainFreeze.nextGridZ < brainFreeze.gridZ)
			{
				MovingPos(getX(), 0, getZ() - speed);
			}

			brainFreeze.idle = false; // Used to stop the AI from functioning...

			//MovingPos(brainFreeze.testX, 0, brainFreeze.testZ);
			//Pos.z = brainFreeze.testZ;
			//Rot.x = brainFreeze.rotX;
			//Rot.z = brainFreeze.rotZ;
			//Rot.y = brainFreeze.rotY;
			//brainFreeze.nextGridPos = brain->NextPathPos(Pos);
			//brainFreeze.initialisedStartGoal = false;
			//brainFreeze.foundGoal = false;
			//brain->FindPath(2000, 2000, &brainFreeze);

			break;*/
	}
}

// UNUSED FUNCTION //
/*
void SmartGameObject::update(MovingGameObject* object)
{
	//updateGridPosition();
	// CCT //
	switch(brain->AIState)
	{
		case(brain->DEAD):			// DEAD
			speed = 0.0;
			break;
		case(brain->IDLE):			// IDLE
			speed = 0.0;
			break;
		case(brain->SWARM):			// ATTACK / CHASE
			speed = 0.2;
			newX = getX();
			newZ = getZ();
			brainFreeze.gridX = abs(floor(getX() / 100));
			brainFreeze.gridZ = abs(floor(getZ() / 100));
			brainFreeze.initialisedStartGoal = false;
			brainFreeze.foundGoal = false;
			//brain->FindPath(object->getX(), object->getZ(), &brainFreeze);
			brain->FindPath(object->getX(), object->getZ(), &brainFreeze);
			//MovingPos(brainFreeze.testX, 0, brainFreeze.testZ);

			if(brainFreeze.nextGridX > brainFreeze.gridX)
			MovingPos(speed + newX, 0, getZ());
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			MovingPos(getX(), 0, speed + newZ);
			if(brainFreeze.nextGridX < brainFreeze.gridX)
			MovingPos(newX - speed, 0, getZ());
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			MovingPos(getX(), 0, newZ - speed);
			//Pos.z = brainFreeze.testZ;
			//Rot.x = brainFreeze.rotX;
			//Rot.z = brainFreeze.rotZ;
			//Rot.y = brainFreeze.rotY;
			//brainFreeze.nextGridPos = brain->NextPathPos(Pos);

			//brainFreeze.idle = false;
			break;
		case(brain->FLEE):
			speed = 1.0;
			//newX = NEED DISTANCE;
			//newZ = NEED DISTANCE;

			brainFreeze.gridX = abs(floor(getX() / 100));
			brainFreeze.gridZ = abs(floor(getZ() / 100));
			brainFreeze.initialisedStartGoal = false;
			brainFreeze.foundGoal = false;

			brain->FindPath(object->getX(), object->getZ(), &brainFreeze);

			//brain->FindPath(object->getX(), object->getZ(), &brainFreeze);
			
			//MovingPos(brainFreeze.testX, 0, brainFreeze.testZ);

			if(brainFreeze.nextGridX > brainFreeze.gridX)
			MovingPos(speed + newX, 0, getZ());
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			MovingPos(getX(), 0, speed + newZ);
			if(brainFreeze.nextGridX < brainFreeze.gridX)
			MovingPos(newX - speed, 0, getZ());
			if(brainFreeze.nextGridZ > brainFreeze.gridZ)
			MovingPos(getX(), 0, newZ - speed);
			//Pos.z = brainFreeze.testZ;
			//Rot.x = brainFreeze.rotX;
			//Rot.z = brainFreeze.rotZ;
			//Rot.y = brainFreeze.rotY;
			//brainFreeze.nextGridPos = brain->NextPathPos(Pos);

			//brainFreeze.idle = false;
			break;
	}
}*/


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
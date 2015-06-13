#include "GameGrid.h"
#include "GameObject.h"

GameGrid::GameGrid(void)
{
	 
}

void GameGrid::createGrid(int w, int h)
{
	meters = 100;

	totalW = meters * w;
	totalH = meters * h;


	int r = 1;
	int c = 1;
	for(int i = 0; i <= w; i++)
	{
		if(i == w)
		{
			if(r==h)
				break;

			r++;
			c = 1;
			i = 0;
		}
		
		gridArray[c-1][r-1].x = (c-1) * meters;
		gridArray[c-1][r-1].y = (r-1) * meters;
		c++;
	}

}

int GameGrid::getGridPosX(float positionX)
{
	int a = floor(positionX/meters);
	return (a);
}
int GameGrid::getGridPosZ(float positionZ)
{
	int a = floor(positionZ/meters);
	return (a);
}
GameGrid::~GameGrid(void)
{

}
int GameGrid::getW() const
{
	return gameGridW;
}
int GameGrid::getH() const
{
	return gameGridH;
}
void GameGrid::setW(int gridW)
{
	gameGridW = gridW;
}
void GameGrid::setH(int gridH) 
{
	gameGridH = gridH;
}
float GameGrid::getArrayX(int c, int r) const
{
	return gridArray[c][r].x;
}
float GameGrid::getArrayZ(int c, int r) const
{
	return gridArray[c][r].y;
}
int GameGrid::getMeter() const
{
	return meters;
}


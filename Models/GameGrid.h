#include "GameObject.h"

#ifndef _GAME_GRID_H
#define _GAME_GRID_H


#pragma once


class GameGrid
{
public:
	
	GameGrid(void);
	void createGrid(int w, int h);
	virtual ~GameGrid(void);	

	void setW(int gameGridW);
	void setH(int gameGridH);
	void setMeter(int meters);

	float getArrayX(int c, int r) const;
	float getArrayZ(int c, int r) const;
	int getW() const;
	int getH() const;
	int getMeter() const;
	int getTotalW() const;
	int getTotalH() const;
	int getGridPosX(float positionX);
	int getGridPosZ(float positionZ);


private:
	
	int playerW;
	int playerH;
	int grid;
	int meters;			// How many units a meter is 
	int gameGridW;		// Width of grid
	int totalW;			// Total calculation of the grid width, multiplies meters by how many meters there should be 
	int gameGridH;		// Length of grid
	int totalH;			// Total calculation of the grid height, multiplies meters by how many meters there should be
	XMFLOAT2 gridArray[ARRAY_WIDTH][ARRAY_HEIGHT];
	
};


#endif
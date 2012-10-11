#include "Grid.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>


Grid::Grid(void)
{
	// fill for half grid - quarter grid till half grid + quarter grid on both x and y axis
	// for 100x100 grid this means that square 25-75 gets filled
	
	for (int i = (GRID_SIZE/2)-(GRID_SIZE/4); i < (GRID_SIZE/2)+(GRID_SIZE/4); ++i)
	{
		for (int j = (GRID_SIZE/2)-(GRID_SIZE/4); j < (GRID_SIZE/2)+(GRID_SIZE/4); ++j)
		{
			double choice = (double)rand() / (RAND_MAX + 1.0);			
			if (choice < spawnBreederProbability)
			{
				agents[i][j] = new Breeder();
			}
			else if (choice < (spawnBreederProbability + spawnCupidProbability))
			{
				agents[i][j] = new Cupid();
			}
			else if (choice < (spawnBreederProbability + spawnCupidProbability + spawnReaperProbability))
			{
				agents[i][j] = new Reaper();
			}
			else
			{
				agents[i][j] = new CandidateSolution();
			}

			agents[i][j]->RandomizeGenome();
		}
	}
	for (int i = -AGENT_ACTION_RADIUS; i <= AGENT_ACTION_RADIUS; ++i)
	{
		for (int j = -AGENT_ACTION_RADIUS; j <= AGENT_ACTION_RADIUS; ++j)
		{
			if (Distance(0, 0, i, j) <= AGENT_ACTION_RADIUS && (i != 0 || j != 0))
			{
				m_xoffset.push_back(i);
				m_yoffset.push_back(j);
			}
		}
	}
	/*for (int i = 0; i < GRID_SIZE; ++i)
	{
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			std::cout << "agent " << i << "," << j << ": " << agents[i][j] << std::endl;
			sleep(0.1);
		}
	}*/
}	



Grid::~Grid(void)
{
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			if (agents[i][j] != 0)
			{
				delete agents[i][j];
			}
		}
	}
}

void Grid::DoMovement()
{
	
	int edgesToSwap = (int)(GRID_SIZE * GRID_SIZE * edgeSwapProbability);

	int positionFirstX, positionFirstY, type, positionSecondX, positionSecondY;
	for (int i = 0; i < edgesToSwap; ++i)
	{

		positionFirstX = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		positionFirstY = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		type = (int)((double)rand() / (RAND_MAX + 1.0) * 2.0);
			
		if (type == 0)
		{
			//Vertical swap
			positionSecondX = positionFirstX;
			positionSecondY = (GRID_SIZE + positionFirstY - 1) % GRID_SIZE;	
		}
		else
		{
			//Horizontal swap
			positionSecondX = (GRID_SIZE + positionFirstX - 1) % GRID_SIZE;
			positionSecondY = positionFirstY;				
		}
			
		Agent* helper = agents[positionFirstX][positionFirstY];
		agents[positionFirstX][positionFirstY] = agents[positionSecondX][positionSecondY];
		agents[positionSecondX][positionSecondY] = helper;

	}

}

double Grid::Distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int Grid::GetNeighbourhoodSize()
{
	return m_xoffset.size();
}

void Grid::GetNeighbourhood(int x, int y, vector<Agent**> & neighbourhood)
{
	neighbourhood.clear();

	for (size_t i = 0; i < m_xoffset.size(); ++i)
	{
		neighbourhood.push_back(&agents[(GRID_SIZE + x + m_xoffset[i]) % GRID_SIZE][(GRID_SIZE + y + m_yoffset[i]) % GRID_SIZE]);
	}
}

Agent** Grid::GetAgent(int x, int y)
{
	return &agents[x][y];
}

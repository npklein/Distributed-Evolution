#include "Grid.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>


Grid::Grid(void)
{
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			for (int z = 0; z < GRID_SIZE; ++z)
			{
				for (int e = 0; e < GRID_SIZE; ++e)
				{
					double choice = (double)rand() / (RAND_MAX + 1.0);

					if (choice < spawnBreederProbability)
					{
						agents[i][j][z][e] = new Breeder();
					}
					else if (choice < (spawnBreederProbability + spawnCupidProbability))
					{
						agents[i][j][z][e] = new Cupid();
					}
					else if (choice < (spawnBreederProbability + spawnCupidProbability + spawnReaperProbability))
					{
						agents[i][j][z][e] = new Reaper();
					}
					else
					{
						agents[i][j][z][e] = new CandidateSolution();
					}
					agents[i][j][z][e]->RandomizeGenome();
				}
			}
		}
	}
	
	for (int i = -AGENT_ACTION_RADIUS; i <= AGENT_ACTION_RADIUS; ++i)
	{
		for (int j = -AGENT_ACTION_RADIUS; j <= AGENT_ACTION_RADIUS; ++j)
		{
			for (int z = -AGENT_ACTION_RADIUS; z <= AGENT_ACTION_RADIUS; ++z)
			{
				for (int e = -AGENT_ACTION_RADIUS; e <= AGENT_ACTION_RADIUS; ++e)
				{
					if (Distance(0, 0, 0, 0, i, j, z, e) <= AGENT_ACTION_RADIUS && (i != 0 || j != 0 || z != 0 || e != 0))
					{
						m_xoffset.push_back(i);
						m_yoffset.push_back(j);
						m_zoffset.push_back(z);
						m_eoffset.push_back(e);
					}
				}
			}
		}
	}
}


Grid::~Grid(void)
{
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			for (int z = 0; z < GRID_SIZE; ++z)
			{
			for (int e = 0; e < GRID_SIZE; ++e)
				{
					if (agents[i][j][z][e] != NULL)
					{
						delete agents[i][j][z];
					}
				}
			}
		}
	}
}

void Grid::DoMovement()
{
	int edgesToSwap = (int)(GRID_SIZE * GRID_SIZE * GRID_SIZE * edgeSwapProbability);

	int positionFirstX, positionFirstY, positionFirstZ, type, positionSecondX, positionSecondY, positionSecondZ;
	for (int i = 0; i < edgesToSwap; ++i)
	{
		positionFirstX = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		positionFirstY = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		positionFirstZ = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		type = (int)((double)rand() / (RAND_MAX + 1.0) * 3.0);
		if (type == 0)
		{
			//Vertical swap
			positionSecondX = positionFirstX;
			positionSecondY = (GRID_SIZE + positionFirstY - 1) % GRID_SIZE;	
			positionSecondZ = positionFirstZ;
		}
		else if (type == 1)
		{
			//Horizontal swap
			positionSecondX = (GRID_SIZE + positionFirstX - 1) % GRID_SIZE;
			positionSecondY = positionFirstY;			
			positionSecondZ = positionFirstZ;
		}
		else 
		{
			//Z plane swap
			positionSecondX = positionFirstX;
			positionSecondY = positionFirstY;
			positionSecondZ = (GRID_SIZE + positionFirstZ - 1) % GRID_SIZE;
		}
	
		Agent* helper = agents[positionFirstX][positionFirstY][positionFirstZ];
		agents[positionFirstX][positionFirstY][positionFirstZ] = agents[positionSecondX][positionSecondY][positionSecondZ];
		agents[positionSecondX][positionSecondY][positionSecondZ] = helper;
	}
}

double Grid::Distance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

int Grid::GetNeighbourhoodSize()
{
	return m_xoffset.size();
}

void Grid::GetNeighbourhood(int x, int y, int z, vector<Agent**> & neighbourhood)
{
	neighbourhood.clear();

	for (size_t i = 0; i < m_xoffset.size(); ++i)
	{
		neighbourhood.push_back(&agents[(GRID_SIZE + x + m_xoffset[i]) % GRID_SIZE][(GRID_SIZE + y + m_yoffset[i]) % GRID_SIZE][(GRID_SIZE + z + m_zoffset[i]) % GRID_SIZE]);
	}
}

Agent** Grid::GetAgent(int x, int y, int z)
{
	return &agents[x][y][z];
}

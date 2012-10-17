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
				for (int u = 0; e < GRID_SIZE; ++u)
				{
					double choice = (double)rand() / (RAND_MAX + 1.0);

					if (choice < spawnBreederProbability)
					{
						agents[i][j][z][u] = new Breeder();
					}
					else if (choice < (spawnBreederProbability + spawnCupidProbability))
					{
						agents[i][j][z][u] = new Cupid();
					}
					else if (choice < (spawnBreederProbability + spawnCupidProbability + spawnReaperProbability))
					{
						agents[i][j][z][u] = new Reaper();
					}
					else
					{
						agents[i][j][z][u] = new CandidateSolution();
					}
					agents[i][j][z][u]->RandomizeGenome();
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
				for (int u = -AGENT_ACTION_RADIUS; u <= AGENT_ACTION_RADIUS; ++u)
				{
					if (Distance(0, 0, 0, 0, i, j, z, u) <= AGENT_ACTION_RADIUS && (i != 0 || j != 0 || z != 0 || u != 0))
					{
						m_xoffset.push_back(i);
						m_yoffset.push_back(j);
						m_zoffset.push_back(z);
						m_eoffset.push_back(u);
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
			for (int e = 0; u < GRID_SIZE; ++u)
				{
					if (agents[i][j][z][u] != NULL)
					{
						delete agents[i][j][z][u];
					}
				}
			}
		}
	}
}

/*
Movement is only in one dimension at a time, so no diagonals.
*/
void Grid::DoMovement()
{
	int edgesToSwap = (int)(GRID_SIZE * GRID_SIZE * GRID_SIZE * GRID_SIZE * edgeSwapProbability);

	int positionFirstX, positionFirstY, positionFirstZ, positionFirstU, type, positionSecondX, positionSecondY, positionSecondZ, positionSecondU;
	for (int i = 0; i < edgesToSwap; ++i)
	{
		positionFirstX = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		positionFirstY = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		positionFirstZ = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		positionFirstU = (int)((double)rand() / (RAND_MAX + 1.0) * (double) GRID_SIZE);
		type = (int)((double)rand() / (RAND_MAX + 1.0) * 4.0);
		if (type == 0)
		{
			//Vertical swap
			positionSecondX = positionFirstX;
			positionSecondY = (GRID_SIZE + positionFirstY - 1) % GRID_SIZE;	
			positionSecondZ = positionFirstZ;
			positionSecondU = positionFirstU;
		}
		else if (type == 1)
		{
			//Horizontal swap
			positionSecondX = (GRID_SIZE + positionFirstX - 1) % GRID_SIZE;
			positionSecondY = positionFirstY;			
			positionSecondZ = positionFirstZ;
			positionSecondU = positionFirstU;
		}
		else if (type == 2)
		{
			//Z plane swap
			positionSecondX = positionFirstX;
			positionSecondY = positionFirstY;
			positionSecondZ = (GRID_SIZE + positionFirstZ - 1) % GRID_SIZE;
			positionSecondU = positionFirstU;
		}
		else 
		{
			//E plane swap
			positionSecondX = positionFirstX;
			positionSecondY = positionFirstY;
			positionSecondZ = positionFirstZ;
			positionSecondU = (GRID_SIZE + positionFirstU - 1) % GRID_SIZE;
		}
	
		Agent* helper = agents[positionFirstX][positionFirstY][positionFirstZ][positionFirstU];
		agents[positionFirstX][positionFirstY][positionFirstZ] = agents[positionSecondX][positionSecondY][positionSecondZ][positionFirstU];
		agents[positionSecondX][positionSecondY][positionSecondZ][positionFirstU] = helper;
	}
}

// don't know if distance is correct for higher dimensions. Got to check!
double Grid::Distance(double x1, double y1, double z1, double u1, double x2, double y2, double z2, double u2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) + (u1 - u2) * (u1 -u2));
}

int Grid::GetNeighbourhoodSize()
{
	return m_xoffset.size();
}

void Grid::GetNeighbourhood(int x, int y, int z, int u, vector<Agent**> & neighbourhood)
{
	neighbourhood.clear();

	for (size_t i = 0; i < m_xoffset.size(); ++i)
	{
		neighbourhood.push_back(&agents[(GRID_SIZE + x + m_xoffset[i]) % GRID_SIZE][(GRID_SIZE + y + m_yoffset[i]) % GRID_SIZE][(GRID_SIZE + z + m_zoffset[i]) % GRID_SIZE][(GRID_SIZE + u + m_zoffset[i]) % GRID_SIZE]);
	}
}

Agent** Grid::GetAgent(int x, int y, int z, int u)
{
	return &agents[x][y][z][u];
}

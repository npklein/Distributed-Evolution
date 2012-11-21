#include "Island.h"
#include <cstdlib>
#include <cmath>
#include <iostream>


using namespace std;

Island::Island(void)
{
	for (int i = 0; i < ISLAND_SIZE; ++i)
	{
		for (int j = 0; j < ISLAND_SIZE; ++j)
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
}


Island::~Island(void)
{
	for (int i = 0; i < ISLAND_SIZE; ++i)
	{
		for (int j = 0; j < ISLAND_SIZE; ++j)
		{
			if (agents[i][j] != NULL)
			{
				delete agents[i][j];
			}
		}
	}
}

void Island::DoMovement()
{
	int edgesToSwap = (int)(ISLAND_SIZE * ISLAND_SIZE * edgeSwapProbability);

	int positionFirstX, positionFirstY, type, positionSecondX, positionSecondY;
	for (int i = 0; i < edgesToSwap; ++i)
	{
		positionFirstX = (int)((double)rand() / (RAND_MAX + 1.0) * (double) ISLAND_SIZE);
		positionFirstY = (int)((double)rand() / (RAND_MAX + 1.0) * (double) ISLAND_SIZE);
		type = (int)((double)rand() / (RAND_MAX + 1.0) * 2.0);
			
		if (type == 0)
		{
			//Vertical swap
			positionSecondX = positionFirstX;
			positionSecondY = (ISLAND_SIZE + positionFirstY - 1) % ISLAND_SIZE;
		}
		else
		{
			//Horizontal swap
			positionSecondX = (ISLAND_SIZE + positionFirstX - 1) % ISLAND_SIZE;
			positionSecondY = positionFirstY;				
		}
			
		Agent* helper = agents[positionFirstX][positionFirstY];
		agents[positionFirstX][positionFirstY] = agents[positionSecondX][positionSecondY];
		agents[positionSecondX][positionSecondY] = helper;
	}
}

double Island::Distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int Island::GetNeighbourhoodSize()
{
	return m_xoffset.size();
}

void Island::GetNeighbourhood(int x, int y, vector<Agent**> & neighbourhood)
{
	neighbourhood.clear();

	for (size_t i = 0; i < m_xoffset.size(); ++i)
	{
		neighbourhood.push_back(&agents[(ISLAND_SIZE + x + m_xoffset[i]) % ISLAND_SIZE][(ISLAND_SIZE + y + m_yoffset[i]) % ISLAND_SIZE]);
	}
}

Agent** Island::GetAgent(int x, int y)
{
	return &agents[x][y];
}

void Island::SetAgent(Agent *a, int x, int y)
{
	agents[x][y] = a;
}

void Island::Exchange()
{
	for(int i = 0; i < EXCHANGE_AGENTS; i++){
		int sourceX = (int)((double)rand() / (RAND_MAX + 1.0) * (double) ISLAND_SIZE);
		int sourceY = (int)((double)rand() / (RAND_MAX + 1.0) * (double) ISLAND_SIZE);
		int destX = (int)((double)rand() / (RAND_MAX + 1.0) * (double) ISLAND_SIZE);
		int destY = (int)((double)rand() / (RAND_MAX + 1.0) * (double) ISLAND_SIZE);

//			cout << "SIZES: " << ISLAND_SIZE << "  " << sourceX << "  " << sourceY << " " << destX << " " << destY << std::endl;
			Agent *tmp1 = agents[sourceX][sourceY];
			Agent *tmp2 =  *this->next->GetAgent(destX, destY);

			if(tmp1 != NULL && tmp2 != NULL){
				this->next->SetAgent(tmp1, destX, destY);
				agents[sourceX][sourceY] = tmp2;
			}
		}

}

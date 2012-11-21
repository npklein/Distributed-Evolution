#ifndef ISLAND_H
#define ISLAND_H


#include "Agent.h"
#include "CandidateSolution.h"
#include "Breeder.h"
#include "Reaper.h"
#include "Cupid.h"
#include <vector>

using namespace std;

const int ISLAND_SIZE = 45;
const int EXCHANGE_AGENTS = 10;					//number of agents that are going to be exchanged
const int AGENT_RANGE = 5;
const int AGENT_ACTION_RADIUS = 5;
const double edgeSwapProbability = 0.5;
const double spawnBreederProbability = 0.0625;
const double spawnCupidProbability = 0.0625;
const double spawnReaperProbability = 0.0625;

class Island
{	
public:

	Island(void);
	~Island(void);

	void DoMovement();
	void GetNeighbourhood(int x, int y, vector<Agent**> & neighbourhood);
	int GetNeighbourhoodSize();
	Agent** GetAgent(int x, int y);
	void SetAgent(Agent * a, int x, int y);
	void Exchange();

	Island *next;			//the next Island in the ring


private:
	vector<int> m_xoffset;
	vector<int> m_yoffset;

	Agent* agents[ISLAND_SIZE][ISLAND_SIZE];

	double Distance(double x1, double y1, double x2, double y2);
};

#endif // !ISLAND_H


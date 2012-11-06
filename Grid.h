#ifndef GRID_H
#define GRID_H


#include "Agent.h"
#include "CandidateSolution.h"
#include "Breeder.h"
#include "Reaper.h"
#include "Cupid.h"
#include <vector>

using namespace std;

const int GRID_SIZE = 12;
const int AGENT_RANGE = 5;
const int AGENT_ACTION_RADIUS = 5;
const double edgeSwapProbability = 0.5;
const double spawnBreederProbability = 0.0625;
const double spawnCupidProbability = 0.0625;
const double spawnReaperProbability = 0.0625;

class Grid
{	
public:
	
	Grid(double lowerBound, double upperBound);
	~Grid(void);

	void DoMovement();
	void GetNeighbourhood(int x, int y, int z, int u, vector<Agent**> & neighbourhood);
	int GetNeighbourhoodSize();
	Agent** GetAgent(int x, int y, int z, int u);

private:
	Agent* agents[GRID_SIZE][GRID_SIZE][GRID_SIZE][GRID_SIZE];
	vector<int> m_xoffset;
	vector<int> m_yoffset;
	vector<int> m_zoffset;
	vector<int> m_uoffset;
	double Distance(double x1, double y1, double z1, double u1, double x2, double y2, double z2, double u2);
};

#endif // !GRID_H


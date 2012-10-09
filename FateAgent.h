
#ifndef FATE_AGENT_H
#define FATE_AGENT_H


#include "Agent.h"
#include "Breeder.h"
#include "CandidateSolution.h"
#include <vector>

class Cupid;
class Reaper;

using namespace std;

const int MAX_TOURNAMENT_SIZE = 50;

const double MAX_INITIAL_SELECTION_PROBABILITIES_CUPID = 0.6;
const double MAX_INITIAL_SELECTION_PROBABILITIES_REAPER = 0.6;


class FateAgent : public Agent
{
public:
	FateAgent(void);
	FateAgent(double* genome, int* intGenome);
	~FateAgent(void);

	virtual void ProcessNeighbourhood(vector<Agent**> const& neighbours);
	virtual void RandomizeGenome() = 0;
	double* GetDoubleGenome();
	int* GetIntGenome();
	void GetGenome(double outputArray[5]);
	int GetSelectedCount();
	

protected:
	vector<Breeder**> m_breeders;
	vector<Reaper**> m_reapers;
	vector<Cupid**> m_cupids;
	vector<CandidateSolution**> m_candidateSolutions;

	vector<Breeder**> m_selectedBreeders;
	vector<Reaper**> m_selectedReapers;
	vector<Cupid**> m_selectedCupids;
	vector<CandidateSolution**> m_selectedCandidateSolutions;

	double m_selectionProbCandidateSolutions;
	double m_selectionProbCupids;
	double m_selectionProbBreeders;
	double m_selectionProbReapers;

	int m_tournamentSize;

	void SetGenome(double* genome, int * intGenome);
};

#endif // !FATE_AGENT_H



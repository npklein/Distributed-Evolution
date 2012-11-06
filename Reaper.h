
#ifndef REAPER_H
#define REAPER_H


#include "FateAgent.h"

class Reaper : public FateAgent
{
public:
	Reaper(void);
	Reaper(double* genome, int* intGenome);
	~Reaper(void);

	void Select();
	virtual void RandomizeGenome(double lowerBound, double upperBound);
	Agent** GetAgentToKill();

private:
	template <class T> void SelectToList(vector<T**> & selectFrom, vector<T**> & selectTo, bool(*compare)(T**, T**), double probability);
	static bool compareCS(CandidateSolution** left, CandidateSolution** right);
	static bool compareCupids(Cupid** left, Cupid** right);
	static bool compareBreeders(Breeder** left, Breeder** right);
	static bool compareReapers(Reaper** left, Reaper** right);
};

#endif // !REAPER_H


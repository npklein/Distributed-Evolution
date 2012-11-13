#ifndef CUPID_H
#define CUPID_H

#include "FateAgent.h"

class Cupid : public FateAgent
{
public:
	Cupid(void);
	Cupid(double* genome, int* intGenome);
	~Cupid(void);
	
	virtual void ProcessBag(int bagSize);
	virtual void SetFitness();
	virtual void RandomizeGenome(double lowerBound, double upperBound);
	void Select();
	Agent** GetParents();
	Agent** GetEmptyCell();
	Breeder* GetRandomBreeder();

private:
	vector<Agent**> m_emptySpaces;
	
	template <class T> void SelectToList(vector<T**> & selectFrom, vector<T**> & selectTo, bool(*compare)(T**, T**), double probability);
	static bool compareCS(CandidateSolution** left, CandidateSolution** right);
	static bool compareCupids(Cupid** left, Cupid** right);
	static bool compareBreeders(Breeder** left, Breeder** right);
	static bool compareReapers(Reaper** left, Reaper** right);
};

#endif // !CUPID_H


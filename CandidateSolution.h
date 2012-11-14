

#ifndef CANDIDATE_SOLUTION_H
#define CANDIDATE_SOLUTION_H


#include "Agent.h"

class CandidateSolution : public Agent
{
public:
	CandidateSolution(void);
	CandidateSolution(double * genome);
	~CandidateSolution(void);
	virtual void RandomizeGenome(double lowerBound, double upperBound);
//	double * GetGenome();

private:
//	double * m_genome;
};

#endif // !CANDIDATE_SOLUTION_H


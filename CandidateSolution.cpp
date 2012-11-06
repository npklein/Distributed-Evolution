#include "CandidateSolution.h"
//#include <random>
#include "fitness.h"


CandidateSolution::CandidateSolution(void)
{
	m_genome = new double[PROBLEM_DIMENSION];
	m_type = candidateSolution;
}

CandidateSolution::CandidateSolution(double * genome)
{
	m_genome = new double[PROBLEM_DIMENSION];
	m_type = candidateSolution;

	for (int i = 0; i < PROBLEM_DIMENSION; ++i)
	{
		m_genome[i] = genome[i];
	}

	m_fitness = fitnessFunction(m_genome);
}

CandidateSolution::~CandidateSolution(void)
{
	delete [] m_genome;
}

void CandidateSolution::RandomizeGenome(double lowerBound, double upperBound)
{
	for (int i = 0;  i < PROBLEM_DIMENSION; ++i)
	{
		m_genome[i] = randomGene(lowerBound, upperBound);
	}
	m_fitness = fitnessFunction(m_genome);
}

double * CandidateSolution::GetGenome()
{
	return m_genome;
}

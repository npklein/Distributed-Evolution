#include "CandidateSolution.h"
#include "fitness.h"


CandidateSolution::CandidateSolution(void)
{
//	m_coordinates = new double[PROBLEM_DIMENSION];
	m_type = candidateSolution;
}

CandidateSolution::CandidateSolution(double * genome)
{
//	m_coordinates = new double[PROBLEM_DIMENSION];
	m_type = candidateSolution;

	for (int i = 0; i < PROBLEM_DIMENSION; ++i)
	{
		m_coordinates[i] = genome[i];
	}

	m_fitness = fitnessFunction(m_coordinates);
}

CandidateSolution::~CandidateSolution(void)
{
//	delete [] m_coordinates;
}

void CandidateSolution::RandomizeGenome(double lowerBound, double upperBound)
{
	for (int i = 0;  i < PROBLEM_DIMENSION; ++i)
	{
		m_coordinates[i] = randomGene(lowerBound, upperBound);
	}
	m_fitness = fitnessFunction(m_coordinates);
}

//double * CandidateSolution::GetGenome()
//{
//	return m_genome;
//}

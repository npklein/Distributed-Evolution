#include "Agent.h"
#include "fitness.h"
#include <iostream>


Agent::Agent(void)
{
	m_age = 0;
	m_fitness = -1;
	m_coordinates = new double[PROBLEM_DIMENSION];
}


Agent::~Agent(void)
{
	delete [] m_coordinates;
}

int Agent::GetAge()
{
	return m_age;
}

double Agent::GetFitness()
{
	return m_fitness;
}

void Agent::IncreaseAge()
{
	m_age++;
}

AgentType Agent::GetType()
{
	return m_type;
}

double *Agent::GetCoordinates()
{
		return m_coordinates;
}


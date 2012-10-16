#include "Agent.h"


Agent::Agent(void)
{
	m_age = 0;
	m_fitness = -1;
}


Agent::~Agent(void)
{
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
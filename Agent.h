#ifndef AGENT_H
#define AGENT_H


enum AgentType { candidateSolution, cupid, reaper, breeder};

class Agent
{
public:
	Agent(void);
	virtual ~Agent(void);
	
	double GetFitness();
	int GetAge();
	void IncreaseAge();
	AgentType GetType();
	virtual void RandomizeGenome() = 0;

protected:
	double m_fitness;
	AgentType m_type;
private:
	int m_age;
};

#endif // !AGENT_H


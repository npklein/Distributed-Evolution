#include "FateAgent.h"


FateAgent::FateAgent(void)
{
}

FateAgent::FateAgent( double* genome, int* intGenome )
{
	SetGenome(genome, intGenome);
}

FateAgent::~FateAgent(void)
{
}

void FateAgent::ProcessNeighbourhood(vector<Agent**> const& neighbours)
{
	m_candidateSolutions.clear();
	m_cupids.clear();
	m_breeders.clear();
	m_reapers.clear();


	for (size_t i = 0; i < neighbours.size(); ++i)
	{
		if ((*(neighbours.at(i))) != NULL)
		{
			switch ((*(neighbours.at(i)))->GetType())
			{
				case candidateSolution:
				{
					m_candidateSolutions.push_back((CandidateSolution**)neighbours.at(i));
					break;
				}
				case cupid:
				{
					m_cupids.push_back((Cupid**)neighbours.at(i));
					break;
				}
				case breeder:
				{
					m_breeders.push_back((Breeder**)neighbours.at(i));
					break;
				}
				case reaper:
				{
					m_reapers.push_back((Reaper**)neighbours.at(i));
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
}

void FateAgent::SetFitness(double averageFitness)
{
	m_fitness = averageFitness;
}


double* FateAgent::GetDoubleGenome()
{
	double* genome = new double[4];

	genome[0] = m_selectionProbBreeders;
	genome[1] = m_selectionProbCandidateSolutions;
	genome[2] = m_selectionProbCupids;
	genome[3] = m_selectionProbReapers;

	return genome;
}

void FateAgent::SetGenome( double* genome, int * intGenome )
{
	m_selectionProbBreeders = genome[0];
	m_selectionProbCandidateSolutions = genome[1];
	m_selectionProbCupids = genome[2];
	m_selectionProbReapers = genome[3];
	m_tournamentSize = intGenome[0];
}

int* FateAgent::GetIntGenome()
{
	int* genome = new int[1];

	genome[0] = m_tournamentSize;

	return genome;
}

void FateAgent::GetGenome( double outputArray[5] )
{
	outputArray[0] = m_selectionProbBreeders;
	outputArray[1] = m_selectionProbCandidateSolutions;
	outputArray[2] = m_selectionProbCupids;
	outputArray[3] = m_selectionProbReapers;
	outputArray[4] = m_tournamentSize;
}

int FateAgent::GetSelectedCount()
{
	return m_selectedBreeders.size() + m_selectedCandidateSolutions.size() + m_selectedCupids.size() + m_selectedReapers.size(); 
}
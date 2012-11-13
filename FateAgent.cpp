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

void FateAgent::ProcessBag(int bagSize)
{
	m_candidateSolutions.clear();
	m_cupids.clear();
	m_breeders.clear();
	m_reapers.clear();

	double max = -1.0;
/*
	for (int i = 0; i < BAG_SIZE; ++i)
	{
		if ((*(bag.at(i))) != NULL)
		{
			switch ((*(bag.at(i)))->GetType())
			{
				case candidateSolution:
				{
					m_candidateSolutions.push_back((CandidateSolution**)bag.at(i));
					if ((*(neighbours.at(i)))->GetFitness() > max)
					{
						max = (*(bag.at(i)))->GetFitness();
					}
					break;
				}
				case cupid:
				{
					m_cupids.push_back((Cupid**)bag.at(i));
					break;
				}
				case breeder:
				{
					m_breeders.push_back((Breeder**)bag.at(i));
					break;
				}
				case reaper:
				{
					m_reapers.push_back((Reaper**)bag.at(i));
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

	m_fitness = max;
 */
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
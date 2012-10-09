#include "Cupid.h"
#include "Reaper.h"
#include <algorithm>


Cupid::Cupid(void)
{
	m_type = cupid;
}

Cupid::Cupid( double* genome, int* intGenome ) : FateAgent(genome, intGenome)
{
	m_type = cupid;
}

Cupid::~Cupid(void)
{
}

void Cupid::ProcessNeighbourhood(vector<Agent**> const& neighbours)
{
	m_candidateSolutions.clear();
	m_cupids.clear();
	m_breeders.clear();
	m_reapers.clear();
	m_emptySpaces.clear();

	double max = -1.0;

	for (size_t i = 0; i < neighbours.size(); ++i)
	{
		if ((*(neighbours.at(i))) != NULL)
		{
			switch ((*(neighbours.at(i)))->GetType())
			{
				case candidateSolution:
				{
					m_candidateSolutions.push_back((CandidateSolution**)neighbours.at(i));
					if ((*(neighbours.at(i)))->GetFitness() > max)
					{
						max = (*(neighbours.at(i)))->GetFitness();
					}
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
		else
		{
			m_emptySpaces.push_back(neighbours.at(i));
		}
	}

	m_fitness = max;
}

template <class T> void Cupid::SelectToList(vector<T**> & selectFrom, vector<T**> & selectTo, bool(*compare)(T**, T**), double probability)
{
	selectTo.clear();
	int realTournamentSize = m_tournamentSize < selectFrom.size() ? m_tournamentSize : selectFrom.size();
	vector<T**> selectedForTournament;
	// selectedForTournament.reserve(realTournamentSize);

	for (int i = 0; i < realTournamentSize; ++i)
	{
		selectedForTournament.push_back(NULL);
	}
		
	for (int i = 0; i < (selectFrom.size() * 2); ++i)
	{
		if(((double)rand() / (RAND_MAX + 1.0)) < probability) // TODO should be probability!
		{
			for (int j = 0; j < realTournamentSize; ++j)
			{
				selectedForTournament.at(j) = selectFrom.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)realTournamentSize));
			}
				
			sort(selectedForTournament.begin(), selectedForTournament.end(), compare);
				
			selectTo.push_back(selectedForTournament.at(realTournamentSize - 1));
		}
	}
		

	if (selectTo.size() % 2 == 1)
	{
		selectTo.push_back(selectTo.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)(selectTo.size()))));
	}
}

bool Cupid::compareCS(CandidateSolution** left, CandidateSolution** right)
{
	return (*left)->GetFitness() < (*right)->GetFitness();
}

bool Cupid::compareCupids(Cupid** left, Cupid** right)
{
	return (*left)->GetFitness() < (*right)->GetFitness();
}

bool Cupid::compareBreeders(Breeder** left, Breeder** right)
{
	return (*left)->GetFitness() < (*right)->GetFitness();
}

bool Cupid::compareReapers(Reaper** left, Reaper** right)
{
	return (*left)->GetFitness() < (*right)->GetFitness();
}

void Cupid::Select()
{
	SelectToList<CandidateSolution>(m_candidateSolutions, m_selectedCandidateSolutions, &Cupid::compareCS, m_selectionProbCandidateSolutions);
	SelectToList<Cupid>(m_cupids, m_selectedCupids, &Cupid::compareCupids, m_selectionProbCupids);
	SelectToList<Breeder>(m_breeders, m_selectedBreeders, &Cupid::compareBreeders, m_selectionProbBreeders);
	SelectToList<Reaper>(m_reapers, m_selectedReapers, &Cupid::compareReapers, m_selectionProbReapers);
}

void Cupid::RandomizeGenome()
{
	m_selectionProbCandidateSolutions = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_SELECTION_PROBABILITIES_CUPID;
	m_selectionProbCupids = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_SELECTION_PROBABILITIES_CUPID;
	m_selectionProbBreeders = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_SELECTION_PROBABILITIES_CUPID;
	m_selectionProbReapers = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_SELECTION_PROBABILITIES_CUPID;
	m_tournamentSize = (int)((double)rand() / (RAND_MAX + 1.0) * (double)MAX_TOURNAMENT_SIZE) + 1;
}

Agent** Cupid::GetParents()
{
	vector<AgentType> typesToChooseFrom;

	if (!m_selectedCandidateSolutions.empty()) {typesToChooseFrom.push_back(candidateSolution);}
	if (!m_selectedCupids.empty()) {typesToChooseFrom.push_back(cupid);}
	if (!m_selectedBreeders.empty()) {typesToChooseFrom.push_back(breeder);}
	if (!m_selectedReapers.empty()) {typesToChooseFrom.push_back(reaper);}

	if (typesToChooseFrom.empty())
	{
		return NULL;
	}

	AgentType chosenType = typesToChooseFrom.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)typesToChooseFrom.size()));

	Agent** parents = new Agent*[2];

	switch(chosenType)
	{
	case candidateSolution:
		{
			parents[0] = *m_selectedCandidateSolutions.at(m_selectedCandidateSolutions.size() - 1);
			m_selectedCandidateSolutions.pop_back();
			parents[1] = *m_selectedCandidateSolutions.at(m_selectedCandidateSolutions.size() - 1);
			m_selectedCandidateSolutions.pop_back();
			break;
		}
	case cupid:
		{
			parents[0] = *m_selectedCupids.at(m_selectedCupids.size() - 1);
			m_selectedCupids.pop_back();
			parents[1] = *m_selectedCupids.at(m_selectedCupids.size() - 1);
			m_selectedCupids.pop_back();
			break;
		}
	case reaper:
		{
			parents[0] = *m_selectedReapers.at(m_selectedReapers.size() - 1);
			m_selectedReapers.pop_back();
			parents[1] = *m_selectedReapers.at(m_selectedReapers.size() - 1);
			m_selectedReapers.pop_back();
			break;
		}
	case breeder:
		{
			parents[0] = *m_selectedBreeders.at(m_selectedBreeders.size() - 1);
			m_selectedBreeders.pop_back();
			parents[1] = *m_selectedBreeders.at(m_selectedBreeders.size() - 1);
			m_selectedBreeders.pop_back();
			break;
		}
	default:
		{
			break;
		}
	}

	return parents;
}

Agent** Cupid::GetEmptyCell()
{
	for (int i = 0; i < m_emptySpaces.size(); ++i)
	{
//		vector<Agent**>::const_iterator it = m_emptySpaces.begin() + i;
        vector<Agent**>::iterator it = m_emptySpaces.begin() + i;

		if (**it != NULL)
		{
			m_emptySpaces.erase(it);
			--i;
		}
	}

	if (m_emptySpaces.empty())
	{
		return NULL;
	}
	
	int position = (int)((double)rand() / (RAND_MAX + 1.0) * (double)m_emptySpaces.size());
	Agent** emptyAgent = m_emptySpaces.at(position);
	m_emptySpaces.erase(m_emptySpaces.begin() + position);

	return emptyAgent;
}

Breeder* Cupid::GetRandomBreeder()
{
	if (m_breeders.empty())
	{
		return NULL;
	}
	return *m_breeders.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)m_breeders.size()));
}

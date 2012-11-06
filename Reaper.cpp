#include "Reaper.h"
#include "Cupid.h"
#include <algorithm>


Reaper::Reaper(void)
{
	m_type = reaper;
}

Reaper::Reaper( double* genome, int* intGenome ) : FateAgent(genome, intGenome)
{
	m_type = reaper;
}

Reaper::~Reaper(void)
{
}

void Reaper::SetFitness()
{
	// need to think of a way to determine fitness
	m_fitness = 1;
}

template <class T> void Reaper::SelectToList(vector<T**> & selectFrom, vector<T**> & selectTo, bool(*compare)(T**, T**), double probability)
{
	selectTo.clear();
	int realTournamentSize = m_tournamentSize < (int)selectFrom.size() ? m_tournamentSize : selectFrom.size();
	vector<T**> selectedForTournament;
	selectedForTournament.reserve(realTournamentSize);

	for (int i = 0; i < realTournamentSize; ++i)
	{
		selectedForTournament.push_back(NULL);
	}
		
	int count = selectFrom.size();
	for (int i = 0; i < count; ++i)
	{
		if(((double)rand() / (RAND_MAX + 1.0)) < probability) // TODO should be probability!
		{
			for (int j = 0; j < realTournamentSize; ++j)
			{
				selectedForTournament.at(j) = selectFrom.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)selectFrom.size()));
			}
				
			sort(selectedForTournament.begin(), selectedForTournament.end(), compare);
				
			selectTo.push_back(selectedForTournament.at(0));

			for (typename vector<T**>::iterator it = selectFrom.begin(); it != selectFrom.end(); ++it)
			{
				if ((*it) == selectedForTournament.at(0))
				{
					selectFrom.erase(it);
					break;
				}
			}
		}
	}
}

void Reaper::Select()
{
	SelectToList<CandidateSolution>(m_candidateSolutions, m_selectedCandidateSolutions, &Reaper::compareCS, m_selectionProbCandidateSolutions);
	SelectToList<Cupid>(m_cupids, m_selectedCupids, &Reaper::compareCupids, m_selectionProbCupids);
	SelectToList<Breeder>(m_breeders, m_selectedBreeders, &Reaper::compareBreeders, m_selectionProbBreeders);
	SelectToList<Reaper>(m_reapers, m_selectedReapers, &Reaper::compareReapers, m_selectionProbReapers);
}

bool Reaper::compareCS(CandidateSolution** left, CandidateSolution** right)
{
	return (*left)->GetFitness() < (*right)->GetFitness();
}

bool Reaper::compareCupids(Cupid** left, Cupid** right)
{
	return (*left)->GetFitness() < (*right)->GetFitness();
}

bool Reaper::compareBreeders(Breeder** left, Breeder** right)
{
	return (*left)->GetFitness() < (*right)->GetFitness();
}

bool Reaper::compareReapers(Reaper** left, Reaper** right)
{
	return (*left)->GetFitness() < (*right)->GetFitness();
}

void Reaper::RandomizeGenome(double lowerBound, double upperBound)
{
	m_selectionProbCandidateSolutions = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_SELECTION_PROBABILITIES_REAPER;
	m_selectionProbCupids = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_SELECTION_PROBABILITIES_REAPER;
	m_selectionProbBreeders = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_SELECTION_PROBABILITIES_REAPER;
	m_selectionProbReapers = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_SELECTION_PROBABILITIES_REAPER;
	m_tournamentSize = (int)((double)rand() / (RAND_MAX + 1.0) * (double)MAX_TOURNAMENT_SIZE) + 1;
}

Agent** Reaper::GetAgentToKill()
{
	Agent** toKill = NULL;

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

	switch(chosenType)
	{
	case candidateSolution:
		{
			toKill = (Agent**)m_selectedCandidateSolutions.at(m_selectedCandidateSolutions.size() - 1);
			m_selectedCandidateSolutions.pop_back();
			break;
		}
	case cupid:
		{
			toKill = (Agent**)m_selectedCupids.at(m_selectedCupids.size() - 1);
			m_selectedCupids.pop_back();
			break;
		}
	case reaper:
		{
			toKill = (Agent**)m_selectedReapers.at(m_selectedReapers.size() - 1);
			m_selectedReapers.pop_back();
			break;
		}
	case breeder:
		{
			toKill = (Agent**)m_selectedBreeders.at(m_selectedBreeders.size() - 1);
			m_selectedBreeders.pop_back();
			break;
		}
	default:
		{
			break;
		}
	}

	return toKill;
}

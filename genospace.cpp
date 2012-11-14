#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "fitness.h"
#include "Breeder.h"
#include "Cupid.h"
#include "Reaper.h"

using namespace std;

const int GENERATION_COUNT = 100;
const int NUMBER_OF_AGENTS = 10000;
//const double AGENT_ACTION_RADIUS = 7.0;	//TODO appropriate value???

const double spawnBreederProbability = 0.0625;
const double spawnCupidProbability = 0.0625;
const double spawnReaperProbability = 0.0625;


void usage(char *name);

/*
 * Calculate the distance between two agents
 * **/
double euclideanDistance(Agent *x, Agent *y);

void getNeighbourhood(Agent **agents, int i, vector<Agent**> &neighborhood, float agent_action_radius);


void runEvolution(int functionID, float agent_action_radius){
	srand(time(NULL));

	double lowerBound;
	double upperBound;
	Agent *agents[NUMBER_OF_AGENTS];

	vector<Agent**> neighbourhood;
	vector<Cupid*> cupids;
	vector<Cupid*> cupidsQueue;
	vector<Reaper**> reapers;
	vector<Reaper**> reapersQueue;

	std::ofstream outputFile;
	outputFile.open("log.txt");

	initializeFitness(functionID);

	if(functionID == 0){
		lowerBound = -M_PI;
		upperBound = M_PI;
	}else{
		lowerBound = -5;
		upperBound = 5;
	}

	//create the initial set of agents
	for(int i = 0; i < NUMBER_OF_AGENTS; i++){

		double choice = (double) rand() / (RAND_MAX + 1.0);

		if(choice < spawnBreederProbability){
			agents[i] = new Breeder();
		}else if (choice < (spawnBreederProbability + spawnCupidProbability))
		{
			agents[i] = new Cupid();
		}
		else if (choice < (spawnBreederProbability + spawnCupidProbability + spawnReaperProbability))
		{
			agents[i] = new Reaper();
		}
		else
		{
			agents[i] = new CandidateSolution();
		}

		agents[i]->RandomizeGenome(lowerBound, upperBound);
	}

	for(int i = 0; i < GENERATION_COUNT; ++i)
	{
		std::cout << i << std::endl;
		cupids.clear();
		reapers.clear();
		neighbourhood.clear();

		for(int j = 0; j < NUMBER_OF_AGENTS; j++)
		{
			if(agents[j] != NULL)
			{
				agents[j]->IncreaseAge();

				switch(agents[j]->GetType())
				{
				case breeder:
				{
					getNeighbourhood(agents, j, neighbourhood, agent_action_radius);
					((Breeder*)agents[j])->ProcessNeighbourhood(neighbourhood);
					break;
				}
				case cupid:
				{
					getNeighbourhood(agents, j, neighbourhood, agent_action_radius);
					((Cupid*)agents[j])->ProcessNeighbourhood(neighbourhood);
					cupids.push_back((Cupid*)agents[j]);
					break;
				}
				case reaper:
				{
					getNeighbourhood(agents, j, neighbourhood, agent_action_radius);
					((Reaper*)agents[j])->ProcessNeighbourhood(neighbourhood);
					reapers.push_back((Reaper**)&agents[j]);
					break;
				}
				default:
					break;
				}
			}
		}

		int cs = 0, rs = 0;

		for (vector<Cupid*>::const_iterator it = cupids.begin(); it != cupids.end(); ++it)
		{
			(*it)->Select();
			cs += (*it)->GetSelectedCount();
		}

		for (vector<Reaper**>::const_iterator it = reapers.begin(); it != reapers.end(); ++it)
		{
			(**it)->Select();
			rs += (**it)->GetSelectedCount();
		}

		random_shuffle(cupids.begin(), cupids.end());
		cupidsQueue.clear();
		copy(cupids.begin(), cupids.end(), back_inserter(cupidsQueue));
		cupids.clear();

		int bred[4] = { 0, 0, 0, 0 };

		while (!cupidsQueue.empty())
		{
			while (!cupidsQueue.empty())
			{
				Cupid* c = *(cupidsQueue.end()-1);
				cupidsQueue.pop_back();

				Agent** parents = c->GetParents();
				if (parents == NULL)
				{
					continue;
				}

				Breeder* b = c->GetRandomBreeder();
				for(int k = 0; k < NUMBER_OF_AGENTS; ++k){
					if(agents[k] == NULL){
						if (b != NULL)
						{
							b->Breed(parents, &agents[k], lowerBound, upperBound);
							cupids.push_back(c);

							switch (agents[k]->GetType())
							{
							case candidateSolution:
							{
								++bred[0];
								break;
							}
							case cupid:
							{
								++bred[1];
								break;
							}
							case breeder:
							{
								++bred[2];
								break;
							}
							case reaper:
							{
								++bred[3];
								break;
							}
							}
							break;
						}
					}
				}

				delete [] parents;
			}

			random_shuffle(cupids.begin(), cupids.end());
			copy(cupids.begin(), cupids.end(), back_inserter(cupidsQueue));
			cupids.clear();
		}

		double fitness = 0;
		double sumFitness = 0;
		double cupidGenome[5+PROBLEM_DIMENSION] = { };
		double reaperGenome[5+PROBLEM_DIMENSION] = { };
		double breederGenome[3+PROBLEM_DIMENSION] = { };

		int e = 0, ca = 0, cu = 0, b = 0, r = 0;

		for(int j = 0; j < NUMBER_OF_AGENTS; ++j)
		{
			if(agents[j] != NULL)
			{
				switch(agents[j]->GetType())
				{
				case candidateSolution:
				{
					ca++;

					sumFitness += agents[j]->GetFitness();
					if (agents[j]->GetFitness() > fitness)
					{
						fitness = agents[j]->GetFitness();
					}

					break;
				}
				case cupid:
				{
					cu++;

					double genome[5+PROBLEM_DIMENSION];
					((Cupid*)agents[j])->GetGenome(genome);
					for (int l = 0; l < 5 + PROBLEM_DIMENSION; ++l)
					{
						cupidGenome[l] += genome[l];
					}

					break;
				}
				case breeder:
				{
					b++;

					double genome[3+PROBLEM_DIMENSION];
					((Breeder*)agents[j])->GetGenome(genome);
					for (int l = 0; l < 3 + PROBLEM_DIMENSION; ++l)
					{
						breederGenome[l] += genome[l];
					}

					break;
				}
				case reaper:
				{
					r++;

					double genome[5+PROBLEM_DIMENSION];
					((Reaper*)agents[j])->GetGenome(genome);
					for (int l = 0; l < 5 + PROBLEM_DIMENSION; ++l)
					{
						reaperGenome[l] += genome[l];
					}

					break;
				}
				default:
					break;
				}
			}
		}

		random_shuffle(reapers.begin(), reapers.end());
		reapersQueue.clear();
		copy(reapers.begin(), reapers.end(), back_inserter(reapersQueue));
		reapers.clear();

		int killed[4] = { 0, 0, 0, 0};

		while (!reapersQueue.empty())
		{
			while (!reapersQueue.empty())
			{
				Reaper** r = *(reapersQueue.end()-1);
				reapersQueue.pop_back();

				if (*r == NULL)
				{
					continue;
				}

				Agent** toKill = (*r)->GetAgentToKill();

				if (toKill != NULL && *toKill != NULL)
				{
					switch ((*toKill)->GetType())
					{
					case candidateSolution:
					{
						++killed[0];
						break;
					}
					case cupid:
					{
						++killed[1];
						break;
					}
					case breeder:
					{
						++killed[2];
						break;
					}
					case reaper:
					{
						++killed[3];
						break;
					}
					}

					delete (*toKill);
					*toKill = NULL;
					reapers.push_back(r);
				}
				else
				{
					if (toKill != NULL)
					{
						reapers.push_back(r);
					}
				}
			}

			random_shuffle(reapers.begin(), reapers.end());
			copy(reapers.begin(), reapers.end(), back_inserter(reapersQueue));
			reapers.clear();
		}

		outputFile << fitness << " " << sumFitness / ca << " " << e << " " << ca << " " << cu << " " << b << " "  << r << " ";

		outputFile << std::endl;
		outputFile.flush();
	}

	outputFile.close();
}


int main(int argc, char **argv){
	if(argc != 3)
		usage(argv[0]);

	int funcID = atoi(argv[1]);
	float agent_action_radius = atof(argv[2]);

	if((funcID == 0 && argv[1][0] != '0') || funcID < 0 || funcID > 30 || agent_action_radius == 0)
		usage(argv[0]);

	runEvolution(funcID, agent_action_radius);
}

void usage(char *name){
	std::cout << name << " n AGENT_ACTION_RADIUS" << std::endl;
	std::cout << "n = 0 is for Fletcher Powell" << std::endl;
	std::cout << "n = 1..24 the BBOB function id" << std::endl;
	std::cout << "AGENT_ACTION_RADIUS between 0.0 and sqrt(PROBLEM_DIMENSION)" << std::endl;

	exit(1);
}

double euclideanDistance(Agent *a, Agent *b){
	double sum = 0;
	double *a_coordinates = a->GetCoordinates();
	double *b_coordinates = b->GetCoordinates();

	for(int i = 0; i < PROBLEM_DIMENSION; i++){
		sum += (a_coordinates[i] - b_coordinates[i]) * (a_coordinates[i] - b_coordinates[i]);
	}

	return sqrt(sum);
}

void getNeighbourhood(Agent **agents, int x, vector<Agent**> &neighborhood, float agent_action_radius){
	neighborhood.clear();
	for(int i = 0; i < NUMBER_OF_AGENTS; i++){
		if(i == x || agents[i] == NULL)
			continue;

		if(euclideanDistance(agents[x], agents[i]) < agent_action_radius)
			neighborhood.push_back(&agents[i]);
	}
}

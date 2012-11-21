#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include "Island.h"
#include "fitness.h"

using namespace std;

const int GENERATION_COUNT = 500;
const int NUMBER_ISLANDS = 5;
const int STEP_SIZE = 5;

void runEvolution()
{
	srand(time(NULL));

	initializeFitness();


	Island islands[NUMBER_ISLANDS];
	for(int i = 0; i < NUMBER_ISLANDS; i++)
		islands[i].next = &islands[(i+1) % NUMBER_ISLANDS];


	vector<Agent**> neighbourhood;
	vector<Cupid*> cupids;
	vector<Reaper**> reapers;
	vector<Cupid*> cupidsQueue;
	vector<Reaper**> reapersQueue;

	std::ofstream outputFile;
	outputFile.open("log.txt");
	outputFile << "islandNumber \t fitness \t sumFitness / ca \t e \t ca \t cu \t b \t r \t bred[0] \t bred[1] "
			"\t bred[2] \t bred[3] \t killed[0] \t killed[1] \t killed[2] \t killed[3] \t "
			"cupidGenome[0] / cu \t cupidGenome[1] / cu \t cupidGenome[2] / cu \t "
			"cupidGenome[3] / cu \t cupidGenome[4] / cu \t reaperGenome[0] / r \t "
			"reaperGenome[1] / r \t reaperGenome[2] / r \t reaperGenome[3] / r \t "
			"reaperGenome[4] / r \t breederGenome[0] / b \t breederGenome[1] / b \t "
			"breederGenome[2] / b"
			<< std::endl;


	for(int i = 0; i < GENERATION_COUNT; ++i)
	{
		for(int isl = 0; isl < NUMBER_ISLANDS; isl++){
			std::cout << "Generation : " << i << std::endl;
			islands[isl].DoMovement();

			cupids.clear();
			reapers.clear();

			for (int j = 0; j < ISLAND_SIZE; ++j)
			{
				for (int k = 0; k < ISLAND_SIZE; ++k)
				{
					if ((*(islands[isl].GetAgent(j, k))) != NULL)
					{
						(*(islands[isl].GetAgent(j, k)))->IncreaseAge();

						switch ((*(islands[isl].GetAgent(j, k)))->GetType())
						{
						case breeder:
						{
							islands[isl].GetNeighbourhood(j, k, neighbourhood);
							((Breeder*)(*(islands[isl].GetAgent(j, k))))->ProcessNeighbourhood(neighbourhood);
							break;
						}
						case cupid:
						{
							islands[isl].GetNeighbourhood(j, k, neighbourhood);
							((Cupid*)(*(islands[isl].GetAgent(j, k))))->ProcessNeighbourhood(neighbourhood);
							cupids.push_back((Cupid*)(*(islands[isl].GetAgent(j, k))));
							break;
						}
						case reaper:
						{
							islands[isl].GetNeighbourhood(j, k, neighbourhood);
							((Reaper*)(*(islands[isl].GetAgent(j, k))))->ProcessNeighbourhood(neighbourhood);
							reapers.push_back((Reaper**)(islands[isl].GetAgent(j, k)));
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

			// std::cout << "selected " << cs << " " << rs << std::endl;

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

					Agent** empty = c->GetEmptyCell();
					Breeder* b = c->GetRandomBreeder();

					if (empty != NULL && b != NULL)
					{
						b->Breed(parents, empty);
						cupids.push_back(c);

						switch ((*empty)->GetType())
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
					}

					delete [] parents;
				}

				random_shuffle(cupids.begin(), cupids.end());
				copy(cupids.begin(), cupids.end(), back_inserter(cupidsQueue));
				cupids.clear();
			}

			double fitness = 0;
			double sumFitness = 0;
			double cupidGenome[5] = { 0.0, 0.0, 0.0, 0.0, 0.0};
			double reaperGenome[5] = { 0.0, 0.0, 0.0, 0.0, 0.0};;
			double breederGenome[3] = { 0.0, 0.0, 0.0 };

			int e = 0, ca = 0, cu = 0, b = 0, r = 0;

			for (int j = 0; j < ISLAND_SIZE; ++j)
			{
				for (int k = 0; k < ISLAND_SIZE; ++k)
				{
					if ((*(islands[isl].GetAgent(j, k))) != NULL)
					{
						switch ((*(islands[isl].GetAgent(j, k)))->GetType())
						{
						case candidateSolution:
						{
							ca++;

							sumFitness += (*(islands[isl].GetAgent(j, k)))->GetFitness();
							if ((*(islands[isl].GetAgent(j, k)))->GetFitness() > fitness)
							{
								fitness = (*(islands[isl].GetAgent(j, k)))->GetFitness();
							}

							break;
						}
						case cupid:
						{
							cu++;

							double genome[5];
							((Cupid*)(*(islands[isl].GetAgent(j, k))))->GetGenome(genome);
							for (int l = 0; l < 5; ++l)
							{
								cupidGenome[l] += genome[l];
							}

							break;
						}
						case breeder:
						{
							b++;

							double genome[3];
							((Breeder*)(*(islands[isl].GetAgent(j, k))))->GetGenome(genome);
							for (int l = 0; l < 3; ++l)
							{
								breederGenome[l] += genome[l];
							}

							break;
						}
						case reaper:
						{
							r++;

							double genome[5];
							((Reaper*)(*(islands[isl].GetAgent(j, k))))->GetGenome(genome);
							for (int l = 0; l < 5; ++l)
							{
								reaperGenome[l] += genome[l];
							}

							break;
						}
						default:
							break;
						}
					}
					else
					{
						e++;
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

			outputFile << isl << " " << fitness << " " << sumFitness / ca << " " << e << " " << ca << " " << cu << " " << b << " "  << r << " ";

			for (int l = 0; l < 4; ++l)
			{
				outputFile << bred[l] << " ";
			}

			for (int l = 0; l < 4; ++l)
			{
				outputFile << killed[l] << " ";
			}

			for (int l = 0; l < 5; ++l)
			{
				outputFile << cupidGenome[l] / cu << " ";
			}

			for (int l = 0; l < 5; ++l)
			{
				outputFile << reaperGenome[l] / r << " ";
			}

			for (int l = 0; l < 3; ++l)
			{
				if (l < 2)
				{
					outputFile << breederGenome[l] / b << " ";
				}
				else {
					outputFile << breederGenome[l] / b;
				}
			}

			outputFile << std::endl;
			outputFile.flush();

		}

		if(i % STEP_SIZE == 0){
			for(int in = 0; in < NUMBER_ISLANDS; in++){
				islands[in].Exchange();
			}
		}
	}
	outputFile.close();
}

int main (int argc, char **argv)
{
	time_t begin, end; 
	time(&begin);
	runEvolution();
	time(&end);
	cout << "Time elapsed: " << difftime(end, begin) << " seconds"<< endl;
	/*
	initializeFitness();

	double genomeA[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	double genomeB[10] = { 0.1, -0.1, 0.1, -0.1, 0.1, -0.1, 0.1, -0.1, 0.1, -0.1 };
	double genomeC[10] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

	std::cout << fitnessFunction(genomeA) << std::endl;
	std::cout << fitnessFunction(genomeB) << std::endl;
	std::cout << fitnessFunction(genomeC) << std::endl;
	 */
	return 0;
}

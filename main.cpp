#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include "Grid.h"
#include "fitness.h"

const int GENERATION_COUNT = 100;
void runEvolution(int FUNCION_ID, double lowerBound, double upperBound)
{
	srand(time(NULL));

	initializeFitness(FUNCION_ID, lowerBound, upperBound);

	Grid grid(lowerBound, upperBound);

	vector<Agent**> neighbourhood;
	vector<Cupid*> cupids;
	vector<Reaper**> reapers;
	vector<Cupid*> cupidsQueue;
	vector<Reaper**> reapersQueue;

	std::ofstream outputFile;
	outputFile.open("log.txt");
	/*outputFile << "fitness \t sumFitness / ca \t e \t ca \t cu \t b \t r \t bred[0] \t bred[1] "
				  "\t bred[2] \t bred[3] \t killed[0] \t killed[1] \t killed[2] \t killed[3] \t " 
				  "cupidGenome[0] / cu \t cupidGenome[1] / cu \t cupidGenome[2] / cu \t "
				  "cupidGenome[3] / cu \t cupidGenome[4] / cu \t reaperGenome[0] / r \t "
	              "reaperGenome[1] / r \t reaperGenome[2] / r \t reaperGenome[3] / r \t "
	              "reaperGenome[4] / r \t breederGenome[0] / b \t breederGenome[1] / b \t "
	              "breederGenome[2] / b"
			      << std::endl;
	*/

	
	
	for(int i = 0; i < GENERATION_COUNT; ++i)
	{
		std::cout << i << std::endl;
		grid.DoMovement(); 

		cupids.clear();
		reapers.clear();

		for (int j = 0; j < GRID_SIZE; ++j)
		{
			for (int k = 0; k < GRID_SIZE; ++k)
			{
				if ((*(grid.GetAgent(j, k))) != NULL)
				{
					(*(grid.GetAgent(j, k)))->IncreaseAge();

					switch ((*(grid.GetAgent(j, k)))->GetType())
					{
						case breeder:
						{
							grid.GetNeighbourhood(j, k, neighbourhood);
							((Breeder*)(*(grid.GetAgent(j, k))))->ProcessNeighbourhood(neighbourhood);
							break;
						}
						case cupid:
						{
							grid.GetNeighbourhood(j, k, neighbourhood);
							((Cupid*)(*(grid.GetAgent(j, k))))->ProcessNeighbourhood(neighbourhood);
							cupids.push_back((Cupid*)(*(grid.GetAgent(j, k))));
							break;
						}
						case reaper:
						{
							grid.GetNeighbourhood(j, k, neighbourhood);
							((Reaper*)(*(grid.GetAgent(j, k))))->ProcessNeighbourhood(neighbourhood);
							reapers.push_back((Reaper**)(grid.GetAgent(j, k)));
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
		std::cout << m_cupids.empty() << std::endl;
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
					b->Breed(parents, empty, lowerBound, upperBound);
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

		for (int j = 0; j < GRID_SIZE; ++j)
		{
			for (int k = 0; k < GRID_SIZE; ++k)
			{
				if ((*(grid.GetAgent(j, k))) != NULL)
				{
					switch ((*(grid.GetAgent(j, k)))->GetType())
					{
					case candidateSolution:
						{
							ca++;

							sumFitness += (*(grid.GetAgent(j, k)))->GetFitness();
							if ((*(grid.GetAgent(j, k)))->GetFitness() > fitness)
							{
								fitness = (*(grid.GetAgent(j, k)))->GetFitness();
							}

							break;
						}
					case cupid:
						{
							cu++;

							double genome[5];
							((Cupid*)(*(grid.GetAgent(j, k))))->GetGenome(genome);
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
							((Breeder*)(*(grid.GetAgent(j, k))))->GetGenome(genome);
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
							((Reaper*)(*(grid.GetAgent(j, k))))->GetGenome(genome);
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

		outputFile << fitness << " \t " << sumFitness / ca << " \t " << e << " \t " << ca << " \t " << cu << " \t " << b << " \t "  << r << " \t ";
 
		for (int l = 0; l < 4; ++l)
		{
			outputFile << bred[l] << " \t ";
		}

		for (int l = 0; l < 4; ++l)
		{
			outputFile << killed[l] << " \t ";
		}

		for (int l = 0; l < 5; ++l)
		{
			outputFile << cupidGenome[l] / cu << " \t ";
		}

		for (int l = 0; l < 5; ++l)
		{
			outputFile << reaperGenome[l] / r << " \t ";
		}

		for (int l = 0; l < 3; ++l)
		{
			if (l < 2)
			{
				outputFile << breederGenome[l] / b << " \t ";
			}
			else {
				outputFile << breederGenome[l] / b;
			}
		}

		outputFile << std::endl;
		outputFile.flush();

	}

	outputFile.close();
}

int main (int argc, char **argv)
{
	time_t begin, end; 
	time(&begin);
	if (argc == 1 || atoi(argv[1]) == 0)
	{
		std::cout << "No argument given. Using Fletcher Powell as fitness function" << std::endl;
		double pi = 3.14159265358979323846;
		runEvolution(0, -pi, pi);
	}
	else 
	{
		int FUNCTION_ID = atoi(argv[1]);
		runEvolution(FUNCTION_ID, -5.0, 5.0);
	}

	time(&end);
	std::cout << "Time elapsed: " << difftime(end, begin) << " seconds"<< endl;
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

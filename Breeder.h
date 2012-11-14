#pragma once
#include "Agent.h"
#include <vector>
#include <cstdlib>
#include <math.h>

using namespace std;

const double CS_MUTATION_RATE = 0.95;
const double FATE_MUTATION_RATE = 0.95;
const double BREEDER_LEARNING_RATE = 0.25;
const double MAX_INITIAL_CS_MUTATION_SIZE = 0.05;
const double MAX_INITIAL_FATE_MUTATION_SIZE = 0.4;
const double MAX_INITIAL_TOURNAMENT_MUTATION_SIZE = 0.4;

class Breeder : public Agent
{
public:
	Breeder(void);
	Breeder(double * genome);
	~Breeder(void);

	double * GetGenome();
	void GetGenome(double outputArray[3]);
	virtual void RandomizeGenome(double lowerBound, double upperBound);
	void Breed(Agent** parents, Agent** emptyPlace, double lowerBound, double upperBound);

private:
	double m_candidateSolutionMutationSize;
	double m_fateMutationSize;
	double m_tournamentMutationSize;

	void DoESLikeMutation(double * genome, int length, double learningRate);
	void DoGaussianMutation(int * genome, int length, double mutationRate, double mutationSize, int lowerBound, int upperBound);
	void DoNonUniformMutation(double * genome, int length, double mutationRate, double mutationSize, double lowerBound, double upperBound);
	void DoUniformCrossover(int * genomeA, int * genomeB, int length, int * target);
	void DoUniformCrossover(double * genomeA, double * genomeB, int length, double * target, double ratio);
};


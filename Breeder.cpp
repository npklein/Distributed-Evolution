#include "Breeder.h"
#include "CandidateSolution.h"
#include "fitness.h"
#include "Cupid.h"
#include "Reaper.h"

// Random number from a normal distribution, taken from: http://www.dreamincode.net/code/snippet1446.htm 
double randn_notrig(double mu=0.0, double sigma=1.0) {
	static bool deviateAvailable=false;        //        flag
	static float storedDeviate;                        //        deviate from previous calculation
	double polar, rsquared, var1, var2;

	//        If no deviate has been stored, the polar Box-Muller transformation is
	//        performed, producing two independent normally-distributed random
	//        deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable) {

		//        choose pairs of uniformly distributed deviates, discarding those
		//        that don't fall within the unit circle
		do {
			var1=2.0*( double(rand())/double(RAND_MAX) ) - 1.0;
			var2=2.0*( double(rand())/double(RAND_MAX) ) - 1.0;
			rsquared=var1*var1+var2*var2;
		} while ( rsquared>=1.0 || rsquared == 0.0);

		//        calculate polar tranformation for each deviate
		polar=sqrt(-2.0*log(rsquared)/rsquared);

		//        store first deviate and set flag
		storedDeviate=var1*polar;
		deviateAvailable=true;

		//        return second deviate
		return var2*polar*sigma + mu;
	}

	//        If a deviate is available from a previous call to this function, it is
	//        returned, and the flag is set to false.
	else {
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}


Breeder::Breeder(void)
{
	m_candidateSolutionMutationSize = -1.0;
	m_fateMutationSize = -1.0;
	m_tournamentMutationSize = -1.0;
	m_type = breeder;
}

Breeder::Breeder( double * genome )
{
	m_candidateSolutionMutationSize = genome[0];
	m_fateMutationSize = genome[1];
	m_tournamentMutationSize = genome[2];
	m_type = breeder;
}

Breeder::~Breeder(void)
{
}

void Breeder::ProcessNeighbourhood(vector<Agent**> const& neighbourhood)
{
	double max = -1.0;

	for (size_t i = 0; i < neighbourhood.size(); ++i)
	{
		if ((*(neighbourhood.at(i))) != NULL && (*(neighbourhood.at(i)))->GetType() == candidateSolution && (*(neighbourhood.at(i)))->GetFitness() > max)
		{
			max = (*(neighbourhood.at(i)))->GetFitness();
		}
	}

	m_fitness = max;
}

double * Breeder::GetGenome()
{
	double * genome = new double[3];

	genome[0] = m_candidateSolutionMutationSize;
	genome[1] = m_fateMutationSize;
	genome[2] = m_tournamentMutationSize;

	return genome;
}

void Breeder::GetGenome( double outputArray[3] )
{
	outputArray[0] = m_candidateSolutionMutationSize;
	outputArray[1] = m_fateMutationSize;
	outputArray[2] = m_tournamentMutationSize;
}
void Breeder::RandomizeGenome(double upperBound, double lowerBound)
{
	m_candidateSolutionMutationSize = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_CS_MUTATION_SIZE;
	m_fateMutationSize = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_FATE_MUTATION_SIZE;
	m_tournamentMutationSize = (double)rand() / (RAND_MAX + 1.0) * MAX_INITIAL_TOURNAMENT_MUTATION_SIZE;
}

void Breeder::Breed( Agent** parents, Agent** emptyPlace, double lowerBound, double upperBound )
{
	switch (parents[0]->GetType())
	{
	case candidateSolution:
		{
			double * newGenome = new double[PROBLEM_DIMENSION];
			std::cout << lowerBound << std::endl;
			DoUniformCrossover(((CandidateSolution*)parents[0])->GetGenome(), ((CandidateSolution*)parents[1])->GetGenome(), PROBLEM_DIMENSION, newGenome, 1.0);
			DoNonUniformMutation(newGenome, PROBLEM_DIMENSION, CS_MUTATION_RATE, m_candidateSolutionMutationSize, lowerBound, upperBound);
			
			*emptyPlace = new CandidateSolution(newGenome);
			
			delete [] newGenome;

			break;
		}
	case cupid:
		{
			int * newIntGenome = new int[1];
			double * newDoubleGenome = ((Cupid*)parents[0])->GetDoubleGenome();

			DoNonUniformMutation(newDoubleGenome, 4, FATE_MUTATION_RATE, m_fateMutationSize, 0.0, 1.0);

			int* iga = ((Cupid*)parents[0])->GetIntGenome();
			int* igb = ((Cupid*)parents[1])->GetIntGenome();

			DoUniformCrossover(iga, igb, 1, newIntGenome);
			DoGaussianMutation(newIntGenome, 1, FATE_MUTATION_RATE, m_tournamentMutationSize, 1, MAX_TOURNAMENT_SIZE);

			*emptyPlace = new Cupid(newDoubleGenome, newIntGenome);

			delete [] newIntGenome;
			delete [] newDoubleGenome;
			delete [] iga;
			delete [] igb;

			break;
		}
	case reaper:
		{
			int * newIntGenome = new int[1];
			double * newDoubleGenome = ((Reaper*)parents[0])->GetDoubleGenome();

			DoNonUniformMutation(newDoubleGenome, 4, FATE_MUTATION_RATE, m_fateMutationSize, 0.0, 1.0);

			int* iga = ((Reaper*)parents[0])->GetIntGenome();
			int* igb = ((Reaper*)parents[1])->GetIntGenome();

			DoUniformCrossover(iga, igb, 1, newIntGenome);
			DoGaussianMutation(newIntGenome, 1, FATE_MUTATION_RATE, m_tournamentMutationSize, 1, MAX_TOURNAMENT_SIZE);

			*emptyPlace = new Reaper(newDoubleGenome, newIntGenome);

			delete [] newIntGenome;
			delete [] newDoubleGenome;
			delete [] iga;
			delete [] igb;

			break;
		}
	case breeder:
		{
			double * newGenome = ((Breeder*)parents[0])->GetGenome();
			DoESLikeMutation(newGenome, 3, BREEDER_LEARNING_RATE);

			*emptyPlace = new Breeder(newGenome);

			delete [] newGenome;

			break;
		}
	default:
		{
			break;
		}
	}
}

void Breeder::DoESLikeMutation( double * genome, int length, double learningRate)
{
	for (int i = 0; i < length; ++i)
	{
		genome[i] *= exp(learningRate * randn_notrig());
	}
}

void Breeder::DoGaussianMutation( int * genome, int length, double mutationRate, double mutationSize, int lowerBound, int upperBound )
{
	for (int i = 0; i < length; ++i)
	{
		if ((double)rand() / (RAND_MAX + 1.0) < mutationRate)
		{
			genome[i] += (int)((randn_notrig() * mutationSize) + 0.5);
			if (genome[i] > upperBound)
			{
				genome[i] = upperBound;
			}
			if (genome[i] < lowerBound)
			{
				genome[i] = lowerBound;
			}
		}
	}
}

void Breeder::DoNonUniformMutation( double * genome, int length, double mutationRate, double mutationSize, double lowerBound, double upperBound )
{
	for (int i = 0; i < length; ++i)
	{
		if ((double)rand() / (RAND_MAX + 1.0) < mutationRate)
		{
			genome[i] += randn_notrig() * mutationSize;
		}
		if (genome[i] > upperBound)
		{
			genome[i] = upperBound;
		}
		if (genome[i] < lowerBound)
		{
			genome[i] = lowerBound;
		}
	}
}

void Breeder::DoUniformCrossover( int * genomeA, int * genomeB, int length, int * target )
{
	for ( int i = 0; i < length; ++i)
	{
		if ((double)rand() / (RAND_MAX + 1.0) < 0.5)
		{
			target[i] = genomeA[i];
		}
		else
		{
			target[i] = genomeB[i];
		}
	}
}

void Breeder::DoUniformCrossover( double * genomeA, double * genomeB, int length, double * target, double ratio )
{
	for ( int i = 0; i < length; ++i)
	{
		if ((double)rand() / (RAND_MAX + 1.0) < 0.5)
		{
			target[i] = (genomeA[i] * ratio) + (genomeB[i] * (1 - ratio));
		}
		else
		{
			target[i] = (genomeB[i] * ratio) + (genomeA[i] * (1 - ratio));
		}
	}
}

#ifndef FITNESS_H
#define FITNESS_H



#ifndef M_PI
  #define M_PI        3.14159265358979323846
#endif

//#define FP

#ifdef FP
	const int FUNCTION_ID = 0; // 0 - Fletcher Powell, 1.. BBOB function ID
	const double lowerBound = -M_PI;
	const double upperBound = M_PI;
#else
	const int FUNCTION_ID = 24; // 0 - Fletcher Powell, 1.. BBOB function ID
	const double lowerBound = -5.0;
	const double upperBound = 5.0;
#endif

const int PROBLEM_DIMENSION = 10;


double fitnessFunction(double * genome);
void initializeFitness();
double randomGene();

#endif // !FITNESS_H

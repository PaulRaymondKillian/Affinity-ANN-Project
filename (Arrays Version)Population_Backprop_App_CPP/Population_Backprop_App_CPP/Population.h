/*------------------------------------------------------------------------
    Program :           ANN header file
    Written by:         Paul Killian
    Created:            2009
    Description:        Header for ANN.cpp
 -------------------------------------------------------------------------*/

#ifndef POPULATION_H
#define POPULATION_H

//Include Files below
#include <sstream>
#include <conio.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include "Backprop.h"

using namespace std;

class Population
{

public:
	
   Population( void );
   Population( string filepath );		//Constructor with 2 parameters
   ~Population( void );									//Destructor

	void RunPopulation( int Epochs );
	void RunGeneration( int Epoch, int NewGenerationMethod, bool GenerateNewPopulation );
	void RunGenerations( void );

	void SortPopulation( void );
	void NewPopulation( int GenerationMethod );

	void ImportPopFile( string filepath );
	void RetainBestBackprop( void );
	double AveragePopulationFitness( void );
	void WritePopFile( string filepath );
	void OutputPopulationData( string filepath );
	void ImportDataSet( string filepath );
	int numRowsInTextFile( string filepath );
	void DeconstructCurrentDirectory( string filepath );
	string GenerateSaveName( void );
	void SavePopData( bool OutputToConsole );
	void Population::a( void );

	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);

	int PopulationSize;
	int exemplarCount;
	int countInputNeurons;
	int countOutputNeurons;

	vector< vector<double> > exemplarInputs;
	vector< vector<double> > exemplarOutputs;
	Backprop *Pop;

	Backprop *sortedPopulation;
	bool *sortedArray;

	string _CLASSNAME;
	string BackpropParms[11];
	int Generations;
	int EpochsPerGeneration;
	int NewGenerationMethod;
	string CurrentDirectory;
	int GenSaveInterval;
	string FileTitle;
	Backprop BestBackprop;
	vector<double> BestBackpropFitness;
	vector<double> AvgGenerationFitness;
	bool VerboseLogging;
	string DataSetFile;
};

#endif
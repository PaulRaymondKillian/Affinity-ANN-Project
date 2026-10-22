/*------------------------------------------------------------------------
	Program :           Affinity Project - Population Object
	Written by:         Paul Killian
	Created:            2020
	Description:        Contains all the needed information for a Population
	Comment:			This object currently is vestigial and is being extracted from the application, but the file remains
						for reference purposes. The Population logic has been added to the main application and removed from
						being a separate object. This purpose of this attempt is to resolve a memory leak issue that has
						so far been extremely tricky to resolve with the original legacy program code.
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
#include "Affinity_Backprop.h"

using namespace std;

class Population
{

public:

	Population(void);
	Population(string filepath);		//Constructor with 2 parameters
	~Population(void);									//Destructor

	void RunPopulation(int Epochs);
	void RunGeneration(int Epoch, int NewPopGenerationMethod, bool GenerateNewPopulation);
	void RunGenerations(void);

	void SortPopulation(void);
	void NewPopulation(int GenerationMethod);

	void ImportAPopFile(string filepath);
	void RetainBestAffinityBackprop(void);
	double AveragePopulationFitness(void);
	void WritePopFile(string filepath);
	void OutputPopulationData(string filepath);
	void ImportDataSet(string filepath);
	int numRowsInTextFile(string filepath);
	void DeconstructCurrentDirectory(string filepath);
	string GenerateSaveName(void);
	void SavePopData(bool OutputToConsole);
	//void Population::a(void);
	void DeleteShit(void);

	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);

	int PopulationSize;
	int exemplarCount;
	int countInputNeurons;
	int countOutputNeurons;

	vector< vector<double> > exemplarInputs;
	vector< vector<double> > exemplarOutputs;
	//Backprop[] Population1;
	//Backprop* Population2;
	vector<AffinityBackprop> Pop;

	AffinityBackprop* sortedPopulation;
	bool* sortedArray;

	string _CLASSNAME;
	
	int Generations;
	string AffinityBackpropParms[11];
	int EpochsPerGeneration;
	int NewPopGenerationMethod;
	string CurrentDirectory;
	int GenSaveInterval;
	string FileTitle;
	AffinityBackprop BestAffinityBackprop;
	vector<double> BestAffinityBackpropFitness;
	vector<double> AvgGenerationFitness;
	bool VerboseLogging;
	string DataSetFile;
};

#endif
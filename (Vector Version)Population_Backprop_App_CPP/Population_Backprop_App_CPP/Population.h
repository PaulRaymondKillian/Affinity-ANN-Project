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

	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);

	
	//Private Const _CLASSNAME As String = "Population"
 //   Private BackpropParms(10) As String
 //   Private Population() As Backprop.Backprop
 //   Private PopulationSize As Integer
 //   Private Generations As Integer
 //   Private EpochsPerGeneration As Integer
 //   Private NewGenerationMethod As Integer
 //   Private CurrentDirectory As String
 //   Private GenSaveInterval As Integer
 //   Private FileTitle As String
 //   Private BestBackprop As Backprop.Backprop
 //   Private BestBackpropFitness() As Double
 //   Private AvgGenerationFitness() As Double
 //   Private Started As DateTime
 //   Private VerboseLogging As Boolean

	int const static PopulationSize = 10;
	int const static exemplarCount = 15;
	int const static countInputNeurons = 1;
	int const static countOutputNeurons = 1;

	//vector< Backprop > Pop;
	vector< vector<double> > exemplarInputs;
	vector< vector<double> > exemplarOutputs;
	Backprop Pop[PopulationSize];
	//double exemplarInputs[exemplarCount][countInputNeurons];
	//double exemplarOutputs[exemplarCount][countOutputNeurons];

	string _CLASSNAME;
	string BackpropParms[11];
	//Backprop *Pop;	-- Replacing Array with Vector
	int Generations;
	int EpochsPerGeneration;
	int NewGenerationMethod;
	string CurrentDirectory;
	int GenSaveInterval;
	string FileTitle;
	Backprop BestBackprop;
	//double *BestBackpropFitness;
	//double *AvgGenerationFitness;
	vector<double> BestBackpropFitness;
	vector<double> AvgGenerationFitness;
	//**DateTime Started;
	bool VerboseLogging;

    //'Create Exemplar Set to be passed by reference
    //Public DataSetFile As String
    //Public exemplarCount As Integer
    //Public exemplarInputs(,) As Double
    //Public exemplarOutputs(,) As Double
    //Public countInputNeurons, countOutputNeurons As Integer
	string DataSetFile;
	//double *exemplarInputs;
	//double *exemplarOutputs;
	

    //Dim ConsoleLibrary As New Console_Library.Console_Library   'Used to change text color in console

};

#endif
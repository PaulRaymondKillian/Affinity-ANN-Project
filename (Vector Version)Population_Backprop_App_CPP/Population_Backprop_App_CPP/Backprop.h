/*------------------------------------------------------------------------
    Program :           ANN header file
    Written by:         Paul Killian
    Created:            2009
    Description:        Header for ANN.cpp
 -------------------------------------------------------------------------*/

#ifndef BACKPROP_H
#define BACKPROP_H

//Include Files below
#include <sstream>
#include <conio.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Backprop
{

public:
	
	Backprop( void );								//Default Constructor
	Backprop( string filepath );		//Constructor with 2 parameters
	Backprop( string *pParms, vector< vector<double> > pExemplarInputs, vector< vector<double> > pExemplarOutputs, int pExemplarCar, int pCountInputNeurons, int pCountOutputNeurons);
	//Backprop::Backprop( string *pParms, double **pExemplarInputs, double **pExemplarOutputs, int pExemplarCount, int pCountInputNeurons, int pCountOutputNeurons);
	~Backprop( void );									//Destructor

	void Backprop::importBackpropParms( string *Parms );
	void Backprop::PrepVectors();
	void Backprop::RandomizeWeightAndBiasArrays();
	double Backprop::RandomWeight( double maxValue );
	int Backprop::RandomNumber( int MaxNumber );
	Backprop Backprop::Copy();
	void Backprop::calcFitness( void );
	void Backprop::run( int exemplarIndex );
	void Backprop::runBackprop(int iteration);
	double Backprop::AbsVal( double value );
	void Backprop::Train( int Epochs, bool VerboseLogging);
	void Backprop::TrainEpoch( bool UseRhoMultipler, bool SimulatedAnnealing);
	double Backprop::Activation(double value);
	double Backprop::fPrime(double u);
	vector<int> Backprop::randomizeTheIndex(int count);
	void Backprop::InitiateSimulatedAnnealing();
	void Backprop::outputNetwork( string fileName );
	void Backprop::outputBPROPFile( string filename, string FileTitle );
	void Backprop::outputNetworkOutput( string fileName);




    //Public ID As String 'Optional String To Use to Identifier the specific object
	string ID;

    //Const VALUE As Integer = 0
    //Const BIAS As Integer = 1
    //Const DELTA As Integer = 2
	int static const VALUE = 0;
	int static const BIAS = 1;
	int static const DELTA = 2;

	int const static exemplarCount = 15;
	int const static countInputNeurons = 1;
	int const static countOutputNeurons = 1;

	vector< vector<double> > exemplarInputs;
	vector< vector<double> > exemplarOutputs;
	//double **exemplarInputs;
	//double **exemplarOutputs;

    //Public DataFileName As String
    //Public _Parms() As String
	string DataFileName;
	string *Parms;

    //'Network Structure Variables
    //Public countInputNeurons As Integer
    //Public countHiddenNeurons As Integer
    //Public countHiddenLayers As Integer
    //Public countOutputNeurons As Integer
    //Public epochs As Integer
	int countHiddenNeurons;
	int countHiddenLayers;
	int epochs;

    //Public outputInterval As Integer
    //Public outputDeltaInterval As Integer
	int outputInterval;
	int outputDeltaInterval;

    //'Create Exemplar Set
    //Public exemplarCount As Integer
    //Dim exemplarInputs(,) As Double
    //Dim exemplarOutputs(,) As Double

    //Dim randomizedIndex() As Integer
	vector<int> randomizedIndex;

    //'Dynamic Rho Variables
    //Public rho As Double
    //Public rhoInterval As Integer
    //Public rhoMultiplier As Double
	double rho;
	int rhoInterval;
	double rhoMultiplier;

    //'Neuron arrays
    //Dim inputValues() As Double
    //Public hiddenValues(,,) As Double
    //Public outputValues(,) As Double
	vector<double> inputValues;
	vector< vector< vector<double> > > hiddenValues;
	vector< vector<double> > outputValues;


    //'Weight arrays
    //Public ihWeights(,,) As Double 'Increased dimension of weight array to include CURRENT/PREVIOUS weights (Momentum)
    //Public hhWeights(,,,) As Double 'Increased dimension of weight array to include CURRENT/PREVIOUS weights (Momentum)
    //Public hoWeights(,,) As Double 'Increased dimension of weight array to include CURRENT/PREVIOUS weights (Momentum)
    //Public maxWeightValue As Double
    //Public maxBiasValue As Double
	vector< vector< vector<double> > > ihWeights;
	vector< vector< vector< vector<double> > > > hhWeights;
	vector< vector< vector<double> > > hoWeights;
	double maxWeightValue;
	double maxBiasValue;

    //'Momentum Related Terms
    //Public UseMomentum As Boolean = False   'Set the false by default
    //Public momentumScaler As Double = 0     'Set to 0 by default
    //Const CURRENT As Integer = 0
    //Const PREVIOUS As Integer = 1
	bool UseMomentum;
	double MomentumScaler;
	int static const CURRENT = 0;
	int static const PREVIOUS = 1;

    //'Simulated Annealing Terms
    //Public annealingMaxPercent As Integer
	int AnnealingMaxPercent;

    //'Interval Value When Program Should Ask If You Want to Save (Used for learning tracking)
    //Public SaveInterval As Integer
    //Public CurrentDirectory As String
    //Public FileTitle As String
	int SaveInterval;
	string CurrentDirectory;
	string FileTitle;

    //'Fitness Variable to determine accuracy/quality of backprop network
    //Public Fitness As Double = 0
	double Fitness;

    //'Store ANN File location used for BPROP output file
    //Private ANNFileName As String
	string ANNFileName;

};

#endif
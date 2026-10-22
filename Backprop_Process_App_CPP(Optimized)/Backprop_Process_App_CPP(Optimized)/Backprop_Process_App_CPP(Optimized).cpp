// Backprop_Process_App_CPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <windows.h>	//Color Console

using namespace std;

HANDLE  hConsole; //Color Console
enum { Blank, DarkBlue, DarkGreen, DarkLightBlue, DarkRed, Purple, DarkYellow, White, Grey, Blue, Green, LightBlue, Red, Pink, Yellow, BrightWhite };	//Color Console

/////////////////////////////////////////////////////////////////////////////////////
//DECLARE VARIABLES

//CONST VALUES
int static const VALUE = 0;
int static const BIAS = 1;
int static const DELTA = 2;
///5double static MaxWeightValue;
//Exemplar Information
string DataSetFile;
int countExemplarSets;
double **InputExemplars;
double **OutputExemplars;

//Architecture
string ANNFileName;
int countInputNeurons;
int countHiddenNeurons;
int countHiddenLayers;
int countOutputNeurons;

//Neuron Arrays
double *inputValues;
double ***hiddenValues;
double **outputValues;

//Weights Arrays
double **ihWeights;
double ***hhWeights;
double **hoWeights;

//Training Configuration
int EpochsPerInterval;
int Intervals;
double MaxWeightValue;
double MaxBiasValue;
double Rho;
double RhoMultiplier;
int RhoInterval;
bool Momentum;
double MomentumScaler;
int SimAnnealMaxPercent;
int SimAnnealInterval;

//Misc Variables
string FileTitle;
int SaveInterval;
int ConsoleLoggingInterval;
int *RandomArray;
time_t TimeIntervalStart;
time_t TimeIntervalEnd;
time_t TimeTrainingStart;
time_t TimeTrainingEnd;
double PreviousFitness;
double CurrentFitness;
int const LargeSetPrecisionValue = 10;
int const SmallSetPrecisionValue = 5;
string CurrentDirectory;
string InputFile;
vector<string> AppPreviousEntries(10);
int countPreviousEntries;
string TrainingAverages;

// END VARIABLE DECLARATIONS
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Begin Method PROTOS

void ImportBPROP2File( string filepath );
void RandomizeTheIndex( void );
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
void InitializeArrays( void );
void RandomizeWeightAndBiasArrays();
void ImportExemplarSet( void );
void InitializeArrays( void );
int numRowsInTextFile( string filepath );
double RandomWeight( double maxValue );
int RandomNumber( int MaxNumber );
double AbsVal( double value );
double CalcFitness( void );
void OutputBPROP2File( string filename, string FileTitle );
void OutputBPROPFile( string filename);
void OutputNetwork( string fileName );
void OutputNetworkOutput( string fileName );
void SaveData( void );
void DeconstructCurrentDirectory( string filepath );
string GenerateSaveName( void );
void SelectInputFile( void );
void LoadBPROP2File( void );
void LoadBPROPFile( void );
void ImportANNFile( void );
int FindNearestPowerOf2( int value );


// END METHOD PROTOS
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// MAIN PROGRAM BEGINS HERE
int _tmain(int argc, _TCHAR* argv[])
{
	/////////////////////////////////////////////////////////////////////
	// BEGIN WINDOWS API CALLS TO MODIFY CONSOLE SCREEN
	////Begin Console Color Prep
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Color Console
	////Console Buffer Mod
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    COORD NewSBSize;
	GetConsoleScreenBufferInfo(hConsole, &SBInfo);
	NewSBSize.X = 300;
    NewSBSize.Y = 500;
    SetConsoleScreenBufferSize(hConsole, NewSBSize);
	/////Console Window Size Mod
	SMALL_RECT DisplayArea = {0, 0, 0, 0};
	int Right = 120;
	int Bottom = 30;
	COORD NewSBSize2;
	NewSBSize = GetLargestConsoleWindowSize(hConsole);
	if( NewSBSize.X <= Right )	//If size is too big for screen
		Right = NewSBSize.X-10;	//Use max screen size - 10
	DisplayArea.Right = Right;
    DisplayArea.Bottom = Bottom;
	SetConsoleWindowInfo(hConsole, TRUE, &DisplayArea);
	// END WINDOWS API CALLS TO MODIFY CONSOLE
	/////////////////////////////////////////////////////////////////////////


	SetConsoleTextAttribute(hConsole, Red);	//Color Console

	cout << "Backprop Process App CPP - Affinity Project (2020)" << endl;
    cout << "Created by: Paul Killian";
    cout << endl << endl;

	SetConsoleTextAttribute(hConsole, White);	//Color Console
	SelectInputFile();

	if( InputFile.find ( string ( ".BPROP2")) != -1 )
		LoadBPROP2File();
	else if( InputFile.find ( string ( ".BPROP")) != -1 )
		LoadBPROPFile();
	else
	{	cout << "Invalid File Inputted... Exiting";	exit;	}

	TimeTrainingStart = time(0);
	for(int Interval=0; Interval<Intervals; Interval++)
	{
		TimeIntervalStart = time(0);
		if( Interval == 0 )
			PreviousFitness = CalcFitness();
		else
			PreviousFitness = CurrentFitness;

		//Begin Sim Anneal Logic
		if( (Interval+1) % SimAnnealInterval == 0 )
		{
			double Fit1, Fit2;
			Fit1 = CalcFitness();
			SetConsoleTextAttribute(hConsole, White);	//Color Console
			cout << "Simulated Annealing Initiated...	Rho: " << Rho << ", MaxPer: " << SimAnnealMaxPercent <<"%,	Delta Variance: (+/-)" << SimAnnealMaxPercent * Rho * 100 <<"% of Prev Weight" << endl;
			//Calculate Input-Hidden Sim Annealed Weights
			for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
				for(int outputNeuron=0; outputNeuron<countOutputNeurons; outputNeuron++)
						hoWeights[hiddenNeuron][outputNeuron] = hoWeights[hiddenNeuron][outputNeuron] + ( RandomWeight(1) * (SimAnnealMaxPercent * hoWeights[hiddenNeuron][outputNeuron]) * (Rho * 100));

				//Calculate Hidden-Hidden Sim Annealed Weights
				if( countHiddenLayers>1)
					for(int layer=countHiddenLayers-2; layer>=0; layer--)
						for(int belowHidden=0; belowHidden<countHiddenNeurons; belowHidden++)
							for(int aboveHidden=0; aboveHidden<countHiddenNeurons; aboveHidden++)
								hhWeights[layer][belowHidden][aboveHidden] = hhWeights[layer][belowHidden][aboveHidden]  + ( RandomWeight(1) * (SimAnnealMaxPercent *  hhWeights[layer][belowHidden][aboveHidden]) * (Rho * 100));


				//Calculate Input-Hidden Sim Annealed Weights
				for(int inputNeuron=0; inputNeuron<countInputNeurons; inputNeuron++)
					for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
						ihWeights[inputNeuron][hiddenNeuron] = ihWeights[inputNeuron][hiddenNeuron]  + ( RandomWeight(1) * (SimAnnealMaxPercent *  ihWeights[inputNeuron][hiddenNeuron]) * (Rho * 100));
			Fit2 = CalcFitness();
			cout << "Sim Anneal Analytics - Prev Fit: " << Fit1 << ", After Fit: " << Fit2 << endl;
		}//if (Sim Anneal)

		SetConsoleTextAttribute(hConsole, Grey);	//Color Console
		for(int Epoch=0; Epoch<EpochsPerInterval; Epoch++)
		{
			//Dynamic Rho Logic
			if( (Epoch+1) % RhoInterval == 0 )
				Rho *= RhoMultiplier;
			
			//Verbose Console Logging Logic
			if( (Epoch+1) % ConsoleLoggingInterval == 0 )
				cout << ".";

			RandomizeTheIndex();
			for( int ExemplarIndex = 0; ExemplarIndex < countExemplarSets; ExemplarIndex++ )
			{
				////////////////////////////////////////////////////////////////////////////
				//Run The Network
				////////////////////////////////////////////////////////////////////////////

				//Set Input Neurons
				for(int i=0; i<=countInputNeurons-1; i++)
					inputValues[i] = InputExemplars[RandomArray[ExemplarIndex]][i];

				//Calculate First Layer of Hidden Neuron Values
				for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
				{
					hiddenValues[0][hiddenNeuron][VALUE] = 0; //Reset to 0
					for(int inputNeuron=0; inputNeuron<countInputNeurons; inputNeuron++)
						hiddenValues[0][hiddenNeuron][VALUE] += inputValues[inputNeuron] * ihWeights[inputNeuron][hiddenNeuron];

					//hiddenValues[0][hiddenNeuron][VALUE] = Activation(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]);
					hiddenValues[0][hiddenNeuron][VALUE] = (1 / (1 + pow(2.71828183,-(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]))));
				}

				//Calculate Hidden Layer Neuron Values
				if( countHiddenLayers > 1)
				{
					for(int layer=1; layer<countHiddenLayers; layer++)
					{
						for(int aboveHidden=0; aboveHidden<countHiddenNeurons; aboveHidden++)
						{
							hiddenValues[layer][aboveHidden][VALUE] = 0;
							for(int belowHidden=0; belowHidden<countHiddenNeurons; belowHidden++)
							{
								hiddenValues[layer][aboveHidden][VALUE] += hiddenValues[layer-1][belowHidden][VALUE] * hhWeights[layer-1][belowHidden][aboveHidden];
							}
							//hiddenValues[layer][aboveHidden][VALUE] = Activation(hiddenValues[layer][aboveHidden][VALUE] +hiddenValues[layer][aboveHidden][BIAS]);
							hiddenValues[layer][aboveHidden][VALUE] = (1 / (1 + pow(2.71828183,-(hiddenValues[layer][aboveHidden][VALUE] + hiddenValues[layer][aboveHidden][BIAS]))));
						}
					}
				}

				//Calculate Output Neuron Values
				for( int outputNeuron=0; outputNeuron<countOutputNeurons; outputNeuron++)
				{
					outputValues[outputNeuron][VALUE] = 0;	//Reset to 0
					for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
					{
						outputValues[outputNeuron][VALUE] += hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE] * hoWeights[hiddenNeuron][outputNeuron];
					}
					//We are not squashing the output neurons, so simply add the bias to it
					outputValues[outputNeuron][VALUE] += outputValues[outputNeuron][BIAS];
				}


				//////////////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////////////
				// THE ANN IS NOW CALCULATED, NOW BEGIN TRAINING THE NETWORK FOR THIS EXEMPLAR SET				//
				//////////////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////////////


				//////////////////////////////////////////////////////////////////////////////////////////////////
				// Run the Backprop Training Algorithm															//
				//////////////////////////////////////////////////////////////////////////////////////////////////

				//////////////////////////////
				// CALCULATE DELTAS			//
				//////////////////////////////

				//Calculate Output Neuron Deltas
				for(int i=0; i<countOutputNeurons; i++)
					outputValues[i][DELTA] = -2 * (OutputExemplars[RandomArray[ExemplarIndex]][i] - outputValues[i][VALUE]);

				//Calculate Top Layer Hidden Neuron Deltas
				//double totalDeltas = 0	//May use later, but leave commented out for now...
				for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
				{
					hiddenValues[countHiddenLayers-1][hiddenNeuron][DELTA] = 0;
					for(int outputNeuron=0; outputNeuron<countOutputNeurons; outputNeuron++)
					{
						hiddenValues[countHiddenLayers-1][hiddenNeuron][DELTA] += ((hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE]) * (1 - (hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE]))) * outputValues[outputNeuron][DELTA] * hoWeights[hiddenNeuron][outputNeuron];
						//totalDeltas += hiddenValues[countHiddenLayers-1][hiddenNeuron][DELTA] //Keep a running total of delta values for analysis
					}
				}

				//Calculate Hidden Layer Neuron Deltas
				if( countHiddenLayers>1 )
				{
					for(int layer=countHiddenLayers-2; layer>=0; layer--)
					{
						for(int belowHidden=0; belowHidden<countHiddenNeurons; belowHidden++)
						{
							hiddenValues[layer][belowHidden][DELTA] = 0;	//Reset value
								for(int aboveHidden=0; aboveHidden<countHiddenNeurons; aboveHidden++)
								{
									//hiddenValues[layer][belowHidden][DELTA] += fPrime(hiddenValues[layer][belowHidden][VALUE]) * hiddenValues[layer+1][aboveHidden][DELTA] * hhWeights[layer][belowHidden][aboveHidden][CURRENT];
									hiddenValues[layer][belowHidden][DELTA] += ((hiddenValues[layer][belowHidden][VALUE]) * (1 - (hiddenValues[layer][belowHidden][VALUE]))) * hiddenValues[layer+1][aboveHidden][DELTA] * hhWeights[layer][belowHidden][aboveHidden];
									//totalDeltas += hiddenValues[layer][belowHidden][DELTA];	//Keep a running total of delta values for analysis
								}
						}
					}
				}
				
				//////////////////////////////////////
				// CALCULATE NEW WEIGHTS			//
				//////////////////////////////////////

				//Calculate New Weights for hoWeights
				for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
					for(int outputNeuron=0; outputNeuron<countOutputNeurons; outputNeuron++)
						hoWeights[hiddenNeuron][outputNeuron] = hoWeights[hiddenNeuron][outputNeuron] - Rho * outputValues[outputNeuron][DELTA] * hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE];

				//Calculate New Hidden-Hidden Weights
				if( countHiddenLayers>1)
					for(int layer=countHiddenLayers-2; layer>=0; layer--)
						for(int belowHidden=0; belowHidden<countHiddenNeurons; belowHidden++)
							for(int aboveHidden=0; aboveHidden<countHiddenNeurons; aboveHidden++)
								hhWeights[layer][belowHidden][aboveHidden] = hhWeights[layer][belowHidden][aboveHidden] - Rho * hiddenValues[layer+1][aboveHidden][DELTA] * hiddenValues[layer][belowHidden][VALUE];

				//Calculate Input-Hidden New Weights
				for(int inputNeuron=0; inputNeuron<countInputNeurons; inputNeuron++)
					for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
						ihWeights[inputNeuron][hiddenNeuron] = ihWeights[inputNeuron][hiddenNeuron] - Rho * hiddenValues[0][hiddenNeuron][DELTA] * inputValues[inputNeuron];

			}//for(ExemplarIndex)
		}//for(Epoch)
		TimeIntervalEnd = time(0);
		
		cout << " - Int: " << Interval << ", " << EpochsPerInterval << " Epochs: " << difftime(TimeIntervalEnd, TimeIntervalStart) << " s, " << countExemplarSets << " Ex Sets, Rho: " << Rho << ", Mom: " << Momentum << ", Arch: " << countInputNeurons << "-" << countHiddenNeurons << "-" << countHiddenLayers << "-" << countOutputNeurons <<endl;

		CurrentFitness = CalcFitness();

		if( CurrentFitness-PreviousFitness < 0 )
			SetConsoleTextAttribute(hConsole, Green);	//Color Console
		else if( CurrentFitness-PreviousFitness > 0 )
			SetConsoleTextAttribute(hConsole, Red);	//Color Console
		else
			SetConsoleTextAttribute(hConsole, White);	//Color Console

		cout << "Fitness: " << CurrentFitness << "	PrvFitness: " << PreviousFitness << "	Diff: " << CurrentFitness - PreviousFitness << "	Avg: " << CurrentFitness / countExemplarSets << " Max Initial Weight = " << MaxWeightValue << " Max Initial Bias = " << MaxBiasValue << endl;
		//cout << "Fitness: " << CurrentFitness << "	PrvFitness: " << PreviousFitness << "	Diff: " << CurrentFitness-PreviousFitness << "	Avg: " << CurrentFitness / countExemplarSets << " Max Initial Weight = " << MaxWeightValue " Max Initial Bias "0 << endl;
	
		TrainingAverages = TrainingAverages + to_string(CurrentFitness / countExemplarSets) + ",";

		if( (Interval+1) % SaveInterval == 0 )
			SaveData();

	}//for(Interval)
	TimeTrainingEnd = time(0);
	SetConsoleTextAttribute(hConsole, White);	//Color Console
	cout << "Training Complete in " << difftime(TimeTrainingEnd, TimeTrainingStart) << "seconds." << endl;
	SaveData();


	//TrainingAverages = TrainingAverages + "Hello You Stupid World!";
	//cout << TrainingAverages;
	cout << "Saving Training Averages to File.";
	std::ofstream os(CurrentDirectory + FileTitle + "_TrainingAverages.csv");
	os << TrainingAverages;
	if (!os) 
	{ 
		std::cerr << "Error writing to ..." << std::endl; 
		cout << "Error writing to file " + CurrentDirectory + FileTitle + "_TrainingAverages.csv";
	}
	else 
	{
		os << TrainingAverages;
		cout << "Successfully wrote to file " + CurrentDirectory + FileTitle + "_TrainingAverages.csv";
		cout << "values: " + TrainingAverages;
	}
	os.close();
	// Note: file is automatically closed, when current scope is left 

	cout << endl << "Please Press Enter to Exit...";
	cin.ignore();
	return 0;
}
// MAIN PROGRAM ENDS HERE
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////
// METHODS BEGIN HERE

void InitializeArrays( void )
{
	//Optimization: Adust array sizes to power of 2
	//Optimization: Explicitly align data to coincide with cache lines - __declspec(align(64))

	//Initialize RandomArray
	RandomArray = new __declspec(align(64)) int[FindNearestPowerOf2(countExemplarSets)];
	
	//Initialize Input Values Array
	inputValues = new __declspec(align(64)) double[FindNearestPowerOf2(countInputNeurons)];

	//Initialize Hidden Values Array
	hiddenValues = new double**[FindNearestPowerOf2(countHiddenLayers)];
	for(int i=0; i<countHiddenLayers; i++)
	{
		double **arr;
		arr = new double*[FindNearestPowerOf2(countHiddenNeurons)];
		for(int j=0; j<countHiddenNeurons; j++)
		{
			arr[j] = new __declspec(align(64)) double[FindNearestPowerOf2(3)];
		}
		hiddenValues[i] = arr;
	}

	//Initialize Output Values Array
	outputValues = new double*[FindNearestPowerOf2(countOutputNeurons)];
	for(int i=0; i<countOutputNeurons; i++)
		outputValues[i] = new __declspec(align(64)) double[FindNearestPowerOf2(3)];


	///////////////////////////////////////////
	////Verify Input Neuron Array Allocation
	//cout << endl << "Input Neurons: " << endl;
	//for(int i=0; i<countInputNeurons; i++)
	//	inputValues[i] = i;

	//for(int i=0; i<countInputNeurons; i++)
	//	cout << inputValues[i] << ",";
	///////////////////////////////////////////

	///////////////////////////////////////////
	////Verify Hidden Neuron Array Allocation
	//cout << endl << "Hidden Neurons: " << endl;
	//for(int i=0; i<countHiddenLayers; i++)
	//	for(int j=0; j<countHiddenNeurons; j++)
	//		for(int k=0; k<3; k++)
	//			hiddenValues[i][j][k] = j;

	//for(int i=0; i<countHiddenLayers; i++)
	//{
	//	for(int j=0; j<countHiddenNeurons; j++)
	//	{
	//		for(int k=0; k<3; k++)
	//			cout << hiddenValues[i][j][k] << ",";

	//		cout << " - ";
	//	}
	//	cout <<"===";
	//}
	//////////////////////////////////////////

	//////////////////////////////////////////
	//// Verify Output Neuron Array Allocation
	//cout << endl << "Output Neurons: " << endl;
	//for(int i=0; i<countOutputNeurons; i++)
	//	for(int j=0; j<3; j++)
	//		outputValues[i][j] = i;

	//for(int i=0; i<countOutputNeurons; i++)
	//	for(int j=0; j<3; j++)
	//		cout << outputValues[i][j] << ",";
	///////////////////////////////////////////
	//
	///////////////////////////////////////////
	////Output Arrays Againt to Verify No Over-written records
	//cout << endl << "Input Neurons: " << endl;
	//for(int i=0; i<countInputNeurons; i++)
	//	cout << inputValues[i] << ",";


	//cout << endl << "Hidden Neurons: " << endl;
	//for(int i=0; i<countHiddenLayers; i++)
	//{
	//	for(int j=0; j<countHiddenNeurons; j++)
	//	{
	//		for(int k=0; k<3; k++)
	//			cout << hiddenValues[i][j][k] << ",";

	//		cout << " - ";
	//	}
	//	cout <<"===";
	//}

	//cout << endl << "Output Neurons: " << endl;
	//for(int i=0; i<countOutputNeurons; i++)
	//	for(int j=0; j<3; j++)
	//		cout << outputValues[i][j] << ",";
	///////////////////////////////////////////







	//Initialize Input-Hidden Weights Array
	ihWeights = new double*[FindNearestPowerOf2(countInputNeurons)];
	for(int i=0; i<countInputNeurons; i++)
	{
		double *arr;
		arr = new __declspec(align(64)) double[FindNearestPowerOf2(countHiddenNeurons)];
		ihWeights[i] = arr;
	}

	//Initialize Hidden-Hidden Weights Array
	hhWeights = new double**[FindNearestPowerOf2(countHiddenLayers)];
	for(int x=0; x<countHiddenLayers; x++)
	{
		double **a;
		a = new double*[FindNearestPowerOf2(countHiddenNeurons)];
		for(int j=0; j<countHiddenNeurons; j++)
		{
			double *arr;
			arr = new __declspec(align(64)) double[FindNearestPowerOf2(countHiddenNeurons)];
			
			a[j] = arr;
		}
		hhWeights[x] = a;
	}


	//Initialize Hidden-Output Weights Array
	hoWeights = new double*[FindNearestPowerOf2(countHiddenNeurons)];
	for(int i=0; i<countHiddenNeurons; i++)
	{
		double *arr;
		arr = new __declspec(align(64)) double[FindNearestPowerOf2(countOutputNeurons)];

		hoWeights[i] = arr;
	}
}//InitializeArrays()

void RandomizeWeightAndBiasArrays()
{
	for(int i=0; i<countInputNeurons;i++)
		for(int j=0; j<countHiddenNeurons;j++)
			ihWeights[i][j] = RandomWeight(MaxWeightValue);

		for(int layer=0; layer<countHiddenLayers-1;layer++)
			for(int i=0; i<countHiddenNeurons;i++)
				for(int j=0; j<countHiddenNeurons;j++)
					hhWeights[layer][i][j] = RandomWeight(MaxWeightValue);

		for(int i=0; i<countHiddenNeurons;i++)
			for(int j=0; j<countOutputNeurons;j++)
				hoWeights[i][j] = RandomWeight(MaxWeightValue);

		//Randomize Bias Values
		for(int layer=0; layer<countHiddenLayers; layer++)
			for(int i=0; i<countHiddenNeurons; i++)
				hiddenValues[layer][i][BIAS] = RandomWeight(MaxBiasValue);

		for(int i=0; i<countOutputNeurons; i++)
			outputValues[i][BIAS] = RandomWeight(MaxBiasValue);

}//RandomizeWeightAndBiasArrays()


void ImportExemplarSet( void )
{
	string _METHODNAME = "ImportDataSet(filepath)";
	string line;
	vector<string> inputs;
	vector<string> sInputValues;
	vector<string> sOutputValues;
	countExemplarSets = numRowsInTextFile( DataSetFile );

	ifstream inStream;			//create new ifstream
	char buffer[16384];	//16384 should be enough for even a very large data row
	inStream.open(DataSetFile.c_str());

	inStream.getline(buffer,16384);	//Buffer
	line = buffer;
	Tokenize(line, inputs, "=");

	Tokenize(inputs[0], sInputValues, ",");
	Tokenize(inputs[1], sOutputValues,",");

	countInputNeurons = sInputValues.size();
	countOutputNeurons = sOutputValues.size();

	////Initialize Exemplar Arrays
	//Input Exemplars Array
	InputExemplars = new double*[countExemplarSets];
	for(int j=0; j<countExemplarSets; j++)
	{
		InputExemplars[j] = new double[countInputNeurons];
	}
	//OutputExemplars Array
	OutputExemplars = new double*[countExemplarSets];
	for(int j=0; j<countExemplarSets; j++)
	{
		OutputExemplars[j] = new double[countOutputNeurons];
	}

	for (int i=0; i<= countExemplarSets-1; i++)
	{
		vector<double> RowInputs(countInputNeurons,0);
		for (int k=0; k <= countInputNeurons-1; k++)
		{
			RowInputs[k] = atof(sInputValues[k].c_str());
			cout << atof(sInputValues[k].c_str());
			cout << " = ";
		}

		vector<double> RowOutputs(countOutputNeurons,0);
		for (int k=0; k <= countOutputNeurons-1; k++)
		{
			RowOutputs[k] = atof(sOutputValues[k].c_str());
			cout << atof(sOutputValues[k].c_str());
			cout << endl;
		}

		//Add the Row Vectors to the Exemplar Vectors
		for (int k=0; k<countInputNeurons; k++)
			InputExemplars[i][k] = RowInputs[k];
		
		for (int k=0; k<countOutputNeurons; k++)
			OutputExemplars[i][k] = RowOutputs[k];

		inStream.getline(buffer,16384);	//Buffer
		line = buffer;

		if (line != "")
		{
			inputs.clear();
			Tokenize(line, inputs, "=");

			vector<string> tempInput;
			vector<string> tempOutput;

			sInputValues = tempInput;
			sOutputValues = tempOutput;

			Tokenize(inputs[0], sInputValues, ",");
			Tokenize(inputs[1], sOutputValues,",");
		}
	}
	inStream.close();
}//ImportExemplarSet()

void ImportBPROP2File( string filepath )
{
		ifstream inStream;			//create new ifstream
		string line;
		vector<string> tokens;
		char buffer[2048];

			inStream.open(filepath.c_str());

			inStream.getline(buffer,2048);	//Buffer
			line = buffer;

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			DataSetFile = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer
			
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			EpochsPerInterval = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			Intervals = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			countHiddenNeurons = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			countHiddenLayers = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			MaxWeightValue = atof(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			MaxBiasValue = atof(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			Rho = atof(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			RhoMultiplier = atof(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			RhoInterval = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			if(tokens[1] == "True" || tokens[1] == "T")
				Momentum = true;
			else
				Momentum = false;

			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			MomentumScaler = atof(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			SimAnnealMaxPercent = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			SimAnnealInterval = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			SaveInterval = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			FileTitle = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			ConsoleLoggingInterval = atoi(tokens[1].c_str());
			tokens.clear();


		inStream.close();

}//ImportBPROP2File

void ImportBPROPFile( string filepath )
{
		ifstream inStream;			//create new ifstream
		string line;
		vector<string> tokens;
		char buffer[2048];

		try
		{
			inStream.open(filepath.c_str());

			inStream.getline(buffer,2048);	//Buffer
			line = buffer;

			inStream.getline(buffer,2048);	//Ignore ID
			line = buffer;
			
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			FileTitle = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			DataSetFile = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			ANNFileName = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			Rho = atof(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			RhoMultiplier = atof(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			RhoInterval = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			if(tokens[1] == "True" || tokens[1] == "T")
				Momentum = true;
			else
				Momentum = false;

			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			MomentumScaler = atof(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			SimAnnealMaxPercent = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			if( line != "" )
			{
			Tokenize(line, tokens, "=");
			SimAnnealInterval = atoi(tokens[1].c_str());
			tokens.clear();
			}
			else
			{
				cout << "Warning- BPROP File Lacking SimAnnealValue (Probably using obsolete BPROP version) - using default value: 25" << endl;
				SimAnnealInterval = 25;
			}
		}
		catch(...)
		{
			cout << "Error Reading BPROP File!";
		}

		inStream.close();

}//ImportBPROPFile


void RandomizeTheIndex( void )
{
	//Set initial values
	for(int i=0; i<countExemplarSets; i++)
		RandomArray[i] = i;

	//Randomize the values
	int rand1, rand2, tempVal;
	for(int i=0; i<countExemplarSets; i++)
	{
		rand1 = AbsVal(RandomNumber(countExemplarSets));
		rand2 = AbsVal(RandomNumber(countExemplarSets));

		tempVal = RandomArray[rand1];
		RandomArray[rand1] = RandomArray[rand2];
		RandomArray[rand2] = tempVal;
	}
}

void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}//Tokenize

int numRowsInTextFile( string filepath )
{
	ifstream inStream;			//create new ifstream
	string line;
	int rows = 0;
	char buffer[16384];	//16384 should be enough for even a very large data row

	inStream.open(filepath.c_str());		

	do
	{
		inStream.getline(buffer,16384);	//Buffer
		line = buffer;
		if( line != "" )
			rows += 1;
	}
	while(line != "");

	return rows;

}

double RandomWeight( double maxValue )
{
	double randWeight;
	if( maxValue > 1 || maxValue < -1)
		randWeight = RandomNumber(maxValue-1) + (double(RandomNumber(1000)) / 1000);
	else
		randWeight = (double(RandomNumber(1000)) / 1000) * maxValue + (double(RandomNumber(1000000)) / 1000000) * maxValue ;

	return randWeight;
}//RandomWeight()

int RandomNumber( int MaxNumber )
{
	return rand() % MaxNumber;
}

double AbsVal( double value )
{
	if( value < 0 )
		return -value;
	else
		return value;
}//AbsVal()

double CalcFitness( void )
{
	double Fitness = 0;
	double ActualValue;
	double DesiredValue;

	for(int iteration=0; iteration<countExemplarSets; iteration++)
	{
		
		////////////////////////////////////////////////////////////////////////////
		//Run The Network
		////////////////////////////////////////////////////////////////////////////

		//Set Input Neurons
		for(int i=0; i<=countInputNeurons-1; i++)
			inputValues[i] = InputExemplars[iteration][i];

		//Calculate First Layer of Hidden Neuron Values
		for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
		{
			hiddenValues[0][hiddenNeuron][VALUE] = 0; //Reset to 0
			for(int inputNeuron=0; inputNeuron<countInputNeurons; inputNeuron++)
				hiddenValues[0][hiddenNeuron][VALUE] += inputValues[inputNeuron] * ihWeights[inputNeuron][hiddenNeuron];

			//hiddenValues[0][hiddenNeuron][VALUE] = Activation(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]);
			hiddenValues[0][hiddenNeuron][VALUE] = (1 / (1 + pow(2.71828183,-(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]))));
		}

		//Calculate Hidden Layer Neuron Values
		if( countHiddenLayers > 1)
		{
			for(int layer=1; layer<countHiddenLayers; layer++)
			{
				for(int aboveHidden=0; aboveHidden<countHiddenNeurons; aboveHidden++)
				{
					hiddenValues[layer][aboveHidden][VALUE] = 0;
					for(int belowHidden=0; belowHidden<countHiddenNeurons; belowHidden++)
					{
						hiddenValues[layer][aboveHidden][VALUE] += hiddenValues[layer-1][belowHidden][VALUE] * hhWeights[layer-1][belowHidden][aboveHidden];
					}
					//hiddenValues[layer][aboveHidden][VALUE] = Activation(hiddenValues[layer][aboveHidden][VALUE] +hiddenValues[layer][aboveHidden][BIAS]);
					hiddenValues[layer][aboveHidden][VALUE] = (1 / (1 + pow(2.71828183,-(hiddenValues[layer][aboveHidden][VALUE] + hiddenValues[layer][aboveHidden][BIAS]))));
				}
			}
		}

		//Calculate Output Neuron Values
		for( int outputNeuron=0; outputNeuron<countOutputNeurons; outputNeuron++)
		{
			outputValues[outputNeuron][VALUE] = 0;	//Reset to 0
			for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
			{
				outputValues[outputNeuron][VALUE] += hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE] * hoWeights[hiddenNeuron][outputNeuron];
			}
			//We are not squashing the output neurons, so simply add the bias to it
			outputValues[outputNeuron][VALUE] += outputValues[outputNeuron][BIAS];
		}

			/////////////////////////////////////////////////////////////////////////
			// END RUN NETWORK													   //
			/////////////////////////////////////////////////////////////////////////

		for(int i=0; i<=countOutputNeurons-1; i++)
		{
			ActualValue = outputValues[i][VALUE];
			DesiredValue = OutputExemplars[iteration][i];
			Fitness += AbsVal(DesiredValue - ActualValue);
		}
	}
	return Fitness;
}//calcFitness()

void OutputBPROP2File( string filename)
{
	if( DataSetFile == "")
		throw exception("Error creating BPROP2 File: DataFileName cannot be nothing!");

	if( ANNFileName == "")
		throw exception("Error creating BPROP2 File: ANNFileName cannot be nothing, output ANN File first, then create BPROP file.");

	ofstream file;
	file.open(filename.c_str());
	file.precision(LargeSetPrecisionValue);
	file.setf(ios::fixed);
	try
	{
            file << "### Data Set ###" << endl;
            file << "Dataset File Location=" << DataSetFile << endl;
            file << endl;
            file << "### Backprop Configuration ###" << endl;
            file << "Epochs Per Interval=" << EpochsPerInterval << endl;
            file << "Intervals=" << Intervals << endl;
            file << endl;
			file << "### Network Topology ###" << endl;
			file << "Hidden Neurons=" << countHiddenNeurons << endl;
			file << "Hidden Layers=" << countHiddenLayers << endl;
			file << endl;
            file << "### Training Configuration ###" << endl;
			file << "Max Weight Value=" << MaxWeightValue << endl;
			file << "Max Bias Value=" << MaxBiasValue << endl;
            file << "Initial rho value=" << Rho << endl;
            file << "Rho multiplier=" << RhoMultiplier << endl;
            file << "Rho interval=" << RhoInterval << endl;
            file << "Use Momentum=" << Momentum << endl;
            file << "Momentum scaler=" << MomentumScaler << endl;
            file << "Simulated annealing max percent(%)=" << SimAnnealMaxPercent << endl;
			file << "Simulated annealing interval=" << SimAnnealInterval << endl;
			file << endl;
			file << "### Save Data Settings ###" << endl;
			file << "Save Interval=" << SaveInterval << endl;
			file << "File Title=" << FileTitle << endl;
			file << endl;
			file << "### Console Logging ###" << endl;
			file << "Console Logging Interval=" << ConsoleLoggingInterval << endl;
	}
	catch(...)
	{
		cout << "Exception Occurred!";
	}
	file.close();
}//outputBPROP2File()

void OutputBPROPFile( string filename)
{

	if( DataSetFile == "")
		throw exception("Error creating BPROP File: DataFileName cannot be nothing!");

	if( ANNFileName == "")
		throw exception("Error creating BPROP File: ANNFileName cannot be nothing, output ANN File first, then create BPROP file.");

	ofstream file;
	file.open(filename.c_str());
	file.precision(LargeSetPrecisionValue);
	file.setf(ios::fixed);
	try
	{
            file << "### Backprop Identification ###" << endl;
            file << "ID=" << FileTitle << endl;
            file << "File Title=" << FileTitle << endl;
            file << endl;
            file << "### Input Files - !!! NEVER MODIFY THIS PART !!! ###" << endl;
            file << "DO NOT MODIFY: Dataset file location=" << DataSetFile << endl;
            file << "DO NOT MODIFY: ANN file location=" << ANNFileName << endl;
            file << endl;
            file << "### Training Configuration ###" << endl;
            file << "Initial rho value=" << Rho << endl;
            file << "Rho multiplier=" << RhoMultiplier << endl;
            file << "Rho interval=" << RhoInterval << endl;
            file << "Use Momentum=" << Momentum << endl;
            file << "Momentum scaler=" << MomentumScaler << endl;
            file << "Simulated annealing max percent(%)=" << SimAnnealMaxPercent << endl;
			file << "Simulated annealing interval=" << SimAnnealInterval << endl;

	}
	catch(...)
	{
		cout << "Exception Occurred!";
	}
	file.close();
}

void OutputNetwork( string fileName )
{
	ANNFileName = fileName;		//Save ANN File Path for use in BPROP File output

	ofstream file;
	file.open(fileName.c_str());
	file.precision(LargeSetPrecisionValue);
	file.setf(ios::fixed);
	file << countInputNeurons << endl;
	file << countHiddenNeurons << endl;
	file << countHiddenLayers << endl;
	file << countOutputNeurons << endl;
	file << ((countInputNeurons + 1) * countHiddenNeurons) + ((countHiddenLayers - 1) * (countHiddenNeurons + 1) * countHiddenNeurons) + ((countHiddenNeurons + 1) * countOutputNeurons) << endl;

	for( int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
	{
		for( int inputNeuron=0; inputNeuron<=countInputNeurons-1; inputNeuron++)
			file << ihWeights[inputNeuron][hiddenNeuron] <<endl;

		file << hiddenValues[0][hiddenNeuron][BIAS] << endl;
	}

	if( countHiddenLayers > 1)
	{
		for( int layer=0; layer<=countHiddenLayers-2; layer++)
		{
			for( int HiddenAbove=0; HiddenAbove<=countHiddenNeurons-1; HiddenAbove++)
			{
				for( int HiddenBelow=0; HiddenBelow<=countHiddenNeurons-1; HiddenBelow++)
					file << hhWeights[layer][HiddenBelow][HiddenAbove] << endl;

				file << hiddenValues[layer+1][HiddenAbove][BIAS] << endl;
			}
		}
	}

	for( int outputNeuron=0; outputNeuron<=countOutputNeurons-1; outputNeuron++)
	{
		for( int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
			file << hoWeights[hiddenNeuron][outputNeuron] << endl;

		file << outputValues[outputNeuron][BIAS] << endl;
	}

	file.close();
}//outputNetwork()

void OutputNetworkOutput( string fileName )
{
	double Fitness = CalcFitness();
	ofstream file;
	file.open(fileName.c_str());
	file.precision(LargeSetPrecisionValue);
	file.setf(ios::fixed);
	try
	{
		file << "Fitness: " << Fitness << " Avg: " << (Fitness / countExemplarSets) << " Rho: " << Rho << endl;

		for( int i=0; i <=countExemplarSets-1; i++ )
		{
			for( int j=0; j<=countInputNeurons-1; j++ )
			{
				file << InputExemplars[i][j];
				if( j != countInputNeurons-1)
					file << ",";
			}

			//////////////////////////////////////////////////////////////////////////
			// BEGIN RUN NETWORK													//
			//////////////////////////////////////////////////////////////////////////

			//Set Input Neurons
			for(int j=0; j<=countInputNeurons-1; j++)
				inputValues[j] = InputExemplars[i][j];

			//Calculate First Layer of Hidden Neuron Values
			for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
			{
				hiddenValues[0][hiddenNeuron][VALUE] = 0; //Reset to 0
				for(int inputNeuron=0; inputNeuron<countInputNeurons; inputNeuron++)
					hiddenValues[0][hiddenNeuron][VALUE] += inputValues[inputNeuron] * ihWeights[inputNeuron][hiddenNeuron];

				//hiddenValues[0][hiddenNeuron][VALUE] = Activation(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]);
				hiddenValues[0][hiddenNeuron][VALUE] = (1 / (1 + pow(2.71828183,-(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]))));
			}

			//Calculate Hidden Layer Neuron Values
			if( countHiddenLayers > 1)
			{
				for(int layer=1; layer<countHiddenLayers; layer++)
				{
					for(int aboveHidden=0; aboveHidden<countHiddenNeurons; aboveHidden++)
					{
						hiddenValues[layer][aboveHidden][VALUE] = 0;
						for(int belowHidden=0; belowHidden<countHiddenNeurons; belowHidden++)
						{
							hiddenValues[layer][aboveHidden][VALUE] += hiddenValues[layer-1][belowHidden][VALUE] * hhWeights[layer-1][belowHidden][aboveHidden];
						}
						//hiddenValues[layer][aboveHidden][VALUE] = Activation(hiddenValues[layer][aboveHidden][VALUE] +hiddenValues[layer][aboveHidden][BIAS]);
						hiddenValues[layer][aboveHidden][VALUE] = (1 / (1 + pow(2.71828183,-(hiddenValues[layer][aboveHidden][VALUE] + hiddenValues[layer][aboveHidden][BIAS]))));
					}
				}
			}

			//Calculate Output Neuron Values
			for( int outputNeuron=0; outputNeuron<countOutputNeurons; outputNeuron++)
			{
				outputValues[outputNeuron][VALUE] = 0;	//Reset to 0
				for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
				{
					outputValues[outputNeuron][VALUE] += hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE] * hoWeights[hiddenNeuron][outputNeuron];
				}
				//We are not squashing the output neurons, so simply add the bias to it
				outputValues[outputNeuron][VALUE] += outputValues[outputNeuron][BIAS];
			}

			/////////////////////////////////////////////////////////////////////////
			// END RUN NETWORK													   //
			/////////////////////////////////////////////////////////////////////////

			file << "=";

			for( int j=0; j<=countOutputNeurons-1; j++ )
			{
				file << outputValues[j][VALUE];
				if( j != countOutputNeurons-1 )
					file << ",";
			}
			file << endl;
		}
	}
	catch(...)
	{
		cout << "Exception Occurred";
	}
	file.close();
}//outputNetworkOutput()

void SaveData( void )
{
	SetConsoleTextAttribute(hConsole, White);	//Color Console
	string SaveName = GenerateSaveName();
	cout << "Saving data to: " << CurrentDirectory << SaveName << "\\" << endl;
	try
	{
		string command;
		command = "mkdir \"" + CurrentDirectory + SaveName + "\"";
		system(command.c_str());

		//cout <<"Saving Network... " << endl;
		OutputNetwork(CurrentDirectory + SaveName + "\\" + SaveName + ".ANN");
		//cout <<"Saving Network Output... " << endl;
		OutputNetworkOutput(CurrentDirectory + SaveName + "\\" + SaveName + ".txt");
		//cout <<"Saving BPROP2 File... " << endl;
		OutputBPROP2File(CurrentDirectory + SaveName + "\\" + SaveName + ".BPROP2");
		//cout <<"Saving BPROP File... " << endl;
		OutputBPROPFile(CurrentDirectory + SaveName + "\\" + SaveName + ".BPROP");
	}
	catch(...)
	{
		cout << "Exception Occurred";
	}
	cout << "Data Saved Successfully!" << endl;

}

void DeconstructCurrentDirectory( string filepath )
{
	vector<string> tokens;

	Tokenize(filepath, tokens, "\\");

	if( tokens.size() > 1) 
	{
		for( int i = 0; i <= tokens.size() -2; i++)
		{
			if( i == 0 )
				CurrentDirectory = tokens[i] + "\\";
			else
				CurrentDirectory += tokens[i] + "\\";
		}
	}
	else
		CurrentDirectory = "";
}

string GenerateSaveName( void )
{
	string AvgError;
	string Dt;

	//Create Average Error String
	ostringstream oss;
	oss << (CalcFitness() / countExemplarSets);
	AvgError = oss.str();

//      'Create Date String
//      Dim CurrentDate As Date = Now
//      Dt = CurrentDate.Month.ToString.PadLeft(2, "0"c) & CurrentDate.Day.ToString.PadLeft(2, "0"c) & CurrentDate.Year.ToString.Substring(2)
//### Add Date Part Later?

	string output;
	output = AvgError;
	output.append("_");
	output.append(FileTitle);

	return output;
}

void SelectInputFile( void )
{
	//Populate AppPreviousEntries
	countPreviousEntries = 0;
	ifstream in("AppPreviousEntries.txt"); // input

	if(!in) {
	cout << "Cannot open test.txt file.\n";
	}
	char buffer[256];

	for(int i=0; i<10; i++)
	{
		in.getline(buffer,256);
		AppPreviousEntries[i] = buffer;
		if(AppPreviousEntries[i] != "")
			countPreviousEntries += 1;
	}
	in.close();


	//Begin Input File Selection
	string file;
	cout << "Previous Input Files: " << endl;
	for(int i=0; i<10; i++)
	{
		if(AppPreviousEntries[i] != "")
			cout << "[" << i+1 << "] - " << AppPreviousEntries[i] << endl;
	}
InputFile:
	cout << "Please Enter Number of Previous Entry to Use or New File Location: ";
	
	string input;
	cin >> input;
	
	int nStartBPROP2, nStartBPROP;
	nStartBPROP2 = input.find ( string ( ".BPROP2"));
	nStartBPROP = input.find ( string ( ".BPROP"));

	if(atoi(input.c_str()) <= countPreviousEntries && atoi(input.c_str()) > 0)
		input = AppPreviousEntries[atoi(input.c_str())-1];
	else if( nStartBPROP != -1 || nStartBPROP2 != -1 )
	{
		//AddFileToPreviousEntries(input);
		for(int i=countPreviousEntries-1; i>=0; i--)
		if(i != 9 )
			AppPreviousEntries[i+1] = AppPreviousEntries[i];

		AppPreviousEntries[0] = input;

		//Update Previous Entries File
		ofstream ofile;
		ofile.open("AppPreviousEntries.txt");

		for(int i=0; i<10; i++)
		{
			if( AppPreviousEntries[i] != "" )
				ofile << AppPreviousEntries[i] << endl;
		}
		ofile.close();
	}
	else
	{
		cout << "Invalid Input Entered! Please Re-enter" << endl;
		goto InputFile;
	}
	cout << "Loading: " << input << "..." << endl;
	InputFile = input;	

}//SelectInputFile

void LoadBPROP2File( void )
{
	int RandSeed = time(NULL)-clock();
	srand ( RandSeed );

	cout << "Importing BPROP2 File...";
	DeconstructCurrentDirectory(InputFile);
	ImportBPROP2File(InputFile);
	cout << "		Success!" << endl;

	cout <<"Importing DataSet File...";
	ImportExemplarSet();
	cout << "		Success!" << endl;

	cout << "Initializing Arrays...";
	InitializeArrays();
	cout << "			Success!" << endl;

	cout << "Randomizing Arrays("<< RandSeed << ")...";
	RandomizeWeightAndBiasArrays();
	cout << "	Success!" << endl;
	cout << endl;
}

void LoadBPROPFile( void )
{
	cout << "Importing BPROP File...";
	DeconstructCurrentDirectory(InputFile);
	ImportBPROPFile(InputFile);
	cout << "		Success!" << endl;

	cout <<"Importing DataSet File...";
	ImportExemplarSet();
	cout << "		Success!" << endl;

	//Arrays will be initialized while importing the ANN file
	////The arrays cannot be initalized any sooner because we wont know the full architecture until we look at the ANN file
	cout << "Importing ANN File...";
	ImportANNFile();
	cout << " Success!(ANN)" << endl;
	cout << endl;


	//OUTPUTTING WEIGHTS
	cout << endl << "howeights:" << endl;
			for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
				for(int outputNeuron=0; outputNeuron<countOutputNeurons; outputNeuron++)
						cout << hoWeights[hiddenNeuron][outputNeuron] << ",";

	cout << endl << "hhweights:" << endl;
				//Calculate Hidden-Hidden Sim Annealed Weights
				if( countHiddenLayers>1)
					for(int layer=countHiddenLayers-2; layer>=0; layer--)
						for(int belowHidden=0; belowHidden<countHiddenNeurons; belowHidden++)
							for(int aboveHidden=0; aboveHidden<countHiddenNeurons; aboveHidden++)
								cout << hhWeights[layer][belowHidden][aboveHidden] << ",";

	cout << endl << "ihweights:" << endl;
				//Calculate Input-Hidden Sim Annealed Weights
				for(int inputNeuron=0; inputNeuron<countInputNeurons; inputNeuron++)
					for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++)
						cout << ihWeights[inputNeuron][hiddenNeuron] << ",";

	cout << endl;
	cout << "Epochs Per Interval: ";
	cin >> EpochsPerInterval;
	cout << "Intervals: ";
	cin >> Intervals;
	cout << "Save Interval: ";
	cin >> SaveInterval;
	cout << "Console Logging Epoch Interval: ";
	cin >> ConsoleLoggingInterval;
	cout << endl;

}

void ImportANNFile( void )
{
	if( ANNFileName == "")
	{	cout << "Error - No ANN Input File is known. Exiting...";	exit;	}

	ifstream inStream;			//create new ifstream
	string line;
	char buffer[2048];

	try
	{
		inStream.open(ANNFileName.c_str());

		//Read in countInputNeurons and verify
		inStream.getline(buffer,2048);	//Buffer
		line = buffer;
		if( atoi(line.c_str()) != countInputNeurons )
		{	cout << "Error - Invalid ANN File. Input Neuron Count does not match! Exiting...";	exit;	}

		//Read in countHiddenNeurons and verify
		inStream.getline(buffer,2048);	//Buffer
		line = buffer;
		countHiddenNeurons = atoi(line.c_str());

		//Read in countHiddenLayers and verify
		inStream.getline(buffer,2048);	//Buffer
		line = buffer;
		countHiddenLayers = atoi(line.c_str());

		//Read in countOutputNeurons and verify
		inStream.getline(buffer,2048);	//Buffer
		line = buffer;
		if( atoi(line.c_str()) != countOutputNeurons )
		{	cout << "Error - Invalid ANN File. Output Neuron Count does not match! Exiting...";	exit;	}

		//Read in count Total Weights (not used in this program, but used in others)
		inStream.getline(buffer,2048);

		//We now know the full network architecture, now we can initalize the arrays...
		cout << " Initializing Arrays...";
		InitializeArrays();
		cout << " Success! (Arrays)";


	//	'Read in the Input-Hidden weights and Biases
	//	For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
	//		For inputNeuron As Integer = 0 To countInputNeurons - 1
	//			ihWeights(inputNeuron, hiddenNeuron, CURRENT) = Convert.ToDouble(sr.ReadLine)
	//		Next
	//		hiddenValues(hiddenNeuron, 0, BIAS) = Convert.ToDouble(sr.ReadLine)
	//	Next
	//	'Below is the code used to write out the I-H Weights
	//	'For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
	//	'    For inputNeuron As Integer = 0 To countInputNeurons - 1
	//	'        sw.WriteLine(ihWeights(inputNeuron, hiddenNeuron, CURRENT))
	//	'    Next
	//	'    sw.WriteLine(hiddenValues(hiddenNeuron, 0, BIAS))
	//	'Next
		//Read in the Input-Hidden Weights and Biases
		for( int hiddenNeuron=0; hiddenNeuron < countHiddenNeurons; hiddenNeuron++ )
		{
			for( int inputNeuron=0; inputNeuron < countInputNeurons; inputNeuron++ )
			{
				inStream.getline(buffer,2048);	//Buffer
				line = buffer;
				ihWeights[inputNeuron][hiddenNeuron] = atof(line.c_str());
			}//for
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			hiddenValues[0][hiddenNeuron][BIAS] = atof(line.c_str());
		}//for


	//	If countHiddenLayers > 1 Then
	//		For layer As Integer = 0 To countHiddenLayers - 2
	//			For HiddenAbove As Integer = 0 To countHiddenNeurons - 1
	//				For HiddenBelow As Integer = 0 To countHiddenNeurons - 1
	//					hhWeights(HiddenBelow, HiddenAbove, layer, CURRENT) = Convert.ToDouble(sr.ReadLine)
	//				Next
	//				hiddenValues(HiddenAbove, layer + 1, BIAS) = Convert.ToDouble(sr.ReadLine)
	//			Next
	//		Next
	//	End If
	//	'Below is the code used to write the H-H weights and biases
	//	'If countHiddenLayers > 1 Then
	//	'    For layer As Integer = 0 To countHiddenLayers - 2
	//	'        For HiddenAbove As Integer = 0 To countHiddenNeurons - 1
	//	'            For HiddenBelow As Integer = 0 To countHiddenNeurons - 1
	//	'                sw.WriteLine(hhWeights(HiddenBelow, HiddenAbove, layer, CURRENT))
	//	'            Next
	//	'            sw.WriteLine(hiddenValues(HiddenAbove, layer + 1, BIAS))
	//	'        Next
	//	'    Next
	//	'End If
		//Read in the Hidden-Hidden Weights and Bias
		if( countHiddenLayers > 1)
		{
			for( int layer=0; layer < countHiddenLayers-1; layer++ )
			{
				for( int hiddenAbove=0; hiddenAbove<countHiddenNeurons; hiddenAbove++ )
				{
					for( int hiddenBelow=0; hiddenBelow<countHiddenNeurons; hiddenBelow++ )
					{
						inStream.getline(buffer,2048);	//Buffer
						line = buffer;
						hhWeights[layer][hiddenBelow][hiddenAbove] = atof(line.c_str());
					}//for
					inStream.getline(buffer,2048);	//Buffer
					line = buffer;
					hiddenValues[layer+1][hiddenAbove][BIAS] = atof(line.c_str());
				}//for
			}//for
		}//if



	//	For outputNeuron As Integer = 0 To countOutputNeurons - 1
	//		For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
	//			hoWeights(hiddenNeuron, outputNeuron, CURRENT) = Convert.ToDouble(sr.ReadLine)
	//		Next
	//		outputValues(outputNeuron, BIAS) = Convert.ToDouble(sr.ReadLine)
	//	Next
	//	'Below is the code used to write out the H-O weights and biases
	//	'For outputNeuron As Integer = 0 To countOutputNeurons - 1
	//	'    For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
	//	'        sw.WriteLine(hoWeights(hiddenNeuron, outputNeuron, CURRENT))
	//	'    Next
	//	'    sw.WriteLine(outputValues(outputNeuron, BIAS))
	//	'Next
		for( int outputNeuron=0; outputNeuron<countOutputNeurons; outputNeuron++ )
		{
			for( int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons; hiddenNeuron++ )
			{
				inStream.getline(buffer,2048);	//Buffer
				line = buffer;
				hoWeights[hiddenNeuron][outputNeuron] = atof(line.c_str());
			}//for
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			outputValues[outputNeuron][BIAS] = atof(line.c_str());
		}//for

	}
	catch(...)
	{
		cout << "Error Reading ANN File!";
	}

	inStream.close();

}

int FindNearestPowerOf2( int value )
{
	int size = 2;
	while( size < value )
		size *= 2;

	return size;
}
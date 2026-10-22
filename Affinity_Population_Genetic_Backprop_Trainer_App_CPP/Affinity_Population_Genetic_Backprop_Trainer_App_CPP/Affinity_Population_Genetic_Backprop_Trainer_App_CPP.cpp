/*
TO DO IDEAS:
	- Create a Population helper class to do things like import the file, tokenize the file, etc.? Rather than
		having that code in the main console app? (which is poor design admittedly...)
	- When code testable, test if memory use for size of population using vectors is exponential or linear.
		Meaning when the vector size is increased, it seems to destroy the object and re-create it. Is that memory
		deallocated when it is destroyed? Or re-used? Or what? Investigate different sizes of Populations.
	- Try vector and array versions of the Backprop app.
	- If Memory leak remains, try keeping the Population objects the same, but simply try re-setting all of the weight values
		upon a new Generation, rather than destroying and re-creating new objects every time. Is not great code design, but
		if memory leak remains and this is a valid work-around to get closer to Production mode, GO FOR IT!
*/



/*------------------------------------------------------------------------
	Program :           Affinity Project - Main Console App - Affinity Population Genetic Backprop Trainer App CPP
	Written by:         Paul Killian
	Created:            2020
	Description:        Contains all the needed logic to create Populations of Backprop objects based off an APOP file
	Comment:			
 -------------------------------------------------------------------------*/

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
#include "Affinity_Backprop.h"
#include <tchar.h>

using namespace std;


//NEW VARIABLES ADDED FOR AFFINITY BACKPROP
int NewPopGenerationMethod = 1; //1 is default
string DataSetFile;
string AffinityBackpropParms[11];
int PopulationSize = 100; //100 is default
int Generations = 100;  //100 is default
int EpochsPerGeneration = 100; //100 is default
int GenSaveInterval = 100; //100 is default
bool VerboseLogging = false; //false is default
bool AutoTopology = false;  //false is default

int exemplarCount;
int countInputNeurons;
int countOutputNeurons;

vector< vector<double> > exemplarInputs;
vector< vector<double> > exemplarOutputs;

string FileTitle;

//PROTO FUNCTIONS BELOW
AffinityBackprop CreateAffinityBackprop(string pFilePath);
void ImportAPOPFile(string pFilePath);
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
void ImportDataSet(string filepath);
int numRowsInTextFile(string filepath);

//HANDLE  hConsole; //Color Console
//enum { Blank, DarkBlue, DarkGreen, DarkLightBlue, DarkRed, Purple, DarkYellow, White, Grey, Blue, Green, LightBlue, Red, Pink, Yellow, BrightWhite };	//Color Console
//
///////////////////////////////////////////////////////////////////////////////////////
////DECLARE VARIABLES
//
////CONST VALUES
//int static const VALUE = 0;
//int static const BIAS = 1;
//int static const DELTA = 2;
//
////Exemplar Information
//string DataSetFile;
//int countExemplarSets;
//long double** InputExemplars;
//long double** OutputExemplars;
//
////Architecture
//string ANNFileName;
//int countInputNeurons;
//int countHiddenNeurons;
//int countHiddenLayers;
//int countOutputNeurons;
//FLOAT128 test;
////Neuron Arrays
//long double* inputValues;
//long double*** hiddenValues;
//long double** outputValues;
//
////Weights Arrays
//long double** ihWeights;
//long double*** hhWeights;
//long double** hoWeights;
//
////Training Configuration
//int EpochsPerInterval;
//int Intervals;
//long double MaxWeightValue;
//long double MaxBiasValue;
//long double Rho;
//long double RhoMultiplier;
//int RhoInterval;
//bool Momentum;
//long double MomentumScaler;
//int SimAnnealMaxPercent;
//int SimAnnealInterval;
//
////Misc Variables
//string FileTitle;
//int SaveInterval;
//int ConsoleLoggingInterval;
//int* RandomArray;
//time_t TimeIntervalStart;
//time_t TimeIntervalEnd;
//time_t TimeTrainingStart;
//time_t TimeTrainingEnd;
//long double PreviousFitness;
//long double CurrentFitness;
//int const LargeSetPrecisionValue = 10;
//int const SmallSetPrecisionValue = 5;
//string CurrentDirectory;
//string InputFile;
//vector<string> AppPreviousEntries(10);
//int countPreviousEntries;
//string TrainingAverages;
//
//// END VARIABLE DECLARATIONS
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
//// Begin Method PROTOS
//
//void ImportBPROP2File(string filepath);
//void RandomizeTheIndex(void);
//void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
//void InitializeArrays(void);
//void RandomizeWeightAndBiasArrays();
//void ImportExemplarSet(void);
//void InitializeArrays(void);
//int numRowsInTextFile(string filepath);
//long double RandomWeight(long double maxValue);
//int RandomNumber(int MaxNumber);
//long double AbsVal(long double value);
//long double CalcFitness(void);
//void OutputBPROP2File(string filename, string FileTitle);
//void OutputBPROPFile(string filename);
//void OutputNetwork(string fileName);
//void OutputNetworkOutput(string fileName);
//void SaveData(void);
//void DeconstructCurrentDirectory(string filepath);
//string GenerateSaveName(void);
//void SelectInputFile(void);
//void LoadBPROP2File(void);
//void LoadBPROPFile(void);
//void ImportANNFile(void);
//int FindNearestPowerOf2(int value);


// END METHOD PROTOS
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// MAIN PROGRAM BEGINS HERE
int _tmain(int argc, _TCHAR* argv[])
{
	string importAPOPFile = "C:\\_KillianIntelligentAnalytics\\Affinity\\Train - Fractions\\Fractions.APOP";
	AffinityBackprop newBackprop = CreateAffinityBackprop(importAPOPFile);
	AffinityBackprop newBackprop2 = CreateAffinityBackprop(importAPOPFile);

	AffinityBackprop Population[100];

	for (int i = 0; i < 100; i++)
	{
		if(i == 99)
			cout << "wtf?!?!?!?!";

		AffinityBackprop newBackprop3 = CreateAffinityBackprop(importAPOPFile);

		Population[i] = CreateAffinityBackprop(importAPOPFile);
	}

	cout << "Backprops successfully created!";

#pragma region Old Code For Reference Below....
	/*{ Old Code Reference Below.....

	//////////////Backprop2 NewPop2 = Backprop2();

	//Backprop NewPop3[100];
	//int a = 1;
	//////////for (int i = 0; i < 100; i++)
	//////////{
	//////////	NewPop3[i].InitFuckingLargeArrays();
	//////////}

	//////////cout << "Done fucking around with damn objects taking up memory! Pitter Patter!" << endl;;

	////Backprop a = Backprop();

	//Population Pop = Population("C:\\_KillianIntelligentAnalytics\\Affinity\\Train - Fractions\\Fractions.APOP");

	//cout << "Now deallocate the Population!";

	//Pop.~Population();
	//delete &Pop.Pop[0];
	//delete &Pop;
	//

	//Pop.~Population();
	////Pop.RunGenerations();

	//cout << "Was memory deallocated????";

	//Pop.DeleteShit();

	///////////////////////////////////////////////////////////////////////
	//// BEGIN WINDOWS API CALLS TO MODIFY CONSOLE SCREEN
	//////Begin Console Color Prep
	//hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Color Console
	//////Console Buffer Mod
	//CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	//COORD NewSBSize;
	//GetConsoleScreenBufferInfo(hConsole, &SBInfo);
	//NewSBSize.X = 300;
	//NewSBSize.Y = 500;
	//SetConsoleScreenBufferSize(hConsole, NewSBSize);
	///////Console Window Size Mod
	//SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
	//int Right = 120;
	//int Bottom = 30;
	//COORD NewSBSize2;
	//NewSBSize = GetLargestConsoleWindowSize(hConsole);
	//if (NewSBSize.X <= Right)	//If size is too big for screen
	//	Right = NewSBSize.X - 10;	//Use max screen size - 10
	//DisplayArea.Right = Right;
	//DisplayArea.Bottom = Bottom;
	//SetConsoleWindowInfo(hConsole, TRUE, &DisplayArea);
	//// END WINDOWS API CALLS TO MODIFY CONSOLE
	///////////////////////////////////////////////////////////////////////////


	//SetConsoleTextAttribute(hConsole, Red);	//Color Console

	//cout << "Backprop Process App CPP - Affinity Project (2020)" << endl;
	//cout << "Created by: Paul Killian";
	//cout << endl << endl;

	//SetConsoleTextAttribute(hConsole, White);	//Color Console
	//SelectInputFile();

	//if (InputFile.find(string(".BPROP2")) != -1)
	//	LoadBPROP2File();
	//else if (InputFile.find(string(".BPROP")) != -1)
	//	LoadBPROPFile();
	//else
	//{
	//	cout << "Invalid File Inputted... Exiting";	exit;
	//}

	//TimeTrainingStart = time(0);
	//for (int Interval = 0; Interval < Intervals; Interval++)
	//{
	//	TimeIntervalStart = time(0);
	//	if (Interval == 0)
	//		PreviousFitness = CalcFitness();
	//	else
	//		PreviousFitness = CurrentFitness;

	//	//Begin Sim Anneal Logic
	//	if ((Interval + 1) % SimAnnealInterval == 0)
	//	{
	//		long double Fit1, Fit2;
	//		Fit1 = CalcFitness();
	//		SetConsoleTextAttribute(hConsole, White);	//Color Console
	//		cout << "Simulated Annealing Initiated...	Rho: " << Rho << ", MaxPer: " << SimAnnealMaxPercent << "%,	Delta Variance: (+/-)" << SimAnnealMaxPercent * Rho * 100 << "% of Prev Weight" << endl;
	//		//Calculate Input-Hidden Sim Annealed Weights
	//		for (int hiddenNeuron = 0; hiddenNeuron < countHiddenNeurons; hiddenNeuron++)
	//			for (int outputNeuron = 0; outputNeuron < countOutputNeurons; outputNeuron++)
	//				hoWeights[hiddenNeuron][outputNeuron] = hoWeights[hiddenNeuron][outputNeuron] + (RandomWeight(1) * (SimAnnealMaxPercent * hoWeights[hiddenNeuron][outputNeuron]) * (Rho * 100));

	//		//Calculate Hidden-Hidden Sim Annealed Weights
	//		if (countHiddenLayers > 1)
	//			for (int layer = countHiddenLayers - 2; layer >= 0; layer--)
	//				for (int belowHidden = 0; belowHidden < countHiddenNeurons; belowHidden++)
	//					for (int aboveHidden = 0; aboveHidden < countHiddenNeurons; aboveHidden++)
	//						hhWeights[layer][belowHidden][aboveHidden] = hhWeights[layer][belowHidden][aboveHidden] + (RandomWeight(1) * (SimAnnealMaxPercent * hhWeights[layer][belowHidden][aboveHidden]) * (Rho * 100));


	//		//Calculate Input-Hidden Sim Annealed Weights
	//		for (int inputNeuron = 0; inputNeuron < countInputNeurons; inputNeuron++)
	//			for (int hiddenNeuron = 0; hiddenNeuron < countHiddenNeurons; hiddenNeuron++)
	//				ihWeights[inputNeuron][hiddenNeuron] = ihWeights[inputNeuron][hiddenNeuron] + (RandomWeight(1) * (SimAnnealMaxPercent * ihWeights[inputNeuron][hiddenNeuron]) * (Rho * 100));
	//		Fit2 = CalcFitness();
	//		cout << "Sim Anneal Analytics - Prev Fit: " << Fit1 << ", After Fit: " << Fit2 << endl;
	//	}//if (Sim Anneal)

	//	SetConsoleTextAttribute(hConsole, Grey);	//Color Console
	//	for (int Epoch = 0; Epoch < EpochsPerInterval; Epoch++)
	//	{
	//		//Dynamic Rho Logic
	//		if ((Epoch + 1) % RhoInterval == 0)
	//			Rho *= RhoMultiplier;

	//		//Verbose Console Logging Logic
	//		if ((Epoch + 1) % ConsoleLoggingInterval == 0)
	//			cout << ".";

	//		RandomizeTheIndex();
	//		for (int ExemplarIndex = 0; ExemplarIndex < countExemplarSets; ExemplarIndex++)
	//		{
	//			////////////////////////////////////////////////////////////////////////////
	//			//Run The Network
	//			////////////////////////////////////////////////////////////////////////////

	//			//Set Input Neurons
	//			for (int i = 0; i <= countInputNeurons - 1; i++)
	//				inputValues[i] = InputExemplars[RandomArray[ExemplarIndex]][i];

	//			//Calculate First Layer of Hidden Neuron Values
	//			for (int hiddenNeuron = 0; hiddenNeuron < countHiddenNeurons; hiddenNeuron++)
	//			{
	//				hiddenValues[0][hiddenNeuron][VALUE] = 0; //Reset to 0
	//				for (int inputNeuron = 0; inputNeuron < countInputNeurons; inputNeuron++)
	//					hiddenValues[0][hiddenNeuron][VALUE] += inputValues[inputNeuron] * ihWeights[inputNeuron][hiddenNeuron];

	//				//hiddenValues[0][hiddenNeuron][VALUE] = Activation(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]);
	//				hiddenValues[0][hiddenNeuron][VALUE] = (1 / (1 + pow(2.71828183, -(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]))));
	//			}

	//			//Calculate Hidden Layer Neuron Values
	//			if (countHiddenLayers > 1)
	//			{
	//				for (int layer = 1; layer < countHiddenLayers; layer++)
	//				{
	//					for (int aboveHidden = 0; aboveHidden < countHiddenNeurons; aboveHidden++)
	//					{
	//						hiddenValues[layer][aboveHidden][VALUE] = 0;
	//						for (int belowHidden = 0; belowHidden < countHiddenNeurons; belowHidden++)
	//						{
	//							hiddenValues[layer][aboveHidden][VALUE] += hiddenValues[layer - 1][belowHidden][VALUE] * hhWeights[layer - 1][belowHidden][aboveHidden];
	//						}
	//						//hiddenValues[layer][aboveHidden][VALUE] = Activation(hiddenValues[layer][aboveHidden][VALUE] +hiddenValues[layer][aboveHidden][BIAS]);
	//						hiddenValues[layer][aboveHidden][VALUE] = (1 / (1 + pow(2.71828183, -(hiddenValues[layer][aboveHidden][VALUE] + hiddenValues[layer][aboveHidden][BIAS]))));
	//					}
	//				}
	//			}

	//			//Calculate Output Neuron Values
	//			for (int outputNeuron = 0; outputNeuron < countOutputNeurons; outputNeuron++)
	//			{
	//				outputValues[outputNeuron][VALUE] = 0;	//Reset to 0
	//				for (int hiddenNeuron = 0; hiddenNeuron < countHiddenNeurons; hiddenNeuron++)
	//				{
	//					outputValues[outputNeuron][VALUE] += hiddenValues[countHiddenLayers - 1][hiddenNeuron][VALUE] * hoWeights[hiddenNeuron][outputNeuron];
	//				}
	//				//We are not squashing the output neurons, so simply add the bias to it
	//				outputValues[outputNeuron][VALUE] += outputValues[outputNeuron][BIAS];
	//			}


	//			//////////////////////////////////////////////////////////////////////////////////////////////////
	//			//////////////////////////////////////////////////////////////////////////////////////////////////
	//			// THE ANN IS NOW CALCULATED, NOW BEGIN TRAINING THE NETWORK FOR THIS EXEMPLAR SET				//
	//			//////////////////////////////////////////////////////////////////////////////////////////////////
	//			//////////////////////////////////////////////////////////////////////////////////////////////////


	//			//////////////////////////////////////////////////////////////////////////////////////////////////
	//			// Run the Backprop Training Algorithm															//
	//			//////////////////////////////////////////////////////////////////////////////////////////////////

	//			//////////////////////////////
	//			// CALCULATE DELTAS			//
	//			//////////////////////////////

	//			//Calculate Output Neuron Deltas
	//			for (int i = 0; i < countOutputNeurons; i++)
	//				outputValues[i][DELTA] = -2 * (OutputExemplars[RandomArray[ExemplarIndex]][i] - outputValues[i][VALUE]);

	//			//Calculate Top Layer Hidden Neuron Deltas
	//			//long double totalDeltas = 0	//May use later, but leave commented out for now...
	//			for (int hiddenNeuron = 0; hiddenNeuron < countHiddenNeurons; hiddenNeuron++)
	//			{
	//				hiddenValues[countHiddenLayers - 1][hiddenNeuron][DELTA] = 0;
	//				for (int outputNeuron = 0; outputNeuron < countOutputNeurons; outputNeuron++)
	//				{
	//					hiddenValues[countHiddenLayers - 1][hiddenNeuron][DELTA] += ((hiddenValues[countHiddenLayers - 1][hiddenNeuron][VALUE]) * (1 - (hiddenValues[countHiddenLayers - 1][hiddenNeuron][VALUE]))) * outputValues[outputNeuron][DELTA] * hoWeights[hiddenNeuron][outputNeuron];
	//					//totalDeltas += hiddenValues[countHiddenLayers-1][hiddenNeuron][DELTA] //Keep a running total of delta values for analysis
	//				}
	//			}

	//			//Calculate Hidden Layer Neuron Deltas
	//			if (countHiddenLayers > 1)
	//			{
	//				for (int layer = countHiddenLayers - 2; layer >= 0; layer--)
	//				{
	//					for (int belowHidden = 0; belowHidden < countHiddenNeurons; belowHidden++)
	//					{
	//						hiddenValues[layer][belowHidden][DELTA] = 0;	//Reset value
	//						for (int aboveHidden = 0; aboveHidden < countHiddenNeurons; aboveHidden++)
	//						{
	//							//hiddenValues[layer][belowHidden][DELTA] += fPrime(hiddenValues[layer][belowHidden][VALUE]) * hiddenValues[layer+1][aboveHidden][DELTA] * hhWeights[layer][belowHidden][aboveHidden][CURRENT];
	//							hiddenValues[layer][belowHidden][DELTA] += ((hiddenValues[layer][belowHidden][VALUE]) * (1 - (hiddenValues[layer][belowHidden][VALUE]))) * hiddenValues[layer + 1][aboveHidden][DELTA] * hhWeights[layer][belowHidden][aboveHidden];
	//							//totalDeltas += hiddenValues[layer][belowHidden][DELTA];	//Keep a running total of delta values for analysis
	//						}
	//					}
	//				}
	//			}

	//			//////////////////////////////////////
	//			// CALCULATE NEW WEIGHTS			//
	//			//////////////////////////////////////

	//			//Calculate New Weights for hoWeights
	//			for (int hiddenNeuron = 0; hiddenNeuron < countHiddenNeurons; hiddenNeuron++)
	//				for (int outputNeuron = 0; outputNeuron < countOutputNeurons; outputNeuron++)
	//					hoWeights[hiddenNeuron][outputNeuron] = hoWeights[hiddenNeuron][outputNeuron] - Rho * outputValues[outputNeuron][DELTA] * hiddenValues[countHiddenLayers - 1][hiddenNeuron][VALUE];

	//			//Calculate New Hidden-Hidden Weights
	//			if (countHiddenLayers > 1)
	//				for (int layer = countHiddenLayers - 2; layer >= 0; layer--)
	//					for (int belowHidden = 0; belowHidden < countHiddenNeurons; belowHidden++)
	//						for (int aboveHidden = 0; aboveHidden < countHiddenNeurons; aboveHidden++)
	//							hhWeights[layer][belowHidden][aboveHidden] = hhWeights[layer][belowHidden][aboveHidden] - Rho * hiddenValues[layer + 1][aboveHidden][DELTA] * hiddenValues[layer][belowHidden][VALUE];

	//			//Calculate Input-Hidden New Weights
	//			for (int inputNeuron = 0; inputNeuron < countInputNeurons; inputNeuron++)
	//				for (int hiddenNeuron = 0; hiddenNeuron < countHiddenNeurons; hiddenNeuron++)
	//					ihWeights[inputNeuron][hiddenNeuron] = ihWeights[inputNeuron][hiddenNeuron] - Rho * hiddenValues[0][hiddenNeuron][DELTA] * inputValues[inputNeuron];

	//		}//for(ExemplarIndex)
	//	}//for(Epoch)
	//	TimeIntervalEnd = time(0);

	//	cout << " - Int: " << Interval << ", " << EpochsPerInterval << " Epochs: " << difftime(TimeIntervalEnd, TimeIntervalStart) << " s, " << countExemplarSets << " Ex Sets, Rho: " << Rho << ", Mom: " << Momentum << ", Arch: " << countInputNeurons << "-" << countHiddenNeurons << "-" << countHiddenLayers << "-" << countOutputNeurons << endl;

	//	CurrentFitness = CalcFitness();

	//	if (CurrentFitness - PreviousFitness < 0)
	//		SetConsoleTextAttribute(hConsole, Green);	//Color Console
	//	else if (CurrentFitness - PreviousFitness > 0)
	//		SetConsoleTextAttribute(hConsole, Red);	//Color Console
	//	else
	//		SetConsoleTextAttribute(hConsole, White);	//Color Console

	//	cout << "Fitness: " << CurrentFitness << "	PrvFitness: " << PreviousFitness << "	Diff: " << CurrentFitness - PreviousFitness << "	Avg: " << CurrentFitness / countExemplarSets << endl;

	//	TrainingAverages = TrainingAverages + to_string(CurrentFitness / countExemplarSets) + ",";

	//	if ((Interval + 1) % SaveInterval == 0)
	//		SaveData();

	//}//for(Interval)
	//TimeTrainingEnd = time(0);
	//SetConsoleTextAttribute(hConsole, White);	//Color Console
	//cout << "Training Complete in " << difftime(TimeTrainingEnd, TimeTrainingStart) << "seconds." << endl;
	//SaveData();


	////TrainingAverages = TrainingAverages + "Hello You Stupid World!";
	////cout << TrainingAverages;
	//cout << "Saving Training Averages to File.";
	//std::ofstream os(CurrentDirectory + FileTitle + "_TrainingAverages.csv");
	//os << TrainingAverages;
	//if (!os)
	//{
	//	std::cerr << "Error writing to ..." << std::endl;
	//	cout << "Error writing to file " + CurrentDirectory + FileTitle + "_TrainingAverages.csv";
	//}
	//else
	//{
	//	os << TrainingAverages;
	//	cout << "Successfully wrote to file " + CurrentDirectory + FileTitle + "_TrainingAverages.csv";
	//	cout << "values: " + TrainingAverages;
	//}
	//os.close();
	//// Note: file is automatically closed, when current scope is left 
#pragma endregion Old Code For Reference Below....
//} */

	cout << endl << "Please Press Enter to Exit...";
	cin.ignore();
	return 0;
}
// MAIN PROGRAM ENDS HERE
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// METHODS FOR MAIN PROGRAM BEGIN HERE
AffinityBackprop CreateAffinityBackprop(string pFilePath)
{
	try
	{

		// Verify file is an APOP file
		size_t found = pFilePath.find(".APOP");
		if (found == string::npos)
			throw new exception("Invalid import file selected. Import file must be an .APOP file!");

		ImportAPOPFile(pFilePath);

		//ImportDataSet will import the dataset file to variables exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, and countOutputNeurons
		ImportDataSet(DataSetFile);

		//exemplarInputs = pExemplarInputs;
		//exemplarOutputs = pExemplarOutputs;
		//VerifyArchitecture();
		//exemplarCount = pExemplarCount;
		//countInputNeurons = pCountInputNeurons;
		//countOutputNeurons = pCountOutputNeurons;

		bool UseMomentum = false; //default value is false

		for (auto& c : AffinityBackpropParms[8]) c = toupper(c);
		if (AffinityBackpropParms[8] == "TRUE" || AffinityBackpropParms[8] == "T")
			UseMomentum = true;
		else
			UseMomentum = false;

		//TO DO NEXT:    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//NEED TO CONVERT STRING PARMS TO PROPER DATA TYPE PER PARAMETER TYPE EXPEXCTED.
		//ALSO NEED TO LOOK INTO PARMS 11 TO 15 ARE NEEDED AND CAN BE INFERRED FROM THE DATA FILE AND NOT NEED TO BE PARAMETERS AT ALL
		AffinityBackprop newAffinityBackprop = AffinityBackprop(DataSetFile, //string - DataFileName
																atoi(AffinityBackpropParms[1].c_str()), //int - CountHiddenNeurons
																atoi(AffinityBackpropParms[2].c_str()), //int - CountHiddenLayers
																atof(AffinityBackpropParms[3].c_str()), //double - MaxWeightValue
																stod(AffinityBackpropParms[4].c_str()), //double - MaxBiasValue
																stod(AffinityBackpropParms[5].c_str()), //double - Rho
																stod(AffinityBackpropParms[6].c_str()), //double - RhoMultiplier
																atoi(AffinityBackpropParms[7].c_str()), //int - RhoInterval
																UseMomentum, //bool - UseMomentum
																atof(AffinityBackpropParms[9].c_str()), //double - MomentumScaler
																atoi(AffinityBackpropParms[10].c_str()), //int - AnnealingMaxPercent
																exemplarInputs, //vector<vector<double>> ExemplarInputs
																exemplarOutputs, //vector<vector<double>> ExemplarOutputs
																exemplarCount, //int ExemplarCount
																countInputNeurons, //int CountInputNeurons
																countOutputNeurons); //int CountOutputNeurons


		//InitializeArrays
		//InitializeArrays();

		//RandomizeWeightAndBiasArrays()
		//RandomizeWeightAndBiasArrays();


	}
	catch (exception e)
	{
		cout << "Exception Occurred Creating Affinity Backprop Object! - " << e.what();
	}

	//        Dim line As String
	//        Dim readLine() As String
	//        Dim sr As StreamReader
	//
	//        Try
	//            sr = File.OpenText(filepath)
	//
	//            sr.ReadLine()   'Buffer
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            ID = readLine(1)  'Dats Set File Location
	//            'ID = sr.ReadLine
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            FileTitle = readLine(1)  'Dats Set File Location
	//            'FileTitle = sr.ReadLine
	//
	//
	//            sr.ReadLine()   'Buffer
	//            sr.ReadLine()   'Buffer
	//
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            DataFileName = readLine(1)  'Dats Set File Location
	//            'DataFileName = sr.ReadLine
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            ANNFileName = readLine(1)  'Dats Set File Location
	//            'ANNFileName = sr.ReadLine
	//
	//
	//            sr.ReadLine()   'Buffer
	//            sr.ReadLine()   'Buffer
	//
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            rho = Convert.ToDouble(readLine(1))  'Dats Set File Location
	//            'rho = Convert.ToDouble(sr.ReadLine)
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            rhoMultiplier = Convert.ToDouble(readLine(1))  'Dats Set File Location
	//            'rhoMultiplier = Convert.ToDouble(sr.ReadLine)
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            rhoInterval = Convert.ToInt32(readLine(1))  'Dats Set File Location
	//            'rhoInterval = Convert.ToInt32(sr.ReadLine)
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            UseMomentum = Convert.ToBoolean(readLine(1))  'Dats Set File Location
	//            'UseMomentum = Convert.ToBoolean(sr.ReadLine)
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            momentumScaler = Convert.ToDouble(readLine(1))  'Dats Set File Location
	//            'momentumScaler = Convert.ToDouble(sr.ReadLine)
	//
	//            line = sr.ReadLine
	//            readLine = line.Split("="c)
	//            If readLine.Count <> 2 Then
	//                Throw New Exception("Error - ANN_Fast_AffinityBackprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
	//                Exit Sub
	//            End If
	//            annealingMaxPercent = Convert.ToInt32(readLine(1))  'Dats Set File Location
	//            'annealingMaxPercent = Convert.ToInt32(sr.ReadLine)
	//        Catch ex As Exception
	//            Throw ex
	//        End Try
	//    End Sub
	//

}//ImportABPROPFile

void ImportAPOPFile(string pFilePath)
{
	ifstream inStream;			//create new ifstream
	string line;
	vector<string> tokens;
	char buffer[2048];
	vector<string> AffBackpropParms;

	try
	{

		// Verify file is an APOP file
		size_t found = pFilePath.find(".APOP");
		if (found == string::npos)
			throw new exception("Invalid import file selected. Import file must be an .POP file!");



		inStream.open(pFilePath.c_str());

		inStream.getline(buffer, 2048);	//Buffer
		line = buffer;

		inStream.getline(buffer, 2048);	//Buffer
		line = buffer;
		Tokenize(line, tokens, "=");
		DataSetFile = tokens[1];
		AffinityBackpropParms[0] = DataSetFile;
		tokens.clear();

		inStream.getline(buffer, 2048);	//Buffer
		inStream.getline(buffer, 2048);	//Buffer

		inStream.getline(buffer, 2048);	//Buffer
		line = buffer;
		Tokenize(line, tokens, "=");
		PopulationSize = atoi(tokens[1].c_str());
		tokens.clear();

		inStream.getline(buffer, 2048);	//Buffer
		line = buffer;
		Tokenize(line, tokens, "=");
		Generations = atoi(tokens[1].c_str());
		tokens.clear();

		inStream.getline(buffer, 2048);	//Buffer
		line = buffer;
		Tokenize(line, tokens, "=");
		EpochsPerGeneration = atoi(tokens[1].c_str());
		tokens.clear();

		inStream.getline(buffer, 2048);	//Buffer
		line = buffer;
		Tokenize(line, tokens, "=");
		NewPopGenerationMethod = atoi(tokens[1].c_str());
		tokens.clear();

		inStream.getline(buffer, 2048);	//Buffer
		inStream.getline(buffer, 2048);	//Buffer

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");	//AutoTopology
		for (auto& c : tokens[1]) c = toupper(c);
		if (tokens[1] == "TRUE" || tokens[1] == "T")
			AutoTopology = true;
		else
			AutoTopology = false;
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[1] = tokens[1];	//Hidden Neurons
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[2] = tokens[1];	//Hidden Layers
		tokens.clear();

		inStream.getline(buffer, 2048);	//Buffer
		inStream.getline(buffer, 2048);	//Buffer

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[3] = tokens[1];	//Max Weight Value
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[4] = tokens[1];
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[5] = tokens[1];
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[6] = tokens[1];
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[7] = tokens[1];
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[8] = tokens[1];
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[9] = tokens[1];
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		AffinityBackpropParms[10] = tokens[1];
		tokens.clear();

		inStream.getline(buffer, 2048);	//Buffer
		inStream.getline(buffer, 2048);	//Buffer

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		GenSaveInterval = atoi(tokens[1].c_str());
		tokens.clear();

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		FileTitle = tokens[1];
		tokens.clear();

		inStream.getline(buffer, 2048);	//Buffer
		inStream.getline(buffer, 2048);	//Buffer

		inStream.getline(buffer, 2048);
		line = buffer;
		Tokenize(line, tokens, "=");
		if (tokens[1] == "True" || tokens[1] == "T")
			VerboseLogging = true;
		else
			VerboseLogging = false;

		tokens.clear();

		//return AffinityBackpropParms;
	}
	catch (exception e)
	{
		cout << "Exception Occurred Importing APOP File! - " << e.what();
	}
}//ImportAPOPFile()

void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

void ImportDataSet(string filepath)
{
	string _METHODNAME = "ImportDataSet(filepath)";
	string line;
	vector<string> inputs;
	vector<string> sInputValues;
	vector<string> sOutputValues;
	exemplarCount = numRowsInTextFile(filepath);

	ifstream inStream;			//create new ifstream
	char buffer[16384];	//16384 should be enough for even a very large data row
	inStream.open(filepath.c_str());

	inStream.getline(buffer, 16384);	//Buffer
	line = buffer;
	Tokenize(line, inputs, "=");

	Tokenize(inputs[0], sInputValues, ",");
	Tokenize(inputs[1], sOutputValues, ",");

	countInputNeurons = sInputValues.size();
	countOutputNeurons = sOutputValues.size();

	for (int i = 0; i <= exemplarCount - 1; i++)
	{
		vector<double> RowInputs(countInputNeurons, 0);
		for (int k = 0; k <= countInputNeurons - 1; k++)
		{
			RowInputs[k] = atof(sInputValues[k].c_str());
		}

		vector<double> RowOutputs(countOutputNeurons, 0);
		for (int k = 0; k <= countOutputNeurons - 1; k++)
		{
			RowOutputs[k] = atof(sOutputValues[k].c_str());
		}

		//Add the Row Vectors to the Exemplar Vectors
		exemplarInputs.push_back(RowInputs);
		exemplarOutputs.push_back(RowOutputs);

		inStream.getline(buffer, 16384);	//Buffer
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
			Tokenize(inputs[1], sOutputValues, ",");
		}
	}
	inStream.close();

}//ImportDataSet

int numRowsInTextFile(string filepath)
{
	ifstream inStream;			//create new ifstream
	string line;
	int rows = 0;
	char buffer[16384];	//16384 should be enough for even a very large data row

	inStream.open(filepath.c_str());

	do
	{
		inStream.getline(buffer, 16384);	//Buffer
		line = buffer;
		if (line != "")
			rows += 1;
	} while (line != "");

	return rows;

}
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

 //Include Files below
#include "Affinity_Population.h"
#include "Affinity_Backprop.h"
#include <ctype.h>
#include <vector>
#include <windows.h>
#include <string>
#include <iomanip>
#include <math.h>


HANDLE  hConsole;
enum { Blank, DarkBlue, DarkGreen, DarkLightBlue, DarkRed, Purple, DarkYellow, White, Grey, Blue, Green, LightBlue, Red, Pink, Yellow, BrightWhite };


time_t Started;

Population::Population(string filepath)
{
	Started = time(0);	//Set Starting Time
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, White);
	string _METHODNAME = "New(filepath)";

	ImportAPopFile(filepath);

	DeconstructCurrentDirectory(filepath);

	ImportDataSet(DataSetFile);

	try
	{
		cout << PopulationSize << " Total: ";
		for (int i = 0; i < PopulationSize; i++)
		{
			Pop.push_back(AffinityBackprop(AffinityBackpropParms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons));
			Pop[i].ID = i;
			cout << i << " ";
		}

		//Set BestAffinityBackprop object to its initial value which will be over-written later
		BestAffinityBackprop = Pop[0].Copy();
		//BestAffinityBackprop.Fitness = numeric_limits<double>::max( );
		BestAffinityBackprop.Fitness = 1.79769e+308;
		cout << endl << "Population Created!" << endl;


		////Generation1 Copy
		//cout << "Copy Generation 1" << endl;
		//for (int i=0; i < PopulationSize; i++)
		//	Pop[i] = Pop[0].Copy;

		//cout << "Copy Generation 2" << endl;
		//for (int i = 0; i < PopulationSize; i++)
		//	Pop[i] = Pop[0].Copy;

		//cout << "How much memory is used when copying generations?..." << endl;



		//RunGenerations();









		//Population2[] = AffinityBackprop[PopulationSize];
		//Population2 = new AffinityBackprop[PopulationSize];
		//cout << PopulationSize << " Total: ";
		//for (int i = 0; i <= PopulationSize - 1; i++)
		//{
		//	cout << i << " ";
		//	//AffinityBackprop* newMember = new AffinityBackprop(AffinityBackpropParms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons);
		//	AffinityBackprop newMember = AffinityBackprop(AffinityBackpropParms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons);
		//	AffinityBackprop* newMember2 = new AffinityBackprop(AffinityBackpropParms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons);

		//	//Convert int to a string - Seems like lots of work for essentially a simple task
		//	string s;
		//	stringstream out;
		//	out << i;
		//	s = out.str();

		//	//newMember->ID = s;
		//	newMember.ID = s;
		//	//Pop[i] = *newMember;
		//	Population1[i] = newMember;
		//	Population2[i] = *newMember2;
		//}

		//Set BestAffinityBackprop object to its initial value which will be over-written later
		//BestAffinityBackprop = Pop[0].Copy();
		////BestAffinityBackprop.Fitness = numeric_limits<double>::max( );
		//BestAffinityBackprop.Fitness = 1.79769e+308;
		//cout << endl << "Population Created!" << endl;

	//	RunGenerations();

	}
	catch (exception e)
	{
		cout << "Exception in Constructor 'Population(string filepath)'! - " << e.what();
	}
}

Population::~Population(void)									//Destructor
{
	//Pop.clear();

	//for (int i = 0; i < Pop.size(); i++)
	//{
	//	//Pop[i].DeleteAffinityBackpropObject();
	//	delete Pop[i];
	//}

	//delete[] *Pop;

	//for (int i = 0; i < 6; i++)
	//{
	//	delete monsters[i];
	//}
}

void Population::DeleteShit(void)
{
	cout << "Delete all the shit....";
}

void Population::RunPopulation(int Epochs)
{
	double PreviousFitness;
	double CurrentFitness;
	for (int i = 0; i <= PopulationSize - 1; i++)
	{
		Pop[i].calcFitness();
		PreviousFitness = Pop[i].Fitness;
		Pop[i].Train(Epochs, VerboseLogging);
		Pop[i].calcFitness();
		CurrentFitness = Pop[i].Fitness;

		if (CurrentFitness < PreviousFitness)
			SetConsoleTextAttribute(hConsole, Green);
		else if (CurrentFitness > PreviousFitness)
			SetConsoleTextAttribute(hConsole, Red);
		else
			SetConsoleTextAttribute(hConsole, BrightWhite);

		cout << Pop[i].ID << ": " << CurrentFitness << "	PrvFit: " << PreviousFitness << "	Diff: " << CurrentFitness - PreviousFitness << "	Avg: " << CurrentFitness / Pop[i].exemplarCount << endl;
	}
}

void Population::RunGeneration(int EpochsToRun, int NewPopGenerationMethod, bool GenerateNewPopulation)
{
	RunPopulation(EpochsToRun);
	SetConsoleTextAttribute(hConsole, LightBlue);
	cout << "Sorting Population... " << endl;
	SortPopulation();
	cout << "Retaining Best AffinityBackprop...";
	RetainBestAffinityBackprop();
	if (GenerateNewPopulation)
	{
		NewPopulation(NewPopGenerationMethod);
	}
}//RunGeneration()

void Population::RunGenerations(void)
{
	cout << "Calculating Initial Average Population Fitness... (Disabled)" << endl;
	AvgGenerationFitness.push_back(AveragePopulationFitness());
	cout << "Initial Average Population Fitness Calculated! (Disabled)" << endl;

	for (int i = 0; i <= Generations - 1; i++)
	{
		SetConsoleTextAttribute(hConsole, Yellow);
		cout << "Generation: " << i << "	Best Rho: " << Pop[0].rho << "	BestAffinityBackprop: " << BestAffinityBackprop.Fitness << endl;
		SetConsoleTextAttribute(hConsole, White);
		if (i < Generations - 1)
			RunGeneration(EpochsPerGeneration, NewPopGenerationMethod, true);
		else
			RunGeneration(EpochsPerGeneration, NewPopGenerationMethod, false);

		BestAffinityBackpropFitness.push_back(BestAffinityBackprop.Fitness);
		AvgGenerationFitness.push_back(AveragePopulationFitness());
		SetConsoleTextAttribute(hConsole, White);
		//Calculating Avage Population Fitness --- NOT IMPLEMENTED YET

		if (i == Generations - 1)
			SavePopData(true);
		else if (i % GenSaveInterval == 0)
			SavePopData(false);
		//        Next
	}
}

void Population::SortPopulation(void)
{
	sortedPopulation = new AffinityBackprop[PopulationSize];
	sortedArray = new bool[PopulationSize];
	double bestFitnessValue;
	int bestFitnessIndex = -1;

	//Set entire sortedArray to false
	for (int i = 0; i <= PopulationSize - 1; i++)
		sortedArray[i] = false;

	for (int i = 0; i <= PopulationSize - 1; i++)
	{
		bestFitnessValue = 1.79769e+308;
		for (int j = 0; j <= PopulationSize - 1; j++)
		{
			if (Pop[j].Fitness < bestFitnessValue && !sortedArray[j])
			{
				bestFitnessValue = Pop[j].Fitness;
				bestFitnessIndex = j;
			}
		}
		sortedPopulation[i] = Pop[bestFitnessIndex];
		sortedArray[bestFitnessIndex] = true;
	}

	//Set each new populatin member in the population array from the sorted array
	for (int i = 0; i <= PopulationSize - 1; i++)
		Pop[i] = sortedPopulation[i];

}//sortPopulation

void Population::NewPopulation(int GenerationMethod)
{
	cout << "### NewPopulation Generation Currently Not Implemented - Entire Population Currently Cloned ###" << endl;;
}

void Population::ImportAPopFile(string filepath)
{
	ifstream inStream;			//create new ifstream
	string line;
	vector<string> tokens;
	char buffer[2048];

	try
	{

		// Verify file is an APOP file
		size_t found = filepath.find(".APOP");
		if (found == string::npos)
			throw new exception("Invalid import file selected. Import file must be an .APOP file!");



		inStream.open(filepath.c_str());

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

	}
	catch (exception e)
	{
		cout << "Exception Occurred Importing APOP File! - " << e.what();
	}
}

void Population::RetainBestAffinityBackprop(void)
{
	cout << " Calculating Current Pop Best Fitness... ";
	Pop[0].calcFitness();
	cout << " Calculating Previous Best AffinityBackprop Fitness...";
	BestAffinityBackprop.calcFitness();
	if (Pop[0].Fitness < BestAffinityBackprop.Fitness || BestAffinityBackprop.Fitness != BestAffinityBackprop.Fitness)
		//Note: Fitness != Fitness essentially checks if the value is not a number
	{
		BestAffinityBackprop = Pop[0].Copy();
		BestAffinityBackprop.ID = BestAffinityBackprop.ID + "-B";
		BestAffinityBackprop.calcFitness();
	}
	cout << "Best AffinityBackprop Retained!" << endl;
}

double Population::AveragePopulationFitness(void)
{
	return 1;
}

void Population::WritePopFile(string filepath)
{
	ofstream file;
	file.open(filepath.c_str());
	file.setf(ios::fixed);
	setprecision(10);
	try
	{
		file << "### Data Set ###" << endl;
		file << "Dataset file location=" << AffinityBackpropParms[0] << endl;
		file << endl;
		file << "### Population Configuration ###" << endl;
		file << "Population Size=" << PopulationSize << endl;
		file << "Generations=" << Generations << endl;
		file << "Epochs Per Generation=" << EpochsPerGeneration << endl;
		file << "New Pop Generation Method=" << NewPopGenerationMethod << endl;
		file << endl;
		file << "### Network Topology ###" << endl;
		file << "Hidden Neurons=" << AffinityBackpropParms[1] << endl;
		file << "Hidden Layers=" << AffinityBackpropParms[2] << endl;
		file << endl;
		file << "### Training Configuation ###" << endl;
		file << "Max weight value=" << AffinityBackpropParms[3] << endl;
		file << "Max bias value=" << AffinityBackpropParms[4] << endl;
		file << "Initial rho value=" << AffinityBackpropParms[5] << endl;
		file << "Rho multiplier=" << AffinityBackpropParms[6] << endl;
		file << "Dynamic rho interval=" << AffinityBackpropParms[7] << endl;
		file << "Use Momentum=" << AffinityBackpropParms[8] << endl;
		file << "Momentum Scaler=" << AffinityBackpropParms[9] << endl;
		file << "Simulated annealing max percent(%)=" << AffinityBackpropParms[10] << endl;
		file << endl;
		file << "### Save Data Settings ###" << endl;
		file << "Generation Save Interval=" << GenSaveInterval << endl;
		file << "File Title=" << FileTitle << endl;
		file << endl;
		file << "### Console Logging ###" << endl;
		file << "Verbose Logging=" << VerboseLogging << endl;
	}
	catch (...)
	{
		cout << "Exception Occurred!";
	}
	file.close();
}

void Population::OutputPopulationData(string filepath)
{
	time_t Finished = time(0);
	int SecondsElapsed = difftime(Finished, Started);

	ofstream file;
	file.open(filepath.c_str());
	file.setf(ios::fixed);
	setprecision(10);
	try
	{
		file << "Population_AffinityBackprop_App_CPP" << endl;
		file << endl;
		cout << "BestAffinityBackpropID: " << BestAffinityBackprop.ID << "	BestAffinityBackprop Fitness: " << BestAffinityBackprop.Fitness << endl;
		file << "BestAffinityBackpropID: " << BestAffinityBackprop.ID << "	BestAffinityBackprop Fitness: " << BestAffinityBackprop.Fitness << endl;
		file << endl;
		cout << "Initial Population Avg Fitness: " << AvgGenerationFitness[0] << endl;
		file << "Initial Population Avg Fitness: " << AvgGenerationFitness[0] << endl;
		for (int i = 0; i <= BestAffinityBackpropFitness.size() - 1; i++)
		{
			cout << "Best AffinityBackprop Fitness Generation " << i << ": " << BestAffinityBackpropFitness[i] << "	AvgFitness: " << AvgGenerationFitness[i + 1] << endl;
			file << "Best AffinityBackprop Fitness Generation " << i << ": " << BestAffinityBackpropFitness[i] << "	AvgFitness: " << AvgGenerationFitness[i + 1] << endl;
		}
		file << endl;
		cout << "Processed In: " << SecondsElapsed << " Seconds" << endl;
		file << "Processed In: " << SecondsElapsed << " Seconds" << endl;
	}
	catch (...)
	{
		cout << "Exception Occurred!";
	}
	file.close();
}

void Population::ImportDataSet(string filepath)
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

int Population::numRowsInTextFile(string filepath)
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

void Population::DeconstructCurrentDirectory(string filepath)
{
	vector<string> tokens;

	Tokenize(filepath, tokens, "\\");

	if (tokens.size() > 1)
	{
		for (int i = 0; i <= tokens.size() - 2; i++)
		{
			if (i == 0)
				CurrentDirectory = tokens[i] + "\\";
			else
				CurrentDirectory += tokens[i] + "\\";
		}
	}
	else
		CurrentDirectory = "";
}

string Population::GenerateSaveName(void)
{
	string AvgError;
	string Dt;
	BestAffinityBackprop.calcFitness();

	//      'Create Average Error String
	ostringstream oss;
	oss << (BestAffinityBackprop.Fitness / BestAffinityBackprop.exemplarCount);
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

void Population::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
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

void Population::SavePopData(bool OutputToConsole)
{
	SetConsoleTextAttribute(hConsole, White);
	cout << "Recalculating Best AffinityBackprop Fitness..." << endl;
	BestAffinityBackprop.calcFitness();
	cout << "Saving data to: " << CurrentDirectory << GenerateSaveName() << "\\" << endl;

	try
	{
		string command;
		command = "mkdir \"" + CurrentDirectory + GenerateSaveName() + "\"";
		system(command.c_str());

		SetConsoleTextAttribute(hConsole, White);
		if (OutputToConsole)
		{
			cout << "Saving Console Output... ";
			OutputPopulationData(CurrentDirectory + "\\ProcessOutput.txt");
		}

		cout << "Saving Network... ";
		BestAffinityBackprop.outputNetwork(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".ANN");
		cout << "Saving Network Output... ";
		BestAffinityBackprop.outputNetworkOutput(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".txt");
		cout << "Saving BPROP File... ";
		BestAffinityBackprop.outputBPROPFile(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".BPROP", FileTitle);
		cout << "Saving POP File... " << endl;
		WritePopFile(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".POP");
		SetConsoleTextAttribute(hConsole, White);

	}
	catch (...)
	{
		cout << "Exception Occurred";
	}
	cout << "Data Saved Successfully!" << endl;
}//SavePopData()

/*------------------------------------------------------------------------
    Program :           Population Object
    Written by:         Paul Killian
    Created:            2009
    Description:        Contains all the needed information for a Population
 -------------------------------------------------------------------------*/

//Include Files below
#include "Population.h"
#include "Backprop.h"
#include <ctype.h>
#include <vector>
#include <windows.h>
#include <string>
#include <iomanip>
#include <math.h>


HANDLE  hConsole;
enum { Blank, DarkBlue, DarkGreen, DarkLightBlue, DarkRed, Purple, DarkYellow, White, Grey, Blue, Green, LightBlue, Red, Pink, Yellow, BrightWhite };

Backprop *Population;
time_t Started;

	Population::Population( string filepath )
	{
		Started = time(0);	//Set Starting Time
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, White);
		string _METHODNAME = "New(filepath)";

		ImportPopFile( filepath );

		DeconstructCurrentDirectory( filepath );

		ImportDataSet( DataSetFile );

		//Testing to Verify DataSet Values for Debugging Purposes...
		//for(int i=0; i<100;i++)
		//	cout << i << ": " << exemplarInputs[i][0] << "," << exemplarInputs[i][1] << "," << exemplarInputs[i][2] << "," << exemplarInputs[i][3] << "," << exemplarInputs[i][4] << exemplarInputs[i][5] << "," << exemplarInputs[i][6] << endl;

		try
		{
			Pop = new Backprop[PopulationSize];
			cout << PopulationSize << " Total: ";
			for( int i=0; i<=PopulationSize-1;i++)
			{
				cout << i << " ";
				Backprop *newMember = new Backprop(BackpropParms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons);
				
				//Convert int to a string - Seems like lots of work for essentially a simple task
				string s;
				stringstream out;
				out << i;
				s = out.str();

				newMember->ID = s;
				Pop[i] = *newMember;
			}

			//Set BestBackprop object to its initial value which will be over-written later
			BestBackprop = Pop[0].Copy();
			//BestBackprop.Fitness = numeric_limits<double>::max( );
			BestBackprop.Fitness = 1.79769e+308;
			cout << endl << "Population Created!"<<endl;

			RunGenerations();

		}
		catch(...)
		{
			cout << "Exception!";
		}
	}

	Population::~Population( void )									//Destructor
	{

	}

	void Population::RunPopulation( int Epochs )
	{
		double PreviousFitness;
		double CurrentFitness;
		for(int i=0; i<=PopulationSize-1; i++)
		{
			Pop[i].calcFitness();
			PreviousFitness = Pop[i].Fitness;
			Pop[i].Train(Epochs, VerboseLogging);
			Pop[i].calcFitness();
			CurrentFitness = Pop[i].Fitness;

			if( CurrentFitness < PreviousFitness )
				SetConsoleTextAttribute(hConsole, Green);
			else if( CurrentFitness > PreviousFitness )
				SetConsoleTextAttribute(hConsole, Red);
			else
				SetConsoleTextAttribute(hConsole, BrightWhite);

			cout << Pop[i].ID << ": " << CurrentFitness << "	PrvFit: " << PreviousFitness << "	Diff: " << CurrentFitness - PreviousFitness << "	Avg: " << CurrentFitness / Pop[i].exemplarCount << endl;
		}
	}

	void Population::RunGeneration( int EpochsToRun, int NewGenerationMethod, bool GenerateNewPopulation )
	{
		RunPopulation(EpochsToRun);
		SetConsoleTextAttribute(hConsole, LightBlue);
		cout << "Sorting Population... " << endl;
		SortPopulation();
		cout << "Retaining Best Backprop...";
		RetainBestBackprop();
		if(GenerateNewPopulation)
		{
			NewPopulation(NewGenerationMethod);
		}
	}//RunGeneration()

	void Population::RunGenerations( void )
	{
		cout << "Calculating Initial Average Population Fitness... (Disabled)" << endl;
		AvgGenerationFitness.push_back(AveragePopulationFitness());
		cout << "Initial Average Population Fitness Calculated! (Disabled)" << endl;

		for(int i=0; i<=Generations-1; i++)
		{
			SetConsoleTextAttribute(hConsole, Yellow);
			cout << "Generation: " << i << "	Best Rho: " << Pop[0].rho << "	BestBackprop: " << BestBackprop.Fitness << endl;
			SetConsoleTextAttribute(hConsole, White);
			if(i < Generations-1)
				RunGeneration(EpochsPerGeneration, NewGenerationMethod, true);
			else
				RunGeneration(EpochsPerGeneration, NewGenerationMethod, false);

			BestBackpropFitness.push_back(BestBackprop.Fitness);
			AvgGenerationFitness.push_back(AveragePopulationFitness());
			SetConsoleTextAttribute(hConsole, White);
			//Calculating Avage Population Fitness --- NOT IMPLEMENTED YET

			if( i == Generations-1)
				SavePopData(true);
			else if( i % GenSaveInterval == 0)
				SavePopData(false);
//        Next
		}
	}

	void Population::SortPopulation( void )
	{
		sortedPopulation = new Backprop[PopulationSize];
		sortedArray = new bool[PopulationSize];
		double bestFitnessValue;
		int bestFitnessIndex = -1;

		//Set entire sortedArray to false
		for( int i=0; i<=PopulationSize-1;i++ )
			sortedArray[i] = false;

		for( int i=0; i<=PopulationSize-1; i++ )
		{
			bestFitnessValue = 1.79769e+308;
			for( int j=0; j<=PopulationSize-1; j++)
			{
				if( Pop[j].Fitness < bestFitnessValue && !sortedArray[j])
				{
					bestFitnessValue = Pop[j].Fitness;
					bestFitnessIndex = j;
				}
			}
			sortedPopulation[i] = Pop[bestFitnessIndex];
			sortedArray[bestFitnessIndex] = true;
		}

		//Set each new populatin member in the population array from the sorted array
		for(int i=0; i<=PopulationSize-1; i++)
			Pop[i] = sortedPopulation[i];

	}//sortPopulation

	void Population::NewPopulation( int GenerationMethod )
	{
		cout << "### NewPopulation Generation Currently Not Implemented - Entire Population Currently Cloned ###" << endl;;
	}

	void Population::ImportPopFile( string filepath )
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
			
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			DataSetFile = tokens[1];
			BackpropParms[0] = DataSetFile;
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			PopulationSize = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			Generations = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			EpochsPerGeneration = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			NewGenerationMethod = atoi(tokens[1].c_str());
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[1] = tokens[1];	//Hidden Neurons
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[2] = tokens[1];	//Hidden Layers
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[3] = tokens[1];	//Max Weight Value
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[4] = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[5] = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[6] = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[7] = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[8] = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[9] = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[10] = tokens[1];
			tokens.clear();

			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			GenSaveInterval = atoi(tokens[1].c_str());
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
			if(tokens[1] == "True" || tokens[1] == "T")
				VerboseLogging = true;
			else
				VerboseLogging = false;

			tokens.clear();

		}
		catch(...)
		{
			cout << "Exception Occurred!";
		}
	}

	void Population::RetainBestBackprop( void )
	{
		cout << " Calculating Current Pop Best Fitness... ";
		Pop[0].calcFitness();
		cout << " Calculating Previous Best Backprop Fitness...";
		BestBackprop.calcFitness();
		if( Pop[0].Fitness < BestBackprop.Fitness || BestBackprop.Fitness != BestBackprop.Fitness )
			//Note: Fitness != Fitness essentially checks if the value is not a number
		{
			BestBackprop = Pop[0].Copy();
			BestBackprop.ID = BestBackprop.ID + "-B";
			BestBackprop.calcFitness();
		}
		cout << "Best Backprop Retained!" << endl;
	}

	double Population::AveragePopulationFitness( void )
	{
		return 1;
	}

	void Population::WritePopFile( string filepath )
	{
		ofstream file;
		file.open(filepath.c_str());
		file.setf(ios::fixed);
		setprecision(10);
		try
			{
			file << "### Data Set ###" << endl;
			file << "Dataset file location=" << BackpropParms[0] << endl;
			file << endl;
			file << "### Population Configuration ###" << endl;
			file << "Population Size=" << PopulationSize << endl;
			file << "Generations=" << Generations << endl;
			file << "Epochs Per Generation=" << EpochsPerGeneration << endl;
			file << "New Generation Method=" << NewGenerationMethod << endl;
			file << endl;
			file << "### Network Topology ###" << endl;
			file << "Hidden Neurons=" << BackpropParms[1] << endl;
			file << "Hidden Layers=" << BackpropParms[2] << endl;
			file << endl;
			file << "### Training Configuation ###" << endl;
			file << "Max weight value=" << BackpropParms[3] << endl;
			file << "Max bias value=" << BackpropParms[4] << endl;
			file << "Initial rho value=" << BackpropParms[5] << endl;
			file << "Rho multiplier=" << BackpropParms[6] << endl;
			file << "Dynamic rho interval=" << BackpropParms[7] << endl;
			file << "Use Momentum=" << BackpropParms[8] << endl;
			file << "Momentum Scaler=" << BackpropParms[9] << endl;
			file << "Simulated annealing max percent(%)=" << BackpropParms[10] << endl;
			file << endl;
			file << "### Save Data Settings ###" << endl;
			file << "Generation Save Interval=" << GenSaveInterval << endl;
			file << "File Title=" << FileTitle << endl;
			file << endl;
			file << "### Console Logging ###" << endl;
			file << "Verbose Logging=" << VerboseLogging << endl;
		}
		catch(...)
		{
			cout << "Exception Occurred!";
		}
		file.close();
	}

	void Population::OutputPopulationData( string filepath )
	{
		time_t Finished = time(0);
		int SecondsElapsed = difftime(Finished, Started);

		ofstream file;
		file.open(filepath.c_str());
		file.setf(ios::fixed);
		setprecision(10);
		try
		{
			file << "Population_Backprop_App_CPP" << endl;
			file << endl;
			cout << "BestBackpropID: " << BestBackprop.ID << "	BestBackprop Fitness: " << BestBackprop.Fitness << endl;
			file << "BestBackpropID: " << BestBackprop.ID << "	BestBackprop Fitness: " << BestBackprop.Fitness << endl;
			file << endl;
			cout << "Initial Population Avg Fitness: " << AvgGenerationFitness[0] << endl;
			file << "Initial Population Avg Fitness: " << AvgGenerationFitness[0] << endl;
			for( int i=0; i<=BestBackpropFitness.size()-1; i++)
			{
				cout << "Best Backprop Fitness Generation " << i << ": " << BestBackpropFitness[i] << "	AvgFitness: " << AvgGenerationFitness[i+1] << endl;
				file << "Best Backprop Fitness Generation " << i << ": " << BestBackpropFitness[i] << "	AvgFitness: " << AvgGenerationFitness[i+1] << endl;
			}
			file << endl;
			cout << "Processed In: " << SecondsElapsed << " Seconds" << endl;
			file << "Processed In: " << SecondsElapsed << " Seconds" << endl;
		}
		catch(...)
		{
			cout << "Exception Occurred!";
		}
		file.close();
	}

	void Population::ImportDataSet( string filepath )
	{
		string _METHODNAME = "ImportDataSet(filepath)";
		string line;
		vector<string> inputs;
		vector<string> sInputValues;
		vector<string> sOutputValues;
		exemplarCount = numRowsInTextFile( filepath );

		ifstream inStream;			//create new ifstream
		char buffer[16384];	//16384 should be enough for even a very large data row
		inStream.open(filepath.c_str());

		inStream.getline(buffer,16384);	//Buffer
		line = buffer;
		Tokenize(line, inputs, "=");

		Tokenize(inputs[0], sInputValues, ",");
		Tokenize(inputs[1], sOutputValues,",");

		countInputNeurons = sInputValues.size();
		countOutputNeurons = sOutputValues.size();

		for (int i=0; i<= exemplarCount-1; i++)
		{
			vector<double> RowInputs(countInputNeurons,0);
			for (int k=0; k <= countInputNeurons-1; k++)
			{
				RowInputs[k] = atof(sInputValues[k].c_str());
			}

			vector<double> RowOutputs(countOutputNeurons,0);
			for (int k=0; k <= countOutputNeurons-1; k++)
			{
				RowOutputs[k] = atof(sOutputValues[k].c_str());
			}

			//Add the Row Vectors to the Exemplar Vectors
			exemplarInputs.push_back(RowInputs);
			exemplarOutputs.push_back(RowOutputs);

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

	}//ImportDataSet

	int Population::numRowsInTextFile( string filepath )
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

	void Population::DeconstructCurrentDirectory( string filepath )
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

	string Population::GenerateSaveName( void )
	{
		string AvgError;
		string Dt;
		BestBackprop.calcFitness();

  //      'Create Average Error String
		ostringstream oss;
		oss << (BestBackprop.Fitness / BestBackprop.exemplarCount);
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
	}

	void Population::SavePopData( bool OutputToConsole )
	{
		SetConsoleTextAttribute(hConsole, White);
		cout << "Recalculating Best Backprop Fitness..." << endl;
		BestBackprop.calcFitness();
		cout << "Saving data to: " << CurrentDirectory << GenerateSaveName() << "\\" << endl;

		try
		{
			string command;
			command = "mkdir \"" + CurrentDirectory + GenerateSaveName() + "\"";
			system(command.c_str());

			SetConsoleTextAttribute(hConsole, White);
			if(OutputToConsole)
			{
				cout <<"Saving Console Output... ";
				OutputPopulationData(CurrentDirectory + "\\ProcessOutput.txt");
			}

			cout <<"Saving Network... ";
			BestBackprop.outputNetwork(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".ANN");
			cout <<"Saving Network Output... ";
			BestBackprop.outputNetworkOutput(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".txt");
			cout <<"Saving BPROP File... ";
			BestBackprop.outputBPROPFile(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".BPROP", FileTitle);
			cout <<"Saving POP File... " << endl;
			WritePopFile(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".POP");
			SetConsoleTextAttribute(hConsole, White);

		}
		catch(...)
		{
			cout << "Exception Occurred";
		}
		cout << "Data Saved Successfully!" << endl;
	}//SavePopData()

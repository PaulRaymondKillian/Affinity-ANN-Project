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
		//Dim _METHODNAME As String = "New(filePath)"
  //      Console.WriteLine()
  //      Console.WriteLine("Creating Population...")
		Started = time(0);	//Set Starting Time
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, White);
		string _METHODNAME = "New(filepath)";
		cout << endl << "Creating Population..." << endl;

        //importPopFile(filepath)
		ImportPopFile( filepath );

        //DeconstructCurrentDirectory(filepath)
		DeconstructCurrentDirectory( filepath );

        //**Started = Now
		//ADD TIME ELEMENTS LATER......

        //importDataSet(DataSetFile)
		ImportDataSet( DataSetFile );

		//Testing to Verify DataSet Values for Debugging Purposes...
		//for(int i=0; i<100;i++)
		//	cout << i << ": " << exemplarInputs[i][0] << "," << exemplarInputs[i][1] << "," << exemplarInputs[i][2] << "," << exemplarInputs[i][3] << "," << exemplarInputs[i][4] << exemplarInputs[i][5] << "," << exemplarInputs[i][6] << endl;



        //ReDim Population(PopulationSize - 1)

        //Try
		try
		{
        //    'Initialize Backprop Objects of Population
        //    Console.Write(PopulationSize & " Total: ")
        //    For i As Integer = 0 To PopulationSize - 1
        //        Console.Write(i & " ")
        //        Population(i) = New Backprop.Backprop(BackpropParms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons)
        //        Population(i).ID = i.ToString
        //    Next
			SetConsoleTextAttribute(hConsole, 1);
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
				//Pop.push_back(*newMember); if using vectors
				Pop[i] = *newMember;
			}

        //    'Set BestBackprop object to its initial value which will be over-written later
        //    BestBackprop = Population(0).Copy
        //    BestBackprop.Fitness = Double.MaxValue  'Set to max value so it will always be overwritten
        //    Console.WriteLine()
        //    Console.WriteLine("Population Created!")
			BestBackprop = Pop[0].Copy();
			//BestBackprop.Fitness = numeric_limits<double>::max( );
			BestBackprop.Fitness = 1.79769e+308;
			cout << endl << "Population Created!"<<endl;

        //    RunGenerations()
			RunGenerations();

		}
        //Catch ex As Exception
		catch(...)
		{
        //    Throw New Exception("Error - Class: " & _CLASSNAME & " Method: " & _METHODNAME & " Descr: Error creating population; input file may be invalid or corrupt.")
        //End Try
		}
	}
	Population::~Population( void )									//Destructor
	{

	}

	void Population::RunPopulation( int Epochs )
	{
        //Dim previousFitness As Double
        //Dim currentFitness As Double
        //For i As Integer = 0 To PopulationSize - 1
        //    Population(i).calcFitness()
        //    previousFitness = Population(i).Fitness
        //    ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.Gray)
        //    Population(i).Train(Epochs, VerboseLogging)
        //    Population(i).calcFitness()
        //    currentFitness = Population(i).Fitness
        //    If currentFitness - previousFitness < 0 Then
        //        ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.Green)
        //    ElseIf currentFitness - previousFitness > 0 Then
        //        ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.Red)
        //    Else
        //        ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.White)
        //    End If
        //    Console.WriteLine(Population(i).ID & ": " & currentFitness & "   PrvFit: " & previousFitness & "    Diff: " & currentFitness - previousFitness & " Avg: " & currentFitness / Population(i).exemplarCount)
        //Next
		double PreviousFitness;
		double CurrentFitness;
		for(int i=0; i<=PopulationSize-1; i++)
		{
			Pop[i].calcFitness();
			PreviousFitness = Pop[i].Fitness;
			//Console Library
			Pop[i].Train(Epochs, VerboseLogging);
			Pop[i].calcFitness();
			CurrentFitness = Pop[i].Fitness;
			//if(CurrentFitness - PreviousFitness < 0)
			//	//ConsoleLibrary
			//else if(CurrentFitness - PreviousFitness > 0)
			//	//ConsoleLibrary
			//else
			//	//ConsoleLibrary

			cout << Pop[i].ID << ": " << CurrentFitness << "	PrvFit: " << PreviousFitness << "	Diff: " << CurrentFitness - PreviousFitness << "	Avg: " << CurrentFitness / Pop[i].exemplarCount << endl;

		}
	}

	void Population::RunGeneration( int EpochsToRun, int NewGenerationMethod, bool GenerateNewPopulation )
	{
//		  RunPopulation(EpochsToRun)
//        SortPopulation()
//        RetainBestBackprop()
//        If GenerateNewPopulation Then
//            ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.White)
//            NewPopulation(NewGenerationMethod)
//        End If
		RunPopulation(EpochsToRun);
		SortPopulation();
		RetainBestBackprop();
		if(GenerateNewPopulation)
		{
			NewPopulation(NewGenerationMethod);
		}
	}//RunGeneration()

	void Population::RunGenerations( void )
	{
//		  ReDim BestBackpropFitness(Generations - 1)
//        ReDim AvgGenerationFitness(Generations)
//
//        Console.WriteLine("Calculating Initial Average Population Fitness...")
//        AvgGenerationFitness(0) = AveragePopulationFitness()
//        Console.WriteLine("Initial Average Population Fitness Calculated!")
		cout << "Calculating Initial Average Population Fitness..." << endl;
		AvgGenerationFitness.push_back(AveragePopulationFitness());
		cout << "Initial Average Population Fitness Calculated!" << endl;
//
//        For i As Integer = 0 To Generations - 1
		for(int i=0; i<=Generations-1; i++)
		{
//            ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.Yellow)
//            Console.WriteLine("Generation: " & i & "    Best-Rho: " & Population(0).rho & "  BestBackprop: " & BestBackprop.Fitness)
//            If i < Generations - 1 Then
//                RunGeneration(EpochsPerGeneration, NewGenerationMethod, True)
//            Else
//                RunGeneration(EpochsPerGeneration, NewGenerationMethod, False)
//            End If
//            BestBackpropFitness(i) = BestBackprop.Fitness
			SetConsoleTextAttribute(hConsole, Yellow);
			cout << "Generation: " << i << "	Best Rho: " << Pop[0].rho << "	BestBackprop: " << BestBackprop.Fitness << endl;
			SetConsoleTextAttribute(hConsole, White);
			if(i < Generations-1)
				RunGeneration(EpochsPerGeneration, NewGenerationMethod, true);
			else
				RunGeneration(EpochsPerGeneration, NewGenerationMethod, false);
			BestBackpropFitness.push_back(BestBackprop.Fitness);
//
//            ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.WhiteForte)
//            Console.WriteLine("Calculating Average Population Fitness...")
//            AvgGenerationFitness(i + 1) = AveragePopulationFitness()
			AvgGenerationFitness.push_back(AveragePopulationFitness());
//            Console.WriteLine("Average Population Fitness Calculated!")
			SetConsoleTextAttribute(hConsole, White);
			//Calculating Avage Population Fitness --- NOT IMPLEMENTED YET
//
//            'Save the Population Data if Save Interval 
//            If i = Generations - 1 Then
//                SavePopData(True)
//            ElseIf i Mod GenSaveInterval = 0 Then
//                SavePopData(False)
//            End If
			if( i == Generations-1)
				SavePopData(true);
			else if( i % GenSaveInterval == 0)
				SavePopData(false);
//        Next
		}
	}

	void Population::SortPopulation( void )
	{
		//cout << "### SortPopulation Currently Not Implemented - Finish This Part Last ###" << endl;
  //	  Dim sortedPopulation(PopulationSize - 1) As Backprop.Backprop
  //      Dim sortedArray(PopulationSize - 1) As Boolean  'Set to false by default
  //      Dim bestFitnessValue As Double
  //      Dim bestFitnessIndex As Integer = -1
		Backprop sortedPopulation[PopulationSize];		//Now using const PopulationSize
		bool sortedArray[PopulationSize];
		double bestFitnessValue;
		int bestFitnessIndex = -1;

		//Set entire sortedArray to false
		for( int i=0; i<=PopulationSize-1;i++ )
			sortedArray[i] = false;

  //      'Selection Sort Algorithm
  //      For i As Integer = 0 To PopulationSize - 1
  //          bestFitnessValue = Double.MaxValue
  //          For j As Integer = 0 To PopulationSize - 1
  //              If Population(j).Fitness < bestFitnessValue And Not sortedArray(j) Then
  //                  bestFitnessValue = Population(j).Fitness
  //                  bestFitnessIndex = j
  //              End If
  //          Next
  //          sortedPopulation(i) = Population(bestFitnessIndex)
  //          sortedArray(bestFitnessIndex) = True
  //      Next
		//Selection Sort Algorithm
		//cout << endl << "Population Fitnesses - " << endl;
		//for(int i=0; i<=PopulationSize-1;i++)
		//	cout << "Pop[" << i << "]: " << Pop[i].Fitness << endl;

		//cout << endl;

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

  //      Population = sortedPopulation

		//cout << endl << "Population Fitnesses - " << endl;
		//for(int i=0; i<=PopulationSize-1;i++)
		//	cout << "sortedPopulation[" << i << "]: " << sortedPopulation[i].Fitness << endl;

		//cout << endl;

		//Cannot do the below, doesnt seem to work in C++
		//*Pop = *sortedPopulation;

		//cout << endl << "Population Fitnesses - " << endl;
		//for(int i=0; i<=PopulationSize-1;i++)
		//	cout << "Pop[" << i << "]: " << Pop[i].Fitness << endl;

		//cout << endl;

		//Set each new populatin member in the population array from the sorted array
		for(int i=0; i<=PopulationSize-1; i++)
			Pop[i] = sortedPopulation[i];

		//cout << endl << "Population Fitnesses - " << endl;
		//for(int i=0; i<=PopulationSize-1;i++)
		//	cout << "Pop[" << i << "]: " << Pop[i].Fitness << endl;

		//cout << endl;

	}

	void Population::NewPopulation( int GenerationMethod )
	{
		cout << "### NewPopulation Generation Currently Not Implemented - Entire Population Currently Cloned ###" << endl;;
	}

	void Population::ImportPopFile( string filepath )
	{
//		  Dim sr As StreamReader
//        Dim line As String
//        Dim readLine() As String
		ifstream inStream;			//create new ifstream
		string line;
		vector<string> tokens;
		char buffer[2048];

		try
		{
			//sr = File.OpenText(filepath)
			inStream.open(filepath.c_str());

//            sr.ReadLine()   'Buffer
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            DataSetFile = readLine(1)  'Dats Set File Location
//            BackpropParms(0) = DataSetFile
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			DataSetFile = tokens[1];
			BackpropParms[0] = DataSetFile;
			tokens.clear();

//
//            sr.ReadLine()   'Buffer
//            sr.ReadLine()   'Buffer
			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer
			
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            PopulationSize = (Convert.ToInt32(readLine(1)))
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			//PopulationSize = atoi(tokens[1].c_str());
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            Generations = Convert.ToInt32((readLine(1)))
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			Generations = atoi(tokens[1].c_str());
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            EpochsPerGeneration = Convert.ToInt32((readLine(1)))
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			EpochsPerGeneration = atoi(tokens[1].c_str());
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            NewGenerationMethod = Convert.ToInt32((readLine(1)))
			inStream.getline(buffer,2048);	//Buffer
			line = buffer;
			Tokenize(line, tokens, "=");
			NewGenerationMethod = atoi(tokens[1].c_str());
			tokens.clear();
//
//            sr.ReadLine()   'Buffer
//            sr.ReadLine()   'Buffer
			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(1) = readLine(1)  'Hidden Neurons
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[1] = tokens[1];	//Hidden Neurons
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(2) = readLine(1)  'Hidden Layers
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[2] = tokens[1];	//Hidden Layers
			tokens.clear();
//
//            sr.ReadLine()   'Buffer
//            sr.ReadLine()   'Buffer
			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer

//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(3) = readLine(1)  'Max Weight Value
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[3] = tokens[1];	//Max Weight Value
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(4) = readLine(1)  'Max Bias Value
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[4] = tokens[1];
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(5) = readLine(1)  'Rho
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[5] = tokens[1];
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(6) = readLine(1)  'Rho Multiplier
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[6] = tokens[1];
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(7) = readLine(1)  'Dynamic Rho Interval
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[7] = tokens[1];
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(8) = readLine(1)  'Use Momentum
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[8] = tokens[1];
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(9) = readLine(1)  'Momentum Scaler
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[9] = tokens[1];
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            BackpropParms(10) = readLine(1)  'Sim Annealing Max Percent
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			BackpropParms[10] = tokens[1];
			tokens.clear();
//
//            sr.ReadLine()   'Buffer
//            sr.ReadLine()   '
			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            GenSaveInterval = Convert.ToInt32(readLine(1))
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			GenSaveInterval = atoi(tokens[1].c_str());
			tokens.clear();
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            FileTitle = (readLine(1))
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			FileTitle = tokens[1];
			tokens.clear();
//
//            sr.ReadLine()   'Buffer
//            sr.ReadLine()   'Buffer
			inStream.getline(buffer,2048);	//Buffer
			inStream.getline(buffer,2048);	//Buffer
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            If readLine(1).ToUpper = "TRUE" Or readLine(1).ToUpper = "T" Then
//                VerboseLogging = True
//            Else
//                VerboseLogging = False
//            End If
			inStream.getline(buffer,2048);
			line = buffer;
			Tokenize(line, tokens, "=");
			if(tokens[1] == "True" || tokens[1] == "T")
				VerboseLogging = true;
			else
				VerboseLogging = false;

			tokens.clear();
//
//        Catch ex As Exception
//            Throw New Exception("Error - Population File is Invalid or Corrupt")
//        End Try



		}
		catch(...)
		{

		}
	}

	void Population::RetainBestBackprop( void )
	{
		//cout << "### RetrainBestBackprop Currently Not Implemented - Finish This Part Last ###" << endl;
		//Population(0).calcFitness()
  //      BestBackprop.calcFitness()
  //      If Population(0).Fitness < BestBackprop.Fitness Or Double.IsNaN(BestBackprop.Fitness) Then
  //          BestBackprop = Population(0).Copy
  //          BestBackprop.ID = BestBackprop.ID & "-B"
  //          BestBackprop.calcFitness()
  //      End If
		Pop[0].calcFitness();
		BestBackprop.calcFitness();
		if( Pop[0].Fitness < BestBackprop.Fitness || BestBackprop.Fitness != BestBackprop.Fitness )
					//Note: Fitness != Fitness essentially checks if the value is not a number
		{
			BestBackprop = Pop[0].Copy();
			BestBackprop.ID = BestBackprop.ID + "-B";
			BestBackprop.calcFitness();
		}
	}

	double Population::AveragePopulationFitness( void )
	{
		return 1;
	}

	void Population::WritePopFile( string filepath )
	{
//		  Dim sw As New StreamWriter(filePath)
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
//        Catch ex As Exception
//            Throw ex
//        Finally
//            sw.Close()
//        End Try
		}
		file.close();
	}

	void Population::OutputPopulationData( string filepath )
	{
//		  Dim Finished As DateTime
//        Finished = Now
//        Dim TimeElapsed As TimeSpan
//        TimeElapsed = Finished - Started
		time_t Finished = time(0);
		int SecondsElapsed = difftime(Finished, Started);
//
//        Dim sw As New StreamWriter(FilePath)
		ofstream file;
		file.open(filepath.c_str());
		file.setf(ios::fixed);
		setprecision(10);
		try
		{
//        sw.WriteLine("ANN_Fast_Backprop_Dynamic_Population")
//        sw.WriteLine()
//        Console.WriteLine("BestBackprop ID: " & BestBackprop.ID & "   BestBackprop Fitness:" & BestBackprop.Fitness)
//        sw.WriteLine("BestBackprop ID: " & BestBackprop.ID & "   BestBackprop Fitness:" & BestBackprop.Fitness)
//        sw.WriteLine()
//        Console.WriteLine("Initial Population Avg Fitness: " & AvgGenerationFitness(0))
//        sw.WriteLine("Initial Population Avg Fitness: " & AvgGenerationFitness(0))
//        For i As Integer = 0 To BestBackpropFitness.Length - 1
//            Console.WriteLine("Best Backprop Fitness Generation " & i & ": " & BestBackpropFitness(i) & "   AvgFitness: " & AvgGenerationFitness(i + 1))
//            sw.WriteLine("Best Backprop Fitness Generation " & i & ": " & BestBackpropFitness(i) & "   AvgFitness: " & AvgGenerationFitness(i + 1))
//        Next
//        sw.WriteLine()
//        Console.WriteLine("Processed In: " & Convert.ToInt32(TimeElapsed.Hours) & " Hours, " & Convert.ToInt32(TimeElapsed.Minutes) & " Minutes, " & Convert.ToInt32(TimeElapsed.Seconds) & " Seconds.")
//        sw.WriteLine("Processed In: " & Convert.ToInt32(TimeElapsed.Hours) & " Hours, " & Convert.ToInt32(TimeElapsed.Minutes) & " Minutes, " & Convert.ToInt32(TimeElapsed.Seconds) & " Seconds.")
//
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

//        sw.Close()
		}
		catch(...)
		{

		}
		file.close();
	}

	void Population::ImportDataSet( string filepath )
	{
//		Const _MethodName As String = "importDataSet(filePath)"
//		Dim line As String
//		Dim inputs() As String
//		Dim sInputValues() As String
//		Dim sOutputValues() As String
//		exemplarCount = numRowsInTextFile(filePath)
		string _METHODNAME = "ImportDataSet(filepath)";
		string line;
		vector<string> inputs;
		vector<string> sInputValues;
		vector<string> sOutputValues;
		//exemplarCount = numRowsInTextFile( filepath );
//
//		Dim sr As StreamReader = File.OpenText(filePath)
//		line = sr.ReadLine
		ifstream inStream;			//create new ifstream
		char buffer[16384];	//16384 should be enough for even a very large data row
		inStream.open(filepath.c_str());
//
//		inputs = line.Split("="c)
		inStream.getline(buffer,16384);	//Buffer
		line = buffer;
		Tokenize(line, inputs, "=");
//
//		If inputs.Count <> 2 Then
//			Throw New Exception("Error - Method: " & _MethodName & _
//										"Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//			Exit Sub
//		End If
//
//		sInputValues = inputs(0).Split(","c)
//		sOutputValues = inputs(1).Split(","c)
		Tokenize(inputs[0], sInputValues, ",");
		Tokenize(inputs[1], sOutputValues,",");
//
//		countInputNeurons = Convert.ToInt32(sInputValues.Count)
//		ReDim exemplarInputs(exemplarCount - 1, countInputNeurons - 1)
//		countOutputNeurons = Convert.ToInt32(sOutputValues.Count)
//		ReDim exemplarOutputs(exemplarCount - 1, countOutputNeurons - 1)
		//countInputNeurons = sInputValues.size();
		//exemplarInputs = new double[exemplarCount][countInputNeurons];
		//////double *temp1 = new double[exemplarCount][countInputNeurons];
		//countOutputNeurons = sOutputValues.size();
		//exemplarOutputs = new double[exemplarCount][countOutputNeurons];
//
//		For i As Integer = 0 To exemplarCount - 1
		for (int i=0; i<= exemplarCount-1; i++)
		{
//
//			For k As Integer = 0 To countInputNeurons - 1
			vector<double> RowInputs(countInputNeurons,0);
			for (int k=0; k <= countInputNeurons-1; k++)
			{
				RowInputs[k] = atof(sInputValues[k].c_str());
				
//				exemplarInputs(i, k) = Convert.ToDouble(sInputValues(k))
				//exemplarInputs[i,k] = copyVal;
				//cout << exemplarInputs[i,k] << "," << sInputValues[k].c_str() << " ";
//			Next
			}
//
//			For k As Integer = 0 To countOutputNeurons - 1
			vector<double> RowOutputs(countOutputNeurons,0);
			for (int k=0; k <= countOutputNeurons-1; k++)
			{
				RowOutputs[k] = atof(sOutputValues[k].c_str());
//				exemplarOutputs(i, k) = Convert.ToDouble(sOutputValues(k))
				//exemplarOutputs[i,k] = atof(sOutputValues[k].c_str());
				//cout << exemplarOutputs[i,k] << "," << sOutputValues[k].c_str() << " ";
//			Next
			}

			//Add the Row Vectors to the Exemplar Vectors
			exemplarInputs.push_back(RowInputs);
			exemplarOutputs.push_back(RowOutputs);
//
//			line = sr.ReadLine
			inStream.getline(buffer,16384);	//Buffer
			line = buffer;
//
//			If Not line = Nothing Then
			if (line != "")
			{
//				inputs = line.Split("="c)
				inputs.clear();
				Tokenize(line, inputs, "=");
//
//				If inputs.Count <> 2 Then
//					Throw New Exception("Error - Method: " & _MethodName & _
//										"Descr: Data File invalid or corrupt")
//
//					Exit Sub
//				End If
//
//				sInputValues = inputs(0).Split(","c)
//				sOutputValues = inputs(1).Split(","c)
			
				//sInputValues.clear();
				//sOutputValues.clear();

				vector<string> tempInput;
				vector<string> tempOutput;

				sInputValues = tempInput;
				sOutputValues = tempOutput;

				Tokenize(inputs[0], sInputValues, ",");
				Tokenize(inputs[1], sOutputValues,",");
//			End If
			}
//
//		Next
		}
//
//		sr.Close()

	}//ImportDataSet

	int Population::numRowsInTextFile( string filepath )
	{
		//Dim sr As StreamReader = File.OpenText(filePath)
		//Dim rows As Integer = 0
		//Dim line As String
		ifstream inStream;			//create new ifstream
		string line;
		int rows = 0;
		char buffer[16384];	//16384 should be enough for even a very large data row

		inStream.open(filepath.c_str());		

		//Do
		//	line = sr.ReadLine
		//	rows += 1
		//Loop Until sr.EndOfStream
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
        //Dim split() As String
		vector<string> tokens;

        //'Extract the Directory Path
        //split = filepath.Split("\"c)
		Tokenize(filepath, tokens, "\\");
        //For i As Integer = 0 To split.Length - 2
        //    If i = 0 Then
        //        CurrentDirectory = split(i) & "\"
        //    Else
        //        CurrentDirectory += split(i) & "\"
        //    End If
        //Next
		for( int i = 0; i <= tokens.size() -2; i++)
		{
			if( i == 0 )
				CurrentDirectory = tokens[i] + "\\";
			else
				CurrentDirectory += tokens[i] + "\\";
		}
	}

	string Population::GenerateSaveName( void )
	{
  //	  Dim AvgError, Dt As String
  //      BestBackprop.calcFitness()
		string AvgError;
		string Dt;
		BestBackprop.calcFitness();

  //      'Create Average Error String
  //      AvgError = Math.Round((BestBackprop.Fitness / BestBackprop.exemplarCount), 3).ToString
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

  //      Return AvgError & "_" & FileTitle & "_" & Dt
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

	   // Public Sub SavePopData(ByVal OutputToConsole As Boolean)
	void Population::SavePopData( bool OutputToConsole )
	{
    //    ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.White)
		SetConsoleTextAttribute(hConsole, White);

    //    BestBackprop.calcFitness()
    //    Console.WriteLine("Saving data to: " & CurrentDirectory & GenerateSaveName() & "\")
		BestBackprop.calcFitness();
		cout << "Saving data to: " << CurrentDirectory << GenerateSaveName() << "\\" << endl;
		//cout << "Saving data to: " << CurrentDirectory << "Because C++ is too fucking retarded and doing anything that should be simple is like fucking pulling teeth from a sabertooth tiger with a spoon. Damnit to hell.";

		try
		{
    //    Try
    //        Directory.CreateDirectory(CurrentDirectory & GenerateSaveName())
			string command;
			command = "mkdir \"" + CurrentDirectory + GenerateSaveName() + "\"";
			system(command.c_str());

    //        If OutputToConsole Then
    //            OutputPopulationData(CurrentDirectory & GenerateSaveName() & "\ProcessOutput.txt")
    //        End If
    //        BestBackprop.outputNetwork(CurrentDirectory & GenerateSaveName() & "\" & GenerateSaveName() & ".ANN")
    //        BestBackprop.outputNetworkOutput(CurrentDirectory & GenerateSaveName() & "\" & GenerateSaveName() & ".txt")
    //        BestBackprop.outputBPROPFile(CurrentDirectory & GenerateSaveName() & "\" & GenerateSaveName() & ".BPROP", FileTitle)
    //        WritePOPFile(CurrentDirectory & GenerateSaveName() & "\" & GenerateSaveName() & ".POP")
			
			if(OutputToConsole)
				OutputPopulationData(CurrentDirectory + "\\ProcessOutput.txt");

			BestBackprop.outputNetwork(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".ANN");
			BestBackprop.outputNetworkOutput(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".txt");
			BestBackprop.outputBPROPFile(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".BPROP", FileTitle);
			WritePopFile(CurrentDirectory + GenerateSaveName() + "\\" + GenerateSaveName() + ".POP");


		}
		catch(...)
		{
    //    Catch ex As Exception
		}
    //    End Try
    //    Console.WriteLine("Data Saved Successfully!")
		cout << "Data Saved Successfully!" << endl;
    //End Sub
	}//SavePopData()

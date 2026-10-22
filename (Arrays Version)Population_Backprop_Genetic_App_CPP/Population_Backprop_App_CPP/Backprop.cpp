//Include Files below
#include "Backprop.h"

HANDLE  hConsole2;
enum { Blank, DarkBlue, DarkGreen, DarkLightBlue, DarkRed, Purple, DarkYellow, White, Grey, Blue, Green, LightBlue, Red, Pink, Yellow, BrightWhite };


Backprop::Backprop( void ) // Default Constructor
{
//Do nothing
}

Backprop::Backprop( string filepath )					//Constructor with 1 parameter
{
//Do nothing for now
}//ANN()


Backprop::Backprop( string *pParms, vector< vector<double> > pExemplarInputs, vector< vector<double> > pExemplarOutputs, int pExemplarCount, int pCountInputNeurons, int pCountOutputNeurons)
{
	hConsole2 = GetStdHandle(STD_OUTPUT_HANDLE);
	//time(NULL) returns time in seconds only, which means that all backprop objects created during the same second time duration
	//  will all have same random seed, which is kinda stupid, so I added -clock() to mix up the seed value so they are all different.
	srand ( time(NULL)-clock() );

	Parms = pParms;
	importBackpropParms(Parms);
	exemplarInputs = pExemplarInputs;
	exemplarOutputs = pExemplarOutputs;
	VerifyArchitecture();
	exemplarCount = pExemplarCount;
	countInputNeurons = pCountInputNeurons;
	countOutputNeurons = pCountOutputNeurons;

    //InitializeArrays
	InitializeArrays();

    //RandomizeWeightAndBiasArrays()
	RandomizeWeightAndBiasArrays();

}

Backprop::~Backprop(void)								//Destructor
{

}


void Backprop::importBackpropParms( string *Parms )
{
	DataFileName = Parms[0];
	countHiddenNeurons = atoi(Parms[1].c_str());
	countHiddenLayers = atoi(Parms[2].c_str());
	maxWeightValue = atof(Parms[3].c_str());
	maxBiasValue = atof(Parms[4].c_str());
	rho = atof(Parms[5].c_str());
	rhoMultiplier = atof(Parms[6].c_str());
	rhoInterval = atoi(Parms[7].c_str());
	if(Parms[8] == "true" || Parms[8] == "True" || Parms[8] == "T")
		UseMomentum = true;
	else
		UseMomentum = false;
	MomentumScaler = atof(Parms[9].c_str());
	AnnealingMaxPercent = atoi(Parms[10].c_str());
}//ImportBackpropParms()


//    Public Sub importBackpropFile(ByVal filepath As String)
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
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            ID = readLine(1)  'Dats Set File Location
//            'ID = sr.ReadLine
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
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
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            DataFileName = readLine(1)  'Dats Set File Location
//            'DataFileName = sr.ReadLine
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
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
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            rho = Convert.ToDouble(readLine(1))  'Dats Set File Location
//            'rho = Convert.ToDouble(sr.ReadLine)
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            rhoMultiplier = Convert.ToDouble(readLine(1))  'Dats Set File Location
//            'rhoMultiplier = Convert.ToDouble(sr.ReadLine)
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            rhoInterval = Convert.ToInt32(readLine(1))  'Dats Set File Location
//            'rhoInterval = Convert.ToInt32(sr.ReadLine)
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            UseMomentum = Convert.ToBoolean(readLine(1))  'Dats Set File Location
//            'UseMomentum = Convert.ToBoolean(sr.ReadLine)
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            momentumScaler = Convert.ToDouble(readLine(1))  'Dats Set File Location
//            'momentumScaler = Convert.ToDouble(sr.ReadLine)
//
//            line = sr.ReadLine
//            readLine = line.Split("="c)
//            If readLine.Count <> 2 Then
//                Throw New Exception("Error - ANN_Fast_Backprop_PopulationApp    Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//                Exit Sub
//            End If
//            annealingMaxPercent = Convert.ToInt32(readLine(1))  'Dats Set File Location
//            'annealingMaxPercent = Convert.ToInt32(sr.ReadLine)
//        Catch ex As Exception
//            Throw ex
//        End Try
//    End Sub
//
//    Public Sub InitializeArrays()


void Backprop::InitializeArrays()
{

	//Initialize Input Values Array
	inputValues = new double[countInputNeurons];

	//Initialize Hidden Values Array
	hiddenValues = new double**[countHiddenLayers];
	for(int i=0; i<countHiddenLayers; i++)
	{
		double **arr;
		arr = new double*[countHiddenNeurons];
		for(int j=0; j<countHiddenNeurons; j++)
		{
			arr[j] = new double[3];
		}
		hiddenValues[i] = arr;
	}

	//Initialize Output Values Array
	outputValues = new double*[countOutputNeurons];
	for(int i=0; i<countOutputNeurons; i++)
		outputValues[i] = new double[3];


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
	ihWeights = new double**[countInputNeurons];
	for(int i=0; i<countInputNeurons; i++)
	{
		double **arr;
		arr = new double*[countHiddenNeurons];
		for(int j=0; j<countHiddenNeurons; j++)
		{
			arr[j] = new double[2];
		}
		ihWeights[i] = arr;
	}

	//Initialize Hidden-Hidden Weights Array
	hhWeights = new double***[countHiddenLayers];
	for(int x=0; x<countHiddenLayers; x++)
	{
		double ***a;
		a = new double**[countHiddenNeurons];
		for(int j=0; j<countHiddenNeurons; j++)
		{
			double **arr;
			arr = new double*[countHiddenNeurons];
			for (int i=0; i< countHiddenNeurons; i++)
				arr[i] = new double[2];
			
			a[j] = arr;
		}
		hhWeights[x] = a;
	}


	//Initialize Hidden-Output Weights Array
	hoWeights = new double**[countHiddenNeurons];
	for(int i=0; i<countHiddenNeurons; i++)
	{
		double **arr;
		arr = new double*[countOutputNeurons];
		for(int j=0; j<countOutputNeurons; j++)
		{
			arr[j] = new double[2];
		}
		hoWeights[i] = arr;
	}

}//InitializeArrays()

void Backprop::RandomizeWeightAndBiasArrays()
{
	for(int i=0; i<countInputNeurons;i++)
		for(int j=0; j<countHiddenNeurons;j++)
		{
			ihWeights[i][j][CURRENT] = RandomWeight(maxWeightValue);
			ihWeights[i][j][PREVIOUS] = ihWeights[i][j][CURRENT];
		}

		for(int layer=0; layer<countHiddenLayers-1;layer++)
			for(int i=0; i<countHiddenNeurons;i++)
				for(int j=0; j<countHiddenNeurons;j++)
				{
					hhWeights[layer][i][j][CURRENT] = RandomWeight(maxWeightValue);
					hhWeights[layer][i][j][PREVIOUS] = hhWeights[layer][i][j][CURRENT];
				}

		for(int i=0; i<countHiddenNeurons;i++)
			for(int j=0; j<countOutputNeurons;j++)
			{
				hoWeights[i][j][CURRENT] = RandomWeight(maxWeightValue);
				hoWeights[i][j][PREVIOUS] = hoWeights[i][j][CURRENT];
			}

		//Randomize Bias Values
		for(int layer=0; layer<countHiddenLayers; layer++)
			for(int i=0; i<countHiddenNeurons; i++)
				hiddenValues[layer][i][BIAS] = RandomWeight(maxBiasValue);

		for(int i=0; i<countOutputNeurons; i++)
			outputValues[i][BIAS] = RandomWeight(maxBiasValue);

}//RandomizeWeightAndBiasArrays()


//    Public Sub importANNFile(ByVal FilePath As String)
//        Dim sr As StreamReader
//        Dim line As String
//
//        Try
//            sr = File.OpenText(FilePath)
//
//            'Read in countInputNeurons and verify
//            line = sr.ReadLine
//            If Convert.ToInt32(line) <> countInputNeurons Then
//                Throw New Exception("Error - Importing ANN from File, Input Neuron Counts do not match between ANN file and DataSet file!")
//            End If
//
//            'Read in countHiddenNeurons
//            countHiddenNeurons = Convert.ToInt32(sr.ReadLine)
//
//            'Read in countHiddenLayers
//            countHiddenLayers = Convert.ToInt32(sr.ReadLine)
//
//            'Read in countOutputNeurons and verify
//            line = sr.ReadLine
//            If Convert.ToInt32(line) <> countOutputNeurons Then
//                Throw New Exception("Error - Importing ANN from File, Output Neuron Counts do not match between ANN file and DataSet file!")
//            End If
//
//            'Read in count Total Weights (not used in this program, but used in others)
//            line = sr.ReadLine
//
//            'Now Initialize Weight and Neuron arrays since we now have all needed data (counts)
//            InitializeArrays()
//
//            'Read in the Input-Hidden weights and Biases
//            For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//                For inputNeuron As Integer = 0 To countInputNeurons - 1
//                    ihWeights(inputNeuron, hiddenNeuron, CURRENT) = Convert.ToDouble(sr.ReadLine)
//                Next
//                hiddenValues(hiddenNeuron, 0, BIAS) = Convert.ToDouble(sr.ReadLine)
//            Next
//            'Below is the code used to write out the I-H Weights
//            'For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//            '    For inputNeuron As Integer = 0 To countInputNeurons - 1
//            '        sw.WriteLine(ihWeights(inputNeuron, hiddenNeuron, CURRENT))
//            '    Next
//            '    sw.WriteLine(hiddenValues(hiddenNeuron, 0, BIAS))
//            'Next
//
//
//
//            If countHiddenLayers > 1 Then
//                For layer As Integer = 0 To countHiddenLayers - 2
//                    For HiddenAbove As Integer = 0 To countHiddenNeurons - 1
//                        For HiddenBelow As Integer = 0 To countHiddenNeurons - 1
//                            hhWeights(HiddenBelow, HiddenAbove, layer, CURRENT) = Convert.ToDouble(sr.ReadLine)
//                        Next
//                        hiddenValues(HiddenAbove, layer + 1, BIAS) = Convert.ToDouble(sr.ReadLine)
//                    Next
//                Next
//            End If
//            'Below is the code used to write the H-H weights and biases
//            'If countHiddenLayers > 1 Then
//            '    For layer As Integer = 0 To countHiddenLayers - 2
//            '        For HiddenAbove As Integer = 0 To countHiddenNeurons - 1
//            '            For HiddenBelow As Integer = 0 To countHiddenNeurons - 1
//            '                sw.WriteLine(hhWeights(HiddenBelow, HiddenAbove, layer, CURRENT))
//            '            Next
//            '            sw.WriteLine(hiddenValues(HiddenAbove, layer + 1, BIAS))
//            '        Next
//            '    Next
//            'End If
//
//
//
//
//            For outputNeuron As Integer = 0 To countOutputNeurons - 1
//                For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//                    hoWeights(hiddenNeuron, outputNeuron, CURRENT) = Convert.ToDouble(sr.ReadLine)
//                Next
//                outputValues(outputNeuron, BIAS) = Convert.ToDouble(sr.ReadLine)
//            Next
//            'Below is the code used to write out the H-O weights and biases
//            'For outputNeuron As Integer = 0 To countOutputNeurons - 1
//            '    For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//            '        sw.WriteLine(hoWeights(hiddenNeuron, outputNeuron, CURRENT))
//            '    Next
//            '    sw.WriteLine(outputValues(outputNeuron, BIAS))
//            'Next
//
//
//        Catch ex As Exception
//            Throw ex
//        End Try
//
//    End Sub




void Backprop::outputNetwork( string fileName )
{
	ANNFileName = fileName;		//Save ANN File Path for use in BPROP File output

	ofstream file;
	file.open(fileName.c_str());
	file.setf(ios::fixed);
	setprecision(10);
	file << countInputNeurons << endl;
	file << countHiddenNeurons << endl;
	file << countHiddenLayers << endl;
	file << countOutputNeurons << endl;
	file << ((countInputNeurons + 1) * countHiddenNeurons) + ((countHiddenLayers - 1) * (countHiddenNeurons + 1) * countHiddenNeurons) + ((countHiddenNeurons + 1) * countOutputNeurons) << endl;

	for( int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
	{
		for( int inputNeuron=0; inputNeuron<=countInputNeurons-1; inputNeuron++)
			file << ihWeights[inputNeuron][hiddenNeuron][CURRENT] <<endl;

		file << hiddenValues[0][hiddenNeuron][BIAS] << endl;
	}

	if( countHiddenLayers > 1)
	{
		for( int layer=0; layer<=countHiddenLayers-2; layer++)
		{
			for( int HiddenAbove=0; HiddenAbove<=countHiddenNeurons-1; HiddenAbove++)
			{
				for( int HiddenBelow=0; HiddenBelow<=countHiddenNeurons-1; HiddenBelow++)
					file << hhWeights[layer][HiddenBelow][HiddenAbove][CURRENT] << endl;

				file << hiddenValues[layer+1][HiddenAbove][BIAS] << endl;
			}
		}
	}

	for( int outputNeuron=0; outputNeuron<=countOutputNeurons-1; outputNeuron++)
	{
		for( int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
			file << hoWeights[hiddenNeuron][outputNeuron][CURRENT] << endl;

		file << outputValues[outputNeuron][BIAS] << endl;
	}

	file.close();
}//outputNetwork()


void Backprop::outputBPROPFile( string filename, string FileTitle )
{
	if( DataFileName == "")
		throw exception("Error creating BPROP File: DataFileName cannot be nothing!");

	if( ANNFileName == "")
		throw exception("Error creating BPROP File: ANNFileName cannot be nothing, output ANN File first, then create BPROP file.");

	ofstream file;
	file.open(filename.c_str());
	file.setf(ios::fixed);
	setprecision(10);
	try
	{
            file << "### Backprop Identification ###" << endl;
            file << "ID=" << ID << endl;
            file << "File Title=" << FileTitle << endl;
            file << endl;
            file << "### Input Files - !!! NEVER MODIFY THIS PART !!! ###" << endl;
            file << "DO NOT MODIFY: Dataset file location=" << DataFileName << endl;
            file << "DO NOT MODIFY: ANN file location=" << ANNFileName << endl;
            file << endl;
            file << "### Training Configuration ###" << endl;
            file << "Initial rho value=" << rho << endl;
            file << "Rho multiplier=" << rhoMultiplier << endl;
            file << "Rho interval=" << rhoInterval << endl;
            file << "Use Momentum=" << UseMomentum << endl;
            file << "Momentum scaler=" << MomentumScaler << endl;
            file << "Simulated annealing max percent(%)=" << AnnealingMaxPercent << endl;

	}
	catch(...)
	{
		cout << "Exception Occurred!";
	}
	file.close();
}//outputBPROPFile()


void Backprop::outputNetworkOutput( string fileName )
{
	ofstream file;
	file.open(fileName.c_str());
	file.setf(ios::fixed);
	setprecision(10);
	try
	{
		file << "Fitness: " << Fitness << " Avg: " << (Fitness / exemplarCount) << " Rho: " << rho << endl;

		for( int i=0; i <=exemplarCount-1; i++ )
		{
			for( int j=0; j<=countInputNeurons-1; j++ )
			{
				file << exemplarInputs[i][j];
				if( j != countInputNeurons-1)
					file << ",";
			}

			run(i);
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

void Backprop::run( int exemplarIndex )
{
	for(int i=0; i<=countInputNeurons-1; i++)
		inputValues[i] = exemplarInputs[exemplarIndex][i];

	//Calculate First Layer of Hidden Neuron Values
	for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
	{
		hiddenValues[0][hiddenNeuron][VALUE] = 0; //Reset to 0
		for(int inputNeuron=0; inputNeuron<=countInputNeurons-1; inputNeuron++)
			hiddenValues[0][hiddenNeuron][VALUE] += inputValues[inputNeuron] * ihWeights[inputNeuron][hiddenNeuron][CURRENT];

		hiddenValues[0][hiddenNeuron][VALUE] = Activation(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]);
	}

	//Calculate Hidden Layer Neuron Values
	if( countHiddenLayers > 1)
	{
		for(int layer=1; layer<=countHiddenLayers-1; layer++)
		{
			for(int aboveHidden=0; aboveHidden<=countHiddenNeurons-1; aboveHidden++)
			{
				hiddenValues[layer][aboveHidden][VALUE] = 0;
				for(int belowHidden=0; belowHidden<=countHiddenNeurons-1; belowHidden++)
				{
					hiddenValues[layer][aboveHidden][VALUE] += hiddenValues[layer-1][belowHidden][VALUE] * hhWeights[layer-1][belowHidden][aboveHidden][CURRENT];
				}
				hiddenValues[layer][aboveHidden][VALUE] = Activation(hiddenValues[layer][aboveHidden][VALUE] +hiddenValues[layer][aboveHidden][BIAS]);
			}
		}
	}

	//Calculate Output Neuron Values
	for( int outputNeuron=0; outputNeuron<=countOutputNeurons-1; outputNeuron++)
	{
		outputValues[outputNeuron][VALUE] = 0;	//Reset to 0
		for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
		{
			outputValues[outputNeuron][VALUE] += hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE] * hoWeights[hiddenNeuron][outputNeuron][CURRENT];
		}
		//We are not squashing the output neurons, so simply add the bias to it
		outputValues[outputNeuron][VALUE] += outputValues[outputNeuron][BIAS];
	}

}//run()


void Backprop::runBackprop(int iteration)
{
	//Calculate Output Neuron Deltas
	for(int i=0; i<=countOutputNeurons-1; i++)
		outputValues[i][DELTA] = -2 * (exemplarOutputs[iteration][i] - outputValues[i][VALUE]);

	//Calculate Top Layer Hidden Neuron Deltas
	//double totalDeltas = 0	//May use later, but leave commented out for now...
	for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
	{
		hiddenValues[countHiddenLayers-1][hiddenNeuron][DELTA] = 0;
		for(int outputNeuron=0; outputNeuron<=countOutputNeurons-1; outputNeuron++)
		{
			hiddenValues[countHiddenLayers-1][hiddenNeuron][DELTA] += fPrime(hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE]) * outputValues[outputNeuron][DELTA] * hoWeights[hiddenNeuron][outputNeuron][CURRENT];
			//totalDeltas += hiddenValues[countHiddenLayers-1][hiddenNeuron][DELTA] //Keep a running total of delta values for analysis
		}
	}

	//Calculate Hidden Layer Neuron Deltas
	if( countHiddenLayers>1 )
	{
		for(int layer=countHiddenLayers-2; layer>=0; layer--)
		{
			for(int belowHidden=0; belowHidden<=countHiddenNeurons-1; belowHidden++)
			{
				hiddenValues[layer][belowHidden][DELTA] = 0;	//Reset value
					for(int aboveHidden=0; aboveHidden<=countHiddenNeurons-1; aboveHidden++)
					{
						hiddenValues[layer][belowHidden][DELTA] += fPrime(hiddenValues[layer][belowHidden][VALUE]) * hiddenValues[layer+1][aboveHidden][DELTA] * hhWeights[layer][belowHidden][aboveHidden][CURRENT];
						//totalDeltas += hiddenValues[layer][belowHidden][DELTA];	//Keep a running total of delta values for analysis
					}
			}
		}
	}

	//Calculate New Weights for hoWeights
	double PreviousWeightChange;
	for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
	{
		for(int outputNeuron=0; outputNeuron<=countOutputNeurons-1; outputNeuron++)
		{
			if(UseMomentum)
			{
				PreviousWeightChange = hoWeights[hiddenNeuron][outputNeuron][CURRENT] - hoWeights[hiddenNeuron][outputNeuron][PREVIOUS];	//Store previous weight differential
				hoWeights[hiddenNeuron][outputNeuron][PREVIOUS] = hoWeights[hiddenNeuron][outputNeuron][CURRENT];	//Store previous weight
				hoWeights[hiddenNeuron][outputNeuron][CURRENT] = hoWeights[hiddenNeuron][outputNeuron][CURRENT] - rho * outputValues[outputNeuron][DELTA] * hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE] + MomentumScaler * PreviousWeightChange;
			}
			else
			{
				hoWeights[hiddenNeuron][outputNeuron][CURRENT] = hoWeights[hiddenNeuron][outputNeuron][CURRENT] - rho * outputValues[outputNeuron][DELTA] * hiddenValues[countHiddenLayers-1][hiddenNeuron][VALUE];
			}
		}
	}

	//Calculate New Hidden-Hidden Weights
	if( countHiddenLayers>1)
	{
		for(int layer=countHiddenLayers-2; layer>=0; layer--)
		{
			for(int belowHidden=0; belowHidden<=countHiddenNeurons-1; belowHidden++)
			{
				for(int aboveHidden=0; aboveHidden<=countHiddenNeurons-1; aboveHidden++)
				{
					if(UseMomentum)
					{
						PreviousWeightChange = hhWeights[layer][belowHidden][aboveHidden][CURRENT] - hhWeights[layer][belowHidden][aboveHidden][PREVIOUS];
						hhWeights[layer][belowHidden][aboveHidden][PREVIOUS] = hhWeights[layer][belowHidden][aboveHidden][CURRENT];
						hhWeights[layer][belowHidden][aboveHidden][CURRENT] = hhWeights[layer][belowHidden][aboveHidden][CURRENT] - rho * hiddenValues[layer+1][aboveHidden][DELTA] * hiddenValues[layer][belowHidden][VALUE] + MomentumScaler * PreviousWeightChange;
					}
					else
					{
						hhWeights[layer][belowHidden][aboveHidden][CURRENT] = hhWeights[layer][belowHidden][aboveHidden][CURRENT] - rho * hiddenValues[layer+1][aboveHidden][DELTA] * hiddenValues[layer][belowHidden][VALUE];
					}
				}
			}
		}
	}

	//Calculate Input-Hidden New Weights
	for(int inputNeuron=0; inputNeuron<=countInputNeurons-1; inputNeuron++)
	{
		for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
		{
			if(UseMomentum)
			{
				PreviousWeightChange = ihWeights[inputNeuron][hiddenNeuron][CURRENT] - ihWeights[inputNeuron][hiddenNeuron][PREVIOUS];
				ihWeights[inputNeuron][hiddenNeuron][PREVIOUS] = ihWeights[inputNeuron][hiddenNeuron][CURRENT];
				ihWeights[inputNeuron][hiddenNeuron][CURRENT] = ihWeights[inputNeuron][hiddenNeuron][CURRENT] - rho * hiddenValues[0][hiddenNeuron][DELTA] * inputValues[inputNeuron] + MomentumScaler * PreviousWeightChange;
			}
			else
			{
				ihWeights[inputNeuron][hiddenNeuron][CURRENT] = ihWeights[inputNeuron][hiddenNeuron][CURRENT] - rho * hiddenValues[0][hiddenNeuron][DELTA] * inputValues[inputNeuron];
			}
		}
	}
}//runBackprop()

void Backprop::TrainEpoch( bool UseRhoMultiplier, bool SimulatedAnnealing)
{
	Fitness = 0;	//Reset Fitness Value
	randomizedIndex = randomizeTheIndex(exemplarCount);

	for(int iteration=0; iteration<=exemplarCount-1; iteration++)
	{
		run(randomizedIndex[iteration]);
		runBackprop(randomizedIndex[iteration]);
	}

	if(SimulatedAnnealing)
		InitiateSimulatedAnnealing();

	if(UseRhoMultiplier)
		rho *= rhoMultiplier;

}//TrainEpoch()

void Backprop::Train( int Epochs, bool VerboseLogging)
{
	SetConsoleTextAttribute(hConsole2, Grey);
	time_t Time1Start = time(0);
	for(int i=1; i<=Epochs; i++)
	{
		if( i % rhoInterval == 0)
			TrainEpoch(true,false);
		else
			TrainEpoch(false,false);

		if(VerboseLogging && i % 100 == 0)
			cout << ".";
	}

	time_t Time1End = time(0);
	if(VerboseLogging)
		cout << " - " << Epochs << " Epochs Trained in: " << difftime(Time1End, Time1Start) << " seconds, " << exemplarCount << " Exemplar Sets, Momentum: " << UseMomentum << ", Network: " << countInputNeurons << "-" << countHiddenNeurons << "-" << countHiddenLayers << "-" << countOutputNeurons <<endl;
}//Train()

//
//    Public Sub InitiateSimulatedAnnealing()
void Backprop::InitiateSimulatedAnnealing()
{
	for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
		for(int outputNeuron=0; outputNeuron<=countOutputNeurons-1; outputNeuron++)
			hoWeights[hiddenNeuron][outputNeuron][CURRENT] = hoWeights[hiddenNeuron][outputNeuron][CURRENT] + ((RandomNumber(AnnealingMaxPercent) / 100) * hoWeights[hiddenNeuron][outputNeuron][CURRENT]);

	if( countHiddenLayers>1)
	{
		for(int layer=countHiddenLayers-2; layer>=0; layer--)
		{
			for(int belowHidden=0; belowHidden<=countHiddenNeurons-1; belowHidden++)
			{
				for(int aboveHidden=0; aboveHidden<=countHiddenNeurons-1; aboveHidden++)
				{
					hhWeights[layer][belowHidden][aboveHidden][CURRENT] = hhWeights[layer][belowHidden][aboveHidden][CURRENT] + ((RandomNumber(AnnealingMaxPercent) / 100) * hhWeights[layer][belowHidden][aboveHidden][CURRENT]);
				}
			}
		}
	}

	for(int inputNeuron=0; inputNeuron<=countInputNeurons-1; inputNeuron++)
	{
		for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
		{
			ihWeights[inputNeuron][hiddenNeuron][CURRENT] = ihWeights[inputNeuron][hiddenNeuron][CURRENT] + ((RandomNumber(AnnealingMaxPercent) / 100) * ihWeights[inputNeuron][hiddenNeuron][CURRENT]);
		}
	}
}//InitiateSimulatedAnnealing()


void Backprop::calcFitness( void )
{
	Fitness = 0;
	double ActualValue;
	double DesiredValue;

	for(int iteration=0; iteration<exemplarCount-1; iteration++)
	{
		run(iteration);
		for(int i=0; i<=countOutputNeurons-1; i++)
		{
			ActualValue = outputValues[i][VALUE];
			DesiredValue = exemplarOutputs[iteration][i];
			Fitness += AbsVal(DesiredValue - ActualValue);
		}
	}
}//calcFitness()

Backprop Backprop::Copy()
{
	Backprop *ACopy = new Backprop(Parms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons);

	ACopy->ID = ID;
	ACopy->rho = rho;
	ACopy->rhoInterval = rhoInterval;
	ACopy->rhoMultiplier = rhoMultiplier;

	for(int i=0; i<=countInputNeurons-1; i++)
		for( int j=0; j<=countHiddenNeurons-1; j++)
		{
			ACopy->ihWeights[i][j][CURRENT] = ihWeights[i][j][CURRENT];
			ACopy->ihWeights[i][j][PREVIOUS] = ihWeights[i][j][PREVIOUS];
		}

		if(countHiddenLayers > 1)
			for(int layer=0; layer<=countHiddenLayers-2; layer++)
				for(int i=0; i<=countHiddenNeurons-1; i++)
					for(int j=0; j<=countHiddenNeurons-1; j++)
					{
						ACopy->hhWeights[layer][i][j][CURRENT] = hhWeights[layer][i][j][CURRENT];
						ACopy->hhWeights[layer][i][j][PREVIOUS] = hhWeights[layer][i][j][PREVIOUS];
					}

		for( int i=0; i<=countHiddenNeurons-1; i++)
			for( int j=0; j<=countOutputNeurons-1; j++)
			{
				ACopy->hoWeights[i][j][CURRENT] = hoWeights[i][j][CURRENT];
				ACopy->hoWeights[i][j][PREVIOUS] = hoWeights[i][j][PREVIOUS];
			}

		for( int layer=0; layer<=countHiddenLayers-1; layer++)
			for( int i=0; i<=countHiddenNeurons-1; i++)
				ACopy->hiddenValues[layer][i][BIAS] = hiddenValues[layer][i][BIAS];

		for( int i=0; i<=countOutputNeurons-1; i++)
			ACopy->outputValues[i][BIAS] = outputValues[i][BIAS];

	return *ACopy;
}//Copy()

//
//    Public Sub importDataSet(ByVal filePath As String)
//        Const _MethodName As String = "importDataSet(filePath)"
//        Dim line As String
//        Dim inputs() As String
//        Dim sInputValues() As String
//        Dim sOutputValues() As String
//        exemplarCount = numRowsInTextFile(filePath)
//
//        Dim sr As StreamReader = File.OpenText(filePath)
//        line = sr.ReadLine
//
//        inputs = line.Split("="c)
//
//        If inputs.Count <> 2 Then
//            Throw New Exception("Error - Method: " & _MethodName & _
//                                        "Descr: Invalid contents of input file. Inputs and DesiredOutput should be delimited by a single '=' character")
//            Exit Sub
//        End If
//
//        sInputValues = inputs(0).Split(","c)
//        sOutputValues = inputs(1).Split(","c)
//
//        countInputNeurons = Convert.ToInt32(sInputValues.Count)
//        ReDim exemplarInputs(exemplarCount - 1, countInputNeurons - 1)
//        countOutputNeurons = Convert.ToInt32(sOutputValues.Count)
//        ReDim exemplarOutputs(exemplarCount - 1, countOutputNeurons - 1)
//
//        For i As Integer = 0 To exemplarCount - 1
//
//            For k As Integer = 0 To countInputNeurons - 1
//                exemplarInputs(i, k) = Convert.ToDouble(sInputValues(k))
//            Next
//
//            For k As Integer = 0 To countOutputNeurons - 1
//                exemplarOutputs(i, k) = Convert.ToDouble(sOutputValues(k))
//            Next
//
//            line = sr.ReadLine
//
//            If Not line = Nothing Then
//                inputs = line.Split("="c)
//
//                If inputs.Count <> 2 Then
//                    Throw New Exception("Error - Method: " & _MethodName & _
//                                        "Descr: Data File invalid or corrupt")
//
//                    Exit Sub
//                End If
//
//                sInputValues = inputs(0).Split(","c)
//                sOutputValues = inputs(1).Split(","c)
//            End If
//
//        Next
//
//        sr.Close()
//
//    End Sub
//
//    Public Sub DeconstructCurrentDirectory(ByVal filepath As String)
//        Dim split() As String
//
//        'Extract the Directory Path
//        split = filepath.Split("\"c)
//        For i As Integer = 0 To split.Length - 2
//            If i = 0 Then
//                CurrentDirectory = split(i) & "\"
//            Else
//                CurrentDirectory += split(i) & "\"
//            End If
//        Next
//    End Sub



//////////////////////////////////////////////////////////////////////////
// Complementary Methods


double Backprop::Activation(double value)
{
	return (1 / (1 + pow(2.71828183,-value)));	//FIX THIS FOR E!!!! AGH!!!!!!!!!
}//Activation()


double Backprop::fPrime(double u)
{
	return u * (1 - u);
}//fPrime()

double Backprop::RandomWeight( double maxValue )
{
	if( maxValue > 1 || maxValue < -1)
		return RandomNumber(maxValue-1) + (RandomNumber(1000000) / 1000000);
	else
		return (double(RandomNumber(1000000)) / 1000000) * maxValue;
}//RandomWeight()

int Backprop::RandomNumber( int MaxNumber )
{
	return rand() % MaxNumber;
}

double Backprop::AbsVal( double value )
{
	if( value < 0 )
		return -value;
	else
		return value;
}//AbsVal()

vector<int> Backprop::randomizeTheIndex(int count)
{
	vector<int> randomArray(count);

	//Set initial values
	for(int i=0; i<=count-1; i++)
		randomArray[i] = i;

	//Randomize the values
	int rand1, rand2, tempVal;
	for(int i=0; i<=count-1; i++)
	{
		rand1 = AbsVal(RandomNumber(count-1));
		rand2 = AbsVal(RandomNumber(count-1));

		tempVal = randomArray[rand1];
		randomArray[rand1] = randomArray[rand2];
		randomArray[rand2] = tempVal;
	}
	return randomArray;
}//randomizeTheIndex()

bool Backprop::VerifyArchitecture()
{
	bool isVerified = true;
	string msg = "";

	//Check ExemplarCount Matches
	if( exemplarCount != exemplarInputs.size() )
	{
		isVerified = false;
		msg.append("Exemplar Count does not match, ");
	}


	//Check Input Neurons -- Add Later?
	//Check Output Neurons -- Add Later?

	return isVerified;
}//VerifyArchitecture





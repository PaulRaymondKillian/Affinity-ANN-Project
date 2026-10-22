/*------------------------------------------------------------------------
    Program :           ANN Object
    Written by:         Paul Killian
    Created:            2009
    Description:        Contains all the needed information for an ANN
 -------------------------------------------------------------------------*/

//Include Files below
#include "Backprop.h"
#include <ctype.h>
#include <math.h>
#include <iomanip>

Backprop::Backprop( void ) // Default Constructor
{
//Do nothing

}

Backprop::Backprop( string filepath )					//Constructor with 1 parameter
{

}//ANN()


Backprop::Backprop( string *pParms, vector< vector<double> > pExemplarInputs, vector< vector<double> > pExemplarOutputs, int pExemplarCount, int pCountInputNeurons, int pCountOutputNeurons)
//Backprop::Backprop( string *pParms, double **pExemplarInputs, double **pExemplarOutputs, int pExemplarCount, int pCountInputNeurons, int pCountOutputNeurons)
{
    //Public Sub New(ByVal Parms() As String, ByRef pExemplarInputs(,) As Double, ByRef pExemplarOutputs(,) As Double, ByVal pExemplarCount As Integer, ByVal pCountInputNeurons As Integer, ByVal pCountOutputNeurons As Integer)
    //    _Parms = Parms
    //    importBackpropParms(_Parms)
    //    exemplarInputs = pExemplarInputs
    //    exemplarOutputs = pExemplarOutputs
    //    exemplarCount = pExemplarCount
    //    countInputNeurons = pCountInputNeurons
    //    countOutputNeurons = pCountOutputNeurons
	
	srand ( time(NULL) );

	Parms = pParms;
	importBackpropParms(Parms);
	exemplarInputs = pExemplarInputs;
	exemplarOutputs = pExemplarOutputs;
	//exemplarCount = pExemplarCount;
	//countInputNeurons = pCountInputNeurons;
	//countOutputNeurons = pCountOutputNeurons;

    //    InitializeArrays
	PrepVectors();

    //    RandomizeWeightAndBiasArrays()
	RandomizeWeightAndBiasArrays();

    //End Sub
}

Backprop::~Backprop(void)								//Destructor
{

}


//Public Sub importBackpropParms(ByVal Parms() As String)
void Backprop::importBackpropParms( string *Parms )
{
//
//        DataFileName = Parms(0)
//        countHiddenNeurons = Convert.ToInt32(Parms(1))
//        countHiddenLayers = Convert.ToInt32(Parms(2))
//        maxWeightValue = Convert.ToDouble(Parms(3))
//        maxBiasValue = Convert.ToDouble(Parms(4))
//        rho = Convert.ToDouble(Parms(5))
//        rhoMultiplier = Convert.ToDouble(Parms(6))
//        rhoInterval = Convert.ToInt32(Parms(7))
//        UseMomentum = Convert.ToBoolean(Parms(8))
//        momentumScaler = Convert.ToDouble(Parms(9))
//        annealingMaxPercent = Convert.ToInt32(Parms(10))
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
	//UseMomentum = Parms[8].c_str();
	MomentumScaler = atof(Parms[9].c_str());
	AnnealingMaxPercent = atoi(Parms[10].c_str());
//
//    End Sub
}//ImportBackpropParms()
//
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
////Since we are using vectors, we do not "initialize" the size of them like an array, however, in this method
////	we are going to prep the vectors to the desired sizes, so that for the rest of the code we can treat it 
////	just like an array, which is good because the code being ported used arrays.
void Backprop::PrepVectors()
{
//        'Initialize Arrays
	//Prep Vector Sizes
	vector<double> temp1;
	vector< vector<double> > temp2;
	vector< vector< vector<double> > > temp3;

//        ReDim inputValues(countInputNeurons - 1)
	inputValues.resize(countInputNeurons,0);

//        ReDim hiddenValues(countHiddenNeurons - 1, countHiddenLayers - 1, 2)
	temp1.resize(3,0);
	temp2.resize(countHiddenNeurons, temp1);
	hiddenValues.resize(countHiddenLayers,temp2);

//        ReDim outputValues(countOutputNeurons - 1, 2)
	temp1.clear();
	temp1.resize(3,0);
	outputValues.resize(countOutputNeurons, temp1);

//        ReDim ihWeights(countInputNeurons - 1, countHiddenNeurons - 1, 1)
	temp1.clear();
	temp2.clear();
	temp1.resize(2,0);
	temp2.resize(countHiddenNeurons,temp1);
	ihWeights.resize(countInputNeurons,temp2);

//        If countHiddenLayers > 1 Then
//            ReDim hhWeights(countHiddenNeurons - 1, countHiddenNeurons - 1, countHiddenLayers - 2, 1)
//        End If
	if(countHiddenLayers > 1)
	{
		temp1.clear();
		temp2.clear();
		temp1.resize(2,0);
		temp2.resize(countHiddenNeurons,temp1);
		temp3.resize(countHiddenNeurons,temp2);
		hhWeights.resize(countHiddenLayers-1,temp3);
	}
//        ReDim hoWeights(countHiddenNeurons - 1, countOutputNeurons - 1, 1)
	temp1.clear();
	temp2.clear();
	temp1.resize(2,0);
	temp2.resize(countOutputNeurons,temp1);
	hoWeights.resize(countHiddenNeurons,temp2);
//    End Sub
}
//
//    Public Sub RandomizeWeightAndBiasArrays()
void Backprop::RandomizeWeightAndBiasArrays()
{
//        'Randomize Weight Arrays
//        For i As Integer = 0 To countInputNeurons - 1
//            For j As Integer = 0 To countHiddenNeurons - 1
//                ihWeights(i, j, CURRENT) = RandomWeight(maxWeightValue)
//                ihWeights(i, j, PREVIOUS) = ihWeights(i, j, CURRENT)    'Setting Previous Weight so first calculation is 0
//            Next
//        Next
	for(int i=0; i<countInputNeurons;i++)
		for(int j=0; j<countHiddenNeurons;j++)
		{
			ihWeights[i][j][CURRENT] = RandomWeight(maxWeightValue);
			ihWeights[i][j][PREVIOUS] = ihWeights[i][j][CURRENT];
		}

//
//        For layer As Integer = 0 To countHiddenLayers - 2
//            For i As Integer = 0 To countHiddenNeurons - 1
//                For j As Integer = 0 To countHiddenNeurons - 1
//                    hhWeights(i, j, layer, CURRENT) = RandomWeight(maxWeightValue)
//                    hhWeights(i, j, layer, PREVIOUS) = hhWeights(i, j, layer, CURRENT)  'Setting Previous Weight so first calculation is 0
//                Next
//            Next
//        Next
		for(int layer=0; layer<countHiddenLayers-1;layer++)
			for(int i=0; i<countHiddenNeurons;i++)
				for(int j=0; j<countHiddenNeurons;j++)
				{
					hhWeights[layer][i][j][CURRENT] = RandomWeight(maxWeightValue);
					hhWeights[layer][i][j][PREVIOUS] = hhWeights[layer][i][j][CURRENT];
				}
//
//        For i As Integer = 0 To countHiddenNeurons - 1
//            For j As Integer = 0 To countOutputNeurons - 1
//                hoWeights(i, j, CURRENT) = RandomWeight(maxWeightValue)
//                hoWeights(i, j, PREVIOUS) = hoWeights(i, j, CURRENT)    'Setting Previous Weight so first calculation is 0
//            Next
//        Next
		for(int i=0; i<countHiddenNeurons;i++)
			for(int j=0; j<countOutputNeurons;j++)
			{
				hoWeights[i][j][CURRENT] = RandomWeight(maxWeightValue);
				hoWeights[i][j][PREVIOUS] = hoWeights[i][j][CURRENT];
			}
//
//        'Randomize Bias Values
//        For layer As Integer = 0 To countHiddenLayers - 1
//            For i As Integer = 0 To countHiddenNeurons - 1
//                hiddenValues(i, layer, BIAS) = RandomWeight(maxBiasValue)
//            Next
//        Next
			for(int layer=0; layer<countHiddenLayers; layer++)
				for(int i=0; i<countHiddenNeurons; i++)
					hiddenValues[layer][i][BIAS] = RandomWeight(maxBiasValue);
//
//        For i As Integer = 0 To countOutputNeurons - 1
//            outputValues(i, BIAS) = RandomWeight(maxBiasValue)
//        Next
		for(int i=0; i<countOutputNeurons; i++)
			outputValues[i][BIAS] = RandomWeight(maxBiasValue);

//    End Sub
}//RandomizeWeightAndBiasArrays()

//
//
//
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
//
//    Public Sub outputNetwork(ByVal fileName As String)
void Backprop::outputNetwork( string fileName )
{
//        ANNFileName = fileName  'Save ANN File Path for use in BPROP File output
	ANNFileName = fileName;
//
//        Dim sw As New StreamWriter(fileName)
//        sw.WriteLine(countInputNeurons)
//        sw.WriteLine(countHiddenNeurons)
//        sw.WriteLine(countHiddenLayers)
//        sw.WriteLine(countOutputNeurons)
//        sw.WriteLine(((countInputNeurons + 1) * countHiddenNeurons) + ((countHiddenLayers - 1) * (countHiddenNeurons + 1) * countHiddenNeurons) + ((countHiddenNeurons + 1) * countOutputNeurons))
	ofstream file;
	file.open(fileName.c_str());
	file.setf(ios::fixed);
	setprecision(10);
	file << countInputNeurons << endl;
	file << countHiddenNeurons << endl;
	file << countHiddenLayers << endl;
	file << countOutputNeurons << endl;
	file << ((countInputNeurons + 1) * countHiddenNeurons) + ((countHiddenLayers - 1) * (countHiddenNeurons + 1) * countHiddenNeurons) + ((countHiddenNeurons + 1) * countOutputNeurons) << endl;

//        For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//            For inputNeuron As Integer = 0 To countInputNeurons - 1
//                sw.WriteLine(ihWeights(inputNeuron, hiddenNeuron, CURRENT))
//            Next
//            sw.WriteLine(hiddenValues(hiddenNeuron, 0, BIAS))
//        Next
	for( int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
	{
		for( int inputNeuron=0; inputNeuron<=countInputNeurons-1; inputNeuron++)
			file << ihWeights[inputNeuron][hiddenNeuron][CURRENT] <<endl;

		file << hiddenValues[0][hiddenNeuron][BIAS] << endl;
	}
//
//        If countHiddenLayers > 1 Then
//            For layer As Integer = 0 To countHiddenLayers - 2
//                For HiddenAbove As Integer = 0 To countHiddenNeurons - 1
//                    For HiddenBelow As Integer = 0 To countHiddenNeurons - 1
//                        sw.WriteLine(hhWeights(HiddenBelow, HiddenAbove, layer, CURRENT))
//                    Next
//                    sw.WriteLine(hiddenValues(HiddenAbove, layer + 1, BIAS))
//                Next
//            Next
//        End If
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
//
//        For outputNeuron As Integer = 0 To countOutputNeurons - 1
//            For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//                sw.WriteLine(hoWeights(hiddenNeuron, outputNeuron, CURRENT))
//            Next
//            sw.WriteLine(outputValues(outputNeuron, BIAS))
//        Next
	for( int outputNeuron=0; outputNeuron<=countOutputNeurons-1; outputNeuron++)
	{
		for( int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
			file << hoWeights[hiddenNeuron][outputNeuron][CURRENT] << endl;

		file << outputValues[outputNeuron][BIAS] << endl;
	}
//
//        sw.Close()
	file.close();
//
//    End Sub
}//outputNetwork()

//
//    Public Sub outputBPROPFile(ByVal filename As String, ByVal FileTitle As String)
void Backprop::outputBPROPFile( string filename, string FileTitle )
{
//        If DataFileName = "" Then
//            Throw New Exception("Error creting BPROP file: DataFileName cannot be nothing")
//        End If
//        If ANNFileName = "" Then
//            Throw New Exception("Error creating BPROP file: ANNFileName cannot be nothing, output ANN File first, then create BPROP file.")
//        End If
	if( DataFileName == "")
		throw exception("Error creating BPROP File: DataFileName cannot be nothing!");

	if( ANNFileName == "")
		throw exception("Error creating BPROP File: ANNFileName cannot be nothing, output ANN File first, then create BPROP file.");

//
//        Dim sw As New StreamWriter(filename)
	ofstream file;
	file.open(filename.c_str());
	file.setf(ios::fixed);
	setprecision(10);
	try
	{
//        Try
//            sw.WriteLine("### Backprop Identification ###")
//            sw.WriteLine("ID=" & ID)
//            sw.WriteLine("File Title=" & FileTitle)
//            sw.WriteLine()
//            sw.WriteLine("### Input Files - !!! NEVER MODIFY THIS PART !!! ###")
//            sw.WriteLine("DO NOT MODIFY: Dataset file location=" & DataFileName)
//            sw.WriteLine("DO NOT MODIFY: ANN file location=" & ANNFileName)
//            sw.WriteLine()
//            sw.WriteLine("### Training Configuration ###")
//            sw.WriteLine("Initial rho value=" & Convert.ToDecimal(rho))
//            sw.WriteLine("Rho multiplier=" & rhoMultiplier)
//            sw.WriteLine("Rho interval=" & rhoInterval)
//            sw.WriteLine("Use Momentum=" & UseMomentum)
//            sw.WriteLine("Momentum scaler=" & momentumScaler)
//            sw.WriteLine("Simulated annealing max percent(%)=" & annealingMaxPercent)

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
//        Catch ex As Exception
//            Throw ex
//        Finally
//            sw.Close()
//        End Try
	}
	file.close();
//    End Sub
}//outputBPROPFile()

//
//    Public Sub outputNetworkOutput(ByVal fileName As String)
void Backprop::outputNetworkOutput( string fileName )
{
//        Dim sw As New StreamWriter(fileName)
//
	ofstream file;
	file.open(fileName.c_str());
	file.setf(ios::fixed);
	setprecision(10);
	try
	{
//        sw.WriteLine("Fitness: " & Fitness & " Avg: " & (Fitness / exemplarCount) & "  Rho: " & rho)
		file << "Fitness: " << Fitness << " Avg: " << (Fitness / exemplarCount) << " Rho: " << rho << endl;
//
//        For i As Integer = 0 To exemplarCount - 1
//            For j As Integer = 0 To countInputNeurons - 1
//                sw.Write(exemplarInputs(i, j))
//                If j <> countInputNeurons - 1 Then
//                    sw.Write(",")
//                End If
//            Next
//
//            run(i)
//            sw.Write("=")
//            For j As Integer = 0 To countOutputNeurons - 1
//                sw.Write(outputValues(j, VALUE))
//                If j <> countOutputNeurons - 1 Then
//                    sw.Write(",")
//                End If
//            Next
//            sw.WriteLine()
//        Next
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
//        Catch ex As Exception
//            Throw ex
//        Finally
//            sw.Close()
//        End Try
	}
	file.close();
//
//        sw.Close()
//
//    End Sub
}//outputNetworkOutput()

//
//    Public Sub run(ByVal exemplarIndex As Integer)
void Backprop::run( int exemplarIndex )
{
//        For i As Integer = 0 To countInputNeurons - 1
//            inputValues(i) = exemplarInputs(exemplarIndex, i)
//        Next
	for(int i=0; i<=countInputNeurons-1; i++)
		inputValues[i] = exemplarInputs[exemplarIndex][i];
//
//        'Calculate First Layer of Hidden Neuron Values
//        For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//            hiddenValues(hiddenNeuron, 0, VALUE) = 0   'Reset to 0
//            For inputNeuron As Integer = 0 To countInputNeurons - 1
//                hiddenValues(hiddenNeuron, 0, VALUE) += inputValues(inputNeuron) * ihWeights(inputNeuron, hiddenNeuron, CURRENT)
//            Next
//            hiddenValues(hiddenNeuron, 0, VALUE) = Activation(hiddenValues(hiddenNeuron, 0, VALUE) + hiddenValues(hiddenNeuron, 0, BIAS))
//        Next
	//Calculate First Layer of Hidden Neuron Values
	for(int hiddenNeuron=0; hiddenNeuron<countHiddenNeurons-1; hiddenNeuron++)
	{
		hiddenValues[0][hiddenNeuron][VALUE] = 0; //Reset to 0
		for(int inputNeuron=0; inputNeuron<=countInputNeurons-1; inputNeuron++)
			hiddenValues[0][hiddenNeuron][VALUE] += inputValues[inputNeuron] * ihWeights[inputNeuron][hiddenNeuron][CURRENT];

		hiddenValues[0][hiddenNeuron][VALUE] = Activation(hiddenValues[0][hiddenNeuron][VALUE] + hiddenValues[0][hiddenNeuron][BIAS]);
	}
//
//        'Calculate Hidden Layer Neuron Values
//        If countHiddenLayers > 1 Then
//            For layer As Integer = 1 To countHiddenLayers - 1
//                For aboveHidden As Integer = 0 To countHiddenNeurons - 1
//                    hiddenValues(aboveHidden, layer, VALUE) = 0   'Reset to 0
//                    For belowHidden As Integer = 0 To countHiddenNeurons - 1
//                        hiddenValues(aboveHidden, layer, VALUE) += hiddenValues(belowHidden, layer - 1, VALUE) * hhWeights(belowHidden, aboveHidden, layer - 1, CURRENT)
//                    Next
//                    hiddenValues(aboveHidden, layer, VALUE) = Activation(hiddenValues(aboveHidden, layer, VALUE) + hiddenValues(aboveHidden, layer, BIAS))
//                Next
//            Next
//        End If
	//Calculate Hidden Layer Neuron Values
	if( countHiddenLayers > 1)
	{
		for(int layer=1; layer<=countHiddenLayers-1; layer++)\
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
//
//        'Calculate Output Neuron Values
//        For outputNeuron As Integer = 0 To countOutputNeurons - 1
//            outputValues(outputNeuron, VALUE) = 0    'Reset to 0
//            For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//                outputValues(outputNeuron, VALUE) += hiddenValues(hiddenNeuron, countHiddenLayers - 1, VALUE) * hoWeights(hiddenNeuron, outputNeuron, CURRENT)
//            Next
//            outputValues(outputNeuron, VALUE) += outputValues(outputNeuron, BIAS)
//        Next
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
//    End Sub

}//run()

//
//    Public Sub Backprop(ByVal iteration As Integer)
void Backprop::runBackprop(int iteration)
{
//        'Calculate Output Neuron Deltas
//        For i As Integer = 0 To countOutputNeurons - 1
//            outputValues(i, DELTA) = -2 * (exemplarOutputs(iteration, i) - outputValues(i, VALUE))
//        Next
	//Calculate Output Neuron Deltas
	for(int i=0; i<=countOutputNeurons-1; i++)
		outputValues[i][DELTA] = -2 * (exemplarOutputs[iteration][i] - outputValues[i][VALUE]);
//
//        'Calculate Top Layer Hidden Neuron Deltas
//        Dim totalDeltas As Double = 0
//        For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//            hiddenValues(hiddenNeuron, countHiddenLayers - 1, DELTA) = 0   'Reset Value
//            For outputNeuron As Integer = 0 To countOutputNeurons - 1
//                hiddenValues(hiddenNeuron, countHiddenLayers - 1, DELTA) += fPrime(hiddenValues(hiddenNeuron, countHiddenLayers - 1, VALUE)) * outputValues(outputNeuron, DELTA) * hoWeights(hiddenNeuron, outputNeuron, CURRENT)
//                totalDeltas += hiddenValues(hiddenNeuron, countHiddenLayers - 1, DELTA) 'Keep running total of delta values for analysis
//            Next
//        Next
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
//
//        'Calculate Hidden Layer Neuron Deltas
//        If countHiddenLayers > 1 Then
//            For layer As Integer = countHiddenLayers - 2 To 0 Step -1
//                For belowHidden As Integer = 0 To countHiddenNeurons - 1
//                    hiddenValues(belowHidden, layer, DELTA) = 0 'Reset Value
//                    For aboveHidden As Integer = 0 To countHiddenNeurons - 1
//                        hiddenValues(belowHidden, layer, DELTA) += fPrime(hiddenValues(belowHidden, layer, VALUE)) * hiddenValues(aboveHidden, layer + 1, DELTA) * hhWeights(belowHidden, aboveHidden, layer, CURRENT)
//                        totalDeltas += hiddenValues(belowHidden, layer, DELTA)  'Keep running total of delta values for analysis
//                    Next
//                Next
//            Next
//        End If
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
//
//        'Calculate Hidden-Output Weights
//        ''Utilizing Momemtum
//        '
//        ''XXXXXXXXX MOMENTUM CURRENTLY DISABLED TO TEST FASTER PROCESSING ENHANCEMENTS XXXXXXXX
//        '
//        Dim previousWeightChange As Double
//        For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//            For outputNeuron As Integer = 0 To countOutputNeurons - 1
//                If UseMomentum Then
//                    previousWeightChange = hoWeights(hiddenNeuron, outputNeuron, CURRENT) - hoWeights(hiddenNeuron, outputNeuron, PREVIOUS)    'Store previous weight change
//                    hoWeights(hiddenNeuron, outputNeuron, PREVIOUS) = hoWeights(hiddenNeuron, outputNeuron, CURRENT)    'Store current weight to be used as the next iteration's previous weight
//                    hoWeights(hiddenNeuron, outputNeuron, CURRENT) = hoWeights(hiddenNeuron, outputNeuron, CURRENT) - rho * outputValues(outputNeuron, DELTA) * hiddenValues(hiddenNeuron, countHiddenLayers - 1, VALUE) + momentumScaler * previousWeightChange
//
//                Else
//                    'XXXX Momentum Stuff Commented Out, otherwise exactly the same as above XXXX
//                    'previousWeightChange = hoWeights(hiddenNeuron, outputNeuron, CURRENT) - hoWeights(hiddenNeuron, outputNeuron, PREVIOUS)    'Store previous weight change
//                    'hoWeights(hiddenNeuron, outputNeuron, PREVIOUS) = hoWeights(hiddenNeuron, outputNeuron, CURRENT)    'Store current weight to be used as the next iteration's previous weight
//                    hoWeights(hiddenNeuron, outputNeuron, CURRENT) = hoWeights(hiddenNeuron, outputNeuron, CURRENT) - rho * outputValues(outputNeuron, DELTA) * hiddenValues(hiddenNeuron, countHiddenLayers - 1, VALUE) ' + momentumScaler * previousWeightChange
//                End If
//            Next
//        Next
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
//
//        'Calculate Hidden-Hidden Weights
//        'Utilizing Momentum
//        If countHiddenLayers > 1 Then
//            For layer As Integer = countHiddenLayers - 2 To 0 Step -1
//                For belowHidden As Integer = 0 To countHiddenNeurons - 1
//                    For aboveHidden As Integer = 0 To countHiddenNeurons - 1
//                        If UseMomentum Then
//                            previousWeightChange = hhWeights(belowHidden, aboveHidden, layer, CURRENT) - hhWeights(belowHidden, aboveHidden, layer, PREVIOUS) 'Store previous weight change
//                            hhWeights(belowHidden, aboveHidden, layer, PREVIOUS) = hhWeights(belowHidden, aboveHidden, layer, CURRENT)  'Store current weight to be used as next iteration's previous weight
//                            hhWeights(belowHidden, aboveHidden, layer, CURRENT) = hhWeights(belowHidden, aboveHidden, layer, CURRENT) - rho * hiddenValues(aboveHidden, layer + 1, DELTA) * hiddenValues(belowHidden, layer, VALUE) + momentumScaler * previousWeightChange
//                        Else
//                            'XXXX Momentum Stuff Commented Out, otherwise exactly the same as above XXXX
//                            'previousWeightChange = hhWeights(belowHidden, aboveHidden, layer, CURRENT) - hhWeights(belowHidden, aboveHidden, layer, PREVIOUS) 'Store previous weight change
//                            'hhWeights(belowHidden, aboveHidden, layer, PREVIOUS) = hhWeights(belowHidden, aboveHidden, layer, CURRENT)  'Store current weight to be used as next iteration's previous weight
//                            hhWeights(belowHidden, aboveHidden, layer, CURRENT) = hhWeights(belowHidden, aboveHidden, layer, CURRENT) - rho * hiddenValues(aboveHidden, layer + 1, DELTA) * hiddenValues(belowHidden, layer, VALUE) ' + momentumScaler * previousWeightChange
//                        End If
//                    Next
//                Next
//            Next
//        End If
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
//
//        'Calculate Input-Hidden Weights
//        ''Utilizing Momentum
//        For inputNeuron As Integer = 0 To countInputNeurons - 1
//            For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//                If UseMomentum Then
//                    previousWeightChange = ihWeights(inputNeuron, hiddenNeuron, CURRENT) - ihWeights(inputNeuron, hiddenNeuron, PREVIOUS)
//                    ihWeights(inputNeuron, hiddenNeuron, PREVIOUS) = ihWeights(inputNeuron, hiddenNeuron, CURRENT)
//                    ihWeights(inputNeuron, hiddenNeuron, CURRENT) = ihWeights(inputNeuron, hiddenNeuron, CURRENT) - rho * hiddenValues(hiddenNeuron, 0, DELTA) * inputValues(inputNeuron) + momentumScaler * previousWeightChange
//                Else
//                    'XXXX Momentum Stuff Commented Out, otherwise exactly the same as above XXXX
//                    'previousWeightChange = ihWeights(inputNeuron, hiddenNeuron, CURRENT) - ihWeights(inputNeuron, hiddenNeuron, PREVIOUS)
//                    'ihWeights(inputNeuron, hiddenNeuron, PREVIOUS) = ihWeights(inputNeuron, hiddenNeuron, CURRENT)
//                    ihWeights(inputNeuron, hiddenNeuron, CURRENT) = ihWeights(inputNeuron, hiddenNeuron, CURRENT) - rho * hiddenValues(hiddenNeuron, 0, DELTA) * inputValues(inputNeuron) ' + momentumScaler * previousWeightChange
//                End If
//            Next
//        Next
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
//    End Sub
}//runBackprop()

//
//    Public Sub TrainEpoch(ByVal UseRhoMultiplier As Boolean, ByVal SimulatedAnnealing As Boolean)
void Backprop::TrainEpoch( bool UseRhoMultiplier, bool SimulatedAnnealing)
{
//        Fitness = 0 'Reset Fitness Value
//        randomizedIndex = randomizeTheIndex(exemplarCount)
	Fitness = 0;	//Reset Fitness Value
	randomizedIndex = randomizeTheIndex(exemplarCount);
//
//        For iteration As Integer = 0 To exemplarCount - 1
//            run(randomizedIndex(iteration))
//            Backprop(randomizedIndex(iteration))
//        Next
	for(int iteration=0; iteration<=exemplarCount-1; iteration++)
	{
		run(randomizedIndex[iteration]);
		runBackprop(randomizedIndex[iteration]);
	}
//
//        'Simulated Annealing
//        ''Adjust each weight in the weights layers to be in a location 'near' its current location (simulating shaking the solution plane)
//        If SimulatedAnnealing Then
//            InitiateSimulatedAnnealing()
//        End If
	//Simulated Annealing
	if(SimulatedAnnealing)
		InitiateSimulatedAnnealing();
//
//        'Dynamic Rho Value
//        If UseRhoMultiplier Then
//            rho *= rhoMultiplier
//        End If
	//Dynamic Rho Value
	if(UseRhoMultiplier)
		rho *= rhoMultiplier;
//
//    End Sub
}//TrainEpoch()

//
//    Public Sub Train(ByVal Epochs As Integer, ByVal VerboseLogging As Boolean)
void Backprop::Train( int Epochs, bool VerboseLogging)
{
//        Dim Time1Start As DateTime = Now
//        Dim Time1End As DateTime
//        Dim TimeSpan1 As TimeSpan
//        For i As Integer = 1 To Epochs
//            If (i Mod rhoInterval) = 0 Then
//                TrainEpoch(True, False)
//            Else
//                TrainEpoch(False, False)
//            End If
//
//            If VerboseLogging And i Mod 100 = 0 Then
//                Console.Write(".")
//            End If
//        Next
//        Time1End = Now
//        TimeSpan1 = Time1End - Time1Start
//        If VerboseLogging Then
//            Console.WriteLine(" - " & Epochs & " Epochs Trained in: " & Convert.ToInt32(TimeSpan1.Hours) & "h, " & Convert.ToInt32(TimeSpan1.Minutes) & "m, " & Convert.ToInt32(TimeSpan1.Seconds) & "s, " & Convert.ToInt32(TimeSpan1.Milliseconds) & "ms - " & exemplarCount & " Exemplar Sets, Momentum: " & UseMomentum & ", Network: " & countInputNeurons & "-" & countHiddenNeurons & "-" & countHiddenLayers & "-" & countOutputNeurons)
//        End If
	//ADD TIME ELEMENTS LATER.......
	//Time1Start =
	//Time1End =
	//TimeSpan1 =
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
//        Time1End = Now
//        TimeSpan1 = Time1End - Time1Start
	time_t Time1End = time(0);
	if(VerboseLogging)
		cout << " - " << Epochs << " Epochs Trained in: " << difftime(Time1End, Time1Start) << " seconds, " << exemplarCount << " Exemplar Sets, Momentum: " << UseMomentum << ", Network: " << countInputNeurons << "-" << countHiddenNeurons << "-" << countHiddenLayers << "-" << countOutputNeurons <<endl;
	//    End Sub
}//Train()

//
//    Public Sub InitiateSimulatedAnnealing()
void Backprop::InitiateSimulatedAnnealing()
{
//
//        For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//            For outputNeuron As Integer = 0 To countOutputNeurons - 1
//                hoWeights(hiddenNeuron, outputNeuron, CURRENT) = hoWeights(hiddenNeuron, outputNeuron, CURRENT) + ((RandomNumber(annealingMaxPercent) / 100) * hoWeights(hiddenNeuron, outputNeuron, CURRENT))
//            Next
//        Next
	for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
		for(int outputNeuron=0; outputNeuron<=countOutputNeurons-1; outputNeuron++)
			hoWeights[hiddenNeuron][outputNeuron][CURRENT] = hoWeights[hiddenNeuron][outputNeuron][CURRENT] + ((RandomNumber(AnnealingMaxPercent) / 100) * hoWeights[hiddenNeuron][outputNeuron][CURRENT]);
//
//        If countHiddenLayers > 1 Then
//            For layer As Integer = countHiddenLayers - 2 To 0 Step -1
//                For belowHidden As Integer = 0 To countHiddenNeurons - 1
//                    For aboveHidden As Integer = 0 To countHiddenNeurons - 1
//                        hhWeights(belowHidden, aboveHidden, layer, CURRENT) = hhWeights(belowHidden, aboveHidden, layer, CURRENT) + ((RandomNumber(annealingMaxPercent) / 100) * hhWeights(belowHidden, aboveHidden, layer, CURRENT))
//                    Next
//                Next
//            Next
//        End If
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
//
//        For inputNeuron As Integer = 0 To countInputNeurons - 1
//            For hiddenNeuron As Integer = 0 To countHiddenNeurons - 1
//                ihWeights(inputNeuron, hiddenNeuron, CURRENT) = ihWeights(inputNeuron, hiddenNeuron, CURRENT) + ((RandomNumber(annealingMaxPercent) / 100) * ihWeights(inputNeuron, hiddenNeuron, CURRENT))
//            Next
//        Next
	for(int inputNeuron=0; inputNeuron<=countInputNeurons-1; inputNeuron++)
	{
		for(int hiddenNeuron=0; hiddenNeuron<=countHiddenNeurons-1; hiddenNeuron++)
		{
			ihWeights[inputNeuron][hiddenNeuron][CURRENT] = ihWeights[inputNeuron][hiddenNeuron][CURRENT] + ((RandomNumber(AnnealingMaxPercent) / 100) * ihWeights[inputNeuron][hiddenNeuron][CURRENT]);
		}
	}
//
//    End Sub
}//InitiateSimulatedAnnealing()


//
//    Public Sub calcFitness()
void Backprop::calcFitness( void )
{
//        Fitness = 0
//        For iteration As Integer = 0 To exemplarCount - 1
//            run(iteration)
//            For i As Integer = 0 To countOutputNeurons - 1
//                Fitness += AbsVal(exemplarOutputs(iteration, i) - outputValues(i, VALUE))  'Error
//            Next
//        Next
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
			//Fitness += AbsVal(exemplarOutputs[iteration][i] - outputValues[i][VALUE]);
		}
	}
//    End Sub
}//calcFitness()
//
//    Public Function Copy() As Backprop
Backprop Backprop::Copy()
{
//        Dim ACopy As New Backprop(_Parms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons)
	Backprop *ACopy = new Backprop(Parms, exemplarInputs, exemplarOutputs, exemplarCount, countInputNeurons, countOutputNeurons);
//
//        ACopy.ID = ID
//        ACopy.rho = rho
//        ACopy.rhoInterval = rhoInterval
//        ACopy.rhoMultiplier = rhoMultiplier
	ACopy->ID = ID;
	ACopy->rho = rho;
	ACopy->rhoInterval = rhoInterval;
	ACopy->rhoMultiplier = rhoMultiplier;
//
//        For i As Integer = 0 To countInputNeurons - 1
//            For j As Integer = 0 To countHiddenNeurons - 1
//                ACopy.ihWeights(i, j, CURRENT) = ihWeights(i, j, CURRENT)
//                ACopy.ihWeights(i, j, PREVIOUS) = ihWeights(i, j, PREVIOUS)
//            Next
//        Next
	for(int i=0; i<=countInputNeurons-1; i++)
		for( int j=0; j<=countHiddenNeurons-1; j++)
		{
			ACopy->ihWeights[i][j][CURRENT] = ihWeights[i][j][CURRENT];
			ACopy->ihWeights[i][j][PREVIOUS] = ihWeights[i][j][PREVIOUS];
		}
//
//        If countHiddenLayers > 1 Then
//            For layer As Integer = 0 To countHiddenLayers - 2
//                For i As Integer = 0 To countHiddenNeurons - 1
//                    For j As Integer = 0 To countHiddenNeurons - 1
//                        ACopy.hhWeights(i, j, layer, CURRENT) = hhWeights(i, j, layer, CURRENT)
//                        ACopy.hhWeights(i, j, layer, PREVIOUS) = hhWeights(i, j, layer, PREVIOUS)
//                    Next
//                Next
//            Next
//        End If
		if(countHiddenLayers > 1)
			for(int layer=0; layer<=countHiddenLayers-2; layer++)
				for(int i=0; i<=countHiddenNeurons-1; i++)
					for(int j=0; j<=countHiddenNeurons-1; j++)
					{
						ACopy->hhWeights[layer][i][j][CURRENT] = hhWeights[layer][i][j][CURRENT];
						ACopy->hhWeights[layer][i][j][PREVIOUS] = hhWeights[layer][i][j][PREVIOUS];
					}
//
//        For i As Integer = 0 To countHiddenNeurons - 1
//            For j As Integer = 0 To countOutputNeurons - 1
//                ACopy.hoWeights(i, j, CURRENT) = hoWeights(i, j, CURRENT)
//                ACopy.hoWeights(i, j, PREVIOUS) = hoWeights(i, j, PREVIOUS)
//            Next
//        Next
		for( int i=0; i<=countHiddenNeurons-1; i++)
			for( int j=0; j<=countOutputNeurons-1; j++)
			{
				ACopy->hoWeights[i][j][CURRENT] = hoWeights[i][j][CURRENT];
				ACopy->hoWeights[i][j][PREVIOUS] = hoWeights[i][j][PREVIOUS];
			}
//
//        For layer As Integer = 0 To countHiddenLayers - 1
//            For i As Integer = 0 To countHiddenNeurons - 1
//                ACopy.hiddenValues(i, layer, BIAS) = hiddenValues(i, layer, BIAS)
//            Next
//        Next
		for( int layer=0; layer<=countHiddenLayers-1; layer++)
			for( int i=0; i<=countHiddenNeurons-1; i++)
				ACopy->hiddenValues[layer][i][BIAS] = hiddenValues[layer][i][BIAS];
//
//        For i As Integer = 0 To countOutputNeurons - 1
//            ACopy.outputValues(i, BIAS) = outputValues(i, BIAS)
//        Next
		for( int i=0; i<=countOutputNeurons-1; i++)
			ACopy->outputValues[i][BIAS] = outputValues[i][BIAS];
//
//        Return ACopy
	return *ACopy;
//    End Function
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


//Public Function Activation(ByVal value As Double) As Double
double Backprop::Activation(double value)
{
//        Return 1 / (1 + Math.Pow(e, -value))
	return (1 / (1 + pow(1.7,-value)));	//FIX THIS FOR E!!!! AGH!!!!!!!!!
//    End Function
}//Activation()

//
//    Public Function fPrime(ByVal u As Double) As Double
double Backprop::fPrime(double u)
{
//        Return u * (1 - u)
	return u * (1 - u);
//    End Function
}//fPrime()

//
//    Public Function RandomWeight(ByVal maxValue As Double) As Double
//        If maxValue > 1 Or maxValue < -1 Then
//            Return RandomNumber(maxValue - 1) + (RandomNumber(1000000) / 1000000)
//        Else    'Random Weight is between 1 and -1
//            Return (RandomNumber(1000000) / 1000000) * maxValue
//        End If
double Backprop::RandomWeight( double maxValue )
{
	if( maxValue > 1 || maxValue < -1)
		return RandomNumber(maxValue-1) + (RandomNumber(1000000) / 1000000);
	else
		return (double(RandomNumber(1000000)) / 1000000) * maxValue;
}//RandomWeight()
//
//    Public Function RandomNumber(ByVal MaxNumber As Integer) As Integer
//        Return r.Next(-MaxNumber, MaxNumber)
//    End Function
int Backprop::RandomNumber( int MaxNumber )
{
	return rand() % MaxNumber;
}
//
//    Public Function AbsVal(ByVal value As Double) As Double
double Backprop::AbsVal( double value )
{
//        If value < 0 Then
//            Return -value
//        Else
//            Return value
//        End If
	if( value < 0 )
		return -value;
	else
		return value;
//    End Function
}//AbsVal()

//
//    Public Function validInputFile(ByVal filePath As String) As Boolean
//        'For now just return true
//        Return True
//    End Function
//
//    Public Function numRowsInTextFile(ByVal filePath As String) As Integer
//        Dim sr As StreamReader = File.OpenText(filePath)
//        Dim rows As Integer = 0
//        Dim line As String
//
//        Do
//            line = sr.ReadLine
//            rows += 1
//        Loop Until sr.EndOfStream
//
//        Return rows
//
//    End Function
//
//    Public Function randomizeTheIndex(ByVal count As Integer) As Integer()
vector<int> Backprop::randomizeTheIndex(int count)
{
//        Dim randomArray(count - 1) As Integer
	vector<int> randomArray(count);
//
//        'Set initial values
//        For i As Integer = 0 To count - 1
//            randomArray(i) = i
//        Next
	//Set initial values
	for(int i=0; i<=count-1; i++)
		randomArray[i] = i;
//
//        'Randomize the values
//        Dim rand1, rand2, tempVal As Integer
//        For i As Integer = 0 To count - 1
//            rand1 = AbsVal(RandomNumber(count - 1))
//            rand2 = AbsVal(RandomNumber(count - 1))
//
//            tempVal = randomArray(rand1)
//            randomArray(rand1) = randomArray(rand2)
//            randomArray(rand2) = tempVal
//        Next
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
//
//        Return randomArray
	return randomArray;
//    End Function
}//randomizeTheIndex()

//
//End Module




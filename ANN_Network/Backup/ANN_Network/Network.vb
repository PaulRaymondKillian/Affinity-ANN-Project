Imports System.IO

Public Class Network
    Private _ClassName As String = "Network"
    Public ID As String = ""
    Public Layer() As Layer
    Public fitness As Double
    Public estimatedError As Double = -1
    Public currentCalcNeuron As Neuron
    Public previousCalcNeuron As Neuron
    Dim r As New Random(System.DateTime.Now.Millisecond)

    Public ReadOnly Property countInputNeurons(ByVal includeBiasNeurons As Boolean) As Integer
        Get
            If includeBiasNeurons = False Then
                Return Layer(0).Neuron.Count - 1
            Else
                Return Layer(0).Neuron.Count
            End If
        End Get
    End Property

    Public ReadOnly Property countHiddenNeurons(ByVal includeBiasNeurons As Boolean) As Integer
        Get
            If includeBiasNeurons = False Then
                Return Layer(1).Neuron.Count - 1
            Else
                Return Layer(1).Neuron.Count
            End If
        End Get
    End Property

    Public ReadOnly Property countOutputNeurons() As Integer
        Get
            Return Layer(Layer.Count - 1).Neuron.Count
        End Get
    End Property

    Public ReadOnly Property countHiddenLayers() As Integer
        Get
            Return Layer.Count - 2
        End Get
    End Property

    Public ReadOnly Property countTotalNeurons(ByVal includeBiasNeurons As Boolean) As Integer
        Get
            If includeBiasNeurons = False Then
                Return countInputNeurons(False) + (countHiddenNeurons(False) * countHiddenLayers) + countOutputNeurons
            Else
                Return countInputNeurons(True) + ((countHiddenNeurons(True)) * countHiddenLayers) + countOutputNeurons
            End If
        End Get
    End Property

    Public ReadOnly Property countWeights() As Integer
        Get
            Dim neuron As Neuron = Layer(1).Neuron(0)
            countWeights = 0
            Do Until neuron Is Nothing
                If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                    countWeights += neuron.belowDendrites.Count
                End If
                neuron = getNextNeuron(neuron)
            Loop
            Return countWeights
        End Get
    End Property

    Public ReadOnly Property getWeightTotal() As Double
        Get
            Dim neuron As Neuron = Layer(1).Neuron(0)
            getWeightTotal = 0
            Do Until neuron Is Nothing
                If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                    For i As Integer = 0 To neuron.belowDendrites.Count - 1
                        getWeightTotal += neuron.belowDendrites(i).value
                    Next
                End If
                neuron = getNextNeuron(neuron)
            Loop
            Return getWeightTotal
        End Get
    End Property

    Public ReadOnly Property getMaxWeightValue() As Double
        Get
            Dim neuron As Neuron = Layer(1).Neuron(0)
            getMaxWeightValue = -99999
            Do Until neuron Is Nothing
                If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                    For i As Integer = 0 To neuron.belowDendrites.Count - 1
                        If neuron.belowDendrites(i).value > getMaxWeightValue Then
                            getMaxWeightValue = neuron.belowDendrites(i).value
                        End If
                    Next
                End If
                neuron = getNextNeuron(neuron)
            Loop
            Return getMaxWeightValue
        End Get
    End Property

    Public ReadOnly Property getMinWeightValue() As Double
        Get
            Dim neuron As Neuron = Layer(1).Neuron(0)
            getMinWeightValue = 999999
            Do Until neuron Is Nothing
                If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                    For i As Integer = 0 To neuron.belowDendrites.Count - 1
                        If neuron.belowDendrites(i).value < getMinWeightValue Then
                            getMinWeightValue = neuron.belowDendrites(i).value
                        End If
                    Next
                End If
                neuron = getNextNeuron(neuron)
            Loop
            Return getMinWeightValue
        End Get
    End Property

    Public ReadOnly Property getCountZeroWeights(ByVal range As Double) As Integer
        Get
            Dim neuron As Neuron = Layer(1).Neuron(0)
            getCountZeroWeights = 0
            Do Until neuron Is Nothing
                If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                    For i As Integer = 0 To neuron.belowDendrites.Count - 1
                        If neuron.belowDendrites(i).value <= range And neuron.belowDendrites(i).value >= -range Then
                            getCountZeroWeights += 1
                        End If
                    Next
                End If
                neuron = getNextNeuron(neuron)
            Loop
            Return getCountZeroWeights
        End Get
    End Property

    Public ReadOnly Property getNetworkNeuronWidth() As Integer
        Get
            Dim maxWidth As Integer = 0
            For i As Integer = 0 To Layer.Count - 1
                If Layer(i).Neuron.Count > maxWidth Then
                    maxWidth = Layer(i).Neuron.Count
                End If
            Next
            Return maxWidth
        End Get
    End Property

    'Public Sub New(ByVal filePath As String)
    '    importNetworkFromFile(filePath)
    'End Sub

    Public Sub New(ByVal inputNeurons As Integer, ByVal hiddenNeurons As Integer, ByVal hiddenLayers As Integer, ByVal outputNeurons As Integer)
        Me.New(inputNeurons, hiddenNeurons, hiddenLayers, outputNeurons, False, 0)
    End Sub

    Public Sub New(ByVal inputNeurons As Integer, ByVal hiddenNeurons As Integer, ByVal hiddenLayers As Integer, ByVal outputNeurons As Integer, ByVal randomWeights As Boolean, ByVal randomizedMaxValue As Double)
        'Initialize the Layers of the network
        ReDim Layer(hiddenLayers + 1)
        For i As Integer = 0 To hiddenLayers + 1
            If i = 0 Then
                Layer(i) = New Layer(i, inputNeurons, hiddenNeurons, 0)
            ElseIf i = 1 And hiddenLayers = 1 Then
                Layer(i) = New Layer(i, hiddenNeurons, outputNeurons, inputNeurons)
            ElseIf i = 1 And hiddenLayers > 1 Then
                Layer(i) = New Layer(i, hiddenNeurons, hiddenNeurons, inputNeurons)
            ElseIf i = hiddenLayers And hiddenLayers <> 1 Then
                Layer(i) = New Layer(i, hiddenNeurons, outputNeurons, hiddenNeurons)
            ElseIf i = hiddenLayers + 1 Then
                Layer(i) = New Layer(i, outputNeurons, 0, hiddenNeurons)
            Else
                Layer(i) = New Layer(i, hiddenNeurons, hiddenNeurons, hiddenNeurons)
            End If
        Next

        'Link the layers together
        For i As Integer = 0 To Layer.Count - 2
            If i = Layer.Count - 2 Then
                linkLayers(Layer(i + 1), Layer(i), True)
            Else
                linkLayers(Layer(i + 1), Layer(i), False)
            End If

        Next

        currentCalcNeuron = Layer(1).Neuron(0)

        If randomWeights = True Then
            randomizeWeights(randomizedMaxValue)
        End If

    End Sub

    Private Sub linkLayers(ByVal aboveLayer As Layer, ByVal belowLayer As Layer, ByVal topLayers As Boolean)

        Dim aboveLayerCount As Integer = aboveLayer.Neuron.Count - 2
        Dim belowLayerCount As Integer = belowLayer.Neuron.Count - 1

        'If top layer, then need to add 1 to upper layer count since no bias exists
        If topLayers = True Then
            aboveLayerCount += 1
        End If

        For i As Integer = 0 To belowLayerCount
            For j As Integer = 0 To aboveLayerCount
                Dim tempDendrite As New Dendrite(aboveLayer.Neuron(j), belowLayer.Neuron(i))
                aboveLayer.Neuron(j).belowDendrites(i) = tempDendrite
                belowLayer.Neuron(i).upperDendrites(j) = tempDendrite
            Next
        Next
    End Sub


    Public Function getNeuronFromIndex(ByVal index As Integer) As Neuron
        Dim neuron As Neuron

        If index <= countInputNeurons(True) Then
            neuron = Layer(0).Neuron(index - 1)
        ElseIf index > countInputNeurons(True) + ((countHiddenNeurons(True)) * countHiddenLayers) Then
            neuron = Layer(Layer.Count - 1).Neuron(index - 1 - (countInputNeurons(True) + ((countHiddenNeurons(True)) * countHiddenLayers)))
        Else
            index -= (Layer(0).Neuron.Count + 1)
            Dim layerLevel As Integer = 0

            Do Until index < countHiddenNeurons(True)
                layerLevel += 1
                index -= (countHiddenNeurons(True))
            Loop

            neuron = Layer(layerLevel + 1).Neuron(index)
        End If

        Return neuron
    End Function

    Public Function getNeuronIndex(ByVal neuron As Neuron) As Integer
        Dim index As Integer = 0

        If neuron.layer = 0 Then
            index = neuron.index + 1
        ElseIf neuron.layer = countHiddenLayers + 1 Then
            index = countInputNeurons(True) + (countHiddenNeurons(True) * countHiddenLayers) + neuron.index + 1
        Else
            index = countInputNeurons(True) + (countHiddenNeurons(True) * (neuron.layer - 1)) + neuron.index + 1
        End If

        Return index
    End Function


    Public Function getNextNeuron(ByVal neuron As Neuron) As Neuron
        Dim _MethodName As String = "getNextNeuron(neuron)"

        Try
            Return getNeuronFromIndex(getNeuronIndex(neuron) + 1)
        Catch ex As Exception
            Return Nothing
        End Try
    End Function

    Public Sub calculateNextNeuron()

        If Not currentCalcNeuron Is Nothing Then
            If currentCalcNeuron.layer = countHiddenLayers + 1 Then
                'If output layer, do not squash calculation
                currentCalcNeuron.Calculate(False)
            Else
                'If hidden layer, squash the calculation
                currentCalcNeuron.Calculate(True)
            End If

            previousCalcNeuron = currentCalcNeuron
            currentCalcNeuron = getNextNeuron(currentCalcNeuron)

            'Check if next neuron is a bias neuron, if it is, skip it. We do not calculate bias neurons
            If Not currentCalcNeuron Is Nothing AndAlso currentCalcNeuron.type = Neuron.NeuronType.BiasNeuron Then
                currentCalcNeuron = getNextNeuron(currentCalcNeuron)
            End If

            If currentCalcNeuron Is Nothing Then
                currentCalcNeuron = Layer(1).Neuron(0)
                previousCalcNeuron = Nothing
            End If

        End If

    End Sub

    Public Sub calculateNextLayer()
        Dim count As Integer
        currentCalcNeuron = Layer(currentCalcNeuron.layer).Neuron(0)

        If currentCalcNeuron.layer = countHiddenLayers + 1 Then
            'Calculating Output Layer (No Bias Neuron)
            count = Layer(currentCalcNeuron.layer).Neuron.Count - 1
        Else
            'Calculating Hidden Layer
            count = Layer(currentCalcNeuron.layer).Neuron.Count - 2
        End If

        For i As Integer = 0 To count
            calculateNextNeuron()
        Next

    End Sub

    Public Sub calculateAll()
        currentCalcNeuron = Layer(1).Neuron(0)
        For i As Integer = 0 To Layer.Count - 1
            calculateNextLayer()
        Next
    End Sub

    Public Sub resetNetwork()
        Dim neuron As Neuron = Layer(1).Neuron(0)
        Do Until neuron Is Nothing
            If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                neuron.value = 0
            End If
            neuron = getNextNeuron(neuron)
        Loop

        currentCalcNeuron = Layer(1).Neuron(0)
        previousCalcNeuron = Nothing

    End Sub

    Public Sub randomizeWeights(ByVal maxValue As Double)
        Dim neuron As Neuron = Layer(1).Neuron(0)
        Dim randomValue As Double

        Do Until neuron Is Nothing
            If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                For i As Integer = 0 To neuron.belowDendrites.Count - 1
                    randomValue = RandomNumber(maxValue - 1) + ((RandomNumber(maxValue) / (1 + Math.Abs((RandomNumber(maxValue) * 100)))))
                    neuron.belowDendrites(i).value = randomValue
                Next
                neuron = getNextNeuron(neuron)
            Else
                neuron = getNextNeuron(neuron)
            End If
        Loop

    End Sub

    Public Function RandomNumber(ByVal MaxNumber As Integer) As Integer

        Return r.Next(-MaxNumber, MaxNumber)

    End Function


    Public Function importNetworkFromFile(ByVal filePath As String) As ANN_Network.Network
        Const _MethodName As String = "loadNetworkFromFile(String)"
        Dim fileCorrupt As Boolean = False
        Dim ANN As Network

        Try
            Dim sr As StreamReader = File.OpenText(filePath)
            Dim numInputNeurons, numHiddenNeurons, numHiddenLayers, numOutputNeurons As Integer
            Dim weightsTotal As Integer
            Dim weightsArray() As Double

            numInputNeurons = Convert.ToInt32(sr.ReadLine)
            numHiddenNeurons = Convert.ToInt32(sr.ReadLine)
            numHiddenLayers = Convert.ToInt32(sr.ReadLine)
            numOutputNeurons = Convert.ToInt32(sr.ReadLine)
            weightsTotal = Convert.ToInt32(sr.ReadLine)
            ReDim weightsArray(weightsTotal - 1)

            'Create weights array from file
            For i As Integer = 0 To weightsTotal - 1
                If sr.EndOfStream = False Then
                    weightsArray(i) = Convert.ToDouble(sr.ReadLine)
                Else
                    weightsArray(i) = 0
                    fileCorrupt = True
                End If
            Next

            'Initialize new network given file parameters
            ANN = New Network(numInputNeurons, numHiddenNeurons, numHiddenLayers, numOutputNeurons)
            'import weights from file to network
            ANN.importWeightsFromArray(weightsArray)

            If fileCorrupt = True Then
                Throw New Exception("Error - Class: " & _ClassName & ", Method: " & _MethodName & _
                                        "Descr: Input file corrupt/invalid/does not exist")
            End If

            Return ANN

        Catch ex As Exception
            Throw ex
        End Try
    End Function

    Public Sub exportNetworkToFile(ByVal filePath As String)

        Try
            Dim sw As StreamWriter = File.CreateText(filePath)
            sw.WriteLine(countInputNeurons(False))
            sw.WriteLine(countHiddenNeurons(False))
            sw.WriteLine(countHiddenLayers)
            sw.WriteLine(countOutputNeurons)
            sw.Close()

            exportWeightsToFile(filePath, False)

        Catch ex As Exception

        End Try

    End Sub

    Public Sub importWeightsFromFile(ByVal filePath As String)
        Const _MethodName As String = "InputWeightsFromFile(String)"
        Dim fileCorrupt As Boolean = False
        Dim weightsTotal As Integer
        Dim weightsArray() As Double

        Try
            Dim sr As StreamReader = File.OpenText(filePath)

            weightsTotal = Convert.ToInt32(sr.ReadLine)
            ReDim weightsArray(weightsTotal - 1)

            'Create weights array from file
            For i As Integer = 0 To weightsTotal - 1
                If sr.EndOfStream = False Then
                    weightsArray(i) = Convert.ToDouble(sr.ReadLine)
                Else
                    weightsArray(i) = 0
                    fileCorrupt = True
                End If
            Next

            If fileCorrupt = False Then
                importWeightsFromArray(weightsArray)
            Else
                Throw New Exception("Error - Class: " & _ClassName & ", Method: " & _MethodName & _
                                        "Descr: Input file corrupt/invalid/does not exist")
            End If

        Catch ex As Exception
            Throw ex
        End Try
    End Sub

    Public Sub importWeightsFromArray(ByVal weights() As Double)
        Const _MethodName As String = "InputWeightsFromArray(Double())"
        Dim index As Integer = 0

        Try
            If weights.Count = countWeights Then
                Dim neuron As Neuron = Layer(1).Neuron(0)
                Do Until neuron Is Nothing
                    If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                        For i As Integer = 0 To neuron.belowDendrites.Count - 1
                            neuron.belowDendrites(i).value = weights(index)
                            index += 1
                        Next
                        neuron = getNextNeuron(neuron)
                    Else
                        neuron = getNextNeuron(neuron)
                    End If

                Loop
            Else
                Throw New Exception("Error - Class: " & _ClassName & ", Method: " & _MethodName & _
                                        "Descr: Number of weights inputted in array invalid; Needed: " & countWeights & ", Inputted: " & weights.Count)
            End If
        Catch ex As Exception
            Throw ex
        End Try

    End Sub

    Public Function exportWeightsToArray() As Double()
        Dim WeightsArray(countWeights - 1) As Double
        Dim index As Integer = 0
        Dim neuron As Neuron = Layer(1).Neuron(0)

        Do Until neuron Is Nothing
            If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                For i As Integer = 0 To neuron.belowDendrites.Count - 1
                    WeightsArray(index) = neuron.belowDendrites(i).value
                    index += 1
                Next
                neuron = getNextNeuron(neuron)
            Else
                neuron = getNextNeuron(neuron)
            End If
        Loop
        Return WeightsArray
    End Function

    Public Sub exportWeightsToFile(ByVal filePath As String)
        exportWeightsToFile(filePath, True)
    End Sub

    Public Sub exportWeightsToFile(ByVal filePath As String, ByVal createNewFile As Boolean)
        Const _MethodName As String = "exportWeightsToFile(String)"
        Dim sw As StreamWriter
        Try
            If createNewFile = True Then
                sw = File.CreateText(filePath)
            Else
                sw = File.AppendText(filePath)
            End If

            Dim neuron As Neuron = Layer(1).Neuron(0)

            sw.WriteLine(countWeights)

            Do Until neuron Is Nothing
                If neuron.type = ANN_Network.Neuron.NeuronType.Neuron Then
                    For i As Integer = 0 To neuron.belowDendrites.Count - 1
                        sw.WriteLine(neuron.belowDendrites(i).value)
                    Next
                    neuron = getNextNeuron(neuron)
                Else
                    neuron = getNextNeuron(neuron)
                End If
            Loop

            sw.Close()

        Catch ex As Exception
            Throw ex
        End Try
    End Sub


    'Added 10/12/2011

    'Public Function GetDendrite(ByVal pLowerLayerIndex As Integer, ByVal pLowerNeuronIndex As Integer, _
    '                       ByVal pUpperLayerIndex As Integer, ByVal pUpperNeuronIndex As Integer) As Dendrite
    '    Dim Neuron1, Neuron2 As Neuron
    '    Neuron1 = getNeuronFromIndex(
    'End Function

    Public Function GetDendrite(ByVal pLowerNeuron As Neuron, ByVal pUpperNeuron As Neuron) As Dendrite
        Dim Dendrite As Dendrite
        For Each d As Dendrite In pLowerNeuron.upperDendrites
            If d.neuronAbove.Equals(pUpperNeuron) Then
                Dendrite = d
            End If
        Next
        If Dendrite IsNot Nothing Then
            Return Dendrite
        Else
            Throw New Exception("No pairing dendrite found for the two inputted neurons: " & pLowerNeuron.ID & ", " & pUpperNeuron.ID)
        End If
    End Function

End Class

Module Module1

    Sub Main()
        Console.WriteLine("ANN Backprop Learner Console v1.0")

        Console.Write("Input File Path: ")
        Dim inputFilePath As String = Console.ReadLine

        Console.Write("Hidden Neurons: ")
        Dim countHiddenNeurons As Integer = Console.ReadLine

        Console.Write("Hidden Layers: ")
        Dim countHiddenLayers As Integer = Console.ReadLine

        Console.Write("Max Weight Value: ")
        Dim weightsRange As Integer = Console.ReadLine

        Console.WriteLine("Creating Backprop and Network Objects...")
        Dim Backprop As New ANN_Backprop.ANN_Backprop(countHiddenNeurons, countHiddenLayers, inputFilePath, weightsRange)
        Console.WriteLine("Backprop and Network Objects Successfully Created!")

        Console.Write("Count Epochs to Execute: ")
        Dim countEpochs As Integer = Console.ReadLine

        Console.Write("Output Data Intervals: ")
        Dim dataInterval As Integer = Console.ReadLine

        Console.Write("Rho Value: ")
        Dim rho As Double = Console.ReadLine
        Backprop.rho = rho

        Console.WriteLine("Rho Halving Interval: ")
        Dim rhoHalvingInterval As Integer = Console.ReadLine

        Console.WriteLine()
        Console.WriteLine("Press Enter to Begin Backprop Network Learning Sequence...")
        Dim z As String = Console.ReadLine

        For i As Integer = 1 To countEpochs
            Backprop.TrainEpoch()
            If i Mod dataInterval = 0 Or i = 1 Then
                Console.WriteLine("Epoch: " & i & " Total Error: " & Backprop.networkError & "  Avg Error: " & Backprop.averageError)
            End If

            If i Mod rhoHalvingInterval = 0 Then
                Backprop.rho = Backprop.rho / 2
                Console.Write("Rho Value Halved: " & Backprop.rho & vbCrLf)
            End If

        Next

        Console.WriteLine("Save Resulting Network? (y/n)")
        If Console.ReadLine = "y" Then
            Console.WriteLine("FileName: ")
            Dim file As String = Console.ReadLine()
            Console.WriteLine("Saving Network File...")
            Backprop.ANN.exportNetworkToFile(file)
            Console.WriteLine("Network File Successfully Saved!")
        End If

        Console.WriteLine()
        Console.WriteLine("Please Press Enter to Exit...")
        z = Console.ReadLine

    End Sub

End Module

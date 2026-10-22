Module Module1

    Sub Main()
        Dim Network As New ANN_Network.Network(5, 5, 4, 5, True, 10)
        'Dim Network As New ANN_Network.Network("C:\z.txt")
        Network.calculateAll()

        Console.WriteLine(Network.countTotalNeurons(True))
        Console.WriteLine(Network.countWeights)
        Console.WriteLine(Network.getWeightTotal)
        Console.WriteLine(Network.getMaxWeightValue)
        Console.WriteLine(Network.getMinWeightValue)
        Console.WriteLine(Network.getCountZeroWeights(0.001))
        Console.WriteLine(Network.getNetworkNeuronWidth)
        Console.WriteLine()
        'For i As Integer = 1 To Network.countTotalNeurons(True)
        '    Console.WriteLine(Network.getNeuronFromIndex(i).ID)
        '    Console.WriteLine()
        'Next

        'Network.exportWeightsToFile("C:\weights.txt", True)
        'Network.InputWeightsFromFile("C:\weights.txt")
        Network.exportNetworkToFile("C:\network.txt")
        Network = Network.importNetworkFromFile("C:\network.txt")
        Network.exportNetworkToFile("C:\network2.txt")

        Dim neuron As ANN_Network.Neuron = Network.Layer(0).Neuron(0)
        For i As Integer = 1 To Network.countTotalNeurons(True)
            Console.WriteLine(neuron.ID)
            If i < Network.countTotalNeurons(True) Then
                neuron = Network.getNextNeuron(neuron)
            End If
        Next

        Console.WriteLine("Please Press Enter to Exit Program...")
        Dim z As String = Console.ReadLine

    End Sub

End Module

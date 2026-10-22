Public Class Layer
    Public Neuron() As Neuron

    Public Sub New(ByVal layerLevel As Integer, ByVal countNeurons As Integer, ByVal neuronsAbove As Integer, ByVal neuronsBelow As Integer)
        'Initialize the Neurons of the layer
        If neuronsAbove = 0 Then    'Check if layer is an output layer
            'Output layers do not have a bias neuron
            ReDim Neuron(countNeurons - 1)
            For i As Integer = 0 To countNeurons - 1
                Neuron(i) = New Neuron(layerLevel, i, ANN_Network.Neuron.NeuronType.Neuron, neuronsAbove, neuronsBelow)
            Next
        Else
            'Non-output layers have a bias node, so add one more neuron to the end of the layer and make it a bias neuron
            ReDim Neuron(countNeurons)
            For i As Integer = 0 To countNeurons
                If i = countNeurons Then
                    Neuron(i) = New Neuron(layerLevel, i, ANN_Network.Neuron.NeuronType.BiasNeuron, neuronsAbove, 0)
                Else
                    Neuron(i) = New Neuron(layerLevel, i, ANN_Network.Neuron.NeuronType.Neuron, neuronsAbove, neuronsBelow)
                End If
            Next
        End If

    End Sub

End Class

Public Class Dendrite
    Public value As Double
    Public neuronAbove As Neuron
    Public neuronBelow As Neuron

    Public Sub New(ByVal above As Neuron, ByVal below As Neuron)
        neuronAbove = above
        neuronBelow = below
        value = 0
    End Sub
End Class

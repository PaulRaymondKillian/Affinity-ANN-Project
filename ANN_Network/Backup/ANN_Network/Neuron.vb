Public Class Neuron
    Const e As Double = 2.71828183

    Private _value As Double
    Private _type As NeuronType
    Private _index As Integer
    Private _layer As Integer
    Private _delta As Double

    Public upperDendrites() As Dendrite
    Public belowDendrites() As Dendrite

    Public Enum NeuronType As Integer
        Neuron = 0
        BiasNeuron = 1
    End Enum

    Public Property value() As Double
        Get
            Return _value
        End Get
        Set(ByVal value As Double)
            _value = value
        End Set
    End Property

    Public ReadOnly Property type() As NeuronType
        Get
            Return _type
        End Get
    End Property

    Public ReadOnly Property ID() As String
        Get
            If type = NeuronType.Neuron Then
                Return layer & "-" & index
            Else
                Return layer & "-B" & index
            End If
        End Get
    End Property

    Public ReadOnly Property index() As Integer
        Get
            Return _index
        End Get
    End Property

    Public ReadOnly Property layer() As Integer
        Get
            Return _layer
        End Get
    End Property


    Public Property delta() As Double
        Get
            Return _delta
        End Get
        Set(ByVal value As Double)
            _delta = value
        End Set
    End Property

    Public Sub New(ByVal layer As String, ByVal index As String, ByVal type As NeuronType, ByVal countNeuronsAbove As Integer, ByVal countNeuronsBelow As Integer)
        ReDim upperDendrites(countNeuronsAbove - 1)

        If type = NeuronType.Neuron Then
            ReDim belowDendrites(countNeuronsBelow)
            _value = 0
        Else
            'It is a Bias Neuron, set value to 1
            _value = 1
        End If

        _layer = layer
        _index = index
        _type = type

    End Sub

    Public Sub linkBelowLayer(ByVal belowLayer As Layer)
        For i As Integer = 0 To belowLayer.Neuron.Count - 1
            belowDendrites(i) = New Dendrite(Me, belowLayer.Neuron(i))
        Next
    End Sub

    Public Sub resetValue()
        value = 0
    End Sub

    Public Function getNeuronInfo() As String
        Dim s As String

        s = ID & "   Type: " & _type & "   Value: " & _value & vbCrLf

        s = s & vbCrLf & "#########################" & vbCrLf

        Return s
    End Function

    Public Sub Calculate(ByVal Sigmoid As Boolean)
        Dim sum As Double = 0

        For i As Integer = 0 To belowDendrites.Count - 1
            sum += belowDendrites(i).value * belowDendrites(i).neuronBelow.value
        Next

        If Sigmoid = True Then
            value = Activation(sum)
        Else
            value = sum
        End If

    End Sub

    'Squash the output between 0 and 1
    Public Function Activation(ByVal sum As Double)
        If sum > 100 Then
            Return 1
        ElseIf sum < -100 Then
            Return 0
        Else
            Return 1 / (1 + Math.Pow(e, -sum))
        End If
    End Function

    Public Function isOutputNeuron() As Boolean
        If upperDendrites.Count = 0 Then
            Return True
        Else
            Return False
        End If
    End Function

End Class

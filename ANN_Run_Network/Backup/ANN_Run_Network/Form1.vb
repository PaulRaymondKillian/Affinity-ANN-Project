Public Class Form1

    Protected Friend Network As ANN_Network.Network
    Dim MyHashTable As New Hashtable

    Private Sub btnLoadNetwork_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnLoadNetwork.Click

        OpenFileDialog1.ShowDialog()
        txtNetworkFilePath.Text = OpenFileDialog1.FileName

        If txtNetworkFilePath.Text <> "" Then
            Try
                Dim ANN As New ANN_Network.Network(1, 1, 1, 1)
                setNetwork(ANN.importNetworkFromFile(txtNetworkFilePath.Text))

            Catch ex As Exception
                MsgBox(ex.Message)
                Network = Nothing
            End Try
        End If

    End Sub

    Public Sub SetupInputValueBoxes()

        'Albiet weird, This for loop within a for loop is necessary to ensure every object in Controls is acted upon, due to odd circumstances when removing a control object
        For i As Integer = 0 To Me.Controls.Count - 1
            For Each Control In Me.Controls
                If InStr(Control.Name, "lblNeuron", CompareMethod.Text) Or InStr(Control.Name, "txtNeuron", CompareMethod.Text) Then
                    Me.Controls.Remove(Control)
                End If
            Next
        Next


        Dim MyHashTable2 As New Hashtable
        MyHashTable = MyHashTable2

        Dim x As Integer = 10
        Dim y As Integer = 100

        For i As Integer = 0 To Network.countInputNeurons(False) - 1
            Dim MyText As New TextBox
            Dim MyLabel As New Label

            MyText.Text = Network.getNeuronFromIndex(i + 1).value
            MyLabel.Text = "Input Neuron " & i & ":"

            MyText.Name = "txtNeuron" & i
            MyLabel.Name = "lblNeuron" & i

            MyText.Location = New Point(x + 100, y + (25 * i) - 3)
            MyLabel.Location = New Point(x, y + (25 * i))

            MyText.Size = New Size(50, 20)

            Me.Controls.Add(MyText)
            Me.Controls.Add(MyLabel)
            Me.MyHashTable.Add("txtNeuron" & i, "txtNeuron" & i)
            'Me.MyHashTable.Add("lblNeuron" & i, "lblNeuron" & i)
        Next

        'Set max height to 700 so it can all fit on the screen
        If (y + (25 * Me.MyHashTable.Count) > 325) And (y + (25 * Me.MyHashTable.Count) < 700) Then
            Me.Height = y + (25 * Me.MyHashTable.Count)
        ElseIf y + (25 * Me.MyHashTable.Count) >= 700 Then
            Me.Height = 700
            MsgBox("Error - Number of input neurons is too many to display properly!")
        End If

    End Sub

    Private Sub btnCalculateNetwork_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnCalculateNetwork.Click
        Dim i As Integer = 1

        Dim MyControl As Control

        Dim MyTextBox As TextBox

        For Each MyControl In Me.Controls

            If TypeOf (MyControl) Is TextBox Then

                MyTextBox = MyControl

                If Me.MyHashTable.Contains(MyTextBox.Name) Then

                    If IsNumeric(MyTextBox.Text) Then
                        Network.getNeuronFromIndex(i).value = Convert.ToDouble(MyTextBox.Text)
                        i += 1
                    Else
                        Throw New Exception("Error - Class: frmSetInputValues, Method: Button1_Click, Desc: " & MyTextBox.Name & "=" & MyTextBox.Text & " is not a valid input value (numbers only)")
                    End If

                End If

            End If
        Next

        Network.calculateAll()

        For j As Integer = 0 To Network.countOutputNeurons - 1
            If j = 0 Then
                txtOutput.Text = Network.Layer(Network.countHiddenLayers + 1).Neuron(j).value
            Else
                txtOutput.Text = txtOutput.Text & ", " & Network.Layer(Network.countHiddenLayers + 1).Neuron(j).value
            End If
        Next

    End Sub


    Public Sub setNetwork(ByVal ANN As ANN_Network.Network)
        Network = ANN
        If txtNetworkFilePath.Text = "" Then
            txtNetworkFilePath.Text = "...Network Loaded From Memory..."
        End If

        'Setup Input Text Boxes
        SetupInputValueBoxes()

        'Set Data Labels
        lblCountInputNeurons.Text = "Input Neurons: " & Network.countInputNeurons(False)
        lblCountHiddenNeurons.Text = "Hidden Neurons: " & Network.countHiddenNeurons(False)
        lblCountHiddenLayers.Text = "Hidden Layers: " & Network.countHiddenLayers
        lblCountOutputNeurons.Text = "Output Neurons: " & Network.countOutputNeurons
        lblEstimatedError.Text = "Estimated Error: " & Network.estimatedError
        lblWeightSum.Text = "Weight Sum: " & Network.getWeightTotal
        lblMaxWeight.Text = "Max Weight: " & Network.getMaxWeightValue
        lblMinWeight.Text = "Min Weight: " & Network.getMinWeightValue
        lblZeroWeights.Text = "Zero Weights: " & Network.getCountZeroWeights(0.001)

        'reset output text
        txtOutput.Text = ""

    End Sub
End Class

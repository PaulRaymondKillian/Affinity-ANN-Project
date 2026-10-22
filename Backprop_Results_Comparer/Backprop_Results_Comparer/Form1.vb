Imports System.IO

Public Class Form1

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnResultsFile.Click
        If OpenFileDialog1.ShowDialog = Windows.Forms.DialogResult.OK Then
            txtResultsFile.Text = OpenFileDialog1.FileName
        End If
    End Sub

    Private Sub btnDataFile_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnDataFile.Click
        If OpenFileDialog2.ShowDialog = Windows.Forms.DialogResult.OK Then
            txtDataFile.Text = OpenFileDialog2.FileName
        End If
    End Sub

    Private Sub btnCompare_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnCompare.Click
        CompareFiles(txtResultsFile.Text, txtDataFile.Text)
    End Sub

    Public Sub CompareFiles(ByVal resultsFile As String, ByVal dataFile As String)
        Dim resultsArray() As Double
        Dim dataArray() As Double
        Dim totalRows As Integer
        Dim line As String
        Dim readLine() As String
        Dim currentDifference As Double
        Dim totalDifference As Double = 0

        If CountRows(resultsFile, True) = CountRows(dataFile, False) Then
            totalRows = CountRows(dataFile, False)
        End If

        ReDim resultsArray(totalRows - 1)
        ReDim dataArray(totalRows - 1)

        'Read in results File
        Dim sr As StreamReader
        sr = File.OpenText(resultsFile)
        sr.ReadLine() 'Ignore first line, since it includes buffer information data and not results
        For i As Integer = 0 To totalRows - 1
            line = sr.ReadLine
            readLine = line.Split("="c)
            If readLine.Count <> 2 Then
                Throw New Exception("Error - Descr: Invalid contents of input file. Inputs should be delimited by a single '=' character")
                Exit Sub
            End If
            resultsArray(i) = Convert.ToDouble((readLine(1)))
        Next

        'Read in data file
        sr = Nothing
        sr = File.OpenText(dataFile)
        For i As Integer = 0 To totalRows - 1
            line = sr.ReadLine
            readLine = line.Split("="c)
            If readLine.Count <> 2 Then
                Throw New Exception("Error - Descr: Invalid contents of input file. Inputs should be delimited by a single '=' character")
                Exit Sub
            End If
            dataArray(i) = Convert.ToDouble((readLine(1)))
        Next

        'load results data to list box
        For i As Integer = 0 To totalRows - 1
            listResults.Items.Add(resultsArray(i))
        Next

        'load data data to list box
        For i As Integer = 0 To totalRows - 1
            listData.Items.Add(dataArray(i))
        Next

        'load difference to list box
        For i As Integer = 0 To totalRows - 1
            currentDifference = AbsVal(resultsArray(i) - dataArray(i))
            totalDifference += currentDifference
            listDifference.Items.Add(currentDifference)
        Next
        txtTotalDifference.Text = totalDifference


    End Sub

    Public Function CountRows(ByVal fileName As String, ByVal resultsFile As Boolean) As Integer
        Dim rows As Integer = 0
        Dim sr As StreamReader
        sr = File.OpenText(fileName)

        Do Until sr.EndOfStream
            sr.ReadLine()
            rows += 1
        Loop

        'Because the first line of the results file template is an information buffer, do not count it.
        If resultsFile Then
            rows -= 1
        End If

        Return rows
    End Function

    Public Function AbsVal(ByVal value As Double) As Double
        If value < 0 Then
            Return -value
        Else
            Return value
        End If
    End Function

End Class

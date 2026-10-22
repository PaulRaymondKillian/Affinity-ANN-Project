<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.txtResultsFile = New System.Windows.Forms.TextBox
        Me.txtDataFile = New System.Windows.Forms.TextBox
        Me.btnResultsFile = New System.Windows.Forms.Button
        Me.btnDataFile = New System.Windows.Forms.Button
        Me.OpenFileDialog1 = New System.Windows.Forms.OpenFileDialog
        Me.OpenFileDialog2 = New System.Windows.Forms.OpenFileDialog
        Me.Label1 = New System.Windows.Forms.Label
        Me.Label2 = New System.Windows.Forms.Label
        Me.btnCompare = New System.Windows.Forms.Button
        Me.listResults = New System.Windows.Forms.ListBox
        Me.listData = New System.Windows.Forms.ListBox
        Me.listDifference = New System.Windows.Forms.ListBox
        Me.Label3 = New System.Windows.Forms.Label
        Me.txtTotalDifference = New System.Windows.Forms.TextBox
        Me.SuspendLayout()
        '
        'txtResultsFile
        '
        Me.txtResultsFile.Location = New System.Drawing.Point(82, 12)
        Me.txtResultsFile.Name = "txtResultsFile"
        Me.txtResultsFile.Size = New System.Drawing.Size(414, 20)
        Me.txtResultsFile.TabIndex = 0
        '
        'txtDataFile
        '
        Me.txtDataFile.Location = New System.Drawing.Point(82, 38)
        Me.txtDataFile.Name = "txtDataFile"
        Me.txtDataFile.Size = New System.Drawing.Size(414, 20)
        Me.txtDataFile.TabIndex = 1
        '
        'btnResultsFile
        '
        Me.btnResultsFile.Location = New System.Drawing.Point(502, 13)
        Me.btnResultsFile.Name = "btnResultsFile"
        Me.btnResultsFile.Size = New System.Drawing.Size(86, 19)
        Me.btnResultsFile.TabIndex = 2
        Me.btnResultsFile.Text = "Browse"
        Me.btnResultsFile.UseVisualStyleBackColor = True
        '
        'btnDataFile
        '
        Me.btnDataFile.Location = New System.Drawing.Point(502, 39)
        Me.btnDataFile.Name = "btnDataFile"
        Me.btnDataFile.Size = New System.Drawing.Size(86, 19)
        Me.btnDataFile.TabIndex = 3
        Me.btnDataFile.Text = "Browse"
        Me.btnDataFile.UseVisualStyleBackColor = True
        '
        'OpenFileDialog1
        '
        Me.OpenFileDialog1.FileName = "OpenFileDialog1"
        '
        'OpenFileDialog2
        '
        Me.OpenFileDialog2.FileName = "OpenFileDialog2"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(12, 15)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(64, 13)
        Me.Label1.TabIndex = 4
        Me.Label1.Text = "Results File:"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(24, 41)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(52, 13)
        Me.Label2.TabIndex = 5
        Me.Label2.Text = "Data File:"
        '
        'btnCompare
        '
        Me.btnCompare.Location = New System.Drawing.Point(208, 64)
        Me.btnCompare.Name = "btnCompare"
        Me.btnCompare.Size = New System.Drawing.Size(187, 32)
        Me.btnCompare.TabIndex = 6
        Me.btnCompare.Text = "Compare"
        Me.btnCompare.UseVisualStyleBackColor = True
        '
        'listResults
        '
        Me.listResults.FormattingEnabled = True
        Me.listResults.Location = New System.Drawing.Point(15, 112)
        Me.listResults.Name = "listResults"
        Me.listResults.Size = New System.Drawing.Size(187, 251)
        Me.listResults.TabIndex = 7
        '
        'listData
        '
        Me.listData.FormattingEnabled = True
        Me.listData.Location = New System.Drawing.Point(208, 112)
        Me.listData.Name = "listData"
        Me.listData.Size = New System.Drawing.Size(187, 251)
        Me.listData.TabIndex = 8
        '
        'listDifference
        '
        Me.listDifference.FormattingEnabled = True
        Me.listDifference.Location = New System.Drawing.Point(401, 112)
        Me.listDifference.Name = "listDifference"
        Me.listDifference.Size = New System.Drawing.Size(187, 251)
        Me.listDifference.TabIndex = 9
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(398, 366)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(86, 13)
        Me.Label3.TabIndex = 10
        Me.Label3.Text = "Total Difference:"
        '
        'txtTotalDifference
        '
        Me.txtTotalDifference.BackColor = System.Drawing.SystemColors.Control
        Me.txtTotalDifference.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.txtTotalDifference.Location = New System.Drawing.Point(480, 366)
        Me.txtTotalDifference.Name = "txtTotalDifference"
        Me.txtTotalDifference.Size = New System.Drawing.Size(116, 13)
        Me.txtTotalDifference.TabIndex = 11
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(607, 389)
        Me.Controls.Add(Me.txtTotalDifference)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.listDifference)
        Me.Controls.Add(Me.listData)
        Me.Controls.Add(Me.listResults)
        Me.Controls.Add(Me.btnCompare)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.btnDataFile)
        Me.Controls.Add(Me.btnResultsFile)
        Me.Controls.Add(Me.txtDataFile)
        Me.Controls.Add(Me.txtResultsFile)
        Me.Name = "Form1"
        Me.Text = "Backprop Results Comparer"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents txtResultsFile As System.Windows.Forms.TextBox
    Friend WithEvents txtDataFile As System.Windows.Forms.TextBox
    Friend WithEvents btnResultsFile As System.Windows.Forms.Button
    Friend WithEvents btnDataFile As System.Windows.Forms.Button
    Friend WithEvents OpenFileDialog1 As System.Windows.Forms.OpenFileDialog
    Friend WithEvents OpenFileDialog2 As System.Windows.Forms.OpenFileDialog
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents btnCompare As System.Windows.Forms.Button
    Friend WithEvents listResults As System.Windows.Forms.ListBox
    Friend WithEvents listData As System.Windows.Forms.ListBox
    Friend WithEvents listDifference As System.Windows.Forms.ListBox
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents txtTotalDifference As System.Windows.Forms.TextBox

End Class

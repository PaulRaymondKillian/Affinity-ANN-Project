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
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip
        Me.FileToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
        Me.btnLoadNetwork = New System.Windows.Forms.Button
        Me.OpenFileDialog1 = New System.Windows.Forms.OpenFileDialog
        Me.txtNetworkFilePath = New System.Windows.Forms.TextBox
        Me.GroupBox1 = New System.Windows.Forms.GroupBox
        Me.lblZeroWeights = New System.Windows.Forms.Label
        Me.lblMinWeight = New System.Windows.Forms.Label
        Me.lblMaxWeight = New System.Windows.Forms.Label
        Me.lblWeightSum = New System.Windows.Forms.Label
        Me.lblEstimatedError = New System.Windows.Forms.Label
        Me.lblCountOutputNeurons = New System.Windows.Forms.Label
        Me.lblCountHiddenLayers = New System.Windows.Forms.Label
        Me.lblCountHiddenNeurons = New System.Windows.Forms.Label
        Me.lblCountInputNeurons = New System.Windows.Forms.Label
        Me.btnCalculateNetwork = New System.Windows.Forms.Button
        Me.txtOutput = New System.Windows.Forms.TextBox
        Me.Label1 = New System.Windows.Forms.Label
        Me.Label2 = New System.Windows.Forms.Label
        Me.MenuStrip1.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.SuspendLayout()
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 325)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Size = New System.Drawing.Size(536, 22)
        Me.StatusStrip1.TabIndex = 0
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'MenuStrip1
        '
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.FileToolStripMenuItem})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Size = New System.Drawing.Size(536, 24)
        Me.MenuStrip1.TabIndex = 1
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'FileToolStripMenuItem
        '
        Me.FileToolStripMenuItem.Name = "FileToolStripMenuItem"
        Me.FileToolStripMenuItem.Size = New System.Drawing.Size(35, 20)
        Me.FileToolStripMenuItem.Text = "&File"
        '
        'btnLoadNetwork
        '
        Me.btnLoadNetwork.Location = New System.Drawing.Point(426, 30)
        Me.btnLoadNetwork.Name = "btnLoadNetwork"
        Me.btnLoadNetwork.Size = New System.Drawing.Size(98, 23)
        Me.btnLoadNetwork.TabIndex = 2
        Me.btnLoadNetwork.Text = "Load Network"
        Me.btnLoadNetwork.UseVisualStyleBackColor = True
        '
        'OpenFileDialog1
        '
        Me.OpenFileDialog1.FileName = "OpenFileDialog1"
        '
        'txtNetworkFilePath
        '
        Me.txtNetworkFilePath.Location = New System.Drawing.Point(12, 33)
        Me.txtNetworkFilePath.Name = "txtNetworkFilePath"
        Me.txtNetworkFilePath.Size = New System.Drawing.Size(408, 20)
        Me.txtNetworkFilePath.TabIndex = 3
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.lblZeroWeights)
        Me.GroupBox1.Controls.Add(Me.lblMinWeight)
        Me.GroupBox1.Controls.Add(Me.lblMaxWeight)
        Me.GroupBox1.Controls.Add(Me.lblWeightSum)
        Me.GroupBox1.Controls.Add(Me.lblEstimatedError)
        Me.GroupBox1.Controls.Add(Me.lblCountOutputNeurons)
        Me.GroupBox1.Controls.Add(Me.lblCountHiddenLayers)
        Me.GroupBox1.Controls.Add(Me.lblCountHiddenNeurons)
        Me.GroupBox1.Controls.Add(Me.lblCountInputNeurons)
        Me.GroupBox1.Location = New System.Drawing.Point(324, 59)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(200, 184)
        Me.GroupBox1.TabIndex = 4
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Network Information"
        '
        'lblZeroWeights
        '
        Me.lblZeroWeights.AutoSize = True
        Me.lblZeroWeights.Location = New System.Drawing.Point(12, 163)
        Me.lblZeroWeights.Name = "lblZeroWeights"
        Me.lblZeroWeights.Size = New System.Drawing.Size(74, 13)
        Me.lblZeroWeights.TabIndex = 8
        Me.lblZeroWeights.Text = "Zero Weights:"
        '
        'lblMinWeight
        '
        Me.lblMinWeight.AutoSize = True
        Me.lblMinWeight.Location = New System.Drawing.Point(12, 137)
        Me.lblMinWeight.Name = "lblMinWeight"
        Me.lblMinWeight.Size = New System.Drawing.Size(64, 13)
        Me.lblMinWeight.TabIndex = 7
        Me.lblMinWeight.Text = "Min Weight:"
        '
        'lblMaxWeight
        '
        Me.lblMaxWeight.AutoSize = True
        Me.lblMaxWeight.Location = New System.Drawing.Point(11, 150)
        Me.lblMaxWeight.Name = "lblMaxWeight"
        Me.lblMaxWeight.Size = New System.Drawing.Size(67, 13)
        Me.lblMaxWeight.TabIndex = 6
        Me.lblMaxWeight.Text = "Max Weight:"
        '
        'lblWeightSum
        '
        Me.lblWeightSum.AutoSize = True
        Me.lblWeightSum.Location = New System.Drawing.Point(12, 124)
        Me.lblWeightSum.Name = "lblWeightSum"
        Me.lblWeightSum.Size = New System.Drawing.Size(68, 13)
        Me.lblWeightSum.TabIndex = 5
        Me.lblWeightSum.Text = "Weight Sum:"
        '
        'lblEstimatedError
        '
        Me.lblEstimatedError.AutoSize = True
        Me.lblEstimatedError.Location = New System.Drawing.Point(12, 111)
        Me.lblEstimatedError.Name = "lblEstimatedError"
        Me.lblEstimatedError.Size = New System.Drawing.Size(81, 13)
        Me.lblEstimatedError.TabIndex = 4
        Me.lblEstimatedError.Text = "Estimated Error:"
        '
        'lblCountOutputNeurons
        '
        Me.lblCountOutputNeurons.AutoSize = True
        Me.lblCountOutputNeurons.Location = New System.Drawing.Point(12, 76)
        Me.lblCountOutputNeurons.Name = "lblCountOutputNeurons"
        Me.lblCountOutputNeurons.Size = New System.Drawing.Size(85, 13)
        Me.lblCountOutputNeurons.TabIndex = 3
        Me.lblCountOutputNeurons.Text = "Output Neurons:"
        '
        'lblCountHiddenLayers
        '
        Me.lblCountHiddenLayers.AutoSize = True
        Me.lblCountHiddenLayers.Location = New System.Drawing.Point(12, 63)
        Me.lblCountHiddenLayers.Name = "lblCountHiddenLayers"
        Me.lblCountHiddenLayers.Size = New System.Drawing.Size(78, 13)
        Me.lblCountHiddenLayers.TabIndex = 2
        Me.lblCountHiddenLayers.Text = "Hidden Layers:"
        '
        'lblCountHiddenNeurons
        '
        Me.lblCountHiddenNeurons.AutoSize = True
        Me.lblCountHiddenNeurons.Location = New System.Drawing.Point(12, 50)
        Me.lblCountHiddenNeurons.Name = "lblCountHiddenNeurons"
        Me.lblCountHiddenNeurons.Size = New System.Drawing.Size(87, 13)
        Me.lblCountHiddenNeurons.TabIndex = 1
        Me.lblCountHiddenNeurons.Text = "Hidden Neurons:"
        '
        'lblCountInputNeurons
        '
        Me.lblCountInputNeurons.AutoSize = True
        Me.lblCountInputNeurons.Location = New System.Drawing.Point(12, 37)
        Me.lblCountInputNeurons.Name = "lblCountInputNeurons"
        Me.lblCountInputNeurons.Size = New System.Drawing.Size(77, 13)
        Me.lblCountInputNeurons.TabIndex = 0
        Me.lblCountInputNeurons.Text = "Input Neurons:"
        '
        'btnCalculateNetwork
        '
        Me.btnCalculateNetwork.Location = New System.Drawing.Point(373, 271)
        Me.btnCalculateNetwork.Name = "btnCalculateNetwork"
        Me.btnCalculateNetwork.Size = New System.Drawing.Size(110, 28)
        Me.btnCalculateNetwork.TabIndex = 5
        Me.btnCalculateNetwork.Text = "Calculate Network"
        Me.btnCalculateNetwork.UseVisualStyleBackColor = True
        '
        'txtOutput
        '
        Me.txtOutput.BackColor = System.Drawing.Color.White
        Me.txtOutput.Location = New System.Drawing.Point(180, 93)
        Me.txtOutput.Multiline = True
        Me.txtOutput.Name = "txtOutput"
        Me.txtOutput.ReadOnly = True
        Me.txtOutput.Size = New System.Drawing.Size(137, 229)
        Me.txtOutput.TabIndex = 7
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(12, 62)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(98, 13)
        Me.Label1.TabIndex = 8
        Me.Label1.Text = "Input Neuron Data:"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(177, 62)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(118, 13)
        Me.Label2.TabIndex = 9
        Me.Label2.Text = "Output Neuron Results:"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(536, 347)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.txtOutput)
        Me.Controls.Add(Me.btnCalculateNetwork)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.txtNetworkFilePath)
        Me.Controls.Add(Me.btnLoadNetwork)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Controls.Add(Me.MenuStrip1)
        Me.MainMenuStrip = Me.MenuStrip1
        Me.Name = "Form1"
        Me.Text = "ANN Run Network"
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents StatusStrip1 As System.Windows.Forms.StatusStrip
    Friend WithEvents MenuStrip1 As System.Windows.Forms.MenuStrip
    Friend WithEvents FileToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents btnLoadNetwork As System.Windows.Forms.Button
    Friend WithEvents OpenFileDialog1 As System.Windows.Forms.OpenFileDialog
    Friend WithEvents txtNetworkFilePath As System.Windows.Forms.TextBox
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents lblCountOutputNeurons As System.Windows.Forms.Label
    Friend WithEvents lblCountHiddenLayers As System.Windows.Forms.Label
    Friend WithEvents lblCountHiddenNeurons As System.Windows.Forms.Label
    Friend WithEvents lblCountInputNeurons As System.Windows.Forms.Label
    Friend WithEvents lblZeroWeights As System.Windows.Forms.Label
    Friend WithEvents lblMinWeight As System.Windows.Forms.Label
    Friend WithEvents lblMaxWeight As System.Windows.Forms.Label
    Friend WithEvents lblWeightSum As System.Windows.Forms.Label
    Friend WithEvents lblEstimatedError As System.Windows.Forms.Label
    Friend WithEvents btnCalculateNetwork As System.Windows.Forms.Button
    Friend WithEvents txtOutput As System.Windows.Forms.TextBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label

End Class

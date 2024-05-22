VERSION 5.00
Object = "{0842D103-1E19-101B-9AAF-1A1626551E7C}#1.0#0"; "GRAPH32.OCX"
Begin VB.Form Form1
   Caption         =   "SigLib DLL Test"
   ClientHeight    =   7230
   ClientLeft      =   1140
   ClientTop       =   1515
   ClientWidth     =   9030
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   7230
   ScaleWidth      =   9030
   Begin GraphLib.Graph SigLibData
      Height          =   4815
      Left            =   1080
      TabIndex        =   0
      Top             =   960
      Width           =   6495
      _Version        =   65536
      _ExtentX        =   11456
      _ExtentY        =   8493
      _StockProps     =   96
      BorderStyle     =   1
      GraphType       =   6
      LabelEvery      =   100
      PatternedLines  =   1
      RandomData      =   1
      ThickLines      =   0
      YAxisTicks      =   10
      ColorData       =   0
      ExtraData       =   0
      ExtraData[]     =   0
      FontFamily      =   4
      FontSize        =   4
      FontSize[0]     =   200
      FontSize[1]     =   150
      FontSize[2]     =   100
      FontSize[3]     =   100
      FontStyle       =   4
      GraphData       =   0
      GraphData[]     =   0
      LabelText       =   0
      LegendText      =   0
      PatternData     =   0
      SymbolData      =   0
      XPosData        =   0
      XPosData[]      =   0
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

x	Private Sub Form_Load()
    Static Buffer(512) As Double
'
    Length& = 512
    SignalPhase# = 0#
    SignalMagn# = 0.9
    SignalFreq# = 0.01562
    SignalOffset# = 0#
    Zero# = 0#
    Dummy& = 0

    fDummy# = SUF_SiglibVersion()

    Dummy& = SDA_SignalGenerate(Buffer(0), SINE_WAVE, SignalMagn#, SIG_FILL, SignalFreq#, SignalOffset#, Zero#, Zero#, SignalPhase#, 0, Length&)
    SigLibData.ThisSet = 1
    SigLibData.NumPoints = Length&
    SigLibData.AutoInc = 1
    For I% = 1 To Length&
        SigLibData.GraphData = Buffer(I%)
    Next I%
End Sub


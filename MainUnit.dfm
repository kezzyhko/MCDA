object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = #1050#1086#1084#1087#1100#1102#1090#1077#1088#1085#1072#1103' '#1089#1080#1089#1090#1077#1084#1072' '#1084#1085#1086#1075#1086#1082#1088#1080#1090#1077#1088#1080#1072#1083#1100#1085#1086#1075#1086' '#1072#1085#1072#1083#1080#1079#1072
  ClientHeight = 455
  ClientWidth = 833
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object States: TPageControl
    Tag = -2
    Left = 0
    Top = 0
    Width = 833
    Height = 438
    ActivePage = FactorsPriorityCalculation
    Align = alClient
    MultiLine = True
    OwnerDraw = True
    ParentShowHint = False
    ShowHint = False
    TabHeight = 350
    TabOrder = 0
    TabPosition = tpLeft
    TabWidth = 20
    OnChange = StatesChange
    OnChanging = StatesChanging
    OnDrawTab = StatesDrawTab
    ExplicitWidth = 1128
    object ProblemChoise: TTabSheet
      Caption = #1042#1099#1073#1086#1088' '#1072#1085#1072#1083#1080#1079#1080#1088#1091#1077#1084#1086#1081' '#1079#1072#1076#1072#1095#1080
      OnShow = ProblemChoiseShow
      ExplicitWidth = 770
      object AddingProblemError: TLabel
        Left = 16
        Top = 167
        Width = 3
        Height = 13
      end
      object NewProblemNameLabel: TLabel
        Left = 16
        Top = 112
        Width = 52
        Height = 13
        Caption = #1053#1072#1079#1074#1072#1085#1080#1077':'
      end
      object NewProblemLabel: TLabel
        Left = 16
        Top = 90
        Width = 129
        Height = 13
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1085#1086#1074#1091#1102' '#1079#1072#1076#1072#1095#1091':'
      end
      object AddProblem: TButton
        Left = 16
        Top = 136
        Width = 249
        Height = 25
        Caption = #1057#1086#1079#1076#1072#1090#1100
        TabOrder = 0
        OnClick = AddProblemClick
      end
      object Problems: TComboBox
        Left = 16
        Top = 24
        Width = 249
        Height = 21
        TabOrder = 1
        Text = #1042#1099#1073#1077#1088#1080#1090#1077' '#1079#1072#1076#1072#1095#1091
        OnChange = ProblemsChange
      end
      object NewProblemName: TEdit
        Left = 70
        Top = 109
        Width = 195
        Height = 21
        TabOrder = 2
      end
    end
    object FactorsInput: TTabSheet
      Caption = #1042#1074#1086#1076' '#1082#1088#1080#1090#1077#1088#1080#1077#1074
      ImageIndex = 1
      ExplicitWidth = 770
      object NewFactorErrorLabel: TLabel
        Left = 322
        Top = 267
        Width = 3
        Height = 13
      end
      object FactorsList: TDBGrid
        Left = 15
        Top = 51
        Width = 249
        Height = 329
        DataSource = FactorsSource
        TabOrder = 0
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Expanded = False
            FieldName = 'name_'
            Title.Caption = #1050#1088#1080#1090#1077#1088#1080#1080':'
            Width = 100
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'isPositive'
            PickList.Strings = (
              #1055#1088#1103#1084#1086#1081
              #1054#1073#1088#1072#1090#1085#1099#1081)
            Title.Caption = #1058#1080#1087' '#1087#1086#1082#1072#1079#1072#1090#1077#1083#1103
            Visible = True
          end>
      end
      object NewFactorName: TEdit
        Left = 271
        Top = 209
        Width = 138
        Height = 21
        TabOrder = 1
      end
      object AddFactor: TButton
        Left = 270
        Top = 236
        Width = 138
        Height = 25
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1082#1088#1080#1090#1077#1088#1080#1081
        TabOrder = 2
        OnClick = AddFactorClick
      end
      object RemoveFactor: TButton
        Left = 271
        Top = 51
        Width = 138
        Height = 25
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1082#1088#1080#1090#1077#1088#1080#1081
        TabOrder = 3
        OnClick = RemoveFactorClick
      end
    end
    object FactorsComparasion: TTabSheet
      Caption = #1055#1086#1087#1072#1088#1085#1086#1077' '#1089#1088#1072#1074#1085#1077#1085#1080#1077' '#1082#1088#1080#1090#1077#1088#1080#1077#1074
      ImageIndex = 2
      ExplicitWidth = 770
      object FactorsComparasionsScrollBox: TScrollBox
        Left = 0
        Top = 0
        Width = 475
        Height = 430
        Align = alClient
        TabOrder = 0
        ExplicitWidth = 770
      end
    end
    object FactorsPriorityCalculation: TTabSheet
      Caption = #1056#1072#1089#1095#1105#1090' '#1074#1077#1082#1090#1086#1088#1072' '#1087#1088#1080#1086#1088#1080#1090#1077#1090#1086#1074' '#1082#1088#1080#1090#1077#1088#1080#1077#1074
      ImageIndex = 3
      ExplicitWidth = 770
      object FactorsPriorityGrid: TStringGrid
        Left = 16
        Top = 35
        Width = 441
        Height = 201
        TabOrder = 0
        RowHeights = (
          24
          24
          24
          24
          24)
      end
    end
    object AlternativesInput: TTabSheet
      Caption = #1042#1074#1086#1076' '#1072#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074
      ImageIndex = 4
      ExplicitWidth = 770
      object NewAlternativeErrorLabel: TLabel
        Left = 271
        Top = 267
        Width = 3
        Height = 13
      end
      object AlternativesList: TDBGrid
        Left = 16
        Top = 51
        Width = 249
        Height = 329
        DataSource = AlternativesSource
        TabOrder = 0
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Expanded = False
            FieldName = 'name_'
            Title.Caption = #1040#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074#1099':'
            Visible = True
          end>
      end
      object RemoveAlternative: TButton
        Left = 271
        Top = 51
        Width = 138
        Height = 25
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1072#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074#1091
        TabOrder = 1
        OnClick = RemoveAlternativeClick
      end
      object NewAlternativeName: TEdit
        Left = 271
        Top = 209
        Width = 138
        Height = 21
        TabOrder = 2
      end
      object AddAlternative: TButton
        Left = 271
        Top = 236
        Width = 138
        Height = 25
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1072#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074#1091
        TabOrder = 3
        OnClick = AddAlternativeClick
      end
    end
    object AlternativesValuesInput: TTabSheet
      Caption = #1042#1074#1086#1076' '#1079#1085#1072#1095#1077#1085#1080#1081' '#1072#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074
      ImageIndex = 5
      ExplicitWidth = 770
      object AlternativesValuesGrid: TStringGrid
        Left = 16
        Top = 35
        Width = 441
        Height = 201
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        TabOrder = 0
        OnKeyPress = AlternativesValuesGridKeyPress
        OnSetEditText = AlternativesValuesGridSetEditText
        RowHeights = (
          24
          24
          24
          24
          24)
      end
    end
    object AlternativesPriotiryCalculation: TTabSheet
      Caption = #1056#1072#1089#1095#1105#1090' '#1074#1077#1082#1090#1086#1088#1086#1074' '#1087#1088#1080#1086#1088#1080#1090#1077#1090#1086#1074' '#1072#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074
      ImageIndex = 6
      ExplicitWidth = 770
      object Factors: TComboBox
        Left = 16
        Top = 24
        Width = 249
        Height = 21
        TabOrder = 0
        Text = #1042#1099#1073#1077#1088#1080#1090#1077' '#1082#1088#1080#1090#1077#1088#1080#1081
        OnChange = FactorsChange
      end
    end
    object AlternativesGlobalPriotiryCalculation: TTabSheet
      Caption = #1056#1072#1089#1095#1105#1090' '#1075#1083#1086#1073#1072#1083#1100#1085#1086#1075#1086' '#1074#1077#1082#1090#1086#1088#1072' '#1087#1088#1080#1086#1088#1080#1090#1077#1090#1086#1074' '#1072#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074
      ImageIndex = 7
      ExplicitWidth = 770
      object AlternativesGlobalPriorityGrid: TStringGrid
        Left = 16
        Top = 35
        Width = 441
        Height = 201
        FixedRows = 2
        TabOrder = 0
        OnSetEditText = AlternativesValuesGridSetEditText
        ColWidths = (
          64
          64
          64
          64
          64)
      end
    end
    object MostFavorableAlternatives: TTabSheet
      Caption = #1053#1072#1080#1073#1086#1083#1077#1077' '#1073#1083#1072#1075#1086#1087#1088#1080#1103#1090#1085#1099#1077' '#1072#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074#1099
      ImageIndex = 8
      ExplicitWidth = 770
      object MostFavorableAlternativesGrid: TStringGrid
        Left = 16
        Top = 24
        Width = 249
        Height = 356
        ColCount = 2
        FixedCols = 0
        TabOrder = 0
      end
    end
  end
  object NextState: TButton
    Tag = 1
    Left = 403
    Top = 399
    Width = 75
    Height = 25
    Caption = '--->'
    TabOrder = 1
    Visible = False
    OnClick = ChangeStateClick
  end
  object PreviousState: TButton
    Tag = -1
    Left = 298
    Top = 399
    Width = 75
    Height = 25
    Caption = '<---'
    TabOrder = 2
    Visible = False
    OnClick = ChangeStateClick
  end
  object ProgressBar: TProgressBar
    Left = 0
    Top = 438
    Width = 833
    Height = 17
    Align = alBottom
    Max = 5000
    TabOrder = 3
    Visible = False
    ExplicitWidth = 1128
  end
  object ProblemConnection: TADOConnection
    LoginPrompt = False
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 144
    Top = 208
  end
  object FactorsTable: TADOTable
    Connection = ProblemConnection
    CursorType = ctStatic
    AfterPost = FactorsTableAfterPost
    TableName = 'Factors'
    Left = 72
    Top = 256
  end
  object FactorsSource: TDataSource
    DataSet = FactorsTable
    Left = 72
    Top = 304
  end
  object FactorsComparasionTable: TADOTable
    Connection = ProblemConnection
    TableName = 'FactorsComparasion'
    Left = 72
    Top = 352
  end
  object AlternativesTable: TADOTable
    Connection = ProblemConnection
    AfterPost = AlternativesTableAfterPost
    TableName = 'Alternatives'
    Left = 208
    Top = 256
  end
  object AlternativesSource: TDataSource
    DataSet = AlternativesTable
    Left = 208
    Top = 304
  end
  object AlternativesValuesTable: TADOTable
    Connection = ProblemConnection
    TableName = 'AlternativesValues'
    Left = 208
    Top = 352
  end
  object ProgressTimer: TTimer
    Enabled = False
    Interval = 10
    OnTimer = ProgressTimerTimer
    Left = 24
    Top = 400
  end
end

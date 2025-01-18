//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <CheckLst.hpp>
#include <Grids.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPageControl *States;
	TTabSheet *ProblemChoise;
	TTabSheet *FactorsInput;
	TButton *AddProblem;
	TEdit *NewProblemName;
	TComboBox *Problems;
	TLabel *AddingProblemError;
	TADOConnection *ProblemConnection;
	TADOTable *FactorsTable;
	TDataSource *FactorsSource;
	TDBGrid *FactorsList;
	TButton *PreviousState;
	TButton *NextState;
	TTabSheet *FactorsComparasion;
	TTabSheet *FactorsPriorityCalculation;
	TADOTable *FactorsComparasionTable;
	TTabSheet *AlternativesInput;
	TStringGrid *FactorsPriorityGrid;
	TLabel *NewProblemNameLabel;
	TADOTable *AlternativesTable;
	TDBGrid *AlternativesList;
	TLabel *NewProblemLabel;
	TButton *RemoveFactor;
	TButton *AddFactor;
	TEdit *NewFactorName;
	TLabel *NewFactorErrorLabel;
	TDataSource *AlternativesSource;
	TButton *RemoveAlternative;
	TEdit *NewAlternativeName;
	TButton *AddAlternative;
	TLabel *NewAlternativeErrorLabel;
	TADOTable *AlternativesValuesTable;
	TTabSheet *AlternativesValuesInput;
	TStringGrid *AlternativesValuesGrid;
	TScrollBox *FactorsComparasionsScrollBox;
	TTabSheet *AlternativesPriotiryCalculation;
	TComboBox *Factors;
	TTabSheet *AlternativesGlobalPriotiryCalculation;
	TStringGrid *AlternativesGlobalPriorityGrid;
	TTabSheet *MostFavorableAlternatives;
	TStringGrid *MostFavorableAlternativesGrid;
	TProgressBar *ProgressBar;
	TTimer *ProgressTimer;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall StatesDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect,
		  bool Active);
	void __fastcall AddProblemClick(TObject *Sender);
	void __fastcall ProblemsChange(TObject *Sender);
	void __fastcall ChangeStateClick(TObject *Sender);
	void __fastcall ProblemChoiseShow(TObject *Sender);
	void __fastcall CalculateFactorsPriority();
	void __fastcall StatesChanging(TObject *Sender, bool &AllowChange);
	void __fastcall StatesChange(TObject *Sender);
	void __fastcall FactorsComparationChange(TObject *Sender);
	void __fastcall AddFactorClick(TObject *Sender);
	void __fastcall FactorsTableAfterPost(TDataSet *DataSet);
	void __fastcall RemoveFactorClick(TObject *Sender);
	void __fastcall ShowFactorsComparasions();
	void __fastcall AddAlternativeClick(TObject *Sender);
	void __fastcall AlternativesTableAfterPost(TDataSet *DataSet);
	void __fastcall RemoveAlternativeClick(TObject *Sender);
	void __fastcall SetupAlternativesValuesInput();
	void __fastcall AlternativesValuesGridKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall AlternativesValuesGridSetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value);
	void __fastcall CalculateAlternativesPriority();
	void __fastcall FactorsChange(TObject *Sender);
	void __fastcall AutosizeGrid(TStringGrid *grid);
	void __fastcall ProgressTimerTimer(TObject *Sender);
	void __fastcall ProgressStart(int type);





private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

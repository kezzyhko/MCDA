//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
#include <vector.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	States->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StatesDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect,
          bool Active)
{
	if (Active) {
		States->Canvas->Font->Style = TFontStyles() << fsBold;
    }
	States->Canvas->TextOut(Rect.Left + 10, Rect.Top+(Active ? 5 : 0), States->Pages[TabIndex]->Caption);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ChangeStateClick(TObject *Sender)
{
	States->ActivePageIndex += ((TButton*)Sender)->Tag;
	StatesChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StatesChanging(TObject *Sender, bool &AllowChange)
{
	AllowChange = !ProgressTimer->Enabled && States->Tag >= 0;
	if (States->Tag < 0 && !ProgressTimer->Enabled) {
		ShowMessage("Сначала необходимо выбрать задачу");
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StatesChange(TObject *Sender)
{
	States->Tag += States->ActivePageIndex;
	States->ActivePageIndex = States->Tag - States->ActivePageIndex;
	States->Tag -= States->ActivePageIndex;
	ProgressStart(2);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ProblemChoiseShow(TObject *Sender)
{
	Problems->Items->Clear();
	TSearchRec sr;
	if (FindFirst("./Problems/*.mdb", faAnyFile, sr) == 0) {
		do
		{
			if (sr.Name != ".mdb" && sr.Name != "." && sr.Name != "..") {
				Problems->Items->Add(sr.Name.SubString(0, sr.Name.Length()-4));
			}
		} while (FindNext(sr) == 0);
		FindClose(sr);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AddProblemClick(TObject *Sender)
{
	bool created = false;
	char * NewPath = ("./Problems/" + NewProblemName->Text + ".mdb").t_str();
	AddingProblemError->Caption  =  (NewProblemName->Text == "") ? "Введите название проблемы" :
									(FileExists(NewPath)) ?	"Проблема с таким названием уже существует" :
									(created = (CopyFile("./Problems/.mdb", NewPath, true) != 0)) ? "Успешно" :
									"Неизвестная ошибка";
	if (created) {
		ProblemChoiseShow(Sender);
		Problems->ItemIndex = Problems->Items->IndexOf(NewProblemName->Text);
		ProblemsChange(Sender);
		NewProblemName->Text = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ProblemsChange(TObject *Sender)
{
	MainForm->Caption = "Компьютерная среда многокритериального анализа - " + Problems->Text;

	ProblemConnection->Connected = false;
	ProblemConnection->ConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=./Problems/" + Problems->Text + ".mdb;Persist Security Info=False";
	ProblemConnection->Connected = true;
	FactorsTable->Active = true;
	FactorsComparasionTable->Active = true;
	AlternativesTable->Active = true;
	AlternativesValuesTable->Active = true;

	ProgressStart(1);

	ShowFactorsComparasions();
	CalculateFactorsPriority();
	SetupAlternativesValuesInput();
	CalculateAlternativesPriority();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AddFactorClick(TObject *Sender)
{
	if (NewFactorName->Text == "") {
		NewFactorErrorLabel->Caption = "Введите название критерия";
	}
	else {
		FactorsTable->Insert();
		FactorsTable->FieldByName("name_")->AsAnsiString = NewFactorName->Text;
		FactorsTable->FieldByName("isPositive")->AsAnsiString = "Прямой";
		FactorsTable->Post();
		NewFactorName->Text = "";
		NewFactorErrorLabel->Caption = "";

		AlternativesTable->First();
		for (int i = 0; i < AlternativesTable->RecordCount; i++) {
			AlternativesValuesTable->Insert();
			AlternativesValuesTable->FieldByName("Alternative")->AsInteger = AlternativesTable->FieldByName("ID")->AsInteger;
			AlternativesValuesTable->FieldByName("Factor")->AsInteger = FactorsTable->FieldByName("ID")->AsInteger;
			AlternativesValuesTable->FieldByName("value_")->AsInteger = 0;
			AlternativesValuesTable->Post();

			AlternativesTable->Next();
		}

		int leftID, rightID;
		leftID = FactorsTable->FieldByName("ID")->AsInteger;
		FactorsTable->First();
		for (int i = 0; i < FactorsTable->RecordCount - 1; i++) {
			rightID = FactorsTable->FieldByName("ID")->AsInteger;

			FactorsComparasionTable->Insert();
			FactorsComparasionTable->FieldByName("Factor1")->AsInteger = leftID;
			FactorsComparasionTable->FieldByName("Factor2")->AsInteger = rightID;
			FactorsComparasionTable->FieldByName("priority")->AsInteger = 0;
			FactorsComparasionTable->Post();

			FactorsTable->Next();
		}
		ShowFactorsComparasions();
		CalculateFactorsPriority();
		SetupAlternativesValuesInput();
		CalculateAlternativesPriority();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FactorsTableAfterPost(TDataSet *DataSet)
{
	ShowFactorsComparasions();
	CalculateFactorsPriority();
	SetupAlternativesValuesInput();
	CalculateAlternativesPriority();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RemoveFactorClick(TObject *Sender)
{
	if (!FactorsTable->IsEmpty()) {
		int RecNo = FactorsTable->RecNo;

		while (FactorsComparasionTable->Locate("Factor1", FactorsTable->FieldByName("ID")->AsInteger, TLocateOptions())
			|| FactorsComparasionTable->Locate("Factor2", FactorsTable->FieldByName("ID")->AsInteger, TLocateOptions())) {
			FactorsComparasionTable->Delete();
		}
		while (AlternativesValuesTable->Locate("Factor", FactorsTable->FieldByName("ID")->AsInteger, TLocateOptions())) {
			AlternativesValuesTable->Delete();
		}
		FactorsTable->Delete();

		ShowFactorsComparasions();
		CalculateFactorsPriority();
		SetupAlternativesValuesInput();
		CalculateAlternativesPriority();

		FactorsTable->First();
		FactorsTable->MoveBy(RecNo-1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ShowFactorsComparasions()
{
	while (FactorsComparasionsScrollBox->ControlCount > 0) {
		delete FactorsComparasionsScrollBox->Controls[0];
	}
	FactorsComparasionTable->First();
	for (int i = 1; i <= FactorsComparasionTable->RecordCount; i++) {
		TTrackBar* comparation = new TTrackBar(MainForm);
		comparation->Parent = FactorsComparasionsScrollBox;
		comparation->Tag = FactorsComparasionTable->FieldByName("ID")->AsInteger;
		comparation->Min = -5;
		comparation->Max = 5;
		comparation->Position = FactorsComparasionTable->FieldByName("priority")->AsInteger;
		comparation->Left = 150;
		comparation->Top = i*50-2;
		comparation->OnChange = FactorsComparationChange;
		comparation->TickStyle = tsNone;
		comparation->TickMarks = tmBoth;

		FactorsTable->Locate("ID", FactorsComparasionTable->FieldByName("Factor1")->AsInteger, TLocateOptions());
		TLabel* Factor1 = new TLabel(MainForm);
		Factor1->Parent = FactorsComparasionsScrollBox;
		Factor1->Caption = FactorsTable->FieldByName("name_")->AsAnsiString;
		Factor1->Left = comparation->Left - Factor1->Width;
		Factor1->Top = i*50;

		FactorsTable->Locate("ID", FactorsComparasionTable->FieldByName("Factor2")->AsInteger, TLocateOptions());
		TLabel* Factor2 = new TLabel(MainForm);
		Factor2->Parent = FactorsComparasionsScrollBox;
		Factor2->Caption = FactorsTable->FieldByName("name_")->AsAnsiString;
		Factor2->Left = comparation->Left + comparation->Width;
		Factor2->Top = i*50;

		FactorsComparasionTable->Next();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FactorsComparationChange(TObject *Sender)
{
	FactorsComparasionTable->Locate("ID", ((TTrackBar*)Sender)->Tag, TLocateOptions());
	FactorsComparasionTable->Edit();
	FactorsComparasionTable->FieldByName("priority")->AsInteger = ((TTrackBar*)Sender)->Position;
	FactorsComparasionTable->Post();
	CalculateFactorsPriority();
	CalculateAlternativesPriority();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::CalculateFactorsPriority()
{
	FactorsPriorityGrid->RowCount = 1 + FactorsTable->RecordCount;
	FactorsPriorityGrid->ColCount = 1 + FactorsTable->RecordCount + 3;

	for (int i = 0; i <= FactorsTable->RecordCount; i++) {
		FactorsPriorityGrid->Cells[FactorsTable->RecordCount+1][i] = "";
	}
	FactorsTable->First();
	for (int i = 1; i <= FactorsTable->RecordCount; i++) {
		FactorsPriorityGrid->Cells[0][i] = FactorsTable->FieldByName("name_")->AsAnsiString;
		FactorsPriorityGrid->Cells[i][0] = FactorsTable->FieldByName("name_")->AsAnsiString;
		FactorsPriorityGrid->Cells[i][i] = FormatFloat("0.000", 1);
		FactorsTable->Next();
	}

	int leftID, rightID;
	for (int j = 1; j <= FactorsTable->RecordCount-1; j++) {
		FactorsTable->First();
		FactorsTable->MoveBy(j-1);
		leftID = FactorsTable->FieldByName("ID")->AsInteger;
		for (int i = j+1; i <= FactorsTable->RecordCount; i++) {
			FactorsTable->Next();
			rightID = FactorsTable->FieldByName("ID")->AsInteger;

			bool reverce;
			if (reverce = !FactorsComparasionTable->Locate("Factor1;Factor2", VarArrayOf(OPENARRAY(Variant, (leftID, rightID))), TLocateOptions())) {
				FactorsComparasionTable->Locate("Factor1;Factor2", VarArrayOf(OPENARRAY(Variant, (rightID, leftID))), TLocateOptions());;
			}
			float priority = FactorsComparasionTable->FieldByName("priority")->AsInteger;
			if (reverce) {
				priority = -priority;
			}
			priority = (priority<0) ? -priority+1 : 1.0/(priority+1);

			FactorsPriorityGrid->Cells[i][j] = FormatFloat("0.000", priority);
			FactorsPriorityGrid->Cells[j][i] = FormatFloat("0.000", 1/priority);
		}
	}

	FactorsPriorityGrid->Cells[FactorsTable->RecordCount+2][0] = "Вектор приоритетов";
	FactorsPriorityGrid->Cells[FactorsTable->RecordCount+3][0] = "Нормализованный вектор приоритетов";
	float max = 0;
	for (int j = 1; j <= FactorsTable->RecordCount; j++) {
		float priority = 0;
		for (int i = 1; i <= FactorsTable->RecordCount; i++) {
			priority += FactorsPriorityGrid->Cells[i][j].ToDouble();
		}
		FactorsPriorityGrid->Cells[FactorsTable->RecordCount+2][j] = FormatFloat("0.000", priority);
		if (priority > max) max = priority;
	}
	for (int j = 1; j <= FactorsTable->RecordCount; j++) {
		FactorsPriorityGrid->Cells[FactorsTable->RecordCount+3][j] = FormatFloat("0.000", FactorsPriorityGrid->Cells[FactorsTable->RecordCount+2][j].ToDouble()/max);
	}
	AutosizeGrid(FactorsPriorityGrid);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AddAlternativeClick(TObject *Sender)
{
	if (NewAlternativeName->Text == "") {
		NewAlternativeErrorLabel->Caption = "Введите название альтернативы";
	}
	else {
		AlternativesTable->Insert();
		AlternativesTable->FieldByName("name_")->AsAnsiString = NewAlternativeName->Text;
		AlternativesTable->Post();
		NewAlternativeName->Text = "";
		NewAlternativeErrorLabel->Caption = "";

		FactorsTable->First();
		for (int i = 0; i < FactorsTable->RecordCount; i++) {
			AlternativesValuesTable->Insert();
			AlternativesValuesTable->FieldByName("Alternative")->AsInteger = AlternativesTable->FieldByName("ID")->AsInteger;
			AlternativesValuesTable->FieldByName("Factor")->AsInteger = FactorsTable->FieldByName("ID")->AsInteger;
			AlternativesValuesTable->FieldByName("value_")->AsInteger = 0;
			AlternativesValuesTable->Post();

			FactorsTable->Next();
		}
		SetupAlternativesValuesInput();
		CalculateAlternativesPriority();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AlternativesTableAfterPost(TDataSet *DataSet)
{
	SetupAlternativesValuesInput();
	CalculateAlternativesPriority();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RemoveAlternativeClick(TObject *Sender)
{
	if (!AlternativesTable->IsEmpty()) {
		int RecNo = AlternativesTable->RecNo;

		while (AlternativesValuesTable->Locate("Alternative", AlternativesTable->FieldByName("ID")->AsInteger, TLocateOptions())) {
			AlternativesValuesTable->Delete();
		}
		AlternativesTable->Delete();

		SetupAlternativesValuesInput();
		CalculateAlternativesPriority();

		AlternativesTable->First();
		AlternativesTable->MoveBy(RecNo-1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetupAlternativesValuesInput()
{
	AlternativesValuesGrid->RowCount = AlternativesTable->RecordCount + 1;
	AlternativesTable->First();
	for (int j = 1; j <= AlternativesTable->RecordCount; j++) {
		AlternativesValuesGrid->Cells[0][j] = AlternativesTable->FieldByName("name_")->AsAnsiString;
		AlternativesTable->Next();
	}

	AlternativesValuesGrid->ColCount = FactorsTable->RecordCount + 1;
	FactorsTable->First();
	for (int i = 1; i <= FactorsTable->RecordCount; i++) {
		AlternativesValuesGrid->Cells[i][0] = FactorsTable->FieldByName("name_")->AsAnsiString;
		FactorsTable->Next();
	}

	FactorsTable->First();
	for (int i = 1; i <= FactorsTable->RecordCount; i++) {
		AlternativesTable->First();
		for (int j = 1; j <= AlternativesTable->RecordCount; j++) {
			AlternativesValuesTable->Locate("Alternative;Factor", VarArrayOf(OPENARRAY(Variant, (AlternativesTable->FieldByName("ID")->AsInteger, FactorsTable->FieldByName("ID")->AsInteger))), TLocateOptions());;
			AlternativesValuesGrid->Cells[i][j] = AlternativesValuesTable->FieldByName("value_")->AsAnsiString;
			AlternativesTable->Next();
		}
		FactorsTable->Next();
	}
	AutosizeGrid(AlternativesValuesGrid);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AlternativesValuesGridKeyPress(TObject *Sender, wchar_t &Key)
{
	if ((Key<'0' || Key>'9') && Key != VK_BACK && Key != VK_RETURN) Key = 0;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::AlternativesValuesGridSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value)
{
	AlternativesValuesGrid->Cells[ACol][ARow] = UnicodeString((Value == "") ? 0 : Value.ToInt());

	AlternativesTable->First();
	AlternativesTable->MoveBy(ARow-1);
	FactorsTable->First();
	FactorsTable->MoveBy(ACol-1);
	AlternativesValuesTable->Locate("Alternative;Factor", VarArrayOf(OPENARRAY(Variant, (AlternativesTable->FieldByName("ID")->AsInteger, FactorsTable->FieldByName("ID")->AsInteger))), TLocateOptions());;
	AlternativesValuesTable->Edit();
	AlternativesValuesTable->FieldByName("value_")->AsInteger = AlternativesValuesGrid->Cells[ACol][ARow].ToInt();
	AlternativesValuesTable->Post();
	CalculateAlternativesPriority();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CalculateAlternativesPriority()
{
	Factors->Items->Clear();
	while (AlternativesPriotiryCalculation->ControlCount > 1) {
		delete AlternativesPriotiryCalculation->Controls[1];
	}

	FactorsTable->First();
	for (int k = 1; k <= FactorsTable->RecordCount; k++) {
		Factors->Items->Add(FactorsTable->FieldByName("name_")->AsAnsiString);

		TStringGrid* grid = new TStringGrid(MainForm);
		grid->Parent = AlternativesPriotiryCalculation;
		grid->Left = 16;
		grid->Top = 51;
		grid->Height = 201;
		grid->Width = 441;
		grid->Visible = false;

		grid->RowCount = 1 + AlternativesTable->RecordCount;
		grid->ColCount = 1 + AlternativesTable->RecordCount + 3;

		for (int i = 0; i <= AlternativesTable->RecordCount; i++) {
			grid->Cells[AlternativesTable->RecordCount+1][i] = "";
		}
		AlternativesTable->First();
		for (int i = 1; i <= AlternativesTable->RecordCount; i++) {
			grid->Cells[0][i] = AlternativesTable->FieldByName("name_")->AsAnsiString;
			grid->Cells[i][0] = AlternativesTable->FieldByName("name_")->AsAnsiString;
			grid->Cells[i][i] = FormatFloat("0.000", 0);
			AlternativesTable->Next();
		}

		int leftValue, rightValue;
		for (int j = 1; j <= AlternativesTable->RecordCount-1; j++) {
			leftValue = AlternativesValuesGrid->Cells[k][j].ToInt();
			for (int i = j+1; i <= AlternativesTable->RecordCount; i++) {
				rightValue = AlternativesValuesGrid->Cells[k][i].ToInt();

				grid->Cells[i][j] = FormatFloat("0.000", rightValue==0 ? 0 : (float)leftValue/rightValue);
				grid->Cells[j][i] = FormatFloat("0.000", leftValue==0 ? 0 : (float)rightValue/leftValue);
			}
		}

		grid->Cells[AlternativesTable->RecordCount+2][0] = "Вектор приоритетов";
		grid->Cells[AlternativesTable->RecordCount+3][0] = "Нормализованный вектор приоритетов";
		float max = -1, min = -1;
		for (int j = 1; j <= AlternativesTable->RecordCount; j++) {
			float priority = 0;
			for (int i = 1; i <= AlternativesTable->RecordCount; i++) {
				priority += grid->Cells[i][j].ToDouble();
			}
			grid->Cells[AlternativesTable->RecordCount+2][j] = FormatFloat("0.000", priority);
			if (priority > max) max = priority;
			if (priority < min || min == -1) min = priority;
		}
		for (int j = 1; j <= AlternativesTable->RecordCount; j++) {
			float priority = grid->Cells[AlternativesTable->RecordCount+2][j].ToDouble();
			grid->Cells[AlternativesTable->RecordCount+3][j] = FormatFloat("0.000",
				(max == min) ?
					1 :
					FactorsTable->FieldByName("isPositive")->AsAnsiString=="Прямой" ?
						priority/max :
						(max-priority)/(max-min)
			);
		}

		AutosizeGrid(grid);
		FactorsTable->Next();
	}



	AlternativesGlobalPriorityGrid->RowCount = 2 + AlternativesTable->RecordCount;
	AlternativesGlobalPriorityGrid->ColCount = 1 + FactorsTable->RecordCount + 3;

	for (int i = 0; i <= 2 + AlternativesTable->RecordCount; i++) {
		AlternativesGlobalPriorityGrid->Cells[FactorsTable->RecordCount+1][i] = "";
	}
	AlternativesTable->First();
	for (int i = 2; i <= 1 + AlternativesTable->RecordCount; i++) {
		AlternativesGlobalPriorityGrid->Cells[0][i] = AlternativesTable->FieldByName("name_")->AsAnsiString;
		AlternativesTable->Next();
	}
	FactorsTable->First();
	for (int i = 1; i <= FactorsTable->RecordCount; i++) {
		AlternativesGlobalPriorityGrid->Cells[i][0] = FactorsTable->FieldByName("name_")->AsAnsiString;
		AlternativesGlobalPriorityGrid->Cells[i][1] = FactorsPriorityGrid->Cells[FactorsTable->RecordCount+3][i];
		AlternativesTable->First();
		for (int j = 1; j <= AlternativesTable->RecordCount; j++) {
			AlternativesGlobalPriorityGrid->Cells[i][1+j] = ((TStringGrid*)AlternativesPriotiryCalculation->Controls[i])->Cells[AlternativesTable->RecordCount+3][j];
			AlternativesTable->Next();
		}
		FactorsTable->Next();
	}

	AlternativesGlobalPriorityGrid->Cells[FactorsTable->RecordCount+2][0] = "Вектор приоритетов";
	AlternativesGlobalPriorityGrid->Cells[FactorsTable->RecordCount+3][0] = "Нормализованный вектор приоритетов";
	float max = 0;
	for (int j = 2; j <= 1 + AlternativesTable->RecordCount; j++) {
		float priority = 0;
		for (int i = 1; i <= FactorsTable->RecordCount; i++) {
			priority += AlternativesGlobalPriorityGrid->Cells[i][j].ToDouble() * AlternativesGlobalPriorityGrid->Cells[i][1].ToDouble();
		}
		AlternativesGlobalPriorityGrid->Cells[FactorsTable->RecordCount+2][j] = FormatFloat("0.000", priority);
		if (priority > max) max = priority;
	}
	for (int j = 2; j <= 1 + AlternativesTable->RecordCount; j++) {
		AlternativesGlobalPriorityGrid->Cells[FactorsTable->RecordCount+3][j] = FormatFloat("0.000", max == 0 ? 0 : AlternativesGlobalPriorityGrid->Cells[FactorsTable->RecordCount+2][j].ToDouble()/max);
	}
	AutosizeGrid(AlternativesGlobalPriorityGrid);




	MostFavorableAlternativesGrid->Cells[0][0] = "Альтернатива";
	MostFavorableAlternativesGrid->Cells[1][0] = "Вектор благоприятности";
	MostFavorableAlternativesGrid->RowCount = 1 + AlternativesTable->RecordCount;
	for (int j = 1; j <= AlternativesTable->RecordCount; j++) {
		MostFavorableAlternativesGrid->Cells[0][j] = AlternativesGlobalPriorityGrid->Cells[0][j+1];
		MostFavorableAlternativesGrid->Cells[1][j] = AlternativesGlobalPriorityGrid->Cells[FactorsTable->RecordCount+3][j+1];
	}
	for (int j = 1; j <= 1 + AlternativesTable->RecordCount - 1; j++) {
		for (int i = 1; i <= 1 + AlternativesTable->RecordCount - j; i++) {
			if (MostFavorableAlternativesGrid->Cells[1][i] < MostFavorableAlternativesGrid->Cells[1][i+1]) {
				MostFavorableAlternativesGrid->Rows[MostFavorableAlternativesGrid->RowCount+1] = MostFavorableAlternativesGrid->Rows[i+1];
				MostFavorableAlternativesGrid->Rows[i+1] = MostFavorableAlternativesGrid->Rows[i];
				MostFavorableAlternativesGrid->Rows[i] = MostFavorableAlternativesGrid->Rows[MostFavorableAlternativesGrid->RowCount+1];
			}
		}
	}
/*
	MostFavorableAlternativesGrid->Cells[1][1] = "Наиболее благоприятный выбор";
	for (int j = 2; j <= AlternativesTable->RecordCount; j++) {
		MostFavorableAlternativesGrid->Cells[1][j] = UnicodeString((1-MostFavorableAlternativesGrid->Cells[1][j].ToDouble())*100) + "%";
	}
*/
	AutosizeGrid(MostFavorableAlternativesGrid);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FactorsChange(TObject *Sender)
{
	for (int i = 1; i < AlternativesPriotiryCalculation->ControlCount; i++) {
		AlternativesPriotiryCalculation->Controls[i]->Visible = false;
	}
	ProgressStart(3);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AutosizeGrid(TStringGrid *grid)
{
	for (int i = 0; i < grid->ColCount; i++) {
		int maxW = 0;
		for (int j = 0; j < grid->RowCount; j++) {
			maxW = max(maxW, grid->Canvas->TextWidth(grid->Cells[i][j]));
		}
		grid->ColWidths[i] = maxW + 10;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ProgressTimerTimer(TObject *Sender)
{
	ProgressTimer->Interval = random(1000);
	if (ProgressBar->Position == ProgressBar->Max) {
		ProgressTimer->Enabled = false;
		ProgressBar->Position = 0;
		ProgressBar->Visible = false;
		switch (ProgressBar->Tag) {
			case 1:
				States->Tag = 0;
				NextState->Visible = true;
			break;
			case 2:
				States->ActivePageIndex = States->Tag;
				PreviousState->Visible = (States->ActivePageIndex != 0);
				NextState->Visible = (States->ActivePageIndex != States->PageCount-1);
			break;
			case 3:
				AlternativesPriotiryCalculation->Controls[Factors->ItemIndex+1]->Visible = true;
			break;
			default:
				ShowMessage("WTF? " + UnicodeString(ProgressBar->Tag) + " is not between 1 and 3!");
			break;
		}

	}
	else {
		ProgressBar->Position += random(ProgressBar->Max - ProgressBar->Position + 1);
		if (ProgressBar->Position >= 0.8 * ProgressBar->Max) ProgressBar->Position = ProgressBar->Max;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ProgressStart(int type)
{
	if (!ProgressTimer->Enabled) {
		ProgressBar->Tag = type;
		ProgressBar->Visible = true;
		ProgressTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------


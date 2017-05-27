//---------------------------------------------------------------------------
#pragma hdrstop
#include "stringData.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall stringData::stringData(TComponent *myOwner, tileType typeOfTile) : zeroTile(myOwner,typeOfTile)
{
	ID = 14;
	type = typeOfTile;
	//teskt w kafelkach - LABEL 1
	label1 = new TLabel(this);
	label1->Parent = this;
	label1->Width = Width;
	label1->Height = Height/4;
	label1->Position->Y = (Height/4)*0;
	label1->Scale->Y = 2;
	label1->VertTextAlign = label1->TextAlign;
	label1->Font->Family = "Courier";
	label1->Font->Size = 12;
	label1->Text = "LABEL 1";
	label1->FontFill->Color = black;
	//teskt w kafelkach - LABEL 2
	label2 = new TLabel(this);
	label2->Parent = this;
	label2->Width = Width;
	label2->Height = Height/4;
	label2->Position->Y = (Height/4)*1;
	label2->Scale->Y = 2;
	label2->VertTextAlign = label1->TextAlign;
	label2->Font->Family = "Courier";
	label2->Font->Size = 12;
	label2->Text = "LABEL 2";
	label2->FontFill->Color = black;
	//teskt w kafelkach - LABEL 3
	labelVal = new TLabel(this);
	labelVal->Parent = this;
	labelVal->Width = Width;
	labelVal->Height = Height/4;
	labelVal->Position->Y = (Height/4)*2+5;
	labelVal->Scale->Y = 2;
	labelVal->VertTextAlign = TTextAlign::taLeading;
	labelVal->TextAlign = TTextAlign::taCenter;
	labelVal->Font->Family = "Courier";
	labelVal->Font->Size = 10;
	labelVal->Text = "<string>";
	labelVal->FontFill->Color = black;
	//separator
	separator = new TLine(this);
	separator->Parent = this;
	separator->Height = 10;
	separator->Width = Width-2;
	separator->Position->X = 1;
	separator->Position->Y = (Height/4)*2+3;
	separator->LineType = TLineType::ltTop;
	separator->Stroke->Color = label1->FontFill->Color;
	separator->HitTest = false;
	separator->BringToFront();
	//startowy sygnal oraz liczba sygnalow
	variable = "";
	//flagi wewnetrzne kafelka
	operationCond = false;
	//przyciski opcji dajemy na front
	noField->BringToFront();
	idField->BringToFront();
	cancelActionButton->BringToFront();
	delButton->BringToFront();
	swapButton->BringToFront();
	swapCancelButton->BringToFront();
	addPatternButton->BringToFront();
	doneButton->BringToFront();
	copyButton->BringToFront();
	pasteButton->BringToFront();
	modButton->BringToFront();
}
//---------------------------------------------------------------------------
//destruktor
//---------------------------------------------------------------------------
__fastcall stringData::~stringData()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	variable = "";
	operationCond = false;
	/************************************
	 **** USUWAMY LABELE  ***************
	 ************************************/
	if (separator) {
		delete separator;
		separator = NULL;
	}
	if (labelVal) {
		delete labelVal;
		labelVal = NULL;
	}
	if (label2) {
		delete label2;
		label2 = NULL;
	}
	if (label1) {
		delete label1;
		label1 = NULL;
	}
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* stringData::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new stringData(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* stringData::showEditor(void)
{
	//chowamy przyciski z menu
	menuOptions(false);
	tilePropertiesWindow->callerTile = this;
	tilePropertiesWindow->ShowModal();
	//zwracamy adres uaktualnionego kafelka
	//(jezeli zmieni sie ID to bedzie zupelnie nowy adres)
	return tilePropertiesWindow->callerTile;
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z edytora do kafelka (pobranie wartosci koncowych)
 */
void stringData::setTileContent(void)
{
	try {
		//uzupelniamy wyglad kafelka
		background_altOff->Fill->Color = tilePropertiesWindow->colorButtonBackg_altOff->Color;
		label1->Text = tilePropertiesWindow->editLabel1->Text;
		label2->Text = tilePropertiesWindow->editLabel2->Text;
		label1->ApplyStyleLookup();
		label1->FontFill->Color = tilePropertiesWindow->colorButtonLabel->Color;
		label2->ApplyStyleLookup();
		label2->FontFill->Color = tilePropertiesWindow->colorButtonLabel->Color;
		labelVal->ApplyStyleLookup();
		labelVal->FontFill->Color = tilePropertiesWindow->colorButtonLabel->Color;
		separator->Stroke->Color = label1->FontFill->Color;
		//uzupelniamy zmienne wewnetrzne kafelka
		//startowy sygnal oraz liczba sygnalow
		variable = tilePropertiesWindow->editVariableName->Text;
		//flagi wewnetrzne kafelka
		operationCond = tilePropertiesWindow->checkBoxOperationCond->IsChecked;
	} catch (...) {
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do kafelka (pobranie wartosci koncowych)
 */
void stringData::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("stringData")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void stringData::getTileContent(void)
{
	try {
		//pobieramy wyglad kafelka
		tilePropertiesWindow->colorButtonBackg_altOff->Color = background_altOff->Fill->Color;
		tilePropertiesWindow->colorButtonLabel->Color = label1->FontFill->Color;
		tilePropertiesWindow->editLabel1->Text = label1->Text;
		tilePropertiesWindow->editLabel2->Text = label2->Text;
		//pobieramy zmienne wewnetrzne kafelka
		//startowy sygnal oraz liczba sygnalow
		tilePropertiesWindow->editVariableName->Text = variable;
		//flagi wewnetrzne kafelka
		tilePropertiesWindow->checkBoxOperationCond->IsChecked = operationCond;
	} catch (...) {
	}
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void stringData::saveToXML(TXMLDocument *xmlSource, int tileNo)
{
	//sprawdzamy istnieje zrodlo XML
	if (xmlSource) {
		IXMLNode *nodeRoot, *nodeTile, *nodeParams;
		//ustawienia poczatkowe
		xmlSource->Active = true;
		xmlSource->Version = "1.0";
		xmlSource->Encoding = "UTF-8";
		//znajdujemy wezel glowny
		nodeRoot = xmlSource->DocumentElement;
		//sprawdzamy czy wezel ma w ogole dzieci
		if (nodeRoot->HasChildNodes) {
			//sprawdzamy czy argument tileNo >= 0  i istnieje tyle pochodnycj
			if (tileNo>=0 && nodeRoot->ChildNodes->Count>=tileNo+1) {
				//usuwamy stary wpis
				nodeRoot->ChildNodes->Delete(tileNo);
                //wstawiamy nowy wpis
				nodeTile = nodeRoot->AddChild("tile",tileNo);
			} else {
				//dopisujemy na koncu pliku
				nodeTile = nodeRoot->AddChild("tile",nodeRoot->ChildNodes->Count);
            }
		} else {
			//wezel nie ma dzieci dodajemy na koncu
			nodeTile = nodeRoot->AddChild("tile",nodeRoot->ChildNodes->Count);
		}

		tileColors lblColor, bcgColor = static_cast<tileColors>(background_altOff->Fill->Color);
		//ustawiamy wszystkie parametry
		nodeTile->SetAttribute("ID",ID);
		nodeTile->SetAttribute("type",type);
		nodeTile->SetAttribute("backg",colorToStr(bcgColor));
		nodeTile->SetAttribute("oper",operationCond);
		//dodajemy informacje o sygnale
		nodeParams = nodeTile->AddChild("variable",0);
		nodeParams->Text = variable;
		//dodajemy wartosc tekstu z labelu 1
		nodeParams = nodeTile->AddChild("label1",1);
		lblColor = static_cast<tileColors>(label1->FontFill->Color);
		nodeParams->SetAttribute("color",colorToStr(lblColor));
		nodeParams->Text = label1->Text;
		//dodajemy wartosc tekstu z labelu 2
		nodeParams = nodeTile->AddChild("label2",2);
		lblColor = static_cast<tileColors>(label2->FontFill->Color);
		nodeParams->SetAttribute("color",colorToStr(lblColor));
		nodeParams->Text = label2->Text;
		//formatujemy XML (opcja doNodeAutoIndent slabo dziala)
		xmlSource->XML->Text = FormatXMLData(xmlSource->XML->Text);
		xmlSource->Active = true;
	}
}
//---------------------------------------------------------------------------
/* metoda odczytania ustawien z pliku
 * arg: fileName - nazwa pliku y ktorego odcyztujemz
 */
void stringData::loadFromXML(TXMLDocument *xmlSource, int tileNo)
{
	//sprawdzamy istnieje zrodlo XML
	if (xmlSource) {
		IXMLNode *nodeRoot, *nodeTile, *nodeParams;
		//znajdujemy wezel glowny
		nodeRoot = xmlSource->DocumentElement;
		//sprawdzamy czy wezel ma w ogole dzieci
		if (nodeRoot->HasChildNodes && nodeRoot->ChildNodes->Count>=tileNo+1) {
			//odczytujemy odpowiedni wezel podrzedny i spr czy ma dobry ID
			nodeTile = nodeRoot->ChildNodes->Get(tileNo);
			if ((int)nodeTile->GetAttribute("ID")==ID) {
				type = static_cast<tileType>((int)nodeTile->GetAttribute("type"));
				//tutaj uzupelniamy informacje o kafelku na podstawie informacji z pliku
				background_altOff->Fill->Color = strToColor(nodeTile->GetAttribute("backg"));
				operationCond = nodeTile->GetAttribute("oper");
				//odczytujemy informacje zmiennych wewnetrznych kafelka
				nodeParams = nodeTile->ChildNodes->Get(0);
				variable = nodeParams->Text;
				//odczytujemy ustawienia tekstu z labelu 1
				String labelColor;
				nodeParams = nodeTile->ChildNodes->Get(1);
				labelColor = nodeParams->GetAttribute("color");
				label1->Text = nodeParams->Text;
				label1->ApplyStyleLookup();
				label1->FontFill->Color = strToColor(labelColor);
				//odczytujemy ustawienia tekstu z labelu 2
				nodeParams = nodeTile->ChildNodes->Get(2);
				labelColor = nodeParams->GetAttribute("color");
				label2->Text = nodeParams->Text;
				label2->ApplyStyleLookup();
				label2->FontFill->Color = strToColor(labelColor);
				labelVal->ApplyStyleLookup();
				labelVal->FontFill->Color = strToColor(labelColor);
				separator->Stroke->Color = label1->FontFill->Color;
			}
		}
	}
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku projektu (*.ifp)
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
AnsiString stringData::saveToIFP(int tileNo)
{
	tileColors currColor;
	String result = "";
	//ID kafelka
	result += IntToStr(tileNo)+","+IntToStr(ID)+",";
	//nazwa zmiennej
	result += "\""+variable+"\",";
	//teksty w labelach
	result += "\""+label1->Text+"\",";
	result += "\""+label2->Text+"\",";
	//kolor labela
	currColor = static_cast<tileColors>(label1->FontFill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//kolor tla
	currColor = static_cast<tileColors>(background_altOff->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//operation condition
	if (operationCond) {
		result += "1";
	} else {
		result += "0";
	}
	result += "\n";

	return result;
}
//---------------------------------------------------------------------------
/* metoda odczytania ustawien kafelka z pliku projektu (*.ifp)
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void stringData::loadFromIFP(String ifpData, int tileNo)
{
	String bcgColor, labelColor, temp, labelsText[2], var;
	int labelStart, labelStop, oper;
	temp = ifpData;
	//odczytujemy nazwe zmiennej
	labelStart = temp.Pos("\"")+1;
	labelStop = temp.SubString(labelStart,temp.Length()-labelStart).Pos("\"")-1;
	var = ifpData.SubString(labelStart,labelStop);
	//odczytujemy tekst LABELA 1
	temp = temp.SubString(labelStart+labelStop+3,temp.Length()+1-(labelStart+labelStop+3));
	labelStop = temp.Pos("\"");
	labelsText[0] = temp.SubString(0,labelStop-1);
	//odczytujemy tekst LABELA 2
	temp = temp.SubString(labelStop+3,temp.Length()+1-(labelStop+3));
	labelStop = temp.Pos("\"");
	labelsText[1] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o kolorze labeli
	temp = temp.SubString(labelStop+2,temp.Length()+1-(labelStop+2));
	labelStop = temp.Pos(",");
	labelColor = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o kolorze tla
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	bcgColor = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o operation cond
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	oper = StrToBool(temp.SubString(0,1));
	/********* UAKTUALNIAMY KAFELEK *********/
	variable = var;
	//ustawienia tla i tekstu
	background_altOff->Fill->Color = intToColor(StrToInt(bcgColor));
	label1->Text = labelsText[0];
	label1->ApplyStyleLookup();
	label1->FontFill->Color = intToColor(StrToInt(labelColor));
	label2->Text = labelsText[1];
	label2->ApplyStyleLookup();
	label2->FontFill->Color = intToColor(StrToInt(labelColor));
	labelVal->ApplyStyleLookup();
	labelVal->FontFill->Color = intToColor(StrToInt(labelColor));
	separator->Stroke->Color = label1->FontFill->Color;
	//zmienne wewnetrzne
	operationCond = oper;
}
//---------------------------------------------------------------------------

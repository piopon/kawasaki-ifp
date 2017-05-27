//---------------------------------------------------------------------------
#pragma hdrstop
#include "digitalSwitch.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall digitalSwitch::digitalSwitch(TComponent *myOwner, tileType typeOfTile) : digitalDisplay(myOwner,typeOfTile)
{
	ID = 6;
	type = typeOfTile;
	//limit gorny i dolny
	dataLimit = false;
	upperLimit = 0;
	lowerLimit = 0;
	//flagi wewnetrzne kafelka
	inpTypeKeys = true;
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
__fastcall digitalSwitch::~digitalSwitch()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	dataLimit = false;
	upperLimit = 0;
	lowerLimit = 0;
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* digitalSwitch::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new digitalSwitch(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* digitalSwitch::showEditor(void)
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
void digitalSwitch::setTileContent(void)
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
		topSignalNo = tilePropertiesWindow->editButtonSignal_altTop->Text.ToInt();
		signalsNo = tilePropertiesWindow->spinBoxSigNo->Value;
		//liczba wyswietlanych znakow
		figuresNo = tilePropertiesWindow->spinBoxFigures->Value;
		//limit gorny i dolny
		dataLimit = tilePropertiesWindow->checkBoxDataLimit->IsChecked;
		if (dataLimit) {
			upperLimit = tilePropertiesWindow->editDataLimUpper->Text.ToInt();
			lowerLimit = tilePropertiesWindow->editDataLimLower->Text.ToInt();
		}
		//flagi wewnetrzne kafelka
		minusOn = tilePropertiesWindow->checkBoxMinusVal->IsChecked;
		sigTypeBCD = tilePropertiesWindow->sigTypeBCD->IsChecked;
		inpTypeKeys = tilePropertiesWindow->inpTypeKeys->IsChecked;
		operationCond = tilePropertiesWindow->checkBoxOperationCond->IsChecked;
	} catch (...) {
		//tylko wykonanie funkcji ToInt() moze wywolac wyjatek, gdy Text = ""
		topSignalNo = 0;
		upperLimit = 0;
		lowerLimit = 0;
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do kafelka (pobranie wartosci koncowych)
 */
void digitalSwitch::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("digitalSwitch")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void digitalSwitch::getTileContent(void)
{
	try {
		//pobieramy wyglad kafelka
		tilePropertiesWindow->colorButtonBackg_altOff->Color = background_altOff->Fill->Color;
		tilePropertiesWindow->colorButtonLabel->Color = label1->FontFill->Color;
		tilePropertiesWindow->editLabel1->Text = label1->Text;
		tilePropertiesWindow->editLabel2->Text = label2->Text;
		//pobieramy zmienne wewnetrzne kafelka
		//startowy sygnal oraz liczba sygnalow
		tilePropertiesWindow->editButtonSignal_altTop->Text = IntToStr(topSignalNo);
		tilePropertiesWindow->spinBoxSigNo->Value = signalsNo;
		//liczba wyswietlanych znakow
		tilePropertiesWindow->spinBoxFigures->Value = figuresNo;
		//limit gorny i dolny
		tilePropertiesWindow->checkBoxDataLimit->IsChecked = dataLimit;
		tilePropertiesWindow->editDataLimUpper->Text = IntToStr(upperLimit);
		tilePropertiesWindow->editDataLimLower->Text = IntToStr(lowerLimit);
		//flagi wewnetrzne kafelka
		tilePropertiesWindow->checkBoxOperationCond->IsChecked = operationCond;
		tilePropertiesWindow->checkBoxMinusVal->IsChecked = minusOn;
		if (sigTypeBCD) {
			tilePropertiesWindow->sigTypeBCD->IsChecked = true;
			tilePropertiesWindow->sigTypeBin->IsChecked = false;
		} else {
			tilePropertiesWindow->sigTypeBCD->IsChecked = false;
			tilePropertiesWindow->sigTypeBin->IsChecked = true;
		}
		if (inpTypeKeys) {
			tilePropertiesWindow->inpTypeKeys->IsChecked = true;
			tilePropertiesWindow->inpTypeSwitch->IsChecked = false;
		} else {
			tilePropertiesWindow->inpTypeKeys->IsChecked = false;
			tilePropertiesWindow->inpTypeSwitch->IsChecked = true;
		}
	} catch (...) {
	}
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void digitalSwitch::saveToXML(TXMLDocument *xmlSource, int tileNo)
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
		nodeTile->SetAttribute("minus",minusOn);
		nodeTile->SetAttribute("bcd",sigTypeBCD);
		nodeTile->SetAttribute("keys",inpTypeKeys);
		nodeTile->SetAttribute("oper",operationCond);
		//dodajemy informacje o sygnale
		nodeParams = nodeTile->AddChild("sig",0);
		nodeParams->SetAttribute("top",topSignalNo);
		nodeParams->SetAttribute("no",signalsNo);
		nodeParams->SetAttribute("fig",figuresNo);
		nodeParams->SetAttribute("lim",dataLimit);
		nodeParams->SetAttribute("limUp",upperLimit);
		nodeParams->SetAttribute("limLow",lowerLimit);
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
void digitalSwitch::loadFromXML(TXMLDocument *xmlSource, int tileNo)
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
				minusOn = nodeTile->GetAttribute("minus");
				sigTypeBCD = nodeTile->GetAttribute("bcd");
				inpTypeKeys = nodeTile->GetAttribute("keys");
				operationCond = nodeTile->GetAttribute("oper");
				//odczytujemy informacje zmiennych wewnetrznych kafelka
				nodeParams = nodeTile->ChildNodes->Get(0);
				topSignalNo = nodeParams->GetAttribute("top");
				signalsNo = nodeParams->GetAttribute("no");
				figuresNo = nodeParams->GetAttribute("fig");
				dataLimit = nodeParams->GetAttribute("lim");
				upperLimit = nodeParams->GetAttribute("limUp");
				lowerLimit = nodeParams->GetAttribute("limLow");
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
AnsiString digitalSwitch::saveToIFP(int tileNo)
{
	tileColors currColor;
	String result = "";
	//ID kafelka
	result += IntToStr(tileNo)+","+IntToStr(ID)+",";
	//teksty w labelach
	result += "\""+label1->Text+"\",";
	result += "\""+label2->Text+"\",";
	//kolor labela
	currColor = static_cast<tileColors>(label1->FontFill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//kolor tla
	currColor = static_cast<tileColors>(background_altOff->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//numer sygnalu
	result += IntToStr(topSignalNo)+",";
	//ile kolejnych sygnalow
	result += IntToStr(signalsNo)+",";
	//ile cyfr wyswietlac
	result += IntToStr(figuresNo)+",";
	//czy jest limit danych
	if (dataLimit) {
		result += "1,";
	} else {
		result += "0,";
	}
	//limit gorny
	result += IntToStr(upperLimit)+",";
	//limit dolny
	result += IntToStr(lowerLimit)+",";
	//bcd czy binarnie
	if (sigTypeBCD) {
		result += "1,";
	} else {
		result += "0,";
	}
	//wyswietlac klawisze czy switcha
	if (inpTypeKeys) {
		result += "1,";
	} else {
		result += "0,";
	}
	//czy ze znakiem minus
	if (minusOn) {
		result += "1,";
	} else {
		result += "0,";
	}
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
void digitalSwitch::loadFromIFP(String ifpData, int tileNo)
{
	String bcgColor, labelColor, temp, labelsText[2];
	bool dataLim, sigBCD, inKeys, minus, opCond;
	int labelStart, labelStop, topSig, sigsNo, figNo, upLim, lowLim;
	temp = ifpData;
    //odczytujemy tekst LABELA 1
	labelStart = temp.Pos("\"")+1;
	labelStop = temp.SubString(labelStart,temp.Length()-labelStart).Pos("\"")-1;
	labelsText[0] = ifpData.SubString(labelStart,labelStop);
	//odczytujemy tekst LABELA 2
	temp = temp.SubString(labelStart+labelStop+3,temp.Length()+1-(labelStart+labelStop+3));
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
	//odczytujemy informacje o numerze sygnalu startowego
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	topSig = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o liczbie kolejnych sygnalow
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	sigsNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o liczbie wyswietlanych cyfr
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	figNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o ograniczeniach ustawianej liczby
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	dataLim = StrToBool(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o gornym ograniczeniu
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	upLim = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o dolnym ograniczeniu
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	lowLim = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o sposobie wyswietlania liczby (BCD / BIN)
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	sigBCD = StrToBool(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o sposobie wprowadzania danych (KEYS/SWITCH)
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	inKeys = StrToBool(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o tym czy chcemy miec znak MINUS
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	minus = StrToBool(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o boolu operation condition
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	opCond = StrToInt(temp.SubString(0,1));
	/********* UAKTUALNIAMY KAFELEK *********/
	topSignalNo = topSig;
	signalsNo = sigsNo;
	figuresNo = figNo;
	dataLimit = dataLim;
	upperLimit = upLim;
	lowerLimit = lowLim;
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
	sigTypeBCD = sigBCD;
	inpTypeKeys = inKeys;
	minusOn = minus;
	operationCond = opCond;
}
//---------------------------------------------------------------------------

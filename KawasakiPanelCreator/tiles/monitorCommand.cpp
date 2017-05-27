//---------------------------------------------------------------------------
#pragma hdrstop
#include "monitorCommand.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall monitorCommand::monitorCommand(TComponent *myOwner, tileType typeOfTile) : zeroTile(myOwner,typeOfTile)
{
	ID = 10;
	type = typeOfTile;
	//kolor tla kafelka w trybie ON
	backgroundOn = new TRectangle(this);
	backgroundOn->Parent = this;
	backgroundOn->HitTest = false;
	backgroundOn->Height = Height;
	backgroundOn->Width = Width;
	backgroundOn->Fill->Color = gray;
	//tlo wlaczone musi byc domyslnie schowane a wyl na wierzchu
	backgroundOn->SendToBack();
	background_altOff->BringToFront();
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
	label3 = new TLabel(this);
	label3->Parent = this;
	label3->Width = Width;
	label3->Height = Height/4;
	label3->Position->Y = (Height/4)*2;
	label3->Scale->Y = 2;
	label3->VertTextAlign = label1->TextAlign;
	label3->Font->Family = "Courier";
	label3->Font->Size = 12;
	label3->Text = "LABEL 3";
	label3->FontFill->Color = black;
	//teskt w kafelkach - LABEL 4
	label4 = new TLabel(this);
	label4->Parent = this;
	label4->Width = Width;
	label4->Height = Height/4;
	label4->Position->Y = (Height/4)*3;
	label4->Scale->Y = 2;
	label4->VertTextAlign = label1->TextAlign;
	label4->Font->Family = "Courier";
	label4->Font->Size = 12;
	label4->Text = "LABEL 4";
	label4->FontFill->Color = black;
	//uzupelniamy pozostale wartosci
	commandNo = 0;
	command = "";
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
__fastcall monitorCommand::~monitorCommand()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	commandNo = 0;
	command = "";
	operationCond = false;
	/************************************
	 **** USUWAMY LABELE  ***************
	 ************************************/
	if (label4) {
		delete label4;
		label4 = NULL;
	}
	if (label3) {
		delete label3;
		label3 = NULL;
	}
	if (label2) {
		delete label2;
		label2 = NULL;
	}
	if (label1) {
		delete label1;
		label1 = NULL;
	}
	/**************************************
	 ** USUWAMY TLO KAFELKA ***************
	 **************************************/
	if (backgroundOn) {
		delete backgroundOn;
		backgroundOn = NULL;
	}
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* monitorCommand::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new monitorCommand(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* monitorCommand::showEditor(void)
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
void monitorCommand::setTileContent(void)
{
	try {
		//uzupelniamy wyglad kafelka
		background_altOff->Fill->Color = tilePropertiesWindow->colorButtonBackg_altOff->Color;
		backgroundOn->Fill->Color = tilePropertiesWindow->colorButtonBackgOn->Color;
		label1->Text = tilePropertiesWindow->editLabel1->Text;
		label2->Text = tilePropertiesWindow->editLabel2->Text;
		label3->Text = tilePropertiesWindow->editLabel3->Text;
		label4->Text = tilePropertiesWindow->editLabel4->Text;
		label1->ApplyStyleLookup();
		label1->FontFill->Color = tilePropertiesWindow->colorButtonLabel->Color;
		label2->ApplyStyleLookup();
		label2->FontFill->Color = tilePropertiesWindow->colorButtonLabel->Color;
		label3->ApplyStyleLookup();
		label3->FontFill->Color = tilePropertiesWindow->colorButtonLabel->Color;
		label4->ApplyStyleLookup();
		label4->FontFill->Color = tilePropertiesWindow->colorButtonLabel->Color;
		//uzupelniamy zmienne wewnetrzne kafelka
		commandNo = tilePropertiesWindow->spinBoxCommandStrNo->Value;
		command = tilePropertiesWindow->editCommandString->Text;
		operationCond = tilePropertiesWindow->checkBoxOperationCond->IsChecked;
	} catch (...) {
		//tylko wykonanie funkcji ToInt() moze wywolac wyjatek, gdy Text = ""
        commandNo = 0;
	}

}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do kafelka (pobranie wartosci koncowych)
 */
void monitorCommand::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("monitorCommand")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void monitorCommand::getTileContent(void)
{
	try {
		//pobieramy wyglad kafelka
		tilePropertiesWindow->colorButtonBackg_altOff->Color = background_altOff->Fill->Color;
		tilePropertiesWindow->colorButtonBackgOn->Color = backgroundOn->Fill->Color;
		tilePropertiesWindow->colorButtonLabel->Color = label1->FontFill->Color;
		tilePropertiesWindow->editLabel1->Text = label1->Text;
		tilePropertiesWindow->editLabel2->Text = label2->Text;
		tilePropertiesWindow->editLabel3->Text = label3->Text;
		tilePropertiesWindow->editLabel4->Text = label4->Text;
		//pobieramy zmienne wewnetrzne kafelka
		tilePropertiesWindow->spinBoxCommandStrNo->Value = commandNo;
		tilePropertiesWindow->editCommandString->Text = command;
		tilePropertiesWindow->checkBoxOperationCond->IsChecked = operationCond;
	} catch (...) {
	}

}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void monitorCommand::saveToXML(TXMLDocument *xmlSource, int tileNo)
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
		tileColors bcgColor,lblColor;
		//ustawiamy wszystkie parametry
		nodeTile->SetAttribute("ID",ID);
		nodeTile->SetAttribute("type",type);
		nodeTile->SetAttribute("oper",operationCond);
		//dodajemy informacje o komendzie
		nodeParams = nodeTile->AddChild("command",0);
		nodeParams->SetAttribute("no",commandNo);
		nodeParams->Text = command;
		//dodajemy kolory tla
		nodeParams = nodeTile->AddChild("backg",1);
		bcgColor = static_cast<tileColors>(backgroundOn->Fill->Color);
		nodeParams->SetAttribute("on",colorToStr(bcgColor));
		bcgColor = static_cast<tileColors>(background_altOff->Fill->Color);
		nodeParams->SetAttribute("off",colorToStr(bcgColor));
		//dodajemy wartosc tekstu z labelu 1
		nodeParams = nodeTile->AddChild("label1",2);
		lblColor = static_cast<tileColors>(label1->FontFill->Color);
		nodeParams->SetAttribute("color",colorToStr(lblColor));
		nodeParams->Text = label1->Text;
		//dodajemy wartosc tekstu z labelu 2
		nodeParams = nodeTile->AddChild("label2",3);
		lblColor = static_cast<tileColors>(label2->FontFill->Color);
		nodeParams->SetAttribute("color",colorToStr(lblColor));
		nodeParams->Text = label2->Text;
		//dodajemy wartosc tekstu z labelu 3
		nodeParams = nodeTile->AddChild("label3",4);
		lblColor = static_cast<tileColors>(label3->FontFill->Color);
		nodeParams->SetAttribute("color",colorToStr(lblColor));
		nodeParams->Text = label3->Text;
		//dodajemy wartosc tekstu z labelu 4
		nodeParams = nodeTile->AddChild("label4",5);
		lblColor = static_cast<tileColors>(label4->FontFill->Color);
		nodeParams->SetAttribute("color",colorToStr(lblColor));
		nodeParams->Text = label4->Text;
		//formatujemy XML (opcja doNodeAutoIndent slabo dziala)
		xmlSource->XML->Text = FormatXMLData(xmlSource->XML->Text);
		xmlSource->Active = true;
	}
}
//---------------------------------------------------------------------------
/* metoda odczytania ustawien z pliku
 * arg: fileName - nazwa pliku y ktorego odcyztujemz
 */
void monitorCommand::loadFromXML(TXMLDocument *xmlSource, int tileNo)
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
				operationCond = nodeTile->GetAttribute("oper");
				//odczytujemy informacje o komendzie
				nodeParams = nodeTile->ChildNodes->Get(0);
				commandNo = nodeParams->GetAttribute("no");
				command = nodeParams->Text;
				String bcgColor, labelColor;
				//odczytujemy informacje o kolorze tla kafelka
				nodeParams = nodeTile->ChildNodes->Get(1);
				bcgColor = nodeParams->GetAttribute("on");
				backgroundOn->Fill->Color = strToColor(bcgColor);
				bcgColor = nodeParams->GetAttribute("off");
				background_altOff->Fill->Color = strToColor(bcgColor);
				//odczytujemy ustawienia tekstu z labelu 1
				nodeParams = nodeTile->ChildNodes->Get(2);
				labelColor = nodeParams->GetAttribute("color");
				label1->Text = nodeParams->Text;
				label1->ApplyStyleLookup();
				label1->FontFill->Color = strToColor(labelColor);
				//odczytujemy ustawienia tekstu z labelu 2
				nodeParams = nodeTile->ChildNodes->Get(3);
				labelColor = nodeParams->GetAttribute("color");
				label2->Text = nodeParams->Text;
				label2->ApplyStyleLookup();
				label2->FontFill->Color = strToColor(labelColor);
				//odczytujemy ustawienia tekstu z labelu 3
				nodeParams = nodeTile->ChildNodes->Get(4);
				labelColor = nodeParams->GetAttribute("color");
				label3->Text = nodeParams->Text;
				label3->ApplyStyleLookup();
				label3->FontFill->Color = strToColor(labelColor);
				//odczytujemy ustawienia tekstu z labelu 4
				nodeParams = nodeTile->ChildNodes->Get(5);
				labelColor = nodeParams->GetAttribute("color");
				label4->Text = nodeParams->Text;
				label4->ApplyStyleLookup();
				label4->FontFill->Color = strToColor(labelColor);
			}
		}
	}
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku projektu (*.ifp)
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
AnsiString monitorCommand::saveToIFP(int tileNo)
{
	tileColors currColor;
	String result = "";
	//ID kafelka
	result += IntToStr(tileNo)+","+IntToStr(ID)+",";
	//teksty w labelach
	result += "\""+label1->Text+"\",";
	result += "\""+label2->Text+"\",";
	result += "\""+label3->Text+"\",";
	result += "\""+label4->Text+"\",";
	//kolor labela
	currColor = static_cast<tileColors>(label1->FontFill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//kolor gdy ON
	currColor = static_cast<tileColors>(backgroundOn->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//kolor gdy OFF
	currColor = static_cast<tileColors>(background_altOff->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//numer komendy
	result += IntToStr(commandNo)+",";
	result += "\""+command+"\",";
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
void monitorCommand::loadFromIFP(String ifpData, int tileNo)
{
	String bcgColor[2], labelColor, temp, labelsText[4], cmd;
    int labelStart, labelStop, cmdNo, opCond;
	temp = ifpData;
    //odczytujemy tekst LABELA 1
	labelStart = temp.Pos("\"")+1;
	labelStop = temp.SubString(labelStart,temp.Length()-labelStart).Pos("\"")-1;
	labelsText[0] = ifpData.SubString(labelStart,labelStop);
	//odczytujemy tekst LABELA 2
	temp = temp.SubString(labelStart+labelStop+3,temp.Length()+1-(labelStart+labelStop+3));
	labelStop = temp.Pos("\"");
	labelsText[1] = temp.SubString(0,labelStop-1);
	//odczytujemy tekst LABELA 3
	temp = temp.SubString(labelStop+3,temp.Length()+1-(labelStop+3));
	labelStop = temp.Pos("\"");
	labelsText[2] = temp.SubString(0,labelStop-1);
	//odczytujemy tekst LABELA 4
	temp = temp.SubString(labelStop+3,temp.Length()+1-(labelStop+3));
	labelStop = temp.Pos("\"");
	labelsText[3] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o kolorze labeli
	temp = temp.SubString(labelStop+2,temp.Length()+1-(labelStop+2));
	labelStop = temp.Pos(",");
	labelColor = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o kolorze tla ON
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	bcgColor[0] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o kolorze tla OFF
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	bcgColor[1] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o numerze komendy
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	cmdNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o komendzie
	temp = temp.SubString(labelStop+2,temp.Length()+1-(labelStop+2));
	labelStop = temp.Pos("\"");
	cmd = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o boolu
	temp = temp.SubString(labelStop+2,temp.Length()+1-(labelStop+2));
	opCond = StrToInt(temp.SubString(0,1));
	/********* UAKTUALNIAMY KAFELEK *********/
	commandNo = cmdNo;
	command = cmd;
	operationCond = opCond;
	backgroundOn->Fill->Color = intToColor(StrToInt(bcgColor[0]));
	background_altOff->Fill->Color = intToColor(StrToInt(bcgColor[1]));
	label1->Text = labelsText[0];
	label1->ApplyStyleLookup();
	label1->FontFill->Color = intToColor(StrToInt(labelColor));
	label2->Text = labelsText[1];
	label2->ApplyStyleLookup();
	label2->FontFill->Color = intToColor(StrToInt(labelColor));
	label3->Text = labelsText[2];
	label3->ApplyStyleLookup();
	label3->FontFill->Color = intToColor(StrToInt(labelColor));
	label4->Text = labelsText[3];
	label4->ApplyStyleLookup();
	label4->FontFill->Color = intToColor(StrToInt(labelColor));
}
//---------------------------------------------------------------------------

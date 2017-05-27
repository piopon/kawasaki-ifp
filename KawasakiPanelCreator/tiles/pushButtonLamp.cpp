//---------------------------------------------------------------------------
#pragma hdrstop
#include "pushButtonLamp.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall pushButtonLamp::pushButtonLamp(TComponent *myOwner, tileType typeOfTile) : pushButton(myOwner,typeOfTile)
{
	ID = 3;
	type = typeOfTile;
	//uzupelniamy pozostale wartosci
	signalLampNo = 0;
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
__fastcall pushButtonLamp::~pushButtonLamp()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	signalLampNo = 0;
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* pushButtonLamp::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new pushButtonLamp(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* pushButtonLamp::showEditor(void)
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
void pushButtonLamp::setTileContent(void)
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
		signalNo = tilePropertiesWindow->editButtonSignal_altTop->Text.ToInt();
		signalLampNo = tilePropertiesWindow->editLampSignal->Text.ToInt();
		operationCond = tilePropertiesWindow->checkBoxOperationCond->IsChecked;
	} catch (...) {
		//tylko wykonanie funkcji ToInt() moze wywolac wyjatek, gdy Text = ""
		signalNo = 0;
		signalLampNo = 0;
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do kafelka (pobranie wartosci koncowych)
 */
void pushButtonLamp::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("pushButtonLamp")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void pushButtonLamp::getTileContent(void)
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
		tilePropertiesWindow->editButtonSignal_altTop->Text = IntToStr(signalNo);
		tilePropertiesWindow->editLampSignal->Text = IntToStr(signalLampNo);
		tilePropertiesWindow->checkBoxOperationCond->IsChecked = operationCond;
	} catch (...) {
	}

}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void pushButtonLamp::saveToXML(TXMLDocument *xmlSource, int tileNo)
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
		nodeTile->SetAttribute("sigBcg",signalNo);
		nodeTile->SetAttribute("sigLamp",signalLampNo);
		nodeTile->SetAttribute("oper",operationCond);
		//dodajemy kolory tla
		nodeParams = nodeTile->AddChild("backg",0);
		bcgColor = static_cast<tileColors>(backgroundOn->Fill->Color);
		nodeParams->SetAttribute("on",colorToStr(bcgColor));
		bcgColor = static_cast<tileColors>(background_altOff->Fill->Color);
		nodeParams->SetAttribute("off",colorToStr(bcgColor));
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
		//dodajemy wartosc tekstu z labelu 3
		nodeParams = nodeTile->AddChild("label3",3);
		lblColor = static_cast<tileColors>(label3->FontFill->Color);
		nodeParams->SetAttribute("color",colorToStr(lblColor));
		nodeParams->Text = label3->Text;
		//dodajemy wartosc tekstu z labelu 4
		nodeParams = nodeTile->AddChild("label4",4);
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
void pushButtonLamp::loadFromXML(TXMLDocument *xmlSource, int tileNo)
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
				signalNo = nodeTile->GetAttribute("sigBcg");
				signalLampNo = nodeTile->GetAttribute("sigLamp");
				operationCond = nodeTile->GetAttribute("oper");
                String bcgColor, labelColor;
				//odczytujemy informacje o kolorze tla kafelka
				nodeParams = nodeTile->ChildNodes->Get(0);
				bcgColor = nodeParams->GetAttribute("on");
				backgroundOn->Fill->Color = strToColor(bcgColor);
				bcgColor = nodeParams->GetAttribute("off");
				background_altOff->Fill->Color = strToColor(bcgColor);
				//odczytujemy ustawienia tekstu z labelu 1
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
				//odczytujemy ustawienia tekstu z labelu 3
				nodeParams = nodeTile->ChildNodes->Get(3);
				labelColor = nodeParams->GetAttribute("color");
				label3->Text = nodeParams->Text;
				label3->ApplyStyleLookup();
				label3->FontFill->Color = strToColor(labelColor);
				//odczytujemy ustawienia tekstu z labelu 4
				nodeParams = nodeTile->ChildNodes->Get(4);
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
AnsiString pushButtonLamp::saveToIFP(int tileNo)
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
	//numer sygnalu
	result += IntToStr(signalNo)+",";
	//numer sygnalu lampki
	result += IntToStr(signalLampNo)+",";
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
void pushButtonLamp::loadFromIFP(String ifpData, int tileNo)
{
	String bcgColor[2], labelColor, temp, labelsText[4];
    int labelStart, labelStop, sigNo, sigLmpNo, opCond;
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
	//odczytujemy informacje o sygnale
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	sigNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o sygnale lampki
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	sigLmpNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o boolu
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	opCond = StrToInt(temp.SubString(0,1));
	/********* UAKTUALNIAMY KAFELEK *********/
	signalNo = sigNo;
	signalLampNo = sigLmpNo;
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
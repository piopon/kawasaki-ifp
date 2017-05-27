//---------------------------------------------------------------------------
#pragma hdrstop
#include "selector3.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall selector3::selector3(TComponent *myOwner, tileType typeOfTile) : selector2(myOwner,typeOfTile)
{
	ID = 5;
	type = typeOfTile;
	//---------  tlo kafelka w trybie up/down  --------
	//pozycje z klasy bazowej kafelka selector2
	barColor_L_U->Position->Y = 25;
	barColor_R_D->Position->Y = 75;
	barColor_R_D->Height -= 1;
	//pozycja MIDDLE
	barColor_M = new TRectangle(this);
	barColor_M->Parent = this;
	barColor_M->HitTest = false;
	barColor_M->Height = Height/4;
	barColor_M->Width = Width-2;
	barColor_M->Position->X = 1;
	barColor_M->Position->Y = 50;
	barColor_M->Fill->Color = gray;
	barColor_M->Stroke->Kind = TBrushKind::bkNone;
	//musimy odslonic label 3
	label3->BringToFront();
	//uzupelniamy pozostale wartosci
	signalMiddle = 0;
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
	//uaktualniamy selektor do odpowiedniego trybu (up/down lub rotation)
	selectDisplayType(rotDispType);
}
//---------------------------------------------------------------------------
//destruktor
//---------------------------------------------------------------------------
__fastcall selector3::~selector3()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	signalMiddle = 0;
	/************************************
	 **** USUWAMY ELEMENTY POKRETLA  ****
	 ************************************/
	if (barColor_M) {
		delete barColor_M;
		barColor_M = NULL;
	}
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* selector3::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new selector3(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* funkcja uaktualniajaca wyglad selektora (up/down  <->  rotation) */
void selector3::selectDisplayType(bool rotType, bool changeState)
{
	if (rotType) {
		//pokazujemytlo w trybie ROTATION
		backgroundKnob->Visible = true;
		backgroundBar->Visible = true;
		//chowamy tlo w trybie UP/DOWN
		barColor_L_U->Visible = false;
		barColor_M->Visible = false;
		barColor_R_D->Visible = false;
	} else {
		//chowamy tlo w trybie ROTATION
		backgroundKnob->Visible = false;
		backgroundBar->Visible = false;
		//pokazujemy tlo w trybie UP/DOWN
		barColor_L_U->Visible = true;
		barColor_M->Visible = true;
		barColor_R_D->Visible = true;
    }
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* selector3::showEditor(void)
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
void selector3::setTileContent(void)
{
	try {
		//uzupelniamy wyglad kafelka
		//1. tlo defaultowe (nie ma opcji zmieniac)
		background_altOff->Fill->Color = gray;
		//2. tlo prostokatow gora/dol
		barColor_L_U->Fill->Color = tilePropertiesWindow->colorButtonSwitchL->Color;
		barColor_M->Fill->Color = tilePropertiesWindow->colorButtonSwitchM->Color;
		barColor_R_D->Fill->Color = tilePropertiesWindow->colorButtonSwitchR->Color;
		//3. tlo pokretla
		backgroundKnob->Fill->Color = barColor_R_D->Fill->Color;
		backgroundBar->Fill->Color = white1;
		barColor->Fill->Color = barColor_R_D->Fill->Color;
		//4. tekst labeli
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
		//5. zmienne wewnetrzne kafelka
		signalLeft_altUp = tilePropertiesWindow->editSwitchSignalL_altUp->Text.ToInt();
		signalMiddle = tilePropertiesWindow->editSwitchSignalM->Text.ToInt();
		signalRight_altDown = tilePropertiesWindow->editSwitchSignalR_altDown->Text.ToInt();
		operationCond = tilePropertiesWindow->checkBoxOperationCond->IsChecked;
		rotDispType = tilePropertiesWindow->dispTypeRotation->IsChecked;
		//uaktualniamy wyglad kafelka (up/down LUB rotation)
		selectDisplayType(rotDispType);
	} catch (...) {
		//tylko wykonanie funkcji ToInt() moze wywolac wyjatek, gdy Text = ""
		signalLeft_altUp = 0;
		signalRight_altDown = 0;
	}

}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do kafelka (pobranie wartosci koncowych)
 */
void selector3::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("selector3")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void selector3::getTileContent(void)
{
	try {
		//pobieramy wyglad kafelka
		//1. tlo
		tilePropertiesWindow->colorButtonSwitchL->Color = barColor_L_U->Fill->Color;
		tilePropertiesWindow->colorButtonSwitchM->Color = barColor_M->Fill->Color;
		tilePropertiesWindow->colorButtonSwitchR->Color = barColor_R_D->Fill->Color;
        //2. labele
		tilePropertiesWindow->colorButtonLabel->Color = label1->FontFill->Color;
		tilePropertiesWindow->editLabel1->Text = label1->Text;
		tilePropertiesWindow->editLabel2->Text = label2->Text;
		tilePropertiesWindow->editLabel3->Text = label3->Text;
		tilePropertiesWindow->editLabel4->Text = label4->Text;
		//3. zmienne wewnetrzne kafelka
		tilePropertiesWindow->editSwitchSignalL_altUp->Text = IntToStr(signalLeft_altUp);
		tilePropertiesWindow->editSwitchSignalM->Text = IntToStr(signalMiddle);
		tilePropertiesWindow->editSwitchSignalR_altDown->Text = IntToStr(signalRight_altDown);
		tilePropertiesWindow->checkBoxOperationCond->IsChecked = operationCond;
		if (rotDispType) {
			tilePropertiesWindow->dispTypeRotation->IsChecked = true;
			tilePropertiesWindow->dispTypeUpDown->IsChecked = false;
		} else {
			tilePropertiesWindow->dispTypeRotation->IsChecked = false;
			tilePropertiesWindow->dispTypeUpDown->IsChecked = true;
        }

	} catch (...) {
	}

}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void selector3::saveToXML(TXMLDocument *xmlSource, int tileNo)
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
		nodeTile->SetAttribute("sigL_U",signalLeft_altUp);
		nodeTile->SetAttribute("sigM",signalMiddle);
		nodeTile->SetAttribute("sigR_D",signalRight_altDown);
		nodeTile->SetAttribute("oper",operationCond);
		nodeTile->SetAttribute("rotDisp",rotDispType);
		//dodajemy kolory tla
		nodeParams = nodeTile->AddChild("backg",0);
		bcgColor = static_cast<tileColors>(barColor_L_U->Fill->Color);
		nodeParams->SetAttribute("L_U",colorToStr(bcgColor));
		bcgColor = static_cast<tileColors>(barColor_M->Fill->Color);
		nodeParams->SetAttribute("M",colorToStr(bcgColor));
		bcgColor = static_cast<tileColors>(barColor_R_D->Fill->Color);
		nodeParams->SetAttribute("R_D",colorToStr(bcgColor));
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
void selector3::loadFromXML(TXMLDocument *xmlSource, int tileNo)
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
				signalLeft_altUp = nodeTile->GetAttribute("sigL_U");
				signalMiddle = nodeTile->GetAttribute("sigM");
				signalRight_altDown = nodeTile->GetAttribute("sigR_D");
				operationCond = nodeTile->GetAttribute("oper");
				rotDispType = nodeTile->GetAttribute("rotDisp");
                String bcgColor, labelColor;
				//odczytujemy informacje o kolorze tla kafelka
				nodeParams = nodeTile->ChildNodes->Get(0);
				bcgColor = nodeParams->GetAttribute("L_U");
				barColor_L_U->Fill->Color = strToColor(bcgColor);
				bcgColor = nodeParams->GetAttribute("M");
				barColor_M->Fill->Color = strToColor(bcgColor);
				bcgColor = nodeParams->GetAttribute("R_D");
				barColor_R_D->Fill->Color = strToColor(bcgColor);
				//uzupelniamy informacje dla pokretla
				backgroundKnob->Fill->Color = barColor_R_D->Fill->Color;
				barColor->Fill->Color = barColor_R_D->Fill->Color;
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
				//uaktualniamy wyglad kafelka (up/down LUB rotation)
				selectDisplayType(rotDispType);
			}
		}
	}
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku projektu (*.ifp)
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
AnsiString selector3::saveToIFP(int tileNo)
{
	tileColors currColor;
	String result = "";
	//ID kafelka
	result += IntToStr(tileNo)+","+IntToStr(ID)+",";
	//typ wyswietlania (rotation / up-down)
	if (rotDispType) {
		result += "1,";
	} else {
		result += "0,";
	}
	//teksty w labelach
	result += "\""+label1->Text+"\",";
	result += "\""+label2->Text+"\",";
	result += "\""+label3->Text+"\",";
	result += "\""+label4->Text+"\",";
	//kolor labela
	currColor = static_cast<tileColors>(label1->FontFill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//kolor gdy lewo lub gora
	currColor = static_cast<tileColors>(barColor_L_U->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//kolor gdy srodek
	currColor = static_cast<tileColors>(barColor_M->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//kolor gdy prawo lub dol
	currColor = static_cast<tileColors>(barColor_R_D->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//numer sygnalu gdy lewo lub gora
	result += IntToStr(signalLeft_altUp)+",";
	//numer sygnalu gdy srodek
	result += IntToStr(signalMiddle)+",";
	//numer sygnalu gdy prawo lub dol
	result += IntToStr(signalRight_altDown)+",";
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
void selector3::loadFromIFP(String ifpData, int tileNo)
{
	String switchColor[3], labelColor, temp, labelsText[4];
	int labelStart, labelStop, sigLU, sigM, sigRD, opCond, rotDis;
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
	//odczytujemy informacje o kolorze tla lewo lub gora
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	switchColor[0] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o kolorze tla srodek
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	switchColor[1] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o kolorze tla prawo lub dol
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	switchColor[2] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o sygnale lewo/gora
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	sigLU = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o sygnale srodek
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	sigM = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o sygnale prawo/dol
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	sigRD = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o boolu op.condition
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	opCond = StrToInt(temp.SubString(0,1));
	//odczytujemy informacje o boolu rotation display
    rotDis = StrToInt(ifpData.SubString(labelStart-3,1));
	/********* UAKTUALNIAMY KAFELEK *********/
	barColor_L_U->Fill->Color = intToColor(StrToInt(switchColor[0]));
	barColor_M->Fill->Color = intToColor(StrToInt(switchColor[1]));
	barColor_R_D->Fill->Color = intToColor(StrToInt(switchColor[2]));
	//uzupelniamy informacje dla pokretla
	backgroundKnob->Fill->Color = barColor_R_D->Fill->Color;
	barColor->Fill->Color = barColor_R_D->Fill->Color;
	//uzupelniamy informacje o tekscie
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
	//zmienne wewnetrzne
	signalLeft_altUp = sigLU;
	signalMiddle = sigM;
	signalRight_altDown = sigRD;
	operationCond = opCond;
	rotDispType = rotDis;
	//uaktualniamy wyglad kafelka (up/down LUB rotation)
	selectDisplayType(rotDispType);
}
//---------------------------------------------------------------------------

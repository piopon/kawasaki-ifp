//---------------------------------------------------------------------------
#pragma hdrstop
#include "selector2.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall selector2::selector2(TComponent *myOwner, tileType typeOfTile) : zeroTile(myOwner,typeOfTile)
{
	ID = 4;
	type = typeOfTile;
	//---------  tlo kafelka w trybie up/down  --------
	//pozycja UP
	barColor_L_U = new TRectangle(this);
	barColor_L_U->Parent = this;
	barColor_L_U->HitTest = false;
	barColor_L_U->Height = Height/4;
	barColor_L_U->Width = Width-2;
	barColor_L_U->Position->X = 1;
	barColor_L_U->Position->Y = 25;
	barColor_L_U->Fill->Color = gray;
	barColor_L_U->Stroke->Kind = TBrushKind::bkNone;
	//pozycja DOWN
	barColor_R_D = new TRectangle(this);
	barColor_R_D->Parent = this;
	barColor_R_D->HitTest = false;
	barColor_R_D->Height = Height/4;
	barColor_R_D->Width = Width-2;
	barColor_R_D->Position->X = 1;
	barColor_R_D->Position->Y = 50;
	barColor_R_D->Fill->Color = gray;
	barColor_R_D->Stroke->Kind = TBrushKind::bkNone;
	//---------  tlo kafelka w trybie rotation  --------
	//pokretlo
	backgroundKnob = new TCircle(this);
	backgroundKnob->Parent = this;
	backgroundKnob->HitTest = false;
	backgroundKnob->Height = Height*0.7;
	backgroundKnob->Width = Width*0.7;
	backgroundKnob->Position->X = (Width-backgroundKnob->Width)/2;
	backgroundKnob->Position->Y = (Height-backgroundKnob->Height)/2+15;
	backgroundKnob->Fill->Color = barColor_R_D->Fill->Color;
	//belka na pokretle
	backgroundBar = new TRectangle(backgroundKnob);
	backgroundBar->Parent = backgroundKnob;
	backgroundBar->HitTest = false;
	backgroundBar->Height = 60;
	backgroundBar->Width = 16;
	backgroundBar->Position->X = backgroundKnob->Width/2 - backgroundBar->Width/2;
	backgroundBar->Position->Y = 5;
	backgroundBar->Fill->Color = white1;
	//kolor belki
	barColor = new TRectangle(backgroundBar);
	barColor->Parent = backgroundBar;
	barColor->HitTest = false;
	barColor->Height = backgroundKnob->Height*0.3;
	barColor->Width = backgroundBar->Width;
	barColor->Position->X = 0;
	barColor->Position->Y = 0;
	barColor->Fill->Color = barColor_R_D->Fill->Color;
	//obracamy pokretlo o 45 stopni
    backgroundKnob->RotationAngle = 45.0;
	//--------------------------------------------------
	//teskt w kafelkach - LABEL 1
	label1 = new TLabel(this);
	label1->Parent = this;
	label1->Width = Width;
	label1->Height = Height/4;
	label1->Position->Y = (Height/4)*0;
	label1->Font->Family = "Courier";
	label1->Text = "LABEL 1";
	label1->Font->Size = 12;
	label1->FontFill->Color = black;
	label1->Scale->Y = 2;
	label1->VertTextAlign = label1->TextAlign;
	//teskt w kafelkach - LABEL 2
	label2 = new TLabel(this);
	label2->Parent = this;
	label2->Width = Width;
	label2->Height = Height/4;
	label2->Position->Y = (Height/4)*1;
	label2->Font->Family = "Courier";
	label2->Text = "LABEL 2";
	label2->Font->Size = 12;
	label2->FontFill->Color = black;
	label2->Scale->Y = 2;
	label2->VertTextAlign = label2->TextAlign;
	//teskt w kafelkach - LABEL 3
	label3 = new TLabel(this);
	label3->Parent = this;
	label3->Width = Width;
	label3->Height = Height/4;
	label3->Position->Y = (Height/4)*2;
	label3->Font->Family = "Courier";
	label3->Text = "LABEL 3";
	label3->Font->Size = 12;
	label3->FontFill->Color = black;
	label3->Scale->Y = 2;
	label3->VertTextAlign = label3->TextAlign;
	//teskt w kafelkach - LABEL 4
	label4 = new TLabel(this);
	label4->Parent = this;
	label4->Width = Width;
	label4->Height = Height/4;
	label4->Position->Y = (Height/4)*3;
	label4->Font->Family = "Courier";
	label4->Text = "LABEL 4";
	label4->Font->Size = 12;
	label4->FontFill->Color = black;
	label4->Scale->Y = 2;
	label4->VertTextAlign = label4->TextAlign;
	//uzupelniamy pozostale wartosci
	signalLeft_altUp = 0;
	signalRight_altDown = 0;
	operationCond = false;
	rotDispType = true;
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
__fastcall selector2::~selector2()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	signalLeft_altUp = 0;
	signalRight_altDown = 0;
	operationCond = false;
	rotDispType = true;
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
	/************************************
	 **** USUWAMY ELEMENTY POKRETLA  ****
	 ************************************/
	if (barColor) {
		delete barColor;
		barColor = NULL;
	}
 	if (backgroundBar) {
		delete backgroundBar;
		backgroundBar = NULL;
	}
	if (backgroundKnob) {
		delete backgroundKnob;
		backgroundKnob = NULL;
	}
	if (barColor_R_D) {
		delete barColor_R_D;
		barColor_R_D = NULL;
	}
	if (barColor_L_U) {
		delete barColor_L_U;
		barColor_L_U = NULL;
	}
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* selector2::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new selector2(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* funkcja uaktualniajaca wyglad selektora (up/down  <->  rotation) */
void selector2::selectDisplayType(bool rotType, bool changeState)
{
	if (rotType) {
		//pokazujemytlo w trybie ROTATION
		backgroundKnob->Visible = true;
		backgroundBar->Visible = true;
		//chowamy tlo w trybie UP/DOWN
		barColor_L_U->Visible = false;
		barColor_R_D->Visible = false;
	} else {
		//chowamy tlo w trybie ROTATION
		backgroundKnob->Visible = false;
		backgroundBar->Visible = false;
		//pokazujemy tlo w trybie UP/DOWN
		barColor_L_U->Visible = true;
		barColor_R_D->Visible = true;
    }
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* selector2::showEditor(void)
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
void selector2::setTileContent(void)
{
	try {
		//uzupelniamy wyglad kafelka
		//1. tlo defaultowe (nie ma opcji zmieniac)
		background_altOff->Fill->Color = gray;
		//2. tlo prostokatow gora/dol
		barColor_L_U->Fill->Color = tilePropertiesWindow->colorButtonSwitchL->Color;
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
void selector2::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("selector2")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void selector2::getTileContent(void)
{
	try {
		//pobieramy wyglad kafelka
		//1. tlo
		tilePropertiesWindow->colorButtonSwitchL->Color = barColor_L_U->Fill->Color;
		tilePropertiesWindow->colorButtonSwitchR->Color = barColor_R_D->Fill->Color;
        //2. labele
		tilePropertiesWindow->colorButtonLabel->Color = label1->FontFill->Color;
		tilePropertiesWindow->editLabel1->Text = label1->Text;
		tilePropertiesWindow->editLabel2->Text = label2->Text;
		tilePropertiesWindow->editLabel3->Text = label3->Text;
		tilePropertiesWindow->editLabel4->Text = label4->Text;
		//3. zmienne wewnetrzne kafelka
		tilePropertiesWindow->editSwitchSignalL_altUp->Text = IntToStr(signalLeft_altUp);
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
void selector2::saveToXML(TXMLDocument *xmlSource, int tileNo)
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
		nodeTile->SetAttribute("sigR_D",signalRight_altDown);
		nodeTile->SetAttribute("oper",operationCond);
		nodeTile->SetAttribute("rotDisp",rotDispType);
		//dodajemy kolory tla
		nodeParams = nodeTile->AddChild("backg",0);
		bcgColor = static_cast<tileColors>(barColor_L_U->Fill->Color);
		nodeParams->SetAttribute("L_U",colorToStr(bcgColor));
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
void selector2::loadFromXML(TXMLDocument *xmlSource, int tileNo)
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
				signalRight_altDown = nodeTile->GetAttribute("sigR_D");
				operationCond = nodeTile->GetAttribute("oper");
				rotDispType = nodeTile->GetAttribute("rotDisp");
                String bcgColor, labelColor;
				//odczytujemy informacje o kolorze tla kafelka
				nodeParams = nodeTile->ChildNodes->Get(0);
				bcgColor = nodeParams->GetAttribute("L_U");
				barColor_L_U->Fill->Color = strToColor(bcgColor);
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
AnsiString selector2::saveToIFP(int tileNo)
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
	//kolor gdy prawo lub dol
	currColor = static_cast<tileColors>(barColor_R_D->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//numer sygnalu gdy lewo lub gora
	result += IntToStr(signalLeft_altUp)+",";
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
void selector2::loadFromIFP(String ifpData, int tileNo)
{
	String switchColor[2], labelColor, temp, labelsText[4];
	int labelStart, labelStop, sigLU, sigRD, opCond, rotDis;
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
	//odczytujemy informacje o kolorze tla prawo lub dol
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	switchColor[1] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o sygnale lewo/gora
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	sigLU = StrToInt(temp.SubString(0,labelStop-1));
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
	barColor_R_D->Fill->Color = intToColor(StrToInt(switchColor[1]));
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
	signalRight_altDown = sigRD;
	operationCond = opCond;
	rotDispType = rotDis;
	//uaktualniamy wyglad kafelka (up/down LUB rotation)
	selectDisplayType(rotDispType);
}
//---------------------------------------------------------------------------
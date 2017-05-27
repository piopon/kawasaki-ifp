//---------------------------------------------------------------------------
#pragma hdrstop
#include "palletize.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall palletizeTile::palletizeTile(TComponent *myOwner, tileType typeOfTile) : zeroTile(myOwner,typeOfTile)
{
	ID = 13;
	type = typeOfTile;
	//ikona paletyzacji - tlo BIG
	palletizeB = new TRectangle(this);
	palletizeB->Parent = this;
	palletizeB->HitTest = false;
	palletizeB->Height = 0.9*Height;
	palletizeB->Width = 0.9*Width;
	palletizeB->Position->X = 0.05*Width;
	palletizeB->Position->Y = 0.05*Height;
	palletizeB->Fill->Color = gray;
	//ikona paletyzacji - tlo MEDIUM
	palletizeM = new TRectangle(palletizeB);
	palletizeM->Parent = palletizeB;
	palletizeM->HitTest = false;
	palletizeM->Height = 0.3*palletizeB->Height;
	palletizeM->Width = 0.7*palletizeB->Width;
	palletizeM->Position->X = 0.15*palletizeB->Width;
	palletizeM->Position->Y = 0.4*palletizeB->Height;
	palletizeM->Fill->Color = gray;
	//ikona paletyzacji - tlo SMALL
	palletizeS = new TRectangle(palletizeM);
	palletizeS->Parent = palletizeM;
	palletizeS->HitTest = false;
	palletizeS->Height = 8;
	palletizeS->Width = 8;
	palletizeS->Position->X = 10;
	palletizeS->Position->Y = (palletizeM->Height-palletizeS->Height)/2;
	palletizeS->Fill->Color = gray;
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
	signalNo = 0;
	signalLampNo = 0;
	workNo = 0;
	patternNo = 0;
	frameOn = false;
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
__fastcall palletizeTile::~palletizeTile()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	signalNo = 0;
	signalLampNo = 0;
	workNo = 0;
	patternNo = 0;
	frameOn = false;
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
	if (palletizeS) {
		delete palletizeS;
		palletizeS = NULL;
	}
	if (palletizeM) {
		delete palletizeM;
		palletizeM = NULL;
	}
	if (palletizeB) {
		delete palletizeB;
		palletizeB = NULL;
	}
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* palletizeTile::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new palletizeTile(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* palletizeTile::showEditor(void)
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
void palletizeTile::setTileContent(void)
{
	try {
		//uzupelniamy wyglad kafelka
		background_altOff->Fill->Color = tilePropertiesWindow->colorButtonBackg_altOff->Color;
		palletizeB->Fill->Color = tilePropertiesWindow->colorButtonBackgOn->Color;
		palletizeM->Fill->Color = background_altOff->Fill->Color;
		palletizeS->Fill->Color = palletizeB->Fill->Color;
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
		workNo = tilePropertiesWindow->spinBoxWorkNo->Value;
		patternNo = tilePropertiesWindow->spinBoxPatternNo->Value;
		frameOn = tilePropertiesWindow->checkBoxFrame->IsChecked;
	} catch (...) {
		//tylko wykonanie funkcji ToInt() moze wywolac wyjatek, gdy Text = ""
		signalNo = 0;
		signalLampNo = 0;
	}

}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do kafelka (pobranie wartosci koncowych)
 */
void palletizeTile::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("palletizeTile")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void palletizeTile::getTileContent(void)
{
	try {
		//pobieramy wyglad kafelka (TLO)
		tilePropertiesWindow->colorButtonBackg_altOff->Color = background_altOff->Fill->Color;
		tilePropertiesWindow->colorButtonBackgOn->Color = palletizeB->Fill->Color;
		//pobieramy informacje o labelach kafelka
		tilePropertiesWindow->colorButtonLabel->Color = label1->FontFill->Color;
		tilePropertiesWindow->editLabel1->Text = label1->Text;
		tilePropertiesWindow->editLabel2->Text = label2->Text;
		tilePropertiesWindow->editLabel3->Text = label3->Text;
		tilePropertiesWindow->editLabel4->Text = label4->Text;
		//pobieramy zmienne wewnetrzne kafelka
		tilePropertiesWindow->editButtonSignal_altTop->Text = IntToStr(signalNo);
		tilePropertiesWindow->editLampSignal->Text = IntToStr(signalLampNo);
		tilePropertiesWindow->spinBoxWorkNo->Value = workNo;
		tilePropertiesWindow->spinBoxPatternNo->Value = patternNo;
		tilePropertiesWindow->checkBoxFrame->IsChecked = frameOn;
	} catch (...) {
	}

}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void palletizeTile::saveToXML(TXMLDocument *xmlSource, int tileNo)
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
		nodeTile->SetAttribute("sigNo",signalNo);
		nodeTile->SetAttribute("sigLamp",signalLampNo);
		//dodajemy parametry patternow
		nodeParams = nodeTile->AddChild("pattern",0);
		nodeParams->SetAttribute("no",patternNo);
		nodeParams->SetAttribute("workNo",workNo);
		nodeParams->SetAttribute("frame",frameOn);
		//dodajemy kolory tla
		nodeParams = nodeTile->AddChild("backg",1);
		bcgColor = static_cast<tileColors>(background_altOff->Fill->Color);
		nodeParams->SetAttribute("one",colorToStr(bcgColor));
		bcgColor = static_cast<tileColors>(palletizeB->Fill->Color);
		nodeParams->SetAttribute("two",colorToStr(bcgColor));
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
void palletizeTile::loadFromXML(TXMLDocument *xmlSource, int tileNo)
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
				signalNo = nodeTile->GetAttribute("sigNo");
				signalLampNo = nodeTile->GetAttribute("sigLamp");
				//odczytujemy informacje o patternach
				nodeParams = nodeTile->ChildNodes->Get(0);
				patternNo = nodeParams->GetAttribute("no");
				workNo = nodeParams->GetAttribute("workNo");
				frameOn = nodeParams->GetAttribute("frame");
                String bcgColor, labelColor;
				//odczytujemy informacje o kolorze tla kafelka
				nodeParams = nodeTile->ChildNodes->Get(1);
				bcgColor = nodeParams->GetAttribute("one");
				background_altOff->Fill->Color = strToColor(bcgColor);
				bcgColor = nodeParams->GetAttribute("two");
				palletizeB->Fill->Color = strToColor(bcgColor);
				//uzupelniamy info o pozostalych kolorach
				palletizeM->Fill->Color = background_altOff->Fill->Color;
				palletizeS->Fill->Color = palletizeB->Fill->Color;
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
AnsiString palletizeTile::saveToIFP(int tileNo)
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
	//kolor NUMER 1
	currColor = static_cast<tileColors>(background_altOff->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//kolor NUMER 2
	currColor = static_cast<tileColors>(palletizeB->Fill->Color);
	result += IntToStr(colorToInt(currColor))+",";
	//numer sygnalu
	result += IntToStr(signalNo)+",";
	//numer sygnalu lampki
	result += IntToStr(signalLampNo)+",";
	//numer patternu
	result += IntToStr(patternNo)+",";
	//numer zadania
	result += IntToStr(workNo)+",";
	//widocznosc ramki
	if (frameOn) {
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
void palletizeTile::loadFromIFP(String ifpData, int tileNo)
{
	String bcgColor[2], labelColor, labelsText[4], temp;
    int labelStart, labelStop;
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
	//odczytujemy informacje o kolorze tla NUMER 1
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	bcgColor[0] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o kolorze tla NUMER 2
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	bcgColor[1] = temp.SubString(0,labelStop-1);
	//odczytujemy informacje o sygnale
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	signalNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o sygnale lampki
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	signalLampNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o numerze patternu
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	patternNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o numerze zadania
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos(",");
	workNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o boolu
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	frameOn = StrToInt(temp.SubString(0,1));
	/********* UAKTUALNIAMY KAFELEK *********/
	background_altOff->Fill->Color = intToColor(StrToInt(bcgColor[0]));
	palletizeB->Fill->Color = intToColor(StrToInt(bcgColor[1]));
	palletizeM->Fill->Color = background_altOff->Fill->Color;
	palletizeS->Fill->Color = palletizeB->Fill->Color;
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

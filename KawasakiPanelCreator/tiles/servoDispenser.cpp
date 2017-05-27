//---------------------------------------------------------------------------
#pragma hdrstop
#include "servoDispenser.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall servoDispenser::servoDispenser(TComponent *myOwner, tileType typeOfTile) : zeroTile(myOwner,typeOfTile)
{
	ID = 21;
	type = typeOfTile;
	//kolor tla kafelka w trybie ON
	image = new TImage(this);
	image->Parent = this;
	image->HitTest = false;
	image->Height = Height;
	image->Width = Width;
	image->WrapMode = TImageWrapMode::iwStretch;
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
	partNo = 0;
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
__fastcall servoDispenser::~servoDispenser()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	signalNo = 0;
	partNo = 0;
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
	 **** USUWAMY OBRAZ TLA  ************
	 ************************************/
	if (image) {
		delete image;
		image = NULL;
	}
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* servoDispenser::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new servoDispenser(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* servoDispenser::showEditor(void)
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
void servoDispenser::setTileContent(void)
{
	try {
		//uzupelniamy wyglad kafelka
		partNo = tilePropertiesWindow->comboServoPart->ItemIndex;
		image->Bitmap = tilePropertiesWindow->imageServoPart->Bitmap;
		label1->Text = tilePropertiesWindow->editLabel1->Text;
		label2->Text = tilePropertiesWindow->editLabel2->Text;
		label3->Text = tilePropertiesWindow->editLabel3->Text;
		label4->Text = tilePropertiesWindow->editLabel4->Text;
		label1->ApplyStyleLookup();
		label1->FontFill->Color = black;
		label2->ApplyStyleLookup();
		label2->FontFill->Color = black;
		label3->ApplyStyleLookup();
		label3->FontFill->Color = black;
		label4->ApplyStyleLookup();
		label4->FontFill->Color = black;
		//labele na front
		image->SendToBack();
		//uzupelniamy zmienne wewnetrzne kafelka
		signalNo = tilePropertiesWindow->editButtonSignal_altTop->Text.ToInt();
	} catch (...) {
		//tylko wykonanie funkcji ToInt() moze wywolac wyjatek, gdy Text = ""
        signalNo = 0;
	}

}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do kafelka (pobranie wartosci koncowych)
 */
void servoDispenser::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("servoDispenser")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void servoDispenser::getTileContent(void)
{
	try {
		//pobieramy wyglad kafelka
		tilePropertiesWindow->comboServoPart->ItemIndex = partNo;
		tilePropertiesWindow->imageServoPart->Bitmap = image->Bitmap;
		tilePropertiesWindow->editLabel1->Text = label1->Text;
		tilePropertiesWindow->editLabel2->Text = label2->Text;
		tilePropertiesWindow->editLabel3->Text = label3->Text;
		tilePropertiesWindow->editLabel4->Text = label4->Text;
		//pobieramy zmienne wewnetrzne kafelka
		tilePropertiesWindow->editButtonSignal_altTop->Text = IntToStr(signalNo);
	} catch (...) {
		signalNo = 0;
	}

}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void servoDispenser::saveToXML(TXMLDocument *xmlSource, int tileNo)
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
		//ustawiamy wszystkie parametry
		nodeTile->SetAttribute("ID",ID);
		nodeTile->SetAttribute("type",type);
		nodeTile->SetAttribute("sigNo",signalNo);
		nodeTile->SetAttribute("part",partNo);
		//dodajemy wartosc tekstu z labelu 1
		nodeParams = nodeTile->AddChild("label1",0);
		nodeParams->SetAttribute("color",colorToStr(black));
		nodeParams->Text = label1->Text;
		//dodajemy wartosc tekstu z labelu 2
		nodeParams = nodeTile->AddChild("label2",1);
		nodeParams->SetAttribute("color",colorToStr(black));
		nodeParams->Text = label2->Text;
		//dodajemy wartosc tekstu z labelu 3
		nodeParams = nodeTile->AddChild("label3",2);
		nodeParams->SetAttribute("color",colorToStr(black));
		nodeParams->Text = label3->Text;
		//dodajemy wartosc tekstu z labelu 4
		nodeParams = nodeTile->AddChild("label4",3);
		nodeParams->SetAttribute("color",colorToStr(black));
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
void servoDispenser::loadFromXML(TXMLDocument *xmlSource, int tileNo)
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
				partNo = nodeTile->GetAttribute("part");
				image->Bitmap = mainWindow->loadServoImage(partNo);
				//ustawiamy image
                String labelColor;
				//odczytujemy ustawienia tekstu z labelu 1
				nodeParams = nodeTile->ChildNodes->Get(0);
				labelColor = nodeParams->GetAttribute("color");
                label1->Text = nodeParams->Text;
				label1->ApplyStyleLookup();
				label1->FontFill->Color = strToColor(labelColor);
				//odczytujemy ustawienia tekstu z labelu 2
				nodeParams = nodeTile->ChildNodes->Get(1);
				labelColor = nodeParams->GetAttribute("color");
				label2->Text = nodeParams->Text;
				label2->ApplyStyleLookup();
				label2->FontFill->Color = strToColor(labelColor);
				//odczytujemy ustawienia tekstu z labelu 3
				nodeParams = nodeTile->ChildNodes->Get(2);
				labelColor = nodeParams->GetAttribute("color");
				label3->Text = nodeParams->Text;
				label3->ApplyStyleLookup();
				label3->FontFill->Color = strToColor(labelColor);
				//odczytujemy ustawienia tekstu z labelu 4
				nodeParams = nodeTile->ChildNodes->Get(3);
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
AnsiString servoDispenser::saveToIFP(int tileNo)
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
	//numer sygnalu
	result += IntToStr(signalNo)+",";
	//numer czesci
	result += IntToStr(partNo);
	result += "\n";

	return result;
}
//---------------------------------------------------------------------------
/* metoda odczytania ustawien kafelka z pliku projektu (*.ifp)
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void servoDispenser::loadFromIFP(String ifpData, int tileNo)
{
	String labelColor, temp, labelsText[4];
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
	//odczytujemy informacje o sygnale
	temp = temp.SubString(labelStop+2,temp.Length()+1-(labelStop+2));
	labelStop = temp.Pos(",");
	signalNo = StrToInt(temp.SubString(0,labelStop-1));
	//odczytujemy informacje o boolu
	temp = temp.SubString(labelStop+1,temp.Length()+1-(labelStop+1));
	labelStop = temp.Pos("\r");
	partNo = StrToInt(temp.SubString(0,labelStop-1));
	/********* UAKTUALNIAMY KAFELEK *********/
	//ustawiamy grafike na kafelku
	image->Bitmap = mainWindow->loadServoImage(partNo);
	//zajmujemy sie labelami
	label1->Text = labelsText[0];
	label1->ApplyStyleLookup();
	label1->FontFill->Color = black;
	label2->Text = labelsText[1];
	label2->ApplyStyleLookup();
	label2->FontFill->Color = black;
	label3->Text = labelsText[2];
	label3->ApplyStyleLookup();
	label3->FontFill->Color = black;
	label4->Text = labelsText[3];
	label4->ApplyStyleLookup();
	label4->FontFill->Color = black;
}
//---------------------------------------------------------------------------

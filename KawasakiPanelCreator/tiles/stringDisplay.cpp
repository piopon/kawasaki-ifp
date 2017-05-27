//---------------------------------------------------------------------------
#pragma hdrstop
#include "stringDisplay.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall stringDisplay::stringDisplay(TComponent *myOwner, tileType typeOfTile, int tileNo)
	: zeroTile(myOwner,typeOfTile)
{
	ID = 9;
	type = typeOfTile;
	//startowy sygnal oraz liczba sygnalow
	number = 1;
	size = 1;
	//zapamietujemy zmienna wewnetrzna
	sizeTileNo = tileNo;
	//teskt w kafelku - numer okna
	labelWinNo = new TLabel(this);
	labelWinNo->Parent = this;
	labelWinNo->Width = Width;
	labelWinNo->Height = Height/4;
	labelWinNo->Position->Y = 25;
	labelWinNo->Scale->Y = 2;
	labelWinNo->TextAlign = TTextAlign::taCenter;
	labelWinNo->VertTextAlign = TTextAlign::taLeading;
	labelWinNo->Font->Family = "Courier";
	labelWinNo->Font->Size = 12;
	labelWinNo->Text = "WIN NO: ";
	labelWinNo->FontFill->Color = black;
	//teskt w kafelkach - LABEL 2
	labelTileNfo = new TLabel(this);
	labelTileNfo->Parent = this;
	labelTileNfo->Width = Width;
	labelTileNfo->Height = Height/4;
	labelTileNfo->Position->Y = 50;
	labelTileNfo->Scale->Y = 2;
	labelTileNfo->TextAlign = TTextAlign::taCenter;
	labelTileNfo->VertTextAlign = TTextAlign::taLeading;
	labelTileNfo->Font->Family = "Courier";
	labelTileNfo->Font->Size = 12;
	labelTileNfo->Text = "NO / SIZE";
	labelTileNfo->FontFill->Color = black;
	//obwiednia kafelka zalezna od rozmiaru
	boundRect = new TRectangle(this);
	boundRect->Parent = this;
	boundRect->HitTest = false;
	boundRect->Fill->Color = NULL;
	boundRect->Stroke->Color = background_altOff->Fill->Color;
	boundRect->StrokeThickness = 2;
	boundRect->StrokeCap = TStrokeCap::scRound;
	boundRect->StrokeDash = TStrokeDash::sdDot;
	boundRect->StrokeJoin = TStrokeJoin::sjRound;
	boundRect->SendToBack();
	boundRect->Sides >> TSide::sdTop >> TSide::sdBottom >> TSide::sdLeft >> TSide::sdRight;
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
__fastcall stringDisplay::~stringDisplay()
{
	/************************************
	 **** ZERUJEMY ZMIENNE WEWNETRZNE  **
	 ************************************/
	number = 1;
	size = 1;
	/************************************
	 **** USUWAMY PROSTOKAT OKALAJACY  **
	 ************************************/
	if (boundRect) {
		delete boundRect;
		boundRect = NULL;
	}
	/************************************
	 **** USUWAMY LABELE  ***************
	 ************************************/
	if (labelTileNfo) {
		delete labelTileNfo;
		labelTileNfo = NULL;
	}
	if (labelWinNo) {
		delete labelWinNo;
		labelWinNo = NULL;
	}
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* stringDisplay::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new stringDisplay(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* stringDisplay::showEditor(void)
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
/* metoda do uaktualniania prostokata okalajacego w zaleznosci od miejsca
 */
void stringDisplay::bound(int tileNo, int ofTiles)
{
	boundRect->Position->X = -4; boundRect->Position->Y = -4;
	boundRect->Height = Height+8; boundRect->Width = Width+8;
	boundRect->Stroke->Color = background_altOff->Fill->Color;
	if (tileNo==1) {
		boundRect->Sides << TSide::sdLeft << TSide::sdTop << TSide::sdBottom;
		ofTiles==1 ? boundRect->Sides << TSide::sdRight : boundRect->Sides >> TSide::sdRight;
	} else if (tileNo==2) {
		boundRect->Sides >> TSide::sdRight;
		boundRect->Sides << TSide::sdTop << TSide::sdBottom;
		ofTiles==2 ? boundRect->Sides << TSide::sdRight : boundRect->Sides >> TSide::sdRight;
	} else if (tileNo==3) {
		boundRect->Sides >> TSide::sdLeft;
		boundRect->Sides << TSide::sdRight << TSide::sdTop << TSide::sdBottom;
	}
}
//---------------------------------------------------------------------------
/* metoda do uaktualniania prostokata okalajacego
 */
void stringDisplay::bound(bool left, bool right, bool top, bool bottom)
{
	boundRect->Position->X = -4;
	boundRect->Position->Y = -4;
	boundRect->Height = Height+8;
	boundRect->Width = Width+8;
	//sprawdzamy poszczegolne krawedzie
	if (left) {
		boundRect->Sides << TSide::sdLeft;
	}
	if(right){
		boundRect->Sides << TSide::sdRight;
	}
	if(top){
		boundRect->Sides << TSide::sdTop;
	}
	if(bottom){
		boundRect->Sides << TSide::sdBottom;
    }
}
//---------------------------------------------------------------------------
/* metoda do uaktualniania koloru tekstu w zaleznosci od koloru tla
 */
void stringDisplay::adjustLabelsColor(tileColors backgroundColor)
{
	tileColors labelColor;
	if(backgroundColor==gray) {
    	labelColor = black;
	} else if (backgroundColor==blue) {
		labelColor = black;
	} else if (backgroundColor==red) {
		labelColor = black;
	} else if (backgroundColor==orange) {
		labelColor = black;
	} else if (backgroundColor==green) {
		labelColor = black;
	} else if (backgroundColor==paleBlue) {
		labelColor = black;
	} else if (backgroundColor==yellow) {
		labelColor = black;
	} else if (backgroundColor==white1) {
		labelColor = black;
	} else if (backgroundColor==pink) {
		labelColor = black;
	} else if (backgroundColor==white2) {
		labelColor = black;
	} else if (backgroundColor==black) {
		labelColor = white1;
	} else if (backgroundColor==cyan) {
		labelColor = black;
	} else if (backgroundColor==navy) {
		labelColor = white1;
	} else if (backgroundColor==redBrown) {
		labelColor = black;
	} else if (backgroundColor==darkGreen) {
		labelColor = white1;
	} else if (backgroundColor==lavender) {
    	labelColor = black;
	}
	//uaktualniamy kolor labeli
	labelWinNo->ApplyStyleLookup();
	labelWinNo->FontFill->Color = labelColor;
	labelTileNfo->ApplyStyleLookup();
	labelTileNfo->FontFill->Color = labelColor;
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z edytora do kafelka (pobranie wartosci koncowych)
 */
void stringDisplay::setTileContent(void)
{
	try {
		//uzupelniamy wyglad kafelka
		background_altOff->Fill->Color = tilePropertiesWindow->colorButtonBackg_altOff->Color;
		//numer i rozmiar okna
		number = tilePropertiesWindow->spinBoxWinNr->Value;
		size = tilePropertiesWindow->spinBoxWinSize->Value;
		//uzupelniamy prostokat okalajacy
		boundRect->Stroke->Color = background_altOff->Fill->Color;
		bound(sizeTileNo,size);
		//uzupelniamy tekst w kafelkach
		labelWinNo->Text = "WIN NO: "+IntToStr(number);
		labelTileNfo->Text = IntToStr(sizeTileNo)+" / "+IntToStr(size);
		//uaktualniamy kolor tekstu
		adjustLabelsColor(static_cast<tileColors>(background_altOff->Fill->Color));
	} catch (...) {
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do kafelka (pobranie wartosci koncowych)
 */
void stringDisplay::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("stringDisplay")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void stringDisplay::getTileContent(void)
{
	try {
		//pobieramy wyglad kafelka
		tilePropertiesWindow->colorButtonBackg_altOff->Color = background_altOff->Fill->Color;
		//numer i rozmiar okna
		tilePropertiesWindow->spinBoxWinNr->Value = number;
		tilePropertiesWindow->spinBoxWinSize->Value = size;
	} catch (...) {
	}
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void stringDisplay::saveToXML(TXMLDocument *xmlSource, int tileNo)
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
		nodeTile->SetAttribute("num",number);
		nodeTile->SetAttribute("size",size);
		//formatujemy XML (opcja doNodeAutoIndent slabo dziala)
		xmlSource->XML->Text = FormatXMLData(xmlSource->XML->Text);
		xmlSource->Active = true;
	}
}
//---------------------------------------------------------------------------
/* metoda odczytania ustawien z pliku
 * arg: fileName - nazwa pliku y ktorego odcyztujemz
 */
void stringDisplay::loadFromXML(TXMLDocument *xmlSource, int tileNo)
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
				number = nodeTile->GetAttribute("num");
				size = nodeTile->GetAttribute("size");
				//pokazujemy odpowiedni prostokat okalajacy
				bound(true,true,true,true);
				//uzupelniamy tekst w kafelkach
				labelWinNo->Text = "WIN NO: "+IntToStr(number);
				labelTileNfo->Text = IntToStr(sizeTileNo)+" / "+IntToStr(size);
			    //uaktualniamy kolor tekstu
				adjustLabelsColor(static_cast<tileColors>(background_altOff->Fill->Color));
			}
		}
	}
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku projektu (*.ifp)
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
AnsiString stringDisplay::saveToIFP(int tileNo)
{
	tileColors currColor;
	String result = "";
	if (sizeTileNo==1) {
		//ID kafelka
		result += IntToStr(tileNo)+","+IntToStr(ID)+",";
		//numer okna
		result += IntToStr(number)+",";
		//rozmiar okna
		result += IntToStr(size)+",";
		//kolor tla
		currColor = static_cast<tileColors>(background_altOff->Fill->Color);
		result += IntToStr(colorToInt(currColor));
		result += "\n";
	}
	return result;
}
//---------------------------------------------------------------------------
/* metoda odczytania ustawien kafelka z pliku projektu (*.ifp)
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void stringDisplay::loadFromIFP(String ifpData, int tileNo)
{
	String temp = ifpData;
	int bcgColor, commaPos;
	//omijamy tileNo oraz ID (odczytane juz wczesniej) i odczytujemy numer okna
	commaPos = temp.Pos(",")+1;
	temp = temp.SubString(commaPos,temp.Length()-commaPos+1);
	commaPos = temp.Pos(",")+1;
	number = temp.SubString(commaPos,1).ToInt();
	//odczytujemy rozmiar
	temp = temp.SubString(commaPos,temp.Length()-commaPos+1);
	commaPos = temp.Pos(",")+1;
	size = temp.SubString(commaPos,1).ToInt();
	//odczytujemy kolor
	temp = temp.SubString(commaPos,temp.Length()-commaPos+1);
	commaPos = temp.Pos(",")+1;
	bcgColor = temp.SubString(commaPos,temp.Length()-commaPos).ToInt();
	background_altOff->Fill->Color = intToColor(StrToInt(bcgColor));
	boundRect->Stroke->Color = background_altOff->Fill->Color;
	//pokazujemy odpowiedni prostokat okalajacy
	bound(1,size);
	//uzupelniamy tekst w kafelkach
	labelWinNo->Text = "WIN NO: "+IntToStr(number);
	labelTileNfo->Text = IntToStr(sizeTileNo)+" / "+IntToStr(size);
    //uaktualniamy kolor tekstu
	adjustLabelsColor(static_cast<tileColors>(background_altOff->Fill->Color));
}

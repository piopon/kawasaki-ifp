//---------------------------------------------------------------------------
#pragma hdrstop
#include "zeroTile.h"
#include "tileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* konstruktor
 * arg: myOwner - wlasciciel (konieczny przez dziedziczenie po TPanel)
 */
__fastcall zeroTile::zeroTile(TComponent *myOwner,tileType typeOfTile,tileSource dataSource) : TPanel(myOwner)
{
	//sprawdzamy jakiego typu jest to kafelek
	type = typeOfTile;
	//ustawiamy parametry kafelka zerowego
	ID = 0;
	Height = 100;
	Width  = 80;
	size = 1;
	//kolor tla kafelka
	background_altOff = new TRectangle(this);
	background_altOff->Parent = this;
	background_altOff->HitTest = false;
	background_altOff->Height = Height;
	background_altOff->Width = Width;
	background_altOff->Fill->Color = gray;
	background_altOff->BringToFront();
	//pole z ID kafelka
	idField = new TRectangle(this);
	idField->Parent = this;
	idField->HitTest = false;
	idField->Height = 13;
	idField->Width = 13;
	idField->Position->X = background_altOff->Width-idField->Width+2;
	idField->Position->Y = background_altOff->Height-idField->Height+2;
	idField->Fill->Color = claSpringgreen;
	idField->BringToFront();
	idField->Visible = false;
	//tekst zawierajacy ID kafelka
	idText = new TLabel(idField);
	idText->Parent = idField;
	idText->HitTest = false;
	idText->Align = TAlignLayout::alContents;
	idText->TextAlign = TTextAlign::taCenter;
	idText->VertTextAlign = TTextAlign::taCenter;
	idText->Font->Size = 8;
	idText->Text = IntToStr(ID);
	idText->Visible = true;
	//pole z numerem kafelka
	noField = new TRectangle(this);
	noField->Parent = this;
	noField->HitTest = false;
	noField->Height = 13;
	noField->Width = 17;
	noField->Position->X = background_altOff->Width-noField->Width+2;
	noField->Position->Y = -3;
	noField->Fill->Color = claAqua;
	noField->BringToFront();
	noField->Visible = false;
	//tekst zawierajacy numer kafelka
	noText = new TLabel(noField);
	noText->Parent = noField;
	noText->HitTest = false;
	noText->Align = TAlignLayout::alContents;
	noText->TextAlign = TTextAlign::taCenter;
	noText->VertTextAlign = TTextAlign::taCenter;
	noText->Font->Size = 8;
	noText->Text = IntToStr(no);
	noText->Visible = true;
	//ustawiamy przycisk usuwania danego kafelka
	delButton = new TButton(this);
	delButton->Parent = this;
	delButton->Visible = false;
	delButton->Height = 25;
	delButton->Width = 25;
	delButton->Position->X = -4;
	delButton->Position->Y = Height-delButton->Height+5;
	delButton->StyleLookup = "deleteTileStyle1";
	//ustawiamy przycisk zamiany danego kafelka
	swapButton = new TButton(this);
	swapButton->Parent = this;
	swapButton->Visible = false;
	swapButton->Height = 27;
	swapButton->Width = 27;
	swapButton->Position->X = Width/2-swapButton->Width/2;
	swapButton->Position->Y = Height-swapButton->Height+6;
	swapButton->StyleLookup = "swapTileStyle1";
	//ustawiamy przycisk anulowania zamiany kafelka
	swapCancelButton = new TButton(this);
	swapCancelButton->Parent = this;
	swapCancelButton->Visible = false;
	swapCancelButton->Height = 50;
	swapCancelButton->Width = 50;
	swapCancelButton->Position->X = (Width-swapCancelButton->Width)/2;
	swapCancelButton->Position->Y = (Height-swapCancelButton->Height)/2-3;
	swapCancelButton->StyleLookup = "swapCancelTileStyle1";
	//ustawiamy przycisk dodania danego kafelka do wzorcow
	addPatternButton = new TButton(this);
	addPatternButton->Parent = this;
	addPatternButton->Visible = false;
	addPatternButton->Height = 30;
	addPatternButton->Width = 30;
	addPatternButton->Position->X = -3;
	addPatternButton->Position->Y = -4;
	addPatternButton->StyleLookup = "addPatternTileStyle1";
	//ustawiamy przycisk kopiowania danego kafelka
	copyButton = new TButton(this);
	copyButton->Parent = this;
	copyButton->Visible = false;
	copyButton->Height = 30;
	copyButton->Width = 30;
	copyButton->Position->X = (Width-copyButton->Width)/2;
	copyButton->Position->Y = -4;
	copyButton->StyleLookup = "copyTileStyle1";
	//ustawiamy przycisk wklejania danego kafelka
	pasteButton = new TButton(this);
	pasteButton->Parent = this;
	pasteButton->Visible = false;
	pasteButton->Height = 30;
	pasteButton->Width = 30;
	pasteButton->Position->X = Width-pasteButton->Width/2-10;
	pasteButton->Position->Y = -4;
	pasteButton->StyleLookup = "pasteTileStyle1";
	//ustawiamy przycisk anulowania dodania kafelka do wzorcow
	cancelActionButton = new TButton(this);
	cancelActionButton->Parent = this;
	cancelActionButton->Visible = false;
	cancelActionButton->Height = 50;
	cancelActionButton->Width = 50;
	cancelActionButton->Position->X = (Width-cancelActionButton->Width)/2;
	cancelActionButton->Position->Y = (Height-cancelActionButton->Height)/2-3;
	cancelActionButton->StyleLookup = "cancelActionTileStyle1";
	//ustawiamy przycisk modyfikacji danego kafelka
	modButton = new TButton(this);
	modButton->Parent = this;
	modButton->Visible = false;
	modButton->Height = 50;
	modButton->Width = 50;
	modButton->Position->X = (Width-modButton->Width)/2;
	modButton->Position->Y = (Height-modButton->Height)/2-3;
	modButton->StyleLookup = "modifyTileStyle1";
	//ustawiamy przycisk akceptowania danego kafelka
	doneButton = new TButton(this);
	doneButton->Parent = this;
	doneButton->Visible = false;
	doneButton->Enabled = true;
	doneButton->Height = 32;
	doneButton->Width = 32;
	doneButton->Position->X = Width-doneButton->Width/2-10;
	doneButton->Position->Y = Height-doneButton->Height+7;
	doneButton->StyleLookup = "doneTileStyle1";
	//jezeli jest to kafelek zablokowany to nie udostepniamy przycisku kasowania
	if (type == patternLocked) {
		delButton->Enabled = false;
		modButton->Enabled = false;
	}
	//definiujemy metody PRIVATE (dziedziczone dla kazdego z kafelkow ale nie mozliwe do zmiany
	//efiniujemy zdarzenia myszy
	OnMouseEnter = tileMouseEnter;
	OnMouseLeave = tileMouseLeave;
	//definiujemy efekt podswietlenia panelu
	glow = new TGlowEffect(myOwner);
	glow->Parent = this;
	glow->Softness = 0.5;
	glow->Opacity  = 1;
	glow->Enabled  = false;
	if (type == tileType::patternLocked) {
		glow->GlowColor = claGreen;
	} else if(type == tileType::patternCustom){
		glow->GlowColor = claOrange;
	} else if(type == tileType::regular){
		glow->GlowColor = claBlue;
	} else {
		glow->GlowColor = claBlue;
	}
	//definiujemy efekt wyroznienia panelu
	emphasize = new TColorAnimation(this->background_altOff);
	emphasize->Parent = this->background_altOff;
	emphasize->PropertyName = "Fill.Color";
	emphasize->StopValue = claGold;
	emphasize->Inverse = true;
	emphasize->Duration = 0.4;
	//definiujemy efekt rozmycia
	selection = new TGlowEffect(myOwner);
	selection->Parent = this->background_altOff;
	selection->Softness = 0.3;
	selection->Opacity  = 1;
	selection->Enabled = false;
	selection->SendToBack();
	selection->GlowColor = glow->GlowColor;
	selected = false;
	//definiujemy linie 1 niekompatybilnego kafelka
	cross1 = new TLine(this);
	cross1->Parent = this;
	cross1->Width = Width;
	cross1->Height = Height;
	cross1->Stroke->Color = claRed;
	cross1->StrokeThickness = 3;
	cross1->LineType = TLineType::ltDiagonal;
	cross1->SendToBack();
	cross1->Visible = false;
	//definiujemy linie 2 niekompatybilnego kafelka
	cross2 = new TLine(this);
	cross2->Parent = this;
	cross2->Width = Width;
	cross2->Height = Height;
	cross2->Stroke->Color = claRed;
	cross2->StrokeThickness = 3;
	cross2->LineType = TLineType::ltDiagonal;
	cross2->RotationAngle = 77;
	cross2->SendToBack();
	cross2->Visible = false;
}
//---------------------------------------------------------------------------
/* konstruktor kopiuj¹cy
 * arg: original -
 */
zeroTile::zeroTile(const zeroTile &original) : TPanel(original.Owner)
{

}
//---------------------------------------------------------------------------
//destruktor
//---------------------------------------------------------------------------
__fastcall zeroTile::~zeroTile()
{
	/**************************************
	 ** USUWAMY PODSWIETLENIE+ZAZNACZENIE *
	 **************************************/
	if (cross2) {
		delete cross2;
		cross2 = NULL;
	}
	if (cross1) {
		delete cross1;
		cross1 = NULL;
	}
	if (selection) {
		delete selection;
		selection = NULL;
	}
	if (emphasize) {
		delete emphasize;
		emphasize = NULL;
	}
	if (glow) {
		delete glow;
		glow = NULL;
	}
	/************************************
	 **** USUWAMY GUZIKI  ***************
	 ************************************/
	if (doneButton) {
		delete doneButton;
		doneButton = NULL;
	}
	if (modButton) {
		delete modButton;
		modButton = NULL;
	}
	if (cancelActionButton) {
		delete cancelActionButton;
		cancelActionButton = NULL;
	}
	if (pasteButton) {
		delete pasteButton;
		pasteButton = NULL;
	}
	if (copyButton) {
		delete copyButton;
		copyButton = NULL;
	}
	if (addPatternButton) {
		delete addPatternButton;
		addPatternButton = NULL;
	}
	if (swapCancelButton) {
		delete swapCancelButton;
		swapCancelButton = NULL;
	}
	if (swapButton) {
		delete swapButton;
		swapButton = NULL;
	}
	if (delButton) {
		delete delButton;
		delButton = NULL;
	}
	/**************************************
	 ** USUWAMY TLO KAFELKA ***************
	 **************************************/
	if (background_altOff) {
		delete background_altOff;
		background_altOff = NULL;
	}
	/**************************************
	 ** ZERUJEMY PARENTA ******************
	 **************************************/
	if (Parent) {
		Parent = NULL;
    }
}
//---------------------------------------------------------------------------
//operator przypisania
//---------------------------------------------------------------------------
zeroTile& zeroTile::operator=(zeroTile &original)
{
	Height = original.Height;
	Width  = original.Width;
	ID     = original.ID;
	return *this;
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie nowego domyslnego kafelka)
 * ret: baseTile =
 * arg: myOwner -
 */
zeroTile* zeroTile::createTile(TComponent *myOwner, tileType typeOfTile)
{
	return new zeroTile(myOwner,typeOfTile);
}
//---------------------------------------------------------------------------
/* funkcje symulujace "konstruktor wirtualny" (tworzenie kopii danego kafelka)
 * ret: baseTile =
 */
zeroTile* zeroTile::copyTile()
{
	return new zeroTile(*this);
}
//---------------------------------------------------------------------------
//akcja na wjazd myszy na kafelek
//---------------------------------------------------------------------------
void __fastcall zeroTile::tileMouseEnter(TObject *Sender)
{
	glow->Parent=this;
	glow->Enabled=true;
}
//---------------------------------------------------------------------------
//akcja na wyjazd myszy z kafelka
//---------------------------------------------------------------------------
void __fastcall zeroTile::tileMouseLeave(TObject *Sender)
{
	glow->Enabled=false;
	TControl *parent = dynamic_cast<TControl*>(this->Owner);
	parent->Repaint();
}
//---------------------------------------------------------------------------
//uaktualnienie kolorow poswiaty kafelka
//---------------------------------------------------------------------------
void zeroTile::setGlowColor(unsigned int regular, unsigned int protect, unsigned int pattern)
{
	if (type == tileType::patternLocked) {
		this->glow->GlowColor = pattern;
	} else if(type == tileType::patternCustom){
		this->glow->GlowColor = protect;
	} else if(type == tileType::regular){
		this->glow->GlowColor = regular;
	} else {
		glow->GlowColor = regular;
	}
}
//---------------------------------------------------------------------------
//uaktualnienie koloru wyroznania kafelka
//---------------------------------------------------------------------------
void zeroTile::setEmphasizeColor(unsigned int newColor)
{
	emphasize->StopValue = newColor;
}
//---------------------------------------------------------------------------
//pokazanie/chowanie niekompatybilnego kafelka
//---------------------------------------------------------------------------
void zeroTile::viewIncompatibleTile(bool show)
{
	if (show) {
		cross1->BringToFront();
		cross2->BringToFront();
	} else {
		cross1->SendToBack();
		cross2->SendToBack();
	}
	cross1->Visible = show;
	cross2->Visible = show;
}
//---------------------------------------------------------------------------
//wybranie kafelka
//---------------------------------------------------------------------------
void zeroTile::selectTile(bool state)
{
	if (state) {
		selection->BringToFront();
	} else {
		selection->SendToBack();
	}
	selection->Enabled = state;
	selected = state;
}
//---------------------------------------------------------------------------
//sprawdzenie czy kafelek jest wybrany
//---------------------------------------------------------------------------
bool zeroTile::isSelected(void)
{
    return this->selected;
}
//---------------------------------------------------------------------------
//animacja na wskazanym kafelku (podkreslenie danego kafelka)
//---------------------------------------------------------------------------
void zeroTile::emphasizeTile(void)
{
	//sprawdzamy czy animacja nie jest uruchomiona
	//trzeba to zrobic by nie zmienic koloru startowego (pierwsza linijka to pobieranie)
	if(!emphasize->Running) {
		//zapamietujemy startowy kolor
		TColor startColor = static_cast<TColor>(getBackgroundColor());
		emphasize->StartValue = startColor;
		//odtwarzamy animacje
		emphasize->Start();
    }
}
//---------------------------------------------------------------------------
/* metoda do ustawiania tla kafelka
 * arg: newColor - nowy kolor danego kafla
 */
void zeroTile::setBackgroundColor(tileColors newColor)
{
	this->background_altOff->Fill->Color = newColor;
}
//---------------------------------------------------------------------------
/* metoda do pobierania tla kafelka
 * ret: color - nowy kolor danego kafla
 */
tileColors zeroTile::getBackgroundColor(void)
{
	return static_cast<tileColors>(this->background_altOff->Fill->Color);
}
//---------------------------------------------------------------------------
/* metoda do odczytywania typu danego kafelka
 * arg: tileType - typ kafelka
 */
tileType zeroTile::getType(void)
{
	return this->type;
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania okienka edycyjnego danego kafelka
 */
zeroTile* zeroTile::showEditor(void)
{
	//chowamy przyciski z menu
	menuOptions(false);
	//pokazujemy okienko edycji kafelka
	tilePropertiesWindow->callerTile = this;
	tilePropertiesWindow->ShowModal();
	//zwracamy adres uaktualnionego kafelka
	//(jezeli zmieni sie ID to bedzie zupelnie nowy adres)
	return tilePropertiesWindow->callerTile;
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z edytora do kafelka (pobranie wartosci koncowych)
 */
void zeroTile::setTileContent(void)
{
	background_altOff->Fill->Color = tilePropertiesWindow->colorButtonBackg_altOff->Color;
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z edytora do kafelka (pobranie wartosci koncowych)
 */
void zeroTile::setTileContent(zeroTile *originalTile)
{
	if (originalTile && originalTile->ClassNameIs("zeroTile")) {
		originalTile->getTileContent();
		this->setTileContent();
	}
}
//---------------------------------------------------------------------------
/* metoda do wyslania zmian z kafelka do edytora (pobranie wartosci poczatkowych)
 */
void zeroTile::getTileContent(void)
{
    //KAFELEK PUSTY - wiec wszystkie informacje sa zerowe!!!
	tilePropertiesWindow->editLabel1->Text = "";
	tilePropertiesWindow->editLabel2->Text = "";
	tilePropertiesWindow->editLabel3->Text = "";
	tilePropertiesWindow->editLabel4->Text = "";
	tilePropertiesWindow->colorButtonBackg_altOff->Color = background_altOff->Fill->Color;
	tilePropertiesWindow->colorButtonBackgOn->Color = claNull;
	tilePropertiesWindow->colorButtonLabel->Color = claNull;
	tilePropertiesWindow->spinBoxSigNo->Value = 0;
}
//---------------------------------------------------------------------------
/* metoda do wyswietlania opcji
 * arg: show - pokazac / schowac buttony do opcji
 */
void zeroTile::menuOptions(bool show, bool swappingTiles, bool insertingPattern, int tilesSelectedNo)
{
	if (swappingTiles && insertingPattern) {
		//blad logiczny - nie mozna jednoczesnie zamieniac kafelki i wkladac patterna
        return;
	}
	//w zaleznosci od kafelka i obecnego trybu wyswietlamy odpowiednie opcje
	if (type == regular) {
		if (tilesSelectedNo>1) {
			this->cancelActionButton->Visible = false;
			this->delButton->Visible = show;
			this->swapButton->Visible = false;
			this->swapCancelButton->Visible = false;
			this->addPatternButton->Visible = false;
			this->modButton->Visible = false;
			this->copyButton->Visible = false;
			this->pasteButton->Visible = false;
			this->doneButton->Visible = show;
		} else {
			if (swappingTiles) {
				this->cancelActionButton->Visible = false;
				this->delButton->Visible = false;
				this->swapButton->Visible = false;
				this->swapCancelButton->Visible = show;
				this->addPatternButton->Visible = false;
				this->modButton->Visible = false;
				this->copyButton->Visible = false;
				this->pasteButton->Visible = false;
				this->doneButton->Visible = show;
			} else if (insertingPattern) {
				this->cancelActionButton->Visible = show;
				this->delButton->Visible = false;
				this->swapButton->Visible = false;
				this->swapCancelButton->Visible = false;
				this->addPatternButton->Visible = false;
				this->modButton->Visible = false;
				this->copyButton->Visible = false;
				this->pasteButton->Visible = false;
				this->doneButton->Visible = show;
			} else {
				this->cancelActionButton->Visible = false;
				this->swapButton->Visible = show;
				//sprawdzamy czy nie pokazujemy menu na zeroTile
				if(ID>0){
					//to jest zeroTile - nie mozna usunac i dodac do patternow!
					this->delButton->Visible = show;
					this->addPatternButton->Visible = show;
					this->copyButton->Visible = show;
				} else {
					this->delButton->Visible = false;
					this->addPatternButton->Visible = false;
					this->copyButton->Visible = false;
				}
				this->swapCancelButton->Visible = false;
				this->pasteButton->Visible = show;
				this->modButton->Visible = show;
				this->doneButton->Visible = show;
			}
		}
	} else if (type == patternLocked) {
        this->cancelActionButton->Visible = false;
		this->delButton->Visible = false;
		this->swapButton->Visible = false;
		this->swapCancelButton->Visible = false;
		this->addPatternButton->Visible = false;
		this->modButton->Visible = false;
		this->copyButton->Visible = false;
		this->pasteButton->Visible = false;
		this->doneButton->Visible = show;
	} else {
        this->cancelActionButton->Visible = false;
		this->delButton->Visible = show;
		this->swapButton->Visible = false;
		this->swapCancelButton->Visible = false;
		this->addPatternButton->Visible = false;
		this->modButton->Visible = show;
		this->copyButton->Visible = false;
		this->pasteButton->Visible = false;
		this->doneButton->Visible = show;
    }
}
//---------------------------------------------------------------------------
/* metoda do odczytywania ID kafelka
 * ret: int -
 */
int zeroTile::getID(void)
{
	return this->ID;
}
//---------------------------------------------------------------------------
//funkcja pokazywania/chowania identyfikatorow kafelka
//---------------------------------------------------------------------------
void zeroTile::viewID(bool visible)
{
	this->idField->Visible = visible;
	idText->Text = IntToStr(ID);
	idText->UpdateStyle();
}
//---------------------------------------------------------------------------
//funkcja pokazywania/chowania numeru kafelka
//---------------------------------------------------------------------------
void zeroTile::viewNO(int nr, bool visible)
{
	this->noField->Visible = visible;
    no = nr;
	noText->Text = IntToStr(no);
	noText->UpdateStyle();
}
//---------------------------------------------------------------------------
/* metoda do zapisywania ID mastera (kafelka nadrzednego/glownego)
 */
void zeroTile::setUniqueID(int newID)
{
	uniqueID = newID;
}
//---------------------------------------------------------------------------
/* metoda do odczytywania ID mastera (kafelka nadrzednego/glownego)
 */
int zeroTile::getUniqueID(void)
{
	return uniqueID;
}
//---------------------------------------------------------------------------
//funkcja do zamiany koloru kafelka na liczbe calkowita
//---------------------------------------------------------------------------
int zeroTile::colorToInt(tileColors color)
{
	int result;
	switch (color) {
		case gray: 			result=0; break;
		case blue: 			result=1; break;
		case red: 			result=2; break;
		case orange: 		result=3; break;
		case green: 		result=4; break;
		case paleBlue: 		result=5; break;
		case yellow: 		result=6; break;
		case white1,white2: result=7; break;
		case pink: 			result=8; break;
		case black: 		result=10; break;
		case cyan: 			result=11; break;
		case navy: 			result=12; break;
		case redBrown: 		result=13; break;
		case darkGreen:		result=14; break;
		case lavender: 		result=15; break;
		default: break;
    }
	return result;
}
//---------------------------------------------------------------------------
//funkcja do zamiany liczby calkowitej na kolor kafelka
//---------------------------------------------------------------------------
tileColors zeroTile::intToColor(int color)
{
	tileColors result;
	switch (color) {
		case 0:	result=gray; break;
		case 1: result=blue; break;
		case 2: result=red; break;
		case 3: result=orange; break;
		case 4: result=green; break;
		case 5: result=paleBlue; break;
		case 6: result=yellow; break;
		case 7: result=white1; break;
		case 8: result=pink; break;
		case 9: result=white2; break;
		case 10:result=black; break;
		case 11:result=cyan; break;
		case 12:result=navy; break;
		case 13:result=redBrown; break;
		case 14:result=darkGreen; break;
		case 15:result=lavender; break;
		default: break;
    }
	return result;
}
//---------------------------------------------------------------------------
//funkcja do zamiany koloru kafelka na string
//---------------------------------------------------------------------------
String zeroTile::colorToStr(tileColors color)
{
	String result;
	switch (color) {
		case gray: result="gray"; break;
		case blue: result="blue"; break;
		case red: result="red"; break;
		case orange: result="orange"; break;
		case green: result="green"; break;
		case paleBlue: result="paleBlue"; break;
		case yellow: result="yellow"; break;
		case white1,white2: result="white1"; break;
		case pink: result="pink"; break;
		case black: result="black"; break;
		case cyan: result="cyan"; break;
		case navy: result="navy"; break;
		case redBrown: result="redBrown"; break;
		case darkGreen: result="darkGreen"; break;
		case lavender: result="lavender"; break;
		default: break;
    }
	return result;
}
//---------------------------------------------------------------------------
//funkcja do zamiany stringu na kolor kafelka
//---------------------------------------------------------------------------
tileColors zeroTile::strToColor(String color)
{
	tileColors result;
	if(color=="gray") {
		result=gray;
	} else if(color=="blue") {
		result=blue;
	} else if(color=="red") {
		result=red;
	} else if(color=="orange") {
		result=orange;
	} else if(color=="green") {
		result=green;
	} else if(color=="paleBlue") {
		result=paleBlue;
	} else if(color=="yellow") {
		result=yellow;
	} else if(color=="white1") {
		result=white1;
	} else if(color=="pink") {
		result=pink;
	} else if(color=="black") {
		result=black;
	} else if(color=="cyan") {
		result=cyan;
	} else if(color=="navy") {
		result=navy;
	} else if(color=="redBrown") {
		result=redBrown;
	} else if(color=="darkGreen") {
		result=darkGreen;
	} else if(color=="lavender") {
		result=lavender;
	} else {
        result=gray;
    }

	return result;
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku XML
 * arg: fileName - nazwa pliku do ktorego zapisujemy
 */
void zeroTile::saveToXML(TXMLDocument *xmlSource, int tileNo)
{
	//sprawdzamy istnieje strumien XML
	if (xmlSource) {
		IXMLNode *nodeRoot, *nodeTile;
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
		nodeTile->SetAttribute("ID",ID);
		nodeTile->SetAttribute("type",type);
		//formatujemy XML (opcja doNodeAutoIndent slabo dziala)
		xmlSource->XML->Text = FormatXMLData(xmlSource->XML->Text);
		xmlSource->Active = true;
	}
}
//---------------------------------------------------------------------------
/* metoda odczytania ustawien z pliku XML
 * arg: fileName - nazwa pliku y ktorego odcyztujemz
 */
void zeroTile::loadFromXML(TXMLDocument *xmlSource, int tileNo)
{
	//sprawdzamy jest plik z ustawieniami
	if (&xmlSource) {
        IXMLNode *nodeRoot, *nodeTile;
		//znajdujemy wezel glowny
		nodeRoot = xmlSource->DocumentElement;
		//sprawdzamy czy wezel ma w ogole dzieci
		if (nodeRoot->HasChildNodes) {
			//odczytujemy odpowiedni wezel podrzedny
			nodeTile = nodeRoot->ChildNodes->Get(tileNo);
			if ((int)nodeTile->GetAttribute("ID")==ID) {
				type = static_cast<tileType>((int)nodeTile->GetAttribute("type"));
				//tutaj uzupelniamy informacje o kafelku na podstawie informacji z pliku
				background_altOff->Fill->Color = gray;
            }
		}
	}
}
//---------------------------------------------------------------------------
/* metoda zapisanie zmian do pliku projektu (*.ifp)
 * arg: ifpData - linia do zapisania
 */
AnsiString zeroTile::saveToIFP(int tileNo)
{
	//DLA BRAKU KAFELKA NIE ZAPISUJEMY NICZEGO
	return "";
}
//---------------------------------------------------------------------------
/* metoda odczytania ustawien kafelka z pliku projektu (*.ifp)
 * arg: ifpData - nazwa pliku do ktorego zapisujemy
 */
void zeroTile::loadFromIFP(String ifpData, int tileNo)
{
	//tutaj uzupelniamy informacje o kafelku na podstawie informacji z pliku
	background_altOff->Fill->Color = gray;
	//wychodzimy
	return;
}
//---------------------------------------------------------------------------




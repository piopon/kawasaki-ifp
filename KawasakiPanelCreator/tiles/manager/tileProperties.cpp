//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "tileProperties.h"
#include "KPCmessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TtilePropertiesWindow *tilePropertiesWindow;
//---------------------------------------------------------------------------
__fastcall TtilePropertiesWindow::TtilePropertiesWindow(TComponent* Owner)
	: TForm(Owner)
{
	nowOpened = false;
    //ustawienia dot. labeli
	editLabel1->MaxLength = 10;
	editLabel2->MaxLength = 10;
	editLabel3->MaxLength = 10;
	editLabel4->MaxLength = 10;
	//ustawienia dot. sygnalow
	editButtonSignal_altTop->FilterChar = "0123456789";
	editButtonSignal_altTop->MaxLength = 4;
	editLampSignal->FilterChar = "0123456789";
	editLampSignal->MaxLength = 4;
	editSwitchSignalL_altUp->FilterChar = "0123456789";
	editSwitchSignalL_altUp->MaxLength = 4;
	editSwitchSignalM->FilterChar = "0123456789";
	editSwitchSignalM->MaxLength = 4;
	editSwitchSignalR_altDown->FilterChar = "0123456789";
	editSwitchSignalR_altDown->MaxLength = 4;
	//ograniczenia wartosci
	editDataLimLower->FilterChar = "0123456789";
	editDataLimUpper->FilterChar = "0123456789";
	//command string
	editCommandString->MaxLength = 75;
	//variable
	editVariableName->MaxLength = 15;
}
//---------------------------------------------------------------------------

void __fastcall TtilePropertiesWindow::FormActivate(TObject *Sender)
{
	//zapamietujemy parametry wybranego kafelka i uaktualniamy widok
	if(callerTile && nowOpened == false){
		//ten event ma sie wykonac tylko przy wlaczeniu okienka
		nowOpened = true;
		//zapamietujemy jaki typ kafelka ustawiono
		backupConstParams();
		//uaktualniamy widok (wywolany event combo OnChange)
		comboTileType->ItemIndex = encodeID(openID);
		//pobieramy informacje o obecnym kafelku
		callerTile->getTileContent();
		//uaktualniamy pozycje okna i zmieniamy ikone glowna
		Top = mainWindow->Top+70;
		Left = mainWindow->Left+10;
		mainWindow->mainIcoInnerGlow->Enabled = true;
		changesMade = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TtilePropertiesWindow::buttonAcceptClick(TObject *Sender)
{
	//zapamietujemy jaki typ kafelka ustawiono
	closeID = decodeID(comboTileType->ItemIndex);
	//sprawdzamy czy zmienil sie rodzaj kafelka
	if (callerTile->getID()!=closeID) {
		//---> mamy nowy rodzaj kafelka
		//poniewaz nie mozna zmienic typu w run-time wiec kasujemy starego kafelka
		if (callerTile) {
			delete callerTile;
			callerTile = NULL;
		}
		//w miejsce przed chwila usunietego kafelka tworzymy nowy o odpowiednim rodzaju
		updateTile();
		//byla zmiana - ustawiamy odpowiednia flage
		if (!mainWindow->lblAppTitle->Text.Pos("*"))
			mainWindow->lblAppTitle->Text = mainWindow->lblAppTitle->Text+"*";
		mainWindow->changesDone = true;
	} else {
		//---> rodzaj kafelka nie ulegl zmianie
		//zapisujemy zmiany w obrebie danego kafelka
		callerTile->setTileContent();
		//sprawdzamy czy byla jakakolwiek zmiana w obrebie kafelka
		if (changesMade) {
			//byla zmiana - ustawiamy odpowiednia flage
			if (!mainWindow->lblAppTitle->Text.Pos("*"))
				mainWindow->lblAppTitle->Text = mainWindow->lblAppTitle->Text+"*";
			mainWindow->changesDone = true;
        }
	}
	//na koniec sprawdzamy kafelek pod katem string display window
	mainWindow->wideTileCreate(callerTile);
	//zamykamy okienko (ustawiamy flage wewnetrzna)
	nowOpened = false;
	this->Close();
	//przywracamy ikone glowna programu
	mainWindow->mainIcoInnerGlow->Enabled = false;
}
//---------------------------------------------------------------------------
//wstawiamy nowy kafelek
//---------------------------------------------------------------------------
void TtilePropertiesWindow::updateTile(void)
{
	zeroTile *tileUpd = mainWindow->createTile(closeID,currTileType,currTileParent);
	if (tileUpd) {
		tileUpd->Position->X=currTilePosX;
		tileUpd->Position->Y=currTilePosY;
		tileUpd->setTileContent();
	}
	//uaktualniamy caller tile
	callerTile = tileUpd;
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::backupConstParams(void)
{
	//zapamietujemy zmienne wewnetrzne okienka
	openID = callerTile->getID();
	//zapamietujemy pozostale zmienne "sztywne"
	currTileType   = callerTile->getType();
	currTileParent = callerTile->Parent;
	currTilePosX   = callerTile->Position->X;
	currTilePosY   = callerTile->Position->Y;
	//zapamietujemy informacje dodatkowe
	currTileNO 	   = callerTile->no;
	colorEmphasize = callerTile->emphasize->StopValue;
	colorGlow = callerTile->glow->GlowColor;
}
//---------------------------------------------------------------------------
int TtilePropertiesWindow::decodeID(int comboIndex)
{
	int result = -1;
	if (comboIndex<=10) {
		result = comboIndex;
	} else if (comboIndex>10 && comboIndex<=14) {
		result = comboIndex+2;
	} else if (comboIndex == 15) {
		result = 21;
	}
	return result;
}
//---------------------------------------------------------------------------
int TtilePropertiesWindow::encodeID(int id)
{
	int result = -1;
	if (id<=10) {
		result = id;
	} else if (id>=13 && id<=16) {
		result = id-2;
	} else if (id == 21) {
		result = 15;
	}
	return result;
}
//---------------------------------------------------------------------------
//procedura do updateowania layoutu okienka
void TtilePropertiesWindow::updateView(int currID)
{
	bool checkLocked = currTileType==tileType::patternLocked;;
	//ustawiamy layout okienka w zaleznosci od rodzaju kafelka
	switch (currID) {
		case 0:
			viewTileZero(checkLocked);
			break;
		case 1:
			viewTilePilotLamp(checkLocked);
			break;
		case 2:
			viewTilePushButton(checkLocked);
			break;
		case 3:
			viewTilePushButtonWLamp(checkLocked);
			break;
		case 4:
			viewTile2Selector(checkLocked);
			break;
		case 5:
			viewTile3Selector(checkLocked);
			break;
		case 6:
			viewTileDigitalSwitch(checkLocked);
			break;
		case 7:
			viewTileDigitalDisplay(checkLocked);
			break;
		case 8:
			viewTileVarDataDisplay(checkLocked);
			break;
		case 9:
			viewTileStrDisplayWindow(checkLocked);
			break;
		case 10:
			viewTileMonitorCommand(checkLocked);
			break;
		case 11:
			viewTilePalletize(checkLocked);
			break;
		case 12:
			viewTileStrDataDisplay(checkLocked);
			break;
		case 13:
			viewTile2SelectorWLamp(checkLocked);
			break;
		case 14:
			viewTile3SelectorWLamp(checkLocked);
			break;
		case 15:
			viewTileServoDispenser(checkLocked);
			break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
//ustawianie widoku (TYLKO WIDOK!) dla kafelka typu zero (PUSTY)
void TtilePropertiesWindow::viewTileZero(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 110;
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = false;
	e_Icon->Visible = false;
	d_Icon->Visible = false;
	dP_Icon->Visible = false;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	/******* LABELE *******/
	//chowamy odpowiednie pola edycyjne
	layoutLabels12->Visible = false;
	layoutLabels34->Visible = false;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {0,0,0,0};
	prepareColorsView(colorBtns,true,true);
	colorButtonBackg_altOff->Color = gray; //dajemy kolor defaultowy
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,0,0,0};
	prepareSignalsView(signals,right,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,true);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{0,0,false},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
//ustawianie widoku (TYLKO WIDOK!) dla kafelka typu zero (PUSTY)
void TtilePropertiesWindow::viewTilePilotLamp(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 470;
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {1,1,2,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,1,0,0};
	prepareSignalsView(signals,left,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,false);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{0,0,false},{155,398,true},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
//ustawianie widoku (TYLKO WIDOK!)  dla kafelka typu PUSH BUTTON
void TtilePropertiesWindow::viewTilePushButton(bool locked)
{
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ustawiamy parametry okna
	Width = 300;
	Height = 420;
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {2,1,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {1,0,0,0};
	prepareSignalsView(signals,left,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,false);
	/******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{155,344,true},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTilePushButtonWLamp(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 436;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {2,1,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {1,1,0,0};
	prepareSignalsView(signals,left,horizontal);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,false);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{85,374,true},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTile2Selector(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 460;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {0,1,0,2};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,0,2,0};
	prepareSignalsView(signals,left,horizontal);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,true);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{5,390,true},{0,0,false},{140,376,true}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTile3Selector(bool locked)
{
	//ustawiamy parametry okna
	Width = 600;
	Height = 310;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {0,1,0,3};
	prepareColorsView(colorBtns,true,true);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,0,3,0};
	prepareSignalsView(signals,right,horizontal);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,true);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{305,236,true},{0,0,false},{440,222,true}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTileDigitalSwitch(bool locked)
{
	//ustawiamy parametry okna
	Width = 600;
	Height = 262;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = false;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {1,1,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {1,0,0,3};
	prepareSignalsView(signals,right,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,true);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{310,202,true},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTileDigitalDisplay(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 350;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = false;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {1,1,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {1,0,0,2};
	prepareSignalsView(signals,left,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,false);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{0,0,false},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTileVarDataDisplay(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 356;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = false;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {1,1,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,0,0,1};
	prepareSignalsView(signals,left,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {1,1};
	prepareVariablesView(variables,false);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{138,233,true},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTileStrDisplayWindow(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 204;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = false;
	layoutLabels34->Visible = false;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {1,0,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,0,0,0};
	prepareSignalsView(signals,right,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,false);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{0,0,false},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(stringWindow,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTileMonitorCommand(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 450;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = true;
	dP_Icon->Visible = true;
	layoutContIcons->Width = d_Icon->Width+dP_Icon->Width+e_Icon->Width+2;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {2,1,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,0,0,0};
	prepareSignalsView(signals,left,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,false);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{80,386,true},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(monitorCmd,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTilePalletize(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 468;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = false;
	dP_Icon->Visible = false;
	layoutContIcons->Width = e_Icon->Width;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {2,1,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {1,1,0,0};
	prepareSignalsView(signals,left,horizontal);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,false);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{0,0,false},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(palletize,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTileStrDataDisplay(bool locked)
{
	//ustawiamy parametry okna
	Width = 300;
	Height = 340;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = false;
	dP_Icon->Visible = false;
	layoutContIcons->Width = e_Icon->Width;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = false;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {1,1,0,0};
	prepareColorsView(colorBtns,true,false);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,0,0,0};
	prepareSignalsView(signals,left,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {1,0};
	prepareVariablesView(variables,false);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{80,276,true},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTile2SelectorWLamp(bool locked)
{
	//ustawiamy parametry okna
	Width = 600;
	Height = 308;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = false;
	dP_Icon->Visible = false;
	layoutContIcons->Width = e_Icon->Width;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {0,1,2,0};
	prepareColorsView(colorBtns,true,true);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,1,2,0};
	prepareSignalsView(signals,right,horizontal);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,true);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{305,238,true},{0,0,false},{440,224,true}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTile3SelectorWLamp(bool locked)
{
	//ustawiamy parametry okna
	Width = 600;
	Height = 308;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = false;
	dP_Icon->Visible = false;
	layoutContIcons->Width = e_Icon->Width;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {0,1,2,0};
	prepareColorsView(colorBtns,false,true);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {0,1,3,0};
	prepareSignalsView(signals,right,horizontal);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,true);
	/******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{305,238,true},{0,0,false},{440,224,true}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(none,false);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::viewTileServoDispenser(bool locked)
{
	//ustawiamy parametry okna
	Width = 442;
	Height = 300;
	//blokujemy edycje w zaleznosci od flagi locked
	lockDecision(locked);
	//ikony kompatybilnosci kontrolerow
	lblCompability->Visible = true;
	e_Icon->Visible = true;
	d_Icon->Visible = false;
	dP_Icon->Visible = false;
	layoutContIcons->Width = e_Icon->Width;
	/******* LABELE *******/
	//pokazujemy odpowiednie pola edycyjne
	layoutLabels12->Visible = true;
	layoutLabels34->Visible = true;
	/******* KOLORY - jakie kolory sa aktywne {backg,label,lamp,switch} */
	int colorBtns[4] = {0,0,0,0};
	prepareColorsView(colorBtns,true,true);
	/******* SYGNALY - jakie sygnaly sa aktywne {backg,lamp,switch,features} */
	int signals[4] = {1,0,0,0};
	prepareSignalsView(signals,right,vertical);
	/******* ZMIENNE - jakie pola sa aktywne {name,type} */
	int variables[2] = {0,0};
	prepareVariablesView(variables,true);
    /******* ROZNE - jakie pola sa aktywne {oper,!,dispType} */
	componentPos misc[3] = {{0,0,false},{0,0,false},{0,0,false}};
	prepareMiscView(misc,true);
	/******* SPECJALNY WIDOK - jeden z widokow 1)paletyzacja 2)str cmd 3)servo */
	prepareSpecialView(servo,true);
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::buttonCancelClick(TObject *Sender)
{
   	nowOpened = false;
	Close();
	//przywracamy ikone glowna programu
	mainWindow->mainIcoInnerGlow->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::comboTileTypeChange(TObject *Sender)
{
	updateView(comboTileType->ItemIndex);
}
//---------------------------------------------------------------------------
void TtilePropertiesWindow::lockDecision(bool lock)
{
	for (int i = 0; i < ChildrenCount; i++) {
		TControl* currComponent = dynamic_cast<TControl*>(Children[i]);
		currComponent->Enabled = !lock;
	}
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::rectColorClick(TObject *Sender)
{
	if(Sender->ClassNameIs("TRectangle")){
		TRectangle *clickedRect = dynamic_cast<TRectangle*>(Sender);
		callerColorButton->Color = clickedRect->Fill->Color;
	}
	//chowamy odsloniety panel
    panelColorSelect->Visible = false;
	//odblokowujemy wszystkie kontrolki (oprocz kolorkow)
	lockDecision(false);
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::colorButtonClick(TObject *Sender)
{
	if(Sender->ClassNameIs("TColorButton")){
		//zapamietujemy ktory guzik wywolal pojawienie sie panelu z kolorami
		callerColorButton = dynamic_cast<TColorButton*>(Sender);
		//pokaz panel z kolorami
		panelColorSelect->Position->X =(tilePropertiesWindow->Width  - panelColorSelect->Width) /2;
		panelColorSelect->Position->Y =(tilePropertiesWindow->Height - panelColorSelect->Height)/2;
		//blokujemy wszystkie kontrolki (oprocz panelu z kolorkami)
		panelColorSelect->BringToFront();
		lockDecision(true);
		panelColorSelect->Enabled = true;
		panelColorSelect->Visible = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::comboServoPartChange(TObject *Sender)
{
	imageServoPart->Bitmap = mainWindow->loadServoImage(comboServoPart->ItemIndex);
	//ustawiamy flage ze cos zmieniono
	changesMade = true;
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::checkBoxDataLimitChange(TObject *Sender)
{
	editDataLimLower->Enabled = checkBoxDataLimit->IsChecked;
	editDataLimUpper->Enabled = checkBoxDataLimit->IsChecked;
	//ustawiamy flage ze cos zmieniono
	changesMade = true;
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::dispTypeUpDownChange(TObject *Sender)
{
	if (dispTypeUpDown->IsChecked) {
		titleSwitchL->Text = "U:";
		titleSwitchR->Text = "D:";
		titleSwitchSignalL_altUp->Text = "SWITCH U";
		titleSwitchSignalR_altDown->Text = "SWITCH D";
	} else {
		titleSwitchL->Text = "L:";
		titleSwitchR->Text = "R:";
		titleSwitchSignalL_altUp->Text = "SWITCH L";
		titleSwitchSignalR_altDown->Text = "SWITCH R";
	}
	//ustawiamy flage ze cos zmieniono
	changesMade = true;
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::contIconMouseEnter(TObject *Sender)
{
	//sprawdzamy czy najechalismy na komponent typu TImage
	TImage *currImg = dynamic_cast<TImage*>(Sender);
	if (currImg) {
		//ustawiamy odpowiednio nasz panel z podpowiedzia
		hintPanel->Position->X = layoutContIcons->Position->X + currImg->Position->X - hintPanel->CalloutOffset;
		hintPanel->Position->Y = currImg->Position->Y + currImg->Height;
		//ustawiamy tekst w chmurce
		String contType = currImg->Name.SubString(0,currImg->Name.Pos("_")-1);
		if (contType.Pos("P")>0) contType = "D+";
		lblHint->Text = contType.UpperCase()+" CONTROLLER COMPATIBLE";
		//pokazujemy podpowiedz
		hintPanel->Visible = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::contIconMouseLeave(TObject *Sender)
{
	//sprawdzamy czy najechalismy na komponent typu TImage
	TImage *currImg = dynamic_cast<TImage*>(Sender);
	if (currImg) {
		//myszka wyszla z obrazka = chowamy podpowiedz
    	hintPanel->Visible = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TtilePropertiesWindow::dataChange(TObject *Sender)
{
	//jezeli cokolwiek sie zmienilo to ustawiamy odpowiednia flage
	changesMade = true;
}
//---------------------------------------------------------------------------



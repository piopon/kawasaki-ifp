//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "KPC.h"
#include "KPCmessage.h"
#include "KPCoptions.h"
#include "kawasakiComm.h"
//---------------------------------------------------------------------------
#include <fstream.h>
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TkpcOpt *kpcOpt;
//---------------------------------------------------------------------------

/***************************************************************************
 ******  OBSLUGA OKIENKA    ************************************************
 ***************************************************************************/

//---------------------------------------------------------------------------
__fastcall TkpcOpt::TkpcOpt(TComponent* Owner)
	: TForm(Owner)
{
	opened = false;
	//uaktualniamy zakladki
	updateTabs();
	//skladowe adresu IP
	editIPaddres1->MaxLength = 3;
	editIPaddres1->FilterChar = "0123456789";
	editIPaddres2->MaxLength = 3;
	editIPaddres2->FilterChar = "0123456789";
	editIPaddres3->MaxLength = 3;
	editIPaddres3->FilterChar = "0123456789";
	editIPaddres4->MaxLength = 3;
	editIPaddres4->FilterChar = "0123456789";
	//numer portu
	editPortNo->MaxLength = 4;
	editPortNo->FilterChar = "0123456789";
	//interwal autozapisu
	editAutosaveInterval->MaxLength = 2;
	editAutosaveInterval->FilterChar = "0123456789";
	//wczytujemy elementy listy z pliku
	loadRobotFromRecent(mainWindow->recentRobotsFileName);
	listRecentRobots->ItemIndex = -1;
	//uzupelniamy informacje o wersji aplikacji
	String fileVer = GetCurrentDir()+"\\KawasakiPanelCreator.exe";
	lblVersion->Text = "ver. "+mainWindow->getVersionNumber(fileVer);
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::FormActivate(TObject *Sender)
{
	//sprawdzamy czy to nowo otwarte okno
	if (!opened) {
		getSettings();
		//uaktualniamy liczbe otwartych stron
		pageNoSpin->Value = mainWindow->tabPanelPages->TabCount;
		pageNoSpin->Repaint();
		//uaktualniamy widok okna (zalezne od obecnego stanu aplikacji)
		updateView();
		//uaktualniamy pozycje okna i jego komponentow
		updatePosition();
		//ustawiamy ze okno jest nowo-otwarte
		opened = true;
	}
}
//---------------------------------------------------------------------------
void TkpcOpt::updatePosition(void)
{
	int childPosYCorr = 7;
	//ustawiamy pozycje okna
	Left = mainWindow->Left + 1;
	Top = mainWindow->Top + mainWindow->mainMenuTop->Height + 1;
	//ustawiamy wymiary okna
	Width = mainWindow->Width - 2;
	Height = mainWindow->mainBackground->Height - 1;
	//uaktualniamy pozycje zakladek w zaleznosci od wielkosci okna
	if (Height>550) {
		//zakladka GENERAL
		groupMain0_GUI->Position->Y = 10;
		groupMain0_GUI->Height = 180;
		groupMain1_Tiles->Position->Y = 200;
		groupMain1_Tiles->Height = 180;
		groupMain2_App->Position->Y = 400;
		groupMain2_App->Height = 180;
		//zakladka ROBOT
		groupMain0_Conn->Position->Y = 10;
		groupMain0_Conn->Height = 200;
		groupSettings->Position->Y = 45;
		groupAction->Position->Y = 45;
		groupMain1_Recent->Position->Y = 225;
		groupMain1_Recent->Height = 360;
		groupRecentList->Position->Y = 40;
		groupRecentAction->Position->Y = 220;
		groupMain2_Info->Position->Y = 225;
		groupMain2_Info->Height = 360;
		groupRobotAS->Position->Y = 40;
		groupServo->Position->Y = 220;
		//zakladka ABOUT
		if (groupMain1_Debug->Visible == false) {
			groupMain0_About->Position->Y = 180;
		} else {
			groupMain0_About->Position->Y = 60;
			groupMain1_Debug->Position->Y = 340;
		}
	} else {
		//zakladka GENERAL
		groupMain0_GUI->Position->Y = 8;
		groupMain0_GUI->Height = 140;
		groupMain1_Tiles->Position->Y = 152;
		groupMain1_Tiles->Height = 150;
		groupMain2_App->Position->Y = 307;
		groupMain2_App->Height = 150;
		//zakladka ROBOT
		groupMain0_Conn->Position->Y = 8;
		groupMain0_Conn->Height = 146;
		groupSettings->Position->Y = 21;
		groupAction->Position->Y = 21;
		groupMain1_Recent->Position->Y = 159;
		groupMain1_Recent->Height = 300;
		groupRecentList->Position->Y = 24;
		groupRecentAction->Position->Y = 193;
		groupMain2_Info->Position->Y = 159;
		groupMain2_Info->Height = 300;
		groupRobotAS->Position->Y = 24;
		groupServo->Position->Y = 193;
		//zakladka ABOUT
		if (groupMain1_Debug->Visible == false) {
			groupMain0_About->Position->Y = 100;
		} else {
			groupMain0_About->Position->Y = 8;
			groupMain1_Debug->Position->Y = 288;
		}
	}
	//przegladamy wszystkie groupBoxy w zakladce GENERAL zeby je odpowiednio wyrownac w pionie
	for (int i = 0; i < optGeneral->ChildrenCount; i++) {
		if (optGeneral->Children[i]->ClassNameIs("TContent")) {
			//mamy zawartosc TTabItem - w niej przeszukujemy groupBoxow
			TFmxObject *tabContent = optGeneral->Children[i];
			for (int j = 0; j < tabContent->ChildrenCount; j++) {
				TControl *tabChild = dynamic_cast<TControl*>(tabContent->Children[j]);
				//sprawdzamy czy w danej zakladce mamy groupBoxa
				if (tabChild && tabChild->ClassNameIs("TGroupBox")) {
					//jest groupBox - wyrownujemy w pionie wszystkie jego dzieci
					for (int k = 0; k < tabChild->ChildrenCount; k++) {
						TControl *currChild = dynamic_cast<TControl*>(tabChild->Children[k]);
						if (currChild && currChild->ClassNameIs("TGroupBox")) {
							currChild->Position->Y = childPosYCorr + (tabChild->Height - currChild->Height)/2;
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TkpcOpt::updateTabs(void)
{
	//ustawiamy aktywna pierwsza zakladke
	tabsOptions->ActiveTab = tabsOptions->Tabs[0];
	//ustawiamy "wcisniecie" pierwszego guzika
	btnOptGeneral->IsPressed = true;
	btnOptGeneral->BringToFront();
	btnRobot->IsPressed = false;
	btnRobot->SendToBack();
	btnAbout->IsPressed = false;
	btnAbout->SendToBack();
}
//---------------------------------------------------------------------------
void TkpcOpt::updateView(void)
{
	//zakladka GENERAL
	if (pageNoSpin->Value>4) {
		//kompatybilny jest tylko kontroler typu E
		e_contIcon->Visible = true;
		e_contIcon->Position->X = 155;
		e_contIcon->Position->Y = 50;
		d_contIcon->Visible = false;
		dP_contIcon->Visible = false;
	} else {
		//kompatybilne sa wszystkie kontrolery
		e_contIcon->Visible = true;
		e_contIcon->Position->X = 155;
		e_contIcon->Position->Y = 23;
		d_contIcon->Visible = true;
		d_contIcon->Position->X = 155;
		d_contIcon->Position->Y = 50;
		dP_contIcon->Visible = true;
		dP_contIcon->Position->X = 155;
		dP_contIcon->Position->Y = 77;
    }
	//zakladka ROBOT
	if (mainWindow->robot->isDLLinit()) {
		btnDLL_init->Enabled = false;
		btnDLL_getVer->Enabled = true;
		btnDLL_destroy->Enabled = true;
		if (mainWindow->robot->isConnected()) {
			btnConnect->Enabled = false;
			btnDisconnect->Enabled = true;
			btnUpdRobotInfo->Enabled = true;
		} else {
			btnConnect->Enabled = true;
			btnDisconnect->Enabled = false;
			btnUpdRobotInfo->Enabled = false;
        }
	} else {
		btnDLL_init->Enabled = true;
		btnDLL_getVer->Enabled = false;
		btnDLL_destroy->Enabled = false;
		btnConnect->Enabled = false;
		btnDisconnect->Enabled = false;
		btnUpdRobotInfo->Enabled = false;
	}
	//zakladka ABOUT - nic do robienia
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnOptClick(TObject *Sender)
{
	//sprawdzamy czy sender to button
	if (Sender->ClassNameIs("TButton")) {
		TButton *senderButton = dynamic_cast<TButton*>(Sender);
		//szukamy wszystkich buttonow opcji w tym formie
		for (int i = 0; i < kpcOpt->ChildrenCount; i++) {
			if (kpcOpt->Children[i]->ClassNameIs("TButton")) {
				TButton *currButton = dynamic_cast<TButton*>(kpcOpt->Children[i]);
				if (currButton == senderButton && currButton->Tag > 0) {
					currButton->IsPressed = true;
					currButton->BringToFront();
					tabsOptions->ActiveTab = tabsOptions->Tabs[currButton->Tag-1];
				} else {
					if (currButton->Tag > 0) {
						//jest to guzik opcji (Tag mowi ktora zakladke otworzyc)
						currButton->IsPressed = false;
						currButton->SendToBack();
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnOKClick(TObject *Sender)
{
	//pierw sprawdzamy poprawnosc wprowadzonych danych
	if (checkSettings()) {
		this->Close();
		opened = false;
		//uaktualniamy zakladki (robimy to tutaj bo w Activate widac moment przelaczania!)
		updateTabs();
		applySettings();
	}
	//rozlaczamy sie z robotem o ile bylismy podlaczeni
	if (mainWindow->robot->isConnected()) {
        mainWindow->robot->disconnect();
	}
}
//---------------------------------------------------------------------------
//odczyt ustawien z okna glownego mainWindow do okna opcji
void TkpcOpt::getSettings(void)
{
    //zakladka GUI
	checkShowSplash->IsChecked = mainWindow->showSplash;
	checkShowExpanded->IsChecked = mainWindow->patternsAtFront;
	checkOptLocally->IsChecked = mainWindow->rightOptions;

	radioAnimOff->IsChecked = false;
	radioAnimSlow->IsChecked = false;
	radioAnimQuick->IsChecked = false;

	if (mainWindow->animationTime < 0.002) {
		radioAnimOff->IsChecked = true;
	} else if (mainWindow->animationTime == 1) {
		radioAnimSlow->IsChecked = true;
	} else if (mainWindow->animationTime < 0.3) {
		radioAnimQuick->IsChecked = true;
	}

	radioStartupEmpty->IsChecked = false;
	radioStartupLast->IsChecked = false;
	radioStartupNothing->IsChecked = false;
	if (mainWindow->openAction == action::newProject) {
		radioStartupEmpty->IsChecked = true;
	} else if (mainWindow->openAction == action::lastProject) {
		radioStartupLast->IsChecked = true;
	} else if (mainWindow->openAction == action::nothing) {
		radioStartupNothing->IsChecked = true;
	}
    //zakladka TILES
	checkShowID->IsChecked = mainWindow->tilesID;
	checkShowNumbers->IsChecked = mainWindow->tilesNO;
	checkTileOptOnLeftClick->IsChecked = mainWindow->leftOptions;
	checkEmphasizeActive->IsChecked = mainWindow->tileEmphasize;
	colorGlowRegular->Color = mainWindow->glowRegular;
	colorGlowPattern->Color = mainWindow->glowPattern;
	colorGlowProtected->Color = mainWindow->glowProtected;
	colorEmphasizeOptTile->Color = mainWindow->colorEmphasize;

	//zakladka APPLICATION
	comboAppPriority->ItemIndex = 10-mainWindow->appPriority;
	checkAppAllowNewPattern->IsChecked = mainWindow->patternsAdd;

	if (mainWindow->patternSave == action::nothing) {
		comboPatternTilesAction->ItemIndex=0;
	} else if (mainWindow->patternSave == action::atExit) {
		comboPatternTilesAction->ItemIndex=1;
	} else if (mainWindow->patternSave == action::atSaveProject) {
		comboPatternTilesAction->ItemIndex=2;
	}

	checkMsgOnExit->IsChecked = mainWindow->msgOnExit;
	checkMsgProjNotSaved->IsChecked = mainWindow->msgOnProject;
	checkMsgTileDelete->IsChecked = mainWindow->msgOnTileDel;
	checkMsgOnNoTitle->IsChecked = mainWindow->msgOnNoTitles;
	checkAutosaveActive->IsChecked = mainWindow->autosaveActive;

	if (mainWindow->autosaveMethod == action::newFile) {
		radioAutosaveNewF->IsChecked = true;
		radioAutosaveCurrFile->IsChecked = false;
	} else if (mainWindow->autosaveMethod == action::overwrite) {
		radioAutosaveCurrFile->IsChecked = true;
		radioAutosaveNewF->IsChecked = false;
	}
	editAutosaveInterval->Text = FloatToStr(mainWindow->autosaveTime);
    //zakladka ROBOT
	checkDLL_initAtStart->IsChecked = mainWindow->initDLL;
	checkAddToRecent->IsChecked = mainWindow->addToRecent;
	groupMain1_Debug->Visible = mainWindow->debugActive;
}
//---------------------------------------------------------------------------
//odczyt ustawien z okna opcji do okna mainWindow
void TkpcOpt::applySettings(void)
{
    bool colorChanged = false;
	//zakladka GUI
	mainWindow->showSplash = checkShowSplash->IsChecked;
	mainWindow->patternsAtFront = checkShowExpanded->IsChecked;
	mainWindow->rightOptions = checkOptLocally->IsChecked;

	if (radioAnimOff->IsChecked) {
		mainWindow->animationTime = 0.001;
	} else if (radioAnimSlow->IsChecked) {
		mainWindow->animationTime = 1;
	} else if (radioAnimQuick->IsChecked) {
		mainWindow->animationTime = 0.2;
	}

	if (radioStartupEmpty->IsChecked) {
		mainWindow->openAction = action::newProject;
	} else if (radioStartupLast->IsChecked) {
		mainWindow->openAction = action::lastProject;
	} else if (radioStartupNothing->IsChecked) {
		mainWindow->openAction = action::nothing;
	}
    //zakladka TILES
	mainWindow->tilesID = checkShowID->IsChecked;
	mainWindow->tilesNO = checkShowNumbers->IsChecked;
 	mainWindow->updateTileInfo(mainWindow->tilesID,mainWindow->tilesNO);
	mainWindow->leftOptions = checkTileOptOnLeftClick->IsChecked;
	mainWindow->tileEmphasize = checkEmphasizeActive->IsChecked;
	//sprawdzamy czy byla zmiana koloru
	if (mainWindow->glowRegular != colorGlowRegular->Color) {
		mainWindow->glowRegular = colorGlowRegular->Color;
		colorChanged = true;
	}
	if (mainWindow->glowPattern != colorGlowPattern->Color) {
		mainWindow->glowPattern = colorGlowPattern->Color;
		colorChanged = true;
	}
	if (mainWindow->glowProtected != colorGlowProtected->Color) {
		mainWindow->glowProtected = colorGlowProtected->Color;
		colorChanged = true;
	}
	if (mainWindow->colorEmphasize != colorEmphasizeOptTile->Color) {
		mainWindow->colorEmphasize = colorEmphasizeOptTile->Color;
		colorChanged = true;
	}
	//sprawdzamy czy byla zmiana jakiegokolwiek koloru
	if (colorChanged) mainWindow->updateTileColors();
	//zakladka APPLICATION
	if (comboAppPriority->ItemIndex==0) {
		mainWindow->appPriority = action::lowPrior;
	} else if (comboAppPriority->ItemIndex==1) {
		mainWindow->appPriority = action::normalPrior;
	} else if (comboAppPriority->ItemIndex==2) {
		mainWindow->appPriority = action::highPrior;
	} else if (comboAppPriority->ItemIndex==3) {
		mainWindow->appPriority = action::realTimePrior;
	}
	//ustawienie priorytetu aplikacji
	mainWindow->setAppPriority();
	mainWindow->patternsAdd = checkAppAllowNewPattern->IsChecked;
	if (comboPatternTilesAction->ItemIndex==0) {
		mainWindow->patternSave = action::nothing;
	} else if (comboPatternTilesAction->ItemIndex==1) {
		mainWindow->patternSave = action::atExit;
	} else if (comboPatternTilesAction->ItemIndex==2) {
		mainWindow->patternSave = action::atSaveProject;
	}
	mainWindow->msgOnExit = checkMsgOnExit->IsChecked;
	mainWindow->msgOnProject = checkMsgProjNotSaved->IsChecked;
	mainWindow->msgOnTileDel = checkMsgTileDelete->IsChecked;
	mainWindow->msgOnNoTitles = checkMsgOnNoTitle->IsChecked;
	//uaktualniamy opcje autozapisu (resetujemy odliczanie jezeli byla zmiana)
	mainWindow->autosaveActive  = checkAutosaveActive->IsChecked;
	if (radioAutosaveNewF->IsChecked) {
		mainWindow->autosaveMethod = action::newFile;
	} else if (radioAutosaveCurrFile->IsChecked) {
		mainWindow->autosaveMethod = action::overwrite;
	}
	//sprawdzamy czy byla zmiana w czasie autozapisu
	if (mainWindow->autosaveTime != editAutosaveInterval->Text.ToInt()) {
		mainWindow->autosaveTime = editAutosaveInterval->Text.ToInt();
		mainWindow->autosaveTimer->Enabled = false;
		mainWindow->autosaveTimer->Interval = mainWindow->autosaveTime*60*1000;
	}
	mainWindow->autosaveTimer->Enabled = mainWindow->autosaveActive;
	//zakladka ROBOT
	mainWindow->initDLL = checkDLL_initAtStart->IsChecked;
	mainWindow->addToRecent = checkAddToRecent->IsChecked;
}
//---------------------------------------------------------------------------
//sprawdzenei poprawnosci wprowadzonych opcji (true jak wszystko jest ok)
bool TkpcOpt::checkSettings(void)
{
	bool result = true;
	//interwal czasu autozapisu > 0
	if (editAutosaveInterval->Text.ToInt()<=0) {
		kpcMsg->show("Autosave interval must be positive!",MB_OK,MB_ICONERROR);
		result = false;
	}

	return result;
}
//---------------------------------------------------------------------------

/***************************************************************************
 ******  ZAKLADKA GENERAL    ***********************************************
 ***************************************************************************/

//---------------------------------------------------------------------------
void __fastcall TkpcOpt::pageNoSpinChange(TObject *Sender)
{
	mainWindow->setNoOfTabsVisible(pageNoSpin->Value);
	//ustawiamy ikony kompatybilnych kontrolerow
	if (pageNoSpin->Value>4) {
		//kompatybilny jest tylko kontroler typu E
		e_contIcon->Visible = true;
		e_contIcon->Position->X = 155;
		e_contIcon->Position->Y = 50;
		d_contIcon->Visible = false;
		dP_contIcon->Visible = false;
	} else {
		//kompatybilne sa wszystkie kontrolery
		e_contIcon->Visible = true;
		e_contIcon->Position->X = 155;
		e_contIcon->Position->Y = 23;
		d_contIcon->Visible = true;
		d_contIcon->Position->X = 155;
		d_contIcon->Position->Y = 50;
		dP_contIcon->Visible = true;
		dP_contIcon->Position->X = 155;
		dP_contIcon->Position->Y = 77;
    }
}
//---------------------------------------------------------------------------

/***************************************************************************
 ******  ZAKLADKA ROBOT    *************************************************
 ***************************************************************************/

//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnDLL_initClick(TObject *Sender)
{
	kawasakiStatus commStatus = mainWindow->robot->initComm();
	if (commStatus != kawasakiStatus::ok) {
		if (commStatus == kawasakiStatus::dllAlreadyInit) {
			kpcMsg->show("DLL alrady initialized!",MB_OK,MB_ICONWARNING);
		} else {
			kpcMsg->show("DLL not found!",MB_OK,MB_ICONERROR);
		}
	} else {
		btnDLL_init->Enabled = false;
		btnDLL_getVer->Enabled = true;
		btnDLL_destroy->Enabled = true;
		btnConnect->Enabled = true;
		btnDisconnect->Enabled = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnDLL_destroyClick(TObject *Sender)
{
	//przed usunieciem sprawdzamy czy jestesmy polaczeni
	if (mainWindow->robot->isConnected()) mainWindow->robot->disconnect();
	//rozlaczylismy sie teraz wylaczamy modul komunikacyjny
	kawasakiStatus commStatus = mainWindow->robot->closeComm();
	if (commStatus != kawasakiStatus::ok) {
		kpcMsg->show("DLL not initialized!",MB_OK,MB_ICONWARNING);
	} else {
		//GUI polaczenia
		btnConnect->Enabled = false;
		btnDisconnect->Enabled = false;
		rectStatus->Fill->Color = claSilver;
		lblStatus->Text = "CONN STATUS...";
		//GUI dll
		btnDLL_destroy->Enabled = false;
		btnDLL_getVer->Enabled = false;
		btnDLL_init->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnDLL_getVerClick(TObject *Sender)
{
	String currentVersion = "";
	mainWindow->robot->getVersion(currentVersion);
	if (currentVersion != "") {
		int separatorPos = currentVersion.Pos(":");
		String dll = currentVersion.SubString(0,separatorPos-3);
		String comm = currentVersion.SubString(separatorPos+3,currentVersion.Length()-separatorPos);
        String msg = "DLL:\n"+dll+"\nCOMM:\n"+comm;
		kpcMsg->show(msg,MB_OK,MB_ICONINFORMATION);
	}
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnConnectClick(TObject *Sender)
{
	if (editIPaddres1->Text!="" && editIPaddres2->Text!="" && editIPaddres3->Text!="" && editIPaddres4->Text!="") {
		//skladamy adres IP i uaktualniamy parametry polaczenia
		String ipStr = editIPaddres1->Text+"."+editIPaddres2->Text+"."+editIPaddres3->Text+"."+editIPaddres4->Text;
		mainWindow->robot->updateConnParams(ipStr,StrToInt(editPortNo->Text));
		//probujemy sie polaczyc
		kawasakiStatus commStatus = mainWindow->robot->connect();
		if (commStatus == kawasakiStatus::ok || commStatus == kawasakiStatus::alreadyConn) {
			rectStatus->Fill->Color = claLime;
			lblStatus->Text = "CONN OK!";
			btnConnect->Enabled = false;
			btnDisconnect->Enabled = true;
			btnUpdRobotInfo->Enabled = true;
			//jezeli zaznaczony jest check to dodajemy robota do listy
			if (checkAddToRecent->IsChecked) {
				addCurrRobotToRecent(ipStr,editRobotName->Text,false);
			}
		} else {
			rectStatus->Fill->Color = claRed;
			lblStatus->Text = "CONN NOK!";
			kpcMsg->show("Cant connect...\nDLL ERROR NO ["+IntToStr(commStatus)+"]:\n"+kawasakiStatusToStr(commStatus),MB_OK,MB_ICONERROR);
			btnConnect->Enabled = true;
			btnDisconnect->Enabled = false;
			btnUpdRobotInfo->Enabled = false;
		}
	} else {
		kpcMsg->show("IP address is incomplete!",MB_OK,MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnDisconnectClick(TObject *Sender)
{
	//probujemy sie polaczyc
	kawasakiStatus commStatus = mainWindow->robot->disconnect();
	if (commStatus == kawasakiStatus::ok || commStatus == kawasakiStatus::alreadyDisconn) {
		rectStatus->Fill->Color = claSilver;
		lblStatus->Text = "CONN END!";
		btnDisconnect->Enabled = false;
		btnUpdRobotInfo->Enabled = false;
		btnConnect->Enabled = true;
	} else {
		rectStatus->Fill->Color = claRed;
		lblStatus->Text = "DISCONN OK!";
		kpcMsg->show("Cant disconnect...\nDLL ERROR NO ["+IntToStr(commStatus)+"]:\n"+kawasakiStatusToStr(commStatus),MB_OK,MB_ICONERROR);
		btnDisconnect->Enabled = false;
		btnConnect->Enabled = true;
		btnUpdRobotInfo->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnClearConnSettingsClick(TObject *Sender)
{
	//czyscimy pola edycyjne
	editIPaddres1->Text = "";
	editIPaddres2->Text = "";
	editIPaddres3->Text = "";
	editIPaddres4->Text = "";
	editPortNo->Text = "";
	editRobotName->Text = "";
	//wylaczamy aktywnego robota
	listRecentRobots->ItemIndex = -1;
	//wylaczamy informacje dodatkowe
	groupRobotAS->Text = "AS GROUP: [unknown]  ";
	lblUserIfAS->Text = "[unknown]";
	lblUserIfTP->Text = "[unknown]";
	lblArmAs->Text = "[unknown]";
	lblArmData->Text = "[unknown]";
	lblASMsg->Text = "[unknown]";
	lblContType->Text = "[unknown]";
	groupServo->Text = "SERVO GROUP: [unknown]  ";
	lblControlServo->Text = "[unknown]";
	lblDataFile->Text = "[unknown]";
	lblRobotArm->Text = "[unknown]";
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnAddRobotClick(TObject *Sender)
{
	//sprawdzamy czy wszystkie pola sa uzupelnione
	if (editIPaddres1->Text!="" && editIPaddres2->Text!="" && editIPaddres3->Text!="" && editIPaddres4->Text!="") {
		if (editRobotName->Text!="") {
			if	(editPortNo->Text!="") {
				String ipStr = editIPaddres1->Text+"."+editIPaddres2->Text+"."+editIPaddres3->Text+"."+editIPaddres4->Text;
				addCurrRobotToRecent(ipStr,editRobotName->Text);
			} else {
                kpcMsg->show("Port number is empty!",MB_OK,MB_ICONERROR);
            }
		} else {
			kpcMsg->show("Robot name is empty!",MB_OK,MB_ICONERROR);
        }
	} else {
		kpcMsg->show("IP address is incomplete!",MB_OK,MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void TkpcOpt::addCurrRobotToRecent(String currIP, String currName, bool showFeedback)
{
	int robotNode = 0;
	//sprawdzamy czy dany robot jest juz na liscie
	if (!checkIfRecentRobot(currIP,currName,robotNode)) {
		//robota nie ma na liscie
		int newElementNo = listRecentRobots->Items->Count;
		//robota nie ma na liscie - dodajemy go z uzyciem nazwy na koncu listy
		listRecentRobots->Items->Add(currName);
		//sprawdzamy czy jestesmy polaczeni z robotem
		if (mainWindow->robot->isConnected()) {
			//robot jest polaczony bedziemy sciagac z niego plik = budujemy jego nazwe
			String fileName = currName+"_IP"+currIP;  int dotPos = 0;
			//usuwamy kropki (z adresu IP) z nazwy pliku
			do {
				dotPos = fileName.Pos(".");
				fileName = fileName.Delete(dotPos,1);
			} while (dotPos > 0);
			//probujemy pobrac informacje z kontrolera i zapisac je do zdefiniowanego pliku
            btnUpdRobotInfoClick(NULL);
		}
		//zapisujemy dane o robocie z DANYCH NA FORMIE do pliku (na ostatnie miejsce)
		saveRobotToRecent(mainWindow->recentRobotsFileName,newElementNo);
		//zaznaczamy nowo dodanego robota
		listRecentRobots->ItemIndex = newElementNo;
	} else {
		if (showFeedback) {
			//wyswietlamy komunikat ze robot o podanym adresie istnieje
			kpcMsg->show("Specified robot (name or IP address) is already on list!",MB_OK,MB_ICONINFORMATION);
		}
		//pokazujemy ktory robot (zaznaczamy na liscie)
		listRecentRobots->ItemIndex = robotNode;
		//robot jest juz na liscie - odczytujemy informacje z pliku
		loadRobotFromRecent(mainWindow->recentRobotsFileName,robotNode);
	}
}
//---------------------------------------------------------------------------
bool TkpcOpt::checkIfRecentRobot(String ip, String name, int &whichNode)
{
	bool result = false;
	//lista zawierajaca historie robotow zawiera ich nazwy dlatego sprawdzamy plik XML
	if (FileExists(mainWindow->recentRobotsFileName,false)) {
		//jest plik - otwieramy go i sprawdzamy po kolei dzieci
		TXMLDocument *xml = new TXMLDocument(this);
		IXMLNode *nodeRoot, *nodeTile;
		//plik istnieje odczytujemy
		xml->LoadFromFile(mainWindow->recentRobotsFileName);
		//znajdujemy wezel glowny
		nodeRoot = xml->DocumentElement;
		//odczytujemy po kolei jego dzieci
		for (int i=0; i < nodeRoot->ChildNodes->Count; i++) {
			whichNode = i;
			nodeTile = nodeRoot->ChildNodes->Get(i);
			String currName = nodeTile->GetAttribute("name");
			String currIP = nodeTile->GetAttribute("IP");
			if (currIP == ip || currName == name) {
				result = true;
				break;
			}
		}
		//kasujemy plik xml
		delete xml;
	} else {
		//nie ma pliku tzn. nie ma zadnego robota na liscie
		result = false;
		whichNode = 0;
	}
	return result;
}
//---------------------------------------------------------------------------
void TkpcOpt::fillRobotInfo(String fromFile)
{
	//sprawdzamy czy dany plik istnieje
	if (FileExists(fromFile,false)) {
		//czy plik jest w katalogu robots
		if (!fromFile.Pos("\\robots\\")) {
			//w ramach porzadku przenosimy go
			String oldFileName = fromFile;
			fromFile = "robots//"+fromFile;
			MoveFileW(oldFileName.c_str(),fromFile.c_str());
		}
		//jak juz go mamy w folderze to dopiero odczytujemy zawartosc
		ifstream plikIFP; 	string msgOriginal;
		//odczytujemy plik
		plikIFP.open(fromFile.c_str(), ios::binary);
		/***** sprawdzamy czy udalo sie odczytac plik ************************/
		if(!plikIFP && !plikIFP.is_open()) {
			kpcMsg->show("Cant open file: "+fromFile,MB_OK,MB_ICONERROR);
			return;
		}

		int dataStart = 0; String dataPick = "";
		/*********************************************************************
		 *** odczytujemy linijki pliku az natrafimy na definicje grupy AS ****
		 *********************************************************************/
		do {
			getline(plikIFP,msgOriginal);
		} while (msgOriginal != "" && msgOriginal.compare(0,18,".*=== AS GROUP ===") != 0);
		//odczytujemy po kolei informacje do panelu INFORMATION
		//0. groupBox AS GROUP
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
        groupRobotAS->Text = "AS GROUP: "+dataPick.SubString(0,dataPick.Pos(" "));
		//1. label USER IF AS
		getline(plikIFP,msgOriginal);
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
		lblUserIfAS->Text = dataPick;
		//2. label USER IF TP
		getline(plikIFP,msgOriginal);
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
		lblUserIfTP->Text = dataPick;
		//3. label ARM CTRL AS
		getline(plikIFP,msgOriginal);
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
		lblArmAs->Text = dataPick;
		//4. label MESSAGE FILE
		getline(plikIFP,msgOriginal);
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
		lblASMsg->Text = dataPick;
		/***** omijamy kilka linijek tekstu aby odczytac kolejne info */
		do {
			getline(plikIFP,msgOriginal);
		} while (msgOriginal != "" && msgOriginal.compare(0,15,".*ARM DATA FILE") != 0);
		//5. label ARM DATA
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
		lblArmData->Text = dataPick;

		/*********************************************************************
		 *** odczytujemy linijki pliku az natrafimy na definicje servo AS ****
		 *********************************************************************/
 		do {
			getline(plikIFP,msgOriginal);
		} while (msgOriginal != "" && msgOriginal.compare(0,21,".*=== SERVO GROUP ===") != 0);
		//odczytujemy po kolei informacje do panelu INFORMATION
		//0. groupBox SERVO GROUP
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
        groupServo->Text = "SERVO GROUP: "+dataPick.SubString(0,dataPick.Pos(" "));
		//1. label CONTROL
		getline(plikIFP,msgOriginal);
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
		lblControlServo->Text = dataPick;
		//2. label DATA FILE
		getline(plikIFP,msgOriginal);
		dataStart = msgOriginal.find_first_of(':');
		dataPick = msgOriginal.substr(dataStart+2,msgOriginal.length()-dataStart-1).c_str();
		lblDataFile->Text = dataPick;
		/***** omijamy kilka linijek tekstu aby odczytac kolejne info */
		do {
			getline(plikIFP,msgOriginal);
		} while (msgOriginal != "" && msgOriginal.compare(0,11,"ZROBOT.TYPE") != 0);
		//3. label ROBOT ARM
		dataStart = msgOriginal.find_first_of(" ");
		int robotStart = msgOriginal.find_first_of("QWERTYUIOPASDFGHJKLZXCVBNM",dataStart+1);
		int robotStop = msgOriginal.find_first_of(" ",robotStart+1);
		dataPick = msgOriginal.substr(robotStart,robotStop-robotStart).c_str();
		lblRobotArm->Text = dataPick;

		/*********************************************************************
		 *** odczytujemy linijki pliku az natrafimy na definicje kontrolera **
		 *********************************************************************/
		do {
			getline(plikIFP,msgOriginal);
		} while (msgOriginal != "" && msgOriginal.compare(0,9,"CONT_CODE") != 0);
		//6. label CONTROLLER
		dataStart = msgOriginal.find_first_of(" ");
		int contStart = msgOriginal.find_first_of("0123456789",dataStart+1);
		int contStop = msgOriginal.find_first_of(" ",contStart+1);
		dataPick = msgOriginal.substr(contStart,contStop-contStart).c_str();
		if (dataPick == "701") {
			lblContType->Text = "D TYPE";
		} else if (dataPick == "") {
			lblContType->Text = "E TYPE";
		} else {
			lblContType->Text = "[unknown]";
		}
		//ustawiamy opis ze udalo sie zgrac informacje o robocie
		lblStatus->Text = "Robot info ok!";
	} else {
		kpcMsg->show("No file to fill robot info data...",MB_OK,MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
//whichNode = jezeli -1 to zapisz wszystko, w przeciwnym wypadku nadpisz wskazany
void TkpcOpt::saveRobotToRecent(String toFile, int whichNode)
{
	TXMLDocument *xml = new TXMLDocument(this);
	IXMLNode *nodeRoot, *nodeRobot, *nodeGroup, *nodeParam;
	//sprawdzamy czy dany plik istnieje
	if (FileExists(toFile,false) && whichNode >= 0) {
		//plik istnieje odczytujemy
		xml->LoadFromFile(toFile);
		//szukamy czy dany robot istnieje - jak tak to musimy tylko uaktualnic dany wezel
		nodeRoot = xml->DocumentElement;
		//sprawdzamy czy wezel ma w ogole dzieci i jest ich odpowiednia ilosc
		if (nodeRoot->HasChildNodes && nodeRoot->ChildNodes->Count > whichNode) {
			//usuwamy stary wpis
			nodeRoot->ChildNodes->Delete(whichNode);
			//wstawiamy nowy wpis
			nodeRobot = nodeRoot->AddChild("robot",whichNode);
		} else {
			//wezel nie ma dzieci lub ma ich za malo = dodajemy na koncu
			nodeRobot = nodeRoot->AddChild("robot",nodeRoot->ChildNodes->Count);
		}
	} else {
		//plik nie istnieje - tworzymy nowy
		xml->Active = true;
		xml->Version = "1.0";
		xml->Encoding = "UTF-8";
		nodeRoot = xml->AddChild("recentRobots");
		nodeRobot = nodeRoot->AddChild("robot",0);
	}
	/***** PARAMETRY GLOWNE ***************************/
	nodeRobot->SetAttribute("name",editRobotName->Text);
	String ipStr = editIPaddres1->Text+"."+editIPaddres2->Text+"."+editIPaddres3->Text+"."+editIPaddres4->Text;
	nodeRobot->SetAttribute("IP",ipStr);
	nodeRobot->SetAttribute("port",editPortNo->Text);
	nodeRobot->SetAttribute("type",lblRobotArm->Text);
	nodeRobot->SetAttribute("cont",lblContType->Text);

	/***** PARAMETRY PODRZEDNE 1 = plik zrodlowy ******/
	//wstawiamy nazwe pliku zrodlowego z ustawieniami robota
	String ipNoDot = editIPaddres1->Text+editIPaddres2->Text+editIPaddres3->Text+editIPaddres4->Text;
	String sourceFile = "robots//"+editRobotName->Text+"_IP"+ipNoDot+".as";
	nodeGroup = nodeRobot->AddChild("source",0);
	nodeGroup->SetAttribute("file",sourceFile);

	/***** PARAMETRY PODRZEDNE 2 = GROUP AS ***********/
	//wstawiamy opcje grupy AS
	nodeGroup = nodeRobot->AddChild("groupAS",1);
	nodeGroup->SetAttribute("id",groupRobotAS->Text);
	nodeParam = nodeGroup->AddChild("userAS",0);
	nodeParam->Text = lblUserIfAS->Text.SetLength(lblUserIfAS->Text.Length());
	nodeParam = nodeGroup->AddChild("userTP",1);
	nodeParam->Text = lblUserIfTP->Text.SetLength(lblUserIfTP->Text.Length());
	nodeParam = nodeGroup->AddChild("armCtrl",2);
	nodeParam->Text = lblArmAs->Text.SetLength(lblArmAs->Text.Length());
	nodeParam = nodeGroup->AddChild("armData",3);
	nodeParam->Text = lblArmData->Text.SetLength(lblArmData->Text.Length());
	nodeParam = nodeGroup->AddChild("msgFile",4);
	nodeParam->Text = lblASMsg->Text.SetLength(lblASMsg->Text.Length());

	/***** PARAMETRY PODRZEDNE 3 = SERVO AS ***********/
	//wstawiamy opcje grupy SERVO
	nodeGroup = nodeRobot->AddChild("groupSERVO",2);
	nodeGroup->SetAttribute("id",groupServo->Text);
	nodeParam = nodeGroup->AddChild("control",0);
	nodeParam->Text = lblControlServo->Text.SetLength(lblControlServo->Text.Length());
	nodeParam = nodeGroup->AddChild("dataFile",1);
	nodeParam->Text = lblDataFile->Text.SetLength(lblDataFile->Text.Length());
	//formatujemy XML (opcja doNodeAutoIndent slabo dziala)
	xml->XML->Text = FormatXMLData(xml->XML->Text);
	xml->Active = true;
	//zapisujemy plik
	xml->SaveToFile(toFile);
	//kasujemy obiekt xml
	delete xml;
}
//---------------------------------------------------------------------------
//whichNode = jezeli -1 to uaktualniamy liste, w przeciwnym wypadku info danego robota
void TkpcOpt::loadRobotFromRecent(String fromFile, int whichNode)
{
	TXMLDocument *xml = new TXMLDocument(this);
	IXMLNode *nodeRoot, *nodeRobot, *nodeGroup, *nodeParam;
	//sprawdzamy jest plik z ustawieniami
	if (FileExists(fromFile,false)) {
		//plik istnieje odczytujemy
		xml->LoadFromFile(fromFile);
		//znajdujemy wezel glowny
		nodeRoot = xml->DocumentElement;
		//sprawdzamy czy odczytac wszystkie pozycje i uzupelnic liste
		if (whichNode == -1) {
			//czyscimy liste zanim na nowo ja uzupelnimy
			listRecentRobots->Items->Clear();
			//odczytujemy po kolei jego dzieci
			for (int i=0; i < nodeRoot->ChildNodes->Count; i++) {
				nodeRobot = nodeRoot->ChildNodes->Get(i);
				//odczytujemy po kolei nazwy robotow i uzupelniamy liste
				String robName = nodeRobot->GetAttribute("name");
				listRecentRobots->Items->Add(robName);
			}
		} else {
			//chemy odczytac jeden konkretny robot, ale z detalami (o ile taki jest)
			if (whichNode <= nodeRoot->ChildNodes->Count) {
				/***** PARAMETRY GLOWNE ***************************/
				nodeRobot = nodeRoot->ChildNodes->Get(whichNode);
				//odczytujemy parametry polaczenia i uzupelniamy odpowiednie pola
				editRobotName->Text = nodeRobot->GetAttribute("name");
				editPortNo->Text = nodeRobot->GetAttribute("port");
				lblRobotArm->Text = nodeRobot->GetAttribute("type");
				lblContType->Text = nodeRobot->GetAttribute("cont");
                //adres IP
				String currIP = nodeRobot->GetAttribute("IP"), ipTemp=currIP;	int dotPos[3];
				//---- part 1 ----
				dotPos[0] = ipTemp.Pos(".");
				editIPaddres1->Text = ipTemp.SubString(0,dotPos[0]-1);
				ipTemp = ipTemp.SubString(dotPos[0]+1,ipTemp.Length()+1-dotPos[0]);
				//---- part 2 ----
				dotPos[1] = ipTemp.Pos(".")+dotPos[0];
				editIPaddres2->Text = ipTemp.SubString(0,(dotPos[1]-dotPos[0])-1);
				ipTemp = ipTemp.SubString((dotPos[1]-dotPos[0])+1,ipTemp.Length()+1-(dotPos[1]-dotPos[0]));
				//---- part 3 ----
				dotPos[2] = ipTemp.Pos(".")+dotPos[1];
				editIPaddres3->Text = ipTemp.SubString(0,(dotPos[2]-dotPos[1])-1);
				ipTemp = ipTemp.SubString((dotPos[2]-dotPos[1])+1,ipTemp.Length()+1-(dotPos[2]-dotPos[1]));
				//---- part 4 ----
				editIPaddres4->Text = ipTemp;

				/***** PARAMETRY PODRZEDNE 1 = plik zrodlowy ******/
				nodeGroup = nodeRobot->ChildNodes->Get(0);
				mainWindow->robot->updateSourceFile(nodeGroup->GetAttribute("file"));

				/***** PARAMETRY PODRZEDNE 2 = GROUP AS ***********/
				//odczytujemy opcje grupy AS
				nodeGroup = nodeRobot->ChildNodes->Get(1);
				groupRobotAS->Text = nodeGroup->GetAttribute("id");
				nodeParam = nodeGroup->ChildNodes->Get(0);
				lblUserIfAS->Text = nodeParam->Text;
				nodeParam = nodeGroup->ChildNodes->Get(1);
				lblUserIfTP->Text = nodeParam->Text;
				nodeParam = nodeGroup->ChildNodes->Get(2);
				lblArmAs->Text = nodeParam->Text;
				nodeParam = nodeGroup->ChildNodes->Get(3);
				lblArmData->Text = nodeParam->Text;
				nodeParam = nodeGroup->ChildNodes->Get(4);
				lblASMsg->Text = nodeParam->Text;

				/***** PARAMETRY PODRZEDNE 3 = SERVO AS ***********/
				//odczytujemy opcje grupy SERVO
				nodeGroup = nodeRobot->ChildNodes->Get(2);
				groupServo->Text = nodeGroup->GetAttribute("id");
				nodeParam = nodeGroup->ChildNodes->Get(0);
				lblControlServo->Text = nodeParam->Text;
				nodeParam = nodeGroup->ChildNodes->Get(1);
				lblDataFile->Text = nodeParam->Text;
			}
		}
 	}
	//kasujemy obiekt XML - juz nam nie jest potrzebny
	delete xml;
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnRemoveRobotClick(TObject *Sender)
{
	//wyswietlamy komunikat z pytaniem czy na pewno chcemy usunac
	kpcMsg->show("Remove selected robot?",MB_YESNO,MB_ICONWARNING);
	if (kpcMsg->getRes()==idYes) {
		//sprawdzamy czy zaznaczono jakis element listy ktory bedziemy chcieli usunac
		int selectedToRemove = listRecentRobots->Selected->Index;
		if (selectedToRemove > -1) {
			//usuwamy wpis z pliku
			deleteRobotFromRecent(mainWindow->recentRobotsFileName,selectedToRemove);
			//usuwamy element z listy
			listRecentRobots->Items->Delete(selectedToRemove);
			//zerujemy wybrany element oraz czyscimy pola opisu polaczenia
			listRecentRobots->ItemIndex = -1;
			btnClearConnSettingsClick(Sender);
		}
	}
}
//---------------------------------------------------------------------------
//whichNode = jezeli -1 to usuwamy cala liste, w przeciwnym wypadku tylko danego robota
void TkpcOpt::deleteRobotFromRecent(String recentFile, int whichNode)
{
	TXMLDocument *xml = new TXMLDocument(this);
	IXMLNode *nodeRoot, *nodeRobot;
	//sprawdzamy czy chcemy usunac tylko jednego robota z listy czy cala liste
	if (whichNode >= 0) {
		//usuwamy jednego robota
		if (FileExists(recentFile,false)) {
			//plik istnieje odczytujemy
			xml->LoadFromFile(recentFile);
			//szukamy czy dany robot istnieje - jak tak to musimy tylko uaktualnic dany wezel
			nodeRoot = xml->DocumentElement;
			//sprawdzamy czy wezel ma w ogole dzieci i jest ich odpowiednia ilosc
			if (nodeRoot->HasChildNodes && nodeRoot->ChildNodes->Count >= whichNode) {
				//usuwamy stary wpis
				nodeRoot->ChildNodes->Delete(whichNode);
			}
			//formatujemy XML (opcja doNodeAutoIndent slabo dziala)
			xml->XML->Text = FormatXMLData(xml->XML->Text);
			xml->Active = true;
			//zapisujemy plik
			xml->SaveToFile(recentFile);
        }
	} else {
		//usuwamy cala liste = caly plik
		if (FileExists(recentFile,false)) DeleteFileW(recentFile);
	}
	//kasujemy obiekt xml
	delete xml;
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::listRecentRobotsClick(TObject *Sender)
{
	if (listRecentRobots->Items->Count > 0) {
		if (listRecentRobots->Selected) {
			int selectedItem = listRecentRobots->Selected->Index;
			if (selectedItem >= 0) {
				loadRobotFromRecent(mainWindow->recentRobotsFileName,selectedItem);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnUpdateRobotClick(TObject *Sender)
{
	if (listRecentRobots->Selected) {
		int selectedItem = listRecentRobots->Selected->Index;
		//zapisujemy dane o robocie do pliku
		saveRobotToRecent(mainWindow->recentRobotsFileName,selectedItem);
		//uaktualniamy liste (bardziej chodzi o opis)
		loadRobotFromRecent(mainWindow->recentRobotsFileName);
		//wracamy z wyborem na zmieniony element
		listRecentRobots->ItemIndex = selectedItem;
	} else {
		//nic nie wybrano = nie wiadomo co updateowac
		kpcMsg->show("Select robot from list!",MB_OK,MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnUpdRobotInfoClick(TObject *Sender)
{
    String ext = ".as";
	//sprawdzamy czy jestesmy polaczeni z robotem
	if (mainWindow->robot->isConnected()) {
		//deaktywujemy przycisk rozlaczenia
		btnDisconnect->Enabled = false;
		//robot jest polaczony bedziemy sciagac z niego plik = budujemy jego nazwe
		String ipStr = editIPaddres1->Text+editIPaddres2->Text+editIPaddres3->Text+editIPaddres4->Text;
		String destLocation = GetCurrentDir()+"\\robots\\"+editRobotName->Text+"_IP"+ipStr;
        String tempFile = "C:\\tempKPC";
		//probujemy pobrac informacje o robocie z kontrolera
		if (FileExists(tempFile+ext,false)) {
			DeleteFileW(tempFile+ext);
		}
		kawasakiStatus currStatus = mainWindow->robot->getRobotInfo(tempFile);
		if (currStatus == ok) {
			lblStatus->Text = "Downloading file...";
			Application->ProcessMessages();
			//sprawdzamy jaki jest stan robota (czy zgral plik czy jeszcze zgrywa)
			String feedback = ""; int retryCount = 0;
			do {
				//czekamy 25 milisekund (zgodnie z dokumentacja)
				Sleep(100);
				currStatus = mainWindow->robot->getStatus(feedback);
				retryCount++;
				if (retryCount % 10 == 0) {
					lblStatus->Text += ".";
					Application->ProcessMessages();
				}
			} while (currStatus != ok  && retryCount < 100);
			//sprawdzamy czy juz jest plik
			if (FileExists(tempFile+ext,false)) {
				//udalo sie sciagnac plik - przenosimy go pod zmieniona nazwa do katalogu robots
				if (FileExists(destLocation+ext,false)) {
					DeleteFileW(destLocation+ext);
				}
				//przenosimy plik
				MoveFileW((tempFile+ext).w_str(),(destLocation+ext).w_str());
				//uzupelniamy wszystkie pola w grupie INFORMATION
				fillRobotInfo(destLocation+ext);
			} else {
				kpcMsg->show("Cant read file from robot!",MB_OK,MB_ICONERROR);
            }
		} else {
			kpcMsg->show("Cant download file from robot!",MB_OK,MB_ICONERROR);
		}
		//po wszystkim aktywujemy przycisk rozlaczenia
		btnDisconnect->Enabled = true;
	} else {
        kpcMsg->show("Not connected to robot!",MB_OK,MB_ICONERROR);
    }
}
//---------------------------------------------------------------------------

/***************************************************************************
 ******  ZAKLADKA ABOUT    *************************************************
 ***************************************************************************/

//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnUpdateChildCountClick(TObject *Sender)
{
	String msg = "";
	TFmxObject *objWithTiles = new TFmxObject(NULL);
	for (int i = 0; i < mainWindow->tabPanelPages->TabCount; i++) {
		if (mainWindow->objectHasTiles(mainWindow->tabPanelPages->Tabs[i],*objWithTiles)) {
			int childrenCnt = objWithTiles->ChildrenCount;
			msg += "Page no. "+IntToStr(i+1)+" -> "+IntToStr(childrenCnt)+"\n";
		}
	}
	kpcMsg->show(msg,MB_OK,MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TkpcOpt::btnTest_extractProjClick(TObject *Sender)
{
    String source, dest;
	//pokazujemy okno z wyborem pliku zrodlowego (w domysle zgranego z robota)
	mainWindow->openProjectDialog->Filter = "All files (*.*)|*.*";
	mainWindow->openProjectDialog->InitialDir = GetCurrentDir();
	if (mainWindow->openProjectDialog->Execute()) {
		//jezeli wybrano jakikolwiek plik to mozemy probowac wyodrebnic z niego projekt
		source = mainWindow->openProjectDialog->FileName;
		int extPos = source.Pos(".");
		dest = source.SubString(0,extPos-1)+"_ext.ifp";
		mainWindow->extractProjectFromFile(source,dest);
	}
}
//---------------------------------------------------------------------------


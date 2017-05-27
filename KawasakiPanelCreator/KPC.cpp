//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop
#include "KPC.h"
#include "KPCmessage.h"
#include "KPCoptions.h"
#include "KPCrobot.h"
#include "newProject.h"
#include "openProject.h"
#include "newPage.h"
//---------------------------------------------------------------------------
#include <fstream.h>
#include <string>
#include <inifiles.hpp>
#include <vector>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TmainWindow *mainWindow;
//---------------------------------------------------------------------------
/*
 *
 */
__fastcall TmainWindow::TmainWindow(TComponent* Owner)
	: TForm(Owner)
{
	//-----------------------------------------------------------------------
	// 0. odczytujemy ustawienia dla wszystkich flag
	settingsFilePath = GetCurrentDir()+"\\system\\app.settings";
	loadSettings(settingsFilePath);
	//-----------------------------------------------------------------------
	// 1. sprawdzamy czy pokazac okno powitalne (splash screen)
	if (showSplash) {
		//uzupelniamy informacje o wersji aplikacji
		String fileVer = GetCurrentDir()+"\\KawasakiPanelCreator.exe";
		lblSplashVerNo->Text = "ver. "+getVersionNumber(fileVer);
		//ustawiamy splashscreen na srodku pulpitu
		splashScreen->Position->X = (Width-splashScreen->Width)/2;
		splashScreen->Position->Y = (Height-splashScreen->Height)/2;
        //widoczny ma tylko byc TPanel (splashScreen)
		mainWindow->Transparency = true;
		mainWindow->Visible = false;
		mainWindow->mainBackground->Visible = false;
		mainWindow->mainMenuTop->Visible = false;
		mainWindow->splashScreen->Visible = true;
		//pokazujemy informacje o otwieraniu projektu
		lblSplashStatus->Text = "STATUS: opening application...";
	} else {
    	mainWindow->splashScreen->Visible = false;
    }
	//-----------------------------------------------------------------------
	// 2. obsluga stron z kafelkami
	noOfPages = 4;
	mostTop = 10; mostLeft = 10;
	setNoOfTabsVisible(noOfPages);
	page1Mask->BringToFront();	page2Mask->BringToFront();	page3Mask->BringToFront();
	page4Mask->BringToFront();	page5Mask->BringToFront();	page6Mask->BringToFront();
	page7Mask->BringToFront();	page8Mask->BringToFront();
	//-----------------------------------------------------------------------
	// 3. stan okna + zakladki z patternami
	Width = 647;	Top = 50;
	Left = (GetSystemMetrics(SM_CXSCREEN)-Width)/2;
	//zakladka z patternami
	patternTilesUnfolded=false;
	tileSelectionGrabber->Height = 160;
	tileSelectionGrabber->Width = ClientWidth;
	tileSelectionGrabber->Position->X = 0;
	tileSelectionGrabber->BringToFront();
	//wyswietlenie liczby kafli bazowych
	lblPatternTilesNo->Text = "0";
	//sprawdzamy czy okno ma byc wieksze i miec otwarte patterny
	if (!patternsAtFront) {
		//patterny schowane
		Height = 620;
		_gmiAnimation->Enabled = true;
		_gmiAlwaysOn->IsChecked = false;
		tileSelectionGrabber->Position->Y=ClientHeight-mainMenuTop->Height-30;
		buttonSHtiles->Canvas->Fill->Color = claLightgray;
	} else {
		//patterny zawsze na wierzchu
		Height = 750;
		_gmiAnimation->Enabled = false;
		_gmiAlwaysOn->IsChecked = true;
		tileSelectionGrabber->Position->Y=ClientHeight-mainMenuTop->Height-160;
		buttonSHtiles->Canvas->Fill->Color = claLightgray;
	}
	//sprawdzamy czy jest plik z kafelkami wzorcowymi
	patternsFileName = GetCurrentDir()+"\\patternTiles.xml";
	if (!FileExists(patternsFileName,false)) {
		//pliku nie ma dlatego wczytujemy oryginal z zasobow programu
		String resName = L"originalPatterns";
		TResourceStream *rStream = new TResourceStream((unsigned int)HInstance,resName,RT_RCDATA);
		rStream->SaveToFile(patternsFileName);
		delete rStream;
	}
	//wczytujemy kafelki wzorcowe
	loadTilesFromXML(scrollBaseTiles,patternsFileName);
	//-----------------------------------------------------------------------
	// 4. sprawdzamy akcje ktora nalezy wykonac po otwarciu aplikacji
	if (openAction == action::lastProject) {
		if (saveFileName != "") {
			//URUCHAMIAMY WATEK WCZYTUJACY OSTATNI PROJEKT
			loadIndicator->Enabled = true;
			loadIndicator->Visible = true;
			//tworzymy nowy projekt
			TThreadOpenProj *theadOpenProj = new TThreadOpenProj(true,saveFileName);
			theadOpenProj->Priority = tpLowest;
			theadOpenProj->Start();
		} else {
			if (showSplash) {
				lblSplashProgress->Visible = false;
				lblSplashStatus->Text = "STATUS: opening application...";
				splashTime->Interval = 3000;
				splashTime->Enabled = true;
			}
        }
	} else if (openAction == action::newProject) {
		//URUCHAMIAMY WATEK OTWIERAJACY NOWY PROJEKT
		loadIndicator->Enabled = true;
		loadIndicator->Visible = true;
		//tworzymy nowy projekt
		TThreadNewProj *theadNewProj = new TThreadNewProj(true);
		theadNewProj->Priority = tpLowest;
		theadNewProj->Start();
	} else if (openAction == action::nothing) {
		//uruchamiamy timer po ktorym schowamy splash screen
		if (showSplash) {
			lblSplashProgress->Visible = false;
			lblSplashStatus->Text = "STATUS: opening application...";
			splashTime->Interval = 3000;
			splashTime->Enabled = true;
		}
	}
	//-----------------------------------------------------------------------
	// 5. obsluga menu glownego
	projectActive = false;
	changesDone = false;
	saveOK = false;
	saveMode = saveQuick;
	robotControl = true;
	robotCheckEnabled = false;
	TResourceStream *resource = new TResourceStream((unsigned int)HInstance,L"mainMenuImg",RT_RCDATA);
	mainMenuIcon->Bitmap->LoadFromStream(resource);
	delete resource;
	//ustawiamy panel z menu glownym tak zeby nie byl widoczny
	mainMenuVisible = false;
	panelMenu->Position->X = -150;
	//ustawienia nazwy pliku do zapisu
	btnSave->Enabled = false;
	saveFileName = "";
	//-----------------------------------------------------------------------
	// 6. obiekt do komunikacji z robotem Kawasaki
	robot = new kawasakiComm("",21,false);
	recentRobotsFileName = GetCurrentDir()+"\\robots\\recentRobots.xml";
	//usuwamy zalegly proces komunikacji (o ile istnieje i o ile tego chcemy)
    killDLLcomm();
	//inicjalizujemy DLLke
	if (initDLL) robot->initComm();
	//czyscimy tablice wybranych elementow
	selectedTableClear();
	//-----------------------------------------------------------------------
	// 7. opcja autosave'u
	autosaveTimer->Interval = autosaveTime*60*1000;
	autosaveTimer->Enabled = autosaveActive;
	//-----------------------------------------------------------------------
	// 8. obsluga kursora
	mainWindow->Cursor = crArrow;
	//-----------------------------------------------------------------------
	// 9. zastosowanie ustawien
	applySettings();
	//-----------------------------------------------------------------------
	// 10. obsluga priorytetu aplikacji
	setAppPriority();
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::buttonExitClick(TObject *Sender)
{
	bool exitOK = true;
	//sprawdzamy czy wyswietlic wiadomosc o projekcie
	if (msgOnProject) {
		//sprawdzamy czy juz jest otwarty jest jakis projekt
		if (projectActive) {
			//sprawdzamy czy byly jakiekolwiek zmiany w projekcie
			if (changesDone) {
				//byly zmiany wiec pytamy sie usera czy chce zapisac obecny projekt
				kpcMsg->show("Do you want to save current project before exit?",MB_YESNOCANCEL,MB_ICONQUESTION);
				int msgResult = kpcMsg->getRes();
				//sprawdzamy co wcisnieto
				if (msgResult == idCancel){
					exitOK = false;
				} else if (msgResult == idYes){
					btnSaveMouseDown(Sender,TMouseButton::mbLeft,(TShiftState)ssShift,0,0);
				}
			} else {
				//nie bylo zadnych zmian wiec nie ma po co pytac usera o zapis
                exitOK = true;
            }
		}
	} else {
		exitOK = true;
    }
	//sprawdzamy czy wyswietlic wiadomosc o projekcie (jezeli nie bylo cancel i pokazujemy wiadomosci)
	if (exitOK) {
		if (msgOnExit) {
			//pytamy sie usera czy chce zapisac obecny projekt
			kpcMsg->show("Exit application?",MB_YESNO,MB_ICONQUESTION);
			int msgResult = kpcMsg->getRes();
			//sprawdzamy co wcisnieto
			if (msgResult == idNo){
				exitOK = false;
			} else if (msgResult == idYes){
				exitOK = true;
			}
		} else {
			exitOK = true;
		}
	}
	//sprawdzamy czy jest zgoda na wylaczenie aplikacji
	if (exitOK) {
		//zapisuemy sciezke do otwartego projektu
		if (projectActive) {
			mainWindow->saveSettingsProjPath(saveFileName);
		}
		//sprawdzamy czy zapisac kafelki wzorcowe
		if (patternSave == action::atExit) {
			saveTilesToXML(scrollBaseTiles,"patternTiles.xml");
		}
		//mamy juz jakis projekt - musimy skasowac poprzednie kafelki!
		for (int i = 0; i < tabPanelPages->TabCount; i++) {
			clearTilesInPage(tabPanelPages->Tabs[i]);
		}
		//usuwamy obiekt odpowiedzialny za polaczenie z robotem
		delete robot;
		//wylaczamy aplikacje
		Application->Terminate();
	}
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::pathMouseOnProcess(TObject *Sender)
{
	buttonSHtiles->Enabled=false;
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::pathMouseOffProcess(TObject *Sender)
{
	buttonSHtiles->Enabled=false;
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::pathMouseOffFinish(TObject *Sender)
{
	patternTilesUnfolded=false;
	buttonSHtiles->Enabled=true;
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::pathMouseOnFinish(TObject *Sender)
{
	patternTilesUnfolded=true;
	buttonSHtiles->Enabled=true;
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::_gmiAnimationClick(TObject *Sender)
{
	//zmieniamy stan zaznaczenia
	_gmiAnimation->IsChecked = !_gmiAnimation->IsChecked;
	//wlaczamy lub wylaczamy animacje w zaleznosci od ustawienia
	if(_gmiAnimation->IsChecked) {
		animationTime = lastSpeed;
	} else {
		animationTime = 0.01;
	}
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::_gmiAlwaysOnClick(TObject *Sender)
{
	//zmieniamy stan zaznaczenia
	_gmiAlwaysOn->IsChecked = !_gmiAlwaysOn->IsChecked;
	//pokazujemy lub wylaczamy okno w zaleznosci od ustawien
	if(_gmiAlwaysOn->IsChecked) {
		_gmiAnimation->Enabled=false;
		mainWindow->ClientHeight+=130;
		tileSelectionGrabber->Position->Y=ClientHeight-mainMenuTop->Height-160;
		buttonSHtiles->Canvas->Fill->Color = claLightgray;
	} else {
		_gmiAnimation->Enabled=true;
		mainWindow->ClientHeight-=130;
		buttonSHtiles->Canvas->Fill->Color = claOrange;
		//czy przed poprzednia zmiana okno z kaflami bazowymi bylo wlaczone
		if(patternTilesUnfolded) {
			tileSelectionGrabber->Position->Y=ClientHeight-mainMenuTop->Height-160;
		} else {
			tileSelectionGrabber->Position->Y=ClientHeight-mainMenuTop->Height-30;
		}
	}
	mainWindow->Position=TFormPosition::poScreenCenter;
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::buttonSHtilesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
    //sprawdzamy czy nie ma gdzies aktywnych przyciskow opcji
	if(!optionsTile) {
		if(Button==TMouseButton::mbLeft) {
			if(!_gmiAlwaysOn->IsChecked) {
				//chowamy/otwieramy menu
				int positionHide = tileSelectionGrabber->Height-30;
				if(patternTilesUnfolded) {
					pathMouseOff->Path->Data="M 0,0 L 0,"+IntToStr(positionHide);
					pathMouseOff->Duration = animationTime;
					pathMouseOff->Start();
				} else {
					pathMouseOn->Path->Data="M 0,0 L 0,-"+IntToStr(positionHide);
					pathMouseOn->Duration = animationTime;
					pathMouseOn->Start();
				}
			}
		} else if(Button==TMouseButton::mbRight) {
			//ustawienia menu - wywolywany popup
			//tutaj mozna tez dodac jakas akcje ktora ma sie dodatkowo wykonac
		}
	} else {
		//pokaz ktory kafelek ma aktywny przycisk opcji
		if (tileEmphasize) optionsTile->emphasizeTile();
	}
}
//---------------------------------------------------------------------------
/*
 *
 */
void __fastcall TmainWindow::addNewPatternTileClick(TObject *Sender)
{
	//sprawdzamy czy w ogole aktywna jest opcja dodawania kafelkow wzorcowych
	if (patternsAdd) {
		//sprawdzamy czy nie ma gdzies aktywnych przyciskow opcji
		if(!optionsTile) {
			//tworzymy nowy zerowy element
			zeroTile *newPatternTile = createTile(0,patternCustom,scrollBaseTiles);
			//liczymy ile jest elementow podrzednych w wewnetrznym kontenerze (!)
			int elementsNo = scrollBaseTiles->Children[1]->ChildrenCount;
			newPatternTile->Position->X=(elementsNo-1)*92;
			newPatternTile->Position->Y=5;
			//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
			newPatternTile->setGlowColor(glowRegular,glowProtected,glowPattern);
			newPatternTile->setEmphasizeColor(colorEmphasize);
			//aktualizacja liczby kafelkow bazowych
			lblPatternTilesNo->Text = IntToStr(elementsNo);
			//ponizszy kod ze wzgledu na BUG w komponencie TScrollBox
			if (elementsNo>5) {
				TPanel *tempTile = new TPanel(scrollBaseTiles);
				tempTile->Parent=scrollBaseTiles;
				tempTile->Height = 100;
				tempTile->Width = 80;
				tempTile->Position->X=(elementsNo)*100;
				tempTile->Position->Y=5;
				//usuwamy tymczasowy element
				tempTile->Parent=NULL;
				delete tempTile;
			}
		} else {
			//pokaz ktory kafelek ma aktywny przycisk opcji
			if (tileEmphasize) optionsTile->emphasizeTile();
		}
	} else {
		//info do usera ze dodawanie jest zablokowane
		kpcMsg->show("Adding patterns disabled.",MB_OK,MB_ICONWARNING);
    }
}
//---------------------------------------------------------------------------
//przesuwamy formularz
//---------------------------------------------------------------------------
void __fastcall TmainWindow::mainMenuTopMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	StartWindowDrag();
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::mainMenuIconClick(TObject *Sender)
{
	if (mainMenuVisible) {
		mainMenuPosXanim->StartValue = 59;
		mainMenuPosXanim->StopValue = -160;
		mainMenuPosXanim->Start();
	} else {
		mainMenuPosXanim->StartValue = -160;
		mainMenuPosXanim->StopValue = 59;
		mainMenuPosXanim->Start();
	}
    mainMenuVisible = !mainMenuVisible;
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::mainMenuBtnMouseLeave(TObject *Sender)
{
	if (Sender->ClassNameIs("TButton")) {
		TButton *btn = dynamic_cast<TButton*>(Sender);
		if (btn->Parent->ClassNameIs("TPanel")) {
			TPanel *panelParent = dynamic_cast<TPanel*>(btn->Parent);
			panelParent->Repaint();
		}
	}
}
//---------------------------------------------------------------------------
//metoda po kliknieciu w przycisk tworzenia nowego projektu
//---------------------------------------------------------------------------
void __fastcall TmainWindow::btnNewClick(TObject *Sender)
{
	try {
		//jezeli aktywny jest tryb sprawdzenia to nie wczytaj
		if (!robotCheckEnabled) {
			bool okToGo = true;
			//sprawdzamy czy wyswietlic wiadomosc o projekcie
			if (msgOnProject) {
				//chcemy komunikat o projekcie sprawdzamy czy jest jakis otwarty
				if (projectActive) {
					//sprawdzamy czy byly jakiekolwiek zmiany w projekcie
					if (changesDone) {
						//byly zmainy wiec pytamy sie usera czy chce zapisac obecny projekt
						kpcMsg->show("Do you want to save curret project before creating new one?",MB_YESNOCANCEL,MB_ICONQUESTION);
						int msgResult = kpcMsg->getRes();
						//sprawdzamy co wcisnieto
						if (msgResult == idCancel){
							okToGo = false;
						} else if (msgResult == idYes){
							btnSaveMouseDown(Sender,TMouseButton::mbLeft,(TShiftState)ssShift,0,0);
						}
					} else {
						//nie bylo zadnych zmian w projekcie - mozna smialo tworzyc nowy
						okToGo = true;
					}
				} else {
					//nie ma jeszcze nowego projektu - smialo mozna tworzyc
					okToGo = true;
				}
			} else {
				//nie chcemy komunikatu o projekcie
				okToGo = true;
			}
			//sprawdzamy czy spelnione sa wszystkie warunki aby otworzyc nowy projekt
			if (okToGo) {
				lblAppTitle->Text = "KAWASAKI PANEL CREATOR";
				//pokazujemy animacje wczytywania
				loadIndicator->Enabled = true;
				loadIndicator->Visible = true;
				//usuwamy obecne kafelki
				for (int i = 0; i < tabPanelPages->TabCount; i++) {
					clearTilesInPage(tabPanelPages->Tabs[i]);
				}
				//tworzymy nowy projekt
				TThreadNewProj *theadNewProj = new TThreadNewProj(true);
				theadNewProj->Priority = tpLowest;
				theadNewProj->Start();
			}
		} else {
			kpcMsg->show("Turn off project check.",MB_OK,MB_ICONINFORMATION);
		}
	} catch (...) {
		kpcMsg->show("Exception: in new project !!!",MB_OK,MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::btnSaveMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	if (Button==TMouseButton::mbLeft) {
 		//jezeli aktywny jest tryb sprawdzenia to nie wczytaj
		if (!robotCheckEnabled) {
			try {
				bool showDialog = false, forDCont = false;
				//sprawdzamy czy jest aktywny projekt (nowy lub otwarty)
				if (projectActive) {
					//sprawdzamy czy pokazac okienko dialogowe do zapisu pliku
					if (saveMode == saveAs || saveFileName.IsEmpty()) {
						showDialog = true;
						forDCont = false;
					} else {
						//nie jest to tryb saveAs oraz zostal juz wczytany jakis plik
						if (saveMode == saveQuick) {
							//po prostu zapisujemy plik
							showDialog = false;
							forDCont = false;
						} else if (saveMode == saveCont) {
							//pytanie do usera dla jakiego kontrolera zapisac plik
							String labels[3] = {"D CONTROLLER","D+ CONTROLLER","E CONTROLLER"}; int ids[3] = {1,1,2};
							kpcMsg->showCustom("Select controler type.",MB_ICONINFORMATION,3,labels,ids);
							forDCont = kpcMsg->getRes()==ids[0];
							showDialog = true;
						}
					}
					//w zaleznosci od ustawien zapisujemy plik lub pokazujemy okno dialogowe
					if (showDialog) {
						//ustawiamy folder poczakowy
						saveProjectDialog->Filter = "KPC project files (*.ifp)|*.ifp|""All files (*.*)|*.*";
						saveProjectDialog->InitialDir = GetCurrentDir()+"\\project\\";
						if (saveProjectDialog->Execute()) {
							saveProjectToIFP(saveProjectDialog->FileName,forDCont);
							saveOK = true;
						}
						//symulujemy klikniecie myszy (srodkowy przycisk ktory nic nie robi)
						mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0);
						mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0);
					} else {
						saveProjectToIFP(saveFileName);
					}
					//sprawdzamy czy zapisac kafelki wzorcowe
					if (patternSave == action::atSaveProject) {
						saveTilesToXML(scrollBaseTiles,"patternTiles.xml");
					}
					//po wszystkim uzupelniamy informacje o ostatnio zapisanym pliku
					if (showDialog && saveOK && saveMode == saveQuick) {
						saveFileName = saveProjectDialog->FileName;
					}
					lblAppTitle->Text = "KAWASAKI PANEL CREATOR  ["+projNameFromPath(saveFileName)+"]";
					//resetujemy flage o zmianach w projekcie bo sa juz zapisane
					changesDone = false;
				}
			} catch (...) {
				kpcMsg->show("Exception: in save project !!!",MB_OK,MB_ICONWARNING);
			}
		} else {
			kpcMsg->show("Turn off project check.",MB_OK,MB_ICONINFORMATION);
		}
	} else if (Button==TMouseButton::mbRight) {
		//zmieniamy tryb zapisu i od razu zmieniamy ikone
		if (saveMode == saveQuick) {
			saveMode = saveAs;
			//mamy tryb Zapisz jako...
			btnSave->StyleLookup = "btnSaveAsStyle1";
		} else if (saveMode == saveAs) {
			saveMode = saveCont;
			//mamy tryb Zapisz dla kontrolera...
			btnSave->StyleLookup = "btnSaveContStyle1";
		} else if (saveMode == saveCont) {
			saveMode = saveQuick;
			//mamy tryb Zapisz
			btnSave->StyleLookup = "btnSaveStyle1";
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::btnOpenClick(TObject *Sender)
{
	try {
		//jezeli aktywny jest tryb sprawdzenia to nie wczytaj
		if (!robotCheckEnabled) {
			bool okToGo = true;
			//sprawdzamy czy wyswietlic wiadomosc o projekcie
			if (msgOnProject) {
				//chcemy komunikat o projekcie sprawdzamy czy jest jakis otwarty
				if (projectActive) {
					//sprawdzamy czy byly jakiekolwiek zmiany w projekcie
					if (changesDone) {
						//byly zmiany wiec pytamy sie usera czy chce zapisac obecny projekt
						kpcMsg->show("Do you want to save curret project before opening new one?",MB_YESNOCANCEL,MB_ICONQUESTION);
						int msgResult = kpcMsg->getRes();
						//sprawdzamy co wcisnieto
						if (msgResult == idCancel){
							okToGo = false;
						} else if (msgResult == idYes){
							btnSaveMouseDown(Sender,TMouseButton::mbLeft,(TShiftState)ssShift,0,0);
						}
					} else {
						//nie bylo zadnych zmian w projekcie - mozna smialo otworzyc
						okToGo = true;
					}
				} else {
					//nie ma jeszcze nowego projektu - smialo mozna otwierac
					okToGo = true;
				}
			} else {
				//nie chcemy komunikatu o projekcie
				okToGo = true;
			}
			//sprawdzamy czy spelnione sa wszystkie warunki aby otworzyc nowy projekt
			if (okToGo) {
				//ustawiamy folder poczakowy
				openProjectDialog->Filter = "KPC project files (*.ifp)|*.ifp|""All files (*.*)|*.*";
				openProjectDialog->InitialDir = GetCurrentDir()+"\\project\\";
				if (openProjectDialog->Execute()) {
					String fileName = openProjectDialog->FileName;
					mainWindow->lblAppTitle->Text = "KAWASAKI PANEL CREATOR  ["+projNameFromPath(fileName)+"]";
					//pokazujemy animacje wczytywania
					loadIndicator->Enabled = true;
					loadIndicator->Visible = true;
					//usuwamy obecne kafelki
					for (int i = 0; i < tabPanelPages->TabCount; i++) {
						clearTilesInPage(tabPanelPages->Tabs[i]);
					}
					//tworzymy nowy projekt
					TThreadOpenProj *theadOpenProj = new TThreadOpenProj(true,fileName);
					theadOpenProj->Priority = tpLowest;
					theadOpenProj->Start();
				}
			}
		} else {
			kpcMsg->show("Turn off project check.",MB_OK,MB_ICONINFORMATION);
		}
	} catch (...) {
		kpcMsg->show("Exception: in open project !!!",MB_OK,MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::btnRobotMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, float X, float Y)
{
	if (Button==TMouseButton::mbLeft) {
		//sprawdzamy w jakim trybie jestesmy
		if (robotControl) {
			//tryb kontroli robota (wgranie/odczyt panelu)
			mainMenuTop->Enabled = false;
			KPCtoRobot->ShowModal();
			//symulujemy klikniecie myszy (srodkowy przycisk ktory nic nie robi)
			mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0);
			mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0);
			mainMenuTop->Enabled = true;
		} else {
			//tryb sprawdzenia projektu
			robotCheckEnabled = !robotCheckEnabled;
			robotGray->Enabled = !robotCheckEnabled;
			viewDControllerCompability(robotCheckEnabled);
			if (robotCheckEnabled) {
				btnRobot->StyleLookup = "btnRobotCheckOnStyle1";
			} else {
				btnRobot->StyleLookup = "btnRobotCheckOffStyle1";
			}
		}
	} else if (Button==TMouseButton::mbRight) {
		//tryb mozemy zmienic tylko gdy wylaczymy pokazywanie niekompatybilnosci
		if (!robotCheckEnabled) {
			robotControl = !robotControl;
		}
    	//zmieniamy ikone menu glownego
		if (robotControl) {
			btnRobot->StyleLookup = "btnRobotStyle1";
			robotGray->Enabled = false;
		} else {
			if (robotCheckEnabled) {
				btnRobot->StyleLookup = "btnRobotCheckOnStyle1";
			} else {
				btnRobot->StyleLookup = "btnRobotCheckOffStyle1";
			}
			robotGray->Enabled = !robotCheckEnabled;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::btnSettingsClick(TObject *Sender)
{
	mainMenuTop->Enabled = false;
	kpcOpt->ShowModal();
	//zapisujemy ustawienia do pliku
	saveSettings(settingsFilePath);
	mainMenuTop->Enabled = true;
	//zastosowujemy ustawienia
    applySettings();
}
//---------------------------------------------------------------------------

//*********************** MANAGER KAFELKOW **********************************
//---------------------------------------------------------------------------
//funkcja do tworzenia nowego kafelka
//---------------------------------------------------------------------------
zeroTile* TmainWindow::createTile(int ID,tileType type,TFmxObject *parent)
{
	if (ID == 0) {
		zeroTile *tileZero = new zeroTile(parent,type);
		tileZero->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileZero->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileZero->OnMouseDown = tileMouseDown;
		tileZero->delButton->OnClick  = tileDeleteButtonClick;
		tileZero->swapButton->OnClick  = tileSwapButtonClick;
		tileZero->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileZero->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileZero->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileZero->copyButton->OnClick  = tileCopyButtonClick;
		tileZero->pasteButton->OnClick  = tilePasteButtonClick;
		tileZero->modButton->OnClick  = tileModifyButtonClick;
		tileZero->doneButton->OnClick = tileDoneButtonClick;
		return tileZero;
	} else if (ID == 1) {
		pilotLamp *tilePilotLamp = new pilotLamp(parent,type);
		tilePilotLamp->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tilePilotLamp->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tilePilotLamp->OnMouseDown = tileMouseDown;
		tilePilotLamp->delButton->OnClick  = tileDeleteButtonClick;
		tilePilotLamp->swapButton->OnClick  = tileSwapButtonClick;
		tilePilotLamp->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tilePilotLamp->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tilePilotLamp->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tilePilotLamp->copyButton->OnClick  = tileCopyButtonClick;
		tilePilotLamp->pasteButton->OnClick  = tilePasteButtonClick;
		tilePilotLamp->modButton->OnClick  = tileModifyButtonClick;
		tilePilotLamp->doneButton->OnClick = tileDoneButtonClick;
		return tilePilotLamp;
	} else if (ID == 2) {
		pushButton *tilePushButton = new pushButton(parent,type);
		tilePushButton->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tilePushButton->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tilePushButton->OnMouseDown = tileMouseDown;
		tilePushButton->delButton->OnClick  = tileDeleteButtonClick;
		tilePushButton->swapButton->OnClick  = tileSwapButtonClick;
		tilePushButton->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tilePushButton->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tilePushButton->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tilePushButton->copyButton->OnClick  = tileCopyButtonClick;
		tilePushButton->pasteButton->OnClick  = tilePasteButtonClick;
		tilePushButton->modButton->OnClick  = tileModifyButtonClick;
		tilePushButton->doneButton->OnClick = tileDoneButtonClick;
		return tilePushButton;
	} else if (ID == 3) {
		pushButtonLamp *tilePushButtonWLamp = new pushButtonLamp(parent,type);
		tilePushButtonWLamp->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tilePushButtonWLamp->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tilePushButtonWLamp->OnMouseDown = tileMouseDown;
		tilePushButtonWLamp->delButton->OnClick  = tileDeleteButtonClick;
		tilePushButtonWLamp->swapButton->OnClick  = tileSwapButtonClick;
		tilePushButtonWLamp->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tilePushButtonWLamp->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tilePushButtonWLamp->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tilePushButtonWLamp->copyButton->OnClick  = tileCopyButtonClick;
		tilePushButtonWLamp->pasteButton->OnClick  = tilePasteButtonClick;
		tilePushButtonWLamp->modButton->OnClick  = tileModifyButtonClick;
		tilePushButtonWLamp->doneButton->OnClick = tileDoneButtonClick;
		return tilePushButtonWLamp;
	} else if (ID == 4) {
		selector2 *tileSelector2 = new selector2(parent,type);
		tileSelector2->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileSelector2->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileSelector2->OnMouseDown = tileMouseDown;
		tileSelector2->delButton->OnClick  = tileDeleteButtonClick;
		tileSelector2->swapButton->OnClick  = tileSwapButtonClick;
		tileSelector2->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileSelector2->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileSelector2->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileSelector2->copyButton->OnClick  = tileCopyButtonClick;
		tileSelector2->pasteButton->OnClick  = tilePasteButtonClick;
		tileSelector2->modButton->OnClick  = tileModifyButtonClick;
		tileSelector2->doneButton->OnClick = tileDoneButtonClick;
		return tileSelector2;
	} else if (ID == 5) {
		selector3 *tileSelector3 = new selector3(parent,type);
		tileSelector3->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileSelector3->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileSelector3->OnMouseDown = tileMouseDown;
		tileSelector3->delButton->OnClick  = tileDeleteButtonClick;
		tileSelector3->swapButton->OnClick  = tileSwapButtonClick;
		tileSelector3->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileSelector3->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileSelector3->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileSelector3->copyButton->OnClick  = tileCopyButtonClick;
		tileSelector3->pasteButton->OnClick  = tilePasteButtonClick;
		tileSelector3->modButton->OnClick  = tileModifyButtonClick;
		tileSelector3->doneButton->OnClick = tileDoneButtonClick;
		return tileSelector3;
	} else if (ID == 6) {
		digitalSwitch *tileDigitalSwitch = new digitalSwitch(parent,type);
		tileDigitalSwitch->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileDigitalSwitch->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileDigitalSwitch->OnMouseDown = tileMouseDown;
		tileDigitalSwitch->delButton->OnClick  = tileDeleteButtonClick;
		tileDigitalSwitch->swapButton->OnClick  = tileSwapButtonClick;
		tileDigitalSwitch->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileDigitalSwitch->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileDigitalSwitch->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileDigitalSwitch->copyButton->OnClick  = tileCopyButtonClick;
		tileDigitalSwitch->pasteButton->OnClick  = tilePasteButtonClick;
		tileDigitalSwitch->modButton->OnClick  = tileModifyButtonClick;
		tileDigitalSwitch->doneButton->OnClick = tileDoneButtonClick;
		return tileDigitalSwitch;
	} else if (ID == 7) {
		digitalDisplay *tileDigitalDisplay = new digitalDisplay(parent,type);
		tileDigitalDisplay->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileDigitalDisplay->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileDigitalDisplay->OnMouseDown = tileMouseDown;
		tileDigitalDisplay->delButton->OnClick  = tileDeleteButtonClick;
		tileDigitalDisplay->swapButton->OnClick  = tileSwapButtonClick;
		tileDigitalDisplay->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileDigitalDisplay->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileDigitalDisplay->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileDigitalDisplay->copyButton->OnClick  = tileCopyButtonClick;
		tileDigitalDisplay->pasteButton->OnClick  = tilePasteButtonClick;
		tileDigitalDisplay->modButton->OnClick  = tileModifyButtonClick;
		tileDigitalDisplay->doneButton->OnClick = tileDoneButtonClick;
		return tileDigitalDisplay;
	} else if (ID == 8) {
		variableData *tileVariableData = new variableData(parent,type);
		tileVariableData->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileVariableData->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileVariableData->OnMouseDown = tileMouseDown;
		tileVariableData->delButton->OnClick  = tileDeleteButtonClick;
		tileVariableData->swapButton->OnClick  = tileSwapButtonClick;
		tileVariableData->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileVariableData->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileVariableData->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileVariableData->copyButton->OnClick  = tileCopyButtonClick;
		tileVariableData->pasteButton->OnClick  = tilePasteButtonClick;
		tileVariableData->modButton->OnClick  = tileModifyButtonClick;
		tileVariableData->doneButton->OnClick = tileDoneButtonClick;
		return tileVariableData;
	} else if (ID == 9) {
		stringDisplay *tileStringDisplay = new stringDisplay(parent,type);
		tileStringDisplay->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileStringDisplay->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileStringDisplay->OnMouseDown = tileMouseDown;
		tileStringDisplay->delButton->OnClick  = tileDeleteButtonClick;
		tileStringDisplay->swapButton->OnClick  = tileSwapButtonClick;
		tileStringDisplay->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileStringDisplay->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileStringDisplay->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileStringDisplay->copyButton->OnClick  = tileCopyButtonClick;
		tileStringDisplay->pasteButton->OnClick  = tilePasteButtonClick;
		tileStringDisplay->modButton->OnClick  = tileModifyButtonClick;
		tileStringDisplay->doneButton->OnClick = tileDoneButtonClick;
		return tileStringDisplay;
	} else if (ID == 10) {
		monitorCommand *tileMonitorCommand = new monitorCommand(parent,type);
		tileMonitorCommand->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileMonitorCommand->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileMonitorCommand->OnMouseDown = tileMouseDown;
		tileMonitorCommand->delButton->OnClick  = tileDeleteButtonClick;
		tileMonitorCommand->swapButton->OnClick  = tileSwapButtonClick;
		tileMonitorCommand->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileMonitorCommand->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileMonitorCommand->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileMonitorCommand->copyButton->OnClick  = tileCopyButtonClick;
		tileMonitorCommand->pasteButton->OnClick  = tilePasteButtonClick;
		tileMonitorCommand->modButton->OnClick  = tileModifyButtonClick;
		tileMonitorCommand->doneButton->OnClick = tileDoneButtonClick;
		return tileMonitorCommand;
	} else if (ID == 13) {
		palletizeTile *tilePalletize = new palletizeTile(parent,type);
		tilePalletize->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tilePalletize->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tilePalletize->OnMouseDown = tileMouseDown;
		tilePalletize->delButton->OnClick  = tileDeleteButtonClick;
		tilePalletize->swapButton->OnClick  = tileSwapButtonClick;
		tilePalletize->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tilePalletize->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tilePalletize->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tilePalletize->copyButton->OnClick  = tileCopyButtonClick;
		tilePalletize->pasteButton->OnClick  = tilePasteButtonClick;
		tilePalletize->modButton->OnClick  = tileModifyButtonClick;
		tilePalletize->doneButton->OnClick = tileDoneButtonClick;
		return tilePalletize;
	} else if (ID == 14) {
		stringData *tileStringData = new stringData(parent,type);
		tileStringData->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileStringData->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileStringData->OnMouseDown = tileMouseDown;
		tileStringData->delButton->OnClick  = tileDeleteButtonClick;
		tileStringData->swapButton->OnClick  = tileSwapButtonClick;
		tileStringData->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileStringData->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileStringData->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileStringData->copyButton->OnClick  = tileCopyButtonClick;
		tileStringData->pasteButton->OnClick  = tilePasteButtonClick;
		tileStringData->modButton->OnClick  = tileModifyButtonClick;
		tileStringData->doneButton->OnClick = tileDoneButtonClick;
		return tileStringData;
	} else if (ID == 15) {
		selector2Lamp *tileSelector2Lamp = new selector2Lamp(parent,type);
		tileSelector2Lamp->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileSelector2Lamp->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileSelector2Lamp->OnMouseDown = tileMouseDown;
		tileSelector2Lamp->delButton->OnClick  = tileDeleteButtonClick;
		tileSelector2Lamp->swapButton->OnClick  = tileSwapButtonClick;
		tileSelector2Lamp->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileSelector2Lamp->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileSelector2Lamp->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileSelector2Lamp->copyButton->OnClick  = tileCopyButtonClick;
		tileSelector2Lamp->pasteButton->OnClick  = tilePasteButtonClick;
		tileSelector2Lamp->modButton->OnClick  = tileModifyButtonClick;
		tileSelector2Lamp->doneButton->OnClick = tileDoneButtonClick;
		return tileSelector2Lamp;
	} else if (ID == 16) {
		selector3Lamp *tileSelector3Lamp = new selector3Lamp(parent,type);
		tileSelector3Lamp->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileSelector3Lamp->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileSelector3Lamp->OnMouseDown = tileMouseDown;
		tileSelector3Lamp->delButton->OnClick  = tileDeleteButtonClick;
		tileSelector3Lamp->swapButton->OnClick  = tileSwapButtonClick;
		tileSelector3Lamp->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileSelector3Lamp->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileSelector3Lamp->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileSelector3Lamp->copyButton->OnClick  = tileCopyButtonClick;
		tileSelector3Lamp->pasteButton->OnClick  = tilePasteButtonClick;
		tileSelector3Lamp->modButton->OnClick  = tileModifyButtonClick;
		tileSelector3Lamp->doneButton->OnClick = tileDoneButtonClick;
		return tileSelector3Lamp;
	} else if (ID == 21) {
		servoDispenser *tileServoDisp = new servoDispenser(parent,type);
		tileServoDisp->Parent=parent;
		//nadajemy ID MASTERA dla tego kafelka (ktory jest masterem) jako liczba losowa
		srand(time(NULL));
		tileServoDisp->setUniqueID(rand());
		//dodajemy akcje na wcisniecie na panel oraz jego opcje
		tileServoDisp->OnMouseDown = tileMouseDown;
		tileServoDisp->delButton->OnClick  = tileDeleteButtonClick;
		tileServoDisp->swapButton->OnClick  = tileSwapButtonClick;
		tileServoDisp->swapCancelButton->OnClick  = tileSwapCancelButtonClick;
		tileServoDisp->addPatternButton->OnClick  = tileAddPatternButtonClick;
		tileServoDisp->cancelActionButton->OnClick  = tileCancelActionButtonClick;
		tileServoDisp->copyButton->OnClick  = tileCopyButtonClick;
		tileServoDisp->pasteButton->OnClick  = tilePasteButtonClick;
		tileServoDisp->modButton->OnClick  = tileModifyButtonClick;
		tileServoDisp->doneButton->OnClick = tileDoneButtonClick;
		return tileServoDisp;
	}

	//jezeli tutaj dotarlismy to znaczy ze byl zly ID !!!
	return NULL;
}
//---------------------------------------------------------------------------
//sprawdzamy czy dany obiekt w jakims stopniu wywodzi sie z klasy zeroTile
//---------------------------------------------------------------------------
bool TmainWindow::checkClassZeroTile(TObject *obj)
{
	bool result;

	zeroTile *test = dynamic_cast<zeroTile*>(obj);
	(test!=NULL) ? result=true : result=false;

	return result;
}
//---------------------------------------------------------------------------
//odczytujemy typ obiektu i rzutujemy na odpowiednia klase
//---------------------------------------------------------------------------
zeroTile* TmainWindow::getTilePointer(TObject *obj)
{
	if(obj->ClassNameIs("zeroTile")){
		zeroTile* result = dynamic_cast<zeroTile*>(obj);
        return result;
	} else if(obj->ClassNameIs("pilotLamp")){
		pilotLamp* result = dynamic_cast<pilotLamp*>(obj);
		return result;
	} else if(obj->ClassNameIs("pushButton")){
		pushButton* result = dynamic_cast<pushButton*>(obj);
		return result;
	} else if(obj->ClassNameIs("pushButtonLamp")){
		pushButtonLamp* result = dynamic_cast<pushButtonLamp*>(obj);
		return result;
	} else if(obj->ClassNameIs("selector2")){
		selector2* result = dynamic_cast<selector2*>(obj);
		return result;
	} else if(obj->ClassNameIs("selector3")){
		selector3* result = dynamic_cast<selector3*>(obj);
		return result;
	} else if(obj->ClassNameIs("digitalSwitch")){
		digitalSwitch* result = dynamic_cast<digitalSwitch*>(obj);
		return result;
	} else if(obj->ClassNameIs("digitalDisplay")){
		digitalDisplay* result = dynamic_cast<digitalDisplay*>(obj);
		return result;
	} else if(obj->ClassNameIs("variableData")){
		variableData* result = dynamic_cast<variableData*>(obj);
		return result;
	} else if(obj->ClassNameIs("stringDisplay")){
		stringDisplay* result = dynamic_cast<stringDisplay*>(obj);
		return result;
	} else if(obj->ClassNameIs("monitorCommand")){
		monitorCommand* result = dynamic_cast<monitorCommand*>(obj);
		return result;
	}  else if(obj->ClassNameIs("palletizeTile")){
		palletizeTile* result = dynamic_cast<palletizeTile*>(obj);
		return result;
	} else if(obj->ClassNameIs("stringData")){
		stringData* result = dynamic_cast<stringData*>(obj);
		return result;
	} else if(obj->ClassNameIs("selector2Lamp")){
		selector2Lamp* result = dynamic_cast<selector2Lamp*>(obj);
		return result;
	} else if(obj->ClassNameIs("selector3Lamp")){
		selector3Lamp* result = dynamic_cast<selector3Lamp*>(obj);
		return result;
	} else if(obj->ClassNameIs("servoDispenser")){
		servoDispenser* result = dynamic_cast<servoDispenser*>(obj);
		return result;
	}
	return NULL;
}
//---------------------------------------------------------------------------
//metoda sprawdzajaca jak kafelek o rozmiarze > 1 wplywa na pole
// ret: -1000: 	kafelek nie moze miec rozmiaru > 1,
//		-1: 	kafelek wychodzi poza pole,
//		0,1,2: 	liczba pustych kafelkow
//---------------------------------------------------------------------------
int TmainWindow::wideTileCheckArea(zeroTile *tile)
{
	int result = -1000;
	//zanim zaczniemy - sprawdzamy czy jest to kafelek ktory moze miec rozmiar > 1
	if (tile->getID()==9) {
        result = 0;
		TFmxObject *currParent = tile->Parent;
		int currPosX = tile->Position->X, currPosY = tile->Position->Y;
		int maxSize = 3, wantedSize = tile->size;
		//sprawdzamy rozmiar kafelka + jak wplywa na istniejace kafelki
		for (int i = 1; i < maxSize; i++) {
			zeroTile *selected = findTileByPos(currParent,currPosX+i*88,currPosY);
			if (selected==NULL) {
				if (i<wantedSize) {
					tile->size = i;
					result = -1;
					break;
				}
			} else {
				//jest jakis kafelek = sprawdzamy czy jest pusty (jezeli ma go obejmowac display)
				//lub jest to pozostalosc po wiekszym rozmiarze (jezeli jest wiekszy od rozmiaru)
				int nonEmptyID = selected->getID();
				if (i<wantedSize) {
					if(nonEmptyID==0) {
						result++;
					} else if(nonEmptyID==9 && selected->uniqueID == tile->uniqueID) {
						result++;
                    }
				} else {
					//sprawdzamy pozostalosci po poprzednim kafelku
					if(nonEmptyID==9 && selected->uniqueID == tile->uniqueID) {
						//w jego miejsce tworzymy pusty
						zeroTile *emptyTile = createTile(0,regular,selected->Parent);
						emptyTile->Position->X=selected->Position->X;
						emptyTile->Position->Y=selected->Position->Y;
						//usuwamy obecny kafelek
						delete selected;
						selected = NULL;
                    }
				}
			}
		}
	}
	return result;
}
//---------------------------------------------------------------------------
//metoda czyszczaca pole przed wstawieniem kafelka o rozmiarze > 1
//---------------------------------------------------------------------------
void TmainWindow::wideTileCleanArea(zeroTile *tile, int toDoAction)
{
	//zanim zaczniemy - sprawdzamy czy jest to kafelek ktory moze miec rozmiar > 1
	if (tile->getID()==9 && toDoAction >= -1) {
		TFmxObject *currParent = tile->Parent;
		zeroTile *selected = NULL, *emptyTile = NULL;
		int currPosX = tile->Position->X, currPosY = tile->Position->Y;
		int nonEmptyID = -1, maxSize = 3, wantedSize = tile->size;
		int masterID = tile->uniqueID;
		//wykonujemy akcje na sasiednich kafelkach - sprawdzamy czy wyszlismy poza pole
		if (toDoAction == -1) {
			//jest NULL czyli nie znaleziono kafelka = wyszlismy poza zakres
			kpcMsg->show("Size to big! Fit?",MB_YESNO,MB_ICONERROR);
			if (kpcMsg->getRes()==idYes) {
				tile->size = wantedSize;
				//musimy uaktualnic kafelek glowny oraz okno z ustawieniami
				tile->getTileContent();
				tile->setTileContent();
				//zatrzymujemy podswietlanie kafelkow
				if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
			} else {
				//usuwamy obecny kafelek i ewentualne pozostalosci po string display
				for (int i = 0; i < wantedSize; i++) {
					selected = findTileByPos(currParent,currPosX+i*88,currPosY);
					if (selected) {
						nonEmptyID = selected->getID();
						//usuwamy pozostalosc po string display
						if (nonEmptyID==9) {
							if (i==0) {
								delete tile;
								tile = NULL;
							} else {
								delete selected;
								selected = NULL;
                            }
							//w jego miejsce tworzymy pusty
							emptyTile = createTile(0,regular,currParent);
							emptyTile->Position->X=currPosX+i*88;
							emptyTile->Position->Y=currPosY;
						}
					}
				}
				//zatrzymujemy podswietlanie kafelkow
				if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
				//wychodzimy z funkcji
				return;
			}
		} else if (toDoAction != wantedSize-1) {
			kpcMsg->show("Next tiles arent empty! Delete?",MB_YESNO,MB_ICONQUESTION);
			if (kpcMsg->getRes()==idYes) {
				//usuwamy sasiednie uzywane kafelki
				for (int i = 1; i < wantedSize; i++) {
					//sprawdzamy czy sasiad jest niepusty
					selected = findTileByPos(currParent,currPosX+i*88,currPosY);
					if (selected->size>1) {
						//mamy kafelek o wiekszym rozmiarze niz 1
						wideTileDelete(selected, true);
					} else {
						delete selected;
						selected = NULL;
                    }
				}
				//zatrzymujemy podswietlanie kafelkow
				if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
			} else {
				//usuwamy obecny kafelek i ewentualne pozostalosci po string display
				for (int i = 0; i < wantedSize; i++) {
					selected = findTileByPos(currParent,currPosX+i*88,currPosY);
					if (selected) {
						nonEmptyID = selected->getID();
						//usuwamy pozostalosc po string display
						if (nonEmptyID==9 && selected->uniqueID == masterID) {
							if (i==0) {
								delete tile;
								tile = NULL;
							} else {
								delete selected;
								selected = NULL;
                            }
							//w jego miejsce tworzymy pusty
							emptyTile = createTile(0,regular,currParent);
							emptyTile->Position->X=currPosX+i*88;
							emptyTile->Position->Y=currPosY;
						}
					}
				}
				//zatrzymujemy podswietlanie kafelkow
				if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
				//wychodzimy z funkcji
				return;

			}
		}
	}
}
//---------------------------------------------------------------------------
//metoda wstawiajaca kafelek o rozmiarze > 1
//---------------------------------------------------------------------------
void TmainWindow::wideTileAddToArea(zeroTile *startTile)
{
	//zanim zaczniemy - sprawdzamy czy jest to kafelek ktory moze miec rozmiar > 1
	if (startTile->getID()==9) {
		tileColors selectedColor = startTile->getBackgroundColor();
		TFmxObject *currParent = startTile->Parent;
		int currPosX = startTile->Position->X, currPosY = startTile->Position->Y;
		int wantedSize = startTile->size;
		//w obrebie zainteresowania mamy same zerowe kafelki = usuwamy je
		for (int i = 1; i < wantedSize; i++) {
			zeroTile *selected = findTileByPos(currParent,currPosX+i*88,currPosY);
			if (selected) {
				int nonEmptyID = selected->getID();
				//usuwamy obecny kafelek (tylko zerowy)
				if (nonEmptyID==0 || nonEmptyID==9) {
					delete selected;
					selected = NULL;
				}
			}
		}
		//tworzymy sasiednie kafelki (pobieramy kolor tla i obwiedni)
		startTile->getTileContent();
		for (int i = 1; i < wantedSize; i++) {
			stringDisplay *tileStringDisplay = new stringDisplay(currParent,regular,i+1);
			tileStringDisplay->Parent=currParent;
			tileStringDisplay->setUniqueID(startTile->getUniqueID());
			tileStringDisplay->Position->X=currPosX+i*88;
			tileStringDisplay->Position->Y=currPosY;
			tileStringDisplay->setTileContent();
			tileStringDisplay->bound(i+1,wantedSize);
			//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
			tileStringDisplay->setGlowColor(glowRegular,glowProtected,glowPattern);
			tileStringDisplay->setEmphasizeColor(colorEmphasize);
			//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
			tileStringDisplay->viewID(tilesID);
			tileStringDisplay->viewNO(getTileNoByPos(currParent,currPosX+i*88,currPosY),tilesNO);
		}
		if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
	}
}
//---------------------------------------------------------------------------
//metoda wstawiajaca kafelki zajmujace kilka pol
//---------------------------------------------------------------------------
void TmainWindow::wideTileCreate(zeroTile *masterTile)
{
	//zanim zaczniemy - sprawdzamy czy jest to kafelek ktory moze miec rozmiar > 1
	if (masterTile->getID()==9) {
		int emptySiblingTiles = wideTileCheckArea(masterTile);
		wideTileCleanArea(masterTile, emptySiblingTiles);
		wideTileAddToArea(masterTile);
	}
}
//---------------------------------------------------------------------------
//metoda usuwajaca kafelki zajmujace kilka pol
//---------------------------------------------------------------------------
void TmainWindow::wideTileDelete(zeroTile *masterTile, bool delMasterTile)
{
	if (masterTile->getID()==9) {
		TFmxObject *currParent = masterTile->Parent;
		zeroTile *selected = NULL, *emptyTile = NULL;
		int currPosX = masterTile->Position->X, currPosY = masterTile->Position->Y, maxSize = 3;
		int masterID = masterTile->uniqueID;
		//usuwamy obecny kafelek i ewentualne pozostalosci po string display
		for (int i = 0; i < maxSize; i++) {
			selected = findTileByPos(currParent,currPosX+i*88,currPosY);
			if (selected) {
				int nonEmptyID = selected->getID();
				//usuwamy pozostalosc po string display
				if (nonEmptyID==9 && selected->uniqueID == masterID) {
					if (i==0 && delMasterTile) {
						delete masterTile;
						masterTile = NULL;
						//w jego miejsce tworzymy pusty
						emptyTile = createTile(0,regular,currParent);
						emptyTile->Position->X=currPosX+i*88;
						emptyTile->Position->Y=currPosY;
					} else if(i>0) {
						delete selected;
						selected = NULL;
						//w jego miejsce tworzymy pusty
						emptyTile = createTile(0,regular,currParent);
						emptyTile->Position->X=currPosX+i*88;
						emptyTile->Position->Y=currPosY;
					}
					//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
					emptyTile->setGlowColor(glowRegular,glowProtected,glowPattern);
					emptyTile->setEmphasizeColor(colorEmphasize);
					//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
					emptyTile->viewID(tilesID);
					emptyTile->viewNO(getTileNoByPos(currParent,currPosX+i*88,currPosY),tilesNO);
				}
			}
		}
		//zatrzymujemy podswietlanie kafelkow
		if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
	}
}
//---------------------------------------------------------------------------
//metoda zwracajaca wskaznik na kafelek o okreslonej pozycji w danym parencie
//---------------------------------------------------------------------------
zeroTile* TmainWindow::findTileByPos(TFmxObject *tileParent, int x, int y)
{
	//liczymy ile kafelkow ma dany parent
	int currTileNo = -1, childCount = tileParent->ChildrenCount;
	//analizujemy kazde z dzieci
	for (int j = 0; j < childCount; j++) {
		//pobieramy obecne dziecko
		zeroTile *currTile = getTilePointer(tileParent->Children[j]);
		if (currTile) {
			//sprawdz czy pozycja obecnego kafelka jest zgodna z zadana
			if (currTile->Position->X == x && currTile->Position->Y == y) {
				//zwracamy kafelek ktorego pozycja sie zgadza
                return currTile;
			}
		}
	}
	//jezeli doszlismy tutaj to nie udalo sie znalezc kafelka o danej pozycji
	return NULL;
}
//---------------------------------------------------------------------------
//metoda sprawdzajaca czy kafelek o okreslonej pozycji jest pusty (zeroTile)
//---------------------------------------------------------------------------
int TmainWindow::ifTileEmpty(TFmxObject *parent, int x, int y)
{
	int result = -1;

	zeroTile *selected = findTileByPos(parent,x,y);
	if (selected) {
		//kafelek o podanych koordynatach istnieje - sprawdzamy czy jest pusty
		result = selected->ClassNameIs("zeroTile");
	} else {
		//w podanych koordynatach NIE MA KAFELKA zwroc -1
		result = -1;
	}
	return result;
}
//---------------------------------------------------------------------------
//metoda zwracajaca wskaznik na kafelek o okreslonym numerze
//---------------------------------------------------------------------------
zeroTile* TmainWindow::findTileByNo(int tileNo)
{
	TFmxObject *tileParent = dynamic_cast<TFmxObject*>(getTileParent(tileNo));
	int posX = mostLeft+tileNo%7*(80+8);
	int posY = mostTop+(tileNo%28)/7*(100+8);
	//jezeli doszlismy tutaj to nie udalo sie znalezc kafelka o danej pozycji
	return findTileByPos(tileParent,posX,posY);
}
//---------------------------------------------------------------------------
//metoda zwracajaca wskaznik na kafelek o okreslonym numerze
//---------------------------------------------------------------------------
TComponent* TmainWindow::getTileParent(int tileNo)
{
	int pageNo = tileNo/28;
	return mainWindow->FindComponent("layout"+IntToStr(pageNo+1)+"_down");
}
//---------------------------------------------------------------------------
//metoda zwracajaca numer kafelka o okreslonej pozycji
//---------------------------------------------------------------------------
int TmainWindow::getTileNoByPos(TFmxObject *parent, int x, int y)
{
	//sprawdzamy ktory to jest kafelek na danej stronie
	int colNo = 1+(x-mostLeft)/(80+8);
	int rowNo = 1+(y-mostTop)/(100+8);
	//sprawdzmamy ktora to strona
	int pageNo = StrToInt(parent->Name.SubString(7,1));
	//zwracamy wynik
	return ((rowNo-1)*7+colNo)+(pageNo-1)*28;
}
//---------------------------------------------------------------------------
//funkcja do sprawdzania czy dany obiekt ma kafelki
//---------------------------------------------------------------------------
bool TmainWindow::objectHasTiles(TFmxObject *object)
{
    bool result;
	for (int i = 0; i < object->ChildrenCount; i++) {
		if (checkClassZeroTile(object->Children[i])){
			result=true;
			//konczymy szukanie
			i = object->ChildrenCount;
		} else {
			result=objectHasTiles(object->Children[i]);
			//konczymy szukanie
			if(result==true) {
				i = object->ChildrenCount;
            }
        }
	}
	return result;
}
//---------------------------------------------------------------------------
//funkcja do sprawdzania czy dany obiekt ma kafelki i pobiera adres
//bezposredniego rodzica kafelkow
//---------------------------------------------------------------------------
bool TmainWindow::objectHasTiles(TFmxObject *object, TFmxObject &hasTilesName)
{
    bool result;
	for (int i = 0; i < object->ChildrenCount; i++) {
		if (checkClassZeroTile(object->Children[i])){
			result=true;
			hasTilesName = *object;
			//konczymy szukanie
			i = object->ChildrenCount;
		} else {
			result=objectHasTiles(object->Children[i],hasTilesName);
			//konczymy szukanie
			if(result==true) {
				i = object->ChildrenCount;
            }
        }
	}
	return result;
}
//---------------------------------------------------------------------------
//funkcja do kopiowania danego kafelka
//---------------------------------------------------------------------------
bool TmainWindow::copyTile(zeroTile *tileToCopy)
{
	//kopiujemy adres tymczasowego kafelka
	copiedTile = tileToCopy;
	//jezeli doszlismy tutaj to sie udalo :)
	return true;
}
//---------------------------------------------------------------------------
//funkcja do przenoszenia kafelka z zapamietanego wzorca na plansze
//---------------------------------------------------------------------------
bool TmainWindow::placePatternTile(zeroTile *currTile)
{
	//sprawdzamy czy zostal skopiowany kafelek
	if (copiedTile) {
		//zapamietujemy informacje o obecnym kafelku
		TFmxObject *parent = currTile->Parent;
		int x = currTile->Position->X, y = currTile->Position->Y;
		//usuwamy obecny kafelek
		delete currTile;
		//w miejsce obecnego kafelka tworzymy nowy (odpowiedniego typu)
		zeroTile *updTile = createTile(copiedTile->getID(),regular,parent);
		updTile->Position->X = x;
		updTile->Position->Y = y;
		//uaktualniamy zawartosc nowego kafelka
		updTile->setTileContent(copiedTile);
		//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
		updTile->setGlowColor(glowRegular,glowProtected,glowPattern);
		updTile->setEmphasizeColor(colorEmphasize);
		//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
		updTile->viewID(tilesID);
		updTile->viewNO(getTileNoByPos(parent,x,y),tilesNO);
		//musimy odswiezyc kafelek bo inaczej nie wyswietlaja sie zadane numery
		updTile->ApplyStyleLookup();
		//dodalismy kafelek = byla zmiana w projekcie
		changesDone = true;
		if (!lblAppTitle->Text.Pos("*")) lblAppTitle->Text = lblAppTitle->Text+"*";
		return true;
	}
	//jezeli doszlismy tutaj to cos nie poszlo :)
	return false;
}
//---------------------------------------------------------------------------
//funkcja do przenoszenia kafelka z zapamietanego wzorca na plansze
//---------------------------------------------------------------------------
bool TmainWindow::swapRegularTile(zeroTile *currTile)
{
	//sprawdzamy czy zostal zapamietany jakis kafelek
	if (copiedTile) {
		//zapamietujemy informacje o obecnym kafelku
		TFmxObject *parent = currTile->Parent;
		int x = currTile->Position->X, y = currTile->Position->Y;
		//obecny kafelek idzie w miejsce zapamietanego
		if (currTile->size > 1) wideTileDelete(currTile, false);
		currTile->Parent = copiedTile->Parent;
		currTile->Position->X = copiedTile->Position->X;
		currTile->Position->Y = copiedTile->Position->Y;
		//uaktualniamy numer kafelka obecnego
		currTile->viewNO(getTileNoByPos(currTile->Parent,currTile->Position->X,currTile->Position->Y),tilesNO);
		//zapamietany kafelek idzie w miejsce obecnego
		if (copiedTile->size > 1) wideTileDelete(copiedTile, false);
		copiedTile->Parent = parent;
		copiedTile->Position->X = x;
		copiedTile->Position->Y = y;
		//uaktualniamy numer kafelka obecnego
		copiedTile->viewNO(getTileNoByPos(copiedTile->Parent,copiedTile->Position->X,copiedTile->Position->Y),tilesNO);
		//sprawdzamy czy nowe miejsce jest OK
		if (currTile->size > 1) {
			int emptySiblingTiles = wideTileCheckArea(currTile);
			wideTileCleanArea(currTile, emptySiblingTiles);
			wideTileAddToArea(currTile);
		}
		//sprawdzamy czy nowe miejsce jest OK
		if (copiedTile->size > 1) {
			int emptySiblingTiles = wideTileCheckArea(copiedTile);
			wideTileCleanArea(copiedTile, emptySiblingTiles);
			wideTileAddToArea(copiedTile);
		}
		//zamienilismy kafelki miejscami = byla zmiana w projekcie
		changesDone = true;
		if (!lblAppTitle->Text.Pos("*")) lblAppTitle->Text = lblAppTitle->Text+"*";
		return true;
	}
	//jezeli doszlismy tutaj to cos nie poszlo :)
	return false;
}
//---------------------------------------------------------------------------
//akcja na wcisniecie myszy na kafelku
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileMouseDown(TObject *Sender, TMouseButton Button,
										TShiftState Shift, float X, float Y)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (checkClassZeroTile(Sender)) {
		zeroTile *currTile = getTilePointer(Sender);
		if(Button==TMouseButton::mbLeft){
			//sprawdzamy jaki typ kafelka to jest
			if(currTile->getType() == tileType::regular) {
				if(!optionsTile) {
					//sprawdzamy czy mamy skopiowany kafelek wzorcowy
					if (patternInsert) {
						//mamy skopiowany kafelek z patternow - musimy go wrzucic w to miejsce
						patternInsert = !placePatternTile(currTile);
						copiedTile = NULL;
						//wlaczamy mozliwosc klikniecia na cokolwiek
						onlyTilesEnabled(false);
						//przywracamy oryginalny kursor
						kpcSetCursor(crArrow);
					} else if(tileSwap) {
						//mamy zapamietany kafelek - zamieniamy go miejscami z obecnym
						tileSwap = !swapRegularTile(currTile);
						copiedTile = NULL;
						//deaktywniamy opcje klikniecia tylko na kafelki
						onlyTilesEnabled(false);
						//przywracamy oryginalny kursor
						kpcSetCursor(crArrow);
						//przesuwamy sztucznie kursor (zeby poswiata tez nie zmienila swojego pol)
						TPointF cursorPos = Fmx::Platform::Platform->GetMousePos();
						SetCursorPos(cursorPos.x+1,cursorPos.y);
					} else {
						if (Shift.Contains(ssCtrl)) {
							//mamy wcisniety CONTROL = zaznaczamy kafelki
							bool newState = !currTile->isSelected();
							currTile->selectTile(newState);
							if (newState) {
								selectedTablePushBack(currTile);
							} else {
								selectedTableUnselect(currTile);
                            }
						} else {
							//klikamy zwykly kafelek na "planszy" - kasujemy cale zaznaczenie
							int selectedTilesNo = selectedTableCapacity();
							for (int i = 0; i < selectedTilesNo; i++) {
								selectedTiles[0]->selectTile(false);
								selectedTableUnselect(selectedTiles[0]);
							}
							//odswiezamy layout (zeby zaznaczenie zniknelo)
							TControl *parent = dynamic_cast<TControl*>(currTile->Parent);
							parent->Repaint();
							//jezeli dzialaja opcle pod LMB to pokaz okno z edycja
							if (leftOptions) {
								//jezeli aktywny jest tryb sprawdzenia to nie wczytaj
								if (!robotCheckEnabled) {
									currTile->showEditor();
									//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
									currTile->setGlowColor(glowRegular,glowProtected,glowPattern);
									currTile->setEmphasizeColor(colorEmphasize);
									//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
									currTile->viewID(tilesID);
									//tylko dla kafelkow typu regular (tzn na planszy) pokazujemy numery
									if (currTile->getType() == tileType::regular) {
										int currTileNo = getTileNoByPos(currTile->Parent,currTile->Position->X,currTile->Position->Y);
										currTile->viewNO(currTileNo,tilesNO);
									}
								} else {
									kpcMsg->show("Turn off project check.",MB_OK,MB_ICONINFORMATION);
								}
							}
							optionsTile = NULL;
							if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
						}
					}
				} else {
					//pokaz ktory kafelek ma aktywny przycisk opcji
					if (tileEmphasize) optionsTile->emphasizeTile();
				}
			} else {
				//kafelek z menu wzorcow = wstawiamy na plansze (o ile jest otwarty projekt)
				if(!optionsTile) {
					if (projectActive) {
						//jezeli aktywny jest tryb sprawdzenia to nie wczytaj
						if (!robotCheckEnabled) {
							//kopiujemy do schowka wybrany kafelek
							patternInsert = copyTile(currTile);
							//wylaczamy mozliwosc klikniecia na co innego niz plansza z kafelkami
							onlyTilesEnabled(true);
							//wlaczamy odpowiedni kursor
							kpcSetCursor(crUpArrow);
							//chowamy menu
							buttonSHtilesMouseDown(Sender,Button,Shift,X,Y);
						} else {
							kpcMsg->show("Turn off project check.",MB_OK,MB_ICONINFORMATION);
                        }
					}
				} else {
					//pokaz ktory kafelek ma aktywny przycisk opcji
					if (tileEmphasize) optionsTile->emphasizeTile();
                }
			}
		} else if(Button==TMouseButton::mbRight) {
			//jezeli aktywny jest tryb sprawdzenia to nie wczytaj
			if (!robotCheckEnabled) {
				if(!optionsTile) {
					//dodajemy obecny kafelek do zaznaczenia (o ile go tam juz nie ma i jest to ragular)
					if (currTile->getType() == regular && !selectedTableElExists(currTile)) {
						selectedTablePushBack(currTile);
						currTile->selectTile(true);
					}
					//pokazujemy przyciski opcji dla danego kafelka
					currTile->menuOptions(true,tileSwap,patternInsert,selectedTableCapacity());
					//zapamietujemy adres obiektu z aktywnymi klaiwszami opcji
					optionsTile = currTile;
				} else {
					//pokaz ktory kafelek ma aktywny przycisk opcji
					if (tileEmphasize) optionsTile->emphasizeTile();
				}
			} else {
            	kpcMsg->show("Turn off project check.",MB_OK,MB_ICONINFORMATION);
            }
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku usuwania kafelka
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileDeleteButtonClick(TObject *Sender)
{
    bool deleteTile = false;
	//sprawdzamy czy wyswietlic wiadomosc o usunieciu kafelka
	if (msgOnTileDel) {
		kpcMsg->show("Delete this tile?",MB_YESNO,MB_ICONWARNING);
		if (kpcMsg->getRes()==idYes) {
			deleteTile = true;
		} else if (kpcMsg->getRes()==idNo)  {
        	deleteTile = false;
        }
	} else {
		deleteTile = true;
	}
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			//sprawdzamy czy mamy usunac kafelek
			if (deleteTile) {
				//sprawdzamy jaki to jest kafelek
				if (currTile->getType() == patternCustom) {
					//pobieramy adres parenta (powinien byæ to scrollbox)
					TFmxObject *parent = currTile->Parent;
					//sprawdzamy czy ten to on ma guziki opcji (musi miec)
					if(optionsTile == currTile) optionsTile = NULL;
					//jest to kafelek z listy wzorcow usera = kasujemy go
					delete currTile;
					//uaktualniamy pozycje kafelkow w scrollu
					int childTileNo = 0, elementsNo = parent->ChildrenCount;
					for (int i = 0; i < elementsNo; i++) {
						if (checkClassZeroTile(parent->Children[i])) {
							//mamy kolejny kafelek :)
							childTileNo++;
							zeroTile *childTile = dynamic_cast<zeroTile*>(parent->Children[i]);
							childTile->ApplyStyleLookup();
							//sprawdzamy pozycje obecnego kafelka
							if (childTile->Position->X != (childTileNo-1)*92) {
							   childTile->Position->X = (childTileNo-1)*92;
							}
						}
					}
					//na koniec updateujemy GUI (wyglad ScrollBoxa z kafelkami wzorcowymi)
					if (parent->Parent->ClassNameIs("TScrollBox")) {
						TScrollBox *scroll = dynamic_cast<TScrollBox*>(parent->Parent);
						if (elementsNo == 5) {
							//jezeli teraz jest osiem elementow to wszystkie sie mieszcza
							//wracamy scrollem na pierwszy element
							scroll->HScrollBar->Value = 0;
							//powinien zniknac scroll
							scroll->HScrollBar->Visible = false;
						} else if(elementsNo > 5) {
							//jezeli mamy wiecej niz osiem elementow to powinien nam sie
							//updateowac scroll - tzn jego szerokosc(automatycznie tego nie zrobi)
							for (int i = 0; i < scroll->ChildrenCount; i++) {
								if (scroll->Children[i]->ClassNameIs("TScrollContent")) {
									TScrollContent *temp = dynamic_cast<TScrollContent*>(scroll->Children[i]);
									temp->Realign();
								}
							}
						}
					}
					//uaktualniamy wyswietlenie liczby kafelkow
					lblPatternTilesNo->Text = IntToStr(elementsNo);
					//na koniec zatrzymujemy animacje jezeli jest uruchomiona
					if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
				} else if(currTile->getType() == regular){
					//sprawdzamy czy sa zaznaczone jakies kafelki
					int selectedTilesNo = selectedTableCapacity();
					//sprawdzamy po kolei wszystkie wybrane kafelki w tablicy
					for (int i = 0; i < selectedTilesNo; i++) {
						currTile = selectedTiles[i];
						//mamy kafelek na planszy - sprawdzamy czy jest niepusty
						if (currTile->getID()>0) {
							//sprawdzamy czy ten to on ma guziki opcji (musi miec)
							if(optionsTile == currTile) optionsTile = NULL;
							//sprawdzamy rozmiar usuwanego kafelka
							if (currTile->size>1) {
								//mamy kafelek o wiekszym rozmiarze niz 1
								wideTileDelete(currTile, true);
							} else {
								//zapamietujemy informacje o obecnym kafelku
								TFmxObject *parent = currTile->Parent;
								int x = currTile->Position->X, y = currTile->Position->Y;
								//ten kafelek usuwamy i tworzymy na jego miejscu nowy (pusty)
								delete currTile;
								zeroTile *newTile = createTile(0,regular,parent);
								newTile->Position->X = x;
								newTile->Position->Y = y;
								//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
								newTile->setGlowColor(glowRegular,glowProtected,glowPattern);
								newTile->setEmphasizeColor(colorEmphasize);
								//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
								newTile->viewID(tilesID);
								newTile->viewNO(getTileNoByPos(parent,x,y),tilesNO);
								//musimy odswiezyc kafelek bo inaczej nie wyswietlaja sie zadane numery
								newTile->UpdateStyle();
							}
							//usunelismy kafelek - byla zmiana w programie
							changesDone = true;
							if (!lblAppTitle->Text.Pos("*")) lblAppTitle->Text = lblAppTitle->Text+"*";
						} else {
							//jest to kafelek zerowy = nie usuwamy ale odznaczamy
							if(optionsTile == currTile) optionsTile = NULL;
							currTile->menuOptions(false);
                            currTile->selectTile(false);
						}
					}
					//czyscimy tablice wybranych kafelkow
					selectedTableClear();
				} else {
					kpcMsg->show("Cant delete this tile!",MB_OK,MB_ICONWARNING);
					if (kpcMsg->getRes()==idOK) {
						currTile->menuOptions(false);
						optionsTile = NULL;
					}
				}
			} else {
				//chowamy guziki opcji
				currTile->menuOptions(false);
				optionsTile = NULL;
			}
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku zamiany kafelkow miejscami
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileSwapButtonClick(TObject *Sender)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			copiedTile = currTile;
			tileSwap = true;
			//chowamy menu i zerujemy kafelek z opcjami
			currTile->menuOptions(false);
			optionsTile = NULL;
			//odznaczamy obecny kafelek (bo chcemy wkleic a nie dodawac do usuwania)
			if (selectedTableElExists(currTile)) {
				selectedTableUnselect(currTile);
				currTile->selectTile(false);
			}
			//zmieniamy kursor
			kpcSetCursor(crSizeAll);
			//uaktywniamy opcje klikniecia tylko na kafelki
			onlyTilesEnabled(true);
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku anulowania zamiany kafelkow miejscami
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileSwapCancelButtonClick(TObject *Sender)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			//chowamy menu i zerujemy kafelek z opcjami
			currTile->menuOptions(false);
			optionsTile = NULL;
			//odznaczamy obecny kafelek (bo chcemy wkleic a nie dodawac do usuwania)
			if (selectedTableElExists(currTile)) {
				selectedTableUnselect(currTile);
				currTile->selectTile(false);
			}
			//wylaczamy tryb przenoszenia kafelka
			tileSwap = false;
			copiedTile = NULL;
			//deaktywniamy opcje klikniecia tylko na kafelki
			onlyTilesEnabled(false);
			//przywracamy oryginalny kursor
			kpcSetCursor(crArrow);
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku anulowania zamiany kafelkow miejscami
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileCancelActionButtonClick(TObject *Sender)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			//chowamy menu i zerujemy kafelek z opcjami
			currTile->menuOptions(false);
			optionsTile = NULL;
			//odznaczamy obecny kafelek (bo chcemy wkleic a nie dodawac do usuwania)
			if (selectedTableElExists(currTile)) {
				selectedTableUnselect(currTile);
				currTile->selectTile(false);
			}
			//wylaczamy tryb przenoszenia kafelka
			patternInsert = false;
			copiedTile = NULL;
			//deaktywniamy opcje klikniecia tylko na kafelki
			onlyTilesEnabled(false);
			//przywracamy oryginalny kursor
			kpcSetCursor(crArrow);
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku dodanoia kafelka do wzorcow
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileAddPatternButtonClick(TObject *Sender)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			//chowamy menu i ustawiamy ze zaden kafelek nie ma aktywnych opcji
			optionsTile = NULL;
			currTile->menuOptions(false);
			//odznaczamy obecny kafelek (bo chcemy wkleic a nie dodawac do usuwania)
			if (selectedTableElExists(currTile)) {
				selectedTableUnselect(currTile);
				currTile->selectTile(false);
			}
			//sprawdzamy czy w ogole aktywna jest opcja dodawania kafelkow wzorcowych
			if (patternsAdd) {
				//tworzymy nowy zerowy element
				zeroTile *newPatternTile = createTile(currTile->getID(),patternCustom,scrollBaseTiles);
				int tempSize = currTile->size;
				//liczymy ile jest elementow podrzednych w wewnetrznym kontenerze (!)
				int elementsNo = scrollBaseTiles->Children[1]->ChildrenCount;
				newPatternTile->Position->X=(elementsNo-1)*92;
				newPatternTile->Position->Y=5;
				//tymczasowo ustawiamy rozmiar na 1 (zeby tak wrzucic do patternow)
				currTile->size = 1;
				currTile->getTileContent();
				newPatternTile->setTileContent();
				//przywracamy oryginalny rozmiar
				currTile->size = tempSize;
				//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
				newPatternTile->setGlowColor(glowRegular,glowProtected,glowPattern);
				newPatternTile->setEmphasizeColor(colorEmphasize);
				//jezeli jest to konieczne to pokazujemy ID kafelka
				newPatternTile->viewID(tilesID);
				//aktualizacja liczby kafelkow bazowych
				lblPatternTilesNo->Text = IntToStr(elementsNo);
				//ponizszy kod ze wzgledu na BUG w komponencie TScrollBox
				if (elementsNo>5) {
					TPanel *tempTile = new TPanel(scrollBaseTiles);
					tempTile->Parent=scrollBaseTiles;
					tempTile->Height = 100;
					tempTile->Width = 80;
					tempTile->Position->X=(elementsNo)*100;
					tempTile->Position->Y=5;
					//usuwamy tymczasowy element
					tempTile->Parent=NULL;
					delete tempTile;
				}
			} else {
				//info do usera ze dodawanie jest zablokowane
				kpcMsg->show("Adding patterns disabled.",MB_OK,MB_ICONWARNING);
			}
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku modyfikacji kafelka
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileModifyButtonClick(TObject *Sender)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			//sprawdzamy jaki to jest kafelek
			tileType currType = currTile->getType();
			if (currType == patternCustom || currType == regular) {
				//odznaczamy obecny kafelek (bo chcemy modyfikowac a nie dodawac do usuwania)
				if (selectedTableElExists(currTile)) {
					selectedTableUnselect(currTile);
					currTile->selectTile(false);
				}
				currTile = currTile->showEditor();
				//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
				currTile->setGlowColor(glowRegular,glowProtected,glowPattern);
				currTile->setEmphasizeColor(colorEmphasize);
				//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
				currTile->viewID(tilesID);
				//tylko dla kafelkow typu regular (tzn na planszy) pokazujemy numery
				if (currTile->getType() == tileType::regular) {
					int currTileNo = getTileNoByPos(currTile->Parent,currTile->Position->X,currTile->Position->Y);
					currTile->viewNO(currTileNo,tilesNO);
				}
				optionsTile = NULL;
				if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
			}
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku zakonczenia edycji kafelka
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileDoneButtonClick(TObject *Sender)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			//sprawdzamy czy ten to on ma guziki opcji (musi miec)
			if(optionsTile == currTile) {
				optionsTile = NULL;
				currTile->menuOptions(false);
				if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
			}
			//sprawdzamy czy byl wybrany tylko ten kafelek - jak tak to anulujemy
			if (selectedTableElExists(currTile)) {
				selectedTableUnselect(currTile);
				currTile->selectTile(false);
			}
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku kopiowania kafelka
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tileCopyButtonClick(TObject *Sender)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			//kopiujemy do schowka wybrany kafelek
			tileCopy = copyTile(currTile);
			//chowamy menu z opcjami
			if(optionsTile == currTile) {
				optionsTile = NULL;
				currTile->menuOptions(false);
				if(animEmphasizeOptionsTile->Running) animEmphasizeOptionsTile->Stop();
			}
			//usuwamy obecny kafelek z zaznaczenia (bo chcemy kopiowac a nie dodawac do usuwania)
			if (selectedTableElExists(currTile)) {
            	selectedTableUnselect(currTile);
				currTile->selectTile(false);
			}
		}
	}
}
//---------------------------------------------------------------------------
//akcja na wcisniecie przycisku wklejania kafelka
//---------------------------------------------------------------------------
void __fastcall TmainWindow::tilePasteButtonClick(TObject *Sender)
{
	//sprawdzamy czy rzeczywiscie klikniety jest element typu kafelek
	if (Sender->ClassNameIs("TButton")) {
		TButton *currButton = dynamic_cast<TButton*>(Sender);
		if (checkClassZeroTile(currButton->Parent)) {
			zeroTile *currTile = getTilePointer(currButton->Parent);
			//sprawdzamy czy jest jakis skopiowany kafelek w pamieci
			if (tileCopy) {
				//chowamy menu i usuwamy obecny kafelek
				currTile->menuOptions(false);
				//zerujemy kafelek z opcjami
				optionsTile = NULL;
				//odznaczamy obecny kafelek (bo chcemy wkleic a nie dodawac do usuwania)
				if (selectedTableElExists(currTile)) {
					selectedTableUnselect(currTile);
					currTile->selectTile(false);
				}
				//sprawdzamy czy nie chcemy wkleic ten sam kafelek
				if (copiedTile && copiedTile != currTile) {
					//zapamietujemy informacje o obecnym kafelku
					TFmxObject *parent = currTile->Parent;
					int x = currTile->Position->X, y = currTile->Position->Y;
					int currSize = copiedTile->size;
					//usuwamy obecny kafelek
					if (currTile->size>1) wideTileDelete(currTile,true);
					delete currTile;
					//w miejsce obecnego kafelka tworzymy nowy (odpowiedniego typu)
					zeroTile *updTile = createTile(copiedTile->getID(),regular,parent);
					updTile->Position->X = x;
					updTile->Position->Y = y;
					updTile->size = currSize;
					//sprawdzamy czy jest to kafelek o rozmiarze > 1
					if (updTile->size>1) {
						updTile->setTileContent(copiedTile);
						wideTileCreate(updTile);
					} else {
						//uaktualniamy zawartosc nowego kafelka
						updTile->setTileContent(copiedTile);
					}
					//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
					updTile->setGlowColor(glowRegular,glowProtected,glowPattern);
					updTile->setEmphasizeColor(colorEmphasize);
					//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
					updTile->viewID(tilesID);
					updTile->viewNO(getTileNoByPos(parent,x,y),tilesNO);
					//musimy odswiezyc kafelek bo inaczej nie wyswietlaja sie zadane numery
					updTile->ApplyStyleLookup();
					//wkelilismy kafelek = byla zmiana w projekcie
					if (!lblAppTitle->Text.Pos("*")) lblAppTitle->Text = lblAppTitle->Text+"*";
					changesDone = true;
				}
 			} else {
				//zerujemy kafelek z opcjami
				currTile->menuOptions(false);
				optionsTile = NULL;
				//wyswietlamy monit dla usera
				kpcMsg->show("No tile in clipboard...",MB_OK,MB_ICONWARNING);
				kpcMsg->getRes();
			}
		}
	}
}
//---------------------------------------------------------------------------
//uaktualnianie koloru podswietlenia wszystkich kafelkow
//---------------------------------------------------------------------------
void TmainWindow::updateTileColors(void)
{
	if (projectActive) {
		//sprawdzamy ile jest aktywnych stron
		for (int i = 0; i < tabPanelPages->TabCount; i++) {
			//dla kazdej ze stron zmieniamy kolor kazdego jej kafelka
			String pageNo = tabPanelPages->Tabs[i]->Name.SubString(tabPanelPages->Tabs[i]->Name.Length(),1);
			String tilesParent = "layout"+pageNo+"_down";
			TFmxObject *objWithTiles = dynamic_cast<TFmxObject*>(FindComponent(tilesParent));
			//sprawdzamy czy pokazujemy na jakiœ fmx object
			if (objWithTiles) {
				//ten obiekt ma w swoich dzieciach kafelki!
				for (int i = 0; i < objWithTiles->ChildrenCount; i++) {
					if (checkClassZeroTile(objWithTiles->Children[i])) {
						zeroTile *test = getTilePointer(objWithTiles->Children[i]);
						if(test){
							test->setGlowColor(glowRegular,glowProtected,glowPattern);
							test->setEmphasizeColor(colorEmphasize);
						}
					}
				}
			}
		}
	}
	//niezaleznie od stanu projektu uaktualniamy kolor kafelkow wzorcowych
	for (int i = 0; i < scrollBaseTiles->Children[1]->ChildrenCount; i++) {
		if (checkClassZeroTile(scrollBaseTiles->Children[1]->Children[i])) {
			zeroTile *test = getTilePointer(scrollBaseTiles->Children[1]->Children[i]);
			if(test){
				test->setGlowColor(glowRegular,glowProtected,glowPattern);
				test->setEmphasizeColor(colorEmphasize);
			}
		}
	}
}
//---------------------------------------------------------------------------
//obsluga (pokazywanie/chowanie) identyfikatorow i/lub numerow kafelkow
//---------------------------------------------------------------------------
void TmainWindow::updateTileInfo(bool showIDs, bool showNOs)
{
	if (projectActive) {
		//sprawdzamy ile jest aktywnych stron
		for (int i = 0; i < tabPanelPages->TabCount; i++) {
			//dla kazdej ze stron zmieniamy kolor kazdego jej kafelka
			String pageNo = tabPanelPages->Tabs[i]->Name.SubString(tabPanelPages->Tabs[i]->Name.Length(),1);
			String tilesParent = "layout"+pageNo+"_down";
			TFmxObject *objWithTiles = dynamic_cast<TFmxObject*>(FindComponent(tilesParent));
			//sprawdzamy czy pokazujemy na jakiœ fmx object
			if (objWithTiles) {
				//ten obiekt ma w swoich dzieciach kafelki!
				for (int i = 0; i < objWithTiles->ChildrenCount; i++) {
					if (checkClassZeroTile(objWithTiles->Children[i])) {
						zeroTile *test = getTilePointer(objWithTiles->Children[i]);
						if(test){
							test->viewID(showIDs);
							int currNo = getTileNoByPos(objWithTiles,test->Position->X,test->Position->Y);
							test->viewNO(currNo,showNOs);
						}
					}
				}
			}
		}
	}
	//niezaleznie od stanu projektu uaktualniamy informacje kafelkow wzorcowych
	for (int i = 0; i < scrollBaseTiles->Children[1]->ChildrenCount; i++) {
		if (checkClassZeroTile(scrollBaseTiles->Children[1]->Children[i])) {
			zeroTile *test = getTilePointer(scrollBaseTiles->Children[1]->Children[i]);
			if(test){
				test->viewID(showIDs);
			}
		}
	}
}
//---------------------------------------------------------------------------

//*********************** OBSLUGA PROJEKTOW *********************************
//---------------------------------------------------------------------------
//funkcja do usuwania kafelkow z danej strony
//---------------------------------------------------------------------------
void TmainWindow::clearTilesInPage(TFmxObject *page)
{
	try {
		String pageNo = page->Name.SubString(page->Name.Length(),1);
		String tilesParent = "layout"+pageNo+"_down";
		TFmxObject *objWithTiles = dynamic_cast<TFmxObject*>(FindComponent(tilesParent));
		//sprawdzamy czy pokazujemy na jakiœ fmx object
		if (objWithTiles) {
			//ten obiekt ma w swoich dzieciach kafelki!
			int childCount = objWithTiles->ChildrenCount;
			//musi byc co najmniej 28 dzieci (bo powinno byc 28 kafelkow!)
			if (childCount >= 28) {
				for (int i = 0; i < childCount; i++) {
					if (checkClassZeroTile(objWithTiles->Children[0])) {
						zeroTile *test = getTilePointer(objWithTiles->Children[0]);
						if(test) {
							delete test;
						}
					}
				}
				//skasowalismy kafelki na stronie - dajemy panel (maske) na front
				String maskName = "page"+pageNo+"Mask";
				TComponent *mask = FindComponent(maskName);
				if (mask) {
					TRectangle *maskRect = dynamic_cast<TRectangle*>(mask);
					maskRect->BringToFront();
				}
			}
			//nie chcemy juz na nic pokazywac
			objWithTiles = NULL;
		}

	} catch (...) {
		kpcMsg->show("Cant clear tiles !!!",MB_OK,MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
//funkcja do tworzenia pustej strony kafelkow (same kafelki zerowe!!!)
//---------------------------------------------------------------------------
void TmainWindow::createEmptyPage(TFmxObject *page, double *logPercent)
{
	try {
		int elementsPerPage = 28; 	double part = 0;
		for (int i=0; i < elementsPerPage; i++) {
			zeroTile *emptyTile = createTile(0,regular,page);
			//liczymy ile jest elementow podrzednych w wewnetrznym kontenerze (!)
			int posX = mostLeft+i%7*(emptyTile->Width+8);
			int posY = mostTop+i/7*(emptyTile->Height+8);
			emptyTile->Position->X=posX;
			emptyTile->Position->Y=posY;
			//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
			emptyTile->setGlowColor(glowRegular,glowProtected,glowPattern);
			emptyTile->setEmphasizeColor(colorEmphasize);
			//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
			emptyTile->viewID(tilesID);
			emptyTile->viewNO(getTileNoByPos(page,posX,posY),tilesNO);
			//sprawdzamy czy chcemy logowac calkowity postep
			if (logPercent) {
				part = (i*100)/(elementsPerPage*tabPanelPages->TabCount);
				lblLoadPercent->Text = FloatToStr(*logPercent+part)+"%";
				if (showSplash) lblSplashProgress->Text = "PROGRESS: "+FloatToStr(*logPercent+part)+"%";
			}
			//sprawdzamy czy jest okno powitalne - jak tak to uaktualniamy opis
			if (splashScreen->Visible) {
				lblSplashStatus->Text = "STATUS: creating empty workspace...";
			}
		}
		//pobieramy komponent zawierajacy tytul
		TLayout *currPage = dynamic_cast<TLayout*>(page);
		if (currPage) {
			String pageNoStr = currPage->Name.SubString(7,1);
			TComponent *titleComp = FindComponent("editTitlePage"+pageNoStr);
			if (titleComp && titleComp->ClassNameIs("TClearingEdit")) {
				//znaleziono komponent i jest to TClearingEdit
				TClearingEdit *editTitle = dynamic_cast<TClearingEdit*>(titleComp);
				editTitle->Text = "";
			}
        }
		//jezeli chcemy logowac postep to uaktualniamy wartosc procentowa
		if (logPercent) *logPercent += part;
	} catch (...) {
		kpcMsg->show("Cant create empty page !!!",MB_OK,MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
//funkcja do tworzenia pustego projektu (wszystkie dostepne okna sa puste)
//---------------------------------------------------------------------------
void TmainWindow::createEmptyProject(double *logPercent)
{
	try {
		//pierw sprawdzamy czy jest wlaczony jakis projekt
		if (projectActive) {
			lblLoadInfo->Text = "CLEARING WORKSPACE...";
			if (splashScreen->Visible) {
				lblSplashStatus->Text = "STATUS: clearing current workspace...";
			}
			//mamy juz jakis projekt - musimy skasowac poprzednie kafelki!
			for (int i = 0; i < tabPanelPages->TabCount; i++) {
				clearTilesInPage(tabPanelPages->Tabs[i]);
			}
			//na koniec ustawiamy flage ze nie ma aktywnego projektu
			projectActive = false;
		}
		//uzupelniamy wszystkie dostepne strony wraz z logowaniem !!!
		lblLoadInfo->Text = "CREATING WORKSPACE...";
		if (splashScreen->Visible) {
			lblSplashStatus->Text = "STATUS: creating empty workspace...";
		}
		//tworzymy strony wypelnionymi zerowymi kafelkami
		createEmptyPage(layout1_down,logPercent);
		createEmptyPage(layout2_down,logPercent);
		createEmptyPage(layout3_down,logPercent);
		createEmptyPage(layout4_down,logPercent);
		//sprawdzamy czy jest wiecej stron niz minimum
		if(tabPanelPages->TabCount>4) createEmptyPage(layout5_down,logPercent);
		if(tabPanelPages->TabCount>5) createEmptyPage(layout6_down,logPercent);
		if(tabPanelPages->TabCount>6) createEmptyPage(layout7_down,logPercent);
		if(tabPanelPages->TabCount>7) createEmptyPage(layout8_down,logPercent);
	} catch (...) {
		kpcMsg->show("Cant create new project !!!",MB_OK,MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
//funkcja do odczytywania predefiniowanych kafelkow
//---------------------------------------------------------------------------
void TmainWindow::loadTilesFromXML(TFmxObject *owner, String fileName)
{
	try {
		if (showSplash) lblSplashStatus->Text = "STATUS: loading pattern tiles...";
		TXMLDocument *xml = new TXMLDocument(this);
		IXMLNode *nodeRoot, *nodeTile;
		//sprawdzamy jest plik z ustawieniami
		if (FileExists(fileName,false)) {
			//plik istnieje odczytujemy
			xml->LoadFromFile(fileName);
			//znajdujemy wezel glowny
			nodeRoot = xml->DocumentElement;
			//odczytujemy po kolei jego dzieci
			for (int i=0; i < nodeRoot->ChildNodes->Count; i++) {
				nodeTile = nodeRoot->ChildNodes->Get(i);
				int currID = nodeTile->GetAttribute("ID");
				tileType currType = static_cast<tileType>((int)nodeTile->GetAttribute("type"));
				//tworzymy nowy obiekt
				zeroTile *newTile = createTile(currID,currType,owner);
				newTile->Position->X=(i)*92;
				newTile->Position->Y=5;
				newTile->loadFromXML(xml,i);
				//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
				newTile->setGlowColor(glowRegular,glowProtected,glowPattern);
				newTile->setEmphasizeColor(colorEmphasize);
				//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
				newTile->viewID(tilesID);
				//aktualizacja liczby kafelkow bazowych
				lblPatternTilesNo->Text = IntToStr(i+1);
			}
		}
		//kasujemy obiekt XML - juz nam nie jest potrzebny
		delete xml;
	} catch (...) {
		kpcMsg->show("Cant load pattern tiles !!!",MB_OK,MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
//funkcja do odczytywania predefiniowanych kafelkow
//---------------------------------------------------------------------------
void TmainWindow::saveTilesToXML(TFmxObject *owner, String fileName)
{
	TXMLDocument *xml = new TXMLDocument(this);
	IXMLNode *nodeRoot, *nodeTile;
	//plik nie istnieje - tworzymy nowy
	xml->Active = true;
	xml->Version = "1.0";
	xml->Encoding = "UTF-8";
	xml->AddChild("patternTiles");
	//sprawdzamy czy zapisujemy ze scrolla (defacto do XML zapisujemy PATTERNY)
	if(owner->ClassNameIs("TScrollBox")) {
		TScrollBox *parentScroll = dynamic_cast<TScrollBox*>(owner);
        //pobieramy adres kontenera z elementami
		TScrollContent *scrollCont = dynamic_cast<TScrollContent*>(parentScroll->Children[1]);
		//dla kazdego elementu w kontenerze zapisujemy kafelki
		for (int i = 0; i < scrollCont->ChildrenCount; i++) {
			if(checkClassZeroTile(scrollCont->Children[i])) {
				zeroTile *currTile = getTilePointer(scrollCont->Children[i]);
				currTile->saveToXML(xml,i);
			}
		}
		xml->SaveToFile(fileName);
    }
	//kasujemy obiekt XML - juz nam nie jest potrzebny
	delete xml;
}
//---------------------------------------------------------------------------
void TmainWindow::saveProjectToIFP(String fileName, bool forDcontroller)
{
	//zmienna typu strumien plikowy wyjsciowy
	ofstream plikIFP;
	TFmxObject *objWithTiles = new TFmxObject(NULL);
	//nadpisujemy plik
	plikIFP.open(fileName.c_str(), ios::out);
	//dajemy naglowek
	plikIFP << ".INTER_PANEL_D" << endl;
	//dla wszystkich kafelkow uzupelniamy plik
	AnsiString msg;
	//odczytujemy wszystkie aktywne strony projektu
	for (int i = 0; i < tabPanelPages->TabCount; i++) {
		if (objectHasTiles(tabPanelPages->Tabs[i],*objWithTiles)) {
			int childCount = objWithTiles->ChildrenCount, tileNo = 0;
			for (int j = 0; j < childCount; j++) {
				//ten obiekt ma w swoich dzieciach kafelki!
				if (checkClassZeroTile(objWithTiles->Children[j])) {
					int posX = mostLeft+tileNo%7*(80+8);
					int posY = mostTop+tileNo/7*(100+8);
					zeroTile *test = findTileByPos(objWithTiles,posX,posY);
					//sprawdzamy czy mamy zapisac projek ze zgodnoscia z kontrolerem typu D
					if (forDcontroller) {
						//dla kontrolera typu D:
						// - kafelki o ID wiekszym niz 10 sa nieobslugiwane
						// - maksymalnie 4 strony (4*28=112 kafelkow)
						if (test->getID()>10 || ((i*28)+tileNo)>111) {
							msg = "";
						} else {
	 						msg = test->saveToIFP(i*28+tileNo);
                        }
					} else {
						//dla kontrolera typu E nie ma ograniczen
						//odczytujemy zapis pliku z danego kafelka
						msg = test->saveToIFP(i*28+tileNo);
					}
					//zwiekszamy liczbe kafelkow
					tileNo++;
					//gdy zapis jest pusty to omijamy (zerowy kafelek)
					if (msg!="") plikIFP << msg.c_str();
				}
			}
		}
	}
	plikIFP << ".END" << endl;
	//uzupelniamy plik o tytuly danych zakladek (tylko dla kontrolera E)
	if (!forDcontroller) {
		AnsiString pageTitle; int addParam;
		plikIFP << ".INTER_PANEL_TITLE" << endl;
		for (int j = 0; j < tabPanelPages->TabCount; j++) {
			//pobieramy komponent zawierajacy tytul
			TComponent *titleComp = FindComponent("editTitlePage"+IntToStr(j+1));
			if (titleComp && titleComp->ClassNameIs("TClearingEdit")) {
				//znaleziono komponent i jest to TClearingEdit
				TClearingEdit *editTitle = dynamic_cast<TClearingEdit*>(titleComp);
				pageTitle = editTitle->Text;
				pageTitle != "" ? addParam=1 : addParam=0;
				msg = "\""+pageTitle+"\","+IntToStr(addParam);
				plikIFP << msg.c_str() << endl;
			}
		}
		plikIFP << ".END";
	}
	//zamykamy utworzony strumien
	plikIFP.close();
}
//---------------------------------------------------------------------------
void TmainWindow::loadProjectFromIFP(String fileName,double *logPercent)
{
	//sprawdzamy jest plik z ustawieniami
	if (FileExists(fileName,false)) {
		lblLoadInfo->Text = "LOADING TILES...";
		if (splashScreen->Visible) {
			lblSplashStatus->Text = "STATUS: loading tiles...";
		}
		//sprawdzamy ile zostalo do 100%
		int percentLeft = 0;
		if (logPercent) percentLeft = 100 - (*logPercent);
		//zmienna typu strumien plikowy wyjsciowy
		ifstream plikIFP; 	string msgOriginal;
		//odczytujemy plik
		plikIFP.open(fileName.c_str(), ios::binary);
		/***** sprawdzamy czy udalo sie odczytac plik ************************/
		if(!plikIFP && !plikIFP.is_open()) {
			kpcMsg->show("Cant open file: "+fileName,MB_OK,MB_ICONERROR);
			int msgResult = kpcMsg->getRes();
			lblLoadInfo->Text = "";
			return;
		}
		/***** odczytujemy linijki pliku az natrafimy na definicje panelu IF */
		do {
			plikIFP >> msgOriginal;
		} while (msgOriginal != "" && msgOriginal != ".INTER_PANEL_D");
        //uzupelniamy informacje o postepie ladowania
		if (logPercent) {
			*logPercent += (percentLeft/2)/2;
			lblLoadPercent->Text = FloatToStr(*logPercent)+"%";
			if (showSplash) lblSplashProgress->Text = "PROGRESS: "+FloatToStr(*logPercent)+"%";
        }
		/***** sprawdzamy czy byla linijka z definicja panelu IF *************/
		if (msgOriginal==".INTER_PANEL_D") {
			int commas[2], pageNo, totalPages;
			//kolejne linijki powinny zawierac w sobie definicje kafekow
			while (msgOriginal != "" && msgOriginal != ".END" && msgOriginal != ".END\r") {
				//odczytujemy kolejna linie z pliku
				getline(plikIFP,msgOriginal);
				if (isdigit(msgOriginal[0])) {
					//odczytujemy pozycje przecinkow w danej linii
					commas[0] = msgOriginal.find_first_of(',');
					commas[1] = msgOriginal.find_first_of(',',commas[0]+1);
					//odczytujemy numer kafelka i jego ID
					int tileNo = atoi(msgOriginal.substr(0,commas[0]).c_str());
					int tileID = atoi(msgOriginal.substr(commas[0]+1,commas[1]).c_str());
					//sprawdzamy na ktorej stronie powinien byc dany kafelek
					pageNo = tileNo/28;
					totalPages = tabPanelPages->TabCount-1;
					//sprawdzamy czy obeczna liczba stron jest wystarczajaca
					if (pageNo > totalPages) {
						//pokazujemy nowe strony do tej z nowym kafelkiem
						for (int i=0; i < pageNo-totalPages; i++) {
							//pokazujemy dana strone
							TTabItem *currItem = dynamic_cast<TTabItem*>(tabPanelPages->Children[pageNo-i]);
							currItem->Visible = true;
							//uzupelniamy ja (pustymi) kafelkami
							TFmxObject *currPage = dynamic_cast<TFmxObject*>(getTileParent((pageNo-i)*28));
							createEmptyPage(currPage);
							//chowamy maske nowo dodanej strony
							TComponent *testComp = mainWindow->FindComponent("page"+IntToStr(pageNo-i+1)+"Mask");
							if (testComp) {
								TRectangle *mask = dynamic_cast<TRectangle*>(testComp);
								mask->SendToBack();
							}
						}
					}
					//odczytujemy i zapamietujemy rodzica kafelka o danym numerze
					TFmxObject *tileParent = dynamic_cast<TFmxObject*>(getTileParent(tileNo));
					//kasujemy stary tile
					zeroTile *temp = findTileByNo(tileNo);
					delete temp;
					temp = NULL;
					//tworzymy nowy kafelek w miejsce starego
					zeroTile *newTile = createTile(tileID,regular,tileParent);
					newTile->Position->X=mostLeft+tileNo%7*(newTile->Width+8);
					newTile->Position->Y=mostTop+(tileNo%28)/7*(newTile->Height+8);
					newTile->loadFromIFP(msgOriginal.c_str());
					//nadajemy odpowiedni kolor podswietlenia (zgodnie z ustawieniami)
					newTile->setGlowColor(glowRegular,glowProtected,glowPattern);
					newTile->setEmphasizeColor(colorEmphasize);
					//pokazujemy odpowiednie informacje (ID i/lub numery) kafelkow
					newTile->viewID(tilesID);
					newTile->viewNO(getTileNoByPos(tileParent,newTile->Position->X,newTile->Position->Y),tilesNO);
					//sprawdzamy czy jest to kafelek o rozmiarze > 1
					if (newTile->size>1) wideTileCreate(newTile);
				}
			}
			//chowamy cala reszte zakladek
			for (int j = pageNo+1; j < tabPanelPages->ChildrenCount; j++) {
				if (tabPanelPages->Children[j]->ClassNameIs("TTabItem")) {
					TTabItem *currItem = dynamic_cast<TTabItem*>(tabPanelPages->Children[j]);
					//do czwartej strony zakladki musza byc widoczne, dalej niekoniecznie
					if (j<=3) {
						currItem->Visible = true;
						//kasujemy obecna zawartosc strony
						clearTilesInPage(currItem);
						//w nowej karcie dodajemy kafelki
						TComponent *testComp = mainWindow->FindComponent("layout"+IntToStr(j+1)+"_down");
						if (testComp) {
							TLayout *layout = dynamic_cast<TLayout*>(testComp);
							createEmptyPage(layout);
						}
					} else {
						currItem->Visible = false;
						clearTilesInPage(currItem);
					}
				}
			}
		} else {
			kpcMsg->show("Cant find project in file: "+fileName,MB_OK,MB_ICONERROR);
			int msgResult = kpcMsg->getRes();
			plikIFP.close();
			lblLoadInfo->Text = "";
			return;
		}
        //uzupelniamy informacje o postepie ladowania
		if (logPercent) {
			*logPercent += percentLeft/2;
			lblLoadPercent->Text = FloatToStr(*logPercent)+"%";
        }
		/**** odczytujemy linijki pliku az natrafimy na definicje tytulow IF lub konca pliku */
		do {
			plikIFP >> msgOriginal;
		} while (msgOriginal != "" && msgOriginal != ".INTER_PANEL_TITLE" && msgOriginal != ".END" && msgOriginal != ".END\r");
		/***** sprawdzamy czy byla linijka z definicja tytulu IF *************/
		if (msgOriginal==".INTER_PANEL_TITLE") {
			//mamy linijke z tytulami! odczytujemy po kolei
			int quotas[2], pageTitleNo=0; AnsiString currTitle;
			while (msgOriginal != "" && msgOriginal != ".END\r" && msgOriginal != ".END") {
				//odczytujemy kolejna linie z pliku
				getline(plikIFP,msgOriginal);
				//odczytujemy pozycje przecinkow w danej linii
				quotas[0] = msgOriginal.find_first_of('"');
				quotas[1] = msgOriginal.find_first_of('"',quotas[0]+1);
				//to co pomiedzy cudzyslowem jest naszym tytulem
				if (quotas[1]>quotas[0]) {
					currTitle = msgOriginal.substr(quotas[0]+1,quotas[1]-quotas[0]-1).c_str();
					//pobieramy komponent zawierajacy tytul
					TComponent *titleComp = FindComponent("editTitlePage"+IntToStr(pageTitleNo+1));
					if (titleComp && titleComp->ClassNameIs("TClearingEdit")) {
						//znaleziono komponent i jest to TClearingEdit
						TClearingEdit *editTitle = dynamic_cast<TClearingEdit*>(titleComp);
						editTitle->Text = currTitle;
					}
					pageTitleNo++;
				}
			}
		} else {
			//nie bylo definicji tytulow stron - moze to kontroler typu D
			if (splashScreen->Visible == false && msgOnNoTitles == true) {
				kpcMsg->show("Cant find page titles in file: "+projNameFromPath(fileName),MB_OK,MB_ICONINFORMATION);
				int msgResult = kpcMsg->getRes();
				plikIFP.close();
				lblLoadInfo->Text = "";
				return;
			}
		}
        //uzupelniamy informacje o postepie ladowania
		if (logPercent) {
			*logPercent += (percentLeft/2)/2;
			lblLoadPercent->Text = FloatToStr(*logPercent)+"%";
			if (showSplash) lblSplashProgress->Text = "PROGRESS: "+FloatToStr(*logPercent)+"%";
		}
		/***** wiecej nie mamy potrzeby szukac dlatego zamykamy plik *********/
		plikIFP.close();
		lblLoadInfo->Text = "";
	}
}
//---------------------------------------------------------------------------
bool TmainWindow::extractProjectFromFile(String fromFile,String toFile)
{
	bool result = false;
	//sprawdzamy czy plik zrodlowy istnieje i podano sciezke do pliku docelowego
	if (FileExists(fromFile,false) && toFile!="") {
		//odczytujemy po kolei linijki z pliku docelowego w poszukiwaniu definicji panelu
		ifstream sourceData;	ofstream destData; 	string msgOriginal;
		//odczytujemy plik
		sourceData.open(fromFile.c_str(), ios::binary);
		destData.open(toFile.c_str(), ios::binary);
		/***** sprawdzamy czy udalo sie odczytac plik ************************/
		if(!sourceData && !sourceData.is_open()) {
			return false;
		}
		/***** odczytujemy linijki pliku az natrafimy na definicje panelu IF */
		do {
			sourceData >> msgOriginal;
		} while (msgOriginal != "" && msgOriginal != ".INTER_PANEL_D");
		/***** sprawdzamy czy byla linijka z definicja panelu IF *************/
		if (msgOriginal==".INTER_PANEL_D") {
			//mamy definicje panelu - kopiujemy zawartosc z jednego pliku do drugiego
			destData << msgOriginal;
			while (msgOriginal != "" && msgOriginal != ".END\r" && msgOriginal != ".END") {
				getline(sourceData,msgOriginal);
				destData << msgOriginal << endl;  // endl = "\n"
				//stosujac powyzsza metode zapisu do pliku pojawia sie problem przy wgraniu
				//panelu do robota "missing expected character", zas stosujac ponizsze jest
				//problem przy odczytywaniu (odczytuje jedna wielka linijke projektu)
				//destData.write(msgOriginal.c_str(),msgOriginal.length());
            }
		} else {
			return false;
		}
		/**** odczytujemy linijki pliku az natrafimy na definicje tytulow IF */
		do {
			sourceData >> msgOriginal;
		} while (msgOriginal != "" && msgOriginal != ".INTER_PANEL_TITLE" && msgOriginal != ".SIG_COMMENT");
		/***** sprawdzamy czy byla linijka z definicja tytulu IF *************/
		if (msgOriginal==".INTER_PANEL_TITLE") {
			//mamy definicje tytulow - kopiujemy zawartosc z jednego pliku do drugiego
			destData << msgOriginal;
			while (msgOriginal != "" && msgOriginal != ".END\r" && msgOriginal != ".END") {
				getline(sourceData,msgOriginal);
				destData << msgOriginal << "\n";
            }
		} else {
			//jezeli nie ma tytulow paneli to albo ich brakuje albo jest to kontroler typu D/D+
			//w kazdym z tych przypadkow nic zlego sie nie dzieje
			return true;
		}
		destData << "\r";
		/***** wiecej nie mamy potrzeby szukac dlatego zamykamy plik *********/
		sourceData.close();
		destData.close();
	}
    return result;
}
//---------------------------------------------------------------------------
void TmainWindow::onlyTilesEnabled(bool condition)
{
	//w oknie glownym skanujemy wszystkie elementy podrzedne
	for (int i = 0; i < mainWindow->ChildrenCount; i++) {
		TControl *currComp = dynamic_cast<TControl*>(mainWindow->Children[i]);
		if (currComp) {
			if (currComp->Name == "mainBackground") {
				//mamy glowne okno aplikacji - tylko zakladki maja byc aktywne (zawsze)
				for (int j = 0; j < currComp->ChildrenCount; j++) {
					TControl *mainComp = dynamic_cast<TControl*>(currComp->Children[j]);
					if (mainComp) {
						if (mainComp->Name == "tabPanelPages") {
							mainComp->Enabled = true;
						} else {
							mainComp->Enabled = !condition;
						}
					}
				}
			} else {
				//nie jest to glowne tlo
				currComp->Enabled = !condition;
			}
		}
	}
}
//---------------------------------------------------------------------------
void TmainWindow::kpcSetCursor(TCursor curNo)
{
    mainWindow->Cursor = curNo;
	for (int it = 0; it < mainWindow->ChildrenCount; it++) {
		TControl *ctrl = dynamic_cast<TControl*>(mainWindow->Children[it]);
		if(ctrl) kpcSetCursor(ctrl,curNo);
	}
}
//---------------------------------------------------------------------------
void TmainWindow::kpcSetCursor(TControl* currCtrl, TCursor curNo)
{
	//dla kazdego dziecka ustawiamy odpowiedni kursor i wchodzimy do jego srodka
	for (int i = 0; i < currCtrl->ChildrenCount; i++) {
		TControl *childCtrl = dynamic_cast<TControl*>(currCtrl->Children[i]);
		//ustawiamy kursor dla wsyzstkich dzieci tego obiektu
		if (childCtrl) kpcSetCursor(childCtrl,curNo);
	}
	//ustawiamy kursor dla tego obiektu
	currCtrl->Cursor = curNo;
}
//---------------------------------------------------------------------------
//odczyt ustawien z pliku do okna glownego mainWindow
void TmainWindow::loadSettings(String fileName)
{
	//sprawdzamy czy plik w ogole istnieje
	if (FileExists(fileName,false)) {
        //plik istnieje = probujemy odczytac
		TIniFile *settingsFile = new TIniFile(fileName);

		showSplash = settingsFile->ReadBool("GUI","welcomeScreen",true);
		patternsAtFront = settingsFile->ReadBool("GUI","showPatterns",false);
		rightOptions = settingsFile->ReadBool("GUI","rightOptions",true);
		animationTime = settingsFile->ReadFloat("GUI","animSpeed",1.0);
		openAction = static_cast<action>(settingsFile->ReadInteger("GUI","startAction",0));
		tilesID = settingsFile->ReadBool("TILES","showID",false);
		tilesNO = settingsFile->ReadBool("TILES","showNo",false);
		leftOptions = settingsFile->ReadBool("TILES","leftOptions",true);
		tileEmphasize = settingsFile->ReadBool("TILES","emphasize",true);
		glowRegular = settingsFile->ReadInteger("COLORS","lightRegular",claBlue);
		glowPattern = settingsFile->ReadInteger("COLORS","lightPattern",claOrange);
		glowProtected = settingsFile->ReadInteger("COLORS","lightProtect",claGreen);
		colorEmphasize = settingsFile->ReadInteger("COLORS","emphasize",claGold);
		appPriority = static_cast<action>(settingsFile->ReadInteger("APP","priority",9));
		patternsAdd = settingsFile->ReadBool("APP","allowPattern",true);
		patternSave = static_cast<action>(settingsFile->ReadInteger("APP","savePattern",5));
		debugActive = settingsFile->ReadBool("APP","debug",false);
		checkKillDLL = settingsFile->ReadBool("APP","checkDLL",false);
		saveFileName = settingsFile->ReadString("APP","currProj","");
		msgOnExit = settingsFile->ReadBool("MESSAGE","onExit",false);
		msgOnProject = settingsFile->ReadBool("MESSAGE","onProject",true);
		msgOnTileDel = settingsFile->ReadBool("MESSAGE","onDelete",true);
		msgOnNoTitles = settingsFile->ReadBool("MESSAGE","onNoTitle",true);
		autosaveActive = settingsFile->ReadBool("AUTOSAVE","active",false);
		autosaveTime = settingsFile->ReadFloat("AUTOSAVE","interval",10);
		autosaveMethod = static_cast<action>(settingsFile->ReadInteger("AUTOSAVE","file",4));
		initDLL = settingsFile->ReadBool("ROBOT","initDLL",false);
		addToRecent = settingsFile->ReadBool("ROBOT","autoRecent",true);

		delete settingsFile;
	} else {
		//plik nie istnieje - dajemy defaultowe wartosci
		showSplash = true;
		patternsAtFront = false;
		rightOptions = true;
		animationTime = 1;
		openAction = action::nothing;
		tilesID = false;
		tilesNO = false;
		leftOptions = true;
		tileEmphasize = true;
		glowRegular = claBlue;
		glowPattern = claOrange;
		glowProtected = claGreen;
		colorEmphasize = claGold;
		appPriority = action::normalPrior;
		patternsAdd = true;
		patternSave = action::atExit;
		debugActive = false;
		checkKillDLL = false;
		msgOnExit = false;
		msgOnProject = true;
		msgOnTileDel = true;
		msgOnNoTitles = true;
		autosaveActive = false;
		autosaveTime = 10;
		autosaveMethod = action::newFile;
		initDLL = false;
		addToRecent = true;
    }
}
//---------------------------------------------------------------------------
//zapis ustawien z okna glownego mainWindow do pliku
void TmainWindow::saveSettings(String fileName)
{
	TIniFile *settingsFile = new TIniFile(fileName);

	settingsFile->WriteBool("GUI","welcomeScreen",showSplash);
	settingsFile->WriteBool("GUI","showPatterns",patternsAtFront);
	settingsFile->WriteBool("GUI","rightOptions",rightOptions);
	settingsFile->WriteFloat("GUI","animSpeed",animationTime);
	settingsFile->WriteInteger("GUI","startAction",static_cast<int>(openAction));
	settingsFile->WriteBool("TILES","showID",tilesID);
	settingsFile->WriteBool("TILES","showNo",tilesNO);
	settingsFile->WriteBool("TILES","leftOptions",leftOptions);
	settingsFile->WriteBool("TILES","emphasize",tileEmphasize);
	settingsFile->WriteInteger("COLORS","lightRegular",glowRegular);
	settingsFile->WriteInteger("COLORS","lightPattern",glowPattern);
	settingsFile->WriteInteger("COLORS","lightProtect",glowProtected);
	settingsFile->WriteInteger("COLORS","emphasize",colorEmphasize);
	settingsFile->WriteInteger("APP","priority",static_cast<int>(appPriority));
	settingsFile->WriteBool("APP","allowPattern",patternsAdd);
	settingsFile->WriteInteger("APP","savePattern",static_cast<int>(patternSave));
	settingsFile->WriteBool("APP","debug",debugActive);
	settingsFile->WriteBool("APP","checkDLL",checkKillDLL);
	settingsFile->WriteBool("MESSAGE","onExit",msgOnExit);
	settingsFile->WriteBool("MESSAGE","onProject",msgOnProject);
	settingsFile->WriteBool("MESSAGE","onDelete",msgOnTileDel);
	settingsFile->WriteBool("MESSAGE","onNoTitle",msgOnNoTitles);
	settingsFile->WriteBool("AUTOSAVE","active",autosaveActive);
	settingsFile->WriteFloat("AUTOSAVE","interval",autosaveTime);
	settingsFile->WriteInteger("AUTOSAVE","file",static_cast<int>(autosaveMethod));
	settingsFile->WriteBool("ROBOT","initDLL",initDLL);
	settingsFile->WriteBool("ROBOT","autoRecent",addToRecent);

	delete settingsFile;
}
//---------------------------------------------------------------------------
//zapis ustawien z okna glownego mainWindow do pliku
void TmainWindow::saveSettingsProjPath(String fullPath)
{
	TIniFile *settingsFile = new TIniFile(settingsFilePath);
	settingsFile->WriteString("APP","currProj",fullPath);
	delete settingsFile;
}
//---------------------------------------------------------------------------
//zastosowanie ustawien w obecnej instancji aplikacji
void TmainWindow::applySettings(void)
{
	//sprawdzamy prawy klik
	if (rightOptions) {
        buttonSHtiles->PopupMenu = tileGrabberMenu;
	} else {
		buttonSHtiles->PopupMenu = NULL;
	}
	lastSpeed = animationTime;
}
//---------------------------------------------------------------------------
TBitmap* TmainWindow::loadServoImage(int no)
{
	String resourceName;
	//w zaleznosci od zmiany zmieniamy ikone
	switch (no) {
	case 0:
		//DAMPER
		resourceName = L"servoDamper";
		break;
	case 1:
		//MOTOR
		resourceName = L"servoMotor";
		break;
	case 2:
		//GEAR PUMP
		resourceName = L"servoGearPump";
		break;
	case 3:
		//VALVE 1
		resourceName = L"servoValve1";
		break;
	case 4:
		//VALVE 2
		resourceName = L"servoValve2";
		break;
	case 5:
		//VALVE 3
		resourceName = L"servoValve3";
		break;
	case 6:
		//GUN
		resourceName = L"servoGun";
		break;
	case 7:
		//PUMP
		resourceName = L"servoPump";
		break;
	case 8:
		//PIPE 1
		resourceName = L"servoPipe1";
		break;
	case 9:
		//PIPE 2
		resourceName = L"servoPipe2";
		break;
	case 10:
		//PIPE 3
		resourceName = L"servoPipe3";
		break;
	default:
		break;
	}
	TResourceStream *resource = new TResourceStream((unsigned int)HInstance,resourceName,RT_RCDATA);
	TBitmap *result = new TBitmap(resource);
	delete resource;
	return result;
}
//---------------------------------------------------------------------------
void TmainWindow::setNoOfTabsVisible(int value)
{
	TComponent *comp = mainWindow->FindComponent("tabPanelPages");
	if (comp && comp->ClassNameIs("TTabControl")) {
		TTabControl *pageComp = dynamic_cast<TTabControl*>(comp);
		for (int i = 0; i < pageComp->ChildrenCount; i++) {
			if (pageComp->Children[i]->ClassNameIs("TTabItem")){
				TTabItem *currItem = dynamic_cast<TTabItem*>(pageComp->Children[i]);
				if (i<value) {
					//sprawdzamy czy jest aktywny projekt (nowy lub otwarty)
					if (projectActive) {
						//sprawdzamy czy dodano element (nie odjeto)
						if (i==value-1 && value>noOfPages) {
							TThreadNewPage *newP = new TThreadNewPage(true,i+1);
							newP->Priority = tpLowest;
							newP->Start();
						}
					}
					//pokazujemy zakladki do numeru pokazywanego przez spina
					currItem->Visible = true;
				} else {
					//reszte zakladek chowamy
					currItem->Visible = false;
					//przy okazji sprawdzamy czy ma on jakies dzieci
					//jak tak to sa tam najprawdopodobniej kafelki = usuwamy!
					mainWindow->clearTilesInPage(currItem);
				}
			}
		}
		noOfPages = value;
	}
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::autosaveTimerTimer(TObject *Sender)
{
	//autozapis tylko jak jest jakis projekt otwarty
	if (projectActive) {
		//sprawdzamy jaka jest wybrana opcja autozapisu
		if (autosaveMethod == newFile) {
			//tworzymy plik z suffixem "_autosave"
			String autosaveFile = saveFileName.SubString(0,saveFileName.Pos("."));
			autosaveFile += "_autosave.ifp";
			//zapisujemy projekt do tego pliku
        	saveProjectToIFP(autosaveFile);
		} else if (autosaveMethod == overwrite) {
			//nadpisujemy obecny plik
        	saveProjectToIFP(saveFileName);
		}
	}
}
//---------------------------------------------------------------------------
void TmainWindow::setAppPriority(void)
{
	//sprawdzamy jaki priorytet ma byc ustawiony
	unsigned long newPrior;
	if (appPriority == lowPrior) {
		newPrior = IDLE_PRIORITY_CLASS;
	} else if (appPriority == normalPrior) {
		newPrior = NORMAL_PRIORITY_CLASS;
	} else if (appPriority == highPrior) {
		newPrior = HIGH_PRIORITY_CLASS;
	} else if (appPriority == realTimePrior) {
		newPrior = REALTIME_PRIORITY_CLASS;
	}
	//pobieramy informacje o obecnym priorytecie aplikacji
	unsigned long currPrior = GetPriorityClass(GetCurrentProcess());
	//sprawdzamy czy nowy priorytet jest inny
	if (currPrior != newPrior) {
		SetPriorityClass(GetCurrentProcess(), newPrior);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::splashTimeTimer(TObject *Sender)
{
	mainWindow->splashScreen->Visible = false;
	mainWindow->Transparency = false;
	mainWindow->Visible = true;
	mainWindow->mainBackground->Visible = true;
	mainWindow->mainMenuTop->Visible = true;
	//zatrzymujemy zegar zeby juz do tego eventu nie wejsc
	splashTime->Enabled = false;
	SetForegroundWindow(FmxHandleToHWND(mainWindow->Handle));
}
//---------------------------------------------------------------------------
String TmainWindow::getVersionNumber(String fileName)
{
	DWORD  verHandle = NULL;
	UINT   size      = 0;
	LPBYTE lpBuffer  = NULL;
	DWORD  verSize   = GetFileVersionInfoSize(fileName.c_str(), &verHandle);
	String returnString = "", temp = "\\";

	if (verSize != NULL){
		LPSTR verData = new char[verSize];
		if (GetFileVersionInfo(fileName.c_str(), verHandle, verSize, verData)){
			if (VerQueryValue(verData,temp.c_str(),(VOID FAR* FAR*)&lpBuffer,&size)){
				if (size){
					VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
					if (verInfo->dwSignature == 0xfeef04bd){
						int major = HIWORD(verInfo->dwFileVersionMS);
						int minor = LOWORD(verInfo->dwFileVersionMS);
						int build = verInfo->dwFileVersionLS;
						String buildVerMajor = IntToStr(build).SubString(0,4);
						String buildVerMinor = IntToStr(build).SubString(4,4);
						returnString = IntToStr(major)+"."+IntToStr(minor)+"."+buildVerMajor+"."+buildVerMinor;
					}
				}
			}
		}
		delete[] verData;
	}
	return returnString;
}
//---------------------------------------------------------------------------
String TmainWindow::projNameFromPath(String fullPath)
{
	String result = fullPath;
	if (fullPath.Length()>0) {
		string test = ((AnsiString)fullPath).c_str();
		int projNameStart = test.find_last_of("\\");
		int dotPos = test.find_first_of(".");
		result = fullPath.SubString(projNameStart+2,dotPos-projNameStart-1);
	}
    return result;
}
//---------------------------------------------------------------------------
bool TmainWindow::checkDControllerCompability(String &reason)
{
	bool result=true;
	reason = "OK";
	//sprawdzamy po kolei wszystkie kafelki w projekcie pod katem kompatybilnosci z kontrolerem D
	if (projectActive) {
		//sprawdzamy czy tytuly sa puste (kontroler D nie obsluguje tytulow)
		if (checkTitlesEmpty()) {
			//sprawdzamy ile jest aktywnych stron
			for (int i = 0; i < tabPanelPages->TabCount; i++) {
				//dla kazdej ze stron sprawdzamy ID kafelka
				String pageNo = tabPanelPages->Tabs[i]->Name.SubString(tabPanelPages->Tabs[i]->Name.Length(),1);
				String tilesParent = "layout"+pageNo+"_down";
				TFmxObject *objWithTiles = dynamic_cast<TFmxObject*>(FindComponent(tilesParent));
				//sprawdzamy czy pokazujemy na jakiœ fmx object
				if (objWithTiles) {
					//ten obiekt ma w swoich dzieciach kafelki!
					for (int j = 0; j < objWithTiles->ChildrenCount; j++) {
						if (checkClassZeroTile(objWithTiles->Children[j])) {
							zeroTile *test = getTilePointer(objWithTiles->Children[j]);
							if(test && test->getID()>10){
								reason = "Unsupported tile type.";
								//jezeli jest jakis kafelek o ID > 10 to nie jest kompatybilny z D
								result = false;
								break;
							}
						}
					}
				}
				//jezeli w obiegu jednej strony byl niekompatybilny kafelek to nie ma sensu szukac dalej
				if (!result) break;
			}
		} else {
			//jakis tytul jest niepusty
			result = false;
			reason = "Non empty page titles.";
        }
	}

	return result;
}
//---------------------------------------------------------------------------
void TmainWindow::killDLLcomm(void)
{
	if (checkKillDLL) {
		String command = "TASKKILL /im AsKCTCommu.exe";
		system(AnsiString(command).c_str());
	}
}
//---------------------------------------------------------------------------
void __fastcall TmainWindow::titleChangeAttempt(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
	//zmienialismy cos w tytule = byla zmiana w projekcie
	changesDone = true;
	if (!lblAppTitle->Text.Pos("*")) lblAppTitle->Text = lblAppTitle->Text+"*";
}
//---------------------------------------------------------------------------
bool TmainWindow::checkTitlesEmpty(void)
{
	return editTitlePage1->Text.IsEmpty() && editTitlePage2->Text.IsEmpty() &&
		   editTitlePage3->Text.IsEmpty() && editTitlePage4->Text.IsEmpty() &&
		   editTitlePage5->Text.IsEmpty() && editTitlePage6->Text.IsEmpty() &&
		   editTitlePage7->Text.IsEmpty() && editTitlePage8->Text.IsEmpty();
}
//---------------------------------------------------------------------------
void TmainWindow::viewDControllerCompability(bool show)
{
	if (projectActive) {
		//sprawdzamy ile jest aktywnych stron
		for (int i = 0; i < tabPanelPages->TabCount; i++) {
			//dla kazdej ze stron sprawdzamy tytul
			TClearingEdit *pageTitle = dynamic_cast<TClearingEdit*>(FindComponent("editTitlePage"+IntToStr(i+1)));
			if (pageTitle) {
				if(show && !pageTitle->Text.IsEmpty()) {
					//zmieniamy tlo tekstu
					pageTitle->StyleLookup = "editTitleNOKStyle1";
				} else {
					pageTitle->StyleLookup = "editTitleOKStyle1";
				}
			}
			//dla kazdej ze stron sprawdzamy ID kafelka
			TFmxObject *objWithTiles = dynamic_cast<TFmxObject*>(FindComponent("layout"+IntToStr(i+1)+"_down"));
			//sprawdzamy czy pokazujemy na jakiœ fmx object
			if (objWithTiles) {
				//ten obiekt ma w swoich dzieciach kafelki!
				for (int j = 0; j < objWithTiles->ChildrenCount; j++) {
					if (checkClassZeroTile(objWithTiles->Children[j])) {
						zeroTile *test = getTilePointer(objWithTiles->Children[j]);
						if(test && (test->getID()>10 || ((i*28)+(j+1))>112)){
							test->viewIncompatibleTile(show);
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TmainWindow::selectedTableClear(void)
{
	for (int i = 0; i < 10; i++) {
        selectedTiles[i] = NULL;
	}
}
//---------------------------------------------------------------------------
bool TmainWindow::selectedTablePushBack(zeroTile *curr)
{
	bool result = false;
	for (int i = 0; i < 10; i++) {
		if (selectedTiles[i] == NULL) {
			selectedTiles[i]=curr;
			result = true;
			break;
		}
	}
	return result;
}
//---------------------------------------------------------------------------
bool TmainWindow::selectedTableUnselect(zeroTile *curr)
{
	bool unselected = false;
	for (int i = 0; i < 10; i++) {
        //sprawdzamy czy nie usunieto jeszcze kafelka
		if (!unselected) {
			//sprawdzamy czy to ten do odznaczenia
			if (selectedTiles[i] == curr) {
				selectedTiles[i] = NULL;
				unselected = true;
			}
		} else {
			//juz odznaczono kafelek - reszte przesuwamy o jedno miejsce
			if (selectedTiles[i] != NULL) {
				selectedTiles[i-1] = selectedTiles[i];
				selectedTiles[i] = NULL;
			} else {
                break;
            }
		}
	}
	return unselected;
}
//---------------------------------------------------------------------------
bool TmainWindow::selectedTableElExists(zeroTile *curr)
{
    bool result = false;
	for (int i = 0; i < 10; i++) {
		if (selectedTiles[i] == curr) {
			result = true;
			break;
		}
	}
	return result;
}
//---------------------------------------------------------------------------
int TmainWindow::selectedTableCapacity(void)
{
    int capacity = 0;
	for (int i = 0; i < 10; i++) {
		if (selectedTiles[i]) {
			capacity++;
		} else {
            break;
        }
	}
	return capacity;
}
//---------------------------------------------------------------------------

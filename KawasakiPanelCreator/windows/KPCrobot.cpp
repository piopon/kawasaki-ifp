//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "KPCrobot.h"
#include "KPC.h"
#include "KPCmessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TKPCtoRobot *KPCtoRobot;
//---------------------------------------------------------------------------
__fastcall TKPCtoRobot::TKPCtoRobot(TComponent* Owner)
	: TForm(Owner)
{
	opened = false;
}
//---------------------------------------------------------------------------
void __fastcall TKPCtoRobot::FormActivate(TObject *Sender)
{
	if (!opened) {
		//ustawiamy pozycje okna
		Left = mainWindow->Left + 1;
		Top = mainWindow->Top + mainWindow->mainMenuTop->Height + 1;
		//ustawiamy wymiary okna
		Width = mainWindow->Width - 2;
		Height = mainWindow->mainBackground->Height - 1;
		//wczytujemy elementy listy z pliku
		loadRobotFromRecent(mainWindow->recentRobotsFileName);
		//anulujemy ostatni wybor robota
		listRecentRobots->ItemIndex = -1;
		currCont = unknown;
		btnDownload->Enabled = false;
		btnUpload->Enabled = false;
		lblStatus->Text = "STATUS: select robot...";
		lblRobotName->Text = "select robot";
		lblPortNo->Text = "select robot";
		lblArmType->Text = "select robot";
		lblIP->Text = "select robot";
		lblContType->Text = "select robot";
		//ustawiamy flage ze otwarto okno
		opened = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TKPCtoRobot::btnCancelClick(TObject *Sender)
{
	this->Close();
	opened = false;
}
//---------------------------------------------------------------------------
void __fastcall TKPCtoRobot::listRecentRobotsClick(TObject *Sender)
{
	if (listRecentRobots->Items->Count > 0) {
		if (listRecentRobots->Selected) {
			int selectedItem = listRecentRobots->Selected->Index;
			if (selectedItem >= 0) {
				btnDownload->Enabled = true;
				btnUpload->Enabled = true;
				loadRobotFromRecent(mainWindow->recentRobotsFileName,selectedItem);
			}
		}
	}
}
//---------------------------------------------------------------------------
//whichNode = jezeli -1 to uaktualniamy liste, w przeciwnym wypadku info danego robota
void TKPCtoRobot::loadRobotFromRecent(String fromFile, int whichNode)
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
				lblRobotName->Text = nodeRobot->GetAttribute("name");
				lblPortNo->Text = nodeRobot->GetAttribute("port");
				lblArmType->Text = nodeRobot->GetAttribute("type");
				lblIP->Text = nodeRobot->GetAttribute("IP");
				lblContType->Text = nodeRobot->GetAttribute("cont");
				if (lblContType->Text=="D TYPE") {
					currCont = typeD;
				} else if (lblContType->Text=="E TYPE") {
					currCont = typeE;
				} else {
                    currCont = unknown;
                }
			}
		}
 	}
	//kasujemy obiekt XML - juz nam nie jest potrzebny
	delete xml;
}
//---------------------------------------------------------------------------

void __fastcall TKPCtoRobot::btnDownloadClick(TObject *Sender)
{
	bool doContinue = true, doDownload = false, deleteFile = false;
	kawasakiStatus currStatus;
	String tempFile = "", dContNokReason;
	//na poczatku sprawdzamy czy jest otwarty projekt ktory chcemy wgrac do robota
	if (mainWindow->projectActive) {
		//sprawdzamy czy projekt jest zapisany
		if (mainWindow->changesDone) {
			kpcMsg->show("Project not saved! Last saved version will be downloaded.\nContinue?",MB_YESNO,MB_ICONWARNING);
            doContinue = kpcMsg->getRes()==idYes;
		}
		//sprawdzamy czy mozemy kontynuowac z wczytaniem projektu
		if (doContinue) {
			//pierw sprawdzamy czy jest to kontroler typu D i sa kafelki o ID > 10
			if (currCont==typeD) {
				if(!mainWindow->checkDControllerCompability(dContNokReason) || mainWindow->tabPanelPages->TabCount>4) {
					//mamy kontroler D i niekompatybilny z nim projekt - dialog do usera
					int currResult,btnResults[3]={1,2,3};
					String btnLabels[3]={"CONVERT & SAVE","CONVERT & DOWNLOAD","CANCEL"};
					if (dContNokReason == "OK") dContNokReason = "Too many pages.";
					kpcMsg->showCustom("Current project is incompatible with D controller!\nReason: "+dContNokReason,
													MB_ICONWARNING,3,btnLabels,btnResults);
					currResult = kpcMsg->getRes();
					if (currResult == btnResults[0]) {
						//wybrano konwersje i zapis do pliku
						doDownload = false;
						deleteFile = false;
						mainWindow->saveProjectDialog->Filter = "KPC project files (*.ifp)|*.ifp|""All files (*.*)|*.*";
						mainWindow->saveProjectDialog->InitialDir = GetCurrentDir()+"\\project\\";
						if (mainWindow->saveProjectDialog->Execute()) {
							mainWindow->saveProjectToIFP(mainWindow->saveProjectDialog->FileName,true);
						}
					} else if (currResult == btnResults[1]) {
						//wybrano konwersje i wgranie do robota
						doDownload = true;
						deleteFile = true;
						tempFile = "C:\\temp.ifp";
						mainWindow->saveProjectToIFP(tempFile,true);
					} else if (currResult == btnResults[2]) {
						//wybrano anulowanie dalszych czynnosci
						doDownload = false;
						deleteFile = false;
						tempFile = "";
					}
				} else {
					//projekt jest kompatybilny z kontrolerem typu D
					doDownload = true;
					deleteFile = false;
					tempFile = mainWindow->saveFileName;
				}
			} else if (currCont == typeE) {
				//projekt jest kompatybilny z kontrolerem typu E (z gory wiadomo tak jest)
				doDownload = true;
				deleteFile = false;
				tempFile = mainWindow->saveFileName;
			} else if (currCont == unknown) {
				//nie wiemy jakiego typu jest kontroler - wyswietlamy okno dialogowe
				kpcMsg->show("Controler type is undefined. Continue with download?",MB_OKCANCEL,MB_ICONWARNING);
				int currResult = kpcMsg->getRes();
				if (currResult == idOK) {
					//uzytkownik na wlasne ryzyko chce wgrac projekt do kontrolera
					doDownload = true;
					deleteFile = false;
					tempFile = mainWindow->saveFileName;
				} else if (currResult == idCancel) {
					//uzytkownik nie chce wgrywac projektu do kontrolera
					doDownload = false;
					deleteFile = false;
					tempFile = "";
				}
			}
			//sprawdzamy czy mozemy wgrac projekt do robota
			if (doDownload) {
				//inicjalizujemy dllke
				if(!mainWindow->robot->isDLLinit()) {
					lblStatus->Text = "STATUS: initializing DLL...";
					mainWindow->robot->initComm();
				} else {
					lblStatus->Text = "STATUS: DLL initialized.";
				}
				//laczymy sie z robotem (tylko gdy jest zainicjalizowana dllka
				if(mainWindow->robot->isDLLinit()) {
					//sprawdzamy czy jestesmy juz podlaczeni do jakiegos robota
					if (mainWindow->robot->isConnected()) mainWindow->robot->disconnect();
					//laczymy sie do aktualnego robota
					lblStatus->Text = "STATUS: connecting to robot...";
					Application->ProcessMessages();
					mainWindow->robot->updateConnParams(lblIP->Text,StrToInt(lblPortNo->Text));
					currStatus = mainWindow->robot->connect();
					if (currStatus == ok || currStatus == alreadyConn) {
						lblStatus->Text = "STATUS: robot connected.";
					} else {
						lblStatus->Text = "STATUS: can't connect to robot.";
					}
				} else {
					lblStatus->Text = "STATUS: DLL not initialized.";
				}
				//tylko jak jestesmy polaczeni z robotem mozemy wyslac plik
				if(mainWindow->robot->isConnected()) {
					lblStatus->Text = "STATUS: robot connected. Downloading panel.";
					Application->ProcessMessages();
					//formatujemy zadany plik dla robota Kawasaki (zeby nie bylo bledu)
					String updFormatFile = "C:\\format.ifp";
					if (mainWindow->robot->formatFile(tempFile,updFormatFile)) {
						currStatus = mainWindow->robot->downloadPanelToRobot(updFormatFile);
						//sprawdzamy info od robota
						String feedback = "";
						do {
							//czekamy 50 milisekund (zgodnie z dokumentacja)
							Sleep(50);
							currStatus = mainWindow->robot->getStatus(feedback);
						} while (currStatus != ok);
						//po wszystkim wyswietlamy komunikat o powodzeniu
						lblStatus->Text = "STATUS: panel downloaded to controller!";
						//usuwamy plik sformatowany dla robota Kawasaki
						DeleteFileW(updFormatFile);
                    }
					//dodatkowo rozlaczamy sie z robotem
					mainWindow->robot->disconnect();
				}
				//usuwamy plik tymczasowy
				if (deleteFile) DeleteFileW(tempFile);
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TKPCtoRobot::btnUploadClick(TObject *Sender)
{
	kawasakiStatus currStatus;
	String tempFile = "C:\\temp", ext=".as";
	//inicjalizujemy dllke
	if(!mainWindow->robot->isDLLinit()) {
		lblStatus->Text = "STATUS: initializing DLL...";
		mainWindow->robot->initComm();
	} else {
		lblStatus->Text = "STATUS: DLL initialized!";
	}
	//laczymy sie z robotem (tylko gdy jest zainicjalizowana dllka
	if(mainWindow->robot->isDLLinit()) {
		//sprawdzamy czy jestesmy juz podlaczeni do jakiegos robota
		if (mainWindow->robot->isConnected()) mainWindow->robot->disconnect();
		//laczymy sie do aktualnego robota
		lblStatus->Text = "STATUS: connecting to robot...";
		Application->ProcessMessages();
		mainWindow->robot->updateConnParams(lblIP->Text,StrToInt(lblPortNo->Text));
		currStatus = mainWindow->robot->connect();
		if (currStatus == ok || currStatus == alreadyConn) {
			lblStatus->Text = "STATUS: robot connected!";
		} else {
			lblStatus->Text = "STATUS: cant connect to robot...";
		}
	} else {
		lblStatus->Text = "STATUS: DLL not initialized...";
	}
	//tylko jak jestesmy polaczeni z robotem mozemy wyslac plik
	if(mainWindow->robot->isConnected()) {
		lblStatus->Text = "STATUS: robot connected! Uploading panel...";
		Application->ProcessMessages();
		currStatus = mainWindow->robot->uploadPanelFromRobot(tempFile);
		if (currStatus == ok) {
			//sprawdzamy jaki jest stan robota (czy zgral plik czy jeszcze zgrywa)
			String feedback = ""; int retryCount = 0;
			do {
				//czekamy 100 milisekund (zgodnie z dokumentacja)
				Sleep(100);
				currStatus = mainWindow->robot->getStatus(feedback);
				retryCount++;
				if (retryCount % 10 == 0) {
					lblStatus->Text += ".";
					Application->ProcessMessages();
				}
			} while (currStatus != ok && retryCount < 100);
			//robot potwierdzil zgranie panelu - sprawdzamy czy plik z robota jest na dysku
			if (FileExists(tempFile+ext,false)) {
				//plik istnieje! musimy z niego pobrac interesujace nas informacje o panelu
				//zapisujemy je do pliku zdefiniowanego przez usera
				mainWindow->saveProjectDialog->Filter = "KPC project files (*.ifp)|*.ifp|""All files (*.*)|*.*";
				mainWindow->saveProjectDialog->InitialDir = GetCurrentDir()+"\\project\\";
				if (mainWindow->saveProjectDialog->Execute()) {
					String destinationFile = mainWindow->saveProjectDialog->FileName;
					//teraz z pliku zawierajace dane robota wyciagamy sam projekt panelu
					if (mainWindow->extractProjectFromFile(tempFile+ext,destinationFile)) {
						lblStatus->Text = "STATUS: panel uploaded from controller!";
					} else {
						lblStatus->Text = "STATUS: no panel definition on controller...";
					}
				} else {
					lblStatus->Text = "STATUS: uploading cancelled by user...";
				}
				//na koniec usuwamy plik tymczasowy
				DeleteFileW(tempFile+ext);
				//dodatkowo rozlaczamy sie z robotem
				mainWindow->robot->disconnect();
			} else {
				lblStatus->Text = "STATUS: no file from robot...";
			}
		} else {
            lblStatus->Text = "STATUS: cant upload panel from robot...";
        }
	}
}
//---------------------------------------------------------------------------


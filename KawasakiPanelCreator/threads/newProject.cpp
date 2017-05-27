//---------------------------------------------------------------------------
#pragma hdrstop
#include "KPC.h"
#include "newProject.h"
#include <FMX.Controls.hpp>
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TLoadAnimation::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TThreadNewProj::TThreadNewProj(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TThreadNewProj::Execute()
{
	double percent = 0;
	mainWindow->createEmptyProject(&percent);
	Synchronize(&hideLoadIndicator);
}
//---------------------------------------------------------------------------
void __fastcall TThreadNewProj::hideLoadIndicator()
{
	mainWindow->loadIndicator->Visible = false;
	mainWindow->loadIndicator->Enabled = false;
	mainWindow->page1Mask->SendToBack();
	mainWindow->page2Mask->SendToBack();
	mainWindow->page3Mask->SendToBack();
	mainWindow->page4Mask->SendToBack();
    //sprawdzamy czy jest wiecej stron niz minimum
	if(mainWindow->tabPanelPages->TabCount>4) mainWindow->page5Mask->SendToBack();
	if(mainWindow->tabPanelPages->TabCount>5) mainWindow->page6Mask->SendToBack();
	if(mainWindow->tabPanelPages->TabCount>6) mainWindow->page7Mask->SendToBack();
	if(mainWindow->tabPanelPages->TabCount>7) mainWindow->page8Mask->SendToBack();
    //uaktywniamy przycisk SAVE
	mainWindow->btnSave->Enabled = true;
	//uaktualniamy flage ze jest aktywny jakis projekt (w tym przypadku pusty)
	mainWindow->projectActive = true;
	//uaktualniamy flage ze byla zmiana = jest nowy projekt
	mainWindow->changesDone = true;
	//ustawiamy pusty plik
	mainWindow->saveFileName = "";
	mainWindow->lblAppTitle->Text = "KAWASAKI PANEL CREATOR  [empty]";
	//zamykamy splash screen (jezeli jest)
	if (mainWindow->splashScreen->Visible) {
		mainWindow->splashScreen->Visible = false;
		mainWindow->Transparency = false;
		mainWindow->Visible = true;
		mainWindow->mainBackground->Visible = true;
		mainWindow->mainMenuTop->Visible = true;
		SetForegroundWindow(FmxHandleToHWND(mainWindow->Handle));
    }
}

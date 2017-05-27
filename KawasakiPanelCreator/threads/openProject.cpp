//---------------------------------------------------------------------------
#pragma hdrstop
#include "KPC.h"
#include "openProject.h"
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
//      void __fastcall TThreadOpenProj::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TThreadOpenProj::TThreadOpenProj(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	projFileName="";
}
//---------------------------------------------------------------------------
__fastcall TThreadOpenProj::TThreadOpenProj(bool CreateSuspended, String fileN)
	: TThread(CreateSuspended)
{
	projFileName=fileN;
}
//---------------------------------------------------------------------------
void __fastcall TThreadOpenProj::Execute()
{
	//sprawdzamy czy podano nazwe pliku z projektem
	if (projFileName!="") {
		try {
			double percent = 0;
			mainWindow->createEmptyProject(&percent);
			mainWindow->loadProjectFromIFP(projFileName,&percent);
		} catch (...) {
		}
	}
	Synchronize(&hideLoadIndicator);
}
//---------------------------------------------------------------------------
void __fastcall TThreadOpenProj::hideLoadIndicator()
{
	try {
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
		//uaktualniamy flage ze nie bylo zadnych zmian
		mainWindow->changesDone = false;
		//na koniec odswiezamy GUI kart
		mainWindow->tabPanelPages->Repaint();
		//uaktualniamy nazwe pliku
		mainWindow->saveFileName = projFileName;
		String fileName = mainWindow->projNameFromPath(projFileName);
		mainWindow->lblAppTitle->Text = "KAWASAKI PANEL CREATOR  ["+fileName+"]";
		//zamykamy splash screen (jezeli jest)
		if (mainWindow->splashScreen->Visible) {
			mainWindow->splashScreen->Visible = false;
			mainWindow->Transparency = false;
			mainWindow->Visible = true;
			mainWindow->mainBackground->Visible = true;
			mainWindow->mainMenuTop->Visible = true;
			SetForegroundWindow(FmxHandleToHWND(mainWindow->Handle));
		}
	} catch (...) {
	}

}
//---------------------------------------------------------------------------

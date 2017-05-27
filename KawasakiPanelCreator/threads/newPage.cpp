//---------------------------------------------------------------------------
#pragma hdrstop
#include "KPC.h"
#include "newPage.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TThreadNewPage::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TThreadNewPage::TThreadNewPage(bool CreateSuspended, int pgNo)
	: TThread(CreateSuspended)
{
	pageNo = pgNo;
}
//---------------------------------------------------------------------------
void __fastcall TThreadNewPage::Execute()
{
	if (pageNo>0 && pageNo<=8) {
		//pokazujemy maske nowo dodanej strony
		TComponent *testComp = mainWindow->FindComponent("page"+IntToStr(pageNo)+"Mask");
		if (testComp) {
			TRectangle *mask = dynamic_cast<TRectangle*>(testComp);
			mask->BringToFront();
		}
		//w nowej karcie dodajemy kafelki
		testComp = mainWindow->FindComponent("layout"+IntToStr(pageNo)+"_down");
		if (testComp) {
			TLayout *layout = dynamic_cast<TLayout*>(testComp);
			mainWindow->createEmptyPage(layout);
		}
		//chowamy maske nowo dodanej strony
		testComp = mainWindow->FindComponent("page"+IntToStr(pageNo)+"Mask");
		if (testComp) {
			TRectangle *mask = dynamic_cast<TRectangle*>(testComp);
			mask->SendToBack();
		}
	}
}
//---------------------------------------------------------------------------

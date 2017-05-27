//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop
#include "KPC.h"
#include "KPCmessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TkpcMsg *kpcMsg;
//---------------------------------------------------------------------------
__fastcall TkpcMsg::TkpcMsg(TComponent* Owner)
	: TForm(Owner)
{
	Height = 140;
    Width = 300;
}
//---------------------------------------------------------------------------
void TkpcMsg::show(String message, int buttons, int icon)
{
    //ustalamy rozmiary okienka
	Height = 140;
    Width = 300;
	//ustalamy pozycje okienka
	this->Left = mainWindow->Left+(mainWindow->Width-Width)/2;
	this->Top = mainWindow->Top+(mainWindow->Height-Height)/2;
	//uzupelniamy ikone
	updateIcons(icon);
	//uzupelniamy tekst
	this->lblMessage->Text = message;
	//uzupelniamy ikone
	updateButtons(buttons);
	//pokazujemy okienko
	this->ShowModal();
}
//---------------------------------------------------------------------------
void TkpcMsg::showCustom(String message, int icon, int btnNo, String btnLabels[], int btnRes[])
{
	//ustalamy rozmiary okienka
	Height = 140+(btnNo-1)*30;
    Width = 300;
	//ustalamy pozycje okienka
	this->Left = mainWindow->Left+(mainWindow->Width-Width)/2;
	this->Top = mainWindow->Top+(mainWindow->Height-Height)/2;
	//uzupelniamy ikone
	updateIcons(icon);
	//uzupelniamy tekst
	this->lblMessage->Text = message;
	//chowamy wszystkie defaultowe guziki
	updateButtons(-1);
	//tworzymy nowe guziki
	for (int i = 0; i < btnNo; i++) {
		TButton *customButton = new TButton(buttonsLayout);
		customButton->Parent = buttonsLayout;
		customButton->Visible = true;
		customButton->Text = btnLabels[i];
		customButton->Height = 30;
		customButton->Width = 170;
		customButton->Position->X = (buttonsLayout->Width-customButton->Width)/2;
		customButton->Position->Y = i*33;
		customButton->Tag = btnRes[i];
		customButton->OnClick = btnCustomClick;
	}
	//pokazujemy okienko
	this->ShowModal();
}
//---------------------------------------------------------------------------
void TkpcMsg::updateIcons(int currIcon)
{
	if (currIcon==MB_ICONQUESTION) {
		iconsLayout->Visible = true;
		iconQuestion->Visible = true;
		iconError->Visible = false;
		iconWarning->Visible = false;
		iconInfo->Visible = false;
	} else if (currIcon==MB_ICONERROR) {
		iconsLayout->Visible = true;
		iconQuestion->Visible = false;
		iconError->Visible = true;
		iconWarning->Visible = false;
		iconInfo->Visible = false;
	} else if (currIcon==MB_ICONWARNING) {
		iconsLayout->Visible = true;
		iconQuestion->Visible = false;
		iconError->Visible = false;
		iconWarning->Visible = true;
		iconInfo->Visible = false;
	} else if (currIcon==MB_ICONINFORMATION) {
		iconsLayout->Visible = true;
		iconQuestion->Visible = false;
		iconError->Visible = false;
		iconWarning->Visible = false;
		iconInfo->Visible = true;
	} else {
		//nie pokazuj zadnej ikony
		iconsLayout->Visible = false;
	}
}
//---------------------------------------------------------------------------
void TkpcMsg::updateButtons(int currButtons)
{
	if (currButtons==MB_OK) {
		//uaktywniamy interesujace guziki
		btnOk->Visible = true;
		//widoczne guziki - nowe pozycje
		btnOk->Position->X = 88;		btnOk->Position->Y = 0;
		//reszta guzikow nie widoczne
		btnAbort->Visible = false;
		btnHelp->Visible = false;
		btnIgnore->Visible = false;
		btnYes->Visible = false;
		btnNo->Visible = false;
		btnCancel->Visible = false;
		btnRetry->Visible = false;
	} else if (currButtons==MB_OKCANCEL) {
		//uaktywniamy interesujace guziki
		btnOk->Visible = true;
		btnCancel->Visible = true;
		//widoczne guziki - nowe pozycje
		btnOk->Position->X = 45;		btnOk->Position->Y = 0;
		btnCancel->Position->X = 133;	btnCancel->Position->Y = 0;
		//reszta guzikow nie widoczne
		btnNo->Visible = false;
		btnAbort->Visible = false;
		btnHelp->Visible = false;
		btnIgnore->Visible = false;
		btnYes->Visible = false;
		btnRetry->Visible = false;
	} else if (currButtons==MB_ABORTRETRYIGNORE) {
		//uaktywniamy interesujace guziki
		btnAbort->Visible = true;
		btnRetry->Visible = true;
		btnIgnore->Visible = true;
		//widoczne guziki - nowe pozycje
		btnAbort->Position->X = 0;		btnAbort->Position->Y = 0;
		btnRetry->Position->X = 88;		btnRetry->Position->Y = 0;
		btnIgnore->Position->X = 176;	btnIgnore->Position->Y = 0;
		//reszta guzikow nie widoczne
		btnYes->Visible = false;
		btnNo->Visible = false;
		btnHelp->Visible = false;
		btnOk->Visible = false;
		btnCancel->Visible = false;
	} else if (currButtons==MB_YESNOCANCEL) {
		//uaktywniamy interesujace guziki
		btnYes->Visible = true;
		btnNo->Visible = true;
		btnCancel->Visible = true;
		//widoczne guziki - nowe pozycje
		btnYes->Position->X = 0;		btnYes->Position->Y = 0;
		btnNo->Position->X = 88;		btnNo->Position->Y = 0;
		btnCancel->Position->X = 176;	btnCancel->Position->Y = 0;
		//reszta guzikow nie widoczne
		btnAbort->Visible = false;
		btnHelp->Visible = false;
		btnIgnore->Visible = false;
		btnOk->Visible = false;
		btnRetry->Visible = false;
	} else if (currButtons==MB_YESNO) {
		//uaktywniamy interesujace guziki
		btnYes->Visible = true;
		btnNo->Visible = true;
		//widoczne guziki - nowe pozycje
		btnYes->Position->X = 45;		btnYes->Position->Y = 0;
		btnNo->Position->X = 133;		btnNo->Position->Y = 0;
		//reszta guzikow nie widoczne
		btnCancel->Visible = false;
		btnAbort->Visible = false;
		btnHelp->Visible = false;
		btnIgnore->Visible = false;
		btnOk->Visible = false;
		btnRetry->Visible = false;
	} else if (currButtons==MB_RETRYCANCEL) {
		//uaktywniamy interesujace guziki
		btnRetry->Visible = true;
		btnCancel->Visible = true;
		//widoczne guziki - nowe pozycje
		btnRetry->Position->X = 45;		btnRetry->Position->Y = 0;
		btnCancel->Position->X = 133;	btnCancel->Position->Y = 0;
		//reszta guzikow nie widoczne
		btnAbort->Visible = false;
		btnHelp->Visible = false;
		btnIgnore->Visible = false;
		btnOk->Visible = false;
		btnYes->Visible = false;
		btnNo->Visible = false;
	} else if (currButtons==MB_HELP) {
		//uaktywniamy interesujace guziki
		btnOk->Visible = true;
		btnHelp->Visible = true;
		//widoczne guziki - nowe pozycje
		btnOk->Position->X = 45;	btnOk->Position->Y = 0;
		btnHelp->Position->X = 133;	btnHelp->Position->Y = 0;
		//reszta guzikow nie widoczne
		btnAbort->Visible = false;
		btnIgnore->Visible = false;
		btnYes->Visible = false;
		btnNo->Visible = false;
		btnRetry->Visible = false;
		btnCancel->Visible = false;
	} else {
		btnOk->Visible = false;
		btnAbort->Visible = false;
		btnHelp->Visible = false;
		btnIgnore->Visible = false;
		btnYes->Visible = false;
		btnNo->Visible = false;
		btnCancel->Visible = false;
		btnRetry->Visible = false;
	}
}
//---------------------------------------------------------------------------
int TkpcMsg::getRes(void)
{
	return result;
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::FormActivate(TObject *Sender)
{
	result = -1;
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnYesClick(TObject *Sender)
{
	result = idYes;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnNoClick(TObject *Sender)
{
	result = idNo;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnCancelClick(TObject *Sender)
{
	result = idCancel;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnOkClick(TObject *Sender)
{
	result = idOK;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnAbortClick(TObject *Sender)
{
	result = idAbort;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnRetryClick(TObject *Sender)
{
	result = idRetry;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnHelpClick(TObject *Sender)
{
	result = idHelp;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnIgnoreClick(TObject *Sender)
{
	result = idIgnore;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TkpcMsg::btnCustomClick(TObject *Sender)
{
	TButton *currB = dynamic_cast<TButton*>(Sender);
	if (currB) {
		result = currB->Tag;
	}
	//usuwamy dynamicznie tworzone guziki
	int buttonsNo = buttonsLayout->ChildrenCount;
	//sprawdzamy czy w ogole jest wiecej guzikow niz statycznie stworzonych
	if (buttonsNo>8) {
		for (int i = 0; i < buttonsNo; i++) {
			TButton *btn = dynamic_cast<TButton*>(buttonsLayout->Children[i]);
			if (btn && btn->Tag!=0) {
				delete btn;
				//usunelismy jedno dziecko layoutu - zmniejszamy odpowiednie zmienne
				buttonsNo--;
				i--;
			}
		}
	}
	//zamykamy okno
	this->Close();
}
//---------------------------------------------------------------------------


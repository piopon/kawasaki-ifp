//---------------------------------------------------------------------------

#ifndef KPCmessageH
#define KPCmessageH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TkpcMsg : public TForm
{
__published:	// IDE-managed Components
	TButton *btnYes;
	TButton *btnNo;
	TButton *btnCancel;
	TRectangle *background;
	TLabel *lblMessage;
	TImage *iconError;
	TImage *iconQuestion;
	TLayout *iconsLayout;
	TImage *iconWarning;
	TImage *iconInfo;
	TLayout *buttonsLayout;
	TButton *btnOk;
	TButton *btnAbort;
	TButton *btnRetry;
	TButton *btnIgnore;
	TButton *btnHelp;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnYesClick(TObject *Sender);
	void __fastcall btnNoClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnAbortClick(TObject *Sender);
	void __fastcall btnRetryClick(TObject *Sender);
	void __fastcall btnHelpClick(TObject *Sender);
	void __fastcall btnIgnoreClick(TObject *Sender);
private:	// User declarations
	int result;
	void updateIcons(int currIcon);
	void updateButtons(int currButtons);
	void __fastcall btnCustomClick(TObject *Sender);
public:		// User declarations
	__fastcall TkpcMsg(TComponent* Owner);
	void show(String message, int buttons, int icon);
	void showCustom(String message, int icon, int btnNo, String btnLabels[], int btnRes[]);
	int getRes(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TkpcMsg *kpcMsg;
//---------------------------------------------------------------------------
#endif

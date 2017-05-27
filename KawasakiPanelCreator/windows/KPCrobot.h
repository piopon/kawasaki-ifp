//---------------------------------------------------------------------------

#ifndef KPCrobotH
#define KPCrobotH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.TreeView.hpp>
//---------------------------------------------------------------------------
enum controller{
	unknown = 0,
	typeD = 1,
	typeE = 2
};
//---------------------------------------------------------------------------
class TKPCtoRobot : public TForm
{
__published:	// IDE-managed Components
	TListBox *listRecentRobots;
	TLabel *lblTitleSelectRobot;
	TPanel *mainBackground;
	TStyleBook *StyleBook1;
	TLayout *areaBottom;
	TLayout *areaLeft;
	TButton *btnDownload;
	TButton *btnCancel;
	TLayout *areaRight;
	TLabel *lblTitleRobotInfo;
	TButton *btnUpload;
	TLabel *lblTitleArmType;
	TLabel *lblTitleContType;
	TLabel *lblTitleIP;
	TLabel *lblTitlePortNo;
	TLabel *lblTitleRobotName;
	TLabel *lblRobotName;
	TLabel *lblArmType;
	TLabel *lblContType;
	TLabel *lblIP;
	TLabel *lblPortNo;
	TPanel *panelStatus;
	TLabel *lblStatus;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall listRecentRobotsClick(TObject *Sender);
	void __fastcall btnDownloadClick(TObject *Sender);
	void __fastcall btnUploadClick(TObject *Sender);
private:	// User declarations
	void loadRobotFromRecent(String fromFile, int whichNode=-1);
	bool opened;
	controller currCont;
public:		// User declarations
	__fastcall TKPCtoRobot(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TKPCtoRobot *KPCtoRobot;
//---------------------------------------------------------------------------
#endif

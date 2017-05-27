//---------------------------------------------------------------------------
#ifndef KPCoptionsH
#define KPCoptionsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Menus.hpp>
#include <FMX.ExtCtrls.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Colors.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
#include <Xml.Win.msxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
//---------------------------------------------------------------------------
class TkpcOpt : public TForm
{
__published:	// IDE-managed Components
	TPanel *panelButtons;
	TButton *btnOK;
	TTabControl *tabsOptions;
	TTabItem *optGeneral;
	TStyleBook *StyleBook1;
	TButton *btnOptGeneral;
	TSpinBox *pageNoSpin;
	TLabel *lblPageNo;
	TRadioButton *radioStartupLast;
	TRadioButton *radioStartupEmpty;
	TRadioButton *radioStartupNothing;
	TGroupBox *groupAfterStart;
	TTabItem *optAbout;
	TButton *btnAbout;
	TGroupBox *groupAnimTime;
	TRadioButton *radioAnimOff;
	TRadioButton *radioAnimQuick;
	TRadioButton *radioAnimSlow;
	TCheckBox *checkShowSplash;
	TGroupBox *groupGUIGeneral;
	TGroupBox *groupMain1_Tiles;
	TCheckBox *checkShowID;
	TColorComboBox *colorGlowRegular;
	TGroupBox *groupMain0_GUI;
	TGroupBox *groupMain2_App;
	TTabItem *optRobot;
	TButton *btnRobot;
	TLabel *lblGlowRegular;
	TGroupBox *groupHighlights;
	TLabel *lblGlowPattern;
	TLabel *lblGlowProtected;
	TCheckBox *checkEmphasizeActive;
	TColorComboBox *colorGlowPattern;
	TColorComboBox *colorGlowProtected;
	TColorComboBox *colorEmphasizeOptTile;
	TGroupBox *groupTileGeneral;
	TCheckBox *checkShowNumbers;
	TLine *lineSeparator;
	TGroupBox *groupAppAutosave;
	TCheckBox *checkAutosaveActive;
	TEdit *editAutosaveInterval;
	TLabel *lblAutosaveInterval;
	TRadioButton *radioAutosaveCurrFile;
	TRadioButton *radioAutosaveNewF;
	TGroupBox *groupAppGeneral;
	TComboBox *comboAppPriority;
	TListBoxItem *cItemPriorityLow;
	TListBoxItem *cItemPriorityNormal;
	TListBoxItem *cItemPriorityHigh;
	TListBoxItem *cItemPriorityRealTime;
	TLabel *lblAppPriority;
	TCheckBox *checkShowExpanded;
	TCheckBox *checkOptLocally;
	TLabel *lblPatternTilesAction;
	TComboBox *comboPatternTilesAction;
	TListBoxItem *cItemPatternNever;
	TListBoxItem *cItemPatternExit;
	TListBoxItem *cItemPatternOnSave;
	TCheckBox *checkAppAllowNewPattern;
	TCheckBox *checkTileOptOnLeftClick;
	TGroupBox *groupMessage;
	TLine *Line1;
	TCheckBox *checkMsgOnExit;
	TCheckBox *checkMsgProjNotSaved;
	TCheckBox *checkMsgTileDelete;
	TGroupBox *groupMain0_Conn;
	TGroupBox *groupMain1_Recent;
	TCheckBox *checkAddToRecent;
	TGroupBox *groupMain2_Info;
	TGroupBox *groupMain0_About;
	TGroupBox *groupMain1_Debug;
	TButton *btnUpdateChildCount;
	TButton *btnDLL_init;
	TButton *btnDLL_getVer;
	TButton *btnDLL_destroy;
	TLine *lineSeparator2;
	TCheckBox *checkDLL_initAtStart;
	TGroupBox *groupSettings;
	TLayout *layoutIPaddress;
	TLabel *lbIPtitle;
	TEdit *editIPaddres1;
	TEdit *editIPaddres2;
	TEdit *editIPaddres4;
	TEdit *editIPaddres3;
	TLabel *lblIPdot1;
	TLabel *lblIPdot3;
	TLabel *lblIPdot2;
	TEdit *editPortNo;
	TLabel *lblPortNo;
	TGroupBox *groupAction;
	TButton *btnConnect;
	TButton *btnDisconnect;
	TLabel *lblStatus;
	TRectangle *rectStatus;
	TGroupBox *groupRecentList;
	TGroupBox *groupRecentAction;
	TButton *btnAddRobot;
	TButton *btnRemoveRobot;
	TListBox *listRecentRobots;
	TClearingEdit *editRobotName;
	TLabel *lblRobotName;
	TButton *btnClearConnSettings;
	TButton *btnUpdateRobot;
	TLabel *lblTitleUserIfAS;
	TGroupBox *groupRobotAS;
	TLabel *lblTitleUserIfTP;
	TLabel *lblTitleArmAs;
	TLabel *lblTitleArmData;
	TLabel *lblTitleASMsg;
	TLabel *lblASMsg;
	TLabel *lblArmData;
	TLabel *lblArmAs;
	TLabel *lblUserIfTP;
	TLabel *lblUserIfAS;
	TGroupBox *groupServo;
	TLabel *lblTitleControlServo;
	TLabel *lblControlServo;
	TLabel *lblTitleDataFile;
	TLabel *lblDataFile;
	TLabel *lblTitleRobotArm;
	TLabel *lblRobotArm;
	TImage *imageAbout;
	TLabel *lblAboutAppTitle;
	TLabel *lblVersion;
	TButton *btnUpdRobotInfo;
	TLayout *Layout1;
	TLabel *lblAboutAuthor;
	TLabel *lblTitleContType;
	TLabel *lblContType;
	TButton *btnTest_extractProj;
	TImage *e_contIcon;
	TImage *d_contIcon;
	TImage *dP_contIcon;
	TLabel *lblContCompaility;
	TLine *lineSeparatorGeneral;
	TCheckBox *checkMsgOnNoTitle;
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnOptClick(TObject *Sender);
	void __fastcall pageNoSpinChange(TObject *Sender);
	void __fastcall btnUpdateChildCountClick(TObject *Sender);
	void __fastcall btnDLL_initClick(TObject *Sender);
	void __fastcall btnDLL_getVerClick(TObject *Sender);
	void __fastcall btnDLL_destroyClick(TObject *Sender);
	void __fastcall btnConnectClick(TObject *Sender);
	void __fastcall btnDisconnectClick(TObject *Sender);
	void __fastcall btnAddRobotClick(TObject *Sender);
	void __fastcall btnRemoveRobotClick(TObject *Sender);
	void __fastcall listRecentRobotsClick(TObject *Sender);
	void __fastcall btnClearConnSettingsClick(TObject *Sender);
	void __fastcall btnUpdateRobotClick(TObject *Sender);
	void __fastcall btnUpdRobotInfoClick(TObject *Sender);
	void __fastcall btnTest_extractProjClick(TObject *Sender);
private:	// User declarations
	//pobranie i ustawienie ustawien
	void getSettings(void);
	void applySettings(void);
	bool checkSettings(void);
	//zmienne
	bool opened;
	void updatePosition(void);
	void updateView(void);
	void updateTabs(void);
	//dodanie obecnego robota do historii
	void fillRobotInfo(String fromFile);
	void addCurrRobotToRecent(String currIP, String currName, bool showFeedback=true);
	bool checkIfRecentRobot(String ip, String name, int &whichNode);
	void saveRobotToRecent(String toFile, int whichNode);
	void loadRobotFromRecent(String fromFile, int whichNode=-1);
	void deleteRobotFromRecent(String fromFile, int whichNode=-1);
public:		// User declarations
	__fastcall TkpcOpt(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TkpcOpt *kpcOpt;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef tilePropertiesH
#define tilePropertiesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Colors.hpp>
#include <FMX.Objects.hpp>
//---------------------------------------------------------------------------
#include "KPC.h"
#include "zeroTile.h"
#include "pushButton.h"
//---------------------------------------------------------------------------
enum position {
	up = 0,
	bottom,
	left,
	right,
	vertical,
	horizontal
};
//---------------------------------------------------------------------------
enum specialView {
	none = 0,
	palletize,
	stringWindow,
	monitorCmd,
	servo,
};
//---------------------------------------------------------------------------
class componentPos
{
	public:
		int x;
		int y;
		bool visible;
};
//---------------------------------------------------------------------------
class zeroTile;
class pushButton;

class TtilePropertiesWindow : public TForm
{
__published:	// IDE-managed Components
	TButton *buttonAccept;
	TComboBox *comboTileType;
	TListBoxItem *cItemNone;
	TListBoxItem *cItemPushButton;
	TButton *buttonCancel;
	TClearingEdit *editLabel1;
	TLayout *layoutTileType;
	TLabel *titleTileType;
	TLayout *layoutLabels12;
	TLabel *titleLabel1;
	TLabel *titleLabel2;
	TClearingEdit *editLabel2;
	TLayout *layoutButtons;
	TLayout *layoutLabels34;
	TColorButton *colorButtonBackg_altOff;
	TPanel *panelColorSelect;
	TRectangle *rectLightGray;
	TRectangle *rectBlue;
	TRectangle *rectRed;
	TRectangle *rectOrange;
	TRectangle *rectLime;
	TRectangle *rectLightBlue;
	TRectangle *rectYellow;
	TRectangle *rectWhite1;
	TRectangle *rectBlack;
	TRectangle *rectCyan;
	TRectangle *rectRossyBrown;
	TRectangle *rectNavy;
	TRectangle *rectGreen;
	TRectangle *rectLavender;
	TLayout *layoutColor1;
	TLayout *layoutColor2;
	TLayout *layoutColor4;
	TLayout *layoutColor3;
	TRectangle *rectPink;
	TRectangle *rectWhite2;
	TLayout *layoutColors;
	TLabel *titleBackgroundON;
	TColorButton *colorButtonLabel;
	TLabel *titleLabel3;
	TLabel *titleLabel4;
	TClearingEdit *editLabel3;
	TClearingEdit *editLabel4;
	TColorButton *colorButtonBackgOn;
	TGroupBox *groupColorBackg;
	TLabel *titleBackgroundOFF;
	TGroupBox *groupColorLabel;
	TGroupBox *groupColorSwitch;
	TGroupBox *groupColorLamp;
	TColorButton *colorButtonLampOn;
	TColorButton *colorButtonLampOff;
	TColorButton *colorButtonSwitchL;
	TColorButton *colorButtonSwitchM;
	TColorButton *colorButtonSwitchR;
	TLabel *titleLampON;
	TLabel *titleLampOFF;
	TLabel *titleSwitchR;
	TLabel *titleSwitchM;
	TLabel *titleSwitchL;
	TLayout *layoutSignals;
	TClearingEdit *editButtonSignal_altTop;
	TLabel *titleButtonSignal_altTop;
	TClearingEdit *editLampSignal;
	TClearingEdit *editSwitchSignalL_altUp;
	TLabel *titleLampSignal;
	TLabel *titleSwitchSignalL_altUp;
	TLabel *titleSwitchSignalM;
	TLabel *titleSwitchSignalR_altDown;
	TClearingEdit *editSwitchSignalM;
	TClearingEdit *editSwitchSignalR_altDown;
	TSpinBox *spinBoxFigures;
	TLabel *titleSpinFigures;
	TLabel *titleSpinSigNo;
	TSpinBox *spinBoxSigNo;
	TRadioButton *sigTypeBCD;
	TRadioButton *sigTypeBin;
	TGroupBox *groupSignalType;
	TCheckBox *checkBoxMinusVal;
	TGroupBox *groupDataLimit;
	TCheckBox *checkBoxDataLimit;
	TClearingEdit *editDataLimUpper;
	TClearingEdit *editDataLimLower;
	TLabel *titleDataLimUpper;
	TLabel *titleDataLimLower;
	TGroupBox *groupInputType;
	TRadioButton *inpTypeKeys;
	TRadioButton *inpTypeSwitch;
	TLayout *layoutVariables;
	TCheckBox *checkBoxExclMark;
	TCheckBox *checkBoxOperationCond;
	TGroupBox *groupDispType;
	TRadioButton *dispTypeUpDown;
	TRadioButton *dispTypeRotation;
	TGroupBox *groupDataType;
	TRadioButton *dataTypeINT;
	TRadioButton *dataTypeREAL;
	TClearingEdit *editVariableName;
	TLabel *titleVariableName;
	TLayout *layoutPalletize;
	TCheckBox *checkBoxFrame;
	TSpinBox *spinBoxWorkNo;
	TSpinBox *spinBoxPatternNo;
	TLabel *titleWorkNo;
	TLabel *titlePatternNo;
	TLayout *layoutCmdString;
	TClearingEdit *editCommandString;
	TLabel *titleCommandString;
	TSpinBox *spinBoxCommandStrNo;
	TLabel *titleCommandStrNo;
	TLabel *titleWinNr;
	TLabel *titleWinSize;
	TSpinBox *spinBoxWinNr;
	TSpinBox *spinBoxWinSize;
	TListBoxItem *cItemPilotLamp;
	TListBoxItem *cItemPushButtonLamp;
	TListBoxItem *cItem2Selector;
	TListBoxItem *cItem3Selector;
	TListBoxItem *cItemDigitalSwitch;
	TListBoxItem *cItemDigitalDisplay;
	TListBoxItem *cItemVarDataDisplay;
	TListBoxItem *cItemStringDisplayWin;
	TListBoxItem *cItemMonitorCommand;
	TListBoxItem *cItemPalletize;
	TListBoxItem *cItemStringDataDisplay;
	TListBoxItem *cItem2SelectorWLamp;
	TListBoxItem *cItem3SelectorWLamp;
	TListBoxItem *cItemServoDisp;
	TLayout *layoutSignalFeatures;
	TLayout *layoutStrWindow;
	TLayout *layoutServo;
	TComboBox *comboServoPart;
	TLabel *titleServoPart;
	TListBoxItem *pIDamper;
	TListBoxItem *pIMotor;
	TListBoxItem *pIGearPump;
	TListBoxItem *pIValve1;
	TListBoxItem *pIValve2;
	TListBoxItem *pIValve3;
	TListBoxItem *pIGun;
	TListBoxItem *pIPump;
	TListBoxItem *pIPipe1;
	TListBoxItem *pIPipe2;
	TListBoxItem *pIPipe3;
	TImage *imageServoPart;
	TImage *d_Icon;
	TImage *dP_Icon;
	TImage *e_Icon;
	TLayout *overComboLayout;
	TLayout *layoutContIcons;
	TLabel *lblCompability;
	TCalloutPanel *hintPanel;
	TLabel *lblHint;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall buttonAcceptClick(TObject *Sender);
	void __fastcall buttonCancelClick(TObject *Sender);
	void __fastcall comboTileTypeChange(TObject *Sender);
	void __fastcall rectColorClick(TObject *Sender);
	void __fastcall colorButtonClick(TObject *Sender);
	void __fastcall comboServoPartChange(TObject *Sender);
	void __fastcall checkBoxDataLimitChange(TObject *Sender);
	void __fastcall dispTypeUpDownChange(TObject *Sender);
	void __fastcall contIconMouseEnter(TObject *Sender);
	void __fastcall contIconMouseLeave(TObject *Sender);
	void __fastcall dataChange(TObject *Sender);
private:	// User declarations
	bool nowOpened;
	//procedura do tworzenia nowych kafelkow (w przypadku zmiany typu)
	void updateTile(void);
	//procedura o zapamietywania parametrow sztywnych
	void backupConstParams(void);
	bool checkChanges(void);
	//procedura do updateowania layoutu okienka
	void updateView(int currID);
	//ustawianie widokow dla kazdego typu kafelka
	void viewTileZero(bool locked);
	void viewTilePilotLamp(bool locked);
	void viewTilePushButton(bool locked);
	void viewTilePushButtonWLamp(bool locked);
	void viewTile2Selector(bool locked);
	void viewTile3Selector(bool locked);
	void viewTileDigitalSwitch(bool locked);
	void viewTileDigitalDisplay(bool locked);
	void viewTileVarDataDisplay(bool locked);
	void viewTileStrDisplayWindow(bool locked);
	void viewTileMonitorCommand(bool locked);
	void viewTilePalletize(bool locked);
	void viewTileStrDataDisplay(bool locked);
	void viewTile2SelectorWLamp(bool locked);
	void viewTile3SelectorWLamp(bool locked);
	void viewTileServoDispenser(bool locked);
	TColorButton *callerColorButton;
	unsigned int colorEmphasize, colorGlow;
	//kopie parametrow "STALYCH" kafelkow do porownywania czy byla zmiana
	bool changesMade;
	tileType currTileType;
	TFmxObject *currTileParent;
	int currTilePosX, currTilePosY, openID, closeID, currTileNO;
	//wylaczenie dostepu do edycji (jezeli locked)
	void lockDecision(bool lock);
	int decodeID(int comboIndex);
	int encodeID(int id);
	//------------------------------------------------------------
	//funkcje do formatowania okienka edycji kafelka
	//------------------------------------------------------------
	void prepareColorsView(int elementsVisible[4],bool singleFullLen,bool rightSide);
	void prepareSignalsView(int elementsVisible[4],position layoutPos,position editsPos);
	void placeSignalsLayout(position layoutPos);
	void placeSignalsEdits(int elementsVisible[3],int digitalOpt,position editsPos);
	void placeSignalsFeatures(int digitalOpt);
	void prepareVariablesView(int elementsVisible[2],bool rightSide);
	void prepareMiscView(componentPos elements[3],bool rightSide);
	void prepareSpecialView(specialView viewNo,bool rightSide);
	//funkcje pomocnicze w formatowaniu (niekoniecznie formatujace)
	bool entryExistsInTable(String entry, String table[],int size);
	int sumInitialValue(int table[],int size);
	int extremeElementPos(TLayout *layout, position poz);
public:		// User declarations
	__fastcall TtilePropertiesWindow(TComponent* Owner);
	zeroTile *callerTile;
};
//---------------------------------------------------------------------------
extern PACKAGE TtilePropertiesWindow *tilePropertiesWindow;
//---------------------------------------------------------------------------
#endif

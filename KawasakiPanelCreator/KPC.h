//---------------------------------------------------------------------------
#ifndef KPCH
#define KPCH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Effects.hpp>
#include <FMX.Menus.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Filter.Effects.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.Platform.Win.hpp>
//---------------------------------------------------------------------------
#include "kawasakiComm.h"
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "zeroTile.h"
#include "pilotLamp.h"
#include "pushButton.h"
#include "pushButtonLamp.h"
#include "selector2.h"
#include "selector3.h"
#include "digitalSwitch.h"
#include "digitalDisplay.h"
#include "variableData.h"
#include "stringDisplay.h"
#include "monitorCommand.h"
#include "palletize.h"
#include "stringData.h"
#include "selector2Lamp.h"
#include "selector3Lamp.h"
#include "servoDispenser.h"
//---------------------------------------------------------------------------
enum action{
    nothing = 0,
	lastProject = 1,
	newProject = 2,
	overwrite = 3,
	newFile = 4,
	atExit = 5,
	atSaveProject = 6,
	realTimePrior = 7,
	highPrior = 8,
	normalPrior = 9,
	lowPrior = 10,
	saveQuick = 11,
	saveAs = 12,
	saveCont = 13
};
//---------------------------------------------------------------------------
class TmainWindow : public TForm
{
//klasa opcji i robotow jest zaprzyjazniona z oknem glowym
friend class TkpcOpt;
friend class  TKPCtoRobot;
__published:	// IDE-managed Components
	TStyleBook *StyleBook1;
	TPanel *tileSelectionGrabber;
	TPathAnimation *pathMouseOn;
	TPathAnimation *pathMouseOff;
	TButton *buttonSHtiles;
	TPanel *mainBackground;
	TPopupMenu *tileGrabberMenu;
	TMenuItem *_gmiAlwaysOn;
	TMenuItem *_gmiAnimation;
	TPanel *mainMenuTop;
	TButton *buttonExit;
	TLabel *lblAppTitle;
	TScrollBox *scrollBaseTiles;
	TPanel *addNewPatternTile;
	TLabel *lblPatternTilesNo;
	TButton *btnDeleteTile;
	TMonochromeEffect *monochromeEffect;
	TColorAnimation *animEmphasizeOptionsTile;
	TButton *btnModifyTile;
	TButton *btnDoneTile;
	TImage *mainMenuIcon;
	TInnerGlowEffect *mainIcoInnerGlow;
	TButton *btnNew;
	TButton *btnOpen;
	TButton *btnSave;
	TButton *btnSettings;
	TPanel *panelMenu;
	TRectangle *rectMenuMask;
	TFloatAnimation *mainMenuPosXanim;
	TTabControl *tabPanelPages;
	TTabItem *panelPage1;
	TTabItem *panelPage2;
	TTabItem *panelPage3;
	TTabItem *panelPage4;
	TTabItem *panelPage5;
	TTabItem *panelPage6;
	TTabItem *panelPage7;
	TTabItem *panelPage8;
	TClearingEdit *editTitlePage1;
	TLayout *layout1_top;
	TLayout *layout1_down;
	TLabel *titleTitlePage1;
	TLayout *layout2_top;
	TLayout *layout2_down;
	TLayout *layout3_top;
	TLayout *layout3_down;
	TLayout *layout4_top;
	TLayout *layout4_down;
	TLayout *layout5_top;
	TLayout *layout5_down;
	TLayout *layout6_down;
	TLayout *layout6_top;
	TLayout *layout7_top;
	TLayout *layout7_down;
	TLayout *layout8_top;
	TLayout *layout8_down;
	TLabel *titleTitlePage2;
	TLabel *titleTitlePage3;
	TLabel *titleTitlePage4;
	TLabel *titleTitlePage5;
	TLabel *titleTitlePage6;
	TLabel *titleTitlePage7;
	TLabel *titleTitlePage8;
	TClearingEdit *editTitlePage2;
	TClearingEdit *editTitlePage3;
	TClearingEdit *editTitlePage4;
	TClearingEdit *editTitlePage5;
	TClearingEdit *editTitlePage6;
	TClearingEdit *editTitlePage7;
	TClearingEdit *editTitlePage8;
	TAniIndicator *loadIndicator;
	TRectangle *page1Mask;
	TLabel *lblLoadPercent;
	TRectangle *page2Mask;
	TRectangle *page3Mask;
	TRectangle *page4Mask;
	TRectangle *page5Mask;
	TRectangle *page6Mask;
	TRectangle *page7Mask;
	TRectangle *page8Mask;
	TSaveDialog *saveProjectDialog;
	TOpenDialog *openProjectDialog;
	TButton *btnSwapTile;
	TButton *btnAddPatternTile;
	TButton *btnSwapCancelTile;
	TButton *btnCancelAction;
	TButton *btnCopyTile;
	TButton *btnPasteTile;
	TLabel *exclamationMark;
	TLabel *lblLoadInfo;
	TTimer *autosaveTimer;
	TPanel *splashScreen;
	TTimer *splashTime;
	TImage *Image1;
	TLabel *lblSplashStatus;
	TLabel *lblSplashAppTitle;
	TLabel *lblSplashVerNo;
	TLayout *layoutSplashInfo;
	TLabel *lblSplashProgress;
	TButton *btnRobot;
	TShadowEffect *saveShadow;
	TShadowEffect *robotShadow;
	TMonochromeEffect *robotGray;
	void __fastcall pathMouseOnProcess(TObject *Sender);
	void __fastcall pathMouseOffProcess(TObject *Sender);
	void __fastcall pathMouseOffFinish(TObject *Sender);
	void __fastcall pathMouseOnFinish(TObject *Sender);
	void __fastcall buttonSHtilesMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
	void __fastcall _gmiAnimationClick(TObject *Sender);
	void __fastcall _gmiAlwaysOnClick(TObject *Sender);
	void __fastcall buttonExitClick(TObject *Sender);
	void __fastcall addNewPatternTileClick(TObject *Sender);
	void __fastcall mainMenuTopMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
	void __fastcall mainMenuIconClick(TObject *Sender);
	void __fastcall mainMenuBtnMouseLeave(TObject *Sender);
	void __fastcall btnNewClick(TObject *Sender);
	void __fastcall btnSaveMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y);
	void __fastcall btnOpenClick(TObject *Sender);
	void __fastcall btnSettingsClick(TObject *Sender);
	void __fastcall autosaveTimerTimer(TObject *Sender);
	void __fastcall splashTimeTimer(TObject *Sender);
	void __fastcall titleChangeAttempt(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall btnRobotMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
private:	// User declarations
	bool checkClassZeroTile(TObject *obj);
	zeroTile* getTilePointer(TObject *obj);
	String tileColorToStr(tileColors color);
	//funkcja do odczytywania i zapisywania predefiniowanych kafelkow
	void loadTilesFromXML(TFmxObject *owner, String fileName);
	void saveTilesToXML(TFmxObject *owner, String fileName);
	//sprawdzenie czy dany obiekt ma gdzies w sobie kafelki
	bool objectHasTiles(TFmxObject *object);
	bool objectHasTiles(TFmxObject *object, TFmxObject &hasTilesName);
	void clearTilesInPage(TFmxObject *page);
	zeroTile* findTileByPos(TFmxObject *tileParent, int x, int y);
	zeroTile* findTileByNo(int tileNo);
	int getTileNoByPos(TFmxObject *parent, int x, int y);
	TComponent* getTileParent(int tileNo);
	bool copyTile(zeroTile *tileToCopy);
	bool placePatternTile(zeroTile *patternTile);
	bool swapRegularTile(zeroTile *currTile);
	void onlyTilesEnabled(bool condition);
	void setNoOfTabsVisible(int value);
	//zmiana kursora aplikacji
	void kpcSetCursor(TCursor curNo);
	void kpcSetCursor(TControl* currCtrl,TCursor curNo);
	//metody do obslugi kafelkow o mozliwym rozmiarze > 1
	int  wideTileCheckArea(zeroTile *tile);
	void wideTileCleanArea(zeroTile *tile, int toDoAction);
	void wideTileAddToArea(zeroTile *startTile);
	void wideTileDelete(zeroTile *masterTile, bool delMasterTile);
	String getVersionNumber(String fileName);
	action saveMode;
	void killDLLcomm(void);
	bool saveOK, robotControl, robotCheckEnabled;
	//obsluga tablicy wybranych kafelkow
	zeroTile* selectedTiles[10];
	void selectedTableClear(void);
	bool selectedTablePushBack(zeroTile *curr);
	bool selectedTableUnselect(zeroTile *curr);
	bool selectedTableElExists(zeroTile *curr);
	int selectedTableCapacity(void);
protected:
	//funkcja do zapisywania projektu do pliku IFP
	void saveProjectToIFP(String fileName, bool forDcontroller=false);
	bool patternInsert, tileSwap, tileCopy;
	bool patternTilesUnfolded, mainMenuVisible;
	bool showSplash, patternsAtFront, patternsAdd;
	bool rightOptions, leftOptions, debugActive, checkKillDLL;
	bool tilesID, tilesNO, tileEmphasize;
	bool msgOnExit, msgOnTileDel, msgOnProject, msgOnNoTitles;
	bool autosaveActive, initDLL, addToRecent;
	float animationTime, lastSpeed;
	int mostTop, mostLeft, noOfPages, autosaveTime;
	action autosaveMethod, openAction, appPriority, patternSave;
	unsigned int glowRegular, glowPattern, glowProtected, colorEmphasize;
	//zapamietany (skopiowany) kafelek
	zeroTile *copiedTile;
	//funkcje do zapisu/odczytu ustawien
	void loadSettings(String fileName);
	void saveSettings(String fileName);
	void applySettings(void);
	//obiekt do komunikacji z robotem KAWASAKI
	kawasakiComm *robot;
	String recentRobotsFileName, settingsFilePath, patternsFileName;
public:		// User declarations
	__fastcall TmainWindow(TComponent* Owner);
	void __fastcall tileMouseDown(TObject *Sender, TMouseButton Button,TShiftState Shift, float X, float Y);
	void __fastcall tileDeleteButtonClick(TObject *Sender);
	void __fastcall tileSwapButtonClick(TObject *Sender);
	void __fastcall tileSwapCancelButtonClick(TObject *Sender);
	void __fastcall tileAddPatternButtonClick(TObject *Sender);
	void __fastcall tileCancelActionButtonClick(TObject *Sender);
	void __fastcall tileModifyButtonClick(TObject *Sender);
	void __fastcall tileDoneButtonClick(TObject *Sender);
	void __fastcall tileCopyButtonClick(TObject *Sender);
	void __fastcall tilePasteButtonClick(TObject *Sender);
	//adres komponentu ktory ma wyswietlone opcje
	zeroTile *optionsTile;
	zeroTile* createTile(int ID,tileType type,TFmxObject *parent);
	int ifTileEmpty(TFmxObject *parent, int x, int y);
	TBitmap* loadServoImage(int no);
	//uaktualnienie kafelka o mozliwym rozmiarze > 1
	void wideTileCreate(zeroTile *tile);
	//uaktualnianie wygladu wszystkich kafelkow
	void updateTileColors(void);
	void updateTileInfo(bool showIDs, bool showNOs);
	//funkcje do obslugi projektow
	String saveFileName;
	bool projectActive, changesDone;
	void createEmptyProject(double *logPercent=NULL);
	void createEmptyPage(TFmxObject *page, double *logPercent=NULL);
	void loadProjectFromIFP(String fileName,double *logPercent=NULL);
	bool extractProjectFromFile(String fromFile,String toFile);
	void saveSettingsProjPath(String fullPath);
	void setAppPriority(void);
	String projNameFromPath(String fullPath);
	bool checkDControllerCompability(String &reason);
	void viewDControllerCompability(bool show);
	bool checkTitlesEmpty(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TmainWindow *mainWindow;
//---------------------------------------------------------------------------
#endif

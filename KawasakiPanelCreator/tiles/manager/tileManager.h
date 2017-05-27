//---------------------------------------------------------------------------
#ifndef tileManagerH
#define tileManagerH
//---------------------------------------------------------------------------
#include <FMX.Colors.hpp>
//---------------------------------------------------------------------------
//definicja predefiniowanych kolorow kafelkow
enum tileColors{
	gray = claLightgray,
	blue = claBlue,
	red = claRed,
	orange = claOrange,
	green = claLime,
	paleBlue = claLightblue,
	yellow = claYellow,
	white1 = claWhite,
	pink = claPink,
	white2 = claWhite,
	black = claBlack,
	cyan = claDarkcyan,
	navy = claNavy,
	redBrown = claRosybrown,
	darkGreen = claGreen,
	lavender = claLavender
};
//---------------------------------------------------------------------------
//definicja typu kafelka
enum tileType{
	regular = 0,
	patternCustom,
	patternLocked,
};
//---------------------------------------------------------------------------
//definicja zrodla danych kafelka
enum tileSource{
	empty = 0,
	xml,
	ifp,
};
//---------------------------------------------------------------------------
class zeroTile;
//---------------------------------------------------------------------------
class tileManager {
	public:
		//konstruktor i destruktor
		__fastcall tileManager();
		__fastcall ~tileManager();
		//metody do obslugi zrodel, plikow
		void update(tileSource source,String fileName,int whichEntry);
		tileSource getSource(void);
		void getFileInfo(String &fileName, int &entry);
		//metody do obslugi kafelkow
		bool checkClassZeroTile(TObject *obj);
		void backupConstParams(void);
		void updateTile(int newID);
	protected:

	private:
		zeroTile *callerTile;
		//zmienne do obslugi plikow
		tileSource currSource;
		String currFileName;
		int currEntry;
		//zmienne do obslugi kafelkow
		int startID;
		TComponent *currTileOwner;
		tileType currTileType;
		TFmxObject *currTileParent;
		int currTilePosX, currTilePosY;
};
#endif

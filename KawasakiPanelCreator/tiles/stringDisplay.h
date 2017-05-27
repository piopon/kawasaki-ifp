//---------------------------------------------------------------------------
#ifndef stringDisplayH
#define stringDisplayH
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "zeroTile.h"
//---------------------------------------------------------------------------
class zeroTile;
class stringDisplay : public zeroTile {
	public:
		//konstruktor
		__fastcall stringDisplay(TComponent *myOwner, tileType typeOfTile=regular, int tileNo=1);
		//destruktor
		__fastcall ~stringDisplay();
		zeroTile* createTile(TComponent *myOwner, tileType typeOfTile);
		//otwarcie edytora danego kafelka
		zeroTile* showEditor(void);
		//pokazywanie prostokata okalajacego
		void bound(int tileNo, int ofTiles);
		void bound(bool left, bool right, bool top, bool bottom);
		//wyslanie zmian z edytora do kafelka
		void setTileContent(void);
		//wyslanie zmian z kafelka do kafelka
		void setTileContent(zeroTile *originalTile);
        //wyslanie zmian z kafelka do edytora
		void getTileContent(void);
		//uatualnienie koloru labeli w zaleznosci od koloru tla
		void adjustLabelsColor(tileColors backgroundColor);
		//obsluga plikow XML (kafelkow wzorcowych)
		void saveToXML(TXMLDocument *xmlSource, int tileNo=-1);
		void loadFromXML(TXMLDocument *xmlSource, int tileNo=-1);
		//obsluga plikow projektu *.ifp
		AnsiString saveToIFP(int tileNo);
		void loadFromIFP(String ifpData, int tileNo=-1);
	protected:
		TRectangle *boundRect;
		TLabel *labelWinNo;
		TLabel *labelTileNfo;
		//numer okna
		int number;
	private:
		int sizeTileNo;
		//efekt poswiaty wokol kafelka
		TGlowEffect *glowTest;
};
//---------------------------------------------------------------------------
#endif

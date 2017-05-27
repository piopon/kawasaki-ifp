//---------------------------------------------------------------------------
#ifndef selector3LampH
#define selector3LampH
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "selector3.h"
//---------------------------------------------------------------------------
class zeroTile;
class selector3Lamp : public selector3 {
	public:
		//konstruktor
		__fastcall selector3Lamp(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~selector3Lamp();
		zeroTile* createTile(TComponent *myOwner, tileType typeOfTile);
		//otwarcie edytora danego kafelka
		zeroTile* showEditor(void);
		//wyslanie zmian z edytora do kafelka
		void setTileContent(void);
		//wyslanie zmian z kafelka do kafelka
		void setTileContent(zeroTile *originalTile);
        //wyslanie zmian z kafelka do edytora
		void getTileContent(void);
		//obsluga plikow XML (kafelkow wzorcowych)
		void saveToXML(TXMLDocument *xmlSource, int tileNo=-1);
		void loadFromXML(TXMLDocument *xmlSource, int tileNo=-1);
		//obsluga plikow projektu *.ifp
		AnsiString saveToIFP(int tileNo);
		void loadFromIFP(String ifpData, int tileNo=-1);
	protected:
		//liczba calkowita jako nr sygnalu lampki
		int signalLamp;
	private:
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef pushButtonLampH
#define pushButtonLampH
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "pushButton.h"
//---------------------------------------------------------------------------
class zeroTile;

class pushButtonLamp : public pushButton {
	public:
		//konstruktor
		__fastcall pushButtonLamp(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~pushButtonLamp();
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
		//liczba calkowita jako nr sygnalu przycisku
		int signalLampNo;
	private:
};
//---------------------------------------------------------------------------
#endif

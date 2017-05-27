//---------------------------------------------------------------------------
#ifndef digitalDisplayH
#define digitalDisplayH
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "zeroTile.h"
//---------------------------------------------------------------------------
class zeroTile;
class digitalDisplay : public zeroTile {
	public:
		//konstruktor
		__fastcall digitalDisplay(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~digitalDisplay();
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
		TLine *separator;
		//komponenty typu TLabel jako tekst w kafelku
		TLabel *label1;
		TLabel *label2;
		TLabel *labelVal;
		//startowy sygnal oraz liczba sygnalow
		int topSignalNo;
		int signalsNo;
		//liczba wyswietlanych znakow
		int figuresNo;
		//flagi wewnetrzne kafelka
		bool minusOn;
		bool sigTypeBCD;
	private:
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef pilotLampH
#define pilotLampH
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "zeroTile.h"
//---------------------------------------------------------------------------
class zeroTile;
class pilotLamp : public zeroTile {
	public:
		//konstruktor
		__fastcall pilotLamp(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~pilotLamp();
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
		//komponenty typu TLabel jako tekst w kafelku
		TLabel *label1;
		TLabel *label2;
		TLabel *label3;
		TLabel *label4;
		//komponent typu TCircle jako lampka w kafelku
		TCircle *lampOn;
		TCircle *lampOff;
		//liczba calkowita jako nr sygnalu przycisku
		int signalNo;
		//bool jako flaga dostepnosci akcji kafelka
		bool exclMark;
		TLabel *exclamation;
	private:
};
#endif

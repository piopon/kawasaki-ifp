//---------------------------------------------------------------------------

#ifndef selector3H
#define selector3H
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "selector2.h"
//---------------------------------------------------------------------------
class selector2;

class selector3 : public selector2 {
	public:
		//konstruktor
		__fastcall selector3(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~selector3();
		zeroTile* createTile(TComponent *myOwner, tileType typeOfTile);
		//uaktualnienie wygladu selektora
		void selectDisplayType(bool dispType, bool changeState = false);
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
		//UWAGA! Tlo kafelka zostaje defultowe z rodzica zeroTile
		//komponenty typu TRectangle jako tlo selektora (gdy opcja up / down)
		TRectangle *barColor_M;
		//liczba calkowita jako nr sygnalu przycisku
		int signalMiddle;
	private:
};
//---------------------------------------------------------------------------
#endif

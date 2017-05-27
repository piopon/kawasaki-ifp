//---------------------------------------------------------------------------
#ifndef selector2H
#define selector2H
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "zeroTile.h"
//---------------------------------------------------------------------------
class zeroTile;
class selector2 : public zeroTile {
	public:
		//konstruktor
		__fastcall selector2(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~selector2();
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
		//komponenty typu TLabel jako tekst w kafelku
		TLabel *label1;
		TLabel *label2;
		TLabel *label3;
		TLabel *label4;
		//UWAGA! Tlo kafelka zostaje defultowe z rodzica zeroTile
		//komponenty typu TRectangle jako tlo selektora (gdy opcja up / down)
		TRectangle *barColor_L_U;
		TRectangle *barColor_R_D;
		//komponent TCircle i TRectangle jako tlo selektora (gdy opcja rot)
		TCircle *backgroundKnob;
        TRectangle *backgroundBar;
		TRectangle *barColor;
		//liczba calkowita jako nr sygnalu przycisku
		int signalLeft_altUp;
		int signalRight_altDown;
		//bool jako flaga dostepnosci akcji kafelka
		bool operationCond;
		bool rotDispType;
	private:
};
#endif

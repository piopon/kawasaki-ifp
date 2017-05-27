//---------------------------------------------------------------------------
#ifndef servoDispenserH
#define servoDispenserH
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "zeroTile.h"
//---------------------------------------------------------------------------
class zeroTile;
class servoDispenser : public zeroTile {
	public:
		//konstruktor
		__fastcall servoDispenser(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~servoDispenser();
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
		//komponent typu TImage jako tlo w kafelku
		TImage *image;
		int partNo;
		//liczba calkowita jako nr sygnalu przycisku
		int signalNo;
	private:
};
#endif

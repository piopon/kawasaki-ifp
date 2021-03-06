//---------------------------------------------------------------------------

#ifndef monitorCommandH
#define monitorCommandH
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "zeroTile.h"
//---------------------------------------------------------------------------
class zeroTile;
class monitorCommand : public zeroTile {
	public:
		//konstruktor
		__fastcall monitorCommand(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~monitorCommand();
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
		//komponent typu TRectangle jako tlo w kafelku w stanie ON
		TRectangle *backgroundOn;
		//liczba calkowita jako nr sygnalu przycisku
		int commandNo;
		String command;
		//bool jako flaga dostepnosci akcji kafelka
		bool operationCond;
	private:
};
//---------------------------------------------------------------------------
#endif

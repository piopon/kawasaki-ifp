//---------------------------------------------------------------------------
#ifndef digitalSwitchH
#define digitalSwitchH
//---------------------------------------------------------------------------
#include "tileManager.h"
#include "digitalDisplay.h"
//---------------------------------------------------------------------------
class digitalDisplay;
class digitalSwitch : public digitalDisplay {
	public:
		//konstruktor
		__fastcall digitalSwitch(TComponent *myOwner, tileType typeOfTile=regular);
		//destruktor
		__fastcall ~digitalSwitch();
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
		//limit gorny i dolny
		bool dataLimit;
		int upperLimit;
		int lowerLimit;
		//flagi wewnetrzne kafelka
		bool inpTypeKeys;
		bool operationCond;
	private:
};
//---------------------------------------------------------------------------
#endif

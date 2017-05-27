//---------------------------------------------------------------------------
#ifndef zeroTileH
#define zeroTileH
//---------------------------------------------------------------------------
#include <FMX.Effects.hpp>
#include <Xml.Win.msxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
//---------------------------------------------------------------------------
#include "tileManager.h"
//---------------------------------------------------------------------------
class zeroTile : public TPanel {
	//okno z parametrami ma miec dostep do skladnikow protected
	friend class TtilePropertiesWindow;
	friend class TmainWindow;

	public:
		//konstruktor
		__fastcall zeroTile(TComponent *myOwner, tileType typeOfTile=regular, tileSource dataSource=empty);
		//konstruktor kopiuj¹cy
		zeroTile(const zeroTile &original);
		//destruktor
		virtual __fastcall ~zeroTile();
		//operator przypisania
		zeroTile& operator=(zeroTile &original);
		//funkcje symulujace "konstruktor wirtualny"
		virtual zeroTile* createTile(TComponent *myOwner, tileType typeOfTile=regular);
		virtual zeroTile* copyTile();
		//pobieranie (TYLKO!) typu i identyfikatora kafelka
		virtual tileType getType(void);
		int getID(void);
		//funkcje do obslugi koloru tla kafelka
		virtual void setBackgroundColor(tileColors newColor);
		tileColors getBackgroundColor(void);
		//otwieranie i zamykanie opcji
		void menuOptions(bool show, bool swappingTiles=false, bool insertingPattern=false, int tilesSelectedNo=1);
		//otwarcie edytora danego kafelka
		virtual zeroTile* showEditor(void);
		//wyslanie zmian z edytora do kafelka
		virtual void setTileContent();
		//wyslanie zmian z kafelka do kafelka
		virtual void setTileContent(zeroTile *originalTile);
		//wyslanie zmian z kafelka do edytora
		virtual void getTileContent();
		//obsluga plikow XML (kafelkow wzorcowych)
		virtual void saveToXML(TXMLDocument *xmlSource, int tileNo=-1);
		virtual void loadFromXML(TXMLDocument *xmlSource, int tileNo=-1);
		//obsluga plikow projektu *.ifp
		virtual AnsiString saveToIFP(int tileNo);
		virtual void loadFromIFP(String ifpData, int tileNo=-1);
		//wyroznienia (miganie tla) kafelka
		void setGlowColor(unsigned int regular, unsigned int protect, unsigned int pattern);
		void setEmphasizeColor(unsigned int newColor);
		void viewIncompatibleTile(bool show);
		void selectTile(bool state);
		bool isSelected(void);
		void emphasizeTile(void);
		//ustawianie i odczytywania ID mastera
		void setUniqueID(int newID);
		int getUniqueID(void);
		//ustawianie wyswietlania dodatkowych informacji - ID kafelka, nr kafelka
		void viewID(bool visible);
		void viewNO(int nr, bool visible);
	protected:
		//id kafelka oraz jego typ
		int ID;
		int size;
		tileType type;
		//unikalny adres kafelka nadrzednego (gdy rozmiar > 1)
		int uniqueID;
		int no;
		//komponent typu TButton do usuwania, edycji i akceptacji danego kafelka
		TButton *delButton;
		TButton *swapButton;
		TButton *addPatternButton;
		TButton *modButton;
		TButton *doneButton;
		TButton *copyButton;
		TButton *pasteButton;
		TButton *swapCancelButton;
		TButton *cancelActionButton;
		//komponent typu TRectangle jako tlo w kafelku (lub tlo w stanie OFF)
		TRectangle *background_altOff;
		TRectangle *idField;
		TLabel *idText;
		TRectangle *noField;
		TLabel *noText;
		int colorToInt(tileColors color);
		tileColors intToColor(int color);
		String colorToStr(tileColors color);
		tileColors strToColor(String color);
		//efekt poswiaty wokol kafelka
		TGlowEffect *glow;
		//obsluga "wybrania" kafelka
		TGlowEffect *selection;
		bool selected;
		//linia pokazujaca niekompatybilny kafelek
		TLine *cross1;
		TLine *cross2;
	private:
		//efekt migania kafelka ktory ma aktywne przyciski opcji
		TColorAnimation *emphasize;
		//akcja na mysz (poswiata po wjezdzie i jej znikniecie po wyjezdzie
		void __fastcall tileMouseLeave(TObject *Sender);
		void __fastcall tileMouseEnter(TObject *Sender);
};
//---------------------------------------------------------------------------
#endif

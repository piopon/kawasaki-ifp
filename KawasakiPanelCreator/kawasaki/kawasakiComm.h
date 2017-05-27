//---------------------------------------------------------------------------
#ifndef kawasakiCommH
#define kawasakiCommH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
enum kawasakiStatus {
	ok = 0,
	param1Error = -1,
	param2Error = -2,
	param3Error = -3,
	socketNok = -10,
	connFailed = -11,
	disconnFailed = -12,
	connInProgress = -13,
	notConn = -14,
	cmdInProgress = -15,
	cmdError = -16,
	cmdNok = -17,
	commError = -18,
	cmdInquiry = -19,
	cmdAborted = -20,
	dllMissing = -99,
	dataDisplayed = 1,
	dataInput = 2,
	dataContinuous = 3,
	dllNotInit = 4,
	dllAlreadyInit = 5,
	alreadyConn = 10,
	alreadyDisconn = 11
};
String kawasakiStatusToStr(kawasakiStatus currStatus);
//---------------------------------------------------------------------------
class kawasakiComm {
	public:
		//konstruktor
		kawasakiComm(String ip, int port, bool init);
        //destruktor
		~kawasakiComm();
		//uruchomienie serwisu komunikacyjnego + DLL
		kawasakiStatus initComm();
		//zatrzymanie serwisu komunikacyjnego + DLL
		kawasakiStatus closeComm();
		// proba nawiazania polaczenia
		kawasakiStatus connect();
		//rozlaczenie sie z robotem
		kawasakiStatus disconnect();
		//pobranie informacji o robocie
		kawasakiStatus getRobotInfo(String robotInfoFile);
		//pobranie istniejacego IFPanelu z robota
		kawasakiStatus uploadPanelFromRobot(String toFile);
		//wyslanie obecnego IFPanelu do robota
		kawasakiStatus downloadPanelToRobot(String fromFile);
		//pobranie wersji biblioteki
		void getVersion(String &verInfo);
		//sprawdzenie statusu po realizacji zadania
		kawasakiStatus getStatus(String &msg);
		//wyslanie odpowiedzi do robota (gdy robot wymaga np. potwierdzenia)
		kawasakiStatus reply(String &answer);
		//sprawdzenie czy udalo sie uruchomic modul komunikacyjny
		bool isDLLinit();
		//sprawdzenie czy jestesmy polaczeni z robotem
		bool isConnected();
		//uaktualnienie parametrow polaczenia (IP oraz PORT)
		void updateConnParams(String ip, int portNo);
		//uaktualnienie pliku zrodlowego z ustawieniami robota
		void updateSourceFile(String fileLoc);
		//dopasowanie pliku do wymagan robota Kawasaki
		bool formatFile(String source, String formatted);
	protected:
	private:
		HINSTANCE dllHandle;
		bool commInit;
		bool connected;
		String libName;
		String sourceFile;
		long socketID;
		String ipAddress;
		int portNo;
};
//---------------------------------------------------------------------------
#endif

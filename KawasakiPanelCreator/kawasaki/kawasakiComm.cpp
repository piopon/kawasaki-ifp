//---------------------------------------------------------------------------
#pragma hdrstop
#include "kawasakiComm.h"
//---------------------------------------------------------------------------
#include <fstream.h>
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//konstruktor
kawasakiComm::kawasakiComm(String ip, int port, bool init): ipAddress(ip), portNo(port)
{
	libName = "ASKCT.dll";
	sourceFile = "";
	//parametr przyszlosciowy (wg dokumentacji), narazie ustawiany na zero!
	socketID = 0L;
	commInit = false;
	connected = false;
	dllHandle = NULL;
	//sprawdzamy czy od razu trzeba zainicjalizowac modul komunikacyjny
	if (init) initComm();
}
//---------------------------------------------------------------------------
//destruktor
kawasakiComm::~kawasakiComm()
{
	//rozlaczamy sie z robotem (jezeli w ogole byla wlaczona dllka
	if (dllHandle && commInit) {
		kawasakiStatus disconnStatus = disconnect();
		if (disconnStatus == ok || disconnStatus == alreadyDisconn) {
			connected = false;
		}
		//wylaczamy modul komunikacyjny
		closeComm();
		dllHandle = NULL;
		commInit = false;
	}
	//zerujemy informacje o bibliotece, adresie i porcie
	libName = "";
	sourceFile = "";
	ipAddress = "";
	portNo = 0;
	socketID = 0;
}
//---------------------------------------------------------------------------
//uruchomienie serwisu komunikacyjnego + DLL
kawasakiStatus kawasakiComm::initComm()
{
	long result = 0;
	//sprawdzamy czy juz nie byla zainicjalizowana komunikacja i dllka
	if (dllHandle == NULL && commInit == false) {
		//ladujemy biblioteke do pamieci
		dllHandle = LoadLibrary(libName.c_str());
		if(dllHandle != NULL) {
			typedef long (__stdcall *kctInit)(long SockID);
			kctInit init = (kctInit)GetProcAddress(dllHandle, "AsKCTInit");
			//jezeli funkcja istnieje to jest wywolywana
			if(init != NULL) {
				result = init(socketID);
				if (result == 0) commInit = true;
			}
		}  else  {
			result = -99;
			//nie udalo sie polaczyc z biblioteka - zwalniamy ja z zasobow
			FreeLibrary(dllHandle);
		}
	} else {
		//juz jest zainicjalizowana biblioteka
		result = 5;
    }

    return static_cast<kawasakiStatus>(result);
}
//---------------------------------------------------------------------------
//zatrzymanie serwisu komunikacyjnego + DLL
kawasakiStatus kawasakiComm::closeComm()
{
	long result = 0;
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		typedef long (__stdcall *kctDestroy)(long SockID);
		kctDestroy destroy = (kctDestroy)GetProcAddress(dllHandle, "AsKCTDestroy");
		//jezeli funkcja istnieje to jest wywolywana
		if(destroy != NULL) {
			result = destroy(socketID);
			if (result == 0) commInit = false;
		}
		//zwalniamy biblioteke z zasobow
		FreeLibrary(dllHandle);
		dllHandle = NULL;
	}  else  {
		result = 4;
	}

    return static_cast<kawasakiStatus>(result);
}
//---------------------------------------------------------------------------
// proba nawiazania polaczenia
kawasakiStatus kawasakiComm::connect()
{
	long result = 0;
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		typedef long (*kctConnect)(long SockID, char *ip);
		kctConnect connect = (kctConnect)GetProcAddress(dllHandle, "AsKCTConnect");
		//jezeli funkcja istnieje to jest wywolywana
		if(connect != NULL) {
			//rezerwujemy miejsce na adres IP
			char *ipAddr = AnsiString(ipAddress).c_str();
			result = connect(socketID,ipAddr);
			if (result == 0 || result == 10) connected = true;
		}
	} else {
		result = 4;
	}
	return static_cast<kawasakiStatus>(result);
}
//---------------------------------------------------------------------------
//rozlaczenie sie z robotem
kawasakiStatus kawasakiComm::disconnect()
{
	long result = -1;
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		//sprawdzamy czy wczesniej sie polaczylismy
		if (connected) {
			typedef long (*kctDisconnect)(long SockID);
			kctDisconnect disconnect = (kctDisconnect)GetProcAddress(dllHandle, "AsKCTDisconnect");
			//jezeli funkcja istnieje to jest wywolywana
			if(disconnect != NULL) {
				result = disconnect(socketID);
				if (result == 0 || result == 11) connected = false;
			}
		} else {
			result = -14;
        }
	} else {
		result = 4;
	}
	kawasakiStatus final = static_cast<kawasakiStatus>(result);
	return final;
}
//---------------------------------------------------------------------------
//pobranie informacji o robocie
kawasakiStatus kawasakiComm::getRobotInfo(String robotInfoFile)
{
	long result = -1L;
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		//sprawdzamy czy jestesmy polaczeni z robotem
		if (connected) {
			typedef long (*kctRobInfo)(long SockID, char *file, char *mode);
			kctRobInfo robInfo = (kctRobInfo)GetProcAddress(dllHandle, "AsKCTSave");
			//jezeli funkcja istnieje to jest wywolywana
			if(robInfo != NULL) {
				char *fileName = AnsiString(robotInfoFile).c_str();
				char *fileMode = AnsiString("").c_str();
				result = robInfo(socketID, fileName, fileMode);
			}
		} else {
			result = -14;
		}
	} else {
		result = 4;
	}

	return static_cast<kawasakiStatus>(result);
}
//---------------------------------------------------------------------------
//pobranie istniejacego IFPanelu z robota
kawasakiStatus kawasakiComm::uploadPanelFromRobot(String toFile)
{
	long result = -1L;
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		//sprawdzamy czy jestesmy polaczeni z robotem
		if (connected) {
			typedef long (*kctGetPanel)(long SockID, char *file, char *mode);
			kctGetPanel getPanel = (kctGetPanel)GetProcAddress(dllHandle, "AsKCTSave");
			//jezeli funkcja istnieje to jest wywolywana
			if(getPanel != NULL) {
				char *fileName = AnsiString(toFile).c_str();
				char *fileMode = AnsiString("").c_str();
				result = getPanel(socketID, fileName, fileMode);
			}
		} else {
			result = -14;
		}
	} else {
		result = 4;
	}
	return static_cast<kawasakiStatus>(result);
}
//---------------------------------------------------------------------------
//wyslanie obecnego IFPanelu do robota
kawasakiStatus kawasakiComm::downloadPanelToRobot(String fromFile)
{
	long result = -1L;
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		//sprawdzamy czy jestesmy polaczeni z robotem
		if (connected) {
			typedef long (*kctSetPanel)(long SockID, char *file, char *mode);
			kctSetPanel setPanel = (kctSetPanel)GetProcAddress(dllHandle, "AsKCTLoad");
			//jezeli funkcja istnieje to jest wywolywana
			if(setPanel != NULL) {
				char *fileName = AnsiString(fromFile).c_str();
				char *fileMode = AnsiString("").c_str();
				result = setPanel(socketID, fileName, fileMode);
			}
		} else {
			result = -14;
		}
	} else {
		result = 4;
	}
    return static_cast<kawasakiStatus>(result);
}
//---------------------------------------------------------------------------
//pobranie wersji biblioteki
void kawasakiComm::getVersion(String &verInfo)
{
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		typedef void (*getVer)(char *ver);
		getVer version = (getVer)GetProcAddress(dllHandle, "AsKCTVersion");
		//jezeli funkcja istnieje to jest wywolywana
		if(version != NULL) {
			//rezerwujemy miejsce na odpowiedz
			char *recv = new char[50];
			version(recv);
			//usuwamy stworzona tablice
			delete[] recv;
			//kopiujemy zawartosc do argumentu
			verInfo = recv;
			recv = NULL;
		}
	} else {
		verInfo = "DLL NOT INIT!";
	}
}
//---------------------------------------------------------------------------
//sprawdzenie statusu po realizacji zadania
kawasakiStatus kawasakiComm::getStatus(String &msg)
{
	long result = 0;
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		//sprawdzamy czy jestesmy polaczeni z robotem
		if (connected) {
			typedef long (*kctGetStatus)(long SockID, char *message);
			kctGetStatus getStatus = (kctGetStatus)GetProcAddress(dllHandle, "AsKCTCmdCheck");
			//jezeli funkcja istnieje to jest wywolywana
			if(getStatus != NULL) {
				//rezerwujemy miejsce na odpowiedz
				char *mess = new char[4096];
				result = getStatus(socketID, mess);
				msg = mess;
			}
		} else {
			result = -14;
		}
	} else {
		result = 4;
	}
	return static_cast<kawasakiStatus>(result);
}
//---------------------------------------------------------------------------
//wyslanie odpowiedzi do robota (gdy robot wymaga np. potwierdzenia)
kawasakiStatus kawasakiComm::reply(String &answer)
{
	long result = 0;
	//sprawdzamy czy zostala zainicjalizowana komunikacja
	if (dllHandle && commInit) {
		//sprawdzamy czy jestesmy polaczeni z robotem
		if (connected) {
			typedef long (*kctReply)(long SockID, char *ans);
			kctReply reply = (kctReply)GetProcAddress(dllHandle, "AsKCTReply");
			//jezeli funkcja istnieje to jest wywolywana
			if(reply != NULL) {
				char *answ = AnsiString(answer).c_str();
				result = reply(socketID, answ);
			}
		} else {
			result = -14;
		}
	} else {
		result = 4;
	}
    return static_cast<kawasakiStatus>(result);
}
//---------------------------------------------------------------------------
bool kawasakiComm::isDLLinit()
{
	return commInit;
}
//---------------------------------------------------------------------------
bool kawasakiComm::isConnected()
{
	return connected;
}
//---------------------------------------------------------------------------
void kawasakiComm::updateConnParams(String ip, int port)
{
	ipAddress = ip;
	portNo = port;
}
//---------------------------------------------------------------------------
void kawasakiComm::updateSourceFile(String fileLoc)
{
	sourceFile = fileLoc;
}
//---------------------------------------------------------------------------
//dopasowanie pliku do wymagan robota Kawasaki
bool kawasakiComm::formatFile(String source, String formatted)
{
	ifstream sourceData; ofstream destData;
	string msgOriginal, lastMsg;
	int endPresent = 0;
	//odczytujemy plik
	sourceData.open(source.c_str(), ios::binary);
	destData.open(formatted.c_str(), ios::binary);
	/***** sprawdzamy czy udalo sie odczytac plik ************************/
	if(!sourceData && !sourceData.is_open()) {
		return false;
	}
	/***** odczytujemy linijki pliku az natrafimy na definicje panelu IF */
	do {
		lastMsg = msgOriginal;
		endPresent = lastMsg.find(".END",0);
		getline(sourceData,msgOriginal);
		if (lastMsg == msgOriginal && endPresent != -1) {
			msgOriginal = "";
		} else {
			destData << msgOriginal << "\r";
        }
	} while (msgOriginal != "");
	/***** wiecej nie mamy potrzeby szukac dlatego zamykamy plik *********/
	sourceData.close();
	destData.close();
	//jezeli doszlismy tutaj to jest wszystko ok
	return true;
}
//---------------------------------------------------------------------------
String kawasakiStatusToStr(kawasakiStatus currStatus)
{
	switch (currStatus) {
	case kawasakiStatus::ok:
		return "ALL OK!";
	case kawasakiStatus::param1Error:
		return "Error in parameter 1!";
	case kawasakiStatus::param2Error:
		return "Error in parameter 2!";
	case kawasakiStatus::param3Error:
		return "Error in parameter 3!";
	case kawasakiStatus::socketNok:
		return "Socket creation error!";
	case kawasakiStatus::connFailed:
		return "Connection failed!";
	case kawasakiStatus::disconnFailed:
		return "Disconnection failed!";
	case kawasakiStatus::connInProgress:
		return "Connection in progress!";
	case kawasakiStatus::notConn:
		return "Not connected!";
	case kawasakiStatus::cmdInProgress:
		return "Command in progres!";
	case kawasakiStatus::cmdError:
		return "Command error!";
	case kawasakiStatus::cmdNok:
		return "Command nok!";
	case kawasakiStatus::commError:
		return "Communication error!";
	case kawasakiStatus::cmdInquiry:
		return "Command inquiry!";
	case kawasakiStatus::cmdAborted:
		return "Command aborted!";
	case kawasakiStatus::dllMissing:
		return "DLL not found!";
	case kawasakiStatus::dataDisplayed:
		return "Data displayed!";
	case kawasakiStatus::dataInput:
		return "Data input prompt!";
	case kawasakiStatus::dataContinuous:
		return "Continuous data!";
	case kawasakiStatus::dllNotInit:
		return "DLL not initialized!";
	case kawasakiStatus::dllAlreadyInit:
		return "DLL already initialized!";
	case kawasakiStatus::alreadyConn:
		return "Already connected!";
	case kawasakiStatus::alreadyDisconn:
		return "Already disconnected!";
	default: ;
	}
	return "";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------










USEFORM("tiles\manager\tileProperties.cpp", tilePropertiesWindow);
USEFORM("windows\KPCoptions.cpp", kpcOpt);
USEFORM("windows\KPCmessage.cpp", kpcMsg);
USEFORM("KPC.cpp", mainWindow);
USEFORM("windows\KPCrobot.cpp", KPCtoRobot);
//---------------------------------------------------------------------------
extern "C" int FMXmain()
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TmainWindow), &mainWindow);
		Application->CreateForm(__classid(TtilePropertiesWindow), &tilePropertiesWindow);
		Application->CreateForm(__classid(TkpcMsg), &kpcMsg);
		Application->CreateForm(__classid(TkpcOpt), &kpcOpt);
		Application->CreateForm(__classid(TKPCtoRobot), &KPCtoRobot);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

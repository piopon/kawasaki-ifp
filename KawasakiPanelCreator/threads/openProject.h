//---------------------------------------------------------------------------

#ifndef openProjectH
#define openProjectH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TThreadOpenProj : public TThread
{
private:
	String projFileName;
protected:
	void __fastcall Execute();
	void __fastcall hideLoadIndicator();
public:
	__fastcall TThreadOpenProj(bool CreateSuspended);
	__fastcall TThreadOpenProj(bool CreateSuspended, String fileN);
};
//---------------------------------------------------------------------------
#endif

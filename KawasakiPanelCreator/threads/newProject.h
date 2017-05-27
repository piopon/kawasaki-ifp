//---------------------------------------------------------------------------

#ifndef newProjectH
#define newProjectH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TThreadNewProj : public TThread
{
private:
protected:
	void __fastcall Execute();
	void __fastcall hideLoadIndicator();
public:
	__fastcall TThreadNewProj(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif

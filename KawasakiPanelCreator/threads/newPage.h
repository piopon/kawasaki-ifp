//---------------------------------------------------------------------------

#ifndef newPageH
#define newPageH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TThreadNewPage : public TThread
{
private:
	int pageNo;
protected:
	void __fastcall Execute();
public:
	__fastcall TThreadNewPage(bool CreateSuspended, int pgNo=0);
};
//---------------------------------------------------------------------------
#endif

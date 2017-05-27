//---------------------------------------------------------------------------
#pragma hdrstop
#include "tileManager.h"
#include "zeroTile.h"
#include "pushButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//konstruktor
//---------------------------------------------------------------------------
__fastcall tileManager::tileManager()
{
	callerTile = NULL;
	currEntry = 0;
	currSource = empty;
}
//---------------------------------------------------------------------------
//destruktor
//---------------------------------------------------------------------------
__fastcall tileManager::~tileManager()
{

}
//---------------------------------------------------------------------------
//metody do obslugi zrodel, plikow
//---------------------------------------------------------------------------
void tileManager::update(tileSource source,String fileName,int whichEntry)
{

}
tileSource tileManager::getSource(void)
{
    return this->currSource;
}
void tileManager::getFileInfo(String &fileName, int &entry)
{

}
//---------------------------------------------------------------------------
//metody do obslugi kafelkow
//---------------------------------------------------------------------------
bool tileManager::checkClassZeroTile(TObject *obj)
{
    return false;
}
void tileManager::backupConstParams(void)
{

}
void tileManager::updateTile(int newID)
{

}


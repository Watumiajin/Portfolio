
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// Класс управлением процесса создания файла sln
//-------------------------------------------------------------------
class SlnCreator
{
public:
	SlnCreator(NetControl& NetCont) : netCont(& NetCont) {};
	void Create_File(wstring& mainSlnPath);

private:
	// внесение проектов и связей между проектами
	void Insert_Projects(wstring& mainSlnPath);
	// внесение конфигураций решения
	void Insert_Sln_confs();
	// внесение связей между конфигурациями решения и проектов
	void Insert_Prj_confs();
	
	wstring Revision_Path(wstring path, wstring mainSlnPath);
	void Add_Buf(wstring buf, wstring type, wstring confName, bool build);

private:
	// классы
	NetControl *netCont; // сеть

	// поток вывода
	WriteStream fout;

	// буферы
	Qwstring debugBuf;   // сборки debug
	Qwstring releaseBuf; // сборки release
};
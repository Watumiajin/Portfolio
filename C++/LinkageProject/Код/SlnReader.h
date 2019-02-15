
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// Класс для чтения файлов sln
//-------------------------------------------------------------------
class SlnReader
{
public:
	SlnReader(NetControl& NetCont, XmlReader& Xreader) :
		netCont(& NetCont), xreader(& Xreader) {};
	~SlnReader();
	void Read_SlnFiles(VsharSln& slnFiles, int& allPrj, int& allConf);

private:
	void Project_Section(wstring slnPath, SharSlnPrj& selectPrj, size_t poz, int& sumPrj);
	void DepConf_Section(SharSlnPrj& selectPrj, size_t poz, int& sumConf);
	void  Global_Section(size_t poz);
	void     End_Section(SharSlnPrj& selectPrj, size_t poz);

	wstring Revision_Path(wstring path);
	// анализ локальных связей между проектами
	void Indef_Possible_Depend();
	// анализ слабых связей
	void Indef_Weak_Depend();

private:
	// классы
	NetControl *netCont; // сеть
	XmlReader  *xreader; // чтение vcxproj

	// разбор строки
	wchar_t *bufFile; // буфер строки из файла
	wstring bufVal;     // буфер подстроки из строки
	wstring path;       // путь читаемого файла
	
	// флаги состояний
	bool addDep = false;  // добавление зависимости
	bool addConf = false; // добавление сборки

	//
	ReadStream readFile; // поток чтения файла
	map<wstring, int> targetConf; // cборки которые необходимо включить и их приоритет
	Swstring confNotExists; // не найденные сборки в sln

	map<wstring, SharSlnPrj> slnProjects; // проекты и сборки в читаемом sln

};

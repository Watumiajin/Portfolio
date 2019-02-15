
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// Класс для чтения файлов xml
//-------------------------------------------------------------------
class XmlReader
{
public:
	XmlReader(NetControl& NetCont) :
		netCont(& NetCont), pFileStream(NULL), pReader(NULL) {}
	~XmlReader();

	// чтение файлов vcxprj
	void Read_PrjFiles(StkPrjFile& prjFiles);

	// чтение файла конфигурации
	bool Read_Config();

public:
	//std::set<Sline> completePath;

private:
	void Get_Next(LPCWSTR& localName, XmlNodeType& nodeType, LPCWSTR& nodeValue);

	void Read_Attributes(LPCWSTR wanted, wstring& bufval);
	bool Read_Values(LPCWSTR node, wstring& bufval);

	bool Test_Arg();
	wstring Minimaz_ConfName(wstring name);

	void Filling_Project();

private:
	// классы и структуры
	NetControl *netCont; // сеть

	// чтение файла
	IStream *pFileStream; // поток чтения файла
	IXmlReader *pReader;  // прочитанный файл
	wstring path;           // путь читаемого файл

	// создание проекта
	SharPrj selectProject; // заполняемый проект
	SharPrjConf bufconf;   // сборка в заполняемом проекте

};

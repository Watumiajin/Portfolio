
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// структура файла проекта
//-------------------------------------------------------------------
struct PrjFile
{
	PrjFile(wstring Path, wstring Name, wstring GUID) : path(Path), name(Name), typeGUID(GUID) {};

	wstring path;     // путь
	wstring name;     // имя
	wstring typeGUID; // тип проекта
};


//-------------------------------------------------------------------
// структура файла sln
//-------------------------------------------------------------------
struct SlnFile
{
	wstring path; // путь
	map<wstring, int> targetConf; // имя сборки <-> приоритет
	Swstring allConf; // все сборки для добавления

};

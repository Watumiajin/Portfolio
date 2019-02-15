
#pragma once
#include "stdafx.h"

//-------------------------------------------------------------------
// Класс управления процессом поиска файлов
//-------------------------------------------------------------------
class FileFinder
{
public:
	// рекурсивный метод поиска файлов
	void Find_Files(LPCWSTR root, Excluds& exc);

public:
	StkPrjFile prjFiles; // найденные файлы

private:
	size_t Cut_Ext(const wstring st);

};


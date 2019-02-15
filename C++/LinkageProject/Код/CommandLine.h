
#pragma once
#include "stdafx.h"

//-------------------------------------------------------------------
// Класс для анализа командной строки
//-------------------------------------------------------------------
class CommandLine
{
public:
	CommandLine();
	bool Test_Arg();

public:
	// аргументы командной строки
	bool readOp; // режим чтения
	bool outOp;  // режим вывода
	Sline path;  // путь поиска
	Sline o;     // путь вывода
	Sline sln;   // исходная строка с доп. зав.

	//
	VsharSlnFiles slnFiles; // файлы sln которые нужно прочесть

private:
	bool Sln_Parsing();
	size_t Conf_Identification(std::shared_ptr<SlnFile> &selectFile, Sline &bufConf);

};
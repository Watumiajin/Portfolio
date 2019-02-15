
#include "stdafx.h"

//-------------------------------------------------------------------
// Класс для анализа командной строки
//-------------------------------------------------------------------

CommandLine::CommandLine()
{
	CCmdLineParser parser(GetCommandLineW());
	LPCTSTR buf;
	outOp = false;
	readOp = false;

	// определение режима чтения и режима вывода
	buf = parser.GetVal(OPT);
	if (buf)
	{
		Sline bufOp = buf;
		size_t poz;

		poz = bufOp.find(L'1');
		if (poz != Sline::npos)
		{
			readOp = true;
		}

		poz = bufOp.find(L'+');
		if (poz != Sline::npos)
		{
			outOp = true;
		}
	}

	if (readOp)
	{
		buf = parser.GetVal(PATH);
		if (buf)
		{
			path = buf;
		}
	}

	buf = parser.GetVal(O);
	if (buf)
	{
		o = buf;
	}

	buf = parser.GetVal(SLN);
	if (buf)
	{
		sln = buf;
	}
}

//-------------------------------------------------------------------

bool CommandLine::Test_Arg()
{
	// проверка пути поиска
	if (readOp)
	{
		if (path == L"")
		{
			Error er(PATH);
			return false;
		}

		if (outOp)
		{
			DISPOUT << L"### Путь поиска - " << path << DISPEND;
		}
	}

	// проверка пути вывода
	if (o == L"")
	{
		Error er(O);
		return false;
	}

	if (outOp)
	{
		DISPOUT << L"### Путь вывода - " << o << DISPEND;
	}

	// проверка дополнительных зависимостей
	if (sln == L"")
	{
		if (!readOp)
		{
			Error er(SLN);
			return false;
		}

		return true;
	}

	// проверка ввода доп. зав.
	if (!Sln_Parsing())
	{
		Error er(SLN);
		return false;
	}

	if (outOp)
	{
		DISPOUT << L"### Дополнительные зависимости:" << L"\n";
		for (auto &File : slnFiles)
		{
			DISPOUT << L" -- файл - " << File->path << L"\n";
			for (auto &conf : File->targetConf)
			{
				DISPOUT << L"  - целевая сборка - " << conf.first << L"\n";
			}
		}
	}

	return true;
}

bool CommandLine::Sln_Parsing()
{
	//  "<файл с доп. зав.[требуемые сборки] <...[...] ..."

	size_t poz = 0;
	size_t sumS = 0;

	while (sln.size() > poz)
	{
		// поиск начала проекта
		poz = sln.find(L'<', poz);
		if (poz == Sline::npos)
		{
			if (!slnFiles.empty())
			{
				return true;
			}

			return false;
		}
		poz++;

		std::shared_ptr<SlnFile> selectFile = std::make_shared<SlnFile>();

		// поиск конца пути проекта
		sumS = sln.find(L'[', poz);
		if (sumS == Sline::npos)
		{
			return false;
		}
		sumS -= poz;
		Sline slnPath = sln.substr(poz, sumS);
		if (slnPath == L"")
		{
			continue;
		}
		selectFile->path = slnPath;
		poz += sumS + 1;

		// подсчёт символов в секции сборок
		size_t сonfSize = sln.find(L']', poz);
		if (сonfSize == Sline::npos)
		{
			return false;
		}

		Sline bufConf = sln.substr(poz, сonfSize - poz);
		if (bufConf == L"")
		{
			return false;
		}

		poz += Conf_Identification(selectFile, bufConf); //;pozConf
		slnFiles.insert(slnFiles.end(), selectFile);
	}

	return true;
}

size_t CommandLine::Conf_Identification(std::shared_ptr<SlnFile> &selectFile, Sline &bufConf)
{
	size_t pozConf = 0;
	std::map<Sline, int> priorityConf;

	// анализ требуемых сборок
	while (bufConf.size() > pozConf)
	{
		size_t sumS = bufConf.find(L';', pozConf);
		if (sumS == Sline::npos)
		{
			sumS = bufConf.size();
		}

		// определение типа и разрядности сборки
		sumS -= pozConf;
		Sline readConf = bufConf.substr(pozConf, sumS);
		Sline minConf;
		
		size_t poz = readConf.find(L"Debug");
		if (poz != std::string::npos)
		{
			minConf += L"Debug|";
		}

		poz = readConf.find(L"Release");
		if (poz != std::string::npos)
		{
			minConf += L"Release|";
		}

		poz = readConf.find(L"|");
		poz++;
		if (poz != std::string::npos)
		{
			minConf += readConf.substr(poz);
		}

		// добавление сборки и определение приоритета
		priorityConf[minConf] = priorityConf[minConf]++;

		selectFile->targetConf[bufConf.substr(pozConf, sumS)] = priorityConf[minConf];
		pozConf += sumS + 1;
	}

	return pozConf;
}

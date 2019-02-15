// LinkageProject.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

int _tmain ()
{
	// настройка локализации
	setlocale(LC_ALL, APP_LOCALE);

	CCmdLineParser cmdLine (GetCommandLineW());
	LPCTSTR buf = cmdLine.GetVal(L"c");
	Config config;

	if (buf == NULL)
	{
		Error(L"", 0);
		return 1;
	}
	
	else
	{
		config.confPath = buf;
	}

	// управляющие классы
	FileFinder finder;
	NetControl netCont(config);
	XmlReader xmlReader(netCont);
	SlnReader slnReader(netCont, xmlReader);
	SlnCreator slnCreater(netCont);

	// читаем файл конфигурации
	if (!xmlReader.Read_Config())
	{
		return 1;
	}

	wstring mainSlnPath = config.outPath.substr(0, config.outPath.rfind(L"\\") + 1); // путь к главному файлу sln

	// выполнение
	try
	{
		int sumPrj = 0;
		int sumConf = 0;

		// поиск проектов по файлу sln и их чтение
		// -----------------------------------------------------

		DISPOUT << L"\n### Поиск проектов по sln:" << DISPEND;

		slnReader.Read_SlnFiles(config.slnFiles, sumPrj, sumConf);

		// поиск проектов по указанным директориям и их чтение
		// -----------------------------------------------------

		DISPOUT << L"\n### Поиск проектов по директориям:" << DISPEND;

		int countPrj = 0;

		for (auto& path : config.searchPaths)
		{
			if (path == L"")
			{
				DISPOUT << L" -- путь - не задан"  DISPEND;
				continue;
			}

			else
			{
				DISPOUT << L" -- " << path << DISPEND;
			}

 			finder.Find_Files(path.c_str(), config.excluds);
			countPrj = finder.prjFiles.size() - countPrj;
			sumPrj += countPrj;

			if (config.verb)
			{
				DISPOUT << L"  - найдено - " << abs(countPrj) << DISPEND;
			}
		}

		xmlReader.Read_PrjFiles(finder.prjFiles);

		// вывод основной информации по проектам и сборкам
		// -----------------------------------------------------

		DISPOUT << L"\n### Всего найдено: "
				<< L"\n -- проектов: " << sumPrj
				<< L"\n -- сборок: " << sumConf << DISPEND;

		// сортировка сети и выгрузка в выходной файл
		// -----------------------------------------------------

		if (netCont.projects.size())
		{
 			netCont.Sorting();

			DISPOUT << L"\n### Генерация решения:" << DISPEND;

 			slnCreater.Create_File(config.outPath);
		}

		else
		{
			Error(L"", 1);
		}
	}

	catch (int error)
	{
		if (error > -1) Error(L"", error);
		return 1;
	}
	catch (...)
	{
		Error(L"", 2);
		return 1;
	}

	return 0;
}

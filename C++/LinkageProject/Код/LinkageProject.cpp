// LinkageProject.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"

int _tmain ()
{
	// ��������� �����������
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

	// ����������� ������
	FileFinder finder;
	NetControl netCont(config);
	XmlReader xmlReader(netCont);
	SlnReader slnReader(netCont, xmlReader);
	SlnCreator slnCreater(netCont);

	// ������ ���� ������������
	if (!xmlReader.Read_Config())
	{
		return 1;
	}

	wstring mainSlnPath = config.outPath.substr(0, config.outPath.rfind(L"\\") + 1); // ���� � �������� ����� sln

	// ����������
	try
	{
		int sumPrj = 0;
		int sumConf = 0;

		// ����� �������� �� ����� sln � �� ������
		// -----------------------------------------------------

		DISPOUT << L"\n### ����� �������� �� sln:" << DISPEND;

		slnReader.Read_SlnFiles(config.slnFiles, sumPrj, sumConf);

		// ����� �������� �� ��������� ����������� � �� ������
		// -----------------------------------------------------

		DISPOUT << L"\n### ����� �������� �� �����������:" << DISPEND;

		int countPrj = 0;

		for (auto& path : config.searchPaths)
		{
			if (path == L"")
			{
				DISPOUT << L" -- ���� - �� �����"  DISPEND;
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
				DISPOUT << L"  - ������� - " << abs(countPrj) << DISPEND;
			}
		}

		xmlReader.Read_PrjFiles(finder.prjFiles);

		// ����� �������� ���������� �� �������� � �������
		// -----------------------------------------------------

		DISPOUT << L"\n### ����� �������: "
				<< L"\n -- ��������: " << sumPrj
				<< L"\n -- ������: " << sumConf << DISPEND;

		// ���������� ���� � �������� � �������� ����
		// -----------------------------------------------------

		if (netCont.projects.size())
		{
 			netCont.Sorting();

			DISPOUT << L"\n### ��������� �������:" << DISPEND;

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


#include "stdafx.h"

//-------------------------------------------------------------------
// ����� ��� ������� ��������� ������
//-------------------------------------------------------------------

CommandLine::CommandLine()
{
	CCmdLineParser parser(GetCommandLineW());
	LPCTSTR buf;
	outOp = false;
	readOp = false;

	// ����������� ������ ������ � ������ ������
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
	// �������� ���� ������
	if (readOp)
	{
		if (path == L"")
		{
			Error er(PATH);
			return false;
		}

		if (outOp)
		{
			DISPOUT << L"### ���� ������ - " << path << DISPEND;
		}
	}

	// �������� ���� ������
	if (o == L"")
	{
		Error er(O);
		return false;
	}

	if (outOp)
	{
		DISPOUT << L"### ���� ������ - " << o << DISPEND;
	}

	// �������� �������������� ������������
	if (sln == L"")
	{
		if (!readOp)
		{
			Error er(SLN);
			return false;
		}

		return true;
	}

	// �������� ����� ���. ���.
	if (!Sln_Parsing())
	{
		Error er(SLN);
		return false;
	}

	if (outOp)
	{
		DISPOUT << L"### �������������� �����������:" << L"\n";
		for (auto &File : slnFiles)
		{
			DISPOUT << L" -- ���� - " << File->path << L"\n";
			for (auto &conf : File->targetConf)
			{
				DISPOUT << L"  - ������� ������ - " << conf.first << L"\n";
			}
		}
	}

	return true;
}

bool CommandLine::Sln_Parsing()
{
	//  "<���� � ���. ���.[��������� ������] <...[...] ..."

	size_t poz = 0;
	size_t sumS = 0;

	while (sln.size() > poz)
	{
		// ����� ������ �������
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

		// ����� ����� ���� �������
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

		// ������� �������� � ������ ������
		size_t �onfSize = sln.find(L']', poz);
		if (�onfSize == Sline::npos)
		{
			return false;
		}

		Sline bufConf = sln.substr(poz, �onfSize - poz);
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

	// ������ ��������� ������
	while (bufConf.size() > pozConf)
	{
		size_t sumS = bufConf.find(L';', pozConf);
		if (sumS == Sline::npos)
		{
			sumS = bufConf.size();
		}

		// ����������� ���� � ����������� ������
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

		// ���������� ������ � ����������� ����������
		priorityConf[minConf] = priorityConf[minConf]++;

		selectFile->targetConf[bufConf.substr(pozConf, sumS)] = priorityConf[minConf];
		pozConf += sumS + 1;
	}

	return pozConf;
}

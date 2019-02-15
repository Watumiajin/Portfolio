
#include "stdafx.h"

//-------------------------------------------------------------------
// ����� ����������� �������� �������� ����� sln
//-------------------------------------------------------------------

void SlnCreator::Create_File(wstring& mainSlnPath)
{
	// ���������� ��������� ������
	std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	fout.open(mainSlnPath, std::ios::in | std::ios::trunc);
	fout.imbue(utf8_locale);

	if (!fout.fail())
	{
		Insert_Projects(mainSlnPath);
		Insert_Sln_confs();
		Insert_Prj_confs();
	}

	else
	{
		Error(mainSlnPath, 20);
	}

	fout.close(); // ������� ����� ��������� �����
}

// �������� �������� � ������ ����� ���������
void SlnCreator::Insert_Projects(wstring& mainSlnPath)
{
	fout << STR_DECLARATION;

	for (auto& prj : netCont->projects)
	{
		// � ������� ���� ������?
		if (prj->configs.empty())
		{
			Error(prj->prjName, 23);
			continue;
		}

		fout << STR_GUIDTYPEPRJ << L"\"" << prj->GUIDtype << L"\") = "
			 << L"\"" << prj->prjName << L"\", "
			 << L"\"" << Revision_Path(prj->prjPath, mainSlnPath) << L"\", "
			 << L"\"" << prj->GUIDprj << L"\"";

		if (!prj->allDepends.empty())
		{
			fout << STR_PRJSECPRJDEP;

			for (auto& dep : prj->allDepends)
			{
				if (dep.second->GUIDprj != prj->GUIDprj)
				{
					fout << L"\n\t\t" << dep.second->GUIDprj << L" = " << dep.second->GUIDprj;
				}
			}

			fout << STR_ENDPRJSEC;
		}

		fout << STR_ENDPRJ;
	}
}

// �������� ������������ �������
void SlnCreator::Insert_Sln_confs()
{
	fout << STR_CLOBAL << STR_CLBSECSOLCONFPLF;

	for (auto& net : netCont->net)
	{
		for (int num = 1; num <= net.steps; num++)
		{
			wstring conf = net.type + L" Step " + std::to_wstring(num) + L"|" + net.cap;
			fout << L"\n\t\t" << conf << L" = " << conf;
		}
	}

	fout << STR_ENDCLOBSEC;
}

// �������� ������ ����� �������������� ������� � ��������
void SlnCreator::Insert_Prj_confs()
{
	/*
		{GUID �������}.���_������ ���_������|�����������_������.ActiveCfg - ���������
		{GUID �������}.���_������ ���_������|�����������_������.Build.0   - ��������

		������ ����������� �� ���� ������, �� ���������� ������ �� ����� (�� ������)
		������ �� ����� ��������� �� �� ������ �� ����������� ����, �� ������ ��������� ������
		-- ���� ������ ����� ���� ��� ����������� ����, ����� ������

		��� �#-��������, ��� ����������� ������ ���� �������� � ������ �� ��� �� ����,
		��� � ������ csproj, ���� ���� ��� ��� ���������� �� ���������� ����,
		����� ����� ������ ������
	*/

	fout << STR_CLBSECPRJCONFPLF;

	for (auto& prj : netCont->projects)
	{
		// ������� ������ ��������
		if (prj->configs.empty()) continue;

		// ������ ������ � ������������� �� �����
		for (auto& net : netCont->net)
		{
			if (net.vertexs_unsorted.empty()) continue;

			// �������� ��� ��� �� ���� (������� ������ � ����� ������������ � �����)
			bool build = false;

			std::vector<SharPrjConf> configs; // ������ ����� ���� � �����������

			// ����� ������ ��� ���� ����
			for (auto& conf : prj->configs)
			{
				if (conf->release == net.Release && conf->win32 == net.Win32)
				{
					configs.push_back(conf);
					build = true;
				}
			}

			if (configs.empty())
			{
				configs.push_back(prj->configs[0]);
			}

			// ���������� �������
			for (int step = 1; step <= netCont->maxSteps + 1; step++)
			{
				bool collect = false;

				if (net.steps >= step)
				{
					wstring buf = L"\n\t\t" + prj->GUIDprj + L"." +
						net.type + L" Step " + std::to_wstring(step) + L"|" + net.cap;

					for (auto& conf : configs)
					{
						// ������� � �������
						if (conf->stepsBuild.count(step))
						{
							collect = true;
							Add_Buf(buf, net.type, conf->confName, build);
						}
					}

					// ������ ������� (�� ���� ���� �� ����������)
					if (!collect)
					{
						Add_Buf(buf, net.type, configs[0]->confName, false);
					}
				}
			}

			// ������ � �������� ����
			while (!debugBuf.empty())
			{
				fout << debugBuf.front();
				debugBuf.pop();
			}

			while (!releaseBuf.empty())
			{
				fout << releaseBuf.front();
				releaseBuf.pop();
			}
		}
	}

	fout << STR_ENDCLOBSEC << STR_CLBSECSOLPROP << STR_ENDCLOBSEC << STR_ENDCLOBAL;
}

//-----------------------------------------------------------------

wstring SlnCreator::Revision_Path(wstring path, wstring mainSlnPath)
{
	wchar_t szOut[MAX_PATH] = L"";

	PathRelativePathTo(szOut, mainSlnPath.c_str(), 0, path.c_str(), 0);

	return szOut;
}

void SlnCreator::Add_Buf(wstring buf, wstring type, wstring confName, bool build)
{
	if (type == L"Debug")
	{
		debugBuf.push(buf + L".ActiveCfg = " + confName);
		if (build) debugBuf.push(buf + L".Build.0 = " + confName);
	}
	else
	{
		releaseBuf.push(buf + L".ActiveCfg = " + confName);
		if (build) releaseBuf.push(buf + L".Build.0 = " + confName);
	}
}


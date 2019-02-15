
#include "stdafx.h"

//-------------------------------------------------------------------
// Класс управлением процесса создания файла sln
//-------------------------------------------------------------------

void SlnCreator::Create_File(wstring& mainSlnPath)
{
	// найстройка выходного потока
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

	fout.close(); // закрыть поток выходного файла
}

// внесение проектов и связей между проектами
void SlnCreator::Insert_Projects(wstring& mainSlnPath)
{
	fout << STR_DECLARATION;

	for (auto& prj : netCont->projects)
	{
		// у проекта есть сборки?
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

// внесение конфигураций решения
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

// внесение связей между конфигурациями решения и проектов
void SlnCreator::Insert_Prj_confs()
{
	/*
		{GUID проекта}.тип_сборки шаг_сборки|разрядность_сборки.ActiveCfg - описывает
		{GUID проекта}.тип_сборки шаг_сборки|разрядность_сборки.Build.0   - собирает

		сборка описывается во всех этапах, но собирается только на одном (не всегда)
		сборка не может собраться на не равном по разрядности шаге, но должна описаться другой
		-- если сборок этого типа или разрядности нету, брать другие

		Для С#-проектов, все зависимости должны быть включены в сборку на том же шаге,
		что и сборка csproj, даже если они уже собирались на предыдущем шаге,
		иначе будет ошибка сборки
	*/

	fout << STR_CLBSECPRJCONFPLF;

	for (auto& prj : netCont->projects)
	{
		// пропуск пустых проектов
		if (prj->configs.empty()) continue;

		// анализ сборок и распределение их сетям
		for (auto& net : netCont->net)
		{
			if (net.vertexs_unsorted.empty()) continue;

			// собирать или нет на шаге (наличие сборок с такой разрядностью и типом)
			bool build = false;

			std::vector<SharPrjConf> configs; // сборки этого типа и разрядности

			// поиск сборок для этой сети
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

			// наполнение буферов
			for (int step = 1; step <= netCont->maxSteps + 1; step++)
			{
				bool collect = false;

				if (net.steps >= step)
				{
					wstring buf = L"\n\t\t" + prj->GUIDprj + L"." +
						net.type + L" Step " + std::to_wstring(step) + L"|" + net.cap;

					for (auto& conf : configs)
					{
						// собрать и описать
						if (conf->stepsBuild.count(step))
						{
							collect = true;
							Add_Buf(buf, net.type, conf->confName, build);
						}
					}

					// только описать (на этом шаге не собиралась)
					if (!collect)
					{
						Add_Buf(buf, net.type, configs[0]->confName, false);
					}
				}
			}

			// запись в выходной файл
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



#include "stdafx.h"

//-------------------------------------------------------------------
// Класс для чтения файлов sln
//-------------------------------------------------------------------

SlnReader::~SlnReader()
{
	bufFile = NULL;

	if (readFile)
	{
		readFile.close();
	}
}

//-------------------------------------------------------------------

void SlnReader::Read_SlnFiles(VsharSln& slnFiles, int& allPrj, int& allConf)
{
	bufFile = new wchar_t[BUFSIZE];

	for (auto& File : slnFiles)
	{
		slnProjects.clear();
		SharSlnPrj selectProject = NULL;   // читаемый проект
		StkPrjFile prjFiles;
		path = File->path;

		int sumPrj = 0;  // число прочитанных проектов
		int sumConf = 0; // число добавленных сборок

		DISPOUT << L"\n -- " << path << DISPEND;

		readFile.open(path, std::ios::in);
		targetConf = File->targetConf;
		confNotExists = File->allConf;

		wstring readSlnPath = path.substr(0, path.rfind(L"\\") + 1);

		if (!readFile)
		{
			Error(path, 6);
			readFile.close();
			continue;
		}

		// чтение файла
		while (!readFile.eof())
		{
			readFile.getline(bufFile, BUFSIZE);
			bufVal = wstring(bufFile);

			size_t poz = 0; // позиция в строке

			// разбор строки
			while (poz < bufVal.size())
			{
				wchar_t selectChar = bufVal[poz];

				if (selectChar == L'\t')
				{
					poz++;
					continue;
				}

				switch (selectChar)
				{
					// секция проекта
					case L'P': Project_Section(readSlnPath, selectProject, poz, sumPrj); break;
					// секция зависимостей и сборок
					case L'{': DepConf_Section(selectProject, poz, sumConf); break;
					// глобальная секция
					case L'G': Global_Section(poz); break;
					// конец секции
					case L'E': End_Section(selectProject, poz); break;

					default: break;
				}

				break;
			}
		}
		
		// дополнительная информация
		if (netCont->config->verb)
		{
			DISPOUT << L"  - найдено проектов: " << slnProjects.size()
					<< L", из них новых: " << sumPrj << DISPEND;
		}

		allPrj += sumPrj;
		allConf += sumConf;

		// отбор всех прочитанных проектов в sln
		for (auto &slnPrj : slnProjects)
		{
			PrjFile filePrj(slnPrj.second->prj->prjPath, slnPrj.second->prj->prjName, slnPrj.second->prj->GUIDtype);
			prjFiles.push(filePrj);
		}

		// дополнительно прочитать файлы проектов
		if (!prjFiles.empty())
		{
			xreader->Read_PrjFiles(prjFiles);
		}

		Indef_Possible_Depend();
		Indef_Weak_Depend();

		// проверка на присутствие сборок
		for (auto confName : confNotExists)
			Error(confName, 24);

		readFile.close();
	}
}

void SlnReader::Project_Section(wstring slnPath, SharSlnPrj& selectProject, size_t poz, int& sumPrj)
{
	wstring word;

	if (bufVal.size() > poz + 8)
	{
		word = bufVal.substr(poz, 8);
		// новый проект
		if (word == L"Project(")
		{
			try
			{
				size_t sumS;
				poz += 9;

				SharPrj newProject = std::make_shared<Project>();

				// определение типа GUID
				sumS = bufVal.find(L'\"', poz);
				if (sumS == wstring::npos) throw (7);
				sumS -= poz;
				newProject->GUIDtype = bufVal.substr(poz, sumS);
				poz += sumS + 6;

				// определение имени проекта
				sumS = bufVal.find(L'\"', poz);
				if (sumS == wstring::npos) throw (7);
				sumS -= poz;
				newProject->prjName = bufVal.substr(poz, sumS);
				poz += sumS + 4;

				// определение пути проекта
				sumS = bufVal.find(L'\"', poz);
				if (sumS == wstring::npos) throw (7);
				sumS -= poz;
				wstring newPath = bufVal.substr(poz, sumS);
				poz += sumS + 4;
				newProject->prjPath = Revision_Path(slnPath + newPath);

				// определение GUID проекта
				sumS = bufVal.find(L'\"', poz);
				if (sumS == wstring::npos) throw (7);
				sumS -= poz;
				newProject->GUIDprj = bufVal.substr(poz, sumS);

				// проверка на исключение по типу проекта
				if (netCont->config->excluds.typeGUID.count(newProject->GUIDtype))
				{
					if (netCont->config->verb)
					{
						DISPOUT << L"  - исключён (typeGuid) - " << newProject->prjName << DISPEND;
					}
					netCont->config->excluds.prjGUID.insert(newProject->GUIDprj);
					selectProject = NULL;
					return;
				}

				// проверка на исключение по GUID проекта
				if (netCont->config->excluds.prjGUID.count(newProject->GUIDprj))
				{
					selectProject = NULL;
					return;
				}

				// проверка уникальности пути, имени и guid
				int  p = netCont->prjPaths.count(newProject->prjPath),
					 n = netCont->prjNames.count(newProject->prjName),
					 g = netCont->prjGUIDs.count(newProject->GUIDprj);

				if (p || n || g)
				{
					if (netCont->prjPaths[newProject->prjPath] == netCont->prjNames[newProject->prjName] &&
						netCont->prjGUIDs[newProject->GUIDprj] == netCont->prjNames[newProject->prjName])
					{
						selectProject = std::make_shared<SlnProject>(netCont->prjPaths[newProject->prjPath]);
						slnProjects.insert(std::make_pair(selectProject->prj->GUIDprj, selectProject));
						return;
					}

					Error(L"", newProject, p, n, g); // код ошибки: 8

					return;
				}

				// такого проекта не сушествует и путь-имя-guid уникальны, добавляем
				netCont->projects.push_back(newProject);
				netCont->prjPaths[newProject->prjPath] = newProject;
				netCont->prjNames[newProject->prjName] = newProject;
				netCont->prjGUIDs[newProject->GUIDprj] = newProject;

				sumPrj++;
				selectProject = std::make_shared<SlnProject>(newProject);
				slnProjects.insert(std::make_pair(selectProject->prj->GUIDprj, selectProject));
				return;
			}

			catch (int error)
			{
				Error(L"", error);
			}
		}

		// начало узла зависимостей
		if (word == L"ProjectS" && bufVal.size() >= poz + 35 && selectProject)
		{
			word = bufVal.substr(poz, 35);

			if (word == L"ProjectSection(ProjectDependencies)")
			{
				addDep = true;
			}
		}
	}
}

void SlnReader::DepConf_Section(SharSlnPrj& selectProject, size_t poz, int& sumConf)
{
	// новая зависимость
	if (addDep)
	{
		if (!selectProject)
		{
			Error(bufVal, 9);
			return;
		}

		size_t sumS = bufVal.find(L' ', poz);
		if (sumS == wstring::npos)
		{
			Error(bufVal, 10);
		}
		sumS -= poz;

		selectProject->possibleDepends.insert(bufVal.substr(poz, sumS));

		return;
	}

	// новая сборка
	if (addConf)
	{
		// добавлять только Build.0
		// #########################

		wstring guid;
		wstring confName; // имя сборки в проекте
		wstring slnName;  // имя сборки в sln

		size_t sumS_1;
		size_t sumS_2;

		// определение GUID
		sumS_1 = bufVal.find(L'.', poz);
		if (sumS_1 == wstring::npos) Error(bufVal, 11);
		sumS_1 -= poz;
		guid = bufVal.substr(poz, sumS_1);

		// проверка на исключение по guid
		if (netCont->config->excluds.prjGUID.count(guid)) return;

		// поиск проекта по guid
		if (!selectProject || selectProject->prj->GUIDprj != guid)
		{
			if (!slnProjects.count(guid))
			{
				if (!netCont->prjGUIDs.count(guid)) Error(bufVal, 9);

				return;
			}

			selectProject = slnProjects[guid];
		}

		// определение имени сборки в sln
		sumS_2 = bufVal.find(L'=', poz);
		if (sumS_2 == wstring::npos) Error(bufVal, 11);
		sumS_2 -= sumS_1 + 3;
		slnName = bufVal.substr(sumS_1 + 3, sumS_2);

		// определение признака активной сборки (Build)
		sumS_2 = slnName.find(L'.', poz);
		if (sumS_2 == wstring::npos) Error(bufVal, 11);
		if (slnName.substr(sumS_2 + 1) != L"Build.0 ") return; // пропуск неактивных

		slnName = slnName.substr(0, sumS_2);

		// определение реального имени сборки
		poz = bufVal.find(L'=', poz);
		if (poz == wstring::npos || bufVal.size() < poz + 2) Error(bufVal, 11);
		sumS_1 = bufVal.size() - poz;
		poz += 2;
		confName = bufVal.substr(poz, sumS_1);

		int prioryty = 0;

		if (!targetConf.empty())
		{
			prioryty = targetConf[slnName];

			// если сборки указаны и данной сборки нет, то пропускаем
			if (!prioryty) return;
		}

		bool ok = selectProject->Add_Conf(confName, slnName, prioryty, netCont->config->excluds.confName, sumConf);
		if (ok && !confNotExists.empty()) confNotExists.erase(slnName);
	}
}

void SlnReader::Global_Section(size_t poz)
{
	wstring word;

	// не тот узел
	if (bufVal.size() < 44 + poz) return;

	size_t sumS = bufVal.find(L')', poz);
	if (sumS == wstring::npos)
	{
		Error(bufVal, 12);
		return;
	}
	word = bufVal.substr(poz, sumS);
	poz += sumS;

	// новая сборка
	if (word == L"GlobalSection(ProjectConfigurationPlatforms)")
	{
		addConf = true;
	}
}

void SlnReader::End_Section(SharSlnPrj& selectProject, size_t poz)
{
	wstring word = bufVal.substr(poz);

	// секция проекта
	if (word == L"EndProjectSection")
	{
		addDep = false;
		selectProject = NULL;
	}
				
	// глобальная секция
	if (word == L"EndGlobalSection")
	{
		addConf = false;
	}
}

//-------------------------------------------------------------------

wstring SlnReader::Revision_Path(wstring oldPath)
{
	wstring newPath;
	newPath.resize(oldPath.size() + 1);

	TCHAR *pPart = NULL;
	DWORD newPathSize = GetFullPathName(oldPath.c_str(), (DWORD)newPath.size(), &newPath[0], &pPart);

	if (newPathSize > newPath.size())
	{
		newPath.resize(newPathSize + 1);
		newPathSize = GetFullPathName(oldPath.c_str(), (DWORD) newPath.size(), &newPath[0], &pPart);
	}

	if (0 == newPathSize)
	{
		return wstring();
	}
	
	newPath.resize(newPathSize);

	return newPath;
}

// анализ локальных связей между проектами
void SlnReader::Indef_Possible_Depend()
{
	for (auto& slnPrj : slnProjects)
	{
		// выгрузка зависимостей из ProjectReferences
		for (auto& dep : slnPrj.second->prj->prjReference)
		{
			slnPrj.second->possibleDepends.insert(dep);
		}

		slnPrj.second->prj->prjReference.clear();

		wstring additionalDep = L"";  // строка сильных зависимостей для c# проектов

		for (auto& dep : slnPrj.second->possibleDepends)
		{
			if (netCont->prjGUIDs.count(dep))
			{
				additionalDep += netCont->prjGUIDs[dep]->prjName + L".lib;";

				slnPrj.second->prj->allDepends.insert(std::make_pair(dep, netCont->prjGUIDs[dep]));
				slnPrj.second->weakDepends.insert(slnProjects[dep]);
			}
		}

		if (slnPrj.second->prj->GUIDtype == GUIDCSPROJ)
		{
			//
			// c++ получают свой вариант зависимостей из vcxprj, для каждой своей сборки
			// с# проекты имеют другой принцип и эта часть только для них
			//
		
			std::transform(additionalDep.begin(), additionalDep.end(), additionalDep.begin(),
				[](wchar_t ch) { return wchar_t(tolower(ch)); });
		
		
			for (auto& conf : slnPrj.second->prj->configs)
			{
				conf->additionalDependencies += additionalDep;
			}
		}
	}
}

// анализ слабых связей
void SlnReader::Indef_Weak_Depend()
{
	for (auto &slnPrj : slnProjects)
	{
		for (auto &conf : slnPrj.second->configs)
		{
			for (auto &dep : slnPrj.second->weakDepends)
			{
				for (auto &depConf : dep->configs)
				{
					if (depConf->sequence <= conf->sequence &&
						depConf->release == conf->release &&
						depConf->win32 == conf->win32)
					{
						conf->weakDepends.insert(dep->prj->prjName + L"." + depConf->confName);
					}
				}
			}
		}
	}
}
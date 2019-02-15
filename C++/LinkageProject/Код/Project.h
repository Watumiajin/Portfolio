
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// Структура сборки
//-------------------------------------------------------------------
struct ProjectСonf
{
	/*
		расположение параметров в узлах файла vcxproj

		сonfName;				->> ProjectConfiguration(label = Include)
		сonfigurationType;		->> PropertyGroup->ConfigurationType(Text)
		additionalDependencies	->> ItemDefinitionGroup->AdditionalDependencies(Text)
		targetName;				->> PropertyGroup->TargetName(Text)
		projectReference        ->> ProjectReference->Project(Text)
	*/

public:
	ProjectСonf(wstring SlnName, wstring RealName, bool Release, bool Win32, int priority) :
		slnName(SlnName),
		confName(RealName),
		sequence(priority),
		release(Release),
		win32(Win32) {}

public:
	// имена сборки
	wstring slnName;    // минимальное имя сборки по sln
	wstring confName;   // реальное имя сборки
	wstring targetName; // целевое имя сборки

	// ключевые параметры сборки
	wstring сonfigurationType;	  // тип выходного файла
	wstring additionalDependencies; // строка зависимостей
	
	// зависимости
	Swstring weakDepends;   // неявные (слабые)
	Swstring strongDepends; // явные (сильные)
		
	// параметры для сортировки
	size_t release : 1, win32 : 1; // Release = 1 | Debug = 0   Win32 = 1 | x64 = 0
	std::set<int> stepsBuild;      // шаги сборки
	int sequence;                  // последовательность сборки при чтении sln
};


//-------------------------------------------------------------------
// Структура проекта
//-------------------------------------------------------------------
struct Project
{
public:
	Project() {};
	Project(wstring name, wstring path, wstring tpGUID) : prjName(name), prjPath(path), GUIDtype(tpGUID) {};
	
	SharPrjConf Add_Conf(wstring realName, wstring slnName, int priority, ExcConfName& excluds)
	{
		/*
			сборки прочитанные из sln, получают разрядность и платформу согласно sln.
			сборки прочитанные из файла проекта получают разрядность и пратформу по своему имени
		*/

		// платформа
		wstring type;
		bool release;

		// разрядность
		wstring cap;
		bool win32;

		// сборки прочитанные из файла проекта
		if (slnName == L"") slnName = realName;

		// разделение на платформу и разрядность
		size_t poz = slnName.find(L"|") + 1;
		if (poz != std::string::npos)
		{
			// type|cap (Debud|Win32)
			type = slnName.substr(0,poz-1);
			cap = slnName.substr(poz);

			poz = type.find(L"Debug");
			if (poz != std::string::npos)
			{
				type = L"Debug";
				release = false;
			}
			poz = type.find(L"Release");
			if (poz != std::string::npos)
			{
				type = L"Release";
				release = true;
			}

			if (cap == L"x64")
			{
				win32 = false;
			}
			else
			{
				cap = L"Win32";
				win32 = true;
			}
		}

		// проверка на исключения
		if (excluds.cap.count(cap)) return NULL;          // по разрядности
		if (excluds.type.count(type)) return NULL;        // по платформе
		if (excluds.typeCap.count(realName)) return NULL; // по имени сборки

		// возврат если уже была такая сборка
		for (auto &conf : configs)
		{
			if (conf->confName == realName && conf->slnName == (type + L"|" + cap)) return conf;
		}	

		// добавляем если сборка подходит
		if ((type == L"Debug" || type == L"Release") && (cap == L"Win32" || cap == L"x64"))
		{
			configs.push_back(std::make_shared<ProjectСonf>(type + L"|" + cap, realName, release, win32, priority));
			return NULL;
		}

		return NULL;
	}

public:
	// ключевые параметры проекта
	wstring GUIDtype; // GUID типа проекта 
	wstring GUIDprj;  // GUID проекта
	wstring prjName;  // имя проекта
	wstring prjPath;  // путь к файлу проекта

	// сборки и их параметры
	VsharPrjConf configs; // сборки проекта

	// проекты от которых зависим
	Swstring prjReference; // зависимости между проектами полученные из ProjectReference
	map<wstring, SharPrj> allDepends; // все связанные проекты (GUID проекта <-> ссылка на проект)
};


//-------------------------------------------------------------------
// Структура sln-проекта (промежуточная)
//-------------------------------------------------------------------
struct SlnProject
{
public:
	SlnProject(SharPrj Prj) : prj(Prj) {};
	bool Add_Conf(wstring realName, wstring slnName, int priority, ExcConfName& excluds , int& sumConf)
	{
		size_t confSize = prj->configs.size(); // сохранение числа сборок до добавления
		
		std::shared_ptr<ProjectСonf> findConf = prj->Add_Conf(realName, slnName, priority, excluds);
		
		// если стало больше, значит сборка добавилась
		if (confSize < prj->configs.size())
		{
			configs.push_back(prj->configs.back());  
			sumConf++;
			return true;
		}

		// уже была добавлена
		else if (findConf)
		{
			configs.push_back(findConf); 
			return true;
		}

		return false;
	}

public:
	SharPrj prj; // ссылка на проект
	VsharPrjConf configs; // сборки проекта

	// проекты от которых зависим
	Swstring possibleDepends; // потенциальные зависимости между проектами полученные при чтении sln
	SSharSlnPrj weakDepends; // потенциальные слабые зависимости с проектами в этом sln
};

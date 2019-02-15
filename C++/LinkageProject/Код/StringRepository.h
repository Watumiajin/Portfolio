
#pragma once

#define APP_LOCALE "RUSSIAN"

// GUID типов проектов
#define GUIDVSXPROJ L"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}" // guid с++ проекта 
#define GUIDCSPROJ  L"{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}" // guid с# проекта

// сепараторы
#define SEPCONF  L"='"   // сепараторы для определения имени (тип и разрядность) сборки в файле проекта
#define SEPTOKEN L"$()'" // сепараторы для определения имени (target name) сборки в файле проекта
#define SEPDEP   L";"    // сепараторы для определения зависимостей сборки в файле проекта

//-------------------------------------------------------------------
// файл конфигурации
//-------------------------------------------------------------------

// узлы 
#define NOD_VB  L"Verbosity"  // узел: режим вывода
#define NOD_OUP L"Output"     // узел: выходной файл
#define NOD_CF  L"Config"     // узел: сборки проектов для включения в сеть
#define NOD_EXC L"Exclusion"  // узел: исключение
#define NOD_DEP L"Dependence" // узел: зависимость
#define NOD_PH  L"Path"		  // узел: путь к директории для поиска

// атрибуты
#define ATT_TP  L"Type" // атрибут: тип исключения
#define ATT_PH  L"Path" // атрибут: путь к файлу sln

// типы зависимостей
#define DEP_ATT_GL  L"Global"    // тип зависимости: глобальная, для всех проектов
#define DEP_ATT_LP  L"LocalPrj"  // тип зависимости: локальная, между проектами
#define DEP_ATT_LC  L"LocalConf" // тип зависимости: локальная, между сборками

// типы исключений
#define EXC_TPGD  L"TypeGuid"    // тип исключения: по типу guid
#define EXC_PRJGD L"ProjectGuid" // тип исключения: по guid проекта
#define EXC_PRJPH L"ProjectPath" // тип исключения: по пути проекта
#define EXC_PRJNM L"ProjectName" // тип исключения: по имени проекта
#define EXC_СFNM  L"ConfName"    // тип исключения: по имени сборки

//-------------------------------------------------------------------
// файл проекта
//-------------------------------------------------------------------

// узлы 
#define NOD_PRPGROUP   L"PropertyGroup"          // узел: группа свойств
#define NOD_ITDEFGROUP L"ItemDefinitionGroup"    // узел: группа определений
#define NOD_GUID       L"ProjectGuid"            // узел: guid проекта
#define NOD_TARGNAME   L"TargetName"             // узел: target name сборки проекта
#define NOD_PRJ        L"Project"                // узел: guid связанного проекта
#define NOD_PRJCONF    L"ProjectConfiguration"   // узел: сборка проекта
#define NOD_PRJREF     L"ProjectReference"       // узел: дополнительная зависимость
#define NOD_CONFTYPE   L"ConfigurationType"      // узел: расширение сборки
#define NOD_ADDITDEP   L"AdditionalDependencies" // узел: зависимости проекта

// атрибуты
#define ATT_INGL L"Include"       // атрибут: подключение
#define ATT_COND L"Condition"     // атрибут: имя сборки (тип и разрядность)
#define ATT_LAB  L"Label"         // атрибут: тип узла

// типы узлов
#define NOD_ATT_GLOB L"Globals"       // тип узла: глобальные параметры и свойства проекта
#define NOD_ATT_CONF L"Configuration" // тип узла: параметры и свойства сборки

//-------------------------------------------------------------------
// Генерация файла sln 
//-------------------------------------------------------------------

// декларация проекта
#define STR_DECLARATION LR"___(
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio 2015
VisualStudioVersion = 15.0.27130.2027
MinimumVisualStudioVersion = 10.0.40219.1)___"

// узел проекта
#define STR_GUIDTYPEPRJ LR"___(
Project()___"

// секция связей проекта
#define STR_PRJSECPRJDEP  LR"___(
	ProjectSection(ProjectDependencies) = postProject)___"

// конец секции
#define STR_ENDPRJSEC LR"___(
	EndProjectSection)___"

// конец узла проекта
#define STR_ENDPRJ LR"___(
EndProject)___"

// глобальный узел
#define STR_CLOBAL LR"___(
Global)___"

// секция сборок итогового проекта
#define STR_CLBSECSOLCONFPLF LR"___(
	GlobalSection(SolutionConfigurationPlatforms) = preSolution)___"

// секция зависимостей между сборками проектов и сборками итогового проекта
#define STR_CLBSECPRJCONFPLF LR"___(
	GlobalSection(ProjectConfigurationPlatforms) = postSolution)___"

// секция свойств проекта
#define STR_CLBSECSOLPROP LR"___(
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE)___"

// конец секции
#define STR_ENDCLOBSEC LR"___(
	EndGlobalSection)___"

// конец глобального узла
#define STR_ENDCLOBAL LR"___(
EndGlobal)___"
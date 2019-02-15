
#pragma once

#define APP_LOCALE "RUSSIAN"

// GUID ����� ��������
#define GUIDVSXPROJ L"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}" // guid �++ ������� 
#define GUIDCSPROJ  L"{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}" // guid �# �������

// ����������
#define SEPCONF  L"='"   // ���������� ��� ����������� ����� (��� � �����������) ������ � ����� �������
#define SEPTOKEN L"$()'" // ���������� ��� ����������� ����� (target name) ������ � ����� �������
#define SEPDEP   L";"    // ���������� ��� ����������� ������������ ������ � ����� �������

//-------------------------------------------------------------------
// ���� ������������
//-------------------------------------------------------------------

// ���� 
#define NOD_VB  L"Verbosity"  // ����: ����� ������
#define NOD_OUP L"Output"     // ����: �������� ����
#define NOD_CF  L"Config"     // ����: ������ �������� ��� ��������� � ����
#define NOD_EXC L"Exclusion"  // ����: ����������
#define NOD_DEP L"Dependence" // ����: �����������
#define NOD_PH  L"Path"		  // ����: ���� � ���������� ��� ������

// ��������
#define ATT_TP  L"Type" // �������: ��� ����������
#define ATT_PH  L"Path" // �������: ���� � ����� sln

// ���� ������������
#define DEP_ATT_GL  L"Global"    // ��� �����������: ����������, ��� ���� ��������
#define DEP_ATT_LP  L"LocalPrj"  // ��� �����������: ���������, ����� ���������
#define DEP_ATT_LC  L"LocalConf" // ��� �����������: ���������, ����� ��������

// ���� ����������
#define EXC_TPGD  L"TypeGuid"    // ��� ����������: �� ���� guid
#define EXC_PRJGD L"ProjectGuid" // ��� ����������: �� guid �������
#define EXC_PRJPH L"ProjectPath" // ��� ����������: �� ���� �������
#define EXC_PRJNM L"ProjectName" // ��� ����������: �� ����� �������
#define EXC_�FNM  L"ConfName"    // ��� ����������: �� ����� ������

//-------------------------------------------------------------------
// ���� �������
//-------------------------------------------------------------------

// ���� 
#define NOD_PRPGROUP   L"PropertyGroup"          // ����: ������ �������
#define NOD_ITDEFGROUP L"ItemDefinitionGroup"    // ����: ������ �����������
#define NOD_GUID       L"ProjectGuid"            // ����: guid �������
#define NOD_TARGNAME   L"TargetName"             // ����: target name ������ �������
#define NOD_PRJ        L"Project"                // ����: guid ���������� �������
#define NOD_PRJCONF    L"ProjectConfiguration"   // ����: ������ �������
#define NOD_PRJREF     L"ProjectReference"       // ����: �������������� �����������
#define NOD_CONFTYPE   L"ConfigurationType"      // ����: ���������� ������
#define NOD_ADDITDEP   L"AdditionalDependencies" // ����: ����������� �������

// ��������
#define ATT_INGL L"Include"       // �������: �����������
#define ATT_COND L"Condition"     // �������: ��� ������ (��� � �����������)
#define ATT_LAB  L"Label"         // �������: ��� ����

// ���� �����
#define NOD_ATT_GLOB L"Globals"       // ��� ����: ���������� ��������� � �������� �������
#define NOD_ATT_CONF L"Configuration" // ��� ����: ��������� � �������� ������

//-------------------------------------------------------------------
// ��������� ����� sln 
//-------------------------------------------------------------------

// ���������� �������
#define STR_DECLARATION LR"___(
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio 2015
VisualStudioVersion = 15.0.27130.2027
MinimumVisualStudioVersion = 10.0.40219.1)___"

// ���� �������
#define STR_GUIDTYPEPRJ LR"___(
Project()___"

// ������ ������ �������
#define STR_PRJSECPRJDEP  LR"___(
	ProjectSection(ProjectDependencies) = postProject)___"

// ����� ������
#define STR_ENDPRJSEC LR"___(
	EndProjectSection)___"

// ����� ���� �������
#define STR_ENDPRJ LR"___(
EndProject)___"

// ���������� ����
#define STR_CLOBAL LR"___(
Global)___"

// ������ ������ ��������� �������
#define STR_CLBSECSOLCONFPLF LR"___(
	GlobalSection(SolutionConfigurationPlatforms) = preSolution)___"

// ������ ������������ ����� �������� �������� � �������� ��������� �������
#define STR_CLBSECPRJCONFPLF LR"___(
	GlobalSection(ProjectConfigurationPlatforms) = postSolution)___"

// ������ ������� �������
#define STR_CLBSECSOLPROP LR"___(
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE)___"

// ����� ������
#define STR_ENDCLOBSEC LR"___(
	EndGlobalSection)___"

// ����� ����������� ����
#define STR_ENDCLOBAL LR"___(
EndGlobal)___"
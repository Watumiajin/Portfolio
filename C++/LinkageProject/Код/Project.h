
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// ��������� ������
//-------------------------------------------------------------------
struct Project�onf
{
	/*
		������������ ���������� � ����� ����� vcxproj

		�onfName;				->> ProjectConfiguration(label = Include)
		�onfigurationType;		->> PropertyGroup->ConfigurationType(Text)
		additionalDependencies	->> ItemDefinitionGroup->AdditionalDependencies(Text)
		targetName;				->> PropertyGroup->TargetName(Text)
		projectReference        ->> ProjectReference->Project(Text)
	*/

public:
	Project�onf(wstring SlnName, wstring RealName, bool Release, bool Win32, int priority) :
		slnName(SlnName),
		confName(RealName),
		sequence(priority),
		release(Release),
		win32(Win32) {}

public:
	// ����� ������
	wstring slnName;    // ����������� ��� ������ �� sln
	wstring confName;   // �������� ��� ������
	wstring targetName; // ������� ��� ������

	// �������� ��������� ������
	wstring �onfigurationType;	  // ��� ��������� �����
	wstring additionalDependencies; // ������ ������������
	
	// �����������
	Swstring weakDepends;   // ������� (������)
	Swstring strongDepends; // ����� (�������)
		
	// ��������� ��� ����������
	size_t release : 1, win32 : 1; // Release = 1 | Debug = 0   Win32 = 1 | x64 = 0
	std::set<int> stepsBuild;      // ���� ������
	int sequence;                  // ������������������ ������ ��� ������ sln
};


//-------------------------------------------------------------------
// ��������� �������
//-------------------------------------------------------------------
struct Project
{
public:
	Project() {};
	Project(wstring name, wstring path, wstring tpGUID) : prjName(name), prjPath(path), GUIDtype(tpGUID) {};
	
	SharPrjConf Add_Conf(wstring realName, wstring slnName, int priority, ExcConfName& excluds)
	{
		/*
			������ ����������� �� sln, �������� ����������� � ��������� �������� sln.
			������ ����������� �� ����� ������� �������� ����������� � ��������� �� ������ �����
		*/

		// ���������
		wstring type;
		bool release;

		// �����������
		wstring cap;
		bool win32;

		// ������ ����������� �� ����� �������
		if (slnName == L"") slnName = realName;

		// ���������� �� ��������� � �����������
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

		// �������� �� ����������
		if (excluds.cap.count(cap)) return NULL;          // �� �����������
		if (excluds.type.count(type)) return NULL;        // �� ���������
		if (excluds.typeCap.count(realName)) return NULL; // �� ����� ������

		// ������� ���� ��� ���� ����� ������
		for (auto &conf : configs)
		{
			if (conf->confName == realName && conf->slnName == (type + L"|" + cap)) return conf;
		}	

		// ��������� ���� ������ ��������
		if ((type == L"Debug" || type == L"Release") && (cap == L"Win32" || cap == L"x64"))
		{
			configs.push_back(std::make_shared<Project�onf>(type + L"|" + cap, realName, release, win32, priority));
			return NULL;
		}

		return NULL;
	}

public:
	// �������� ��������� �������
	wstring GUIDtype; // GUID ���� ������� 
	wstring GUIDprj;  // GUID �������
	wstring prjName;  // ��� �������
	wstring prjPath;  // ���� � ����� �������

	// ������ � �� ���������
	VsharPrjConf configs; // ������ �������

	// ������� �� ������� �������
	Swstring prjReference; // ����������� ����� ��������� ���������� �� ProjectReference
	map<wstring, SharPrj> allDepends; // ��� ��������� ������� (GUID ������� <-> ������ �� ������)
};


//-------------------------------------------------------------------
// ��������� sln-������� (�������������)
//-------------------------------------------------------------------
struct SlnProject
{
public:
	SlnProject(SharPrj Prj) : prj(Prj) {};
	bool Add_Conf(wstring realName, wstring slnName, int priority, ExcConfName& excluds , int& sumConf)
	{
		size_t confSize = prj->configs.size(); // ���������� ����� ������ �� ����������
		
		std::shared_ptr<Project�onf> findConf = prj->Add_Conf(realName, slnName, priority, excluds);
		
		// ���� ����� ������, ������ ������ ����������
		if (confSize < prj->configs.size())
		{
			configs.push_back(prj->configs.back());  
			sumConf++;
			return true;
		}

		// ��� ���� ���������
		else if (findConf)
		{
			configs.push_back(findConf); 
			return true;
		}

		return false;
	}

public:
	SharPrj prj; // ������ �� ������
	VsharPrjConf configs; // ������ �������

	// ������� �� ������� �������
	Swstring possibleDepends; // ������������� ����������� ����� ��������� ���������� ��� ������ sln
	SSharSlnPrj weakDepends; // ������������� ������ ����������� � ��������� � ���� sln
};

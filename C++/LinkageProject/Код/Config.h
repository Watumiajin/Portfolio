#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// ��������� � ����� ������������
//-------------------------------------------------------------------
struct Depend 
{
	wstring prj;  // GUID ������� 
	wstring conf; // �������� ��� � �������
};

//-------------------------------------------------------------------
// ��������� � ��������������� �������������
//-------------------------------------------------------------------
struct Dependencies
{
	Swstring global; // ��� ���� ��������
	map<wstring, Swstring> localPrj;  // ����� ��������� 
	map<wstring, Vdepend> localConf; // ����� ��������

	// wstring �� ���� ������� ������ GUID �������
};

//-------------------------------------------------------------------
// ��������� � ������������ �� ����� ������ �������
//-------------------------------------------------------------------
struct ExcConfName
{
	Swstring type;    // ���������
	Swstring cap;     // �����������
	Swstring typeCap; // ���������|�����������
};

//-------------------------------------------------------------------
// ��������� � ������������
//-------------------------------------------------------------------
struct Excluds
{
	Swstring typeGUID;    // ���������� �� ���� GUID
	Swstring prjGUID;     // ���������� �� GUID �������
	Swstring prjPath;     // ���������� �� ���� �������
	Swstring prjName;     // ���������� �� ����� �������
	ExcConfName confName; // ���������� �� ����� ������
};


//-------------------------------------------------------------------
// ��������� c ������������ ����������� ����� ������������
//-------------------------------------------------------------------
struct Config
{
	Config() : verb(true) {};

	bool verb; // ����� ������

	wstring confPath;  // ���� � ����� ������������
	wstring outPath;   // ���� ������
	Excluds excluds; // ����������
	Dependencies depends; // �����������

	Swstring searchPaths; // ���������� ������ vxcprj ������
	VsharSln slnFiles;  // sln �����
};

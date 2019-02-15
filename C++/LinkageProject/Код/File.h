
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// ��������� ����� �������
//-------------------------------------------------------------------
struct PrjFile
{
	PrjFile(wstring Path, wstring Name, wstring GUID) : path(Path), name(Name), typeGUID(GUID) {};

	wstring path;     // ����
	wstring name;     // ���
	wstring typeGUID; // ��� �������
};


//-------------------------------------------------------------------
// ��������� ����� sln
//-------------------------------------------------------------------
struct SlnFile
{
	wstring path; // ����
	map<wstring, int> targetConf; // ��� ������ <-> ���������
	Swstring allConf; // ��� ������ ��� ����������

};


#pragma once
#include "stdafx.h"

//-------------------------------------------------------------------
// ����� ���������� ��������� ������ ������
//-------------------------------------------------------------------
class FileFinder
{
public:
	// ����������� ����� ������ ������
	void Find_Files(LPCWSTR root, Excluds& exc);

public:
	StkPrjFile prjFiles; // ��������� �����

private:
	size_t Cut_Ext(const wstring st);

};


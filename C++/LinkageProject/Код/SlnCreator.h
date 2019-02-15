
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// ����� ����������� �������� �������� ����� sln
//-------------------------------------------------------------------
class SlnCreator
{
public:
	SlnCreator(NetControl& NetCont) : netCont(& NetCont) {};
	void Create_File(wstring& mainSlnPath);

private:
	// �������� �������� � ������ ����� ���������
	void Insert_Projects(wstring& mainSlnPath);
	// �������� ������������ �������
	void Insert_Sln_confs();
	// �������� ������ ����� �������������� ������� � ��������
	void Insert_Prj_confs();
	
	wstring Revision_Path(wstring path, wstring mainSlnPath);
	void Add_Buf(wstring buf, wstring type, wstring confName, bool build);

private:
	// ������
	NetControl *netCont; // ����

	// ����� ������
	WriteStream fout;

	// ������
	Qwstring debugBuf;   // ������ debug
	Qwstring releaseBuf; // ������ release
};
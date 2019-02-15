
#pragma once
#include "stdafx.h"

//-------------------------------------------------------------------
// ����� ��� ������� ��������� ������
//-------------------------------------------------------------------
class CommandLine
{
public:
	CommandLine();
	bool Test_Arg();

public:
	// ��������� ��������� ������
	bool readOp; // ����� ������
	bool outOp;  // ����� ������
	Sline path;  // ���� ������
	Sline o;     // ���� ������
	Sline sln;   // �������� ������ � ���. ���.

	//
	VsharSlnFiles slnFiles; // ����� sln ������� ����� ��������

private:
	bool Sln_Parsing();
	size_t Conf_Identification(std::shared_ptr<SlnFile> &selectFile, Sline &bufConf);

};
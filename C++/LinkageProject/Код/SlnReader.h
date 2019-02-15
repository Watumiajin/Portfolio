
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// ����� ��� ������ ������ sln
//-------------------------------------------------------------------
class SlnReader
{
public:
	SlnReader(NetControl& NetCont, XmlReader& Xreader) :
		netCont(& NetCont), xreader(& Xreader) {};
	~SlnReader();
	void Read_SlnFiles(VsharSln& slnFiles, int& allPrj, int& allConf);

private:
	void Project_Section(wstring slnPath, SharSlnPrj& selectPrj, size_t poz, int& sumPrj);
	void DepConf_Section(SharSlnPrj& selectPrj, size_t poz, int& sumConf);
	void  Global_Section(size_t poz);
	void     End_Section(SharSlnPrj& selectPrj, size_t poz);

	wstring Revision_Path(wstring path);
	// ������ ��������� ������ ����� ���������
	void Indef_Possible_Depend();
	// ������ ������ ������
	void Indef_Weak_Depend();

private:
	// ������
	NetControl *netCont; // ����
	XmlReader  *xreader; // ������ vcxproj

	// ������ ������
	wchar_t *bufFile; // ����� ������ �� �����
	wstring bufVal;     // ����� ��������� �� ������
	wstring path;       // ���� ��������� �����
	
	// ����� ���������
	bool addDep = false;  // ���������� �����������
	bool addConf = false; // ���������� ������

	//
	ReadStream readFile; // ����� ������ �����
	map<wstring, int> targetConf; // c����� ������� ���������� �������� � �� ���������
	Swstring confNotExists; // �� ��������� ������ � sln

	map<wstring, SharSlnPrj> slnProjects; // ������� � ������ � �������� sln

};

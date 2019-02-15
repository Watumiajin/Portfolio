
#pragma once
#include "stdafx.h"


//-------------------------------------------------------------------
// ����� ��� ������ ������ xml
//-------------------------------------------------------------------
class XmlReader
{
public:
	XmlReader(NetControl& NetCont) :
		netCont(& NetCont), pFileStream(NULL), pReader(NULL) {}
	~XmlReader();

	// ������ ������ vcxprj
	void Read_PrjFiles(StkPrjFile& prjFiles);

	// ������ ����� ������������
	bool Read_Config();

public:
	//std::set<Sline> completePath;

private:
	void Get_Next(LPCWSTR& localName, XmlNodeType& nodeType, LPCWSTR& nodeValue);

	void Read_Attributes(LPCWSTR wanted, wstring& bufval);
	bool Read_Values(LPCWSTR node, wstring& bufval);

	bool Test_Arg();
	wstring Minimaz_ConfName(wstring name);

	void Filling_Project();

private:
	// ������ � ���������
	NetControl *netCont; // ����

	// ������ �����
	IStream *pFileStream; // ����� ������ �����
	IXmlReader *pReader;  // ����������� ����
	wstring path;           // ���� ��������� ����

	// �������� �������
	SharPrj selectProject; // ����������� ������
	SharPrjConf bufconf;   // ������ � ����������� �������

};

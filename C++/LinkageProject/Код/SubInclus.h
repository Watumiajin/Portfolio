
#pragma once

// ������
#define DISPOUT std::wcout // �������� �����
#define DISPIN  std::wcin  // ������� �����
#define DISPEND L"\n"      // ����� ������

// �����
#define TO_SEE // ��������� ����������
#define TO_UPD // ����������
#define TO_CHK // ������������ ������ ��� �������� ��� ������ � �������

// ���������� ����������
#define BUFSIZE 4096 // ������ ������
#define NETCOUNT 4   // ����� �����

// ���� ������
#define CONFDEBUG false  // ��� ������ debug
#define CONFREALESE true // ��� ������ release

// ����������� ������
#define CONFX64 false    // ����������� ������ x64
#define CONFWIN84 true   // ����������� ������ win84

// ��������������� ���������� ��������� ��������
using std::wstring;

struct SlnFile;
struct PrjFile;
struct Project;
struct SlnProject;
struct Project�onf;
struct Depend;
struct Vertex;

// ��������������� �������

typedef std::wofstream					WriteStream;
typedef std::wifstream					ReadStream;

// ��������������� ������ � ����������

typedef std::shared_ptr<SlnFile>		SharSln;
typedef std::shared_ptr<Project>		SharPrj;
typedef std::shared_ptr<SlnProject>		SharSlnPrj;
typedef std::shared_ptr<Project�onf>	SharPrjConf;
typedef std::shared_ptr<Vertex>			SharVertex;

// ��������������� ��������

typedef std::vector<SharPrj>			VsharPrj;
typedef std::vector<SharPrjConf>		VsharPrjConf;
typedef std::vector<SharSln>			VsharSln;
typedef std::vector<Depend>				Vdepend;
typedef std::stack<PrjFile>				StkPrjFile;
typedef std::set<wstring>				Swstring;
typedef std::set<SharPrj>				SSharPrj;
typedef std::set<SharSlnPrj>			SSharSlnPrj;
typedef std::set<SharVertex>			SSharVertex;
typedef std::queue<wstring>				Qwstring;

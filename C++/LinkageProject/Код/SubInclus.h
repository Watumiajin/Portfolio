
#pragma once

// потоки
#define DISPOUT std::wcout // выходной поток
#define DISPIN  std::wcin  // входной поток
#define DISPEND L"\n"      // конец строки

// метки
#define TO_SEE // подробнее посмотреть
#define TO_UPD // доработать
#define TO_CHK // используется только для контроля или вывода в консоль

// глобальные переменные
#define BUFSIZE 4096 // размер буфера
#define NETCOUNT 4   // число сетей

// типы сборок
#define CONFDEBUG false  // тип сборки debug
#define CONFREALESE true // тип сборки release

// разрядности сборок
#define CONFX64 false    // разрядность сборки x64
#define CONFWIN84 true   // разрядность сборки win84

// предварительное обьявление некоторых структур
using std::wstring;

struct SlnFile;
struct PrjFile;
struct Project;
struct SlnProject;
struct ProjectСonf;
struct Depend;
struct Vertex;

// переопределение потоков

typedef std::wofstream					WriteStream;
typedef std::wifstream					ReadStream;

// переопределение ссылок и указателей

typedef std::shared_ptr<SlnFile>		SharSln;
typedef std::shared_ptr<Project>		SharPrj;
typedef std::shared_ptr<SlnProject>		SharSlnPrj;
typedef std::shared_ptr<ProjectСonf>	SharPrjConf;
typedef std::shared_ptr<Vertex>			SharVertex;

// переопределение массивов

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
